#pragma once
#include <glm/glm.hpp>

namespace HAUtilities {
    using namespace glm;
// Default camera values
    class FPSCamera {
    public:
        static const float CAMERA_DEFAULT_YAW;
        static const float CAMERA_DEFAULT_DEFAULT_PITCH;
        static const float CAMERA_DEFAULT_SPEED;
        static const float CAMERA_DEFAULT_SENSITIVITY;
        static const float CAMERA_DEFAULT_FOV;
        static const float CAMERA_DEFAULT_MAX_PITCH;
        static const float CAMERA_DEFAULT_MIN_PITCH;
        static const float CAMERA_DEFAULT_MIN_YAW;
        static const float CAMERA_DEFAULT_MAX_YAW;
        static const float CAMERA_DEFAULT_MIN_FOV;
        static const float CAMERA_DEFAULT_MAX_FOV;


        FPSCamera(vec3 camPos, vec3 worldUp, float yaw, float pitch, float fov, float width, float height, float nearPlane, float farPlane,
                            float speed = CAMERA_DEFAULT_SPEED, float sensitivity = CAMERA_DEFAULT_SENSITIVITY, 
                            float minPitch = CAMERA_DEFAULT_MIN_PITCH, float maxPitch = CAMERA_DEFAULT_MAX_PITCH,
                            float minYaw = CAMERA_DEFAULT_MIN_YAW, float maxYaw = CAMERA_DEFAULT_MAX_YAW,
                            float minFov = CAMERA_DEFAULT_MIN_FOV, float maxFov = CAMERA_DEFAULT_MAX_FOV);

        mat4 GetViewMatrix();
        mat4 GetProjectionMatrix();
        void MoveForward(float deltaTime);
        void MoveBackward(float deltaTime);
        void MoveStrafeLeft(float deltaTime);
        void MoveStrafeRight(float deltaTime);
        void Rotate(float mouseXOffset, float mouseYOffset);
        void Zoom(float fovOffset);

    private:
        // Camera Attributes
        vec3 position;
        vec3 forward;
        //vec3 up;
        vec3 right;
        vec3 worldUp;
        float yaw, pitch, minPitch, maxPitch, minYaw, maxYaw;
        mat4 viewMat;
        float fov, width, height, nearPlane, farPlane, minFov, maxFov, speed, sensitivity;
        mat4 projectionMat;
        void UpdateCoordinateVectors();
        void UpdateViewMatrix();
        void UpdateProjectionMatrix();
    };
}
