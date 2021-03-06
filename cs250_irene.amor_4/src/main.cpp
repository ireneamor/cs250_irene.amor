/****************************************************************************************/
/*!
\file   main.cpp
\author Irene Amor Mendez
\par    email: irene.amor@digipen.edu
\par    DigiPen login: irene.amor
\par    Course: CS250
\par    Assignment #2
\date   08/02/2022
\brief

This file contains the implementation of the following functions for the
Tank assignment.
Functions include:	main

Hours spent on this assignment: ~20

*/
/****************************************************************************************/

#include "AirplaneFunctions.h"

int main()
{
    //Create a airplane
    Airplane airplane;
    airplane.Airplane_Initialize();

    sf::RenderWindow window(sf::VideoMode(airplane.WIDTH, airplane.HEIGHT), "SFML works!");

    FrameBuffer::Init(airplane.WIDTH, airplane.HEIGHT);

    // Generate image and texture to display
    sf::Image   image;
    sf::Texture texture;
    sf::Sprite  sprite;
    texture.create(airplane.WIDTH, airplane.HEIGHT);
    image.create(airplane.WIDTH, airplane.HEIGHT, sf::Color::Black);


    while (window.isOpen())
    {
        FrameBuffer::Clear(sf::Color::White.r, sf::Color::White.g, sf::Color::White.b);

        // Handle input
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window.close();

        // Calculate airplane position
        airplane.Airplane_Update();

        // Show image on screen
        FrameBuffer::ConvertFrameBufferToSFMLImage(image);

        texture.update(image);
        sprite.setTexture(texture);
        window.draw(sprite);
        window.display();
    }
    
    FrameBuffer::Free();

    return 2;
}
