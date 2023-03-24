#pragma once

#include <SFML/Window.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <array>
#include "Shader.hpp"

#include <SFML/Graphics/Image.hpp>

template<typename T>
struct Point2d
{
    explicit Point2d(const T x = 0, const T y = 0): x(x), y(y) {}
    Point2d(const Point2d& pt): x(pt.x), y(pt.y) {}

    T x;
    T y;
};

template<typename T>
struct Point3d
{
    explicit Point3d(const T x = 0, const T y = 0, const T z = 0): x(x), y(y), z(z) {}
    Point3d(const Point3d& pt): x(pt.x), y(pt.y), z(pt.z) {}

    T x;
    T y;
    T z;
};

template<typename T>
struct Mat4
{
    Mat4() { for(int i = 0; i < 16 ; ++i) m_data[i] = 0; }

    T& operator() (const int i, const int j) { return m_data[j * 4 + i]; }
    const T& operator() (const int i, const int j) const { return m_data[j * 4 + i]; }

    std::array<T, 16> m_data;
};

template<typename T>
Mat4<T> operator*(const Mat4<T>& lhs, const Mat4<T>& rhs)
{
    Mat4<T> result;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            for (int k = 0; k < 4; ++k)
            {
                result(i, j) += lhs(i, k) * rhs(k, j);
            }
        }
    }
    return result;
}

template<typename T>
struct Color
{
    Color(const T& r_ = 0, const T& g_ = 0, const T& b_ = 0, const T& a_ = 0): r(r_), g(g_), b(b_), a(a_) {}
    Color(const Color& color): r(color.r), g(color.g), b(color.b), a(color.a) {}

    T r = 0;
    T g = 0;
    T b = 0;
    T a = 0;
};

template<typename T>
struct vertex_struct_color
{
    Point2d<T> p;
    Color<T> c;
};

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
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &mvp.m_data[0]);

        m_texture.bind();

        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_vertices.size()));
    }

    void update()
    {
        m_angle += 0.025f;
    }

private:
    T m_angle = 0.0f;
    Texture m_texture;
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_program;
    std::array<vertex_type, 3> m_vertices;
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
    using Point3f = Point3d<float>;
    using Trianglef = Triangle<float>;

    /*vertex_struct_color<float> p0{ Point2f { -0.9f, -0.9f }, { 1.0f, 0.0f, 0.0f, 1.0f } };
    vertex_struct_color<float> p1{ Point2f { 0.9f, -0.9f }, { 0.0f, 1.0f, 0.0f, 1.0f } };
    vertex_struct_color<float> p2{ Point2f { 0.9f, 0.9f }, { 0.0f, 0.0f, 1.0f, 1.0f } };*/
    /*vertex_struct_texture<float> p0{ Point2f { -0.9f, -0.9f }, Point2f { -1.0f, -1.0f } };
    vertex_struct_texture<float> p1{ Point2f { 0.9f, -0.9f }, Point2f { 1.0f, -1.0f } };
    vertex_struct_texture<float> p2{ Point2f { 0.9f, 0.9f }, Point2f { 1.0f, 1.0f } };*/
    vertex_struct_texture_3D<float> p0{ Point3f { -0.9f, -0.9f, 0.f }, Point2f { -1.0f, -1.0f } };
    vertex_struct_texture_3D<float> p1{ Point3f { 0.9f, -0.9f, 0.f }, Point2f { 1.0f, -1.0f } };
    vertex_struct_texture_3D<float> p2{ Point3f { 0.9f, 0.9f, 0.f }, Point2f { 1.0f, 1.0f } };
    Trianglef triangle(p0, p1, p2);

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

        Mat4<float> v;
        v(0, 0) = 1.0f;
        v(1, 1) = 1.0f;
        v(2, 2) = 1.0f;
        v(3, 3) = 1.0f;

        Mat4<float> p;
        float aspect = 1.0f;
        float fov = 45.0f / 180.0f * 3.14159265358979323846f;
        float n = 0.1f;
        float f = 100.0f;
        p(0, 0) = 1.0f / (aspect * std::tan(fov / 2.0f));
        p(1, 1) = 1.0f / std::tan(fov / 2.0f);
        p(2, 2) = -(f + n) / (f - n);
        p(2, 3) = -(2.0f * f * n) / (f - n);
        p(3, 2) = -1.0f;

        Mat4<float> vp = p * v;

        // dessin...
        triangle.update();
        triangle.render(vp);
        glFlush();
        // termine la trame courante (en interne, échange les deux tampons de rendu)
        window.display();
    }

    // libération des ressources...

    return 0;
}