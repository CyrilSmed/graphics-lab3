#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <glm/glm.hpp>
#include "PipelineHandler.h"
#include "TextureHandler.h"

GLuint VBO;
GLuint IBO;
GLuint globalLocation;
GLuint globalSampler;

TextureHandler* texture = NULL;

const GLchar pVS[] = "#version 330\n layout (location = 0) in vec3 Position; uniform mat4 gWorld; void main() {gl_Position = gWorld * vec4(Position, 1.0);}";
const GLchar pFS[] = "#version 330\n out vec4 FragColor; void main() {FragColor = vec4(0.8, 0.8, 0.8, 1.0);}";

const float WINDOW_WIDTH = 1000;
const float WINDOW_HEIGHT = 800;

float Scale = 0.0f;

struct vertex {
    glm::vec3 fst;
    glm::vec2 snd;

    vertex(glm::vec3 inp1, glm::vec2 inp2) {
        fst = inp1;
        snd = inp2;
    }
};

static void renderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT);
    Scale += 0.001f;

    PipelineHandler pipeline;

    //pipeline.setPosition(cos(Scale) * 0.25f, sin(Scale) * 0.065f, sin(Scale * 50) * 0.02f);
    //pipeline.setScale(sin(Scale)*0.2f + 1.0f, cos(Scale) * 0.2f + 1.0f, 1.0f);
    //pipeline.setRotation((int)(Scale * 100) % 360, 0, 0);
    pipeline.setPerspective(
        60.0f,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        1.0f,
        100.0f);
    pipeline.setCamera(glm::vec3({ 0.0f,0.0f,0.0f }),
        glm::vec3({ 0.8f,0.8f ,2.0f }),
        glm::vec3({ 0.0f,1.0f ,0.0f }));

    glUniformMatrix4fv(globalLocation, 1, GL_TRUE, (const GLfloat*)pipeline.getTransformationMatrix());

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (const GLvoid*)12);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    texture->Bind(GL_TEXTURE0);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

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
    glutCreateWindow("Laboratory Work 3");

    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }

    glutDisplayFunc(renderSceneCB);
    glutIdleFunc(renderSceneCB);

    glClearColor(0.05f, 0.15f, 0.2f, 0.0f);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    //glm::vec4 vecs[9] = {
    //    {0.3f, 0.3f, 11.5f, 1.0f},
    //    {0.5f, 0.5f, 12.0f, 1.0f},
    //    {-0.1f, 0.5f, 12.5f, 1.0f},

    //    {0.0f, -0.25f, 11.0f, 1.0f},
    //    {0.0f, 0.25f, 12.0f, 1.0f},
    //    {0.0f, -0.25f, 13.0f, 1.0f},

    //    {-0.3f, -0.3f, 12.5f, 1.0f},
    //    {-0.5f, -0.5f, 12.0f, 1.0f},
    //    {0.1f, -0.5f, 11.5f, 1.0f}
    //};

    vertex input[4] = {
        vertex(glm::vec3 {-1.0f, -1.0f, 0.5773f}, glm::vec2 {0.0f, 0.0f}),
        vertex(glm::vec3 {0.0f, -1.0f, -1.1547}, glm::vec2 {0.0f, 0.0f}),
        vertex(glm::vec3 {1.0f, -1.0f, 0.5773f}, glm::vec2 {1.0f, 0.0f}),
        vertex(glm::vec3 {0.0f, 1.0f, 0.0f}, glm::vec2 {0.0f, 1.0f}),
    };

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(input), input, GL_STATIC_DRAW);

    unsigned int Indices[] = { 0, 3, 1,
                       1, 3, 2,
                       2, 3, 0,
                       1, 2, 0 };

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

    GLuint shaderProgram = glCreateProgram();

    createShader(&shaderProgram, GL_VERTEX_SHADER, pVS);
    createShader(&shaderProgram, GL_FRAGMENT_SHADER, pFS);

    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    globalLocation = glGetUniformLocation(shaderProgram, "gWVP");
    globalSampler = glGetUniformLocation(shaderProgram, "gSampler");

    glUniform1i(globalSampler, 0);
    Magick::InitializeMagick(nullptr);
    texture = new TextureHandler(GL_TEXTURE_2D, "Content/test.png");

    if (!texture->Load()) {
        std::cout << "error";
        return 1;
    }

    glutMainLoop();
    return 0;
}