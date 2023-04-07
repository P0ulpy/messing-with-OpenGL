#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics/Image.hpp>

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <array>
#include "Shader.hpp"
#include "Matrix.hpp"
#include "Texture.hpp"
#include "Triangle.hpp"
#include "Cube.hpp"


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
    using Cubef = Cube<float>;

    /*vertex_struct_color<float> p0{ Point2f { -0.9f, -0.9f }, { 1.0f, 0.0f, 0.0f, 1.0f } };
    vertex_struct_color<float> p1{ Point2f { 0.9f, -0.9f }, { 0.0f, 1.0f, 0.0f, 1.0f } };
    vertex_struct_color<float> p2{ Point2f { 0.9f, 0.9f }, { 0.0f, 0.0f, 1.0f, 1.0f } };*/
    /*vertex_struct_texture<float> p0{ Point2f { -0.9f, -0.9f }, Point2f { -1.0f, -1.0f } };
    vertex_struct_texture<float> p1{ Point2f { 0.9f, -0.9f }, Point2f { 1.0f, -1.0f } };
    vertex_struct_texture<float> p2{ Point2f { 0.9f, 0.9f }, Point2f { 1.0f, 1.0f } };*/
    vertex_struct_texture_3D<float> p0{ Point3f { -0.9f, -0.9f, 0.f }, Point2f { -1.0f, 1.0f } };
    vertex_struct_texture_3D<float> p1{ Point3f { 0.9f, -0.9f, 0.f }, Point2f { 1.0f, 1.0f } };
    vertex_struct_texture_3D<float> p2{ Point3f { 0.9f, 0.9f, 0.f }, Point2f { 1.0f, -1.0f } };
    Trianglef triangle(p0, p1, p2);

    Cubef cube {};

    sf::Clock dtClock;

    // la boucle principale
    bool running = true;
    while (running)
    {
        float dt = dtClock.restart().asSeconds();

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

        float aspect = 800.f / 600.f;
        float fov = 45.0f / 180.0f * 3.14159265358979323846f;
        float n = 0.1f;
        float f = 100.0f;
        // Matrice de projection
        auto p = Mat4<float>::projection(aspect, fov, f, n);

        Mat4<float> vp = p * v;

        // dessin...
        //triangle.update(dt);
        //triangle.render(vp);

        cube.update(dt);
        cube.render(vp);
        glFlush();
        // termine la trame courante (en interne, échange les deux tampons de rendu)
        window.display();
    }

    // libération des ressources...

    return 0;
}