//
//  Camera.h
//  softRender
//
//  Created by zpf on 15-1-16.
//  Copyright (c) 2015年 zpf. All rights reserved.
//

#ifndef __softRender__Camera__
#define __softRender__Camera__

#include <stdio.h>
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include <SDL/SDL.h>

class Camera{
    Camera();
    ~Camera();
    
    void LookAt();
    void SetViewport(int x, int y, int width, int height);
    void SetClippingPlanes(float zNear, float zFar);
    void SetFOV(float fovRadians);
    void Update(float dt);
    
    glm::vec3 Position() { return position; }
    glm::vec4 Forward() { return forward; }
    
    glm::mat4 &ViewMatrix() { return view; }
    glm::mat4 &ProjectionMatrix() { return projection; }
    glm::mat4 &ViewProjetionMatrix() { return viewProjection; }
    
    void CaptureKeyboardState();
    void CaptureMouseState();
    
    int x;
    int y;
    int width;
    int height;
    
    float fov;
    float nearPlane;
    float farPlane;
    float aspect;
    
    glm::vec3 position;
    
    glm::vec3 velocity;
    
    glm::vec4 forward;
    glm::vec4 up;
    glm::vec4 right;
    
    float elevation;
    
    glm::quat orientation;
    
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 viewProjection;
};

#endif /* defined(__softRender__Camera__) */
