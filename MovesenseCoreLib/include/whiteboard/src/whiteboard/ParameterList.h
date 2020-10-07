#pragma once
//  Copyright (c) 2015 Suunto Oy. All rights reserved.

#include "whiteboard/WhiteboardConfig.h"
#include "whiteboard/Value.h"
#include "whiteboard/Identifiers.h"

namespace whiteboard
{

/** List of parameters */
class WB_API ParameterList
{
private:
    /** Prevent use of default constructor */
    ParameterList() DELETED;

    /** Prevent use of copy constructor */
    ParameterList(const ParameterList&) DELETED;

    /** Prevent use of assigment operator */
    const ParameterList& operator=(const ParameterList&);

protected:
    /** Constructor
    *
    * @param numberOfParameters Number of parameters in the parameter list
    */
    inline ParameterList(size_t numberOfParameters);

    /// Destructor; calls dtor of each Value in the ParameterList
    virtual ~ParameterList();

public:
    /** Empty parameter list */
    static const ParameterList Empty;

    /** Gets number of parameters in the parameter list
     *
     * @return Number of parameters
     */
    inline size_t getNumberOfParameters() const;

    /** Gets mutable parameter
     *
     * @param index Index of the parameter
     * @return Reference to mutable parameter object
     */
    inline Value& operator[](size_t index);

    /** Gets non-mutable parameter
    *
    * @param index Index of the parameter
    * @return Reference to non-mutable parameter object
    */
    inline const Value& operator[](size_t index) const;

    /** Resets the number of parameters of the parameter list to zero
    *
    * @note This method won't call destructors for removed parameters, so make sure that you don't remove
    *       parameters that have already been initialized.
    */
    inline void clear();

protected:
    /** Number of parameters */
    size_t mNumberOfParameters;

    /** Array of parameters (values) follow */
};

/** List of parameters */
template <size_t NUMBER_OF_PARAMETERS> class ParameterListInstance : public ParameterList
{
private:
    /** Prevent use of copy constructor */
    ParameterListInstance(const ParameterListInstance&) DELETED;

    /** Prevent use of assignment operator */
    const ParameterListInstance& operator=(const ParameterListInstance&) DELETED;

public:
    /** Default constructor */
    inline ParameterListInstance();

    /** Constructor
    *
    * @param numberOfParameters Number of parameters in the parameter list
    */
    inline ParameterListInstance(size_t numberOfParameters);

    /** Sets number of parameters in the parameter list.
     *
     * @note This method won't call destructors for removed parameters, so make sure that you don't remove
     *       parameters that have already been initialized.
     * @param numberOfParameters Number of parameters
     */
    inline void setNumberOfParameters(size_t numberOfParameters);

private:
    /** Array of parameters (values). Reserved only space and don't call constructors.
    * Align on Value's natural alignment. */
    WB_STATIC_VERIFY(WB_TYPE_ALIGNMENT(Value) == WB_TYPE_ALIGNMENT(void*), AlingmentOfValueClassIsNotWhatExpected);
    union
    {
        void* aligner;

        WB_ALIGN(WB_TYPE_ALIGNMENT(Value)) uint8 mData[((NUMBER_OF_PARAMETERS == 0) ? 1 : NUMBER_OF_PARAMETERS) * sizeof(Value)];
    };
};

inline ParameterList::ParameterList(size_t numberOfParameters) : mNumberOfParameters(numberOfParameters)
{
}

inline ParameterList::~ParameterList()
{
#ifdef WB_NEED_STRUCTURE_CLEANING
    for (size_t i = 0; i < this->getNumberOfParameters(); ++i)
    {
        Value& rValue = (*this)[i];
        rValue.~Value();
    }
#endif
}

inline size_t ParameterList::getNumberOfParameters() const
{
    return mNumberOfParameters;
}

inline void ParameterList::clear()
{
    mNumberOfParameters = 0;
}


inline Value& ParameterList::operator[](size_t index)
{
    WB_DEBUG_ASSERT(index < mNumberOfParameters);
    Value* pParameters = reinterpret_cast<Value*>(reinterpret_cast<uint8*>(this) + sizeof(ParameterList));
    return pParameters[index];
}

inline const Value& ParameterList::operator[](size_t index) const
{
    WB_DEBUG_ASSERT(index < mNumberOfParameters);
    const Value* pParameters = reinterpret_cast<const Value*>(reinterpret_cast<const uint8*>(this) + sizeof(ParameterList));
    return pParameters[index];
}

template <size_t NUMBER_OF_PARAMETERS>
inline ParameterListInstance<NUMBER_OF_PARAMETERS>::ParameterListInstance() : ParameterList(NUMBER_OF_PARAMETERS)
{
    WB_STATIC_VERIFY(NUMBER_OF_PARAMETERS <= WB_MAX_NUMBER_OF_PARAMETERS, TooManyParametersInParameterList);
}

template <size_t NUMBER_OF_PARAMETERS>
inline ParameterListInstance<NUMBER_OF_PARAMETERS>::ParameterListInstance(size_t numberOfParameters)
    : ParameterList(numberOfParameters)
{
    WB_STATIC_VERIFY(NUMBER_OF_PARAMETERS <= WB_MAX_NUMBER_OF_PARAMETERS, TooManyParametersInParameterList);
    WB_DEBUG_ASSERT(numberOfParameters <= NUMBER_OF_PARAMETERS);
}

template <size_t NUMBER_OF_PARAMETERS>
inline void ParameterListInstance<NUMBER_OF_PARAMETERS>::setNumberOfParameters(size_t numberOfParameters)
{
    WB_DEBUG_ASSERT(numberOfParameters <= NUMBER_OF_PARAMETERS);
    mNumberOfParameters = numberOfParameters;
}

/**
In this version of ParameterList, the native types can be given during construction. There is a separate implementation
for every number of parameters.

TODO: In C++11 this would be more elegant through variadic templates...
*/
template <typename P0 = NoType, typename P1 = NoType, typename P2 = NoType, typename P3 = NoType,
          typename P4 = NoType, typename P5 = NoType, typename P6 = NoType, typename P7 = NoType>
class ParameterListTemplateInstance FINAL : public ParameterListInstance<8>
{
private:
    /** Prevent use of copy constructor */
    ParameterListTemplateInstance(const ParameterListTemplateInstance&) DELETED;

    /** Prevent use of assignment operator */
    const ParameterListTemplateInstance& operator=(const ParameterListTemplateInstance&) DELETED;

public:
    /** Default constructor */
    inline ParameterListTemplateInstance(
        const P0& rP0, const P1& rP1, const P2& rP2, const P3& rP3, const P4& rP4, const P5& rP5, const P6& rP6, const P7& rP7)
    {
        WB_STATIC_VERIFY((IsSame<P0, Value>::value == 0), P0_Raw_value_must_be_given_NOT_the_Value_wrapper);
        WB_STATIC_VERIFY((IsSame<P1, Value>::value == 0), P1_Raw_value_must_be_given_NOT_the_Value_wrapper);
        WB_STATIC_VERIFY((IsSame<P2, Value>::value == 0), P2_Raw_value_must_be_given_NOT_the_Value_wrapper);
        WB_STATIC_VERIFY((IsSame<P3, Value>::value == 0), P3_Raw_value_must_be_given_NOT_the_Value_wrapper);
        WB_STATIC_VERIFY((IsSame<P4, Value>::value == 0), P4_Raw_value_must_be_given_NOT_the_Value_wrapper);
        WB_STATIC_VERIFY((IsSame<P5, Value>::value == 0), P5_Raw_value_must_be_given_NOT_the_Value_wrapper);
        WB_STATIC_VERIFY((IsSame<P6, Value>::value == 0), P6_Raw_value_must_be_given_NOT_the_Value_wrapper);
        WB_STATIC_VERIFY((IsSame<P7, Value>::value == 0), P7_Raw_value_must_be_given_NOT_the_Value_wrapper);

        new (&(*this)[0]) Value(rP0);
        new (&(*this)[1]) Value(rP1);
        new (&(*this)[2]) Value(rP2);
        new (&(*this)[3]) Value(rP3);
        new (&(*this)[4]) Value(rP4);
        new (&(*this)[5]) Value(rP5);
        new (&(*this)[6]) Value(rP6);
        new (&(*this)[7]) Value(rP7);
    }
};

template <typename P0>
class ParameterListTemplateInstance<P0, NoType, NoType, NoType, NoType, NoType, NoType, NoType> FINAL : public ParameterListInstance<1>
{
private:
    /** Prevent use of copy constructor */
    ParameterListTemplateInstance(const ParameterListTemplateInstance&) DELETED;

    /** Prevent use of assignment operator */
    const ParameterListTemplateInstance& operator=(const ParameterListTemplateInstance&) DELETED;

public:
    /** Default constructor */
    inline ParameterListTemplateInstance(const P0& rP0)
    {
        WB_STATIC_VERIFY((IsSame<P0, Value>::value == 0), P0_Raw_value_must_be_given_NOT_the_Value_wrapper);

        new (&(*this)[0]) Value(rP0);
    }
};

template <typename P0, typename P1>
class ParameterListTemplateInstance<P0, P1, NoType, NoType, NoType, NoType, NoType, NoType> FINAL : public ParameterListInstance<2>
{
private:
    /** Prevent use of copy constructor */
    ParameterListTemplateInstance(const ParameterListTemplateInstance&) DELETED;

    /** Prevent use of assignment operator */
    const ParameterListTemplateInstance& operator=(const ParameterListTemplateInstance&) DELETED;

public:
    /** Default constructor */
    inline ParameterListTemplateInstance(const P0& rP0, const P1& rP1)
    {
        WB_STATIC_VERIFY((IsSame<P0, Value>::value == 0), P0_Raw_value_must_be_given_NOT_the_Value_wrapper);
        WB_STATIC_VERIFY((IsSame<P1, Value>::value == 0), P1_Raw_value_must_be_given_NOT_the_Value_wrapper);

        new (&(*this)[0]) Value(rP0);
        new (&(*this)[1]) Value(rP1);
    }
};

template <typename P0, typename P1, typename P2>
class ParameterListTemplateInstance<P0, P1, P2, NoType, NoType, NoType, NoType, NoType> FINAL : public ParameterListInstance<3>
{
private:
    /** Prevent use of copy constructor */
    ParameterListTemplateInstance(const ParameterListTemplateInstance&) DELETED;

    /** Prevent use of assignment operator */
    const ParameterListTemplateInstance& operator=(const ParameterListTemplateInstance&) DELETED;

public:
    /** Default constructor */
    inline ParameterListTemplateInstance(const P0& rP0, const P1& rP1, const P2& rP2)
    {
        WB_STATIC_VERIFY((IsSame<P0, Value>::value == 0), P0_Raw_value_must_be_given_NOT_the_Value_wrapper);
        WB_STATIC_VERIFY((IsSame<P1, Value>::value == 0), P1_Raw_value_must_be_given_NOT_the_Value_wrapper);
        WB_STATIC_VERIFY((IsSame<P2, Value>::value == 0), P2_Raw_value_must_be_given_NOT_the_Value_wrapper);

        new (&(*this)[0]) Value(rP0);
        new (&(*this)[1]) Value(rP1);
        new (&(*this)[2]) Value(rP2);
    }
};

template <typename P0, typename P1, typename P2, typename P3>
class ParameterListTemplateInstance<P0, P1, P2, P3, NoType, NoType, NoType, NoType> FINAL : public ParameterListInstance<4>
{
private:
    /** Prevent use of copy constructor */
    ParameterListTemplateInstance(const ParameterListTemplateInstance&) DELETED;

    /** Prevent use of assignment operator */
    const ParameterListTemplateInstance& operator=(const ParameterListTemplateInstance&) DELETED;

public:
    /** Default constructor */
    inline ParameterListTemplateInstance(
        const P0& rP0, const P1& rP1, const P2& rP2, const P3& rP3)
    {
        WB_STATIC_VERIFY((IsSame<P0, Value>::value == 0), P0_Raw_value_must_be_given_NOT_the_Value_wrapper);
        WB_STATIC_VERIFY((IsSame<P1, Value>::value == 0), P1_Raw_value_must_be_given_NOT_the_Value_wrapper);
        WB_STATIC_VERIFY((IsSame<P2, Value>::value == 0), P2_Raw_value_must_be_given_NOT_the_Value_wrapper);
        WB_STATIC_VERIFY((IsSame<P3, Value>::value == 0), P3_Raw_value_must_be_given_NOT_the_Value_wrapper);

        new (&(*this)[0]) Value(rP0);
        new (&(*this)[1]) Value(rP1);
        new (&(*this)[2]) Value(rP2);
        new (&(*this)[3]) Value(rP3);
    }
};

template <typename P0, typename P1, typename P2, typename P3, typename P4>
class ParameterListTemplateInstance<P0, P1, P2, P3, P4, NoType, NoType, NoType> FINAL : public ParameterListInstance<5>
{
private:
    /** Prevent use of copy constructor */
    ParameterListTemplateInstance(const ParameterListTemplateInstance&) DELETED;

    /** Prevent use of assignment operator */
    const ParameterListTemplateInstance& operator=(const ParameterListTemplateInstance&) DELETED;

public:
    /** Default constructor */
    inline ParameterListTemplateInstance(
        const P0& rP0, const P1& rP1, const P2& rP2, const P3& rP3, const P4& rP4)
    {
        WB_STATIC_VERIFY((IsSame<P0, Value>::value == 0), P0_Raw_value_must_be_given_NOT_the_Value_wrapper);
        WB_STATIC_VERIFY((IsSame<P1, Value>::value == 0), P1_Raw_value_must_be_given_NOT_the_Value_wrapper);
        WB_STATIC_VERIFY((IsSame<P2, Value>::value == 0), P2_Raw_value_must_be_given_NOT_the_Value_wrapper);
        WB_STATIC_VERIFY((IsSame<P3, Value>::value == 0), P3_Raw_value_must_be_given_NOT_the_Value_wrapper);
        WB_STATIC_VERIFY((IsSame<P4, Value>::value == 0), P4_Raw_value_must_be_given_NOT_the_Value_wrapper);

        new (&(*this)[0]) Value(rP0);
        new (&(*this)[1]) Value(rP1);
        new (&(*this)[2]) Value(rP2);
        new (&(*this)[3]) Value(rP3);
        new (&(*this)[4]) Value(rP4);
    }
};

template <typename P0, typename P1, typename P2, typename P3, typename P4, typename P5>
class ParameterListTemplateInstance<P0, P1, P2, P3, P4, P5, NoType, NoType> FINAL : public ParameterListInstance<6>
{
private:
    /** Prevent use of copy constructor */
    ParameterListTemplateInstance(const ParameterListTemplateInstance&) DELETED;

    /** Prevent use of assignment operator */
    const ParameterListTemplateInstance& operator=(const ParameterListTemplateInstance&) DELETED;

public:
    /** Default constructor */
    inline ParameterListTemplateInstance(
        const P0& rP0, const P1& rP1, const P2& rP2, const P3& rP3, const P4& rP4, const P5& rP5)
    {
        WB_STATIC_VERIFY((IsSame<P0, Value>::value == 0), P0_Raw_value_must_be_given_NOT_the_Value_wrapper);
        WB_STATIC_VERIFY((IsSame<P1, Value>::value == 0), P1_Raw_value_must_be_given_NOT_the_Value_wrapper);
        WB_STATIC_VERIFY((IsSame<P2, Value>::value == 0), P2_Raw_value_must_be_given_NOT_the_Value_wrapper);
        WB_STATIC_VERIFY((IsSame<P3, Value>::value == 0), P3_Raw_value_must_be_given_NOT_the_Value_wrapper);
        WB_STATIC_VERIFY((IsSame<P4, Value>::value == 0), P4_Raw_value_must_be_given_NOT_the_Value_wrapper);
        WB_STATIC_VERIFY((IsSame<P5, Value>::value == 0), P5_Raw_value_must_be_given_NOT_the_Value_wrapper);

        new (&(*this)[0]) Value(rP0);
        new (&(*this)[1]) Value(rP1);
        new (&(*this)[2]) Value(rP2);
        new (&(*this)[3]) Value(rP3);
        new (&(*this)[4]) Value(rP4);
        new (&(*this)[5]) Value(rP5);
    }
};

template <typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
class ParameterListTemplateInstance<P0, P1, P2, P3, P4, P5, P6, NoType> FINAL : public ParameterListInstance<7>
{
private:
    /** Prevent use of copy constructor */
    ParameterListTemplateInstance(const ParameterListTemplateInstance&) DELETED;

    /** Prevent use of assignment operator */
    const ParameterListTemplateInstance& operator=(const ParameterListTemplateInstance&) DELETED;

public:
    /** Default constructor */
    inline ParameterListTemplateInstance(
        const P0& rP0, const P1& rP1, const P2& rP2, const P3& rP3, const P4& rP4, const P5& rP5, const P6& rP6)
    {
        WB_STATIC_VERIFY((IsSame<P0, Value>::value == 0), P0_Raw_value_must_be_given_NOT_the_Value_wrapper);
        WB_STATIC_VERIFY((IsSame<P1, Value>::value == 0), P1_Raw_value_must_be_given_NOT_the_Value_wrapper);
        WB_STATIC_VERIFY((IsSame<P2, Value>::value == 0), P2_Raw_value_must_be_given_NOT_the_Value_wrapper);
        WB_STATIC_VERIFY((IsSame<P3, Value>::value == 0), P3_Raw_value_must_be_given_NOT_the_Value_wrapper);
        WB_STATIC_VERIFY((IsSame<P4, Value>::value == 0), P4_Raw_value_must_be_given_NOT_the_Value_wrapper);
        WB_STATIC_VERIFY((IsSame<P5, Value>::value == 0), P5_Raw_value_must_be_given_NOT_the_Value_wrapper);
        WB_STATIC_VERIFY((IsSame<P6, Value>::value == 0), P6_Raw_value_must_be_given_NOT_the_Value_wrapper);

        new (&(*this)[0]) Value(rP0);
        new (&(*this)[1]) Value(rP1);
        new (&(*this)[2]) Value(rP2);
        new (&(*this)[3]) Value(rP3);
        new (&(*this)[4]) Value(rP4);
        new (&(*this)[5]) Value(rP5);
        new (&(*this)[6]) Value(rP6);
    }
};

} // namespace whiteboard
