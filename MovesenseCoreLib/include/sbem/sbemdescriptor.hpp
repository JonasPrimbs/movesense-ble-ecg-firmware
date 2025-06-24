#pragma once
#include <stdint.h>
#include "sbemmod.hpp"
#include "constdescriptortable.hpp"
#ifdef SBEM_DYNAMIC
#include <string>
#endif

#include "common/core/dbgassert.h"

namespace nea
{
namespace sbem
{
class DescriptorStack;

/** SBEM descriptor class.
 * Descriptor contains all information of SBEM descriptor which
 * is required for reading SBEM data element from SBEM data stream.
 */
class Descriptor
{
    friend class DescriptorTable;

public:
    static const uint8_t CHUNK_SIZE_ESCAPE;

    /** Descriptor type.
     */
    enum Type_e
    {
        Type_e_TagOpen,  /**< Descriptor opens a new TAG */
        Type_e_TagClose, /**< Descriptor closes TAG      */
        Type_e_Value,    /**< Descriptor contains value  */
    };

    /** Descriptor value data format.
     * Format describes data container type of the data inside SBEM element.
     * Data format used by application may be different because descriptor
     * may have modification rule which is used to encode and decode application
     * value into SBEM container.
     */
    enum Format_e
    {
        Format_e_bool,    /**< boolean value fitted into 1 byte */
        Format_e_uint8,   /**< uint8_t  */
        Format_e_uint16,  /**< uint16_t */
        Format_e_uint32,  /**< uint32_t */
        Format_e_uint64,  /**< uint64_t */
        Format_e_int8,    /**< int8_t   */
        Format_e_int16,   /**< int16_t  */
        Format_e_int32,   /**< int32_t  */
        Format_e_int64,   /**< int64_t  */
        Format_e_duint8,  /**< differential uint8_t TODO check implementation  */
        Format_e_duint16, /**< differential uint16_t TODO check implementation */
        Format_e_dint8,   /**< differential int8_t TODO check implementation   */
        Format_e_dint16,  /**< differential int16_t TODO check implementation  */
        Format_e_float32, /**< float  */
        Format_e_float64, /**< double */
        Format_e_local64, /**< time or something similar */
        Format_e_enum,    /**< Enumeration value, packed into 1 byte, has separate enum info inside
                            descriptor table */
        Format_e_utf8,    /**< Handled as ANSI-C string */
        Format_e_bin8,    /**< TODO Not implemented currently, most propably handled as array of data
                             */
        Format_e_utc64,   /**< utc time (microseconds) */
        Format_e_none,    /**< No data format, invalid value if Type_e == Type_e_Value */
        Format_e_numOf    /**< INVALID value shoud never be present in SBEM stream, only used for array
                            size in code */
    };

    enum ReservedSbemId_e
    {
        ReservedSbemId_e_Descriptor = 0,
        ReservedSbemId_e_Escape = 255
    };

    typedef uint16_t id_t;              /**< Descriptor ID */
    static id_t const INVALID = 0xffff; /**< INVALID descriptor ID */

    /** Method for getting descriptor ID.
     *
     * @return Descriptor ID.
     */
    id_t id() const { return m_ownId; }

    /** Method for getting descriptor alias.
     *
     * @return Alias
     */
    char const* alias() const;

    /** Method for getting parent alias descriptor.
     *
     * @return ID of parent alias descriptor.
     */
    Descriptor::id_t parentAlias() const;

    /** Method for getting data format of the descriptor container.
     * Should be called only if descriptor type is Type_e_Value.
     *
     * @return data format of the descriptor container.
     */
    Format_e format() const;

    /** Method for getting descriptor type.
     *
     * @return Type of the descriptor.
     */
    Type_e type() const;

    /** Method for getting path of the descriptor.
     * Method returns descriptors path WITHOUT alias part of the
     * original path string.
     *
     * @return path of the descriptor.
     */
    char const* path() const;

    /** Method for getting descriptors alias string.
     *
     * @return alias string.
     */
    char const* aliasString() const;

    /** Method for getting descriptor data modificator object.
     *
     * @return Modificator object.
     */
    nea::sbem::Mod const modifier() const;

    /** Method for checking if descriptor is valid or not.
     *
     * @return true if descriptor is valid, false if descriptor is invalid.
     */
    bool isValid() const
    {
        if (m_ownId != INVALID)
            return true;
        else
            return false;
    }

    /** Method for getting enumeration list head.
     * Method should be called only if Format_e == Format_e_enum.
     *
     * @return index of enum list.
     */
    uint16_t enumListHead() const;

    bool isGroup() const;

    /** Destructor.
     * never called.
     */
    ~Descriptor() {}

    id_t plusDescriptor() const;

    uint8_t numOfGroupDescriptors() const
    {
#ifdef SBEM_DYNAMIC
        return m_numOfGroupDescriptors;
#else
        if (constDesc())
        {
            return constDesc()->m_numOfDescriptors;
        }
        else
        {
            DEBUG_ASSERT(0);
            return 0;
        }
#endif
    }
    id_t const* groupDescriptors() const
    {
#ifdef SBEM_DYNAMIC
        return m_groupDescriptors;
#else
        if (constDesc())
        {
            return constDesc()->listOfDescriptors;
        }
        else
        {
            DEBUG_ASSERT(0);
            return 0;
        }

#endif
    }
#ifndef SBEM_NO_PRIVATE
private:
#endif
    /** Constructor.
     */
    Descriptor() : m_tableId(0), m_ownId(INVALID) {}

    id_t m_ownId; /**< Descriptor ID */
#ifdef SBEM_DYNAMIC
    bool parsePath(std::string path, std::string& name, uint16_t& aliasId, id_t& plusDescriptor);

    /** Method for initializing descriptor.
     *
     * @param ownId        Descriptor ID
     * @param path         Descriptor path
     * @param alias        Alias string of the
     * @param modifier     Modifier string
     * @param format       Format string
     * @param enumListHead enum list head
     * @param Type         Descriptor type
     */
    void init(id_t const ownId,
              std::string& path,
              std::string& alias,
              std::string& modifier,
              bool const isGroup,
              Format_e const format,
              uint16_t const enumListHead,
              Type_e const Type);

    void initGroupDescriptor(id_t const ownId, std::string string);

    Mod m_mod;               /**< Data modifier object */
    uint16_t m_enumListHead; /**< Enumaration data list head */

    id_t m_parentAliasId; /**< ID of parent descriptor */
#ifdef SBEM_DYNAMIC
    std::string m_path;  /**< Path of descriptor without aliases */
    std::string m_alias; /**< Alias string if descriptor defines one */
#else
    char const* const m_path;
    char const* const m_alias;
#endif
    uint8_t m_format;      /**< SBEM data container format */
    uint8_t m_type : 2;    /**< SBEM descriptor type */
    uint8_t m_isGroup : 1; /**< Bool if descriptor is group start descriptor. */
    id_t m_plusDescriptor; /**< PlusDescriptor */

    uint8_t m_numOfGroupDescriptors;
    id_t* m_groupDescriptors;
#else
    ConstDescriptor const* constDesc() const;

    Descriptor(id_t const _id, uint8_t tableId) : m_ownId(_id), m_tableId(tableId) {}

    uint8_t m_tableId;
#endif
};
}
}
