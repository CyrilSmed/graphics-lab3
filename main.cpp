#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <glm/glm.hpp>
#include "PipelineHandler.h"

GLuint VBO;
GLuint globalLocation;

const GLchar pVS[] = "#version 330\n layout (location = 0) in vec3 Position; uniform mat4 gWorld; void main() {gl_Position = gWorld * vec4(Position, 1.0);}";
const GLchar pFS[] = "#version 330\n out vec4 FragColor; void main() {FragColor = vec4(0.8, 0.8, 0.8, 1.0);}";

const float WINDOW_WIDTH = 1000;
const float WINDOW_HEIGHT = 800;

float Scale = 0.0f;

static void renderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT);
    Scale += 0.001f;

    PipelineHandler pipeline;

    pipeline.setPosition(cos(Scale) * 0.25f, sin(Scale) * 0.065f, sin(Scale * 50) * 0.02f);
    pipeline.setScale(sin(Scale)*0.2f + 1.0f, cos(Scale) * 0.2f + 1.0f, 1.0f);
    pipeline.setRotation((int)(Scale * 100) % 360, 0, 0);
    pipeline.setPerspective(
        30.0f,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        1.0f,
        100.0f);
    pipeline.setCamera(glm::vec3({ 0.0f,0.0f,0.0f }),
        glm::vec3({ 0.8f,0.8f ,2.0f }),
        glm::vec3({ 0.0f,1.0f ,0.0f }));

    glUniformMatrix4fv(globalLocation, 1, GL_TRUE, (const GLfloat*)pipeline.getTransformationMatrix());

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glDrawArrays(GL_TRIANGLES, 0, 9);

    glDisableVertexAttribArray(0);

    glutSwapBuffers();
}

void createShader(GLuint* shaderprog, GLenum shaderType, const GLchar* shaderText)
{
    GLuint ShaderObj = glCreateShader(shaderType);
    const GLchar* p[1];
    p[0] = shaderText;
    GLint Lengths[1];
    Lengths[0] = strlen(shaderText);
    glShaderSource(ShaderObj, 1, p, Lengths);

    glCompileShader(ShaderObj);

    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, sizeof(InfoLog), NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", shaderType, InfoLog);
    }

    glAttachShader(*shaderprog, ShaderObj);

    GLint Success;
    glGetProgramiv(*shaderprog, GL_LINK_STATUS, &Success);
    if (Success == 0) {
        fprintf(stderr, "Error linking shader program\n");
    }

    glValidateProgram(*shaderprog);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Laboratory Work 2");

    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    glutDisplayFunc(renderSceneCB);
    glutIdleFunc(renderSceneCB);

    glClearColor(0.05f, 0.15f, 0.2f, 0.0f);

    glm::vec4 vecs[9] = {
        {0.3f, 0.3f, 11.5f, 1.0f},
        {0.5f, 0.5f, 12.0f, 1.0f},
        {-0.1f, 0.5f, 12.5f, 1.0f},

        {0.0f, -0.25f, 11.0f, 1.0f},
        {0.0f, 0.25f, 12.0f, 1.0f},
        {0.0f, -0.25f, 13.0f, 1.0f},

        {-0.3f, -0.3f, 12.5f, 1.0f},
        {-0.5f, -0.5f, 12.0f, 1.0f},
        {0.1f, -0.5f, 11.5f, 1.0f}
    };

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vecs), vecs, GL_DYNAMIC_DRAW);

    GLuint shaderprog = glCreateProgram();

    createShader(&shaderprog, GL_VERTEX_SHADER, pVS);
    createShader(&shaderprog, GL_FRAGMENT_SHADER, pFS);

    glLinkProgram(shaderprog);
    glUseProgram(shaderprog);

    globalLocation = glGetUniformLocation(shaderprog, "gWorld");

    glutMainLoop();
    return 0;
}