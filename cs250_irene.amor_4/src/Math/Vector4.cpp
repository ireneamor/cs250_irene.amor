/****************************************************************************************/
/*!
\file   Vector4.cpp
\author Irene Amor Mendez
\par    email: irene.amor@digipen.edu
\par    DigiPen login: irene.amor
\par    Course: CS250
\par    Assignment #1
\date   18/01/2022
\brief

This file contains the implementation of the following functions for the
Math Library assignment.
Functions include:	Vector4 constructors, operator=, operator-, operator+,
					operator- (negation), operator *, operator/, operator+=,
					operator-=, operator*=, operator/=, operator==, operator!=,
					Dot, Cross, Length, LengthSq, Normalize, Zero

Hours spent on this assignment: ~10

*/
/****************************************************************************************/

#include "Vector4.h"		//Header file
#include "MathUtilities.h"	//Helper macros


/**
* @brief Default constructor, initializes x,y,z to zeroes, w to defined value
*
* @param (void)
*/
Vector4::Vector4(void)
{
	//Initialize everything to 0
	for (int i = 0; i < 4; i++)
		v[i] = 0.0f;
}

/**
* @brief Copy constructor, copies every component from the other Vector4
*
* @param rhs:	vector to copy from
*/
Vector4::Vector4(const Vector4& rhs)
{
	//Initialize values
	for (int i = 0; i < 4; i++)
		v[i] = rhs.v[i];
}

/**
* @brief Non-Default constructor, self explanatory
*
* @param xx, yy, zz, ww:	values to make vector with
*/
Vector4::Vector4(float xx, float yy, float zz, float ww)
{
	//Initialize values
	x = xx;
	y = yy;
	z = zz;
	w = ww;
}

/**
* @brief Assignment operator, does not need to handle self assignment
*
* @param rhs:		vector to copy from
* @return *this:	altered vector
*/
Vector4& Vector4::operator=(const Vector4& rhs)
{
	//Copy values if the vector is different
	if (*this != rhs)
	{
		for (int i = 0; i < 4; i++)
			v[i] = rhs.v[i];
	}

	return *this;
}

/**
* @brief Unary negation operator, negates all components and returns a copy
*
* @param rhs:		vector to copy from
* @return vec:		vector with negated values
*/
Vector4 Vector4::operator-(void) const
{
	//Make new vector with negated values
	Vector4 vec;
	for (int i = 0; i < 4; i++)
		vec.v[i] = -v[i];

	return vec;
}

/**
* @brief Addition of 2 vectors
*
* @param rhs:		vector to add
* @return vec:		result of the addition
*/
Vector4 Vector4::operator+(const Vector4& rhs) const
{
	//Add the values of the vectors
	Vector4 vec;
	for (int i = 0; i < 4; i++)
		vec.v[i] = v[i] + rhs.v[i];

	return vec;
}

/**
* @brief Substraction of 2 vectors
*
* @param rhs:		vector to substract
* @return vec:		result of the substraction
*/
Vector4 Vector4::operator-(const Vector4& rhs) const
{
	//Substract values
	Vector4 vec;
	for (int i = 0; i < 4; i++)
		vec.v[i] = v[i] - rhs.v[i];

	return vec;
}

/**
* @brief Multiplication of 2 vectors
*
* @param rhs:		vector to multiply
* @return vec:		result of the multiplication
*/
Vector4 Vector4::operator*(const float rhs) const
{
	//Multiply values
	Vector4 vec;
	for (int i = 0; i < 4; i++)
		vec.v[i] = v[i] * rhs;

	return vec;
}

/**
* @brief Division of 2 vectors
*
* @param rhs:		vector to divide
* @return vec:		result of the division
*/
Vector4 Vector4::operator/(const float rhs) const
{
	//Divide values
	Vector4 vec;
	for (int i = 0; i < 4; i++)
		vec.v[i] = v[i] / rhs;

	return vec;
}

/**
* @brief Addition of 2 vectors
*
* @param rhs:		vector to add
* @return *this:	altered vector with result of the addition
*/
Vector4& Vector4::operator+=(const Vector4& rhs)
{
	//Add vectors
	*this = *this + rhs;
	return *this;
}

/**
* @brief Substraction of 2 vectors
*
* @param rhs:		vector to substract
* @return *this:	altered vector with result of the substraction
*/
Vector4& Vector4::operator-=(const Vector4& rhs)
{
	//Substract vectors
	*this = *this - rhs;
	return *this;
}

/**
* @brief Multiplication of 2 vectors
*
* @param rhs:		vector to multiply
* @return *this:	altered vector with result of the multiplication
*/
Vector4& Vector4::operator*=(const float rhs)
{
	//Multiply vectors
	*this = *this * rhs;
	return *this;
}

/**
* @brief Division of 2 vectors
*
* @param rhs:		vector to divide
* @return *this:	altered vector with result of the division
*/
Vector4& Vector4::operator/=(const float rhs)
{
	//Divide vectors
	*this = *this / rhs;
	return *this;
}

/**
* @brief Comparison operator, should use funciton from MathUtilities.h
*
* @param rhs:			vector to compare
* @return true/false:	whether the vectors are equal
*/
bool Vector4::operator==(const Vector4& rhs) const
{
	//Compare vectors using macro function
	for (int i = 0; i < 4; i++)
		if (!isEqual(v[i], rhs.v[i]))
			return false;

	return true;
}

/**
* @brief Comparison operator
*
* @param rhs:			vector to compare
* @return true/false:	whether the vectors are different
*/
bool Vector4::operator!=(const Vector4& rhs) const
{
	//Compare vectors
	return !(*this == rhs);
}

/**
* @brief  Computes the dot product with the other vector.
*
* @param rhs:		vector to multiply
* @return dot:		result of the dot product
*/
float Vector4::Dot(const Vector4& rhs) const
{
	//Multiply row by row and add results (dot product)
	float dot = 0.f;
	for (int i = 0; i < 4; i++)
		dot += v[i] * rhs.v[i];

	return dot;
}

/**
* @brief  Computes the cross product with the other vector
*
* @param rhs:		vector to multiply
* @return vec:		result of the cross product
*/
Vector4 Vector4::Cross(const Vector4& rhs) const
{
	//Do the cross product "manually"
	Vector4 vec;
	vec.v[0] = v[1] * rhs.v[2] - v[2] * rhs.v[1];
	vec.v[1] = v[2] * rhs.v[0] - v[0] * rhs.v[2];
	vec.v[2] = v[0] * rhs.v[1] - v[1] * rhs.v[0];

	return vec;
}

/**
* @brief  Computes the true length of the vector
*
* @param (void)
* @return length:	length of the vector
*/
float Vector4::Length(void) const
{
	//Square root the LengthSq value
	return static_cast<float>(sqrt(LengthSq()));
}

/**
* @brief  Computes the squared length of the vector
*
* @param (void)
* @return length:	squared length of the vector
*/
float Vector4::LengthSq(void) const
{
	//Multiply the vector by itself
	return Dot(*this);
}

/**
* @brief  Normalizes the vector to make the final vector be of length 1
*
* @param (void)
*/
void Vector4::Normalize(void)
{
	float length = Length();

	//Sanity check: If the length is zero then this function should not modify anything
	if (length == 0.f)
		return;

	//Normalize the vector
	for (int i = 0; i < 4; i++)
		v[i] = v[i] / length;
}

/**
* @brief  Sets x,y,z to zeroes, w to defined value
*
* @param (void)
*/
void Vector4::Zero(void)
{
	//Set the vector
	*this = Vector4(0.0f, 0.0f, 0.0f);
}

