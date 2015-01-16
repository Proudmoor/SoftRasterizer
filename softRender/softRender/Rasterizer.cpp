//
//  Rasterizer.cpp
//  softRender
//
//  Created by zpf on 15-1-16.
//  Copyright (c) 2015年 zpf. All rights reserved.
//

#include "Rasterizer.h"
#include "Math.h"
#include "Color.h"


void Rasterizer::SetFrameBuffer(uint32_t *frameBuffer,
                           unsigned int width, unsigned int height)
{
    m_FrameBuffer = frameBuffer;
    m_Width = width;
    m_Height = height;
}

void Rasterizer::SetPixel(unsigned int x, unsigned int y, const Color &color)
{
    if(x >= m_Width || y >= m_Height)
        return;
    
    m_FrameBuffer[y * m_Width + x] = color.Color2Uint();
}

void Rasterizer::SetPixel(float x, float y, const Color &color)
{
    if(x < 0.0f || y < 0.0f)
        return;
    
    SetPixel((unsigned int)x, (unsigned int)y, color);
}

void Rasterizer::Clear()
{
    memset(m_FrameBuffer, 0, sizeof(uint32_t) * m_Height * m_Width);
}


Rasterizer::Rasterizer()
{
    
}

TriType Rasterizer::GetTriType(Vertex* p0, Vertex* p1, Vertex* p2)
{
    TriType type;
    if (FloatEqual(p0->v.y, p1->v.y))
    {
        type = TOP;
    }
    else if (FloatEqual(p1->v.y, p2->v.y))
    {
        type = BOTTOM;
    }
    else
    {
        if (p1->v.y > p2->v.y)
            type = RIGHT;
        else
            type = LEFT;
    }
    return type;
}

/* case 0  平顶三角形 TopTri
 *
 *  p0      p1
 *
 *      p2
 */
void Rasterizer::RasterizeTopTri(Vertex* p0, Vertex* p1, Vertex* p2,  Uint32** cb, float** zb, FillMode fm, ShadeMode sm)
{
    float xs, xe;   // 端点 start end
    float dxy_left, dxy_right;  // 左右边斜率倒数
    
    xs = p0->v.x;
    xe = p1->v.x;
    
    float deltaY = 1 / (p2->v.y - p0->v.y);
    
    dxy_left = deltaY * (p2->v.x - p0->v.x);
    dxy_right = deltaY * (p2->v.x - p1->v.x);
    float t = 0;
    
    // draw scanline
    for (int y = ROUND(p0->v.y); y <= ROUND(p2->v.y); ++y)
    {
        t = (y - p0->v.y) * deltaY;
        Color cxs = LERP(p0->c, p2->c, t);//deltaY * (p0->c * (p2->v.y - y) + p2->c * (y - p0->v.y));
        cxs.Clamp();
        cxs *= 255;
        
        Color cxe = LERP(p1->c, p2->c, t);//deltaY * (p1->c * (p2->v.y - y) + p2->c * (y - p1->v.y));
        cxe.Clamp();
        cxe *= 255;
        
        Color deltac(0, 0, 0);
        if (!FloatEqual(xs, xe))
            deltac = (cxe - cxs) / (xe - xs); // scanline color 变化率
        Color cxi = cxs;
        
        float zxs = LERPf(p0->v.z, p2->v.z, t);
        float zxe = LERPf(p1->v.z, p2->v.z, t);
        float deltaz = 0;
        if (!FloatEqual(xs, xe))
            deltaz = (zxe - zxs) / (xe - xs); //scanline depth 变化率
        float zxi = zxs;
        
        if ( fm == FILL_SOLID )
        {
            if ( sm == SHADE_FLAT )
            {
                for (int x = ROUND(xs); x <= ROUND(xe); ++x)
                {
                    if ( zb != nullptr ) // zbuffer 消隐
                    {
                        if ( zxi < zb[x][y] )
                        {
                            cb[x][y] = p0 -> c.Color2Uint();
                            zb[x][y] = zxi;
                        }
                        zxi += deltaz;
                    }
                    else
                    {
                        cb[x][y] = p0 -> c.Color2Uint();
                    }
                }
            }
            else if ( sm == SHADE_GOURAUD )
            {
                for (int x = ROUND(xs); x <= ROUND(xe); ++x)
                {
                    if ( zb != nullptr ) // zbuffer 消除隐
                    {
                        if ( zxi < zb[x][y] )
                        {
                            cb[x][y] = cxi.Color2Uint();
                            zb[x][y] = zxi;
                        }
                        zxi += deltaz;
                    }
                    else
                    {
                        cb[x][y] = cxi.Color2Uint();
                    }
                    cxi += deltac;
                }
            }
        }
        else if (fm == FILL_WIREFRAME)
        {
            if ( zb != nullptr ) // zbuffer 消隐
            {
                if ( zxs < zb[ROUND(xs)][y] )
                {
                    cb[ROUND(xs)][y] = cxs.Color2Uint();
                    zb[ROUND(xs)][y] = zxs;
                }
                if ( zxe < zb[ROUND(xe)][y] )
                {
                    cb[ROUND(xe)][y] = cxe.Color2Uint();
                    zb[ROUND(xe)][y] = zxe;
                }
            }
            else
            {
                cb[ROUND(xs)][y] = cxs.Color2Uint();
                cb[ROUND(xe)][y] = cxe.Color2Uint();
            }
        }
        
        xs += dxy_left;
        xe += dxy_right;
    }
}

/* case 1  平底三角形 Bottom Tri
 *
 *     p0
 *
 *  p2      p1
 */
void Rasterizer::RasterizeBottomTri(Vertex* p0, Vertex* p1, Vertex* p2, Uint32** cb, float** zb, FillMode fm, ShadeMode sm)
{
    float xs, xe;   //  start end
    float dxy_left, dxy_right;  // 斜率倒数
    
    xs = xe = p0->v.x;
    float deltaY = 1 / (p2->v.y - p0->v.y);
    
    dxy_left = (p2->v.x - p0->v.x)  * deltaY;
    dxy_right = (p1->v.x - p0->v.x) * deltaY;
    float t = 0;
    
    // »æÖÆÃ¿ÌõÉ¨ÃèÏß
    for (int y = ROUND(p0->v.y); y <= ROUND(p1->v.y); ++y)
    {
        t = (y - p0->v.y) * deltaY;
        Color cxs = LERP(p0->c, p2->c, t);//deltaY * (p0->c * (p2->v.y - y) + p2->c * (y - p0->v.y));
        cxs.Clamp();
        cxs *= 255;
        
        Color cxe = LERP(p0->c, p1->c, t);//deltaY * (p0->c * (p1->v.y - y) + p1->c * (y - p0->v.y));
        cxe.Clamp();
        cxe *= 255;
        
        Color deltac(0, 0, 0);
        if (!FloatEqual(xs, xe))
            deltac = (cxe - cxs) / (xe - xs); // 颜色沿扫描线变化率
        Color cxi = cxs;
        
        float zxs = LERPf(p0->v.z, p2->v.z, t);
        float zxe = LERPf(p0->v.z, p1->v.z, t);
        float deltaz = 0;
        if (!FloatEqual(xs, xe))
            deltaz = (zxe - zxs) / (xe - xs); // 深度沿扫描线变换率
        float zxi = zxs;
        
        if ( fm == FILL_SOLID )
        {
            if ( sm == SHADE_FLAT ) // flat shading, faceted look
            {
                for (int x = ROUND(xs); x <= ROUND(xe); ++x)
                {
                    if ( zb != nullptr ) // zbuffer消隐
                    {
                        if ( zxi < zb[x][y] )
                        {
                            cb[x][y] = p0 -> c.Color2Uint();
                            zb[x][y] = zxi;
                        }
                        zxi += deltaz;
                    }
                    else
                    {
                        cb[x][y] = p0 -> c.Color2Uint();
                    }
                }
            }
            else if ( sm == SHADE_GOURAUD ) // Gouraud shading, interpolation
            {
                for (int x = ROUND(xs); x <= ROUND(xe); ++x)
                {
                    if ( zb != nullptr ) // zbuffer消隐
                    {
                        if ( zxi < zb[x][y] )
                        {
                            cb[x][y] = cxi.Color2Uint();
                            zb[x][y] = zxi;
                        }
                        zxi += deltaz;
                    }
                    else
                    {
                        cb[x][y] = cxi.Color2Uint();
                    }
                    cxi += deltac;
                }
            }
        }
        else if (fm == FILL_WIREFRAME)
        {
            if ( zb != nullptr ) // zbuffer消隐
            {
                if ( zxs < zb[ROUND(xs)][y] )
                {
                    cb[ROUND(xs)][y] = cxs.Color2Uint();
                    zb[ROUND(xs)][y] = zxs;
                }
                if ( zxe < zb[ROUND(xe)][y] )
                {
                    cb[ROUND(xe)][y] = cxe.Color2Uint();
                    zb[ROUND(xe)][y] = zxe;
                }
            }
            else
            {
                cb[ROUND(xs)][y] = cxs.Color2Uint();
                cb[ROUND(xe)][y] = cxe.Color2Uint();
            }
        }
        
        xs += dxy_left;
        xe += dxy_right;
    }
}

/* case 2 左三角形 Left Tri
 *    p0
 *
 *  p        p1
 *
 *
 *  p2
 */
void Rasterizer::RasterizeLeftTri(Vertex* p0, Vertex* p1, Vertex* p2, Uint32** cb, float** zb, FillMode fm, ShadeMode sm)
{
    float xs, xe;   // start end
    float dxy_left, dxy_right;  // 左右边斜率倒数
    Vertex p;	// ·分割点
    float T; // ·分割点参数
    glm::vec3 p0p1, p0p2;
    
    // 计算分割点
    float deltaYp0p1 = 1 / (p1->v.y - p0->v.y);
    float deltaYp1p2 = 1 / (p2->v.y - p1->v.y);
    float deltaYp0p2 = 1 / (p2->v.y - p0->v.y);
    dxy_left = deltaYp0p2 * (p2->v.x - p0->v.x);
    dxy_right = deltaYp0p1 * (p1->v.x - p0->v.x);
    float t;
    
    //p0p2直线方程
    p0p2 = glm::vec3((p2->v - p0->v));// 方向
    p0p2 = glm::normalize(p0p2);
    p.v.y = p1->v.y;
    T = (p.v.y - p0->v.y) / p0p2.y;
    p.v.x = p0->v.x + T * p0p2.x;
    //p.c = deltaYp0p2 * (p0->c * (p2->v.y - p.v.y) + p2->c * (p.v.y - p0->v.y));
    p.c = LERP(p0->c, p2->c, T * deltaYp0p2);
    p.v.z = LERPf(p0->v.z, p2->v.z,  T);
    // Bottom Tri 平底三角形 p0p1p
    xs = xe = p0->v.x;
    // 绘制每条扫描线
    for (int y = ROUND(p0->v.y); y <= ROUND(p1->v.y); ++y)
    {
        t = deltaYp0p1 * (y - p0->v.y);
        Color cxs = LERP(p0->c, p.c, t);//deltaYp0p1 * (p0->c * (p.v.y - y) + p.c * (y - p0->v.y));
        cxs.Clamp();
        cxs *= 255;
        
        Color cxe = LERP(p0->c, p1->c, t);//deltaYp0p1 * (p0->c * (p1->v.y - y) + p1->c * (y - p0->v.y));
        cxe.Clamp();
        cxe *= 255;
        
        Color deltac(0, 0, 0);
        if (!FloatEqual(xs, xe))
            deltac = (cxe - cxs) / (xe - xs); // 颜色沿扫描线变化
        Color cxi = cxs;
        
        float zxs = LERPf(p0->v.z, p.v.z, t);
        float zxe = LERPf(p0->v.z, p1->v.z, t);
        float deltaz = 0;
        if (!FloatEqual(xs, xe))
            deltaz = (zxe - zxs) / (xe - xs); // 深度沿着扫描线变化
        float zxi = zxs;
        
        if ( fm == FILL_SOLID )
        {
            if ( sm == SHADE_FLAT )
            {
                for (int x = ROUND(xs); x <= ROUND(xe); ++x)
                {
                    if ( zb != nullptr ) // zbuffer消隐
                    {
                        if ( zxi < zb[x][y] )
                        {
                            cb[x][y] = p0 -> c.Color2Uint();
                            zb[x][y] = zxi;
                        }
                        zxi += deltaz;
                    }
                    else
                    {
                        cb[x][y] = p0 -> c.Color2Uint();
                    }
                }
            }
            else if ( sm == SHADE_GOURAUD )
            {
                for (int x = ROUND(xs); x <= ROUND(xe); ++x)
                {
                    if ( zb != nullptr ) // zbuffer消隐
                    {
                        if ( zxi < zb[x][y] )
                        {
                            cb[x][y] = cxi.Color2Uint();
                            zb[x][y] = zxi;
                        }
                        zxi += deltaz;
                    }
                    else
                    {
                        cb[x][y] = cxi.Color2Uint();
                    }
                    cxi += deltac;
                }
            }
        }
        else if (fm == FILL_WIREFRAME)
        {
            if ( zb != nullptr ) // zbuffer消隐
            {
                if ( zxs < zb[ROUND(xs)][y] )
                {
                    cb[ROUND(xs)][y] = cxs.Color2Uint();
                    zb[ROUND(xs)][y] = zxs;
                }
                if ( zxe < zb[ROUND(xe)][y] )
                {
                    cb[ROUND(xe)][y] = cxe.Color2Uint();
                    zb[ROUND(xe)][y] = zxe;
                }
            }
            else
            {
                cb[ROUND(xs)][y] = cxs.Color2Uint();
                cb[ROUND(xe)][y] = cxe.Color2Uint();
            }
        }
        
        xs += dxy_left;
        xe += dxy_right;
    }
    
    // Top Tri 平顶三角形 pp1p2
    //RasterizeTopTri(&p, p1, p2);
    xs = p.v.x;
    xe = p1->v.x;
    dxy_right = deltaYp1p2 * (p2->v.x - p1->v.x);
    // 绘制每条扫描线
    for (int y = ROUND(p1->v.y); y <= ROUND(p2->v.y); ++y)
    {
        t = deltaYp1p2 * (y - p.v.y);
        Color cxs = LERP(p.c, p2->c, t);//deltaYp1p2 * (p.c * (p2->v.y - y) + p2->c * (y - p.v.y));
        cxs.Clamp();
        cxs *= 255;
        
        Color cxe = LERP(p1->c, p2->c, t);//deltaYp1p2 * (p1->c * (p2->v.y - y) + p2->c * (y - p1->v.y));
        cxe.Clamp();
        cxe *= 255;
        
        Color deltac(0, 0, 0);
        if (!FloatEqual(xs, xe))
            deltac = (cxe - cxs) / (xe - xs); // 颜色沿扫描线变化率
        Color cxi = cxs;
        
        float zxs = LERPf(p.v.z, p2->v.z, t);
        float zxe = LERPf(p1->v.z, p2->v.z, t);
        float deltaz = 0;
        if (!FloatEqual(xs, xe))
            deltaz = (zxe - zxs) / (xe - xs); // 深度沿扫描线变换率
        float zxi = zxs;
        
        if ( fm == FILL_SOLID )
        {
            if ( sm == SHADE_FLAT )
            {
                for (int x = ROUND(xs); x <= ROUND(xe); ++x)
                {
                    if ( zb != nullptr ) // zbuffer消隐
                    {
                        if ( zxi < zb[x][y] )
                        {
                            cb[x][y] = p0->c.Color2Uint() ;
                            zb[x][y] = zxi;
                        }
                        zxi += deltaz;
                    }
                    else
                    {
                        cb[x][y] = p0->c.Color2Uint();
                    }
                }
            }
            else if ( sm == SHADE_GOURAUD )
            {
                for (int x = ROUND(xs); x <= ROUND(xe); ++x)
                {
                    if ( zb != nullptr ) // zbuffer消隐
                    {
                        if ( zxi < zb[x][y] )
                        {
                            cb[x][y] = cxi.Color2Uint();
                            zb[x][y] = zxi;
                        }
                        zxi += deltaz;
                    }
                    else
                    {
                        cb[x][y] = cxi.Color2Uint();
                    }
                    cxi += deltac;
                }
            }
        }
        else if (fm == FILL_WIREFRAME)
        {
            if ( zb != nullptr ) // zbuffer消隐
            {
                if ( zxs < zb[ROUND(xs)][y] )
                {
                    cb[ROUND(xs)][y] = cxs.Color2Uint();
                    zb[ROUND(xs)][y] = zxs;
                }
                if ( zxe < zb[ROUND(xe)][y] )
                {
                    cb[ROUND(xe)][y] = cxe.Color2Uint();
                    zb[ROUND(xe)][y] = zxe;
                }
            }
            else
            {
                cb[ROUND(xs)][y] = cxs.Color2Uint();
                cb[ROUND(xe)][y] = cxe.Color2Uint();
            }
        }
        
        xs += dxy_left;
        xe += dxy_right;
    }
}

/* case 3 右三角形
 *
 *     p0
 *
 *  p2      p
 *
 *
 *			p1
 */
void Rasterizer::RasterizeRightTri(Vertex* p0, Vertex* p1, Vertex* p2, Uint32** cb, float** zb, FillMode fm, ShadeMode sm)
{
    float xs, xe;   // start end
    float dxy_left, dxy_right;  //左右斜率倒数
    Vertex p;	// 分割点
    float T; // 分割点参数
    vec3 p0p1, p0p2;
    
    // 计算分割点
    float deltaYp0p2 = 1 / (p2->v.y - p0->v.y);
    float deltaYp0p1 = 1 / (p1->v.y - p0->v.y);
    float deltaYp1p2 = 1 / (p1->v.y - p2->v.y);
    dxy_left = deltaYp0p2 * (p2->v.x - p0->v.x);
    dxy_right = deltaYp0p1 * (p1->v.x - p0->v.x);
    float t; // 插值参数
    
    // p0p1直线参数方程
    p0p1 = vec3((p1->v - p0->v));// ·½Ïò
    p0p1 = normalize(p0p1);
    p.v.y = p2->v.y;
    T = (p.v.y - p0->v.y) / p0p1.y;
    p.v.x = p0->v.x + T * p0p1.x;
    //p.c = deltaYp0p1 * (p0->c * (p1->v.y - p.v.y) + p1->c * (p.v.y - p0->v.y));
    p.c = LERP(p0->c, p1->c, T * deltaYp0p1);
    p.v.z = LERPf(p0->v.z, p1->v.z,  T);
    //按平底三角形处理 p0 p p2
    xs = xe = p0->v.x;
    // 绘制每一条扫描线
    for (int y = ROUND(p0->v.y); y <= ROUND(p2->v.y); ++y)
    {
        t = deltaYp0p2 * (y - p0->v.y);
        Color cxs = LERP(p0->c, p2->c, t);//deltaYp0p2 * (p0->c * (p2->v.y - y) + p2->c * (y - p0->v.y));
        cxs.Clamp();
        cxs *= 255;
        
        Color cxe = LERP(p0->c, p.c, t);//deltaYp0p2 * (p0->c * (p.v.y - y) + p.c * (y - p0->v.y));
        cxe.Clamp();
        cxe *= 255;
        
        Color deltac(0, 0, 0);
        if (!FloatEqual(xs, xe))
            deltac = (cxe - cxs) / (xe - xs); // 颜色沿扫描线变化率
        Color cxi = cxs;
        
        float zxs = LERPf(p0->v.z, p2->v.z, t);
        float zxe = LERPf(p0->v.z, p.v.z, t);
        float deltaz = 0;
        if (!FloatEqual(xs, xe))
            deltaz = (zxe - zxs) / (xe - xs); // 深度沿扫描线变化率
        float zxi = zxs;
        
        if ( fm == FILL_SOLID )
        {
            if ( sm == SHADE_FLAT )
            {
                for (int x = ROUND(xs); x <= ROUND(xe); ++x)
                {
                    if ( zb != nullptr ) // zbuffer消隐
                    {
                        if ( zxi < zb[x][y] )
                        {
                            cb[x][y] = p0 ->c.Color2Uint();
                            zb[x][y] = zxi;
                        }
                        zxi += deltaz;
                    }
                    else
                    {
                        cb[x][y] = p0->c.Color2Uint();
                    }
                }
            }
            else if ( sm == SHADE_GOURAUD )
            {
                for (int x = ROUND(xs); x <= ROUND(xe); ++x)
                {
                    if ( zb != nullptr ) // zbuffer消隐
                    {
                        if ( zxi < zb[x][y] )
                        {
                            cb[x][y] = cxi.Color2Uint();
                            zb[x][y] = zxi;
                        }
                        zxi += deltaz;
                    }
                    else
                    {
                        cb[x][y] = cxi.Color2Uint();
                    }
                    cxi += deltac;
                }
            }
            
        }
        else if (fm == FILL_WIREFRAME)
        {
            if ( zb != nullptr ) // zbuffer消隐
            {
                if ( zxs < zb[ROUND(xs)][y] )
                {
                    cb[ROUND(xs)][y] = cxs.Color2Uint();
                    zb[ROUND(xs)][y] = zxs;
                }
                if ( zxe < zb[ROUND(xe)][y] )
                {
                    cb[ROUND(xe)][y] = cxe.Color2Uint();
                    zb[ROUND(xe)][y] = zxe;
                }
            }
            else
            {
                cb[ROUND(xs)][y] = cxs.Color2Uint();
                cb[ROUND(xe)][y] = cxe.Color2Uint();
            }
        }
        
        xs += dxy_left;
        xe += dxy_right;
    }
    
    // 按照平顶三角形处理 pp1p2
    xs = p2->v.x;
    xe = p.v.x;
    dxy_left = deltaYp1p2 * (p1->v.x - p2->v.x);
    // 绘制扫描线
    for (int y = ROUND(p2->v.y); y <= ROUND(p1->v.y); ++y)
    {
        t = deltaYp1p2 * (y - p2->v.y);
        Color cxs = LERP(p2->c, p1->c, t);//deltaYp1p2 * (p2->c * (p1->v.y - y) + p1->c * (y - p2->v.y));
        cxs.Clamp();
        cxs *= 255;
        
        Color cxe = LERP(p.c, p1->c, t);//deltaYp1p2 * (p1->c * (y - p.v.y) + p.c * (p1->v.y - y));
        cxe.Clamp();
        cxe *= 255;
        
        Color deltac(0, 0, 0);
        if (!FloatEqual(xs, xe))
            deltac = (cxe - cxs) / (xe - xs); // 颜色沿着扫描线变化率
        Color cxi = cxs;
        
        float zxs = LERPf(p2->v.z, p1->v.z, t);
        float zxe = LERPf(p.v.z, p1->v.z, t);
        float deltaz = 0;
        if (!FloatEqual(xs, xe))
            deltaz = (zxe - zxs) / (xe - xs); // 深度沿着扫描线变化率
        float zxi = zxs;
        
        if ( fm == FILL_SOLID )
        {
            if ( sm == SHADE_FLAT )
            {
                for (int x = ROUND(xs); x <= ROUND(xe); ++x)
                {
                    if ( zb != nullptr ) // zbuffer消隐
                    {
                        if ( zxi < zb[x][y] )
                        {
                            cb[x][y] = p0 -> c.Color2Uint();
                            zb[x][y] = zxi;
                        }
                        zxi += deltaz;
                    }
                    else
                    {
                        cb[x][y] = p0->c.Color2Uint();
                    }
                }
            }
            else if ( sm == SHADE_GOURAUD )
            {
                for (int x = ROUND(xs); x <= ROUND(xe); ++x)
                {
                    for (int x = ROUND(xs); x <= ROUND(xe); ++x)
                    {
                        if ( zb != nullptr ) // zbuffer消隐
                        {
                            if ( zxi < zb[x][y] )
                            {
                                cb[x][y] = cxi.Color2Uint();
                                zb[x][y] = zxi;
                            }
                            zxi += deltaz;
                        } 
                        else
                        {
                            cb[x][y] = cxi.Color2Uint();
                        }					
                        cxi += deltac;
                    }
                }
            }
        }
        else if (fm == FILL_WIREFRAME)
        {
            if ( zb != nullptr ) // zbuffer消隐
            {
                if ( zxs < zb[ROUND(xs)][y] )
                {
                    cb[ROUND(xs)][y] = cxs.Color2Uint();
                    zb[ROUND(xs)][y] = zxs;
                }
                if ( zxe < zb[ROUND(xe)][y] )
                {
                    cb[ROUND(xe)][y] = cxe.Color2Uint();
                    zb[ROUND(xe)][y] = zxe;
                }
            } 
            else
            {
                cb[ROUND(xs)][y] = cxs.Color2Uint();
                cb[ROUND(xe)][y] = cxe.Color2Uint();
            }
        }
        
        xs += dxy_left;
        xe += dxy_right;
    }
}


