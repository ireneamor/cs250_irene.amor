/****************************************************************************************/
/*!
\file   Matrix4.cpp
\author Irene Amor Mendez
\par    email: irene.amor@digipen.edu
\par    DigiPen login: irene.amor
\par    Course: CS250
\par    Assignment #1
\date   18/01/2022
\brief

This file contains the implementation of the following functions for the
Math Library assignment.
Functions include:	Matrix4 constructors, operator=, operator-, operator+,
					operator- (negation), operator *, operator/, operator+=,
					operator-=, operator*=, operator/=, operator==, operator!=,
					Identity, Zero

Hours spent on this assignment: ~10

*/
/****************************************************************************************/

#include "Matrix4.h"			//Header file
#include "MathUtilities.h"		//Helper macros


/**
* @brief Default constructor, sets x,y,z to zero and w to the defined value
*
* @param (void)
*/
Matrix4::Matrix4(void)
{
	//Set all values to zero
	for (int i = 0; i < 16; i++)
		v[i] = 0.0f;
}

/**
* @brief Copy constructor, copies every entry from the other matrix.
*
* @param rhs:	matrix to copy values from
*/
Matrix4::Matrix4(const Matrix4& rhs)
{
	//Set values
	for (int i = 0; i < 16; i++)
		v[i] = rhs.v[i];
}

/**
* @brief Non-Default constructor, self-explanatory

*
* @param mm00, mm01, mm02...:	values to set matrix to
*/
Matrix4::Matrix4(float mm00, float mm01, float mm02, float mm03,
float mm10, float mm11, float mm12, float mm13,
float mm20, float mm21, float mm22, float mm23,
float mm30, float mm31, float mm32, float mm33)
{
	//Set values
	m[0][0] = mm00;	m[0][1] = mm01;	m[0][2] = mm02; m[0][3] = mm03;
	m[1][0] = mm10;	m[1][1] = mm11;	m[1][2] = mm12; m[1][3] = mm13;
	m[2][0] = mm20;	m[2][1] = mm21;	m[2][2] = mm22; m[2][3] = mm23;
	m[3][0] = mm30;	m[3][1] = mm31;	m[3][2] = mm32; m[3][3] = mm33;
}

/**
* @brief Assignment operator, does not need to handle self-assignment
*
* @param rhs:		matrix to copy from
* @return *this:	altered matrix
*/
Matrix4& Matrix4::operator=(const Matrix4& rhs)
{
	//Change values if they are different matrices
	if(*this != rhs)
		for (int i = 0; i < 16; i++)
			v[i] = rhs.v[i];

	return *this;
}

/**
* @brief  Multiplying a Matrix4 with a Vector4
*
* @param rhs:		vector to multiply
* @return vec:		result of the multiplication
*/
Vector4 Matrix4::operator*(const Vector4& rhs) const
{
	Vector4 vec;

	//Do the multiplication
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			vec.v[i] += m[i][j] * rhs.v[j];
		}
	}

	return vec;
}

/**
* @brief  Multiplying a Matrix4 with a Point4
*
* @param rhs:		point to multiply
* @return point:		result of the multiplication
*/
Point4 Matrix4::operator*(const Point4& rhs) const
{
	Point4 point;
	point.w = 0.f;

	//Do the multiplication
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			point.v[i] += m[i][j] * rhs.v[j];
		}
	}

	return point;
}


/**
* @brief  Addition of 2 matrices
*
* @param rhs:		matrix to add
* @return mtx:		result of the addition
*/
Matrix4 Matrix4::operator+(const Matrix4& rhs) const
{
	Matrix4 mtx;

	//Add the matrices
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			mtx.m[i][j] += m[i][j] + rhs.m[i][j];
		}
	}

	return mtx;
}

/**
* @brief  Substraction of 2 matrices
*
* @param rhs:		matrix to substract
* @return mtx:		result of the substraction
*/
Matrix4 Matrix4::operator-(const Matrix4& rhs) const
{
	Matrix4 mtx;

	//Substract matrices
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			mtx.m[i][j] += m[i][j] - rhs.m[i][j];
		}
	}

	return mtx;
}

/**
* @brief  Multiplication of 2 matrices
*
* @param rhs:		matrix to multiply
* @return mtx:		result of the multiplication
*/
Matrix4 Matrix4::operator*(const Matrix4& rhs) const
{
	Matrix4 mtx;

	//Multiply the matrices
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				mtx.m[i][j] += m[i][k] * rhs.m[k][j];
			}
		}
	}

	return mtx;
}

/*
* @brief  Addition of 2 matrices
*
* @param rhs :		matrix to add
* @return *this:	altered point with result of the addition
*/
Matrix4& Matrix4::operator+=(const Matrix4& rhs)
{
	//Add matrices
	*this = *this + rhs;
	return *this;
}

/*
* @brief  Substraction of 2 matrices
*
* @param rhs :		matrix to substract
* @return *this:	altered point with result of the substraction
*/
Matrix4& Matrix4::operator-=(const Matrix4& rhs)
{
	//Substract the matrices
	*this = *this - rhs;
	return *this;
}

/*
* @brief  Multiplication of 2 matrices
*
* @param rhs :		matrix to multiply
* @return *this:	altered point with result of the multiplication
*/
Matrix4& Matrix4::operator*=(const Matrix4& rhs)
{
	//Multiply the matrices
	*this = *this * rhs;
	return *this;
}

/*
* @brief  Multiplication by an scalar
*
* @param rhs :		scalar to multiply
* @return mtx:		result of the multiplication
*/
Matrix4 Matrix4::operator*(const float rhs) const
{
	//Multiply every value by the scalar
	Matrix4 mtx;
	for (int i = 0; i < 16; i++)
		mtx.v[i] = rhs * v[i];

	return mtx;
}

/*
* @brief  Division by an scalar
*
* @param rhs :		scalar to divide
* @return mtx:		result of the division
*/
Matrix4 Matrix4::operator/(const float rhs) const
{
	//Divide every value by the scalar
	Matrix4 mtx;
	for (int i = 0; i < 16; i++)
		mtx.v[i] = v[i] / rhs;

	return mtx;
}

/*
* @brief  Multiplication by an scalar
*
* @param rhs :		scalar to multiply
* @return *this:	altered matrix with result of the multiplication
*/
Matrix4& Matrix4::operator*=(const float rhs)
{
	//Multiply by rhs
	*this = *this * rhs;
	return *this;
}

/*
* @brief  Division by an scalar
*
* @param rhs :		scalar to divide
* @return mtx:		altered matrix with result of the division
*/
Matrix4& Matrix4::operator/=(const float rhs)
{
	//Divide by rhs
	*this = *this / rhs;
	return *this;
}

/**
* @brief Comparison operator, should use funciton from MathUtilities.h
*
* @param rhs:			matrix to compare
* @return true/false:	whether the matrices are equal
*/
bool Matrix4::operator==(const Matrix4& rhs) const
{
	//Compare values in the matrix using macro function
	for (int i = 0; i < 16; i++)
	{
		if (!isEqual(v[i], rhs.v[i]))
			return false;
	}

	return true;
}


/**
* @brief Comparison operator
*
* @param rhs:			marix to compare
* @return true/false:	whether the matrices are different
*/
bool Matrix4::operator!=(const Matrix4& rhs) const
{
	//Compare matrices
	return !(*this == rhs);
}



/**
* @brief  Sets all values of the matrix to zero
*
* @param (void)
*/
void Matrix4::Zero(void)
{
	//Set everything to 0
	for (int i = 0; i < 16; i++)
		v[i] = 0.0f;
}

/**
* @brief  Builds the identity matrix
*
* @param (void)
*/
void Matrix4::Identity(void)
{
	//Make a zero matrix and set the main diagonal to 1
	Zero();
	for (int i = 0; i < 4; i++)
		m[i][i] = 1.0f;
}