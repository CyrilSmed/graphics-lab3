#pragma once
#include <glm/glm.hpp>

class PipelineHandler
{
public:
    PipelineHandler();

    void setPosition(float x, float y, float z);
    void setRotation(int degX, int degY, int degZ);
    void setScale(float x, float y, float z);

    void setPerspective(float fov, 
                        float width, float height,
                        float zNear, float zFar);
    void setCamera(const glm::vec3& pos, 
                   const glm::vec3& target, 
                   const glm::vec3& up);

    glm::mat4* getTransformationMatrix();

private:
    glm::mat4 getTranslationTransformation(float x, float y, float z);
    glm::mat4 getRotationTransformation();
    glm::mat4 getScaleTransformation();
    glm::mat4 getProjectionTransformation();
    glm::mat4 getCameraRotationTransformation(
        const glm::vec3& targetVec, 
        const glm::vec3& upVec);

    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;

    struct ProjectionProps
    {
        float fov;
        float width;
        float height;
        float zNear;
        float zFar;
    };
    ProjectionProps m_projection;

    struct CameraProps
    {
        glm::vec3 posVec;
        glm::vec3 targetVec;
        glm::vec3 upVec;
    };
    CameraProps m_camera;

    glm::mat4 m_transformation;
};