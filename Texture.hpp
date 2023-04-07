//
// Created by Flo on 07/04/2023.
//

#pragma once

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <string>
#include <stdexcept>
#include "Matrix.hpp"
#include "SFML/Graphics/Image.hpp"

template<typename T>
struct vertex_struct_texture
{
    Point2d<T> p;
    Point2d<T> t;
};

template<typename T>
struct vertex_struct_texture_3D
{
    Point3d<T> p;
    Point2d<T> t;
};

struct Texture
{
    Texture(const std::string& filename) : m_texture(0)
    {
        load(filename);
    }

    ~Texture()
    {
        glDeleteTextures(1, &m_texture);
    }

    void load(const std::string& filename)
    {
        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        sf::Image image;
        if (!image.loadFromFile(filename))
            throw std::runtime_error("Cannot load texture");

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void bind() const
    {
        glBindTexture(GL_TEXTURE_2D, m_texture);
    }

    void unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    GLuint m_texture;
};
