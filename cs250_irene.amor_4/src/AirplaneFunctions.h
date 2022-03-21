/****************************************************************************************/
/*!
\file   AirplaneFunctions.h
\author Irene Amor Mendez
\par    email: irene.amor@digipen.edu
\par    DigiPen login: irene.amor
\par    Course: CS250
\par    Assignment #3
\date   06/03/2022
\brief

This file contains the implementation of the following class functions for the
second Airplane assignment.
Functions include:	Airplane_Initialize, Viewport_Transformation, Perspective_Transform,
					ModelToWorld, WorldToCamera_GRM, FindObject, FirstPersonCamera,
					RootedCamera, ThirdPersonCamera, Airplane_Update, GetInput

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

	Matrix4 ModelToWorld(CS250Parser::Transform obj, bool scale = true);	//Calculate the m2w matrix of each object
	Matrix4 WorldToCamera_Orth();					//Calculate the w2c for the corresponding camera

	unsigned GetInput();							//Get user input

	CS250Parser::Transform* FindObject(std::string obj);	//Find the object's transform from its name
	int FindObject_pos(std::string obj);					//Find the object's position in the parser->objects array

	void FirstPersonCamera();						//Functions to get the information of the corresponding camera
	void RootedCamera();
	void ThirdPersonCamera();

	Matrix4 tensor_product(Vector4 u, Vector4 v);
	Matrix4 get_matrix(Vector4 u);

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
	const float ROT_ANGLE = 1.5f;	//Angle of rotation for the inputs

	unsigned draw_mode = solid;		//Drawing mode

	Point4  camera_position;		//Camera information
	Vector4 camera_view;
	Vector4 camera_up;
	Vector4 camera_right;

	Vector4 airplane_up;
	Vector4 airplane_fwd;

	int camera_persp = 0;			//Camera type
	enum camera{first, rooted, third};
	enum draw_mode {depth_buffer, wireframe, solid};
};