/****************************************************************************************/
/*!
\file   Point4.cpp
\author Irene Amor Mendez
\par    email: irene.amor@digipen.edu
\par    DigiPen login: irene.amor
\par    Course: CS250
\par    Assignment #1
\date   18/01/2022
\brief

This file contains the implementation of the following functions for the
Math Library assignment.
Functions include:	Point4 constructors, operator=, operator-, operator+,
					operator- (negation), operator+=, operator-=, operator==,
					operator!=, Zero

Hours spent on this assignment: ~10

*/
/****************************************************************************************/

#include "Point4.h"				//Header file
#include "MathUtilities.h"		//Helper macros


/**
* @brief Default constructor, sets x,y,z to zero and w to the defined value
*
* @param (void)
*/
Point4::Point4(void)
{
	//Set the values
	for (int i = 0; i < 3; i++)
		v[i] = 0.f;

	w = 1.f;
}

/**
* @brief Copy constructor, copies every component from the other Point4
*
* @param rhs:	point to copy from
*/
Point4::Point4(const Point4& rhs)
{
	//Set values
	for (int i = 0; i < 4; i++)
		v[i] = rhs.v[i];
}

/**
* @brief Non-Default constructor, self-explanatory

*
* @param xx, yy, zz, ww:	values to set point to
*/
Point4::Point4(float xx, float yy, float zz, float ww)
{
	//Set values
	x = xx;
	y = yy;
	z = zz;
	w = ww;
}

/**
* @brief Assignment operator, copies every component from the other Point4
* 
* @param rhs:		point to copy from
* @return *this:	altered point
*/
Point4& Point4::operator=(const Point4& rhs)
{
	//Copy values
	for (int i = 0; i < 4; i++)
		v[i] = rhs.v[i];
	return *this;
}

/**
* @brief Unary negation operator, negates all components and returns a copy
*
* @param rhs:		point to copy from
* @return vec:		point with negated values
*/
Point4 Point4::operator-(void) const
{
	//Make new point with negated values

	Point4 point;
	for (int i = 0; i < 4; i++)
		point.v[i] = -v[i];

	return point;
}


/**
* @brief Binary subtraction operator, Subtract two Point4s and you get a Vector4
* 
* @param rhs:		point to substract
* @return vec:		result of the substraction
*/
Vector4 Point4::operator-(const Point4& rhs) const
{
	//Substract the points
	Vector4 vec;
	for (int i = 0; i < 4; i++)
		vec.v[i] = v[i] - rhs.v[i];

	return vec;
}

/**
* @brief Addition of point and vector
*
* @param rhs:		vector to add
* @return point:		result of the addition
*/
Point4 Point4::operator+ (const Vector4& rhs) const
{
	//Add values
	Point4 point;
	for (int i = 0; i < 4; i++)
		point.v[i] = v[i] + rhs.v[i];

	return point;
}

/**
* @brief substractions of point and vector
*
* @param rhs:		vector to substract
* @return point:	result of the substraction
*/
Point4 Point4::operator- (const Vector4& rhs) const
{
	//Substract values
	Point4 point;
	for (int i = 0; i < 4; i++)
		point.v[i] = v[i] - rhs.v[i];

	return point;
}

/**
* @brief Addition of point and vector
*
* @param rhs:		vector to add
* @return *this:	altered point with result of the addition
*/
Point4& Point4::operator+=(const Vector4& rhs)
{
	//Add the values
	*this = *this + rhs;
	return *this;
}

/**
* @brief substractions of point and vector
*
* @param rhs:		vector to substract
* @return *this:	altered point with result of the substraction
*/
Point4& Point4::operator-=(const Vector4& rhs)
{
	//Substract the values
	*this = *this - rhs;
	return *this;
}

/**
* @brief Comparison operator, should use funciton from MathUtilities.h
*
* @param rhs:			point to compare
* @return true/false:	whether the points are equal
*/
bool Point4::operator==(const Point4& rhs) const
{
	//Compare points using macro function
	for (int i = 0; i < 4; i++)
		if (!isEqual(v[i], rhs.v[i]))
			return false;

	return true;
}

/**
* @brief Comparison operator
*
* @param rhs:			point to compare
* @return true/false:	whether the points are different
*/
bool Point4::operator!=(const Point4& rhs) const
{
	//Compare points
	return !(*this == rhs);
}

/**
* @brief  Sets x,y,z to zeroes, w to defined value
*
* @param (void)
*/
void Point4::Zero(void)
{
	//Set the point
	*this = Point4(0.0f, 0.0f, 0.0f);
}

