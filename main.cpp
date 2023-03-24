#pragma once

#include <SFML/Window.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <array>
#include "Shader.hpp"

template<typename T>
struct Point2d
{
    explicit Point2d(const T x = 0, const T y = 0): x(x), y(y) {}
    Point2d(const Point2d& pt): x(pt.x), y(pt.y) {}

    T x;
    T y;
};

template<typename T>
struct Color
{
    explicit Color(const T& r_ = 0, const T& g_ = 0, const T& b_ = 0, const T& a_ = 0): r(r_), g(g_), b(b_), a(a_) {}
    Color(const Color& color): r(color.r), g(color.g), b(color.b), a(color.a) {}

    T r;
    T g;
    T b;
    T a;
};

template<typename T>
class Triangle
{
public:
    using point_type = Point2d<T>;

    Triangle(point_type pt1, point_type pt2, point_type pt3) : m_vao(0), m_vbo(0), m_vertices({ pt1, pt2, pt3 })
    {
        load();
    }
    explicit Triangle(const std::array<point_type, 3>& points) : m_vao(0), m_vbo(0), m_vertices(points)
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
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
    }

    void release() {}

    void render()
    {
        glBindVertexArray(m_vao);

        Color<T> c { 1.0f, 1.0f, 0.0f, 1.0f};
        auto sizeUniform = glGetUniformLocation(m_program, "mySuperColor");
        glUniform4f(sizeUniform, c.r, c.g, c.b, c.a);

        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_vertices.size()));
    }

private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_program;
    std::array<point_type, 3> m_vertices;
};



int main()
{
    // set version of opengl to 4.6
	const sf::ContextSettings context_settings(24, 8, 4, 4, 6);
    // crée la fenêtre
    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, context_settings);
    window.setVerticalSyncEnabled(true);

    // activation de la fenêtre
    window.setActive(true);

    // fucking lines of hell
    glewExperimental = GL_TRUE;
    if (glewInit())
        throw std::runtime_error("Error init glew");

    // chargement des ressources, initialisation des états OpenGL, ...
    using Point2f = Point2d<float>;
    using Trianglef = Triangle<float>;

    Point2f p0{ -0.9f, -0.9f };
    Point2f p1{ 0.9f, -0.9f };
    Point2f p2{ 0.9f, 0.9f };
    Trianglef triangle{ p0, p1, p2 };

    // la boucle principale
    bool running = true;
    while (running)
    {
        // gestion des évènements
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // on stoppe le programme
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                // on ajuste le viewport lorsque la fenêtre est redimensionnée
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }

        // effacement les tampons de couleur/profondeur
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // dessin...
        triangle.render();
        glFlush();
        // termine la trame courante (en interne, échange les deux tampons de rendu)
        window.display();
    }

    // libération des ressources...

    return 0;
}