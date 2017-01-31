/******************************************************************************

        Copyright (c) Suunto Oy 2015.
        All rights reserved.

******************************************************************************/
#pragma once

namespace nea {

template<class Ty>
class LinkedList
{
public:
    LinkedList() : mNext(this), mPrev(this) {}
    LinkedList<Ty>* next(void) { return mNext; }
    LinkedList<Ty>* prev(void) { return mPrev; }
    void append(LinkedList<Ty>* ln) { add(ln, mPrev, this); }
    void remove(LinkedList<Ty>* ln) { del(ln->mPrev, ln->mNext); }
    bool end(LinkedList<Ty>* ln) { return ln == this; }
    bool empty(void) { return mNext == this; }

    Ty item;
private:
    void add(LinkedList<Ty>* ln, LinkedList<Ty>* pn, LinkedList<Ty>* nn)
    {
        nn->mPrev = ln;
        ln->mNext = nn;
        ln->mPrev = pn;
        pn->mNext = ln;
    }
    void del(LinkedList<Ty>* pn, LinkedList<Ty>* nn)
    {
        nn->mPrev = pn;
        pn->mNext = nn;
    }

    LinkedList<Ty>* mNext;
    LinkedList<Ty>* mPrev;
};
}
