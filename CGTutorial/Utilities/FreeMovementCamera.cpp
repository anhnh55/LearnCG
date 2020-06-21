#include <FreeMovementCamera.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace HAUtilities {
     const float FreeMovementCamera::CAMERA_DEFAULT_YAW = -90.0f;
     const float FreeMovementCamera::CAMERA_DEFAULT_DEFAULT_PITCH = 0.0f;
     const float FreeMovementCamera::CAMERA_DEFAULT_SPEED = 2.5f;
     const float FreeMovementCamera::CAMERA_DEFAULT_SENSITIVITY = 0.05f;
     const float FreeMovementCamera::CAMERA_DEFAULT_FOV = 45.0f;
     const float FreeMovementCamera::CAMERA_DEFAULT_MAX_PITCH = 89.0f;
     const float FreeMovementCamera::CAMERA_DEFAULT_MIN_PITCH = -89.0f;
     const float FreeMovementCamera::CAMERA_DEFAULT_MIN_YAW = -359.0f;
     const float FreeMovementCamera::CAMERA_DEFAULT_MAX_YAW = 359.0f;
     const float FreeMovementCamera::CAMERA_DEFAULT_MIN_FOV = 1.0f;
     const float FreeMovementCamera::CAMERA_DEFAULT_MAX_FOV = 89.0f;

     FreeMovementCamera::FreeMovementCamera(vec3 camPos, vec3 worldUp, float yaw, float pitch, float fov, float width, float height, float nearPlane, float farPlane, float speed, 
                                            float sensitivity, float minPitch, float maxPitch, float minYaw, float maxYaw, float minFov, float maxFov)
     {
         this->minPitch = minPitch;
         this->maxPitch = maxPitch;
         this->maxYaw = maxYaw;
         this->minYaw = minYaw;
         this->yaw = yaw;
         this->pitch = pitch;
         //if (yaw > maxYaw) {
         //    this->yaw = maxYaw;
         //}

         //if (yaw < minYaw) {
         //    this->yaw = minYaw;
         //}

         if (pitch > maxPitch) {
             this->pitch = maxPitch;
         }

         if (pitch < minPitch) {
             this->pitch = minPitch;
         }

         this->fov = fov;
         this->maxFov = maxFov;
         this->minFov = minFov;
         this->width = width;
         this->height = height;
         this->nearPlane = nearPlane;
         this->farPlane = farPlane;
         this->speed = speed;
         this->sensitivity = sensitivity;
         if (fov > maxFov) {
             this->fov = maxFov;
         }

         if (fov < minFov) {
             this->fov = minFov;
         }

         this->worldUp = worldUp;
         this->position = camPos;
         UpdateCoordinateVectors();
         UpdateViewMatrix();
         UpdateProjectionMatrix();
     }

     mat4 FreeMovementCamera::GetViewMatrix() const
     {
         return viewMat;
     }
     
     mat4 FreeMovementCamera::GetProjectionMatrix() const
     {
         return projectionMat;
     }

     void FreeMovementCamera::MoveForward(float deltaTime)
     {
         position += forward * speed * deltaTime;
         UpdateViewMatrix();
     }

     void FreeMovementCamera::MoveBackward(float deltaTime)
     {
         position -= forward * speed * deltaTime;
         UpdateViewMatrix();
     }
     
     void FreeMovementCamera::MoveStrafeLeft(float deltaTime)
     {
         position -= right * speed * deltaTime;
         UpdateViewMatrix();
     }
     
     void FreeMovementCamera::MoveStrafeRight(float deltaTime)
     {
         position += right * speed * deltaTime;
         UpdateViewMatrix();
     }

     void FreeMovementCamera::Rotate(float mouseXOffset, float mouseYOffset)
     {
        yaw += mouseXOffset*sensitivity;
        pitch += mouseYOffset*sensitivity;

        //if (yaw > maxYaw) {
        //    this->yaw = maxYaw;
        //}

        //if (yaw < minYaw) {
        //    this->yaw = minYaw;
        //}

        if (pitch > maxPitch) {
            this->pitch = maxPitch;
        }

        if (pitch < minPitch) {
            this->pitch = minPitch;
        }

        UpdateCoordinateVectors();
        UpdateViewMatrix();
     }

     void FreeMovementCamera::Zoom(float fovOffset)
     {
         fov += fovOffset;
         if (fov > maxFov) {
             this->fov = maxFov;
         }

         if (fov < minFov) {
             this->fov = minFov;
         }
         UpdateProjectionMatrix();
     }

     void FreeMovementCamera::UpdateCoordinateVectors()
     {
         forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
         forward.y = sin(glm::radians(pitch));
         forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
         forward = normalize(forward);
         right = normalize(glm::cross(forward, worldUp));
     }

     void FreeMovementCamera::UpdateViewMatrix()
     {
         viewMat = lookAt(position, position + forward, worldUp);
     }

     void FreeMovementCamera::UpdateProjectionMatrix()
     {
         projectionMat = perspective(radians(fov), width / height, nearPlane, farPlane);
     }

     vec3 FreeMovementCamera::GetPosition() const {
         return position;
     }

     vec3 FreeMovementCamera::GetForward() const {
         return forward;
     }
}