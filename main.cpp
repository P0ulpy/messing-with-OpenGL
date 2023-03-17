#include <SFML/Window.hpp>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

template<typename T>
struct Point2d
{
    explicit Point2d(const T x = 0, const T y = 0): x(x), y(y) {}
    Point2d(const Point2d& pt): x(pt.x), y(pt.y) {}

    T x;
    T y;
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