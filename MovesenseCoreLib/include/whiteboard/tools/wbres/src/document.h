#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2014.
    All rights reserved.
******************************************************************************/

#include "shared.h"

// Reserve some resources for Whiteboard use
// Note that these must match definitions in ConstResourceTree and ConstMetadataMap
const size_t MAX_FIXED_ID_RESOURCES = 256;
const size_t MAX_FIXED_ID_DATATYPES = 256;
const size_t MAX_NUMBER_APIS = 256;
const size_t MAX_NUMBER_OF_RESOURCES_PER_API = 256;
const size_t MAX_NUMBER_OF_DATATYPES_PER_API = 256;

enum class ConstrainType : unsigned char
{
    INVALID,

    MINIMUM, /*number*/
    EXCLUSIVE_MINIMUM,/*number*/
    MAXIMUM, /*number*/
    EXCLUSIVE_MAXIMUM, /*number*/
    MINLENGTH, /*int*/
    MAXLENGTH, /*int*/
    MINITEMS, /*int*/
    MAXITEMS, /*int*/
    UNIQUEITEMS, /*bool*/
    MULTIPLEOF, /*number*/
};

enum class ScalarType : unsigned char
{
    NONE,
    BOOLEAN,
    INT8,
    UINT8,
    INT16,
    UINT16,
    INT32,
    UINT32,
    INT64,
    UINT64,
    FLOAT,
    DOUBLE,
    STRING,

    COUNT
};

class Document;

typedef std::pair<std::istream*, Document*> InputStream;
typedef std::pair<std::ostream*, const Document*> OutputStream;

template <typename Type>
inline void save(
    const Type data,
    OutputStream& rStream,
    typename std::enable_if<!std::is_class<Type>::value>::type* = NULL)
{
    rStream.first->write(reinterpret_cast<const char*>(&data), sizeof(data));
}

#if SIZE_MAX == UINT64_MAX
inline void save(size_t data, OutputStream& rStream)
{
    // Save size_t always in 32-bits

    // HACK: Max value has special meaning for us. Maybe someday we should
    // use uint32_t and not size_t in our internal data structures
    if (data == static_cast<size_t>(-1))
    {
        data = static_cast<uint32_t>(-1);
    }
    else if (data >= static_cast<uint32_t>(-1))
    {
        throw std::runtime_error("Integer range overflow");
    }

    ::save(static_cast<uint32_t>(data), rStream);
}
#endif

inline void save(const bool b, OutputStream& rStream)
{
    ::save(static_cast<unsigned char>(b ? 1 : 0), rStream);
}

inline void save(const std::string& s, OutputStream& rStream)
{
    uint32_t length = static_cast<uint32_t>(s.length());
    save(length, rStream);
    rStream.first->write(s.c_str(), length);
}

template <typename Type>
inline void save(
    const Type& rClass,
    OutputStream& rStream,
    typename std::enable_if<std::is_class<Type>::value>::type* = NULL)
{
    rClass.save(rStream);
}

template <typename Type>
inline void load(
    Type& data,
    InputStream& rStream,
    typename std::enable_if<!std::is_class<Type>::value>::type* = NULL)
{
    rStream.first->read(reinterpret_cast<char*>(&data), sizeof(data));
}

#if SIZE_MAX == UINT64_MAX
inline void load(size_t& data, InputStream& rStream)
{
    // Load also size_t always in 32-bits
    uint32_t temp;
    load(temp, rStream);

    // HACK: Max value has special meaning for us. Maybe someday we should
    // use uint32_t and not size_t in our internal data structures
    if (temp == static_cast<uint32_t>(-1))
    {
        data = static_cast<size_t>(-1);
    }
    else
    {
        data = temp;
    }
}
#endif

inline void load(bool& b, InputStream& rStream)
{
    unsigned char data;
    ::load(data, rStream);
    b = data != 0;
}

inline void load(std::string& s, InputStream& rStream)
{
    uint32_t length;
    ::load(length, rStream);

    s.clear();
    if (length > 0)
    {
        s.reserve(length + 1);

        // std::copy_n doesn't increment iterator n times, but n-1 in some implementations
        // we rely on iterator being incremented correctly, so we do this the hard way
        for (size_t i = 0; i < length; ++i)
        {
            char c;
            ::load(c, rStream);
            s.push_back(c);
        }
    }
}

template <typename Type>
inline void load(
    Type& rClass,
    InputStream& rStream,
    typename std::enable_if<std::is_class<Type>::value>::type* = NULL)
{
    rClass.load(rStream);
}

template <typename Type>
inline void load(Type*& pClass, InputStream& rStream)
{
    pClass = Type::loadNew(rStream);
}

template <typename EnumType>
inline void saveEnum(const EnumType e, OutputStream& rStream)
{
    ::save(static_cast<typename std::underlying_type<EnumType>::type>(e), rStream);
}

template <typename EnumType>
inline void loadEnum(EnumType& e, InputStream& rStream)
{
    std::underlying_type<ScalarType>::type type;
    ::load(type, rStream);
    e = static_cast<EnumType>(type);
}

template <typename Type>
inline void saveIfNotNull(const Type* pItem, OutputStream& rStream)
{
    ::save(pItem != NULL, rStream);
    if (pItem != NULL)
    {
        ::save(*pItem, rStream);
    }
}

template <typename Type>
inline void loadNewIfNotNull(Type*& rpItem, InputStream& rStream)
{
    bool hasValue;
    ::load(hasValue, rStream);
    if (hasValue)
    {
        rpItem = Type::loadNew(rStream);
    }
    else
    {
        rpItem = NULL;
    }
}

template <typename Type>
class PointerList;

template <typename Type>
inline void saveReferenceIfNotNull(const Type* pItem, const PointerList<Type>& rItems, OutputStream& rStream)
{
    ::save(pItem != NULL, rStream);
    if (pItem != NULL)
    {
        rItems.saveReference(pItem, rStream);
    }
}

template <typename Type>
inline void loadReferenceIfNotNull(Type*& rpItem, PointerList<Type>& rItems, InputStream& rStream)
{
    bool hasValue;
    ::load(hasValue, rStream);
    if (hasValue)
    {
        rpItem = rItems.loadReference(rStream);
    }
    else
    {
        rpItem = NULL;
    }
}

template <typename Type>
inline void saveList(const std::vector<Type>& items, OutputStream& rStream)
{
    ::save(items.size(), rStream);
    for (auto item : items)
    {
        ::save(item, rStream);
    }
}

template <typename Type>
inline void loadList(std::vector<Type>& items, InputStream& rStream)
{
    size_t count;
    ::load(count, rStream);
    items.reserve(count);

    for (size_t i = 0; i < count; ++i)
    {
        Type item;
        ::load(item, rStream);
        items.push_back(item);
    }
}

class String
{
public:
    String()
        : mOffset(static_cast<size_t>(-1))
    {
    }

    String(const std::string& rValue)
        : mOffset(static_cast<size_t>(-1)), mValue(rValue)
    {
    }

    inline size_t size() const
    {
        return mValue.length();
    }

    void save(OutputStream& rStream) const;
    static String* loadNew(InputStream& rStream);

    size_t mOffset;
    std::string mValue;
};

class SecurityTag
{
public:
    SecurityTag()
        : mId(static_cast<size_t>(-1)), mpName(NULL)
    {
    }

    SecurityTag(const std::string& rName)
        : mId(static_cast<size_t>(-1)), mpName(new String(rName))
    {
    }

    void save(OutputStream& rStream) const;
    static SecurityTag* loadNew(InputStream& rStream);

    size_t mId;
    String* mpName;
};

class UnorderedIdList : public std::vector<size_t>
{
public:
    UnorderedIdList()
    {
    }

    bool operator==(const UnorderedIdList& rOther) const;

    inline bool operator!=(const UnorderedIdList& rOther) const
    {
        return !(*this == rOther);
    }
};

typedef UnorderedIdList SecurityTagIdList;

template <typename Type>
class PointerList : public std::vector<Type*>
{
public:
    static const size_t NOT_FOUND = static_cast<size_t>(-1);

    size_t find(const Type* pPointer) const
    {
        typename std::vector<Type*>::const_iterator it = std::find(this->begin(), this->end(), pPointer);
        if (it == this->end())
        {
            return NOT_FOUND;
        }

        return it - this->begin();
    }

    void saveReference(const Type* pPointer, OutputStream& rStream) const
    {
        size_t index = find(pPointer);
        if (index == NOT_FOUND)
        {
            throw std::runtime_error("Internal error: Item reference was not found");
        }

        ::save(index, rStream);
    }

    Type* loadReference(InputStream& rStream)
    {
        size_t index;
        ::load(index, rStream);
        if (index >= this->size())
        {
            throw std::runtime_error("");
        }

        return (*this)[index];
    }

    void saveItems(OutputStream& rStream) const
    {
        ::save(this->size(), rStream);
        for (auto pItem : *this)
        {
            pItem->save(rStream);
        }
    }

    void loadItems(InputStream& rStream)
    {
        size_t count;
        ::load(count, rStream);
        this->reserve(count);

        for (size_t i = 0; i < count; ++i)
        {
            this->push_back(Type::loadNew(rStream));
        }
    }
};

enum class Priority : unsigned char
{
    NOT_AVAILABLE,
    LOW,
    NORMAL,
    HIGH
};

class ExecutionContext
{
public:
    ExecutionContext():
        mId(static_cast<size_t>(-1)),
        mpName(NULL),
        mNumberOfDpcs(0),
        mNumberOfRequests(0),
        mNumberOfResponses(0),
        mExternalThread(false),
        mStackSize(0),
        mPriority(Priority::NOT_AVAILABLE),
        mForwardDeclaration(false)
    {
    }

    void save(OutputStream& rStream) const;
    static ExecutionContext* loadNew(InputStream& rStream);

    size_t mId;
    String* mpName;

    size_t mNumberOfDpcs;
    size_t mNumberOfRequests;
    size_t mNumberOfResponses;
    bool mExternalThread;
    size_t mStackSize;
    Priority mPriority;
    bool mForwardDeclaration;
};

class ApiExtension
{
public:
    ApiExtension() :
        mId(static_cast<size_t>(-1)),
        mDefaultExecutionContextName("")
    {
    }

    size_t mId;
    std::string mNamePattern;
    std::string mDefaultExecutionContextName;
};

class Constrain
{
public:
    Constrain()
        : mId(static_cast<size_t>(-1)), mType(ConstrainType::INVALID), mDataType(ScalarType::NONE)
    {
    }

    Constrain(ConstrainType type, bool value)
        : mId(static_cast<size_t>(-1)), mType(type), mDataType(ScalarType::BOOLEAN)
    {
        mBoolValue = value;
    }

    Constrain(ConstrainType type, int value)
        : mId(static_cast<size_t>(-1)), mType(type), mDataType(ScalarType::INT32)
    {
        mIntValue = value;
    }

    Constrain(ConstrainType type, double value)
        : mId(static_cast<size_t>(-1)), mType(type), mDataType(ScalarType::DOUBLE)
    {
        mFloatValue = value;
    }

    void save(OutputStream& rStream) const;
    static Constrain* loadNew(InputStream& rStream);

    size_t mId;
    ConstrainType mType;
    ScalarType mDataType;
    union
    {
        bool mBoolValue;
        int mIntValue;
        double mFloatValue;
    };
};

class ConstrainList : public PointerList<Constrain>
{
public:
    ConstrainList()
        : mOffset(static_cast<size_t>(-1))
    {
    }

    void save(OutputStream& rStream) const;
    static ConstrainList* loadNew(InputStream& rStream);

    size_t mOffset;
};

class DataType
{
public:
    inline DataType()
        : mId(static_cast<size_t>(-1))
    {
    }

    virtual ~DataType() {}

    virtual bool isScalar() const { return false; }
    virtual bool isEnumeration() const { return false; }
    virtual bool isNamed() const { return false; }
    virtual bool isArray() const { return false; }
    virtual bool isStructure() const { return false; }

    bool isNull() const
    {
        return !isScalar() && !isEnumeration() && !isNamed() && !isArray() && !isStructure();
    }

    inline bool isFixedId() const { return mId < MAX_FIXED_ID_DATATYPES; }

    void saveBase(OutputStream& rStream) const;
    static DataType* loadBase(InputStream& rStream);

    virtual void save(OutputStream& rStream) const;
    virtual void load(InputStream& rStream);

    size_t mId;
};

class ScalarDataType : public DataType
{
public:
    static const char* const getName(ScalarType type);

    ScalarDataType()
        : mType(ScalarType::NONE), mUnit(unit::None)
    {
    }

    ScalarDataType(ScalarType type)
        : mType(type), mUnit(unit::ScalarQuantity)
    {
    }

    ScalarDataType(ScalarType type, unit::Info unit)
        : mType(type), mUnit(unit)
    {
    }

    virtual bool isScalar() const { return true; }

    virtual void save(OutputStream& rStream) const override;
    virtual void load(InputStream& rStream) override;

    ScalarType mType;
    unit::Info mUnit;
};

class EnumerationItem
{
public:
    EnumerationItem()
        : mpName(NULL), mValue(0)
    {
    }

    EnumerationItem(const std::string& name, int32_t value)
        : mpName(new String(name)), mValue(value)
    {
    }

    EnumerationItem(String* pName, int32_t value)
        : mpName(pName), mValue(value)
    {
    }

    void save(OutputStream& rStream) const;
    static EnumerationItem* loadNew(InputStream& rStream);

    String* mpName;
    int32_t mValue;
};

class EnumerationItemList : public PointerList<EnumerationItem>
{
public:
    EnumerationItemList()
        : mOffset(static_cast<size_t>(-1))
    {
    }

    void save(OutputStream& rStream) const;
    static EnumerationItemList* loadNew(InputStream& rStream);

    size_t mOffset;
};

class EnumerationDataType : public DataType
{
public:
    EnumerationDataType()
        : mpType(NULL), mpItems(NULL)
    {
    }
    
    EnumerationDataType(ScalarDataType* pType)
        : mpType(pType), mpItems(new EnumerationItemList())
    {
    }

    virtual bool isEnumeration() const { return true; }

    virtual void save(OutputStream& rStream) const override;
    virtual void load(InputStream& rStream) override;

    ScalarDataType* mpType;
    EnumerationItemList* mpItems;
};

class NamedDataType : public DataType
{
public:
    NamedDataType()
        : mpType(NULL), mpName(NULL)
    {
    }

    NamedDataType(DataType* pType, const std::string& rName, const std::string& rSourceFile)
        : mpType(pType), mpName(new String(rName))
    {
    }

    virtual bool isNamed() const { return true; }

    virtual void save(OutputStream& rStream) const override;
    virtual void load(InputStream& rStream) override;

    DataType* mpType;
    String* mpName;
};

class ArrayDataType : public DataType
{
public:
    ArrayDataType()
        : mpItemType(NULL)
    {
    }

    ArrayDataType(DataType* pItemType)
        : mpItemType(pItemType)
    {
    }

    virtual bool isArray() const { return true; }

    virtual void save(OutputStream& rStream) const override;
    virtual void load(InputStream& rStream) override;

    DataType* mpItemType;
};

class Property
{
public:
    Property()
        : mId(static_cast<size_t>(-1)), mpType(NULL), mpName(NULL), mRequired(false), mInlineStorage(false), mpConstrains(NULL)
    {
    }

    Property(DataType* pType, const std::string& rName, bool required = false, bool inlineStorage = true)
        : mId(static_cast<size_t>(-1)), mpType(pType), mpName(new String(rName)), mRequired(required), mInlineStorage(inlineStorage), mpConstrains(NULL)
    {
    }

    Property(DataType* pType, String* pName, bool required = false, bool inlineStorage = true)
        : mId(static_cast<size_t>(-1)), mpType(pType), mpName(pName), mRequired(required), mInlineStorage(inlineStorage), mpConstrains(NULL)
    {
    }

    void save(OutputStream& rStream) const;
    static Property* loadNew(InputStream& rStream);

    size_t mId;
    DataType* mpType;
    String* mpName;
    bool mRequired;
    bool mInlineStorage;
    ConstrainList* mpConstrains;
};

class PropertyList : public PointerList<Property>
{
public:
    PropertyList()
        : mOffset(static_cast<size_t>(-1))
    {
    }

    void save(OutputStream& rStream) const;
    static PropertyList* loadNew(InputStream& rStream);

    size_t mOffset;
};

class StructureDataType : public DataType
{
public:
    StructureDataType()
        : mpProperties(NULL)
    {
    }

    void addProperty(Property* pProperty)
    {
        mpProperties->push_back(pProperty);
    }

    virtual bool isStructure() const { return true; }

    virtual void save(OutputStream& rStream) const override;
    virtual void load(InputStream& rStream) override;

    PropertyList* mpProperties;
};

enum class ParameterEncoding : unsigned char
{
    UNKNOWN,
    PATH,
    QUERY,
    HEADER,
    BODY
};

class Parameter
{
public:
    Parameter()
        : mId(static_cast<size_t>(-1)), mpName(NULL), mpType(NULL), mpConstrains(NULL)
    {
    }

    void save(OutputStream& rStream) const;
    static Parameter* loadNew(InputStream& rStream);

    size_t mId;
    ParameterEncoding mEncoding;
    std::string mDescription;
    String* mpName;
    bool mRequired;
    DataType* mpType;
    ConstrainList* mpConstrains;
};

class ParameterList : public PointerList<Parameter>
{
public:
    ParameterList()
        : mOffset(static_cast<size_t>(-1))
    {
    }

    void save(OutputStream& rStream) const;
    static ParameterList* loadNew(InputStream& rStream);

    size_t mOffset;
};

class Response
{
public:
    Response()
        : mId(static_cast<size_t>(-1)), mCode(), mDescription(), mpType(NULL), mpHeaders(NULL)
    {
    }

    Response(unsigned short responseCode, const std::string& rDescription)
        : mId(static_cast<size_t>(-1)), mCode(responseCode), mDescription(rDescription), mpType(NULL), mpHeaders(NULL)
    {
    }

    const char* getName() const;

    static bool isValidResponseCode(unsigned short responseCode);

    // Code that we use to store notification (Will be first in the list when ordered)
    static const unsigned short NOTIFICATION = 0;

    void save(OutputStream& rStream) const;
    static Response* loadNew(InputStream& rStream);

    size_t mId;
    unsigned short mCode;
    std::string mDescription;
    DataType* mpType;
    ParameterList* mpHeaders;
};

class ResponseList : public PointerList<Response>
{
public:
    ResponseList()
        : mOffset(static_cast<size_t>(-1))
    {
    }

    void copy_push_back(const std::vector<Response*>& rOther);

    static const ResponseList* getStandardErrors(const std::string& rListName);

    void save(OutputStream& rStream) const;
    static ResponseList* loadNew(InputStream& rStream);

    size_t mOffset;
};

class Operation
{
public:
    enum class Type : unsigned char
    {
        GET,
        PUT,
        POST,
        DELETE,
        SUBSCRIBE,
        UNSUBSCRIBE
    };

    static const char* const TypeName[];

    static const char* toString(Type type)
    {
        switch (type)
        {
        case Type::GET: return "GET";
        case Type::PUT: return "PUT";
        case Type::POST: return "POST";
        case Type::DELETE: return "DELETE";
        case Type::SUBSCRIBE: return "SUBSCRIBE";
        case Type::UNSUBSCRIBE: return "UNSUBSCRIBE";
        default: return NULL;
        }
    }

    Operation()
        : mId(static_cast<size_t>(-1)), mpParameters(NULL), mpResponses(NULL)
    {
    }

    void save(OutputStream& rStream) const;
    static Operation* loadNew(InputStream& rStream);

    size_t mId;
    Type mType;
    ParameterList* mpParameters;
    ResponseList* mpResponses;
    std::vector<std::string> mTags;
    std::vector<std::string> mSecurityTagNames;

    // Valid after postprocess
    SecurityTagIdList mSecurityTagIds; 
};

class OperationList
{
public:
    OperationList()
        : mId(static_cast<size_t>(-1)),
        mpGetOperation(NULL),
        mpPutOperation(NULL),
        mpPostOperation(NULL),
        mpDeleteOperation(NULL),
        mpSubscribeOperation(NULL),
        mpUnsubscribeOperation(NULL)
    {
    }

    size_t size() const
    {
        return 6;
    }

    bool hasOperations() const
    {
        return
            (mpGetOperation != NULL) ||
            (mpPutOperation != NULL) ||
            (mpPostOperation != NULL) ||
            (mpDeleteOperation != NULL) ||
            (mpSubscribeOperation != NULL) ||
            (mpUnsubscribeOperation != NULL);
    }

    Operation*& operator[](size_t index)
    {
        switch (index)
        {
        case 0: return mpGetOperation;
        case 1: return mpPutOperation;
        case 2: return mpPostOperation;
        case 3: return mpDeleteOperation;
        case 4: return mpSubscribeOperation;
        case 5: return mpUnsubscribeOperation;
        default:
            assert(false);
            static Operation* sNull = NULL;
            return sNull;
        }
    }

    void save(OutputStream& rStream) const;
    static OperationList* loadNew(InputStream& rStream);

    size_t mId;
    Operation* mpGetOperation;
    Operation* mpPutOperation;
    Operation* mpPostOperation;
    Operation* mpDeleteOperation;
    Operation* mpSubscribeOperation;
    Operation* mpUnsubscribeOperation;
};

class Path
{
public:
    Path():
        mId(static_cast<size_t>(-1)),
        mpName(NULL),
        mpExecutionContext(NULL),
        mpOperations(NULL),
        mpPathParameter(NULL),
        mpSymbolicLinkTarget(NULL),
        mpParent(NULL),
        mpNextSibling(NULL),
        mpFirstChild(NULL)
    {
    }

    inline bool isFixedId() const { return mId < MAX_FIXED_ID_RESOURCES; }

    inline bool isGenerated() const
    {
        return 
            mExecutionContextName.empty() &&
            !mpOperations->hasOperations() &&
            mSymbolicLinkPath.empty();
    }

    size_t getPathParameterCount() const
    {
        size_t result = 0;
        if (mpParent != NULL)
        {
            result += mpParent->getPathParameterCount();
        }

        if (mpPathParameter != NULL)
        {
            ++result;
        }

        return result;
    }

    void getPathParameters(ParameterList& rParameters, std::map<Parameter*, Path*>& rPathParameterPaths)
    {
        if (mpParent != NULL)
        {
            mpParent->getPathParameters(rParameters, rPathParameterPaths);
        }

        if (mpPathParameter != NULL)
        {
            rParameters.push_back(mpPathParameter);
            rPathParameterPaths[mpPathParameter] = this;
        }
    }

    void save(OutputStream& rStream) const;
    static Path* loadNew(InputStream& rStream);

    size_t mId;
    std::string mFullPath;
    std::string mExecutionContextName;
    std::string mSymbolicLinkPath;
    String* mpName;
    ExecutionContext* mpExecutionContext;
    OperationList* mpOperations;
    Parameter* mpPathParameter;

    // Valid after post process
    Path* mpSymbolicLinkTarget;
    Path* mpParent;
    Path* mpNextSibling;
    Path* mpFirstChild;
};

template <typename Type>
class UniqueObjects : public PointerList<Type>
{
public:
    UniqueObjects()
    {
    }

    inline void add(Type* pObject)
    {
        pObject->mId = this->size();
        this->push_back(pObject);
    }

    void save(OutputStream& rStream) const
    {
        this->saveItems(rStream);
    }

    void load(InputStream& rStream)
    {
        this->loadItems(rStream);
    }

    static UniqueObjects* loadNew(InputStream& rStream)
    {
        UniqueObjects* pList = new UniqueObjects();
        pList->loadItems(rStream);
        return pList;
    }
};

template <typename Type>
class UniqueObjectLists : public PointerList<Type>
{
public:
    UniqueObjectLists()
        : mCurrentOffset(0)
    {
    }

    inline void add(Type* pObjectList)
    {
        pObjectList->mOffset = mCurrentOffset;
        mCurrentOffset += 1 + pObjectList->size();
        this->push_back(pObjectList);
    }

    void save(OutputStream& rStream) const
    {
        this->saveItems(rStream);
        //::save(mCurrentOffset, rStream);
    }

    void load(InputStream& rStream)
    {
        this->loadItems(rStream);
        //::load(mCurrentOffset, rStream);
    }

    static UniqueObjects<Type>* loadNew(InputStream& rStream)
    {
        UniqueObjects<Type>* pList = new UniqueObjects<Type>();
        pList->load(rStream);
        return pList;
    }

    size_t mCurrentOffset;
};

typedef UniqueObjects<SecurityTag> UniqueSecurityTags;
typedef UniqueObjects<Property> UniqueProperties;
typedef UniqueObjects<Parameter> UniqueParameters;
typedef UniqueObjects<Response> UniqueResponses;
typedef UniqueObjects<Operation> UniqueOperations;
typedef UniqueObjects<OperationList> UniqueOperationLists;

typedef UniqueObjectLists<String> UniqueStrings;
typedef UniqueObjectLists<PropertyList> UniquePropertyLists;
typedef UniqueObjectLists<ParameterList> UniqueParameterLists;
typedef UniqueObjectLists<ResponseList> UniqueResponseLists;
typedef UniqueObjectLists<EnumerationItemList> UniqueEnumerationItemLists;

class Paths : public PointerList<Path>
{
public:
    void savePathReferences(OutputStream& rStream) const;
    void loadPathReferences(InputStream& rStream);
};

class DataTypes : public PointerList<DataType>
{
public:
    void saveReferences(OutputStream& rStream) const;
    void loadReferences(InputStream& rStream);
};

class UniqueDataTypes : public PointerList<DataType>
{
public:
    void saveBaseClassInfo(OutputStream& rStream) const;
    void loadBaseClassInfo(InputStream& rStream);

    void saveRest(OutputStream& rStream) const;
    void loadRest(InputStream& rStream);
};

class Document
{
public:
    Document()
    {
    }

    void save(OutputStream& rStream) const;
    static Document* loadNew(InputStream& rStream);

    PointerList<ExecutionContext> mExecutionContexts;
    PointerList<ApiExtension> mApiExtensionInfo;

    PointerList<SecurityTag> mSecurityTags;
    Paths mPaths;
    DataTypes mTopLevelDataTypes;

    UniqueStrings mStrings;
    UniqueDataTypes mDataTypes;
    UniqueProperties mProperties;
    UniquePropertyLists mPropertyLists;
    UniqueEnumerationItemLists mEnumerationItemLists;
    UniqueParameters mParameters;
    UniqueParameterLists mParameterLists;
    UniqueResponses mResponses;
    UniqueResponseLists mResponseLists;
    UniqueOperations mOperations;
    UniqueOperationLists mOperationLists;

    // TODO: Unique list of constrains.
};

struct InputFile
{
    void save(OutputStream& rStream) const;
    static InputFile* loadNew(InputStream& rStream);

    size_t whiteboardId;
    std::vector<size_t> apiIds;
    std::string alias;
    std::string fileName;
    Document* pDocument;
};

class InputFiles : public PointerList<InputFile>
{
public:
    /** Load input files from given file */
    void load(const std::string& rFilename);

    /** Saves the input files to given file */
    bool save(const std::string& rFilename) const;
};

template <typename ItemType>
class SelectorBase
{
public:
    virtual void select(Document* pDocument, std::vector<ItemType*>& allObjects) = 0;

    void selectLocal(const std::vector<InputFile*>& rInputFiles, std::vector<ItemType*>& allObjects)
    {
        for (auto pInputFile : rInputFiles)
        {
            if (pInputFile->whiteboardId == 0)
            {
                select(pInputFile->pDocument, allObjects);
            }
        }
    }

    void selectRemote(const std::vector<InputFile*>& rInputFiles, std::vector<ItemType*>& allObjects)
    {
        for (auto pInputFile : rInputFiles)
        {
            if (pInputFile->whiteboardId != 0)
            {
                select(pInputFile->pDocument, allObjects);
            }
        }
    }

    void selectAll(const std::vector<InputFile*>& rInputFiles, std::vector<ItemType*>& allObjects)
    {
        for (auto pInputFile : rInputFiles)
        {
            select(pInputFile->pDocument, allObjects);
        }
    }
};

// Item selectors
class SecurityTagSelector : public SelectorBase<SecurityTag>
{
public:
    virtual void select(Document* pDocument, std::vector<SecurityTag*>& allObjects) override
    {
        allObjects.insert(allObjects.end(), pDocument->mSecurityTags.begin(), pDocument->mSecurityTags.end());
    }
};

typedef SecurityTagSelector UniqueSecurityTagSelector;

class ExecutionContextSelector : public SelectorBase<ExecutionContext>
{
public:
    virtual void select(Document* pDocument, std::vector<ExecutionContext*>& allObjects) override
    {
        allObjects.insert(allObjects.end(), pDocument->mExecutionContexts.begin(), pDocument->mExecutionContexts.end());
    }
};

typedef ExecutionContextSelector UniqueExecutionContextSelector;

template <typename ItemType>
class DataTypeSelector : public SelectorBase<ItemType>
{
public:
    virtual void onDataType(DataType* pDataType, std::vector<ItemType*>& allObjects) = 0;

    inline void traverseDataType(
        std::vector<DataType*>& handledDataTypes, DataType* pDataType, std::vector<ItemType*>& allObjects)
    {
        if (pDataType == NULL)
        {
            return;
        }

        // Already handled here?
        if (std::find(handledDataTypes.begin(), handledDataTypes.end(), pDataType) != handledDataTypes.end())
        {
            return;
        }

        handledDataTypes.push_back(pDataType);
        onDataType(pDataType, allObjects);

        if (pDataType->isEnumeration())
        {
            EnumerationDataType* pEnumerationType = static_cast<EnumerationDataType*>(pDataType);
            traverseDataType(handledDataTypes, pEnumerationType->mpType, allObjects);
        }
        else if (pDataType->isNamed())
        {
            NamedDataType* pNamedType = static_cast<NamedDataType*>(pDataType);
            traverseDataType(handledDataTypes, pNamedType->mpType, allObjects);
        }
        else if (pDataType->isArray())
        {
            ArrayDataType* pArrayType = static_cast<ArrayDataType*>(pDataType);
            traverseDataType(handledDataTypes, pArrayType->mpItemType, allObjects);
        }
        else if (pDataType->isStructure())
        {
            StructureDataType* pStructureType = static_cast<StructureDataType*>(pDataType);
            for (auto pProperty : *pStructureType->mpProperties)
            {
                traverseDataType(handledDataTypes, pProperty->mpType, allObjects);
            }
        }
    }

    virtual void select(Document* pDocument, std::vector<ItemType*>& allObjects) override
    {
        std::vector<DataType*> handledDataTypes;
        for (DataType* pDataType : pDocument->mTopLevelDataTypes)
        {
            traverseDataType(handledDataTypes, pDataType, allObjects);
        }

        for (Path* pPath : pDocument->mPaths)
        {
            for (size_t i = 0; i < pPath->mpOperations->size(); ++i)
            {
                auto pOperation = (*pPath->mpOperations)[i];
                if (pOperation == NULL)
                {
                    continue;
                }

                if (pOperation->mpParameters != NULL)
                {
                    for (Parameter* pParameter : *pOperation->mpParameters)
                    {
                        traverseDataType(handledDataTypes, pParameter->mpType, allObjects);
                    }
                }

                if (pOperation->mpResponses != NULL)
                {
                    for (Response* pResponse : *pOperation->mpResponses)
                    {
                        traverseDataType(handledDataTypes, pResponse->mpType, allObjects);
                    }
                }
            }

            if (pPath->mpPathParameter != NULL)
            {
                traverseDataType(handledDataTypes, pPath->mpPathParameter->mpType, allObjects);
            }
        }
    }
};

class StringSelector : public DataTypeSelector<String>
{
public:
    virtual void onDataType(DataType* pDataType, std::vector<String*>& allObjects) override
    {
        if (pDataType->isEnumeration())
        {
            EnumerationDataType* pEnumerationType = static_cast<EnumerationDataType*>(pDataType);
            for (size_t i = 0; i < pEnumerationType->mpItems->size(); ++i)
            {
                addToListIfUnique(allObjects, (*pEnumerationType->mpItems)[i]->mpName);
            }
        }
        else if (pDataType->isNamed())
        {
            NamedDataType* pNamedType = static_cast<NamedDataType*>(pDataType);
            addToListIfUnique(allObjects, pNamedType->mpName);
        }
        else if (pDataType->isStructure())
        {
            StructureDataType* pStructureType = static_cast<StructureDataType*>(pDataType);
            for (auto pProperty : *static_cast<StructureDataType*>(pDataType)->mpProperties)
            {
                addToListIfUnique(allObjects, pProperty->mpName);
            }
        }
    }

    virtual void select(Document* pDocument, std::vector<String*>& allObjects) override
    {
        for (auto pSecurityTag : pDocument->mSecurityTags)
        {
            addToListIfUnique(allObjects, pSecurityTag->mpName);
        }

        for (auto pExecutionContext : pDocument->mExecutionContexts)
        {
            addToListIfUnique(allObjects, pExecutionContext->mpName);
        }

        DataTypeSelector<String>::select(pDocument, allObjects);

        for (Path* pPath : pDocument->mPaths)
        {
            addToListIfUnique(allObjects, pPath->mpName);
            for (size_t i = 0; i < pPath->mpOperations->size(); ++i)
            {
                Operation* pOperation = (*pPath->mpOperations)[i];
                if (pOperation == NULL)
                {
                    continue;
                }

                if (pOperation->mpParameters != NULL)
                {
                    for (Parameter* pParameter : *pOperation->mpParameters)
                    {
                        addToListIfUnique(allObjects, pParameter->mpName);
                    }
                }
            }

            if (pPath->mpPathParameter != NULL)
            {
                addToListIfUnique(allObjects, pPath->mpPathParameter->mpName);
            }
        }
    }
};

class UniqueStringSelector : public SelectorBase<String>
{
public:
    virtual void select(Document* pDocument, std::vector<String*>& allObjects) override
    {
        allObjects.insert(allObjects.end(), pDocument->mStrings.begin(), pDocument->mStrings.end());
    }
};

class AllDataTypesSelector : public DataTypeSelector<DataType>
{
public:
    virtual void onDataType(DataType* pDataType, std::vector<DataType*>& allObjects) override
    {
        allObjects.push_back(pDataType);
    }
};

class AllUniqueDataTypesSelector : public SelectorBase<DataType>
{
public:
    virtual void select(Document* pDocument, std::vector<DataType*>& allObjects) override
    {
        allObjects.insert(allObjects.end(), pDocument->mDataTypes.begin(), pDocument->mDataTypes.end());
    }
};

class TopLevelDataTypeSelector : public SelectorBase<DataType>
{
public:
    virtual void select(Document* pDocument, std::vector<DataType*>& allObjects) override
    {
        allObjects.insert(allObjects.end(), pDocument->mTopLevelDataTypes.begin(), pDocument->mTopLevelDataTypes.end());
    }
};

class PropertySelector : public DataTypeSelector<Property>
{
public:
    virtual void onDataType(DataType* pDataType, std::vector<Property*>& allObjects) override
    {
        if (pDataType->isStructure())
        {
            for (auto pProperty : *static_cast<StructureDataType*>(pDataType)->mpProperties)
            {
                addToListIfUnique(allObjects, pProperty);
            }
        }
    }
};

class UniquePropertySelector : public SelectorBase<Property>
{
public:
    virtual void select(Document* pDocument, std::vector<Property*>& allObjects) override
    {
        allObjects.insert(allObjects.end(), pDocument->mProperties.begin(), pDocument->mProperties.end());
    }
};

class PropertyListSelector : public DataTypeSelector<PropertyList>
{
public:
    virtual void onDataType(DataType* pDataType, std::vector<PropertyList*>& allObjects) override
    {
        if (pDataType->isStructure())
        {
            addToListIfUnique(allObjects, static_cast<StructureDataType*>(pDataType)->mpProperties);
        }
    }
};

class UniquePropertyListSelector : public SelectorBase<PropertyList>
{
public:
    virtual void select(Document* pDocument, std::vector<PropertyList*>& allObjects) override
    {
        allObjects.insert(allObjects.end(), pDocument->mPropertyLists.begin(), pDocument->mPropertyLists.end());
    }
};

class EnumerationItemListSelector : public DataTypeSelector<EnumerationItemList>
{
public:
    virtual void onDataType(DataType* pDataType, std::vector<EnumerationItemList*>& allObjects) override
    {
        if (pDataType->isEnumeration())
        {
            addToListIfUnique(allObjects, static_cast<EnumerationDataType*>(pDataType)->mpItems);
        }
    }
};

class UniqueEnumerationItemListSelector : public SelectorBase<EnumerationItemList>
{
public:
    virtual void select(Document* pDocument, std::vector<EnumerationItemList*>& allObjects) override
    {
        allObjects.insert(allObjects.end(), pDocument->mEnumerationItemLists.begin(), pDocument->mEnumerationItemLists.end());
    }
};

class ParameterSelector : public SelectorBase<Parameter>
{
public:
    virtual void select(Document* pDocument, std::vector<Parameter*>& allObjects) override
    {
        for (Path* pPath : pDocument->mPaths)
        {
            for (size_t i = 0; i < pPath->mpOperations->size(); ++i)
            {
                auto pOperation = (*pPath->mpOperations)[i];
                if ((pOperation == NULL) || (pOperation->mpParameters == NULL))
                {
                    continue;
                }

                for (Parameter* pParameter : *pOperation->mpParameters)
                {
                    addToListIfUnique(allObjects, pParameter);
                }
            }

            if (pPath->mpPathParameter != NULL)
            {
                addToListIfUnique(allObjects, pPath->mpPathParameter);
            }
        }
    }
};

class UniqueParameterSelector : public SelectorBase<Parameter>
{
public:
    virtual void select(Document* pDocument, std::vector<Parameter*>& allObjects) override
    {
        allObjects.insert(allObjects.end(), pDocument->mParameters.begin(), pDocument->mParameters.end());
    }
};

class ParameterListSelector : public SelectorBase<ParameterList>
{
public:
    virtual void select(Document* pDocument, std::vector<ParameterList*>& allObjects) override
    {
        for (Path* pPath : pDocument->mPaths)
        {
            for (size_t i = 0; i < pPath->mpOperations->size(); ++i)
            {
                auto pOperation = (*pPath->mpOperations)[i];
                if ((pOperation == NULL) || (pOperation->mpParameters == NULL))
                {
                    continue;
                }

                addToListIfUnique(allObjects, pOperation->mpParameters);
            }
        }
    }
};

class UniqueParameterListSelector : public SelectorBase<ParameterList>
{
public:
    virtual void select(Document* pDocument, std::vector<ParameterList*>& allObjects) override
    {
        allObjects.insert(allObjects.end(), pDocument->mParameterLists.begin(), pDocument->mParameterLists.end());
    }
};

class ResponseSelector : public SelectorBase<Response>
{
public:
    virtual void select(Document* pDocument, std::vector<Response*>& allObjects) override
    {
        for (Path* pPath : pDocument->mPaths)
        {
            for (size_t i = 0; i < pPath->mpOperations->size(); ++i)
            {
                auto pOperation = (*pPath->mpOperations)[i];
                if ((pOperation == NULL) || (pOperation->mpResponses == NULL))
                {
                    continue;
                }

                for (Response* pResponse : *pOperation->mpResponses)
                {
                    addToListIfUnique(allObjects, pResponse);
                }
            }
        }
    }
};

class UniqueResponseSelector : public SelectorBase<Response>
{
public:
    virtual void select(Document* pDocument, std::vector<Response*>& allObjects) override
    {
        allObjects.insert(allObjects.end(), pDocument->mResponses.begin(), pDocument->mResponses.end());
    }
};

class ResponseListSelector : public SelectorBase<ResponseList>
{
public:
    virtual void select(Document* pDocument, std::vector<ResponseList*>& allObjects) override
    {
        for (Path* pPath : pDocument->mPaths)
        {
            for (size_t i = 0; i < pPath->mpOperations->size(); ++i)
            {
                auto pOperation = (*pPath->mpOperations)[i];
                if ((pOperation == NULL) || (pOperation->mpResponses == NULL))
                {
                    continue;
                }

                addToListIfUnique(allObjects, pOperation->mpResponses);
            }
        }
    }
};

class UniqueResponseListSelector : public SelectorBase<ResponseList>
{
public:
    virtual void select(Document* pDocument, std::vector<ResponseList*>& allObjects) override
    {
        allObjects.insert(allObjects.end(), pDocument->mResponseLists.begin(), pDocument->mResponseLists.end());
    }
};

class OperationSelector : public SelectorBase<Operation>
{
public:
    virtual void select(Document* pDocument, std::vector<Operation*>& allObjects) override
    {
        for (Path* pPath : pDocument->mPaths)
        {
            for (size_t i = 0; i < pPath->mpOperations->size(); ++i)
            {
                auto pOperation = (*pPath->mpOperations)[i];
                if (pOperation == NULL)
                {
                    continue;
                }

                addToListIfUnique(allObjects, pOperation);
            }
        }
    }
};

class UniqueOperationSelector : public SelectorBase<Operation>
{
public:
    virtual void select(Document* pDocument, std::vector<Operation*>& allObjects) override
    {
        allObjects.insert(allObjects.end(), pDocument->mOperations.begin(), pDocument->mOperations.end());
    }
};

class OperationListSelector : public SelectorBase<OperationList>
{
public:
    virtual void select(Document* pDocument, std::vector<OperationList*>& allObjects) override
    {
        for (Path* pPath : pDocument->mPaths)
        {
            addToListIfUnique(allObjects, pPath->mpOperations);
        }
    }
};

class UniqueOperationListSelector : public SelectorBase<OperationList>
{
public:
    virtual void select(Document* pDocument, std::vector<OperationList*>& allObjects) override
    {
        allObjects.insert(allObjects.end(), pDocument->mOperationLists.begin(), pDocument->mOperationLists.end());
    }
};

class PathSelector : public SelectorBase<Path>
{
public:
    virtual void select(Document* pDocument, std::vector<Path*>& allObjects) override
    {
        allObjects.insert(allObjects.end(), pDocument->mPaths.begin(), pDocument->mPaths.end());
    }
};

typedef PathSelector UniquePathSelector;

inline bool isBuiltinType(const DataType& rDataType)
{
    static const char* BuiltinTypes[] = {
        "ResourceId", "ExecutionContextId", "LocalClientId", "LocalProviderId", "LocalResourceId", "LocalDataTypeId", "WhiteboardId",
        "Date", "DateTime", "Time", "Timestamp", "FloatVector2D", "FloatVector3D", "IntVector2D", "IntVector3D",
        "ByteStream"
    };
    if (!rDataType.isNamed())
    {
        return false;
    }

    const NamedDataType& rNamedDataType = static_cast<const NamedDataType&>(rDataType);
    for (size_t i = 0; i < ELEMENTS(BuiltinTypes); ++i)
    {
        if (rNamedDataType.mpName->mValue == BuiltinTypes[i])
        {
            return true;
        }
    }

    return false;
}

bool areScalarTypesEqual(const ScalarDataType& rType1, const ScalarDataType& rType2);
bool areEnumerationItemsEqual(const EnumerationItemList& rList1, const EnumerationItemList& rList2);
bool arePathParametersEqual(const Parameter& rParam1, const Parameter& rParam2);

bool areDataTypeDefinitionsEqual(const DataType& rDataType1, const DataType& rDataType2);

struct DataTypeDefinitionsEqualTo
{
    bool operator()(const DataType& rLeft, const DataType& rRight) const
    {
        return areDataTypeDefinitionsEqual(rLeft, rRight);
    }

    bool operator()(const DataType* pLeft, const DataType* pRight) const
    {
        return areDataTypeDefinitionsEqual(*pLeft, *pRight);
    }
};

NamedDataType* addNamedTypeToListIfUnique(std::vector<NamedDataType*>& rUniqueList, NamedDataType* pNamedDataType);

template <typename Type>
bool areSetsEqual(const std::vector<Type>& rSet1, const std::vector<Type>& rSet2)
{
    for (auto item : rSet1)
    {
        if (std::find(rSet2.begin(), rSet2.end(), item) == rSet2.end())
        {
            return false;
        }
    }

    for (auto item : rSet2)
    {
        if (std::find(rSet1.begin(), rSet1.end(), item) == rSet1.end())
        {
            return false;
        }
    }

    return true;
}

template <typename T>
void collectAssignedIds(const char* objectType, std::map<size_t, bool>& rFixedIdMap, const std::vector<T*>& rObjects)
{
    for (size_t i = 0; i < rObjects.size(); ++i)
    {
        T* pObject = rObjects[i];
        if (pObject->mId != static_cast<size_t>(-1))
        {
            if (rFixedIdMap.find(pObject->mId) == rFixedIdMap.end())
            {
                rFixedIdMap[pObject->mId] = true;
            }
            else
            {
                std::stringstream message;
                message << "Multiple " << objectType << "s with fixed ID '" << pObject->mId << "'." << std::endl;
                throw std::runtime_error(message.str());
            }
        }
    }
}

inline size_t findNextAvailableId(const std::map<size_t, bool>& rFixedIdMap, size_t id)
{
    while (true)
    {
        if (rFixedIdMap.find(id) == rFixedIdMap.end())
        {
            return id;
        }

        ++id;
    }
}
