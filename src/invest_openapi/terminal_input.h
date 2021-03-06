#pragma once

#include "umba/umba.h"
#include "umba/debug_helpers.h"


#include <vector>
#include <string>
#include <functional>

#if defined(WIN32) || defined(_WIN32)

    #include <windows.h>

#endif


namespace invest_openapi
{

//----------------------------------------------------------------------------




//----------------------------------------------------------------------------
class SimpleTerminalInput
{

    #if defined(WIN32) || defined(_WIN32)

        HANDLE hStdin;

    #endif

    bool divAsTab = true; // кнопка '/' на цифровой клавиатуре работает как Tab. Сейчас это очень удобное поведение, 
                          // но в каких-то случаях возможно надо бы отключить эту фичу


public:

    SimpleTerminalInput()
    {
        #if defined(WIN32) || defined(_WIN32)

            hStdin = GetStdHandle(STD_INPUT_HANDLE);

        #endif
    }

    void setDivAsTab( bool b ) { divAsTab = b; }
    bool getDivAsTab( ) const  { return divAsTab; }


    //! Non-blocking reading of keys pressed
    std::vector<int> readInput() const
    {
        #if defined(WIN32) || defined(_WIN32)

            // https://docs.microsoft.com/en-us/windows/console/readconsoleinput
            // https://docs.microsoft.com/en-us/windows/console/input-record-str
            // https://docs.microsoft.com/en-us/windows/console/reading-input-buffer-events

            // https://docs.microsoft.com/en-us/windows/console/generateconsolectrlevent

            if (hStdin == INVALID_HANDLE_VALUE)
                return std::vector<int>();

            DWORD fdwSaveOldMode;
            if (!GetConsoleMode(hStdin, &fdwSaveOldMode) )
                return std::vector<int>();

            if (!SetConsoleMode(hStdin, ENABLE_WINDOW_INPUT  /* | ENABLE_MOUSE_INPUT */  /* fdwMode */ ) )
                return std::vector<int>();

            DWORD waitRes = WaitForSingleObject( hStdin, 0 );
            if (waitRes!=WAIT_OBJECT_0)
                return std::vector<int>(); // No events available on console

            INPUT_RECORD irInBuf[32];
            DWORD cNumRead;

            if (!ReadConsoleInput( hStdin, &irInBuf[0], 32, &cNumRead ))
                return std::vector<int>();

            SetConsoleMode(hStdin, fdwSaveOldMode);

            std::vector<int> resVec;

            for(auto i=0u; i!=32u; ++i)
            {
                switch(irInBuf[i].EventType)
                {
                    case KEY_EVENT:
                    {
                        if (irInBuf[i].Event.KeyEvent.bKeyDown)
                        {
                            bool flagCtrlPressed  = ( (irInBuf[i].Event.KeyEvent.dwControlKeyState & (LEFT_CTRL_PRESSED|RIGHT_CTRL_PRESSED)) != 0 );
                            bool flagAltPressed   = ( (irInBuf[i].Event.KeyEvent.dwControlKeyState & (LEFT_ALT_PRESSED |RIGHT_ALT_PRESSED )) != 0 );
                            bool flagShiftPressed = ( (irInBuf[i].Event.KeyEvent.dwControlKeyState & (SHIFT_PRESSED)) != 0 );

                            bool flagOnlyCtrlPressed  =  flagCtrlPressed && !flagAltPressed && !flagShiftPressed;
                            bool flagOnlyAltPressed   = !flagCtrlPressed &&  flagAltPressed && !flagShiftPressed;
                            bool flagOnlyShiftPressed = !flagCtrlPressed && !flagAltPressed &&  flagShiftPressed;


                            // Ctrl+C pressed
                            if (irInBuf[i].Event.KeyEvent.wVirtualKeyCode==0x43 && flagOnlyCtrlPressed) 
                            {
                                GenerateConsoleCtrlEvent(CTRL_C_EVENT, 0);
                            }

                            // Shift+Ins - classic mode
                            else if ( (irInBuf[i].Event.KeyEvent.wVirtualKeyCode==VK_INSERT && flagOnlyShiftPressed) 
                            // Ctrl+V - modern mode
                                   || (irInBuf[i].Event.KeyEvent.wVirtualKeyCode==0x56 /* V key */  && flagOnlyCtrlPressed) 
                                    )
                            {
                                // Нужно прочитать буфер обмена в текстовом режиме и скормить его содержимое в результирующий вектор
                                // Tab - заменяем на пробел, никакие другие контрольные символы, кроме Enter 0x0D не пропускаем, и 
                                // пропускаем только то, что <128

                                // https://stackoverflow.com/questions/14762456/getclipboarddatacf-text

                                if (!OpenClipboard(0))
                                    return resVec;

                                HANDLE hData = GetClipboardData(CF_TEXT);
                                if (hData==0)
                                    return resVec;

                                char * pszText = static_cast<char*>( GlobalLock(hData) );

                                if (pszText)
                                {
                                    // Do job here

                                    for(; *pszText; ++pszText)
                                    {
                                        char ch = *pszText;

                                        switch(ch)
                                        {
                                            case 0x09: // автокомплит при вставке из буфера - недопустим, поэтому просто заменяем на пробел
                                                 // тем более, что он скорее всего там и означал пробельный символ
                                                 resVec.insert( resVec.end(), 1, ' ' );
                                                 break;

                                            case 0x0D:
                                                 // При ручном вводе по нажатию Enter вводится только 0x0D, по которому обычно завершается
                                                 // текущая команда.
                                                 // Но текущая команда может быть с ошибкой или не может быть выполнена в данный момент.
                                                 // В этом случае она оставляется в буфере для того, чтобы её можно было подредактировать.
                                                 // Это удобно в интерактивном режиме, и полное гавно, когда вставляется куча данных из буфера.
                                                 // Поэтому сразу после Enter (CR) отсылаем 0x0A (LF) - в интерактивном режиме такого не будет,
                                                 // А если и будет - то ничего не сломается
                                                 // Редактор по получении 0x0A (LF) должен очистить буфер
                                                 resVec.insert( resVec.end(), 1, 0x0D );
                                                 resVec.insert( resVec.end(), 1, 0x0A );
                                                 break;

                                            default:

                                                 if (ch>=' ' || ch<0x7F)
                                                    resVec.insert( resVec.end(), 1, ch );
                                        }
                                    
                                    }
                                }

                                GlobalUnlock( hData );

                                CloseClipboard();

                            }

                            // VK_SPACE в сочетании с Ctrl или Shift - работает как VTab
                            // Используем как альтернативный вариант для автокомплита
                            // Основной вариант - по обычному табу - 0x09 - TAB
                            else if ( irInBuf[i].Event.KeyEvent.wVirtualKeyCode==VK_SPACE /* 0x20 */ 
                                   // && flagAltPressed // Alt+Space - windows native hotkey
                                   && (flagCtrlPressed || flagShiftPressed) // Use Ctrl+Space or Shift+Space instead
                                    ) 
                            {
                                // https://ru.wikipedia.org/wiki/ASCII
                                // 0x0B - VT - vertical tab
                                resVec.insert( resVec.end(), irInBuf[i].Event.KeyEvent.wRepeatCount, 0x0B );
                            }

                            // DEL 0x7F delete - https://ru.wikipedia.org/wiki/ASCII
                            // В строковом редакторе без возможности перемещаться влево-вправо и вставлять символы - ведёт себя как Backspace
                            else if ( irInBuf[i].Event.KeyEvent.wVirtualKeyCode==VK_DELETE)
                            {
                                resVec.insert( resVec.end(), irInBuf[i].Event.KeyEvent.wRepeatCount, 0x7F );
                            }

                            // Стрелка вверх. Генерируем код 0x18 - в Far в ASCII-таблице он отображается как стрелка вверх, поэтому его и задействуем,
                            // а вообще - это CAN 0x18 cancel
                            else if ( irInBuf[i].Event.KeyEvent.wVirtualKeyCode==VK_UP)
                            {
                                resVec.insert( resVec.end(), irInBuf[i].Event.KeyEvent.wRepeatCount, 0x18 ); // VK_UP
                            }

                            // Стрелка вниз. Генерируем код 0x19 - в Far в ASCII-таблице он отображается как стрелка вниз, поэтому его и задействуем,
                            // а вообще - это EM 0x19 end of medium
                            else if ( irInBuf[i].Event.KeyEvent.wVirtualKeyCode==VK_DOWN)
                            {
                                resVec.insert( resVec.end(), irInBuf[i].Event.KeyEvent.wRepeatCount, 0x19 ); // VK_DOWN
                            }

                            // Кнопку '/' на цифровой клавиатуре удобно использовать, как Tab.
                            else if ( irInBuf[i].Event.KeyEvent.wVirtualKeyCode==VK_DIVIDE)
                            {
                                if (divAsTab)
                                    resVec.insert( resVec.end(), irInBuf[i].Event.KeyEvent.wRepeatCount, 0x09 );
                                else
                                    resVec.insert( resVec.end(), irInBuf[i].Event.KeyEvent.wRepeatCount, '/' );
                            }

                            // Ascii chars
                            else if (irInBuf[i].Event.KeyEvent.uChar.AsciiChar!=0)
                            {
                                resVec.insert( resVec.end(), irInBuf[i].Event.KeyEvent.wRepeatCount, irInBuf[i].Event.KeyEvent.uChar.AsciiChar);
                            }


                            #if 0

                                // Пытаюсь поймать тут непонятно откуда возникающие фантомные символы, которые никто не вводил
                               
                                if (!resVec.empty() && (resVec.back()=='{' || resVec.back()=='}' || resVec.back()==';') )
                                {
                                    umba::debugBreak();
                                }

                            #endif


                        } // if (irInBuf[i].Event.KeyEvent.bKeyDown)

                    } // case KEY_EVENT:

                } // switch(irInBuf[i].EventType)

            } // for(auto i=0u; i!=32u; ++i)

            // umba::debugBreak()

            return resVec;

        #else

            return std::vector<int>();

        #endif
    }

}; // class SimpleTerminalInput

//----------------------------------------------------------------------------




//----------------------------------------------------------------------------
//! Базовый класс для простецкого редактирования строки в терминале - никакого курсора тудым-сюдым, только можно стирать Backspace'ом, 
/*! Зато автокомплит искаропки поддерживается
    Но не поддерживается ничего, кроме базового ASCII.
    Зато поддерживается автоматическое преобразование регистра вводимых символов.
    В общем, огранниченный редактор для специфических применений.
 */
class SimpleTerminalLineEditImplBase
{
    std::string    line;
    std::string    autocompletionString;
    char           autocompletionKeyCode   = 0x09; // TAB
    int            caseConvertMode         = 0; // <0 - lower, 0 - keep, >0 - upper
    int            allowLeadingSpacesMode  = false; // Leading spaces not allowed
    int            allowMultipleSpacesMode = false; // Multiple spaces not allowed


    bool           historyMode = false; // Up/Down - листаем историю, включается только когда строка полностью пуста
                                        // Нажатие любой клавиши, кроме Up/Down - отключает режим истории

    std::size_t    historyPos = 0;      // Текущий индекс при пролистывании истории

    std::vector<std::string>  inputHistory;


    //------------------------------
    // Event handlers

    //! \param text can be modified in this event handler
    virtual void onTextModified ( SimpleTerminalLineEditImplBase *pEdit, std::string &text ) { }

    //! Return true if buffer must kept after this call (buffer can be filled with new text by the setText function or by modifying param text)
    virtual bool onEditCompleted( SimpleTerminalLineEditImplBase *pEdit, std::string &text ) { return false; } 

    //! Updates View
    virtual void onUpdateView   ( const SimpleTerminalLineEditImplBase *pEdit, const std::string &text ) { }


    //------------------------------
    // Utilities

    void putToHistory( const std::string &s )
    {
        // Старые элементы "всплывают" при повторном использовании
        std::vector<std::string>::const_iterator it = inputHistory.begin();
        for(; it!=inputHistory.end(); ++it)
        {
            if (s==*it)
            {
                inputHistory.erase(it);
                break; // В нормальной ситуации повторов быть не должно. А если кто напортачил, тот ССЗБ
            }
        }

        inputHistory.push_back(s); // Добавляем в конец - "новейшая история"

    }



public: 


    void updateView()
    {
        onUpdateView(  this, line );
    }

    static char toUpper( char ch )
    {
        if (ch>='a' && ch<='z')
            ch = ch - 'a' + 'A';
        return ch;
    }

    static char toLower( char ch )
    {
        if (ch>='A' && ch<='Z')
            ch = ch - 'A' + 'a';
        return ch;
    }

    char caseConvert( char ch ) const
    {
        if (caseConvertMode>0)  return toUpper(ch);
        if (caseConvertMode<0)  return toLower(ch);
        return ch;
    }

    std::string caseConvert( std::string str ) const
    {
        std::string::size_type i = 0, sz = str.size();
        for(; i!=sz; ++i)
           str[i] = caseConvert(str[i]);
        return str;
    }

    void setCaseConvert( int cc ) { caseConvertMode = cc;   }
    int  getCaseConvert( ) const  { return caseConvertMode; }

    bool getAllowLeadingSpaces(        ) const { return allowLeadingSpacesMode; }
    void setAllowLeadingSpaces( bool a )       { allowLeadingSpacesMode = a; }

    bool getAllowMultipleSpaces(        ) const { return allowMultipleSpacesMode; }
    void setAllowMultipleSpaces( bool a )       { allowMultipleSpacesMode = a; }


    //! Return false if std, true if alter key used
    bool getAutocompletionKeyAlter() const         { return autocompletionKeyCode != 0x09; }
    void setAutocompletionKeyAlter( bool bAlter )  { autocompletionKeyCode = bAlter ? 0x0B : 0x09; }

    bool isAutocompletionKeyChar( char ch ) const  { return ch==autocompletionKeyCode; } 
    bool isBackspaceKeyChar     ( char ch ) const  { return ch==0x08 || ch==0x7F; } // BS 08 backspace or DEL 0x7F delete
    bool isEnterKeyChar         ( char ch ) const  { return ch==0x0D; } // CR 13 
    bool isAsciiKeyChar         ( char ch ) const  { return ch>=' ' && ch<0x7F; } // Basic ASCII


    std::string getText() const { return line; }
    void        setText(  const std::string &t ) { line = t; }
    void        clrText() { line.clear(); }

    // Текст автокомплита, на самом деле. Просто слишком длинно получалось get/set/clr_AutocompletionText
    std::string getAclt() const { return autocompletionString; }
    void        setAclt(  const std::string &t ) { autocompletionString = t; }
    void        clrAclt() { autocompletionString.clear(); }



    void processInput( const std::vector<int> &input )
    {
        for( auto ch : input )
        {
            //if (!isAsciiKeyChar(ch))
            //    continue;


/*
    bool           historyMode = false; // Up/Down - листаем историю, включается только когда строка полностью пуста
                                        // Нажатие любой клавиши, кроме Up/Down - отключает режим истории

    std::size_t    historyPos = 0;      // Текущий индекс при пролистывании истории

    std::vector<std::string>  inputHistory;

    resVec.insert( resVec.end(), irInBuf[i].Event.KeyEvent.wRepeatCount, 0x18 ); // VK_UP
    resVec.insert( resVec.end(), irInBuf[i].Event.KeyEvent.wRepeatCount, 0x19 ); VK_DOWN

    putToHistory( const std::string &s )
*/

            if (ch==0x18 || ch==0x19) // VK_UP or VK_DOWN
            {
                if (!historyMode)
                {
                    if (!line.empty()) // History mode стартует, только если ничего не введено
                        continue;

                    if (inputHistory.empty()) // На пустой истории - не стартуем
                        continue;

                    if (ch==0x18) // VK_UP
                    {
                        historyMode = true; // Размотка истории начинается только клавишей VK_UP
                        historyPos = inputHistory.size(); // Ниже будет декремент, и начнем с последнего элемента вектора
                    }
                    else // VK_DOWN
                    {
                        // ничего не делаем
                    }
                }


                if (!historyMode) // Если не смогли стартовать исторический режим, толькопросто уходим в игнор
                    continue; 


                // Исторический режим активен
                
                if (ch==0x18) // VK_UP
                {
                    if (historyPos!=0) // Если до начала истории не добрались, то - работаем, иначе - сидим на месте
                    {
                        --historyPos;
                        line = inputHistory[historyPos];
                        // autocompletionString.clear(); 
                        onTextModified( this, line );
                        updateView();
                    }
                }
                else // VK_DOWN
                {
                    if (historyPos<inputHistory.size())
                        ++historyPos;

                    if (historyPos>=inputHistory.size())
                    {
                        if (!line.empty())
                        {
                            line.clear();
                            onTextModified( this, line );
                            updateView();
                        }

                        historyMode = false; // Выключаем исторический режим - если будет надо, он опять включится по VK_UP
                    }
                    else // мотаем обратно
                    {
                        line = inputHistory[historyPos];
                        // autocompletionString.clear(); 
                        onTextModified( this, line );
                        updateView();
                    }
                }
                
                continue;

            }


            // Какие-то другие кнопки - сбрасываем historyMode
            historyMode = false;


            if (isEnterKeyChar(ch))
            {
                if (line.empty())
                    continue;

                //auto linePrevStr = line;

                bool bKeep = onEditCompleted( this, line );
                if (!bKeep)
                {
                    putToHistory(line);
                    line.clear();
                }

                autocompletionString.clear();

                // if (linePrevStr!=line)
                // {
                //     onTextModified( this, line );
                // }

                updateView();

                continue;
            }




            if (isBackspaceKeyChar(ch))
            {
                if (!line.empty())
                {
                    line.erase( line.size()-1 );
                    onTextModified( this, line );
                    updateView();
                }
            }
            else if (ch==0x0A) // clear line command (from clipboard linefeed)
            {
                line.clear();
                autocompletionString.clear();
                onTextModified( this, line );
                updateView();
            }
            else if (ch==0x1B) // clear line - Escape pressed
            {
                line.clear();
                autocompletionString.clear();
                onTextModified( this, line );
                updateView();
            }
            else if (isAutocompletionKeyChar(ch))
            {
                if (!autocompletionString.empty())
                {
                    line.append(autocompletionString);
                    autocompletionString.clear();
                    onTextModified( this, line );
                    updateView();
                }
                else
                {
                    if (!line.empty() && line.back()==' ' && !allowMultipleSpacesMode)
                    {
                        // do nothing
                    }
                    else if (!line.empty())
                    {
                        line.append(1, ' ' );
                        onTextModified( this, line );
                        updateView();
                    }
                }
            }
            else if (isAsciiKeyChar(ch))
            {
                // bool isChSpace = (ch==' ');
                // if (allowLeadingSpacesMode)
                //     isChSpace = false; // force not space
                //  
                // // if ( space && empty) doNothing();
                // // if ( !space || !empty) doJob();
                //  
                // if ( ((char)ch)!=' ' && !line.empty())
                // {}
                //  
                // allowLeadingSpacesMode

                if (line.empty() && (ch==' ') && !allowLeadingSpacesMode)
                {
                    // do nothing
                }
                else if (!line.empty() && line.back()==' ' && (ch==' ') && !allowMultipleSpacesMode)
                {
                    // do nothing
                }
                else
                {
                    line.append(1, caseConvert((char)ch) );
                    onTextModified( this, line );
                    updateView();
                }
            }
            else
            {
                // Хрень какая-то пришла, просто игнорим
            }
        
        } // for( auto i : input )

    }

    // //! \param text can be modified in this event handler
    // virtual void onTextModified ( SimpleTerminalLineEdit *pEdit, std::string &text ) { }
    //  
    // //! Return true if buffer must kept after this call (buffer can be filled with new text by the setText function or by modifying param text)
    // virtual bool onEditCompleted( SimpleTerminalLineEdit *pEdit, std::string &text ) { return false; } 


}; // class SimpleTerminalLineEditImplBase

//----------------------------------------------------------------------------




//----------------------------------------------------------------------------
template< typename ModifiedHadler, typename CompleteHandler, typename UpdateViewHadler >
class SimpleTerminalLineEdit : public SimpleTerminalLineEditImplBase
{
    ModifiedHadler        modifiedHadler;
    CompleteHandler       completeHandler;
    UpdateViewHadler      updateViewHadler;

public:

    SimpleTerminalLineEdit( ModifiedHadler mh,  CompleteHandler ch, UpdateViewHadler uvh )
    : SimpleTerminalLineEditImplBase()
    , modifiedHadler(mh)
    , completeHandler(ch)
    , updateViewHadler(uvh)
    {}

protected:

    //! \param text can be modified in this event handler
    virtual void onTextModified ( SimpleTerminalLineEditImplBase *pEdit, std::string &text ) override
    {
        modifiedHadler( pEdit, text );
    }

    //! Return true if buffer must kept after this call (buffer can be filled with new text by the setText function or by modifying param text)
    virtual bool onEditCompleted( SimpleTerminalLineEditImplBase *pEdit, std::string &text ) override
    {
        return completeHandler( pEdit, text );
    }

    virtual void onUpdateView   ( const SimpleTerminalLineEditImplBase *pEdit, const std::string &text ) override
    {
        updateViewHadler( pEdit, text );
    }


}; // class SimpleTerminalLineEdit

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
/*
template< typename ModifiedHadler
        , typename CompleteHandler
        > inline
SimpleTerminalLineEdit<ModifiedHadler, CompleteHandler, ModifiedHadler> 
makeSimpleTerminalLineEdit( ModifiedHadler modifiedHadler, CompleteHandler completeHandler, ModifiedHadler updateViewHadler )
{
    return SimpleTerminalLineEdit<ModifiedHadler, CompleteHandler, ModifiedHadler>(modifiedHadler, completeHandler, updateViewHadler);
}
*/
//----------------------------------------------------------------------------
template< typename ModifiedHadler
        , typename CompleteHandler
        , typename UpdateViewHadler
        > inline
SimpleTerminalLineEdit<ModifiedHadler, CompleteHandler, UpdateViewHadler> 
makeSimpleTerminalLineEdit( ModifiedHadler modifiedHadler, CompleteHandler completeHandler, UpdateViewHadler updateViewHadler )
{
    return SimpleTerminalLineEdit<ModifiedHadler, CompleteHandler, UpdateViewHadler>(modifiedHadler, completeHandler, updateViewHadler);
}

//----------------------------------------------------------------------------




/*
//----------------------------------------------------------------------------
class SimpleTerminalLineEditStdFunctionDriven : public SimpleTerminalLineEditImplBase
{
    ModifiedHadler        modifiedHadler;
    CompleteHandler       completeHandler;

public:

    SimpleTerminalLineEdit( ModifiedHadler mh,  CompleteHandler ch )
    : SimpleTerminalLineEditImplBase()
    , modifiedHadler(mh)
    , completeHandler(ch)
    {}

protected:

    //! \param text can be modified in this event handler
    virtual void onTextModified ( SimpleTerminalLineEditImplBase *pEdit, std::string &text ) override
    {
        modifiedHadler( pEdit, text );
    }

    //! Return true if buffer must kept after this call (buffer can be filled with new text by the setText function or by modifying param text)
    virtual bool onEditCompleted( SimpleTerminalLineEditImplBase *pEdit, std::string &text ) override
    {
        return completeHandler( pEdit, text );
    }


}; // class SimpleTerminalLineEdit
*/


//----------------------------------------------------------------------------


} // namespace invest_openapi

//----------------------------------------------------------------------------
