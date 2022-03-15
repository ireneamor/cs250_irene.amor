#ifndef MATHUTILITIES_H
#define MATHUTILITIES_H

#include <limits>

// Macro definitions
#define isZero(x)     ((x < std::numeric_limits<float>::epsilon()) && (x > -std::numeric_limits<float>::epsilon()))
#define isEqual(x, y) (((x >= y) ? (x - y) : (y - x)) < std::numeric_limits<float>::epsilon())

#endif