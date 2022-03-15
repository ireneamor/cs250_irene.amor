
#include "Math/Point4.h"

namespace Rasterizer
{

struct Vertex
{
    Point4 position;
    Point4 color;
};

void DrawMidpointLine(const Vertex & v1, const Vertex & v2);

void DrawTriangleSolid(const Vertex & p0, const Vertex & p1, const Vertex & p2);

} // namespace Rasterize