//
//  Rasterizer.h
//  softRender
//
//  Created by zpf on 15-1-16.
//  Copyright (c) 2015年 zpf. All rights reserved.
//

#ifndef __softRender__Rasterizer__
#define __softRender__Rasterizer__

#include <stdio.h>
#include "glm/glm.hpp"
#include "Color.h"
using namespace glm;

typedef struct tVertex{
    vec4 v;
    vec3 n;
    
    Color c;
} Vertex;

enum TriType {
    TOP, BOTTOM,
    LEFT, RIGHT
};

enum FillMode {
    FILL_POINT = 1,
    FILL_SOLID = 2,
    FILL_WIREFRAME =3,
};

enum ShadeMode
{
    SHADE_FLAT					= 1,
    SHADE_GOURAUD				= 2,
    SHADE_PHONG					= 3,
};
class Rasterizer {
protected:
    Uint32 *m_FrameBuffer;
    unsigned int m_Width, m_Height;
    
public:
    void SetFrameBuffer(Uint32 *frameBuffer, unsigned int width, unsigned int height);
    void SetPixel(unsigned int x, unsigned int y, const Color &color = Color());
    void SetPixel(float x, float y, const Color &color = Color());
    void Clear();
    
    Rasterizer();
    TriType GetTriType(Vertex* p0, Vertex* p1, Vertex* p2);
    // Uint32  is Color mask
    void RasterizeBottomTri(Vertex* p0, Vertex* p1, Vertex* p2, Uint32** cb, float** zb, FillMode fm, ShadeMode sm);
    void RasterizeTopTri(Vertex* p0, Vertex* p1, Vertex* p2, Uint32** cb, float** zb, FillMode fm, ShadeMode sm);
    void RasterizeLeftTri(Vertex* p0, Vertex* p1, Vertex* p2, Uint32** cb, float** zb, FillMode fm, ShadeMode sm);
    void RasterizeRightTri(Vertex* p0, Vertex* p1, Vertex* p2, Uint32** cb, float** zb, FillMode fm, ShadeMode sm);
    
    
    
    void DrawLine(const Color &color1, float x1, float y1,
                  const Color &color2, float x2, float y2);
    
    
};

#endif /* defined(__softRender__Rasterizer__) */
