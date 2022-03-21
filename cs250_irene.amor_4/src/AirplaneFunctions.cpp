
#include "AirplaneFunctions.h"  //Header file

/**
* @brief Airplane_Initialize: initialize airplane object
*
* @param (void)
*/
void Airplane::Airplane_Initialize()
{
    //Read input file
    parser = new CS250Parser;
    parser->LoadDataFromFile("input.txt");

    //Set viewport size
    view_width = parser->right - parser->left;
    view_height = parser->top - parser->bottom;

    //Number of faces per cube and number of vertices per face
    max_faces = parser->faces.size();
    TOTAL_obj = parser->objects.size();

    //Get view and perspective matrices
    Viewport_Transformation();
    Perspective_Transform();

    //Start scene with the rooted camera
    camera_persp = rooted;
    RootedCamera();
    draw_mode = solid;

    CS250Parser::Transform* body = FindObject("body");
    Matrix4 m2w_body = ModelToWorld(*body, false);

    //Get the vectors of the airplane for the new camera
    Point4 airplane_pos = body->pos;
    airplane_fwd = m2w_body * (-parser->view);
    airplane_fwd.Normalize();
    airplane_up = m2w_body * parser->up;
    airplane_up.Normalize();


    //Get the color of each face
    //They are the same for all the cubes
    for (int j = 0; j < max_faces; j++)
    {
        //Normalize color
        color[j] = parser->colors[j];
        color[j].r = color[j].r / 255;
        color[j].g = color[j].g / 255;
        color[j].b = color[j].b / 255;
    }

}


/**
* @brief Airplane_Update: renders the current state of the airplane
*
* @param (void)
*/
void Airplane::Airplane_Update()
{
    //Get inputs from the user
    draw_mode = GetInput();
    

    //Calculate the current camera (sets the w2c matrix)
    if (camera_persp == first)
        FirstPersonCamera();
    else if (camera_persp == third)
        ThirdPersonCamera();


    //Calculate the new state of each object
    for (int obj = 0; obj < TOTAL_obj; obj++)
    {     
        //Need to calculate the model to world matrix first
        //Because it is the same for the whole object
        Matrix4 m2w = ModelToWorld(parser->objects[obj], true);

        //Vertices of the cube
        for (int i = 0; i < max_faces; i++)
        {
            auto face = parser->faces[i];
            Rasterizer::Vertex vtx[3];      //Each vertex of the triangle
            bool draw = true;

            for (int j = 0; j < 3; j++)
            {
                //Get vertices: color
                vtx[j].color = color[i];

                //Get vertices: position
                vtx[j].position = parser->vertices[face.indices[j]];

                //Transform vertices: perspective division and model to world
                vtx[j].position = persp_transf * w2c * m2w * vtx[j].position;

                //Culling: don't draw objects behind the camera
                if (vtx[j].position.z < -parser->nearPlane)
                {
                    draw = false;
                    break;
                }
            
                //Transform vertices: perspective division
                vtx[j].position.x = vtx[j].position.x / vtx[j].position.w;
                vtx[j].position.y = vtx[j].position.y / vtx[j].position.w;
                vtx[j].position.z = vtx[j].position.z / vtx[j].position.w;
                vtx[j].position.w = vtx[j].position.w / vtx[j].position.w;

                //Transform vertices: view transformation
                vtx[j].position = viewport * vtx[j].position;
            }



            //Draw the object
            if (draw)
            {
                if (draw_mode == depth_buffer)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        vtx[j].color.r = (vtx[j].position.z + 1) * 0.5f;
                        vtx[j].color.g = (vtx[j].position.z + 1) * 0.5f;
                        vtx[j].color.b = (vtx[j].position.z + 1) * 0.5f;
                    }

                    Rasterizer::DrawTriangleSolid(vtx[0], vtx[1], vtx[2]);
                }
                else if (draw_mode == solid)
                    Rasterizer::DrawTriangleSolid(vtx[0], vtx[1], vtx[2]);
                else
                {
                    //Every line composing the triangle
                    Rasterizer::DrawMidpointLine(vtx[0], vtx[1]);
                    Rasterizer::DrawMidpointLine(vtx[1], vtx[2]);
                    Rasterizer::DrawMidpointLine(vtx[2], vtx[0]);
                }
            }
        }

    }

}


/**
* @brief Viewport_Transformation: calculate the viewport transformation matrix
*
* @param (void)
*/
void Airplane::Viewport_Transformation()
{
    //Viewport transformation
    viewport.Identity();
    viewport.m[0][0] = static_cast<float>(WIDTH);
    viewport.m[0][3] = WIDTH / 2.f;
    viewport.m[1][1] = static_cast<float>(-HEIGHT);
    viewport.m[1][3] = HEIGHT / 2.f;

}


/**
* @brief Perspective_Projection: calculate the perspective projection matrix
*
* @param (void)
*/
void Airplane::Perspective_Transform()
{
    float a = static_cast<float>(WIDTH) / HEIGHT;

    //Perspective projection
    persp_transf.m[0][0] = parser->focal / view_width;
    persp_transf.m[1][1] = a * parser->focal / view_width;
    persp_transf.m[2][2] = (-parser->nearPlane - parser->farPlane) / (parser->farPlane - parser->nearPlane);
    persp_transf.m[2][3] = (-2 * parser->nearPlane * parser->farPlane) / (parser->farPlane - parser->nearPlane);
    persp_transf.m[3][2] = -1;

}



/**
* @brief FirstPersonCamera: set the camera info for the first person camera
*                           and calculate the corresponding w2c matrix
*
* @param (void)
*/
void Airplane::FirstPersonCamera()
{
    //Calculate the m2w matrix of the turret
    CS250Parser::Transform* body = FindObject("body");
    Matrix4 m2w_body = ModelToWorld(*body, false);

    //Set the camera information
    camera_position = body->pos;    
    camera_view = m2w_body * -parser->view;
    camera_up = m2w_body * parser->up;
    

    //Set the new w2c matrix
    w2c = WorldToCamera_Orth();
}


/**
* @brief RootedCamera: set the camera info for the rooted camera
*                      and calculate the corresponding w2c matrix
*
* @param (void)
*/
void Airplane::RootedCamera()
{
    //Set the camera information
    camera_position = parser->position;
    camera_view = parser->view;
    camera_up = parser->up;

    //Set the new w2c matrix
    w2c = WorldToCamera_Orth();
}


/**
* @brief ThirdPersonCamera: set the camera info for the third person camera
*                           and calculate the corresponding w2c matrix
*
* @param (void)
*/
void Airplane::ThirdPersonCamera()
{
    //Calculate the m2w matrix of the body
    CS250Parser::Transform* body = FindObject("body");
    Matrix4 m2w_body = ModelToWorld(*body, false);

    //Get the vectors of the airplane for the new camera
    Point4 airplane_pos = body->pos;
    //airplane_fwd = m2w_body * airplane_fwd//(-parser->view);
    //airplane_fwd.Normalize();
    //airplane_up = m2w_body * parser->up;
    //airplane_up.Normalize();

    //Set the camera information
    camera_position = airplane_pos - airplane_fwd * parser->distance + airplane_up * parser->height;
    camera_view = (airplane_pos - camera_position) / (airplane_pos - camera_position).Length();
    
    camera_right = camera_view.Cross(camera_up);
    camera_up = camera_right.Cross(camera_view);

    //Set the new w2c matrix
    w2c = WorldToCamera_Orth();
}


/**
* @brief WorldToCamera_GRM:  calculate the world to camera matrix
*
* @param (void)
* @return                    the matrix of the world to camera transformation
*/
Matrix4 Airplane::WorldToCamera_Orth()
{
    //Step 1: ALIGN
    Vector4 v = camera_up;      v.Normalize();
    Vector4 w = -camera_view;   w.Normalize();
    Vector4 u = w.Cross(v);     //u.Normalize();
    
    Vector4 A(camera_position.x, camera_position.y, camera_position.z, camera_position.w);

    Matrix4 align  (-u.x, -u.y, -u.z,   (u.Dot(A)),
                     v.x,  v.y,  v.z,  -(v.Dot(A)),
                     w.x,  w.y,  w.z,  -(w.Dot(A)),
                     0.f,  0.f,  0.f,   1.f);
                                 
    /*//Rotation
    Matrix4 RotY, RotZ, RotX, RotInvX, RotInvY;
    {
        //Rotation y-axis
        float length_Y = sqrtf((w.x * w.x) + (w.z * w.z));
        float cos_Y =  w.z / length_Y;
        float sin_Y = -w.x / length_Y;

        RotY.Identity();
        RotY.m[0][0] = cos_Y;
        RotY.m[0][2] = sin_Y;
        RotY.m[2][0] = -sin_Y;
        RotY.m[2][2] = cos_Y;

        RotInvY.m[0][2] *= -1.f;
        RotInvY.m[2][0] *= -1.f;

        //Rotation x-axis
        Vector4 w_prime = RotY * align * w;
        float length_X = sqrtf((w_prime.y * w_prime.y) + (w_prime.z * w_prime.z));
        float cos_X = -w_prime.z / length_X;
        float sin_X = -w_prime.y / length_X;

        RotX.Identity();
        RotX.m[1][1] = cos_X;
        RotX.m[1][2] = -sin_X;
        RotX.m[2][1] = sin_X;
        RotX.m[2][2] = cos_X;

        RotInvX.m[1][2] *= -1.f;
        RotInvX.m[2][1] *= -1.f;


        //Rotation z-axis
        Vector4 v_prime = RotX * RotY * align * v;

        float length_Z = sqrtf((v_prime.x * v_prime.x) + (v_prime.y * v_prime.y));
        float cos_Z = v_prime.y / length_Z;
        float sin_Z = v_prime.x / length_Z;

        RotZ.Identity();
        RotZ.m[0][0] = cos_Z;
        RotZ.m[0][1] = -sin_Z;
        RotZ.m[1][0] = sin_Z;
        RotZ.m[1][1] = cos_Z;
    }

    //Step 2: ROTATE
    Matrix4 RotX;
    float length_X = sqrtf((u.x * u.x) + (u.z * u.z));
    float cos_X = u.z / length_X;
    float sin_X = -u.x / length_X;

    RotX.Identity();
    RotX.m[1][1] = cos_X;
    RotX.m[1][2] = -sin_X;
    RotX.m[2][1] = sin_X;
    RotX.m[2][2] = cos_X;

    //Step 3: UNDO step 1
    Matrix4 un_align(u.x, v.x, w.x, A.x,
                     u.y, v.y, w.y, A.y,
                     u.z, v.z, w.z, A.z,
                     0.f, 0.f, 0.f, 1.f);

    Matrix4 w2c = un_align * RotInvY * RotInvX * RotZ * RotX * RotY * align;*/

    return align;
}


/**
* @brief ModelToWorld:  calculate the model to world matrix of the object
*
* @param obj:           object to find
* @return               the transform of the found object
*/
CS250Parser::Transform* Airplane::FindObject(std::string obj)
{
    for (int i = 0; i < TOTAL_obj; i++)
    {
        //Find the object
        if (!strcmp(obj.c_str(), parser->objects[i].name.c_str()))
            return &parser->objects[i];
    }

    //If it is never found
    return nullptr;
}

int Airplane::FindObject_pos(std::string obj)
{
    for (int i = 0; i < TOTAL_obj; i++)
    {
        //Find the object
        if (!strcmp(obj.c_str(), parser->objects[i].name.c_str()))
            return i;
    }

    //If it is never found
    return -1;
}

/**
* @brief ModelToWorld:  calculate the model to world matrix of the object
*
* @param obj:           object to calculate the matrix for
* @param scale:         whether to calculate the scale
* @return               model to world matrix
*/
Matrix4 Airplane::ModelToWorld(CS250Parser::Transform obj, bool scale)
{
    //Translation
    Matrix4 Transl;
    {
        Transl.Identity();
        Transl.m[0][3] = obj.pos.x;
        Transl.m[1][3] = obj.pos.y;
        Transl.m[2][3] = obj.pos.z;
    }

    //Rotation
    Matrix4 Rot, RotX, RotY, RotZ;
    Vector4 angle = obj.rot;
    {
        //Rotation x-axis
        RotX.Identity();
        RotX.m[1][1] = cos(angle.x);
        RotX.m[1][2] = -sin(angle.x);
        RotX.m[2][1] = sin(angle.x);
        RotX.m[2][2] = cos(angle.x);

        //Rotation y-axis
        RotY.Identity();
        RotY.m[0][0] = cos(angle.y);
        RotY.m[0][2] = sin(angle.y);
        RotY.m[2][0] = -sin(angle.y);
        RotY.m[2][2] = cos(angle.y);

        //Rotation z-axis
        RotZ.Identity();
        RotZ.m[0][0] = cos(angle.z);
        RotZ.m[0][1] = -sin(angle.z);
        RotZ.m[1][0] = sin(angle.z);
        RotZ.m[1][1] = cos(angle.z);

        //Concatenate rotations
        Rot = RotZ * RotY * RotX;
    }

    //Scale
    Matrix4 Scale;
    Scale.Identity();

    if (scale)
    {
        Scale.m[0][0] = obj.sca.x;
        Scale.m[1][1] = obj.sca.y;
        Scale.m[2][2] = obj.sca.z;
    }

    //Complete concatenation for the m2w matrix
    Matrix4 m2w = Transl * Rot * Scale;

    //If there is a parent, multiply its the M2W matrix
    if (strcmp(obj.parent.c_str(), "None"))
    {
        CS250Parser::Transform* parent = FindObject(obj.parent);

        if(parent)
            m2w = ModelToWorld(*parent, false) * m2w;
    }

    return m2w;
}

Matrix4 Airplane::tensor_product(Vector4 u, Vector4 v)
{
    Matrix4 tensor;
    tensor.Identity();

    tensor.m[0][0] = u.x * v.x;
    tensor.m[0][1] = u.x * v.y;
    tensor.m[0][2] = u.x * v.z;

    tensor.m[1][0] = u.y * v.x;
    tensor.m[1][1] = u.y * v.y;
    tensor.m[1][2] = u.y * v.z;

    tensor.m[2][0] = u.z * v.x;
    tensor.m[2][1] = u.z * v.y;
    tensor.m[2][2] = u.z * v.z;

    return tensor;
}

Matrix4 Airplane::get_matrix(Vector4 u)
{
    Matrix4 m;
    m.Identity();

    m.m[0][1] = -u.z;
    m.m[0][2] =  u.y;

    m.m[1][0] =  u.z;
    m.m[1][2] = -u.x;

    m.m[2][0] = -u.y;
    m.m[2][1] =  u.x;

    return m;
}

/**
* @brief GetInput:  change airplane with input from user
*
* @return           whether to draw the airplane as solid
*/
unsigned Airplane::GetInput()
{
    //Roll airplane body
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        if (CS250Parser::Transform* body = FindObject("body"))
        {
            Matrix4 m2w_body = ModelToWorld(*body, false);
            Vector4 airplane_fwd = m2w_body * (-parser->view);

            Matrix4 identity; identity.Identity();

            Matrix4 AxisAngle;
            AxisAngle.Identity();
            AxisAngle = identity * cos(-ROT_ANGLE)
                        + tensor_product(airplane_fwd, airplane_fwd) * (1 - cos(-ROT_ANGLE))
                        + get_matrix(airplane_fwd) * sin(-ROT_ANGLE);

            //body->rot = AxisAngle * body->rot;
            //camera_view = AxisAngle * camera_view;
            //camera_up = AxisAngle * camera_up;
            //camera_right = AxisAngle * camera_right;
            airplane_fwd = AxisAngle * airplane_fwd;
            airplane_up = AxisAngle * airplane_up;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        if (CS250Parser::Transform* body = FindObject("body"))
             body->rot.z -= 0.025f;
    }


    //Yaw airplane body 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
        if (CS250Parser::Transform* body = FindObject("body"))
        {
            Matrix4 m2w_body = ModelToWorld(*body, false);
            Vector4 airplane_up = m2w_body * (-parser->up);

            Matrix4 identity; identity.Identity();

            Matrix4 AxisAngle;
            AxisAngle.Identity();
            AxisAngle = identity * cos(-ROT_ANGLE)
                + tensor_product(airplane_up, airplane_up) * (1 - cos(-ROT_ANGLE))
                + get_matrix(airplane_up) * sin(-ROT_ANGLE);

            body->rot = AxisAngle * body->rot;
            // camera_view = AxisAngle * camera_view;

        }
        //if (CS250Parser::Transform* body = FindObject("body"))
        //    body->rot.y += 0.025f;
    }   

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        if (CS250Parser::Transform* body = FindObject("body"))
        {
            Matrix4 m2w_body = ModelToWorld(*body, false);
            Vector4 airplane_up = m2w_body * (-parser->up);

            Matrix4 identity; identity.Identity();

            Matrix4 AxisAngle;
            AxisAngle.Identity();
            AxisAngle = identity * cos(ROT_ANGLE)
                + tensor_product(airplane_up, airplane_up) * (1 - cos(ROT_ANGLE))
                + get_matrix(airplane_up) * sin(ROT_ANGLE);

            body->rot = AxisAngle * body->rot;
            // camera_view = AxisAngle * camera_view;

        }
        //if (CS250Parser::Transform* body = FindObject("body"))
        //    body->rot.y -= 0.025f;
    }


    //Pitch airplane body 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        if (CS250Parser::Transform* body = FindObject("body"))
            body->rot.x += 0.025f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        if (CS250Parser::Transform* body = FindObject("body"))
            body->rot.x -= 0.025f;
    }


    //Move airplane forward
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        //Move body
        if (CS250Parser::Transform* body = FindObject("body"))
        {
            body->pos.z += 5.f * cos(body->rot.y);
            body->pos.x += 5.f * sin(body->rot.y);
        }
    }


    //Check solid/wireframe mode
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0))
        return depth_buffer;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
        return wireframe;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
        return solid;


    //Switch camera mode
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
        camera_persp = first;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
        camera_persp = third;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
    {
        camera_persp = rooted;
        RootedCamera();         //Don't need to call it every time
    }


    //Camera distance
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
    {
        if (parser->distance - 10.f > 0.f)
            parser->distance -= 10.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
        parser->distance += 10.f;


    //Camera height
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
    {
        if(parser->height - 10.f > 0.f)
            parser->height -= 10.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
        parser->height += 10.f;


    return draw_mode;
}
