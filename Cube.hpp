//
// Created by Flo on 07/04/2023.
//

#pragma once

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <string>
#include <valarray>
#include "Texture.hpp"
#include "Shader.hpp"
#include "Color.hpp"

template<typename T>
struct vertex_struct_cube
{
    vertex_struct_cube(const Point3d<T>& p, const Color<T>& c)
        : p(p)
        , c(c)
    {}

    Point3d<T> p;
    Color<T> c;
};


template<typename T>
class Cube
{
public:
    Cube()
        : m_vao(0)
        , m_vbo(0)
        , m_program(0)
        , m_nVertices(6)
    {
        load();
    }

    ~Cube()
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

        Point3d<T> P000 = { -1, -1, -1 };
        Point3d<T> P001 = { -1, -1, +1 };
        Point3d<T> P010 = { -1, +1, -1 };
        Point3d<T> P011 = { -1, +1, +1 };
        Point3d<T> P100 = { +1, -1, -1 };
        Point3d<T> P101 = { +1, -1, +1 };
        Point3d<T> P110 = { +1, +1, -1 };
        Point3d<T> P111 = { +1, +1, +1 };

        Color<T> vr = { 1, 0, 0, 1 }; Color<T> vg = { 0, 1, 0, 1 }; Color<T> vb = { 0, 0, 1, 1 };
        Color<T> ar = { 0, 1, 1, 1 }; Color<T> ag = { 1, 0, 1, 1 }; Color<T> ab = { 1, 1, 0, 1 };

        using vt = vertex_struct_cube<T>;

        std::array<vt, 3 * 12> points = {
                // Face 1
                vt{ P000, vr }, vt{ P001, vr }, vt{ P011, vr },
                vt{ P000, vr }, vt{ P011, vr }, vt{ P010, vr },
                // Face 2
                vt{ P100, vg }, vt{ P101, vg }, vt{ P111, vg },
                vt{ P100, vg }, vt{ P111, vg }, vt{ P110, vg },
                // Face 3
                vt{ P000, vb }, vt{ P001, vb }, vt{ P101, vb },
                vt{ P000, vb }, vt{ P101, vb }, vt{ P100, vb },
                // Face 4
                vt{ P010, vb }, vt{ P011, vb }, vt{ P111, vb },
                vt{ P010, vb }, vt{ P111, vb }, vt{ P110, vb },
                // Face 5
                vt{ P000, vb }, vt{ P010, vb }, vt{ P110, vb },
                vt{ P000, vb }, vt{ P110, vb }, vt{ P100, vb },
                // Face 6
                vt{ P001, ag }, vt{ P011, ag }, vt{ P111, ag },
                vt{ P001, ag }, vt{ P111, ag }, vt{ P101, ag },
        };

        m_nVertices = points.size();

        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points.data(), GL_STATIC_DRAW);

        ShaderInfo shader[] = {
                { GL_VERTEX_SHADER, "cube.vert" },
                { GL_FRAGMENT_SHADER, "cube.frag" },
                { GL_NONE, nullptr }
        };

        auto program = Shader::loadShaders(shader);
        glUseProgram(program);
        m_program = program;

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_struct_cube<T>), 0); //Pour 3D
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_struct_cube<T>), (char*)(0) + sizeof(vertex_struct_cube<T>::p));
        glEnableVertexAttribArray(1);

        glEnable(GL_DEPTH_TEST);
    }

    void release() {}

    void render(const Mat4<T>& viewProjection)
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

        Mat4<T> mvp = viewProjection * model;

        auto mvpLocation = glGetUniformLocation(m_program, "model");
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, mvp.data());

        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_nVertices));
    }

    void update(float dt)
    {
        m_angle += 2.5f * dt;
    }

private:
    T m_angle = 0.0f;
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_program;
    GLsizei m_nVertices;
};
