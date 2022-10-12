#pragma once
#include <cstdint>
#ifdef __ICCARM__
#include <stdarg.h>
#else
#include <cstdarg>
#endif
#include <cstring>

#include "common/compiler/genType.h"
namespace settings
{
class InputArchive;
}

namespace nea
{

/**
Convert character to lower case
@param c			The character
@return				Result
*/
inline char toLowerCase(char c)
{
    return (c >= 'A' && c <= 'Z') ? c + 'a' - 'A' : c;
}

/**
        class BText.

        Base class for all size specific Text class(es).
        Has a public ctor to create a text object to a given buffer.

        Example:

        @code

        size_t format(char * textBuf, size_t bufSize, int lineNumber)
        {
                nea::BText temp(textBuf, bufSize);
                temp.print("line: %d", lineNumber);
                return temp.size();
        }

        @endcode
*/
class BText
{
    BText(const BText&);

public:
    /**
            Unsigned integal type to represent size.
    */
    typedef uint16_t size_type;

    /**
            Special value for not found.
    */
    static const size_type npos = 0xffff;

    /**
            A readom access iterator to char  (convertible to const_iterator).
    */
    typedef char* iterator;

    /**
            A random access iterator to const char.
    */
    typedef const char_t* const_iterator;

    /**
            Create a text object.

            @param initial An optional initializer.
    */
    /**
            Return iterator to beginning.
    */
    iterator begin() { return mpText; }

    /**
            Return iterator to end.
    */
    iterator end() { return mpText + mLen; }

    /**
            Return const_iterator to beginning.
    */
    const_iterator cbegin() const { return mpText; }

    /**
            Return const_iterator to end.
    */
    const_iterator cend() const { return mpText + mLen; }

    /**
            Create a base text.

            @param pText Pointer to a text buffer.
            @param sz Size of text buffer.
    */
    BText(char* pText, size_type sz) : mpText(pText), mSz(sz), mLen(0) {}

    /**
            Create a base text.

            @param pText Pointer to a text buffer.
            @param sz Size of text buffer.
            @param initial An initial text.
    */
    BText(char* pText, size_type sz, const char* initial);

    /**
            Assign content to text.

            @param first An iterator.
            @param last An iterator.
    */
    BText& assign(const_iterator first, const_iterator last);

    /**
            Returns a pointer to an array that contains a null-terminated sequence
            of characters (i.e., a C-string) representing the current value of the
            Text object. This array includes the same sequence of characters that
            make up the value of the string object plus an additional terminating
            null-character ('\0') at the end.

            @return pointer to representation
    */
    const char* c_str() const { return mpText; }

    /**
            Returns the length of the Text, in terms of number of characters. This is
            the number of actual characters that conform the contents of the Text,
            which is not necessarily equal to its storage capacity (max_size).

            @return current length of Text object.
    */
    size_type size() const { return mLen; }

    /**
            Returns the maximum length the Text can reach.

            @return maximum	number of bytes Text object can hold.
    */
    size_type max_size() const { return mSz - 1; }

    /**
            Returns whether the Text is empty (i.e. whether its length is 0). This function
            does not modify the value of the Text in any way. To clear the content of
            a string, see Text::clear.

            @return bool @li true - Text is empty.
                                     @li false - Text contains data.
    */
    bool empty() const { return mLen == 0; }

    /**
            Returns whether the Text is foll (i.e. whether its length is max_size()). This function
            does not modify the value of the Text in any way.

            @return bool @li true - Text is full.
                                     @li false - Text can hold more characters.
    */
    bool full() const { return mLen == max_size(); }

    /**
            Erases the contents of the Text, which becomes an empty Text (with a length of
            0 characters).
    */
    void clear();

    /**
            Compares a value with the BText.
            @param rhs A text.
            @return A value indicating wether the two texts are bitwise equal.
    */
    bool operator==(const BText& rhs) { return compare(rhs.c_str()) == 0; }

    /**
            Compares a value with the string.
            @param rhs A text.
            @return A value indicating wether the two texts are bitwise equal.
    */
    bool operator==(const char* rhs) { return compare(rhs) == 0; }

    /**
            Compares a value with the BText.
            @param rhs A text.
            @return A value indicating wether the two texts are bitwise inequal.
    */
    bool operator!=(const BText& rhs) { return !(*this == rhs); }

    /**
            Compares a value with the string.
            @param rhs A text.
            @return A value indicating wether the two texts are bitwise inequal.
    */
    bool operator!=(const char* rhs) { return !(*this == rhs); }

    /**
            Assigns a new value to the BText, replacing its current contents.

            @param rhs A text.

            @return reference to the changed Text.
    */
    BText& operator=(const BText& rhs) { return BText::operator=(rhs.c_str()); }

    /**
            Assigns a new value to the Text, replacing its current contents.

            @param str A c-string.

            @return reference to the changed Text.
    */
    BText& operator=(const char* str);

    /**
            Assigns a new value to the Text, replacing its current contents.

            @param ch A character.

            @return reference to the changed Text.
    */
    BText& operator=(char ch);

    /**
            Extends the Text by appending additional characters at the end of
            its current value. Only characters upto max_size are appended.

            @param rhs A text.

            @return reference to the changed Text.
    */
    BText& operator+=(const BText& rhs) { return BText::operator+=(rhs.c_str()); }

    /**
            Extends the Text by appending additional characters at the end of
            its current value. Only characters upto max_size are appended.

            @param str A c-string.

            @return reference to the changed Text.
    */
    BText& operator+=(const char* str);

    /**
            Extends the Text by appending additional characters at the end of
            its current value. Only characters upto max_size are appended.

            @param ch A character.

            @return reference to the changed Text.
    */
    BText& operator+=(char ch);

    /**
            Erase characters from string.

            @param pos A position.
            @param len Number of character to erase.

            @return changed text.
    */
    BText& erase(size_type pos = 0, size_type len = 0);

    /**
            Inserts the given character to the given Text position.
            The right hand side of the Text starting from the the insertion point is shifted right
       by one character.
            If the Text is full the rightmost character is dropped out. If the given position does
       not fall within the Text,
            nothing is inserted. For example, if the Text is empty, a character can be inserted only
       to position zero.

            @param pos A position.
            @param ch A character.

            @return changed text.
    */
    BText& insert(size_type pos, char ch);
    BText& insert(size_type pos, const char* str);
    BText& insert(size_type pos, const BText& txt);
    BText& insert(size_type pos, const char* first, const char* last);

    /**
            Find a character.

            @param ch A character.
            @param pos A position in text.

            @return index of character in text when successful, or BText::npos if fail.
    */
    size_type find(char ch, size_type pos = 0) const;

    /**
            Find a string

            @param ch A character.
            @param pos A position in text.

            @return index of character in text when successful, or BText::npos if fail.
    */
    size_type find(const char* str, size_type pos = 0) const;

    /**
            Compare with other text.

            @param text A text.

            @return int @li <0 : text is smaller than other text.
                                    @li 0 :  text is equal to other text.
                                    @li > 0 : text is greater than other text.
    */
    int compare(const BText& text) const { return std::strcmp(c_str(), text.c_str()); }

    /**
            Compare with c-string.

            @param str A c-string.

            @return int @li <0 : text is smaller than c-string.
                                    @li 0 :  text is equal to c-string.
                                    @li > 0 : text is greater than c-string.
    */
    int compare(const char* str) const;

    /**
            Print formatted output to text style ala sprintf.
            Existing text is written over.

            @param fmt A format string.
    */
    void print(const char* fmt, ...);
    void format(const char* fmt, ...);
    void format(const char* fmt, va_list argList);

    /**
            Catenate formatted text to end of text.

            @param fmt A format string.
    */
    void printBack(const char* fmt, ...);

private:
    char* mpText;
    size_type mSz;
    size_type mLen;

    // settings::InputArchive has friend access in order to deserialize
    // directly into BText's buffer.
    friend class settings::InputArchive;
};

/**
        Text class.

        Text class is a replacement for plain characters arrays to represent data.
        It has several advantages over the C-approach, offering constant time length
        query, safe assignment and concatenation and more.
*/
template <BText::size_type SZ> class Text : public BText
{
public:
    /**
            Create a text.
    */
    Text(const char* initial = NULL) : BText(mText, SZ), mText{} { *this = initial; }

    /**
       Copy constructor. Note that this must be here
       other way compiler will make binary copy!

       @param rhs Text initial source
     */
    Text(const Text& rhs) : BText(mText, SZ), mText{} { *this = rhs.c_str(); }

    /**
       Copy constructor.

       @param rhs Text initial source
     */
    explicit Text(const BText& rhs) : BText(mText, SZ), mText{} { *this = rhs.c_str(); }

    //////////////////////////////////
    // Allow text object assignment.
    Text& operator=(const BText& rhs)
    {
        BText::operator=(rhs.c_str());
        return *this;
    }

    Text& operator+=(const BText& rhs)
    {
        BText::operator+=(rhs.c_str());
        return *this;
    }

    Text& operator=(const char* str)
    {
        BText::operator=(str);
        return *this;
    }

    Text& operator+=(const char* str)
    {
        BText::operator+=(str);
        return *this;
    }

    Text& operator=(char ch)
    {
        BText::operator+=(ch);
        return *this;
    }

    Text& operator+=(char ch)
    {
        BText::operator+=(ch);
        return *this;
    }

private:
    char mText[SZ];
};

} // namespace nea {
