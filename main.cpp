#include "SFML/Window.hpp"
#include "GL/glew.h"
#include "SFML/OpenGL.hpp"

#include <array>

template<typename T>
struct Point2d
{
    explicit Point2d(const T& _x = 0, const T& _y = 0)
            : x(_x)
            , y(_y)
    {}

    Point2d(const Point2d& pt)
            : x(pt.x)
            , y(pt.y)
    {}

    T x;
    T y;
};

template<typename Type>
class Triangle
{
public:
    using point_type = Point2d<Type>;

    Triangle(const point_type& p0, const point_type& p1, const point_type& p2)
            : m_vao(0)
            , m_vbo(0)
            , m_points{{p0, p1, p2}}
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
        // We want only one buffer with the id generated and stored in m_vao
        glGenVertexArrays(1, &m_vao);

        // create a new active VAO
        glBindVertexArray(m_vao);

        // we want only one buffer with the id generated and stored in m_vbo
        glGenBuffers(1, &m_vbo);

        // 1. create a new active VBO if doesn’t exist
        // 2. if the VBO is active it will make it active
        // 3. if binded to 0, OpenGL stops
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

        // Allocate storage size units of OpenGL
        // Copy data from client to server
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_points), m_points.data(), GL_STATIC_DRAW);


        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
    }

    void render()
    {
        glBindVertexArray(m_vao);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_points.size()));
    }

private:
    GLuint m_vao;
    GLuint m_vbo;
    std::array<point_type, 3> m_points;
};

int main()
{
    const sf::ContextSettings context_settings(24, 8, 4, 4, 6);
    // crée la fenêtre
    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, context_settings);
    window.setVerticalSyncEnabled(true);

    // activation de la fenêtre
    window.setActive(true);

    // chargement des ressources, initialisation des états OpenGL, ...

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

        // termine la trame courante (en interne, échange les deux tampons de rendu)
        window.display();
    }

    // libération des ressources...

    return 0;
}