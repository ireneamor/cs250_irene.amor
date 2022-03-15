
#include "Rasterizer.h"
#include "FrameBuffer.h"

namespace Rasterizer
{

int Round(float f)
{
    int i = static_cast<int>(f + 0.5f);

    return i;
}

int Ceiling(float f)
{
    int i = static_cast<int>(f);

    if (static_cast<float>(i) == f)
        return i;

    return i + 1;
}

void DrawMidpointLine(const Vertex & v0, const Vertex & v1)
{
    int x = Round(v0.position.x);
    int y = Round(v0.position.y);

    int dx    = Round(v1.position.x) - x;
    int dy    = Round(v1.position.y) - y;
    int xStep = 1, yStep = 1;

    if (dx < 0)
    {
        xStep = -1;
        dx    = -dx;
    }
    if (dy < 0)
    {
        yStep = -1;
        dy    = -dy;
    }

    float length = (v1.position - v0.position).Length();

    float rInc = (v1.color.r - v0.color.r) / length;
    float gInc = (v1.color.g - v0.color.g) / length;
    float bInc = (v1.color.b - v0.color.b) / length;

    float r = v0.color.r;
    float g = v0.color.g;
    float b = v0.color.b;

    FrameBuffer::SetPixel(x, y, static_cast<unsigned char>(r * 255.99), static_cast<unsigned char>(g * 255.99), static_cast<unsigned char>(b * 255.99));

    if (abs(dy) > abs(dx)) // |m|>1
    {
        int dstart, dn, dne;
        dstart = 2 * dx - dy;
        dn     = dx;
        dne    = dx - dy;

        while (dy--)
        {
            y += yStep;

            if (dstart > 0)
            {
                dstart += dne;
                x += xStep;
            }
            else
                dstart += dn;

            FrameBuffer::SetPixel(x, y, static_cast<unsigned char>(r * 255.99), static_cast<unsigned char>(g * 255.99), static_cast<unsigned char>(b * 255.99));

            r += rInc;
            g += gInc;
            b += bInc;
        }
    }
    else // |m|<1
    {
        int dstart, de, dne;
        dstart = 2 * dy - dx;
        de     = dy;
        dne    = dy - dx;

        while (dx--)
        {
            x += xStep;

            if (dstart > 0)
            {
                dstart += dne;
                y += yStep;
            }
            else
                dstart += de;

            FrameBuffer::SetPixel(x, y, static_cast<unsigned char>(r * 255.99), static_cast<unsigned char>(g * 255.99), static_cast<unsigned char>(b * 255.99));

            r += rInc;
            g += gInc;
            b += bInc;
        }
    }
}

void DrawTriangleSolid(const Vertex & v0, const Vertex & v1, const Vertex & v2)
{
    // Select TOP, MIDDLE and BOTTOM vertices
    // --------------------------------------
    const Vertex *top, *middle, *bottom;
    bool          middle_is_left = false;

    if (v0.position.y < v1.position.y) // case 4, 5, or 6
    {
        if (v2.position.y < v0.position.y) // case 4
        {
            top            = &v2;
            middle         = &v0;
            bottom         = &v1;
            middle_is_left = true;
        }
        else
        {
            top = &v0;
            if (v1.position.y < v2.position.y) // case 5
            {
                middle         = &v1;
                bottom         = &v2;
                middle_is_left = true;
            }
            else // case 6
            {
                middle         = &v2;
                bottom         = &v1;
                middle_is_left = false;
            }
        }
    }
    else // case 1, 2 or 3
    {
        if (v2.position.y < v1.position.y) // case 2
        {
            top    = &v2;
            middle = &v1;

            bottom         = &v0;
            middle_is_left = false;
        }
        else
        {
            top = &v1;
            if (v0.position.y < v2.position.y) // case 3
            {
                middle         = &v0;
                bottom         = &v2;
                middle_is_left = false;
            }
            else // case 1
            {
                middle         = &v2;
                bottom         = &v0;
                middle_is_left = true;
            }
        }
    }

    // TOP to MIDDLE
    // -------------
    // Select LEFT and RIGHT edge endpoints to use from the top to the middle
    // ----------------------------------------------------------------------
    const Vertex *left, *right;
    if (middle_is_left)
    {
        left  = middle;
        right = bottom;
    }
    else
    {
        right = middle;
        left  = bottom;
    }

    // Calculate the x increments along the LEFT and RIGHT edges (the inverse of the slope
    float xIncLeft  = (left->position.x - top->position.x) / (left->position.y - top->position.y);
    float xIncRight = (right->position.x - top->position.x) / (right->position.y - top->position.y);

    // Initial values
    int   y    = Ceiling(top->position.v[1]);
    int   yMax = Ceiling(middle->position.v[1]) - 1;
    float xL   = top->position.v[0]; // xL MUST ALWAYS be a double, the ceiling is applied to x and xMax
    float xR   = top->position.v[0]; // xL MUST ALWAYS be a double, the ceiling is applied to x and xMax
    int   x;
    int   xMax = Ceiling(xR) - 1;

    // Initial Color Interpolation (Plane Equation Parameters)
    // RED
    float v1R[3] = {middle->position.x - top->position.x, middle->position.y - top->position.y, static_cast<float>(middle->color.r - top->color.r)};
    float v2R[3] = {bottom->position.x - top->position.x, bottom->position.y - top->position.y, static_cast<float>(bottom->color.r - top->color.r)};
    // GREEN
    float v1G[3] = {middle->position.x - top->position.x, middle->position.y - top->position.y, static_cast<float>(middle->color.g - top->color.g)};
    float v2G[3] = {bottom->position.x - top->position.x, bottom->position.y - top->position.y, static_cast<float>(bottom->color.g - top->color.g)};
    // BLUE
    float v1B[3] = {middle->position.x - top->position.x, middle->position.y - top->position.y, static_cast<float>(middle->color.b - top->color.b)};
    float v2B[3] = {bottom->position.x - top->position.x, bottom->position.y - top->position.y, static_cast<float>(bottom->color.b - top->color.b)};
    // NORMALS
    float nR[3] = {v1R[1] * v2R[2] - v1R[2] * v2R[1], v1R[2] * v2R[0] - v1R[0] * v2R[2], v1R[0] * v2R[1] - v1R[1] * v2R[0]};
    float nG[3] = {v1G[1] * v2G[2] - v1G[2] * v2G[1], v1G[2] * v2G[0] - v1G[0] * v2G[2], v1G[0] * v2G[1] - v1G[1] * v2G[0]};
    float nB[3] = {v1B[1] * v2B[2] - v1B[2] * v2B[1], v1B[2] * v2B[0] - v1B[0] * v2B[2], v1B[0] * v2B[1] - v1B[1] * v2B[0]};
    // Increments
    float rIncX = -nR[0] / nR[2];
    float rIncY = -nR[1] / nR[2];
    float gIncX = -nG[0] / nG[2];
    float gIncY = -nG[1] / nG[2];
    float bIncX = -nB[0] / nB[2];
    float bIncY = -nB[1] / nB[2];

    float rL = top->color.r;
    float gL = top->color.g;
    float bL = top->color.b;

    float r, g, b;

    // Start the loop, from the y_top to y_middle
    while (y <= yMax)
    {
        // Loop along the scanline, from left to right
        x    = Ceiling(xL);
        xMax = Ceiling(xR) - 1;

        r = rL;
        g = gL;
        b = bL;


        while (x <= xMax)
        {
            FrameBuffer::SetPixel(x, y, static_cast<unsigned char>(r * 255.99), static_cast<unsigned char>(g * 255.99), static_cast<unsigned char>(b * 255.99));

            ++x;

            r += rIncX;
            g += gIncX;
            b += bIncX;
        }

        xL += xIncLeft;
        xR += xIncRight;
        ++y;

        rL += rIncY + rIncX * xIncLeft;
        gL += gIncY + gIncX * xIncLeft;
        bL += bIncY + bIncX * xIncLeft;
    }

    // MIDDLE to BOTTOM
    // ----------------
    // Select LEFT and RIGHT edge endpoints to use from the top to the middle
    // ----------------------------------------------------------------------
    // Initial values
    y    = Ceiling(middle->position.y);
    yMax = Ceiling(bottom->position.y) - 1;

    if (middle_is_left)
    {
        xIncLeft = (bottom->position.x - left->position.x) / (bottom->position.y - left->position.y);
        xL       = left->position.x;

        rL = middle->color.r;
        gL = middle->color.g;
        bL = middle->color.b;
    }
    else
    {
        xIncRight = (bottom->position.x - right->position.x) / (bottom->position.y - right->position.y);
        xR        = right->position.x;
    }

    // Start the loop, from the y_top to y_middle
    while (y <= yMax)
    {
        x    = Ceiling(xL);
        xMax = Ceiling(xR) - 1;

        r = rL;
        g = gL;
        b = bL;

        // Loop along the scanline, from left to right
        while (x <= xMax)
        {
            FrameBuffer::SetPixel(x, y, static_cast<unsigned char>(r * 255.99), static_cast<unsigned char>(g * 255.99), static_cast<unsigned char>(b * 255.99));

            ++x;

            r += rIncX;
            g += gIncX;
            b += bIncX;
        }

        xL += xIncLeft;
        xR += xIncRight;
        ++y;

        rL += rIncY + rIncX * xIncLeft;
        gL += gIncY + gIncX * xIncLeft;
        bL += bIncY + bIncX * xIncLeft;
    }
}

} // namespace Rasterizer
