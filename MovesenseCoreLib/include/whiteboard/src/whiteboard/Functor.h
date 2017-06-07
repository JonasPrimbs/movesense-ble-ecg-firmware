#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/integration/shared/types.h"
#include "whiteboard/integration/bsp/newOperator.h"

namespace whiteboard
{

/// Parameterless callable interface with return value
template <typename R>
class IFunctor0 : public IDynamicallyAllocatable
{
public:
    /** Destructor */
    virtual ~IFunctor0() {}

    /** Executes the functor */
    virtual R operator() () = 0;
};

/// Callable interface, with return value & one parameter
template <typename R, typename P1>
class IFunctor1 : public IDynamicallyAllocatable
{
public:
    /** Destructor */
    virtual ~IFunctor1() {}

    /** Executes the functor */
    virtual R operator() (P1 p1) = 0;
};

/// Callable interface, with return value & two parameters
template <typename R, typename P1, typename P2>
class IFunctor2 : public IDynamicallyAllocatable
{
public:
    /** Destructor */
    virtual ~IFunctor2() {}

    /** Executes the functor */
    virtual R operator() (P1 p1, P2 p2) = 0;
};

/// Callable interface, with return value & three parameters
template <typename R, typename P1, typename P2, typename P3>
class IFunctor3 : public IDynamicallyAllocatable
{
public:
    /** Destructor */
    virtual ~IFunctor3() {}

    /** Executes the functor */
    virtual R operator() (P1 p1, P2 p2, P3 p3) = 0;
};

/// Callable interface, with return value & four parameters
template <typename R, typename P1, typename P2, typename P3, typename P4>
class IFunctor4 : public IDynamicallyAllocatable
{
public:
    /** Destructor */
    virtual ~IFunctor4() {}

    /** Executes the functor */
    virtual R operator() (P1 p1, P2 p2, P3 p3, P4 p4) = 0;
};

/// Functor that calls parameterless member function
template <class TObject, typename R>
class MemberFunctor0 FINAL : public IFunctor0<R>
{
protected:
    ///  Pointer to class instance to call
    TObject* mpClient;

    /// Member function to call
    R (TObject::*mpFnMemberFunction) ();

public:

    /** Ctor
    @param pInstance [in] Pointer to instance of an object to call
    @param pMemberFunction [in] Member function (parameterless) to call
    */
    MemberFunctor0(TObject* pInstance, R (TObject::*pMemberFunction)())
        : mpClient(pInstance), mpFnMemberFunction(pMemberFunction)
    {
    }

    /// Dtor
    virtual ~MemberFunctor0() {}

    /// Parameterless functor
    virtual R operator() () OVERRIDE
    {
        return (mpClient->*mpFnMemberFunction)();
    }
};

// Functor that calls member function with one parameter
template <class TObject, typename R, typename P1>
class MemberFunctor1 FINAL : public IFunctor1<R, P1>
{
protected:
    ///  Pointer to class instance to call
    TObject* mpClient;

    /// Member function to call
    R (TObject::*mpFnMemberFunction) (P1 p1);

public:

    /** Ctor
    @param pInstance [in] Pointer to instance of an object to call
    @param pMemberFunction [in] Member function (with one parameter) to call
    */
    MemberFunctor1(TObject* pInstance, R (TObject::*pMemberFunction)(P1 p1))
        : mpClient(pInstance), mpFnMemberFunction(pMemberFunction)
    {
    }

    /// Dtor
    virtual ~MemberFunctor1() {}

    /**
    Functor

    @param p1 [in] Parameter to pass to the call target
    */
    virtual R operator() (P1 p1) OVERRIDE
    {
        return (mpClient->*mpFnMemberFunction)(p1);
    }
};

/// Functor that calls member function with two parameters
template <class TObject, typename R, typename P1, typename P2>
class MemberFunctor2 FINAL : public IFunctor2<R, P1, P2>
{
protected:
    ///  Pointer to class instance to call
    TObject* mpClient;

    /// Member function to call
    R (TObject::*mpFnMemberFunction) (P1 p1, P2 p2);

public:

    /** Ctor
    @param pInstance [in] Pointer to instance of an object to call
    @param pMemberFunction [in] Member function (with two parameters) to call
    */
    MemberFunctor2(TObject* pInstance, R (TObject::*pMemberFunction)(P1 p1, P2 p2))
        : mpClient(pInstance), mpFnMemberFunction(pMemberFunction)
    {
    }

    virtual ~MemberFunctor2() {}

    /**
    Functor

    @param p1 [in] 1st parameter to pass to the call target
    @param p2 [in] 2nd parameter to pass to the call target
    */
    virtual R operator() (P1 p1, P2 p2) OVERRIDE
    {
        return (mpClient->*mpFnMemberFunction)(p1, p2);
    }
};

/// Functor that calls member function with three parameters
template <class TObject, typename R, typename P1, typename P2, typename P3>
class MemberFunctor3 FINAL : public IFunctor3<R, P1, P2, P3>
{
protected:
    ///  Pointer to class instance to call
    TObject* mpClient;

    /// Member function to call
    R (TObject::*mpFnMemberFunction) (P1 p1, P2 p2, P3 p3);

public:

    /** Ctor
    @param pInstance [in] Pointer to instance of an object to call
    @param pMemberFunction [in] Member function (with three parameters) to call
    */
    MemberFunctor3(TObject* pInstance, R (TObject::*pMemberFunction)(P1 p1, P2 p2, P3 p3))
        : mpClient(pInstance), mpFnMemberFunction(pMemberFunction)
    {
    }

    /// dtor
    virtual ~MemberFunctor3() {}

    /** Functor
    @param p1 [in] 1st parameter to pass to the call target
    @param p2 [in] 2nd parameter to pass to the call target
    @param p3 [in] 3rd parameter to pass to the call target
    */
    virtual R operator() (P1 p1, P2 p2, P3 p3) OVERRIDE
    {
        return (mpClient->*mpFnMemberFunction)(p1, p2, p3);
    }
};

/// Functor that calls member function with four parameters
template <class TObject, typename R, typename P1, typename P2, typename P3, typename P4>
class MemberFunctor4 FINAL : public IFunctor4<R, P1, P2, P3, P4>
{
protected:
    ///  Pointer to class instance to call
    TObject* mpClient;

    /// Member function to call
    R(TObject::*mpFnMemberFunction) (P1 p1, P2 p2, P3 p3, P4 p4);

public:

    /** Ctor
    @param pInstance [in] Pointer to instance of an object to call
    @param pMemberFunction [in] Member function (with four parameters) to call
    */
    MemberFunctor4(TObject* pInstance, R(TObject::*pMemberFunction)(P1 p1, P2 p2, P3 p3, P4 p4))
        : mpClient(pInstance), mpFnMemberFunction(pMemberFunction)
    {
    }

    /// dtor
    virtual ~MemberFunctor4() {}

    /** Functor
    @param p1 [in] 1st parameter to pass to the call target
    @param p2 [in] 2nd parameter to pass to the call target
    @param p3 [in] 3rd parameter to pass to the call target
    @param p4 [in] 4th parameter to pass to the call target
    */
    virtual R operator() (P1 p1, P2 p2, P3 p3, P4 p4) OVERRIDE
    {
        return (mpClient->*mpFnMemberFunction)(p1, p2, p3, p4);
    }
};

/// Functor that calls parameterless member function
template <class TObject, typename R>
class ConstMemberFunctor0 FINAL : public IFunctor0<R>
{
protected:
    ///  Pointer to class instance to call
    const TObject* const mpClient;

    /// Member function to call
    R(TObject::*mpFnMemberFunction) () const;

public:

    /** Ctor
    @param pInstance [in] Pointer to instance of an object to call
    @param pMemberFunction [in] Member function (parameterless) to call
    */
    ConstMemberFunctor0(const TObject* const pInstance, R(TObject::*pMemberFunction)() const)
        : mpClient(pInstance), mpFnMemberFunction(pMemberFunction)
    {
    }

    /// Dtor
    virtual ~ConstMemberFunctor0() {}

    /// Parameterless functor
    virtual R operator() () OVERRIDE
    {
        return (mpClient->*mpFnMemberFunction)();
    }
};

// Functor that calls member function with one parameter
template <class TObject, typename R, typename P1>
class ConstMemberFunctor1 FINAL : public IFunctor1<R, P1>
{
protected:
    ///  Pointer to class instance to call
    const TObject* const mpClient;

    /// Member function to call
    R(TObject::*mpFnMemberFunction) (P1 p1) const;

public:

    /** Ctor
    @param pInstance [in] Pointer to instance of an object to call
    @param pMemberFunction [in] Member function (with one parameter) to call
    */
    ConstMemberFunctor1(const TObject* const pInstance, R(TObject::*pMemberFunction)(P1 p1) const)
        : mpClient(pInstance), mpFnMemberFunction(pMemberFunction)
    {
    }

    /// Dtor
    virtual ~ConstMemberFunctor1() {}

    /**
    Functor

    @param p1 [in] Parameter to pass to the call target
    */
    virtual R operator() (P1 p1) OVERRIDE
    {
        return (mpClient->*mpFnMemberFunction)(p1);
    }
};

/// Functor that calls member function with two parameters
template <class TObject, typename R, typename P1, typename P2>
class ConstMemberFunctor2 FINAL : public IFunctor2<R, P1, P2>
{
protected:
    ///  Pointer to class instance to call
    const TObject* const mpClient;

    /// Member function to call
    R(TObject::*mpFnMemberFunction) (P1 p1, P2 p2) const;

public:

    /** Ctor
    @param pInstance [in] Pointer to instance of an object to call
    @param pMemberFunction [in] Member function (with two parameters) to call
    */
    ConstMemberFunctor2(const TObject* const pInstance, R(TObject::*pMemberFunction)(P1 p1, P2 p2) const)
        : mpClient(pInstance), mpFnMemberFunction(pMemberFunction)
    {
    }

    virtual ~ConstMemberFunctor2() {}

    /**
    Functor

    @param p1 [in] 1st parameter to pass to the call target
    @param p2 [in] 2nd parameter to pass to the call target
    */
    virtual R operator() (P1 p1, P2 p2) OVERRIDE
    {
        return (mpClient->*mpFnMemberFunction)(p1, p2);
    }
};

/// Functor that calls member function with three parameters
template <class TObject, typename R, typename P1, typename P2, typename P3>
class ConstMemberFunctor3 FINAL : public IFunctor3<R, P1, P2, P3>
{
protected:
    ///  Pointer to class instance to call
    const TObject* const mpClient;

    /// Member function to call
    R(TObject::*mpFnMemberFunction) (P1 p1, P2 p2, P3 p3) const;

public:

    /** Ctor
    @param pInstance [in] Pointer to instance of an object to call
    @param pMemberFunction [in] Member function (with three parameters) to call
    */
    ConstMemberFunctor3(const TObject* const pInstance, R(TObject::*pMemberFunction)(P1 p1, P2 p2, P3 p3) const)
        : mpClient(pInstance), mpFnMemberFunction(pMemberFunction)
    {
    }

    /// dtor
    virtual ~ConstMemberFunctor3() {}

    /** Functor
    @param p1 [in] 1st parameter to pass to the call target
    @param p2 [in] 2nd parameter to pass to the call target
    @param p3 [in] 3rd parameter to pass to the call target
    */
    virtual R operator() (P1 p1, P2 p2, P3 p3) OVERRIDE
    {
        return (mpClient->*mpFnMemberFunction)(p1, p2, p3);
    }
};

/// Functor that calls member function with four parameters
template <class TObject, typename R, typename P1, typename P2, typename P3, typename P4>
class ConstMemberFunctor4 FINAL : public IFunctor4<R, P1, P2, P3, P4>
{
protected:
    ///  Pointer to class instance to call
    const TObject* const mpClient;

    /// Member function to call
    R(TObject::*mpFnMemberFunction) (P1 p1, P2 p2, P3 p3, P4 p4) const;

public:

    /** Ctor
    @param pInstance [in] Pointer to instance of an object to call
    @param pMemberFunction [in] Member function (with four parameters) to call
    */
    ConstMemberFunctor4(const TObject* const pInstance, R(TObject::*pMemberFunction)(P1 p1, P2 p2, P3 p3, P4 p4) const)
        : mpClient(pInstance), mpFnMemberFunction(pMemberFunction)
    {
    }

    /// dtor
    virtual ~ConstMemberFunctor4() {}

    /** Functor
    @param p1 [in] 1st parameter to pass to the call target
    @param p2 [in] 2nd parameter to pass to the call target
    @param p3 [in] 3rd parameter to pass to the call target
    @param p4 [in] 4th parameter to pass to the call target
    */
    virtual R operator() (P1 p1, P2 p2, P3 p3, P4 p4) OVERRIDE
    {
        return (mpClient->*mpFnMemberFunction)(p1, p2, p3, p4);
    }
};

///////////

template <typename R>
class StaticFunctor0 FINAL : public IFunctor0<R>
{
protected:
    /// Function to call
    R(*mpFunction)();

public:

    /** Ctor
    @param pMemberFunction [in] Member function to call
    */
    StaticFunctor0(R(*pFunction)()) : mpFunction(pFunction) {}

    /// dtor
    virtual ~StaticFunctor0() {}

    /// Functor without parameters
    virtual R operator() () OVERRIDE
    {
        return (mpFunction)();
    }
};

template <typename R, typename P1>
class StaticFunctor1 FINAL : public IFunctor1<R, P1>
{
protected:
    /// Function to call
    R(*mpFunction)(P1);

public:
    /** Ctor
    @param pMemberFunction [in] Member function (with one parameter) to call
    */
    StaticFunctor1(R(*pFunction)(P1)) : mpFunction(pFunction) {}

    /// dtor
    virtual ~StaticFunctor1() {}

    /** Functor
    @param p1 [in] 1st parameter to pass to the call target
    */
    virtual R operator() (P1 p1) OVERRIDE
    {
        return (mpFunction)(p1);
    }
};

template <typename R, typename P1, typename P2>
class StaticFunctor2 FINAL : public IFunctor2<R, P1, P2>
{
protected:
    /// Function to call
    R(*mpFunction)(P1, P2);

public:
    /** Ctor
    @param pMemberFunction [in] Member function (with two parameters) to call
    */
    StaticFunctor2(R(*pFunction)(P1, P2)) : mpFunction(pFunction) {}

    /// dtor
    virtual ~StaticFunctor2() {}

    /** Functor
    @param p1 [in] 1st parameter to pass to the call target
    @param p2 [in] 2nd parameter to pass to the call target
    */
    virtual R operator() (P1 p1, P2 p2) OVERRIDE
    {
        return (mpFunction)(p1, p2);
    }
};

template <typename R, typename P1, typename P2, typename P3>
class StaticFunctor3 FINAL : public IFunctor3<R, P1, P2, P3>
{
protected:
    /// Function to call
    R(*mpFunction)(P1, P2, P3);

public:
    /** Ctor
    @param pMemberFunction [in] Member function (with three parameters) to call
    */
    StaticFunctor3(R(*pFunction)(P1, P2, P3)) : mpFunction(pFunction) {}

    /// dtor
    virtual ~StaticFunctor3() {}

    /** Functor
    @param p1 [in] 1st parameter to pass to the call target
    @param p2 [in] 2nd parameter to pass to the call target
    @param p3 [in] 3rd parameter to pass to the call target
    */
    virtual R operator() (P1 p1, P2 p2, P3 p3) OVERRIDE
    {
        return (mpFunction)(p1, p2, p3);
    }
};

template <typename R, typename P1, typename P2, typename P3, typename P4>
class StaticFunctor4 FINAL : public IFunctor4<R, P1, P2, P3, P4>
{
protected:
    /// Function to call
    R(*mpFunction)(P1, P2, P3, P4);

public:
    /** Ctor
    @param pMemberFunction [in] Member function (with four parameters) to call
    */
    StaticFunctor4(R(*pFunction)(P1, P2, P3, P4)) : mpFunction(pFunction) {}

    /// dtor
    virtual ~StaticFunctor4() {}

    /** Functor
    @param p1 [in] 1st parameter to pass to the call target
    @param p2 [in] 2nd parameter to pass to the call target
    @param p3 [in] 3rd parameter to pass to the call target
    @param p4 [in] 4th parameter to pass to the call target
    */
    virtual R operator() (P1 p1, P2 p2, P3 p3, P4 p4) OVERRIDE
    {
        return (mpFunction)(p1, p2, p3, p4);
    }
};

/** There is no FunctionCall0, because it would be pointless to wrap a call to parameterless functor with another */

template <typename R, typename P1>
class FunctionCall1 FINAL : public IFunctor0<R>
{
private:
    // Actual functor to call
    IFunctor1<R, P1>& mrFunctor;

    // 1st parameter to pass that function
    P1 mParam1;

public:
    /** Ctor
    @param rFunctor [in] Function to call
    @param p1 [in] 1st parameter to pass that function
    */
    FunctionCall1(IFunctor1<R, P1>& rFunctor, P1 p1) 
        : mrFunctor(rFunctor), mParam1(p1)
    {}

    /// dtor
    virtual ~FunctionCall1() {}

    /// Functor without parameters that wraps to call with parameters
    virtual R operator() () OVERRIDE
    {
        return mrFunctor(mParam1);
    }
};

template <typename R, typename P1, typename P2>
class FunctionCall2 FINAL : public IFunctor0<R>
{
private:
    // Actual functor to call
    IFunctor1<R, P1>& mrFunctor;

    // 1st parameter to pass that function
    P1 mParam1;

    // 2nd parameter to pass that function
    P2 mParam2;

public:
    /** Ctor
    @param rFunctor [in] Function to call
    @param p1 [in] 1st parameter to pass that function
    @param p2 [in] 2nd parameter to pass that function
    */
    FunctionCall2(IFunctor2<R, P1, P2>& rFunctor, P1 p1, P2 p2)
        : mrFunctor(rFunctor), mParam1(p1), mParam2(p2)
    {
    }

    /// dtor
    virtual ~FunctionCall2() {}

    /// Functor without parameters that wraps to call with parameters
    virtual R operator() () OVERRIDE
    {
        return mrFunctor(mParam1, mParam2);
    }
};

template <typename R, typename P1, typename P2, typename P3>
class FunctionCall3 FINAL : public IFunctor0<R>
{
private:
    // Actual functor to call
    IFunctor3<R, P1, P2, P3>& mrFunctor;

    // 1st parameter to pass that function
    P1 mParam1;

    // 2nd parameter to pass that function
    P2 mParam2;

    // 3rd parameter to pass that function
    P3 mParam3;

public:
    /** Ctor
    @param rFunctor [in] Function to call
    @param p1 [in] 1st parameter to pass that function
    @param p2 [in] 2nd parameter to pass that function
    @param p3 [in] 3rd parameter to pass that function
    */
    FunctionCall3(IFunctor3<R, P1, P2, P3>& rFunctor, P1 p1, P2 p2, P3 p3)
        : mrFunctor(rFunctor), mParam1(p1), mParam2(p2), mParam3(p3)
    {
    }

    /// dtor
    virtual ~FunctionCall3() {}

    /// Functor without parameters that wraps to call with parameters
    virtual R operator() () OVERRIDE
    {
        return mrFunctor(mParam1, mParam2, mParam3);
    }
};

template <typename R, typename P1, typename P2, typename P3, typename P4>
class FunctionCall4 FINAL : public IFunctor0<R>
{
private:
    // Actual functor to call
    IFunctor4<R, P1, P2, P3, P4>& mrFunctor;

    // 1st parameter to pass that function
    P1 mParam1;

    // 2nd parameter to pass that function
    P2 mParam2;

    // 3rd parameter to pass that function
    P3 mParam3;

    // 4th parameter to pass that function
    P4 mParam4;

public:
    /** Ctor
    @param rFunctor [in] Function to call
    @param p1 [in] 1st parameter to pass that function
    @param p2 [in] 2nd parameter to pass that function
    @param p3 [in] 3rd parameter to pass that function
    @param p4 [in] 4th parameter to pass that function
    */
    FunctionCall4(IFunctor4<R, P1, P2, P3, P4>& rFunctor, P1 p1, P2 p2, P3 p3, P4 p4)
        : mrFunctor(rFunctor), mParam1(p1), mParam2(p2), mParam3(p3), mParam4(p4)
    {
    }

    /// dtor
    virtual ~FunctionCall4() {}

    /// Functor without parameters that wraps to call with parameters
    virtual R operator() () OVERRIDE
    {
        return mrFunctor(mParam1, mParam2, mParam3, mParam4);
    }
};

template <typename R>
class VoidFunctionCallWithReturn0 FINAL : public IFunctor0<void>
{
private:
    // Actual functor to call
    IFunctor0<R>& mrFunctor;

    // Stored result value of the function call
    R mReturnValue;

public:
    /** Ctor
    @param rFunctor [in] Function to call
    */
    VoidFunctionCallWithReturn0(IFunctor0<R>& rFunctor)
        : mrFunctor(rFunctor)
    {
    }

    /// dtor
    virtual ~VoidFunctionCallWithReturn0() {}

    /// Functor without parameters that wraps to call with parameters
    virtual void operator() () OVERRIDE
    {
        mReturnValue = mrFunctor();
    }

    /// Gets return value of the call
    inline R getReturnValue()
    {
        return mReturnValue;
    }
};

template <typename R, typename P1>
class VoidFunctionCallWithReturn1 FINAL : public IFunctor0<void>
{
private:
    // Actual functor to call
    IFunctor1<R, P1>& mrFunctor;

    // Stored result value of the function call
    R mReturnValue;

    // 1st parameter to pass that function
    P1 mParam1;

public:
    /** Ctor
    @param rFunctor [in] Function to call
    @param p1 [in] 1st parameter to pass that function
    */
    VoidFunctionCallWithReturn1(IFunctor1<R, P1>& rFunctor, P1 p1)
        : mrFunctor(rFunctor), mParam1(p1)
    {
    }

    /// dtor
    virtual ~VoidFunctionCallWithReturn1() {}

    /// Functor without parameters that wraps to call with parameters
    virtual void operator() () OVERRIDE
    {
        mReturnValue = mrFunctor(mParam1);
    }

    /// Gets return value of the call
    inline R getReturnValue()
    {
        return mReturnValue;
    }
};

template <typename R, typename P1, typename P2>
class VoidFunctionCallWithReturn2 FINAL : public IFunctor0<void>
{
private:
    // Actual functor to call
    IFunctor1<R, P1>& mrFunctor;

    // Stored result value of the function call
    R mReturnValue;

    // 1st parameter to pass that function
    P1 mParam1;

    // 2nd parameter to pass that function
    P2 mParam2;

public:
    /** Ctor
    @param rFunctor [in] Function to call
    @param p1 [in] 1st parameter to pass that function
    @param p2 [in] 2nd parameter to pass that function
    */
    VoidFunctionCallWithReturn2(IFunctor2<R, P1, P2>& rFunctor, P1 p1, P2 p2)
        : mrFunctor(rFunctor), mParam1(p1), mParam2(p2)
    {
    }

    /// dtor
    virtual ~VoidFunctionCallWithReturn2() {}

    /// Functor without parameters that wraps to call with parameters
    virtual void operator() () OVERRIDE
    {
        mReturnValue = mrFunctor(mParam1, mParam2);
    }

    /// Gets return value of the call
    inline R getReturnValue()
    {
        return mReturnValue;
    }
};

template <typename R, typename P1, typename P2, typename P3>
class VoidFunctionCallWithReturn3 FINAL : public IFunctor0<void>
{
private:
    // Actual functor to call
    IFunctor3<R, P1, P2, P3>& mrFunctor;

    // Stored result value of the function call
    R mReturnValue;

    // 1st parameter to pass that function
    P1 mParam1;

    // 2nd parameter to pass that function
    P2 mParam2;

    // 3rd parameter to pass that function
    P3 mParam3;

public:
    /** Ctor
    @param rFunctor [in] Function to call
    @param p1 [in] 1st parameter to pass that function
    @param p2 [in] 2nd parameter to pass that function
    @param p3 [in] 3rd parameter to pass that function
    */
    VoidFunctionCallWithReturn3(IFunctor3<R, P1, P2, P3>& rFunctor, P1 p1, P2 p2, P3 p3)
        : mrFunctor(rFunctor), mParam1(p1), mParam2(p2), mParam3(p3)
    {
    }

    /// dtor
    virtual ~VoidFunctionCallWithReturn3() {}

    /// Functor without parameters that wraps to call with parameters
    virtual void operator() () OVERRIDE
    {
        mReturnValue = mrFunctor(mParam1, mParam2, mParam3);
    }

    /// Gets return value of the call
    inline R getReturnValue()
    {
        return mReturnValue;
    }
};

template <typename R, typename P1, typename P2, typename P3, typename P4>
class VoidFunctionCallWithReturn4 FINAL : public IFunctor0<void>
{
private:
    // Actual functor to call
    IFunctor4<R, P1, P2, P3, P4>& mrFunctor;

    // Stored result value of the function call
    R mReturnValue;

    // 1st parameter to pass that function
    P1 mParam1;

    // 2nd parameter to pass that function
    P2 mParam2;

    // 3rd parameter to pass that function
    P3 mParam3;

    // 4th parameter to pass that function
    P4 mParam4;

public:
    /** Ctor
    @param rFunctor [in] Function to call
    @param p1 [in] 1st parameter to pass that function
    @param p2 [in] 2nd parameter to pass that function
    @param p3 [in] 3rd parameter to pass that function
    @param p4 [in] 4th parameter to pass that function
    */
    VoidFunctionCallWithReturn4(IFunctor4<R, P1, P2, P3, P3>& rFunctor, P1 p1, P2 p2, P3 p3, P4 p4)
        : mrFunctor(rFunctor), mParam1(p1), mParam2(p2), mParam3(p3), mParam4(p4)
    {
    }

    /// dtor
    virtual ~VoidFunctionCallWithReturn4() {}

    /// Functor without parameters that wraps to call with parameters
    virtual void operator() () OVERRIDE
    {
        mReturnValue = mrFunctor(mParam1, mParam2, mParam3, mParam4);
    }

    /// Gets return value of the call
    inline R getReturnValue()
    {
        return mReturnValue;
    }
};

template <class TObject, typename R>
inline MemberFunctor0<TObject, R> MakeFunctor(const TObject* pInstance, R(TObject::*pMemberFunction)())
{
    return MemberFunctor0<TObject, R>(pInstance, pMemberFunction);
}

template <class TObject, typename R, typename P1>
inline MemberFunctor1<TObject, R, P1> MakeFunctor(const TObject* pInstance, R(TObject::*pMemberFunction)(P1 p1))
{
    return MemberFunctor1<TObject, R, P1>(pInstance, pMemberFunction);
}

template <class TObject, typename R, typename P1, typename P2>
inline MemberFunctor2<TObject, R, P1, P2> MakeFunctor(const TObject* pInstance, R(TObject::*pMemberFunction)(P1 p1, P2 p2))
{
    return MemberFunctor2<TObject, R, P1, P2>(pInstance, pMemberFunction);
}

template <class TObject, typename R, typename P1, typename P2, typename P3>
inline MemberFunctor3<TObject, R, P1, P2, P3> MakeFunctor(
    const TObject* pInstance, R(TObject::*pMemberFunction)(P1 p1, P2 p2, P3 p3))
{
    return MemberFunctor3<TObject, R, P1, P2, P3>(pInstance, pMemberFunction);
}

template <class TObject, typename R, typename P1, typename P2, typename P3, typename P4>
inline MemberFunctor4<TObject, R, P1, P2, P3, P4> MakeFunctor(
    const TObject* pInstance, R(TObject::*pMemberFunction)(P1 p1, P2 p2, P3 p3, P4 p4))
{
    return MemberFunctor4<TObject, R, P1, P2, P3, P4>(pInstance, pMemberFunction);
}

template <class TObject, typename R>
inline ConstMemberFunctor0<TObject, R> MakeFunctor(const TObject* const pInstance, R(TObject::*pMemberFunction)() const)
{
    return ConstMemberFunctor0<TObject, R>(pInstance, pMemberFunction);
}

template <class TObject, typename R, typename P1>
inline ConstMemberFunctor1<TObject, R, P1> MakeFunctor(
    const TObject* const pInstance, R(TObject::*pMemberFunction)(P1 p1) const)
{
    return ConstMemberFunctor1<TObject, R, P1>(pInstance, pMemberFunction);
}

template <class TObject, typename R, typename P1, typename P2>
inline ConstMemberFunctor2<TObject, R, P1, P2> MakeFunctor(
    const TObject* const pInstance, R(TObject::*pMemberFunction)(P1 p1, P2 p2) const)
{
    return ConstMemberFunctor2<TObject, R, P1, P2>(pInstance, pMemberFunction);
}

template <class TObject, typename R, typename P1, typename P2, typename P3>
inline ConstMemberFunctor3<TObject, R, P1, P2, P3> MakeFunctor(
    const TObject* const pInstance, R(TObject::*pMemberFunction)(P1 p1, P2 p2, P3 p3) const)
{
    return ConstMemberFunctor3<TObject, R, P1, P2, P3>(pInstance, pMemberFunction);
}

template <class TObject, typename R, typename P1, typename P2, typename P3, typename P4>
inline ConstMemberFunctor4<TObject, R, P1, P2, P3, P4> MakeFunctor(
    const TObject* const pInstance, R(TObject::*pMemberFunction)(P1 p1, P2 p2, P3 p3, P3 p4) const)
{
    return ConstMemberFunctor4<TObject, R, P1, P2, P3, P4>(pInstance, pMemberFunction);
}

template <typename R>
inline StaticFunctor0<R> MakeFunctor(R(*pFunction)())
{
    return StaticFunctor0<R>(pFunction);
}

template <typename R, typename P1>
inline StaticFunctor1<R, P1> MakeFunctor(R(*pFunction)(P1))
{
    return StaticFunctor1<R, P1>(pFunction);
}

template <typename R, typename P1, typename P2>
inline StaticFunctor2<R, P1, P2> MakeFunctor(R(*pFunction)(P1, P2))
{
    return StaticFunctor2<R, P1, P2>(pFunction);
}

template <typename R, typename P1, typename P2, typename P3>
inline StaticFunctor3<R, P1, P2, P3> MakeFunctor(R(*pFunction)(P1, P2, P3))
{
    return StaticFunctor3<R, P1, P2, P3>(pFunction);
}

template <typename R, typename P1, typename P2, typename P3, typename P4>
inline StaticFunctor4<R, P1, P2, P3, P4> MakeFunctor(R(*pFunction)(P1, P2, P3, P4))
{
    return StaticFunctor4<R, P1, P2, P3, P4>(pFunction);
}

template <typename R>
inline IFunctor0<R> MakeFunctionCall(IFunctor0<R>& rFunctor)
{
    return IFunctor0<R>(rFunctor);
}

template <typename R, typename P1>
inline FunctionCall1<R, P1> MakeFunctionCall(IFunctor1<R, P1>& rFunctor, P1 p1)
{
    return FunctionCall1<R, P1>(rFunctor, p1);
}

template <typename R, typename P1, typename P2>
inline FunctionCall2<R, P1, P2> MakeFunctionCall(IFunctor2<R, P1, P2>& rFunctor, P1 p1, P2 p2)
{
    return FunctionCall2<R, P1, P2>(rFunctor, p1, p2);
}

template <typename R, typename P1, typename P2, typename P3>
inline FunctionCall3<R, P1, P2, P3> MakeFunctionCall(IFunctor3<R, P1, P2, P3>& rFunctor, P1 p1, P2 p2, P3 p3)
{
    return FunctionCall3<R, P1, P2, P3>(rFunctor, p1, p2, p3);
}

template <typename R, typename P1, typename P2, typename P3, typename P4>
inline FunctionCall4<R, P1, P2, P3, P4> MakeFunctionCall(IFunctor4<R, P1, P2, P3, P4>& rFunctor, P1 p1, P2 p2, P3 p3, P4 p4)
{
    return FunctionCall4<R, P1, P2, P3, P4>(rFunctor, p1, p2, p3, p4);
}

template <typename R>
inline VoidFunctionCallWithReturn0<R> MakeVoidFunctionCallWithReturn(IFunctor0<R>& rFunctor)
{
    return VoidFunctionCallWithReturn0<R>(rFunctor);
}

template <typename R, typename P1>
inline VoidFunctionCallWithReturn1<R, P1> MakeVoidFunctionCallWithReturn(IFunctor1<R, P1>& rFunctor, P1 p1)
{
    return VoidFunctionCallWithReturn1<R, P1>(rFunctor, p1);
}

template <typename R, typename P1, typename P2>
inline VoidFunctionCallWithReturn2<R, P1, P2> MakeVoidFunctionCallWithReturn(IFunctor2<R, P1, P2>& rFunctor, P1 p1, P2 p2)
{
    return VoidFunctionCallWithReturn2<R, P1, P2>(rFunctor, p1, p2);
}

template <typename R, typename P1, typename P2, typename P3>
inline VoidFunctionCallWithReturn3<R, P1, P2, P3> MakeVoidFunctionCallWithReturn(
    IFunctor3<R, P1, P2, P3>& rFunctor, P1 p1, P2 p2, P3 p3)
{
    return VoidFunctionCallWithReturn3<R, P1, P2, P3>(rFunctor, p1, p2, p3);
}

template <typename R, typename P1, typename P2, typename P3, typename P4>
inline VoidFunctionCallWithReturn4<R, P1, P2, P3, P4> MakeVoidFunctionCallWithReturn(
    IFunctor4<R, P1, P2, P3, P4>& rFunctor, P1 p1, P2 p2, P3 p3, P4 p4)
{
    return VoidFunctionCallWithReturn4<R, P1, P2, P3, P4>(rFunctor, p1, p2, p3, p4);
}

} // namespace whiteboard
