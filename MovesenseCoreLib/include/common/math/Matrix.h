#pragma once
/******************************************************************************
    Copyright © Suunto Oy 2013.

    All rights reserved.

******************************************************************************/
/**
        @file Matrix.h

        Matrix implementation.
*/
#include "oswrapper/hostOs.h"

namespace nea
{

class Matrix
{
public:
    /**
       Default constructor
     */
    Matrix(void);

    /**
       Copy constructor

       @param rM	Reference to the source object
     */
    Matrix(const Matrix& rM);

    /**
       Constructor with source data

       @param i		Matrix width
       @param j		Matric height
       @param pSrc	Pointer to the source data
     */
    Matrix(int i, int j, const float* pSrc = NULL);

    /**
       Destructor
     */
    ~Matrix(void);

    /**
       Set matrix size

       @param i		Matrix width
       @param j		Matric height
     */
    void setSize(int i, int j);

    /**
       Get value

       @param i		Matrix x position
       @param j		Matrix y position

       @return float	Value
     */
    float getValue(int i, int j) const;

    /**
       Set value

       @param i		Matrix x position
       @param j		Matrix y position
       @param value	Position value
     */
    void setValue(int i, int j, float value);

    /**
       Set matrix

       @param i		Matrix width
       @param j		Matric height
       @param pSrc	Pointer to the source data
     */
    void setMatrix(int i, int j, const float* pSrc);

    /**
       Make unit matrix

       @param n	Matrix x & y size

       @return const Matrix&	Reference to the this object
     */
    Matrix& unit(int n);

    /**
       Invert matrix

       @return const Matrix&	Reference to the this object
     */
    Matrix& invert(void);

    /**
       Transpose matrix

       @return const Matrix&	Reference to the this object
     */
    Matrix& transpose(void);

    /**
       Copy operator

       @param rM	Reference to source object

       @return Matrix&	Reference to the this object
     */
    Matrix& operator=(const Matrix& rM);

    /**
       Compare operator

       @param rM	Reference to source object

       @return bool	True if identical matrix
     */
    bool operator==(const Matrix& rM) const;

    /**
       Add matrix

       @param rM	Reference to source object

       @return Matrix	Result matrix
     */
    Matrix operator+(const Matrix& rM) const;

    /**
       Subtract matrix

       @param rM	Reference to source object

       @return Matrix	Result matrix
     */
    Matrix operator-(const Matrix& rM) const;

    /**
       Multiply matrix

       @param rM	Reference to source object

       @return Matrix	Result matrix
     */
    Matrix operator*(const Matrix& rM) const;

    /**
       Divide matrix

       @param rM	Reference to source object

       @return Matrix	Result matrix
     */
    Matrix operator/(const Matrix& rM) const;

    /**
       Multiply matrix with constant

       @param k	Multiply value

       @return Matrix	Result matrix
     */
    Matrix operator*(float k) const;

    /**
       Divide matrix with constant

       @param k	Divide value

       @return Matrix	Result matrix
     */
    Matrix operator/(float k) const;

    /**
       Add matrix

       @param rM	Reference to source object

       @return const Matrix&	Refernce to the this object
     */
    const Matrix& operator+=(const Matrix& rM);

    /**
       Subtract matrix

       @param rM	Reference to source object

       @return const Matrix&	Refernce to the this object
     */
    const Matrix& operator-=(const Matrix& rM);

    /**
       Multiply matrix

       @param rM	Reference to source object

       @return const Matrix&	Refernce to the this object
     */
    const Matrix& operator*=(const Matrix& rM);

    /**
       Divide matrix

       @param rM	Reference to source object

       @return const Matrix&	Refernce to the this object
     */
    const Matrix& operator/=(const Matrix& rM);

    /**
       Multiply matrix with constant

       @param k	Multiply value

       @return const Matrix&	Refernce to the this object
     */
    const Matrix& operator*=(float k);

    /**
       Divide matrix with constant

       @param k	Divide value

       @return const Matrix&	Refernce to the this object
     */
    const Matrix& operator/=(float k);

    /**
       Calculate matrix lines sum

       @return Matrix
     */
    Matrix linesSum(void) const;

    /**
       Add line to the lines

       @param rM	Source value to add
     */
    void addToLines(const Matrix& rM);

    /**
       Make unit matrix

       @param n	Matrix x & y size

       @return Matrix	Created matrix
     */
    static Matrix makeUnit(int n);

    /**
       Make invert matrix

       @param rM	Reference to the source matrix

       @return Matrix	Created matrix
     */
    static Matrix makeInvert(const Matrix& rM);

    /**
       Make transpose

       @param rM	Reference to the source matrix

       @return Matrix	Created matrix
     */
    static Matrix makeTranspose(const Matrix& rM);

    /**
       Calculate linear root

       @param rA	Reference to the source obejct A
       @param rB	Reference to the source obejct B

       @return Matrix	Root matrix
     */
    static Matrix linearRoot(const Matrix& rA, const Matrix& rB);

    /**
       Calculate minner

       @param rA	Reference to the source obejct A
       @param rB	Reference to the source obejct B

       @return Matrix	Root matrix
     */
    static Matrix minner(const Matrix& rA, const Matrix& rB);

    /**
       Check is matrix ok

       @return bool	True if ok
     */
    inline bool isOk(void) const { return ((mWidth > 0) && (mHeight > 0)); }

    /**
       Get matrix width

       @return int	Matrix width
     */
    inline int getWidth(void) const { return mWidth; }

    /**
       Get matrix height

       @return int	Matrix height
     */
    inline int getHeight(void) const { return mHeight; }

    /**
       Get matrix size

       @param i		Matrix width
       @param j		Matrix height
     */
    inline void getSize(int& i, int& j) const
    {
        i = mWidth;
        j = mHeight;
    }

    /**
       Check is matrix same size

       @param M		Reference to the source matrix

       @return bool	True if same size
     */
    inline bool isSameSize(const Matrix& M) const { return ((mWidth == M.mWidth) && (mHeight == M.mHeight)); }

    /**
       Get matrix data

       @return const float*	Pointer to the matrix data
     */
    inline const float* getData(void) const { return maData; }

    /**
       Compare operator

       @param rM	Reference to the source object

       @return bool	True if not equal
     */
    inline bool operator!=(const Matrix& rM) const { return !(*this == rM); }

protected:
    /**
       Matrix memory allocation

       @param i		Matrix width
       @param j		Matrix height
     */
    void alloc(int i, int j);

    /**
       Get reference to given data cell

       @param i		Matrix X position
       @param j		Matrix Y position

       @return const float&	Refernce to the cell
     */
    inline const float& getValueRef(int i, int j) const { return maData[i + j * mWidth]; }

    /**
       Get reference to given data cell

       @param i		Matrix X position
       @param j		Matrix Y position

       @return float&	Refernce to the cell
     */
    inline float& getValueRef(int i, int j) { return maData[i + j * mWidth]; }

    /**
       Get pointer to given data cell

       @param i		Matrix X position
       @param j		Matrix Y position

       @return const float*	Pointer to the cell
     */
    inline const float* getValuePtr(int i, int j) const { return &maData[i + j * mWidth]; }

    /**
       Get pointer to given data cell

       @param i		Matrix X position
       @param j		Matrix Y position

       @return float*	Pointer to the cell
     */
    inline float* getValuePtr(int i, int j) { return &maData[i + j * mWidth]; }

protected:
    int mWidth;
    int mHeight;
    float maData[4 * 8];
};

} // namespace nea
