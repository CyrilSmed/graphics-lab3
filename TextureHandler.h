#pragma once

#include <GL/glew.h>
#include <Magick++.h>
#include <string>

class TextureHandler
{
public:
    TextureHandler(GLenum TextureTarget, const std::string& FileName);

    bool Load();

    void Bind(GLenum TextureUnit);

private:
    std::string m_fileName;
    GLenum m_textureTarget;
    GLuint m_textureObj;
    Magick::Image* m_pImage;
    Magick::Blob m_blob;
};

