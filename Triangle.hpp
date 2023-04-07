//
// Created by Flo on 07/04/2023.
//

#pragma once

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <stdexcept>
#include <valarray>
#include "Matrix.hpp"
#include "SFML/Graphics/Image.hpp"
#include "Shader.hpp"
#include "Texture.hpp"


template<typename T>
class Triangle
{
    using vertex_type = vertex_struct_texture_3D<T>;

public:
    Triangle(const vertex_type& pt1, const vertex_type& pt2, const vertex_type& pt3) : m_vao(0), m_vbo(0), m_vertices({ pt1, pt2, pt3 }), m_texture("moche2.bmp")
    {
        load();
    }
    explicit Triangle(const std::array<vertex_type, 3>& points) : m_vao(0), m_vbo(0), m_vertices(points)
    {
        load();
    }

    ~Triangle()
    {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
    }

    void load()
    {
        glGenVertexArrays(1, &m_vao);

        glBindVertexArray(m_vao);

        glGenBuffers(1, &m_vbo);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

        glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices.data(), GL_STATIC_DRAW);

        ShaderInfo shader[] = {
                { GL_VERTEX_SHADER, "triangles.vert" },
                { GL_FRAGMENT_SHADER, "triangles.frag" },
                { GL_NONE, nullptr }
        };

        auto program = Shader::loadShaders(shader);
        glUseProgram(program);
        m_program = program;

        // le 2 parce qu'on a 2 valeurs par points
        /*glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_type), 0);
        glEnableVertexAttribArray(0);*/ //Pour 2D
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_type), 0); //Pour 3D
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_type), (char*)(0) + sizeof(vertex_type::p));
        glEnableVertexAttribArray(1);
    }

    void release() {}

    void render(const Mat4<T>& viewprojection)
    {
        glBindVertexArray(m_vao);

        /*Color<T> c { 1.0f, 1.0f, 0.0f, 1.0f};
        auto sizeUniform = glGetUniformLocation(m_program, "mySuperColor");
        glUniform4f(sizeUniform, c.r, c.g, c.b, c.a);*/

        Mat4<T> model;
        //Rotation sur l'axe X
        /*model(0, 0) = std::cos(m_angle);
        model(0, 1) = std::sin(m_angle);
        model(1, 0) = -std::sin(m_angle);
        model(1, 1) = std::cos(m_angle);

         model(2, 2) = 1.0f;*/

        //Rotation sur l'axe Y
        model(0, 0) = std::cos(m_angle);
        model(0, 2) = std::sin(m_angle);
        model(2, 0) = -std::sin(m_angle);
        model(2, 2) = std::cos(m_angle);

        model(1, 1) = 1.0f;
        model(3, 3) = 1.0f;

        //Deplacement axe z
        model(2, 3) = -5.0f;

        Mat4<T> mvp = viewprojection * model;

        auto mvpLocation = glGetUniformLocation(m_program, "model");
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, mvp.data());

        m_texture.bind();

        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_vertices.size()));
    }

    void update(float dt)
    {
        m_angle += 2.5f * dt;
    }

private:
    T m_angle = 0.0f;
    Texture m_texture;
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_program;
    std::array<vertex_type, 3> m_vertices;
};
