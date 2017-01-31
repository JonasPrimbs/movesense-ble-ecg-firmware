#pragma once
namespace nea
{

/**
        swap template function.

        Swap two items of same type.

        @param a A varible.
        @param m A variable.
*/
template <typename T> inline void swap(T& a, T& b)
{
    T temp = a;
    a = b;
    b = temp;
}

} // namespace nea {
