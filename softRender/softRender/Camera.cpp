//
//  Camera.cpp
//  softRender
//
//  Created by zpf on 15-1-16.
//  Copyright (c) 2015年 zpf. All rights reserved.
//

#include "Camera.h"


#define CAMERA_LINEAR_SPEED 0.005f

Camera::Camera()
{
    SetViewport(0, 0, 640, 480);
    SetFOV(0.25f * M_PI);
    SetClippingPlanes(1.0f, 100.0f);
    
    position = glm::vec3(0.0f, 0.0f, 10.0f);
    velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    
    forward = glm::inverse(orientation) * glm::vec4(0,0,-1,1);
    up = glm::inverse(orientation) * glm::vec4(0,1,0,1);
    right = glm::inverse(orientation) * glm::vec4(1,0,0,1);
    
    elevation = 0.0f;
    
    orientation = orientation * glm::quat();
    
    view = glm::mat4();
    projection = glm::mat4();
}

Camera::~Camera() {}

void Camera::SetViewport(int x, int y, int width, int height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    
    aspect = width / (float)height;
}

void Camera::SetClippingPlanes(float zNear, float zFar)
{
    nearPlane = zNear;
    farPlane = zFar;
}

void Camera::SetFOV(float fovRadians)
{
    fov = fovRadians;
}

void Camera::Update(float dt)
{
    CaptureMouseState();
    CaptureKeyboardState();
    
    position += velocity * dt;
    
    forward = glm::normalize(glm::rotate(orientation, -glm::vec3::zAxis()));
    up = normalize(rotate(orientation, glm::vec3::yAxis()));
    right = normalize(rotate(orientation, glm::vec3::xAxis()));
    
    view = glm::mat4::lookAt(position, position + forward, up);
    projection = glm::mat4::perspective(fov, aspect, nearPlane, farPlane);
    viewProjection = projection * view;
}

void Camera::CaptureKeyboardState()
{
    const Uint8 *keys = SDL_GetKeyState(nullptr);
    
    if (keys[SDL_SCANCODE_W]) {
        velocity = forward * CAMERA_LINEAR_SPEED;
    } else if (keys[SDL_SCANCODE_S]) {
        velocity = -forward * CAMERA_LINEAR_SPEED;
    } else {
        velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    }
    
    if (keys[SDL_SCANCODE_A]) {
        velocity -= right * CAMERA_LINEAR_SPEED;
    } else if (keys[SDL_SCANCODE_D]) {
        velocity += right * CAMERA_LINEAR_SPEED;
    }
    
    if (keys[SDL_SCANCODE_SPACE]) {
        velocity -= up * CAMERA_LINEAR_SPEED;
    }
}

void Camera::CaptureMouseState()
{
    float maxElevation = 0.49f * (float)M_PI;
    
    int dx, dy;
    
    SDL_GetRelativeMouseState(&dx, &dy);
    
    float rotationX = dy / (float)height;
    float rotationY = dx / (float)width;
    
    elevation += rotationX;
    
    if (elevation > maxElevation) {
        rotationX = maxElevation - (elevation - rotationX);
        elevation = maxElevation;
    }
    
    if (elevation < -maxElevation) {
        rotationX = -maxElevation - (elevation - rotationX);
        elevation = -maxElevation;
    }
    
    if (rotationY != 0.0f) {
        orientation = Quat::rotation(-rotationY, glm::vec3::yAxis()) * orientation;
    }
    
    if (rotationX != 0.0f) {
        orientation = orientation * Quat::rotation(rotationX, glm::vec3::xAxis());
    }
    
    orientation = normalize(orientation);
}