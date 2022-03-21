/****************************************************************************************/
/*!
\file   AirplaneFunctions.h
\author Irene Amor Mendez
\par    email: irene.amor@digipen.edu
\par    DigiPen login: irene.amor
\par    Course: CS250
\par    Assignment #4
\date   19/03/2022
\brief

This file contains the implementation of the following class functions for the
second Airplane assignment.
Functions include:	Airplane_Initialize, Viewport_Transformation, Perspective_Transform,
					ModelToWorld, OrthogonalMethod, WorldToCamera_Orth, AxisAngleMethod,
					FindObject, FirstPersonCamera, RootedCamera, ThirdPersonCamera,
					Airplane_Update, GetInput, tensor_product, get_matrix

Hours spent on this assignment: ~12

*/
/****************************************************************************************/

#include <SFML/Graphics.hpp>

#include "FrameBuffer.h"		//Frame buffer class
#include "Rasterizer.h"			//Rasterizer class
#include "CS250Parser.h"		//Parser class
#include "Math/Matrix4.h"		//Matrix 4*4 class
#include "Math/Point4.h"		//Point of size 4 class


class Airplane
{
public:

	//------------
	//Functions
	//------------

	void Airplane_Initialize();							//Initialize airplane object
	void Airplane_Update();								//Renders the current state of the airplane

	//------------
	//Variables
	//------------

	const int WIDTH = 1280;							//Window size
	const int HEIGHT = 960;

private:

	//------------
	//Functions
	//------------
	void Viewport_Transformation();					//Calculate the viewport transformation matrix
	void Perspective_Transform();					//Calculate the perspective projection matrix

	Matrix4 ModelToWorld(CS250Parser::Transform &obj, bool scale = true);	//Calculate the m2w matrix of each object
	Matrix4 OrthogonalMethod(CS250Parser::Transform& obj);					//Calculate the orthogonal rotation matrix of an object
	Matrix4 WorldToCamera_Orth();											//Calculate the w2c for the corresponding camera
	Matrix4 AxisAngleMethod(float angle, Vector4 vec);						//Calculate the axis angle method rotation matrix

	unsigned GetInput();									//Get user input

	CS250Parser::Transform* FindObject(std::string obj);	//Find the object's transform from its name

	void FirstPersonCamera();						//Functions to get the information of the corresponding camera
	void RootedCamera();
	void ThirdPersonCamera();

	Matrix4 tensor_product(Vector4 u, Vector4 v);	//Calculate the tensor product of two vectors
	Matrix4 get_matrix(Vector4 u);					//Get the matrix of the vector

	//------------
	//Variables
	//------------

	float view_width  = 0.f;		//Viewport size
	float view_height = 0.f;

	size_t max_faces = 0;			//Number of faces per shape
	size_t TOTAL_obj = 0;			//Objects on the scene

	CS250Parser* parser;			//Parser with input data

	Matrix4 viewport;				//Matrices that only need to be computed once
	Matrix4 persp_transf;
	Matrix4 w2c;

	Point4 color[12];				//Color of each triangle

	unsigned draw_mode = solid;		//Drawing mode

	Point4  camera_position;		//Camera information
	Vector4 camera_view;
	Vector4 camera_up;

	const float ROT_ANGLE = 0.05f;	//Angle to rotate by after the input
	const float MOVE_DIST = 7.5f;	//Distance to change the camera distance or height by

	int camera_persp = 0;			//Camera type
	enum camera{first, rooted, third};
	enum draw_mode {depth_buffer, wireframe, solid};
};