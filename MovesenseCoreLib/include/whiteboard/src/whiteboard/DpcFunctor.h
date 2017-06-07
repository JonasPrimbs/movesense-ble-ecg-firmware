#pragma once
// Copyright(c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/Dpc.h"
#include "whiteboard/ExecutionContext.h"

namespace whiteboard
{

/** Type safe and scoped implementation of Dpc method call.
 * Can be used for easily queuing a class method call in wanted context.
 */
class WB_API DpcFunctor
{
public:
    /** Queues new DPC member function that is executed only once in the target execution context.
     *
     * @note This function uses dynamic memory, which use should be avoided in ESW production code
     */
    template <class Ty>
    static void queueOnce(const whiteboard::ExecutionContextId id, Ty* pInstance, void (Ty::*pFnMemberFunction)())
    {
        MemberFunctor0<Ty, void> functor(pInstance, pFnMemberFunction);
        asyncCall(id, functor);
    }

    /** Queues new DPC member function that is executed only once in the target execution context.
    *
    * @note This function uses dynamic memory, which use should be avoided in ESW production code
    */
    template <class Ty>
    static void queueOnce(
        const whiteboard::ExecutionContextId id, Whiteboard& rWhiteboard, Ty* pInstance, void (Ty::*pFnMemberFunction)())
    {
        MemberFunctor0<Ty, void> functor(pInstance, pFnMemberFunction);
        asyncCall(id, rWhiteboard, functor);
    }

    /** Queues new static functor DPC that is executed only once in the target execution context.
    *
    * @note This function uses dynamic memory, which use should be avoided in ESW production code
    */
    template <class Tp>
    static void queueOnce(const whiteboard::ExecutionContextId id, void(*pFunction)(Tp), Tp param)
    {
        StaticFunctionCall1<void, Tp> functor(pFunction, param);
        asyncCall(id, functor);
    }

    /** Queues new static functor DPC that is executed only once in the target execution context.
     *
     * @note This function uses dynamic memory, which use should be avoided in ESW production code
     */
    template <class Tp>
    static void queueOnce(const whiteboard::ExecutionContextId id, Whiteboard& rWhiteboard, void (*pFunction)(Tp), Tp param)
    {
        StaticFunctionCall1<void, Tp> functor(pFunction, param);
        asyncCall(id, rWhiteboard, functor);
    }

    /** Queues new DPC member function that is executed only once in the target execution context. */
    template <class Ty>
    static void syncQueueOnce(const whiteboard::ExecutionContextId id, Ty* pInstance, void (Ty::*pFnMemberFunction)())
    {
        MemberFunctor0<Ty, void> functor(pInstance, pFnMemberFunction);
        SyncDpcFunctionCallWrapper call(id, functor);
    }

    /** Queues new DPC member function that is executed only once in the target execution context. */
    template <class Ty>
    static void syncQueueOnce(
        const whiteboard::ExecutionContextId id, Whiteboard& rWhiteboard, Ty* pInstance, void (Ty::*pFnMemberFunction)())
    {
        MemberFunctor0<Ty, void> functor(pInstance, pFnMemberFunction);
        SyncDpcFunctionCallWrapper call(id, rWhiteboard, functor);
    }

    /** Queues new DPC member function that is executed only once in the target execution context. */
    template <typename Tr, class Ty>
    static Tr syncQueueOnce(const whiteboard::ExecutionContextId id, Ty* pInstance, Tr (Ty::*pFnMemberFunction)())
    {
        MemberFunctor0<Ty, Tr> functor(pInstance, pFnMemberFunction);
        VoidFunctionCallWithReturn0<Tr> functionCall(functor);
        SyncDpcFunctionCallWrapper call(id, functionCall);
        return functionCall.getReturnValue();
    }

    /** Queues new DPC member function that is executed only once in the target execution context. */
    template <typename Tr, class Ty>
    static Tr syncQueueOnce(const whiteboard::ExecutionContextId id, Whiteboard& rWhiteboard, Ty* pInstance, Tr(Ty::*pFnMemberFunction)())
    {
        MemberFunctor0<Ty, Tr> functor(pInstance, pFnMemberFunction);
        VoidFunctionCallWithReturn0<Tr> functionCall(functor);
        SyncDpcFunctionCallWrapper call(id, rWhiteboard, functionCall);
        return functionCall.getReturnValue();
    }

    /** Queues new DPC member function that is executed only once in the target execution context. */
    template <class Ty, typename Tp>
    static void syncQueueOnce(const whiteboard::ExecutionContextId id, Ty* pInstance, void (Ty::*pFnMemberFunction)(Tp), Tp param)
    {
        MemberFunctor1<Ty, void, Tp> functor(pInstance, pFnMemberFunction);
        FunctionCall1<void, Tp> functionCall(functor, param);
        SyncDpcFunctionCallWrapper call(id, functionCall);
    }

    /** Queues new DPC member function that is executed only once in the target execution context. */
    template <class Ty, typename Tp>
    static void syncQueueOnce(
        const whiteboard::ExecutionContextId id, Whiteboard& rWhiteboard, Ty* pInstance, void (Ty::*pFnMemberFunction)(Tp), Tp param)
    {
        MemberFunctor1<Ty, void, Tp> functor(pInstance, pFnMemberFunction);
        FunctionCall1<void, Tp> functionCall(functor, param);
        SyncDpcFunctionCallWrapper call(id, rWhiteboard, functionCall);
    }

    /** Queues new DPC member function that is executed only once in the target execution context. */
    template <typename Tr, class Ty, typename Tp>
    static Tr syncQueueOnce(
        const whiteboard::ExecutionContextId id, Ty* pInstance, Tr(Ty::*pFnMemberFunction)(Tp), Tp param)
    {
        MemberFunctor1<Ty, Tr, Tp> functor(pInstance, pFnMemberFunction);
        VoidFunctionCallWithReturn1<Tr, Tp> functionCall(functor, param);
        SyncDpcFunctionCallWrapper call(id, functionCall);
        return functionCall.getReturnValue();
    }

    /** Queues new DPC member function that is executed only once in the target execution context. */
    template <typename Tr, class Ty, typename Tp>
    static Tr syncQueueOnce(
        const whiteboard::ExecutionContextId id, Whiteboard& rWhiteboard, Ty* pInstance, Tr(Ty::*pFnMemberFunction)(Tp), Tp param)
    {
        MemberFunctor1<Ty, Tr, Tp> functor(pInstance, pFnMemberFunction);
        VoidFunctionCallWithReturn1<Tr, Tp> functionCall(functor, param);
        SyncDpcFunctionCallWrapper call(id, rWhiteboard, functionCall);
        return functionCall.getReturnValue();
    }

    /** Queues new DPC static function that is executed only once in the target execution context. */
    template <class Tp>
    static void syncQueueOnce(const whiteboard::ExecutionContextId id, void (*pFunction)(Tp), Tp param)
    {
        StaticFunctor1<void, Tp> functor(pFunction);
        FunctionCall1<void, Tp> functionCall(functor, param);
        SyncDpcFunctionCallWrapper call(id, functionCall);
    }

    /** Queues new DPC static function that is executed only once in the target execution context. */
    template <class Tp>
    static void syncQueueOnce(const whiteboard::ExecutionContextId id, Whiteboard& rWhiteboard, void(*pFunction)(Tp), Tp param)
    {
        StaticFunctor1<void, Tp> functor(pFunction);
        FunctionCall1<void, Tp> functionCall(functor, param);
        SyncDpcFunctionCallWrapper call(id, rWhiteboard, functionCall);
    }

    /** Queues new DPC static function that is executed only once in the target execution context. */
    template <typename Tr, class Tp>
    static Tr syncQueueOnce(const whiteboard::ExecutionContextId id, Tr (*pFunction)(Tp), Tp param)
    {
        StaticFunctor1<Tr, Tp> functor(pFunction);
        VoidFunctionCallWithReturn1<Tr, Tp> functionCall(functor, param);
        SyncDpcFunctionCallWrapper call(id, functionCall);
        return functionCall.getReturnValue();
    }

    /** Queues new DPC static function that is executed only once in the target execution context. */
    template <typename Tr, class Tp>
    static Tr syncQueueOnce(const whiteboard::ExecutionContextId id, Whiteboard& rWhiteboard, Tr(*pFunction)(Tp), Tp param)
    {
        StaticFunctor1<Tr, Tp> functor(pFunction);
        VoidFunctionCallWithReturn1<Tr, Tp> functionCall(functor, param);
        SyncDpcFunctionCallWrapper call(id, rWhiteboard, functionCall);
        return functionCall.getReturnValue();
    }

public:
    /** Construct a new scoped Dpc with execution context, class instance and method call. */
    template <class Ty> DpcFunctor(const whiteboard::ExecutionContextId id, Ty* pInstance, void (Ty::*pFnMemberFunction)())
    {
        FunctorImpl<Ty>* callTarget = new FunctorImpl<Ty>(pInstance, pFnMemberFunction);
        callTarget->mpDpc = mpDpc = Dpc::allocate(id, *callTarget);
        WB_ASSERT(NULL != mpDpc);
    }

    /// Trigger the async callback
    void queue(const bool isIsr) { mpDpc->queue(isIsr); }

    /// Deallocate DPC from system
    void deallocate()
    {
        if (mpDpc)
        {
            IDpcFunctor* ptr = mpDpc->functorPtr();
            mpDpc->deallocate();
            mpDpc = NULL;
            delete ptr;
        }
    }

    virtual ~DpcFunctor() { this->deallocate(); }

private:
    DpcFunctor() DELETED;
    DpcFunctor(const DpcFunctor&) DELETED;
    DpcFunctor& operator=(const DpcFunctor&) DELETED;

    template <class TObject> struct FunctorImpl FINAL : public IDpcFunctor
    {
    public:
        TObject* mpClient;
        void (TObject::*mpFnMemberFunction) ();
        whiteboard::Dpc* mpDpc;

    public:
        FunctorImpl(TObject* pInstance, void (TObject::*memberFunction)())
            : mpClient(pInstance), mpFnMemberFunction(memberFunction), mpDpc(NULL)
        {
        }

        ~FunctorImpl() OVERRIDE {}

        void operator() () OVERRIDE
        {
            WB_ASSERT(mpDpc->isThisThread(WbThreadGetCurrent()));
            return (mpClient->*mpFnMemberFunction)();
        }
    };

    template <typename FunctionCallType>
    class AsyncDpcFunctionCallWrapper FINAL : public IDpcFunctor
    {
    public:
        FunctionCallType mFunctionCall;
        whiteboard::Dpc* mpDpc;

    public:
        // TODO: handling for calls within the same thread context
        AsyncDpcFunctionCallWrapper(ExecutionContextId executionContextId, const FunctionCallType& rFunctionCall) :
            mFunctionCall(rFunctionCall),
            mpDpc(Dpc::allocate(executionContextId, *this))
        {
            WB_ASSERT(NULL != mpDpc);
            asyncCall();
        }

        AsyncDpcFunctionCallWrapper(ExecutionContextId executionContextId, Whiteboard& rWhiteboard, const FunctionCallType& rFunctionCall) :
            mFunctionCall(rFunctionCall),
            mpDpc(Dpc::allocate(executionContextId, *this, rWhiteboard))
        {
            WB_ASSERT(NULL != mpDpc);
            asyncCall();
        }

        ~AsyncDpcFunctionCallWrapper()
        {
            mpDpc->deallocate();
        }

        // DPC callback
        void operator () () OVERRIDE
        {
            WB_DEBUG_ASSERT(mpDpc->isThisThread(WbThreadGetCurrent()));
            mFunctionCall();
            delete this;
        }

    private:
        inline void asyncCall()
        {
            mpDpc->queue(false);
        }
    };

    // Async queueOnce needs to allocate memory for duration of the call.
    // Having separate functor and function call implementation is difficult,
    // so here is one that combines them together.
    template <typename R, typename P1>
    class StaticFunctionCall1 FINAL : public IFunctor0<R>
    {
    private:
        /// Function to call
        R(*mpFunction)(P1);

        // 1st parameter to pass that function
        P1 mParam1;

    public:
        /** Ctor
        @param pFunction [in] Function to call
        @param p1 [in] 1st parameter to pass that functio
        */
        StaticFunctionCall1(R(*pFunction)(P1), P1 p1)
            : mpFunction(pFunction), mParam1(p1)
        {
        }

        /// dtor
        virtual ~StaticFunctionCall1() {}

        /// Functor without parameters that wraps to call with parameters
        virtual R operator() () OVERRIDE
        {
            return (mpFunction)(mParam1);
        }
    };

    template <typename FunctionCallType>
    static void asyncCall(ExecutionContextId executionContextId, const FunctionCallType& rFunctionCall)
    {
        new AsyncDpcFunctionCallWrapper<FunctionCallType>(executionContextId, rFunctionCall);
    }

    template <typename FunctionCallType>
    static void asyncCall(ExecutionContextId executionContextId, Whiteboard& rWhiteboard, const FunctionCallType& rFunctionCall)
    {
        new AsyncDpcFunctionCallWrapper<FunctionCallType>(executionContextId, rWhiteboard, rFunctionCall);
    }

    class SyncDpcFunctionCallWrapper FINAL : public IDpcFunctor
    {
    public:
        IDpcFunctor& mrFunctionCall;
        whiteboard::Dpc* mpDpc;
        WbSemaphoreHandle mSemaphore;

    public:
        SyncDpcFunctionCallWrapper(ExecutionContextId executionContextId, IDpcFunctor& rFunctionCall) :
            mrFunctionCall(rFunctionCall)
        {
            if (ExecutionContext::isCurrentThread(executionContextId))
            {
                mrFunctionCall();
            }
            else
            {
                mpDpc = Dpc::allocate(executionContextId, *this);
                WB_ASSERT(NULL != mpDpc);
                mSemaphore = WbSemaphoreCreate(0, 1);
                mpDpc->queue(false);
                WbSemaphoreWait(mSemaphore);
                mpDpc->deallocate();
                WbSemaphoreDelete(mSemaphore);
            }
        }

        SyncDpcFunctionCallWrapper(ExecutionContextId executionContextId, Whiteboard& rWhiteboard, IDpcFunctor& rFunctionCall) :
            mrFunctionCall(rFunctionCall)
        {
            if (ExecutionContext::isCurrentThread(executionContextId, rWhiteboard))
            {
                mrFunctionCall();
            }
            else
            {
                mpDpc = Dpc::allocate(executionContextId, *this, rWhiteboard);
                WB_ASSERT(NULL != mpDpc);
                mSemaphore = WbSemaphoreCreate(0, 1);
                mpDpc->queue(false);
                WbSemaphoreWait(mSemaphore);
                mpDpc->deallocate();
                WbSemaphoreDelete(mSemaphore);
            }
        }

        ~SyncDpcFunctionCallWrapper()
        {
        }

        // DPC callback
        void operator () () OVERRIDE
        {
            WB_DEBUG_ASSERT(mpDpc->isThisThread(WbThreadGetCurrent()));
            mrFunctionCall();
            WbSemaphoreRelease(mSemaphore);
        }
    };

protected:

    whiteboard::Dpc* mpDpc;
};

} // namespace whiteboard
