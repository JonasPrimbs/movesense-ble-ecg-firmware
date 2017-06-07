#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

// TODO: This is a temporary copy of nea::LinkedList. Refactor TxBleThread to avoid thread alltogether... 
template<class Ty>
class TempLinkedList
{
public:
    TempLinkedList() : mNext(this), mPrev(this) {}
    TempLinkedList<Ty>* next(void) { return mNext; }
    TempLinkedList<Ty>* prev(void) { return mPrev; }
    void append(TempLinkedList<Ty>* ln) { add(ln, mPrev, this); }
    void remove(TempLinkedList<Ty>* ln) { del(ln->mPrev, ln->mNext); }
    bool end(TempLinkedList<Ty>* ln) { return ln == this; }
    bool empty(void) { return mNext == this; }

    Ty item;
private:
    void add(TempLinkedList<Ty>* ln, TempLinkedList<Ty>* pn, TempLinkedList<Ty>* nn)
    {
        nn->mPrev = ln;
        ln->mNext = nn;
        ln->mPrev = pn;
        pn->mNext = ln;
    }
    void del(TempLinkedList<Ty>* pn, TempLinkedList<Ty>* nn)
    {
        nn->mPrev = pn;
        pn->mNext = nn;
    }

    TempLinkedList<Ty>* mNext;
    TempLinkedList<Ty>* mPrev;
};
