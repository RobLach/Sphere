#pragma once
#include "SphInclude.h"

extern void RenderAxis(float axisSize = 1.0f);
extern void RenderLine(cfloat* point1, cfloat* point2);
extern void RenderSphere(float radius = 1.0f);
extern void RenderIndexedVertices(int indexCount, cushort* m_indices, cfloat* m_vertices);
extern void RenderGuiLine(cfloat* color, float x1, float y1, float x2, float y2, float lineWidth = 1.0f);
extern void RenderGuiImage(uint texture, cfloat* leftXY, cfloat* rightXY);
extern void RenderGuiFrame(cfloat* color, cfloat* leftXY, cfloat* rightXY, float lineWidth);
