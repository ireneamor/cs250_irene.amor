#include "CS250Parser.h"

float   CS250Parser::left;
float   CS250Parser::right;
float   CS250Parser::top;
float   CS250Parser::bottom;
float   CS250Parser::focal;
float   CS250Parser::nearPlane;
float   CS250Parser::farPlane;
float   CS250Parser::distance;
float   CS250Parser::height;
Point4  CS250Parser::position;
Vector4 CS250Parser::view;
Vector4 CS250Parser::up;

std::vector<Point4>            CS250Parser::vertices;
std::vector<CS250Parser::Face> CS250Parser::faces;
std::vector<Point4>            CS250Parser::colors;
std::vector<Point4>            CS250Parser::textureCoords;

std::vector<CS250Parser::Transform> CS250Parser::objects;

void CS250Parser::LoadDataFromFile(const char * filename)
{
    FILE * in;
    
    vertices.clear();
    faces.clear();
    colors.clear();
    textureCoords.clear();
    objects.clear();
    
    fopen_s(&in, filename, "r");
    if (!in)
    {
        printf("Could not open input file\n");
        exit(0);
    }

    
    fscanf_s(in, "camera\n{\n\tleft = %f\n\tright = %f\n\ttop = %f\n\tbottom = %f\n\tfocal = %f\n\tnear = %f\n\tfar = %f\n\tdistance = %f\n\theight = %f\n\tposition = %f,%f,%f\n\tview = %f,%f,%f\n\tup = %f,%f,%f\n}\n",
             &left,
             &right,
             &top,
             &bottom,
             &focal,
             &nearPlane,
             &farPlane,
             &distance,
             &height,
             &position.x, &position.y, &position.z,
             &view.x, &view.y, &view.z,
             &up.x, &up.y, &up.z);

    unsigned i, count;

    //
    fscanf_s(in, "vertexes\n{\n\tcount = %d\n\t", &count);
    for (i = 0; i < count; ++i)
    {
        Point4 position;
        fscanf_s(in, "\t%f,%f,%f,%f\n", &position.x, &position.y, &position.z, &position.w);
        vertices.push_back(position);
    }
    fscanf_s(in, "}\n");
    //

    //
    fscanf_s(in, "faces\n{\n\tcount = %d\n\t", &count);
    unsigned faceNum = count;
    for (i = 0; i < faceNum; i++)
    {
        Face face;
        fscanf_s(in, "\t%i,%i,%i\n", &face.indices[0], &face.indices[1], &face.indices[2]);
        faces.push_back(face);
    }
    fscanf_s(in, "}\n");
    //

    //
    fscanf_s(in, "facecolor\n{\n\t");
    for (i = 0; i < count; i++)
    {
        Point4 color;
        fscanf_s(in, "\t%f,%f,%f\n", &color.x, &color.y, &color.z);
        colors.push_back(color);
    }
    fscanf_s(in, "}\n");
    //

    //
    fscanf_s(in, "texturecoordinates\n{\n\t");
    for (i = 0; i < faceNum * 3; i++)
    {
        Point4 textCoord;
        textCoord.z = 0.0f;
        textCoord.w = 0.0f;
        fscanf_s(in, "\t%f,%f\n", &textCoord.x, &textCoord.y);
        textureCoords.push_back(textCoord);
    }
    fscanf_s(in, "}\n");
    //

    //
    fscanf_s(in, "scene\n{\n\tcount = %d\n", &count);
    unsigned objCount = count;
    for (i = 0; i < objCount; i++)
    {
        Transform transform;
        char      name[512]   = {0};
        char      parent[512] = {0};
        fscanf_s(in, "\t%s = ",
                 name, 512);
        fscanf_s(in, "T(%f,%f,%f), ",
                 &transform.pos.x, &transform.pos.y, &transform.pos.z);
        fscanf_s(in, "R(%f,%f,%f), ",
                 &transform.rot.x, &transform.rot.y, &transform.rot.z);
        fscanf_s(in, "S(%f,%f,%f), ",
                 &transform.sca.x, &transform.sca.y, &transform.sca.z);
        fscanf_s(in, "%s\n",
                 parent, 512);
        transform.name = name;
        transform.parent = parent;
        
        objects.push_back(transform);
    }
    fscanf_s(in, "}\n");
    //

    fclose(in);
}