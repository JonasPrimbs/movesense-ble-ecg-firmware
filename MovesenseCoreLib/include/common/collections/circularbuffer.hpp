#pragma once
#include <stddef.h>

namespace nea
{
/** Circular buffer interface class.
 * Class adds circular buffer interface for array.
 * User may add data into circular buffer by add() method.
 * Amount of data inside buffer can be fetched with size() method.
 * operator[] can be used for getting data from buffer. When
 * user have processed element (struct, line etc) user can
 * set read cursor by calling rebase() method. Rebase method sets
 * index given as parameter as new index value zero. ie.
 * buffer.rebase(4) moves read cursor 4 bytes forward, thus
 * buffer[0] is same as buffer[4] before calling rebase method().
 * User may also read structs and fundamental datatypes from
 * circular buffer by calling bool get(index, DATATYPE & method).
 * ie. get(0, (float) val) reads float from index 0.
 */
class CircularBuffer
{
public:
    /** Create circular buffer interface for buffer.
     *
     * @param buffer pointer to buffer
     * @param size   size of the buffer
     */
    CircularBuffer(char* const buffer, const size_t _size) :
        m_begin(0),
        m_end(0),
        m_len(0),
        m_data(buffer),
        m_totalSize(_size)
    {
    }

    /** Destructor.
     * Destructor does not do anything, because there is no
     * dynamic memory to release.
     */
    ~CircularBuffer() {}

    /** Get character from circular buffer.
     * Gets character from circular buffer. Index of character is
     * Index from cursor.
     *
     * @param index Offset from read cursor.
     *
     * @return character if index is valid, '\0' in case index is invalid.
     */
    char operator[](const size_t index) const
    {
        size_t realIndex = m_begin + index;
        if (realIndex >= m_totalSize)
            realIndex -= m_totalSize;
        if (realIndex < m_totalSize) {
            return m_data[realIndex];
        } else {
            return '\0';
        }
    }

    /** Add data into circular buffer.
     *
     * @param data pointer to data.
     * @param len  length of the data.
     *
     * @return true if data was successfully added, false if adding failed.
     */
    bool add(const char * const data, const size_t len)
    {
        if ((len + m_len) < m_totalSize) {
            size_t counter = 0;
            size_t iter = 0;
            for (counter = 0, iter = m_end; counter < len; counter++, iter++) {
                if (iter >= m_totalSize)
                    iter = 0;
                m_data[iter] = data[counter];
            }
            m_end += len;
            if (m_end >= m_totalSize)
                m_end -= m_totalSize;
            m_len += len;
            return true;
        } else {
            return false;
        }
    }

    /** Rebase read cursor.
     * Move read cursor N characters forward.
     *
     * @param index index where read cursor is moved.
     *
     * @return true in case of success, false in case of failure.
     */
    bool rebase(const size_t index)
    {
        if (index <= m_len) {
            m_begin += index;
            if (m_begin >= m_totalSize)
                m_begin -= m_totalSize;
            m_len -= index;
            return true;
        } else {
            return false;
        }
    }

    /** Get number of items available in circular buffer.
     *
     * @return Return number of items available in circular buffer.
     */
    size_t size() const { return m_len; };

    /** Template method for getting base type from circular buffer.
     * Method is used for getting base data type or structure from circular buffer.
     * ie. float value; circbuff.get(0, value);
     * Method call reads float value from circular buffer from index 0.
     *
     * @param index Index where data is read.
     * @param value Reference to value where data is written.
     *
     * @return true in case of success, false in case of failure.
     */
    template <typename T> bool get(const size_t index, T& value) const
    {
        const size_t len = sizeof(value);
        if (size() >= len) {
            char* const ptr = reinterpret_cast<char*>(&value);
            for (size_t iter = 0; iter < len; iter++) {
                ptr[iter] = (*this)[iter];
            }
            return true;
        } else {
            return false;
        }
    }
private:
    size_t m_begin; /**< Read cursor                       */
    size_t m_end;   /**< Write cursor                      */
    size_t m_len;   /**< Amount of data for read operation */

    char* const m_data;       /**< Pointer to data buffer */
    const size_t m_totalSize; /**< Size of m_data          */
};
}

