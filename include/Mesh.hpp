#pragma once

#include <vector>
#include "glad/glad.h"

namespace mesh
{

// A quad (-1, -1) -- (1, 1) and the corresponding texture quad (0, 0) -- (1, 1)
// In other words this is standard mesh that allows you to draw a texture to the
// whole screen.
extern std::vector<GLfloat> screenQuadTx;

// A quad (0, -1) -- (1, 0) and the tetxrue quad (0, 0) -- (1, 1)
// The quad covers entire fourth (bottom-right) octant.
// May be needed if scaling should stretch the quad down and right.
extern std::vector<GLfloat> oct4QuadTx;

// A quad (0, 0) -- (1, 1) and the corresponding texture quad (0, 1) -- (1, 0)
extern std::vector<GLfloat> oct1QuadTxIy;

}