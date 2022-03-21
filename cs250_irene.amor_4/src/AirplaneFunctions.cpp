
#include "AirplaneFunctions.h"  //Header file
#include <vector>               //For the vector of m2w matrices



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
  
    std::vector<Matrix4> m2w;


    //Need to calculate the model to world matrix before calculating the cameras
    //to get the model to world matrices that get multiplied by the scale
    //This is done to avoid 
    for (int obj = 0; obj < TOTAL_obj; obj++)
    {
        m2w.push_back(ModelToWorld(parser->objects[obj], true));
    }


    //Calculate the current camera (sets the w2c matrix)
    if (camera_persp == first)
        FirstPersonCamera();
    else if (camera_persp == third)
        ThirdPersonCamera();


    //Calculate and draw the vertices
    for (int obj = 0; obj < TOTAL_obj; obj++)
    {
        //Vertices of the cube
        for (int i = 0; i < max_faces; i++)
        {
            auto face = parser->faces[i];
            Rasterizer::Vertex vtx[3];      //Each vertex of the triangle
            bool draw = true;

            //Calculate the vertices
            for (int j = 0; j < 3; j++)
            {
                //Get vertices: color
                vtx[j].color = color[i];

                //Get vertices: position
                vtx[j].position = parser->vertices[face.indices[j]];

                //Transform vertices: perspective division and model to world (using the m2w with the scale)
                vtx[j].position = persp_transf * w2c * m2w[obj] * vtx[j].position;

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
    CS250Parser::Transform* body = FindObject("body");

    //Set the camera information
    camera_position = body->m2w * Point4();    
    camera_view     = body->fwd;
    camera_up       = body->up;
    

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
    camera_view     = parser->view;
    camera_up       = parser->up;

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

    //Get the position of the plane
    Point4 airplane_pos = body->m2w * Point4();

    //Set the camera information
    camera_position = airplane_pos - body->fwd * parser->distance + body->up * parser->height;
    camera_view = (airplane_pos - camera_position) / (airplane_pos - camera_position).Length();
    camera_up = body->right.Cross(camera_view);

    //Set the new w2c matrix
    w2c = WorldToCamera_Orth();
}

/**
* @brief WorldToCamera_Orth:    calculate the world to camera matrix
*
* @param (void)
* @return                       the matrix of the world to camera transformation
*/
Matrix4 Airplane::WorldToCamera_Orth()
{
    //Calulate the alignment matrix
    Vector4 v = camera_up;      v.Normalize();
    Vector4 w = -camera_view;   w.Normalize();
    Vector4 u = w.Cross(v); 
    
    //Turn the position of the camera into a vector
    Vector4 A(camera_position.x, camera_position.y, camera_position.z, camera_position.w);

    //Generate the matrix
    Matrix4 align  (-u.x, -u.y, -u.z,   (u.Dot(A)),
                     v.x,  v.y,  v.z,  -(v.Dot(A)),
                     w.x,  w.y,  w.z,  -(w.Dot(A)),
                     0.f,  0.f,  0.f,   1.f);

    return align;
}



/**
* @brief tensor_product:    calculate the tensor product of two vectors
*
* @param u:                 first vector
* @param v:                 second vector
* @return                   result of the tensor product
*/
Matrix4 Airplane::tensor_product(Vector4 u, Vector4 v)
{
    Matrix4 tensor;
    tensor.Identity();

    //Do the multiplication
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

/**
* @brief get_matrix:    get the matrix of the vector
*
* @param u:             vector to get the matrix for
* @return               matrix
*/
Matrix4 Airplane::get_matrix(Vector4 u)
{
    //Generate the matrix
    Matrix4 m(  0.f, -u.z, u.y, 0.f,
                u.z, 0.f, -u.x, 0.f,
                -u.y, u.x, 0.f, 0.f,
                0.f, 0.f, 0.f, 1.f);

    return m;
}

/**
* @brief AxisAngleMethod:   calculate the axis angle method rotation matrix
*
* @param angle:             angle to rotate object by
* @param vec:               vector to rotate around
* @return                   rotation matrix
*/
Matrix4 Airplane::AxisAngleMethod(float angle, Vector4 vec)
{
    //Identity matrix
    Matrix4 identity; identity.Identity();

    //Calculate the rotation through the axis angle method
    Matrix4 AxisAngle;
    AxisAngle.Identity();
    AxisAngle = identity * cosf(angle)
                + tensor_product(vec, vec) * (1 - cosf(angle))
                + get_matrix(vec) * sinf(angle);

    return AxisAngle;
}



/**
* @brief FindObject:    get the transform of the object
*
* @param obj:           name of the object
* @return               transform of the object
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

/**
* @brief ModelToWorld:  calculate the model to world matrix of the object
*
* @param obj:           object to calculate the matrix for
* @param scale:         whether to calculate the scale
* @return               model to world matrix
*/
Matrix4 Airplane::ModelToWorld(CS250Parser::Transform& obj, bool scale)
{
    //Translation
    Matrix4 Transl;
    {
        Transl.Identity();
        Transl.m[0][3] = obj.pos.x;
        Transl.m[1][3] = obj.pos.y;
        Transl.m[2][3] = obj.pos.z;
    }


    //Axis angle method
    Matrix4 AxisAngle_FWD   = AxisAngleMethod(obj.rot.z, obj.fwd);
    Matrix4 AxisAngle_UP    = AxisAngleMethod(obj.rot.y, obj.up);
    Matrix4 AxisAngle_RIGHT = AxisAngleMethod(obj.rot.x, obj.right);
    Matrix4 AxisAngle = AxisAngle_FWD * AxisAngle_UP * AxisAngle_RIGHT;

    //Update the vectors of the object
    obj.fwd     = AxisAngle * obj.fwd;
    obj.up      = AxisAngle * obj.up;
    obj.right   = AxisAngle * obj.right;
    

    //Rotation
    Matrix4 Rot = OrthogonalMethod(obj);

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

    //Save the m2w of the object without the scale
    //to avoid modifying the vectors of the object every time this function is called
    obj.m2w = Transl * Rot;


    //If there is a parent, multiply its M2W matrix
    CS250Parser::Transform* parent = FindObject(obj.parent);
    while (parent)
    {
        m2w = parent->m2w * m2w;
        parent = FindObject(parent->parent);
    }


    return m2w;
}

/**
* @brief OrthogonalMethod:  calculate the orthogonal rotation matrix of an object
*
* @param obj:               object to calculate the matrix for
* @return                   orthogonal rotation matrix
*/
Matrix4 Airplane::OrthogonalMethod(CS250Parser::Transform& obj)
{
    //Get the vectors of the object
    Vector4 u = -obj.right;     u.Normalize();
    Vector4 v = obj.up;         v.Normalize();
    Vector4 w = obj.fwd;        w.Normalize();
    
    //Create the metrix
    Matrix4 orth(   u.x, v.x, w.x, 0.f,
                    u.y, v.y, w.y, 0.f,
                    u.z, v.z, w.z, 0.f,
                    0.f, 0.f, 0.f, 1.f);

    return orth;
}



/**
* @brief GetInput:  change airplane with input from user
*
* @return           whether to draw the airplane as solid
*/
unsigned Airplane::GetInput()
{
    CS250Parser::Transform* body = FindObject("body");

    //Roll airplane body
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        body->rot.z = ROT_ANGLE;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        body->rot.z = -ROT_ANGLE;
    }
    else
    {
        body->rot.z = 0.f;
    }


    //Yaw airplane body 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
        body->rot.y = ROT_ANGLE;
    }   
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        body->rot.y = -ROT_ANGLE;
    }
    else
    {
        body->rot.y = 0.f;
    }


    //Pitch airplane body 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        body->rot.x = ROT_ANGLE;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        body->rot.x = -ROT_ANGLE;
    }
    else
    {
        body->rot.x = 0.f;
    }

    //Move airplane forward
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        //Move body
        body->pos += body->fwd * 2.f;
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
        if (parser->distance - MOVE_DIST > 0.f)
            parser->distance -= MOVE_DIST;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
        parser->distance += MOVE_DIST;


    //Camera height
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
    {
        if(parser->height - MOVE_DIST > 0.f)
            parser->height -= MOVE_DIST;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
        parser->height += MOVE_DIST;


    return draw_mode;
}
