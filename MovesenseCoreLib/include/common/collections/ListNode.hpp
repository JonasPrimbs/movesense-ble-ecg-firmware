/******************************************************************************

        Copyright (c) Suunto Oy 2015.
        All rights reserved.

******************************************************************************/
#pragma once

/** @deprecated Use template version LinkedList<Ty> instead. */
class ListNode
{
public:
    ListNode() : mNext(this), mPrev(this) {}
    ListNode* next(void) { return mNext; }
    ListNode* prev(void) { return mPrev; }
    void append(ListNode* ln) { add(ln, mPrev, this); }
    void prepend(ListNode* ln) { add(ln, this, mNext); }
    void remove(ListNode* ln) { del(ln->mPrev, ln->mNext); }
    bool end(ListNode* ln) { return ln == this; }
    bool empty(void) { return mNext == this; }

private:
    void add(ListNode* ln, ListNode* pn, ListNode* nn)
    {
        nn->mPrev = ln;
        ln->mNext = nn;
        ln->mPrev = pn;
        pn->mNext = ln;
    }

    void del(ListNode* pn, ListNode* nn)
    {
        nn->mPrev = pn;
        pn->mNext = nn;
    }

    ListNode* mNext;
    ListNode* mPrev;
};
