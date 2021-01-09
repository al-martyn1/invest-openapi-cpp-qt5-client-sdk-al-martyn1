/*! \file
    \brief 
 */

#pragma once

#include <QString>
#include <string>

#include  <cwctype>
#include  <cctype>

// std::wint_t towlower( std::wint_t ch );
// int tolower( int ch );
// toupper
// towupper

// int iswlower( std::wint_t ch );
// int islower( int ch );
// int iswupper( std::wint_t ch );


namespace cpp
{


enum class NameStyle
{
    unknownStyle        ,
    invalidName         ,
    cppStyle            ,
    camelStyle          ,
    pascalStyle         ,
    cppCamelMixedStyle  ,
    cppPascalMixedStyle ,
    defineStyle

};

enum NameCharClass
{
    unknown      ,
    digit        ,
    lower        ,
    upper        ,
    underscore
};



inline std::string toString( NameStyle ns )
{
    switch(ns)
    {
        case NameStyle::invalidName         : return "InvalidName";
        case NameStyle::cppStyle            : return "CppStyle";
        case NameStyle::camelStyle          : return "CamelStyle";
        case NameStyle::pascalStyle         : return "PascalStyle";
        case NameStyle::cppCamelMixedStyle  : return "CppCamelMixedStyle";
        case NameStyle::cppPascalMixedStyle : return "CppPascalMixedStyle";
        case NameStyle::defineStyle         : return "DefineStyle";
        default                             : return "UnknownStyle";
    }
}

inline std::set<NameStyle> makeAllNameStyles()
{
    std::set<NameStyle> allNameStyles;
    allNameStyles.insert( NameStyle::unknownStyle        );
    allNameStyles.insert( NameStyle::invalidName         );
    allNameStyles.insert( NameStyle::cppStyle            );
    allNameStyles.insert( NameStyle::camelStyle          );
    allNameStyles.insert( NameStyle::pascalStyle         );
    allNameStyles.insert( NameStyle::cppCamelMixedStyle  );
    allNameStyles.insert( NameStyle::cppPascalMixedStyle );
    allNameStyles.insert( NameStyle::defineStyle         );

    return allNameStyles;
}



inline bool    isAlpha( char ch )     { return std::isalpha( ch ) ? true : false; }
inline bool    isLower( char ch )     { return std::islower( ch ) ? true : false; }
inline bool    isUpper( char ch )     { return std::isupper( ch ) ? true : false; }
inline bool    isDigit( char ch )     { return std::isdigit( ch ) ? true : false; }
inline bool    getCase( char ch )     { return isUpper(ch); }
inline char    toLower( char ch )     { return (char)std::tolower(ch); }
inline char    toUpper( char ch )     { return (char)std::toupper(ch); }

inline bool    isAlpha( wchar_t ch )  { return std::iswalpha( ch ) ? true : false; }
inline bool    isLower( wchar_t ch )  { return std::iswlower( ch ) ? true : false; }
inline bool    isUpper( wchar_t ch )  { return std::iswupper( ch ) ? true : false; }
inline bool    isDigit( wchar_t ch )  { return std::iswdigit( ch ) ? true : false; }
inline bool    getCase( wchar_t ch )  { return isUpper(ch); }
inline wchar_t toLower( wchar_t ch )  { return (wchar_t)std::towlower(ch); }
inline wchar_t toUpper( wchar_t ch )  { return (wchar_t)std::towupper(ch); }


template <typename CharT> inline
NameCharClass getNameCharClass( CharT ch, NameCharClass digitCharClass = NameCharClass::digit ) // NameCharClass::lower
{
    if (ch==(CharT)'_')
        return NameCharClass::underscore;

    if (isDigit(ch))
        return digitCharClass;

    if (isAlpha(ch))
    {
        if (isLower(ch))
           return NameCharClass::lower;
        else
           return NameCharClass::upper;
    }

    return NameCharClass::unknown;
}


template< class CharT, class Traits = std::char_traits<CharT>, class Allocator = std::allocator<CharT> >
inline bool isUnderscoreString( const std::basic_string< CharT, Traits, Allocator > &str )
{
    for( auto it = str.begin(); it != str.end(); ++it )
    {
        if (*it!=(CharT)'_')
            return false;
    }

    return true;
}


template< class CharT, class Traits = std::char_traits<CharT>, class Allocator = std::allocator<CharT> >
inline std::basic_string< CharT, Traits, Allocator >
toLower( const std::basic_string< CharT, Traits, Allocator > &str )
{
    std::basic_string< CharT, Traits, Allocator > resStr;
    for( auto it = str.begin(); it != str.end(); ++it )
    {
        resStr.append( 1, toLower(*it) );
    }

    return resStr;
}

template< class CharT, class Traits = std::char_traits<CharT>, class Allocator = std::allocator<CharT> >
inline std::basic_string< CharT, Traits, Allocator >
toUpper( const std::basic_string< CharT, Traits, Allocator > &str )
{
    std::basic_string< CharT, Traits, Allocator > resStr;
    for( auto it = str.begin(); it != str.end(); ++it )
    {
        resStr.append( 1, toUpper(*it) );
    }

    return resStr;
}

template< class CharT, class Traits = std::char_traits<CharT>, class Allocator = std::allocator<CharT> >
inline std::basic_string< CharT, Traits, Allocator >
toPascal( const std::basic_string< CharT, Traits, Allocator > &str )
{
    if (str.empty())
        return str;

    std::basic_string< CharT, Traits, Allocator > resStr = toLower(str);
    resStr[0] = toUpper(resStr[0]);

    return resStr;
}


template< class CharT, class Traits = std::char_traits<CharT>, class Allocator = std::allocator<CharT> >
inline std::basic_string< CharT, Traits, Allocator >
makeExpandString( const std::basic_string< CharT, Traits, Allocator > &str, std::size_t expandToSize, CharT ch )
{
    if (str.size() >= expandToSize)
        return std::basic_string< CharT, Traits, Allocator >();

    size_t expandStringSize = expandToSize - str.size();

    return std::basic_string< CharT, Traits, Allocator >( expandStringSize, ch );
}


template< class CharT, class Traits = std::char_traits<CharT>, class Allocator = std::allocator<CharT> >
inline std::basic_string< CharT, Traits, Allocator >
expandAtFront( const std::basic_string< CharT, Traits, Allocator > &str, std::size_t expandToSize, CharT ch = (CharT)' ')
{
    return makeExpandString(str, expandToSize, ch) + str;
}

template< class CharT, class Traits = std::char_traits<CharT>, class Allocator = std::allocator<CharT> >
inline std::basic_string< CharT, Traits, Allocator >
expandAtBack( const std::basic_string< CharT, Traits, Allocator > &str, std::size_t expandToSize, CharT ch = (CharT)' ')
{
    return str + makeExpandString(str, expandToSize, ch);
}

inline std::string  expandAtFront( const char    * pStr, std::size_t expandToSize, char    ch =  ' ' ) { return expandAtFront( std::string(pStr ), expandToSize, ch ); }
inline std::wstring expandAtFront( const wchar_t * pStr, std::size_t expandToSize, wchar_t ch = L' ' ) { return expandAtFront( std::wstring(pStr), expandToSize, ch ); }

inline std::string  expandAtBack ( const char    * pStr, std::size_t expandToSize, char    ch =  ' ' ) { return expandAtBack ( std::string(pStr ), expandToSize, ch ); }
inline std::wstring expandAtBack ( const wchar_t * pStr, std::size_t expandToSize, wchar_t ch = L' ' ) { return expandAtBack ( std::wstring(pStr), expandToSize, ch ); }


template< class CharT, class Traits = std::char_traits<CharT>, class Allocator = std::allocator<CharT> >
inline std::basic_string< CharT, Traits, Allocator >
makeString( const char* pStr )
{
    std::basic_string< CharT, Traits, Allocator > resStr;
    for(; *pStr; ++pStr)
    {
        resStr.append( 1, (CharT)*pStr );
    }

    return resStr;
}




template< class CharT, class Traits = std::char_traits<CharT>, class Allocator = std::allocator<CharT> >
NameStyle detectNameStyle( const std::basic_string< CharT, Traits, Allocator > &str )
{
    bool isValid       = false;
    bool hasFirstUpper = false;
    bool hasLower      = false;
    bool hasUpper      = false;
    bool hasUnderscore = false;
    bool isFirstAlpha  = false;

    std::size_t charCount  = 0;
    std::size_t alphaCount = 0;

    const unsigned FlagHasLower      = 0x01;
    const unsigned FlagHasUpper      = 0x02;
    const unsigned FlagHasUnderscore = 0x04;
    const unsigned FlagFirstUpper    = 0x08;
    //const unsigned FlagFirstUpper    = 0x10;


    for( auto it = str.begin(); it != str.end(); ++it, ++charCount)
    {
        CharT ch = *it;

        if (isAlpha(ch))
        {
            if (!charCount)
            {
                isValid      = true;
                isFirstAlpha = true;
            }

            if (!alphaCount && isUpper(ch))
                hasFirstUpper = true;

            ++alphaCount;

            if (isLower(ch))
                hasLower = true;

            if (isUpper(ch))
                hasUpper = true;

        }

        if (ch==(CharT)'_')
        {
            if (!charCount)
                isValid = true;

            hasUnderscore = true;
        }
    }

    if (!isValid)
        return NameStyle::invalidName;


    unsigned flags = 0;

    if (hasUnderscore)
        flags |= FlagHasUnderscore;

    if (hasLower)
        flags |= FlagHasLower;

    if (hasUpper)
        flags |= FlagHasUpper;

    if (hasFirstUpper)
    {
        flags |= FlagHasUpper;
        flags |= FlagFirstUpper;
    }

    // if (hasFirstUpper)
    //     flags |= FlagFirstUpper;



    switch(flags)
    {
        case FlagHasLower                   :
        case FlagHasLower|FlagHasUnderscore :
             return NameStyle::cppStyle     ;

        case FlagHasUpper                   :
        case FlagHasUpper|FlagFirstUpper    :
        case FlagHasUpper|FlagHasUnderscore :
        case FlagHasUpper|FlagHasUnderscore|FlagFirstUpper :
             return NameStyle::defineStyle  ;

        case FlagHasLower|FlagHasUpper      :
             return NameStyle::camelStyle   ;

        case FlagHasLower|FlagHasUpper|FlagHasUnderscore :
             return NameStyle::cppCamelMixedStyle        ;

        case FlagFirstUpper|FlagHasLower|FlagHasUpper|FlagFirstUpper    :
             return NameStyle::pascalStyle                              ;

        case FlagFirstUpper|FlagHasLower|FlagHasUpper|FlagHasUnderscore|FlagFirstUpper :
             return NameStyle::cppPascalMixedStyle                                     ;

        default: return NameStyle::unknownStyle;

    }
}



 
inline NameStyle detectNameStyle( const char    * pStr )   { return detectNameStyle( std::string(pStr ) ); }
inline NameStyle detectNameStyle( const wchar_t * pStr )   { return detectNameStyle( std::wstring(pStr) ); }
 

template< class CharT, class Traits = std::char_traits<CharT>, class Allocator = std::allocator<CharT> >
inline std::vector< std::basic_string< CharT, Traits, Allocator > >
splitName( const std::basic_string< CharT, Traits, Allocator > &str )
{
    std::vector< std::basic_string< CharT, Traits, Allocator > > resVec;
    //std::basic_string< CharT, Traits, Allocator > postfixString;

    auto beginIt = str.begin();

    // Find undercsore prefix
    for(; beginIt!=str.end() && *beginIt==(CharT)'_'; ++beginIt ) {}

    if (beginIt!=str.begin())
    {
        resVec.push_back( std::basic_string< CharT, Traits, Allocator >( str.begin(), beginIt ) );
        // prefixString = std::basic_string< CharT, Traits, Allocator >( str.begin(), it );
    }

    // Find undercsore postfix
    auto endIt = str.end();
    auto endItPrev = endIt;
    if (endItPrev!=str.begin())
       --endItPrev;
    
    for(; endIt!=beginIt && *endItPrev==(CharT)'_'; --endIt, --endItPrev ) { }

    auto postfixBegin = str.end();
    auto postfixEnd   = str.end();

    if (endIt!=str.end())
    {
        // resVec.push_back( std::basic_string< CharT, Traits, Allocator >( endIt, str.end() ) );
        //postfixString = std::basic_string< CharT, Traits, Allocator >( postfixBegin, postfixEnd );
        postfixBegin = endIt;
    }


    NameCharClass prevCharClass = NameCharClass::unknown;
    if (beginIt!=endIt)
    {
        prevCharClass = getNameCharClass( *beginIt, NameCharClass::lower );
    }


    auto startIt = beginIt;
    auto it      = beginIt;

    for( ; it!=endIt; ++it)
    {
        auto curCharClass = getNameCharClass( *it, NameCharClass::lower );
        if (prevCharClass!=curCharClass && !(prevCharClass==NameCharClass::upper && curCharClass==NameCharClass::lower) )
        {
            auto curPartStr = std::basic_string< CharT, Traits, Allocator >(startIt, it);
            startIt = it;
            if (!isUnderscoreString(curPartStr))
            {
                resVec.push_back(curPartStr);
            }
        }

        prevCharClass = curCharClass;
    }

    if (startIt != it)
    {
        auto curPartStr = std::basic_string< CharT, Traits, Allocator >(startIt, it);
        if (!isUnderscoreString(curPartStr))
        {
            resVec.push_back(curPartStr);
        }
    }

    if (postfixBegin!=postfixEnd)
        resVec.push_back(std::basic_string< CharT, Traits, Allocator >( postfixBegin, postfixEnd ));


//  NameCharClass getNameCharClass( CharT ch, NameCharClass digitCharClass = NameCharClass::digit ) // NameCharClass::lower



    /*
    resVec.push_back( makeString<CharT, Traits, Allocator>( "__" ) );
    resVec.push_back( makeString<CharT, Traits, Allocator>( "Youuu" ) );
    resVec.push_back( makeString<CharT, Traits, Allocator>( "Man" ) );
    resVec.push_back( makeString<CharT, Traits, Allocator>( "__" ) );
    */

    return resVec;
}

inline std::vector< std::string  > splitName( const char    * pStr ) { return splitName( std::string(pStr ) ); }
inline std::vector< std::wstring > splitName( const wchar_t * pStr ) { return splitName( std::wstring(pStr) ); }


template< class CharT, class Traits = std::char_traits<CharT>, class Allocator = std::allocator<CharT> >
inline std::basic_string< CharT, Traits, Allocator >
formatName( const std::basic_string< CharT, Traits, Allocator > &str, NameStyle nameStyle )
{
    std::vector< std::basic_string< CharT, Traits, Allocator > > nameParts = splitName(str);

    if (nameParts.empty())
        return std::basic_string< CharT, Traits, Allocator >();

    auto beginIt  = nameParts.begin();
    auto endIt    = nameParts.end();

    auto prefixIt = nameParts.end();
    if (isUnderscoreString(*beginIt))
    {
        prefixIt = beginIt;
        ++beginIt;
    }

    auto postfixIt = nameParts.end();
    if (nameParts.size()>1)
    {
        auto tmpIt = postfixIt;
        --tmpIt;
        if (isUnderscoreString(*tmpIt))
        {
            postfixIt = tmpIt;
            endIt     = postfixIt;
        }
    }


    std::basic_string< CharT, Traits, Allocator > strRes;
    bool isFirst = true;
    // make result here

    for(auto it = beginIt; it!=endIt; isFirst = false, ++it )
    {
        if (!strRes.empty())
        {
            if (nameStyle==NameStyle::cppStyle
             || nameStyle==NameStyle::cppCamelMixedStyle
             || nameStyle==NameStyle::cppPascalMixedStyle
             || nameStyle==NameStyle::defineStyle)
                strRes.append(1, (CharT)'_');
        }

        switch(nameStyle)
        {
            case NameStyle::cppStyle            :
                 strRes.append(toLower(*it));
                 break;

            case NameStyle::cppCamelMixedStyle  :
            case NameStyle::camelStyle          :
                 if (isFirst)
                     strRes.append(toLower(*it));
                 else
                     strRes.append(toPascal(*it));
                 break;

            case NameStyle::cppPascalMixedStyle :
            case NameStyle::pascalStyle         :
                 strRes.append(toPascal(*it));
                 break;

            case NameStyle::defineStyle         :
                 strRes.append(toUpper(*it));
                 break;

            default                             :
                 strRes.append(*it);

        
        }
    }


/*
    unknownStyle        ,
    invalidName         ,
    cppStyle            ,
    camelStyle          ,
    pascalStyle         ,
    cppCamelMixedStyle  ,
    cppPascalMixedStyle ,
    defineStyle

*/

    // add prefix
    if (prefixIt!=nameParts.end())
    {
        strRes = *prefixIt + strRes;
    }

    // add postfix
    if (postfixIt!=nameParts.end())
    {
        strRes = strRes + *postfixIt;
    }

    return strRes;
}

inline std::string  formatName( const char    * pStr, NameStyle nameStyle ) { return formatName( std::string(pStr ), nameStyle ); }
inline std::wstring formatName( const wchar_t * pStr, NameStyle nameStyle ) { return formatName( std::wstring(pStr), nameStyle ); }
                                   


} // namespace cpp

