//
//  Render.h
//  softRender
//
//  Created by zpf on 15-1-16.
//  Copyright (c) 2015年 zpf. All rights reserved.
//

#ifndef __softRender__Render__
#define __softRender__Render__

#include <stdio.h>
#include <vector>

#include "Rasterizer.h"

#define CLEAR_TARGET  0x00000001l
#define CLEAR_ZBUFFER           0x00000002l  /* Clear target z buffer */
#define CLEAR_STENCIL           0x00000004l

enum ProjectionType
{
    PT_ORTHOGRAPHIC,
    PT_PERSPECTIVE
};

class Light;
class Material;
class Camera;

class SoftRenderer
{
public:
    SoftRenderer(void);
    ~SoftRenderer(void);
    
    void Initialize();
    void SetupMatrices();
    void SetupLights();
    void Render();
    
    //void SetStreamSource();
    void Clear(Uint32 flags, Uint32 color,float Z, Uint32 stencil);	// clear frame buffer
    
    void SetXAngle(float radian);
    void SetYAngle(float radian);
    
    /************************************************************************/
    /* Buffer Object                                                        */
    /************************************************************************/
    Vertex* VB;	// vertex buffer
    Vertex* TVB;	// temporary transformed vertex in post-projection space
    Uint16*	IB;	// index buffer
    
    // temp
    size_t m_uVertexNum;
    size_t m_uPrimtiveNum; // for now, primitive is triangle D3DPT_TRIANGLELIST
    
    /************************************************************************/
    /* Framebuffer (double buffering)                                       */
    /************************************************************************/
    Uint32**	FCB; // color buffer (front)
    float**		FZB; // depth buffer (front)
    
    Uint32**	BCB; // color buffer (back)
    float**		BZB; // depth buffer (back)
    
private:
    float m_fXAngle, m_fYAngle;
    /************************************************************************/
    /* Geometry                                                             */
    /************************************************************************/
    // Geometry Specification
    void CreateVertexBuffer();
    void DeleteVertexBuffer();
    void CreateIndexBuffer();
    void DeleteIndexBuffer();
    
    // Per-Vertex Operations (T&L)
    void TL(size_t vi);
    
    // Primitive Assembly
    // Primitive Processing (Clip/Projection/Viewport/Cull)
    
    // Rasterization
    
    // Fragment Processing (Texture/Fog)
    // Per-Fragment Operations, ROP (Alpha Test/Depth Test/Blending/Dithering/Logic Operations)
    // Framebuffer Operations
    
    /************************************************************************/
    /* Image                                                                */
    /************************************************************************/
    
private:
    ProjectionType m_eProjType;
    
    vec3				m_ViewerPos;
    float					m_Ka;			// ambient
    std::vector<Light*>		m_Lights;
    Material*				m_pM;
    Camera*					m_pCam;
    Rasterizer*				m_pRas;
    
    /************************************************************************/
    /* Transformation matrices                                              */
    /************************************************************************/
    mat4 m_world;	// world transformation	: object space	--> world space
    mat4 m_view;	// viewing transformation	: world space	--> eye space
    mat4 m_proj;	// projection transformation: eye space		--> clip space
    mat4 m_port;	// viewport transformation  : clip space    --> screen space
    
    /************************************************************************/
    /* Render state                                                         */
    /************************************************************************/
    FillMode m_eFillMode;
    ShadeMode m_eShadeMode;
    bool m_bLighting;
    bool m_bCull;
    bool m_bTexture;
    bool m_bFog;
    bool m_bZEnable;
    bool m_bBlend;
    
    bool m_bZWrite;
};




















#endif /* defined(__softRender__Render__) */
