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
    static void queueOnce(const whiteboard::ExecutionContextId id, Ty* pInstance, void (Ty::*pFnMemberFunction)(), bool delayed = false)
    {
        MemberFunctor0<Ty, void> functor(pInstance, pFnMemberFunction);
        asyncCall(id, functor, delayed);
    }

    /** Queues new DPC member function that is executed only once in the target execution context.
    *
    * @note This function uses dynamic memory, which use should be avoided in ESW production code
    */
    template <class Ty>
    static void queueOnce(
        const whiteboard::ExecutionContextId id, Whiteboard& rWhiteboard, Ty* pInstance, void (Ty::*pFnMemberFunction)(), bool delayed = false)
    {
        MemberFunctor0<Ty, void> functor(pInstance, pFnMemberFunction);
        asyncCall(id, rWhiteboard, functor, delayed);
    }

    /** Queues new static functor DPC that is executed only once in the target execution context.
    *
    * @note This function uses dynamic memory, which use should be avoided in ESW production code
    */
    template <class Tp>
    static void queueOnce(const whiteboard::ExecutionContextId id, void(*pFunction)(Tp), Tp param, bool delayed = false)
    {
        StaticFunctionCall1<void, Tp> functor(pFunction, param);
        asyncCall(id, functor, delayed);
    }

    /** Queues new static functor DPC that is executed only once in the target execution context.
     *
     * @note This function uses dynamic memory, which use should be avoided in ESW production code
     */
    template <class Tp>
    static void queueOnce(const whiteboard::ExecutionContextId id, Whiteboard& rWhiteboard, void (*pFunction)(Tp), Tp param, bool delayed = false)
    {
        StaticFunctionCall1<void, Tp> functor(pFunction, param);
        asyncCall(id, rWhiteboard, functor, delayed);
    }

    /** Queues new functor DPC that is executed only once in the target execution context.
    *
    * @note This function uses dynamic memory, which use should be avoided in ESW production code
    */
    static void queueOnce(const whiteboard::ExecutionContextId id, IDpcFunctor& rFunctor, bool delayed = false)
    {
        MemberFunctor0<IDpcFunctor, void> functor(&rFunctor, &IDpcFunctor::operator());
        asyncCall(id, functor, delayed);
    }

    /** Queues new functor DPC that is executed only once in the target execution context.
    *
    * @note This function uses dynamic memory, which use should be avoided in ESW production code
    */
    static void queueOnce(const whiteboard::ExecutionContextId id, Whiteboard& rWhiteboard, IDpcFunctor& rFunctor, bool delayed = false)
    {
        MemberFunctor0<IDpcFunctor, void> functor(&rFunctor, &IDpcFunctor::operator());
        asyncCall(id, rWhiteboard, functor, delayed);
    }
    
    /** Queues new DPC member function that is executed only once in the target execution context. */
    template <class Ty>
    static void syncQueueOnce(const whiteboard::ExecutionContextId id, Ty* pInstance, void (Ty::*pFnMemberFunction)(), bool delayed = false)
    {
        MemberFunctor0<Ty, void> functor(pInstance, pFnMemberFunction);
        SyncDpcFunctionCallWrapper call(id, functor, delayed);
    }

    /** Queues new DPC member function that is executed only once in the target execution context. */
    template <class Ty>
    static void syncQueueOnce(
        const whiteboard::ExecutionContextId id, Whiteboard& rWhiteboard, Ty* pInstance, void (Ty::*pFnMemberFunction)(), bool delayed = false)
    {
        MemberFunctor0<Ty, void> functor(pInstance, pFnMemberFunction);
        SyncDpcFunctionCallWrapper call(id, rWhiteboard, functor, delayed);
    }

    /** Queues new DPC member function that is executed only once in the target execution context. */
    template <typename Tr, class Ty>
    static Tr syncQueueOnce(const whiteboard::ExecutionContextId id, Ty* pInstance, Tr (Ty::*pFnMemberFunction)(), bool delayed = false)
    {
        MemberFunctor0<Ty, Tr> functor(pInstance, pFnMemberFunction);
        VoidFunctionCallWithReturn0<Tr> functionCall(functor);
        SyncDpcFunctionCallWrapper call(id, functionCall, delayed);
        return functionCall.getReturnValue();
    }

    /** Queues new DPC member function that is executed only once in the target execution context. */
    template <typename Tr, class Ty>
    static Tr syncQueueOnce(const whiteboard::ExecutionContextId id, Whiteboard& rWhiteboard, Ty* pInstance, Tr(Ty::*pFnMemberFunction)(), bool delayed = false)
    {
        MemberFunctor0<Ty, Tr> functor(pInstance, pFnMemberFunction);
        VoidFunctionCallWithReturn0<Tr> functionCall(functor);
        SyncDpcFunctionCallWrapper call(id, rWhiteboard, functionCall, delayed);
        return functionCall.getReturnValue();
    }

    /** Queues new DPC member function that is executed only once in the target execution context. */
    template <class Ty, typename Tp>
    static void syncQueueOnce(const whiteboard::ExecutionContextId id, Ty* pInstance, void (Ty::*pFnMemberFunction)(Tp), Tp param, bool delayed = false)
    {
        MemberFunctor1<Ty, void, Tp> functor(pInstance, pFnMemberFunction);
        FunctionCall1<void, Tp> functionCall(functor, param);
        SyncDpcFunctionCallWrapper call(id, functionCall, delayed);
    }

    /** Queues new DPC member function that is executed only once in the target execution context. */
    template <class Ty, typename Tp>
    static void syncQueueOnce(
        const whiteboard::ExecutionContextId id, Whiteboard& rWhiteboard, Ty* pInstance, void (Ty::*pFnMemberFunction)(Tp), Tp param, bool delayed = false)
    {
        MemberFunctor1<Ty, void, Tp> functor(pInstance, pFnMemberFunction);
        FunctionCall1<void, Tp> functionCall(functor, param);
        SyncDpcFunctionCallWrapper call(id, rWhiteboard, functionCall, delayed);
    }

    /** Queues new DPC member function that is executed only once in the target execution context. */
    template <typename Tr, class Ty, typename Tp>
    static Tr syncQueueOnce(
        const whiteboard::ExecutionContextId id, Ty* pInstance, Tr(Ty::*pFnMemberFunction)(Tp), Tp param, bool delayed = false)
    {
        MemberFunctor1<Ty, Tr, Tp> functor(pInstance, pFnMemberFunction);
        VoidFunctionCallWithReturn1<Tr, Tp> functionCall(functor, param);
        SyncDpcFunctionCallWrapper call(id, functionCall, delayed);
        return functionCall.getReturnValue();
    }

    /** Queues new DPC member function that is executed only once in the target execution context. */
    template <typename Tr, class Ty, typename Tp>
    static Tr syncQueueOnce(
        const whiteboard::ExecutionContextId id, Whiteboard& rWhiteboard, Ty* pInstance, Tr(Ty::*pFnMemberFunction)(Tp), Tp param, bool delayed = false)
    {
        MemberFunctor1<Ty, Tr, Tp> functor(pInstance, pFnMemberFunction);
        VoidFunctionCallWithReturn1<Tr, Tp> functionCall(functor, param);
        SyncDpcFunctionCallWrapper call(id, rWhiteboard, functionCall, delayed);
        return functionCall.getReturnValue();
    }

    /** Queues new DPC static function that is executed only once in the target execution context. */
    template <class Tp>
    static void syncQueueOnce(const whiteboard::ExecutionContextId id, void (*pFunction)(Tp), Tp param, bool delayed = false)
    {
        StaticFunctor1<void, Tp> functor(pFunction);
        FunctionCall1<void, Tp> functionCall(functor, param);
        SyncDpcFunctionCallWrapper call(id, functionCall, delayed);
    }

    /** Queues new DPC static function that is executed only once in the target execution context. */
    template <class Tp>
    static void syncQueueOnce(const whiteboard::ExecutionContextId id, Whiteboard& rWhiteboard, void(*pFunction)(Tp), Tp param, bool delayed = false)
    {
        StaticFunctor1<void, Tp> functor(pFunction);
        FunctionCall1<void, Tp> functionCall(functor, param);
        SyncDpcFunctionCallWrapper call(id, rWhiteboard, functionCall, delayed);
    }

    /** Queues new DPC static function that is executed only once in the target execution context. */
    template <typename Tr, class Tp>
    static Tr syncQueueOnce(const whiteboard::ExecutionContextId id, Tr (*pFunction)(Tp), Tp param, bool delayed = false)
    {
        StaticFunctor1<Tr, Tp> functor(pFunction);
        VoidFunctionCallWithReturn1<Tr, Tp> functionCall(functor, param);
        SyncDpcFunctionCallWrapper call(id, functionCall, delayed);
        return functionCall.getReturnValue();
    }

    /** Queues new DPC static function that is executed only once in the target execution context. */
    template <typename Tr, class Tp>
    static Tr syncQueueOnce(const whiteboard::ExecutionContextId id, Whiteboard& rWhiteboard, Tr(*pFunction)(Tp), Tp param, bool delayed = false)
    {
        StaticFunctor1<Tr, Tp> functor(pFunction);
        VoidFunctionCallWithReturn1<Tr, Tp> functionCall(functor, param);
        SyncDpcFunctionCallWrapper call(id, rWhiteboard, functionCall, delayed);
        return functionCall.getReturnValue();
    }

    /** Queues new DPC function that is executed only once in the target execution context. */
    static void syncQueueOnce(const whiteboard::ExecutionContextId id, IDpcFunctor& rFunctor, bool delayed = false)
    {
        SyncDpcFunctionCallWrapper call(id, rFunctor, delayed);
    }

    /** Queues new DPC function that is executed only once in the target execution context. */
    static void syncQueueOnce(const whiteboard::ExecutionContextId id, Whiteboard& rWhiteboard, IDpcFunctor& rFunctor, bool delayed = false)
    {
        SyncDpcFunctionCallWrapper call(id, rWhiteboard, rFunctor, delayed);
    }

public:
    /** Construct a new scoped Dpc with execution context, class instance and method call. */
    template <class Ty> DpcFunctor(const whiteboard::ExecutionContextId id, Ty* pInstance, void (Ty::*pFnMemberFunction)(), bool delayed = false)
    {
        FunctorImpl<Ty>* callTarget = new FunctorImpl<Ty>(pInstance, pFnMemberFunction);
        callTarget->mpDpc = mpDpc = Dpc::allocate(id, *callTarget);
        WB_ASSERT(NULL != mpDpc);
        callTarget->mpDpc->mDelayed = delayed;
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
        AsyncDpcFunctionCallWrapper(ExecutionContextId executionContextId, const FunctionCallType& rFunctionCall, bool delayed = false) :
            mFunctionCall(rFunctionCall),
            mpDpc(Dpc::allocate(executionContextId, *this))
        {
            WB_ASSERT(NULL != mpDpc);
            mpDpc->mDelayed = delayed;
            asyncCall();
        }

        AsyncDpcFunctionCallWrapper(ExecutionContextId executionContextId, Whiteboard& rWhiteboard, const FunctionCallType& rFunctionCall, bool delayed = false) :
            mFunctionCall(rFunctionCall),
            mpDpc(Dpc::allocate(executionContextId, *this, rWhiteboard))
        {
            WB_ASSERT(NULL != mpDpc);
            mpDpc->mDelayed = delayed;
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
    static void asyncCall(ExecutionContextId executionContextId, const FunctionCallType& rFunctionCall, bool delayed = false)
    {
        new AsyncDpcFunctionCallWrapper<FunctionCallType>(executionContextId, rFunctionCall, delayed);
    }

    template <typename FunctionCallType>
    static void asyncCall(ExecutionContextId executionContextId, Whiteboard& rWhiteboard, const FunctionCallType& rFunctionCall, bool delayed = false)
    {
        new AsyncDpcFunctionCallWrapper<FunctionCallType>(executionContextId, rWhiteboard, rFunctionCall, delayed);
    }

    class SyncDpcFunctionCallWrapper FINAL : public IDpcFunctor
    {
    public:
        IDpcFunctor& mrFunctionCall;
        whiteboard::Dpc* mpDpc;
        WbSemaphoreHandle mSemaphore;

    public:
        SyncDpcFunctionCallWrapper(ExecutionContextId executionContextId, IDpcFunctor& rFunctionCall, bool delayed = false) :
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
                mpDpc->mDelayed = delayed;
                mSemaphore = WbSemaphoreCreate(0, 1);
                mpDpc->queue(false);
                WbSemaphoreWait(mSemaphore);
                WbSemaphoreDelete(mSemaphore);
            }
        }

        SyncDpcFunctionCallWrapper(ExecutionContextId executionContextId, Whiteboard& rWhiteboard, IDpcFunctor& rFunctionCall, bool delayed = false) :
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
                mpDpc->mDelayed = delayed;
                mSemaphore = WbSemaphoreCreate(0, 1);
                mpDpc->queue(false);
                WbSemaphoreWait(mSemaphore);
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
            mpDpc->deallocate();
            WbSemaphoreRelease(mSemaphore);
        }
    };

protected:

    whiteboard::Dpc* mpDpc;
};

} // namespace whiteboard
