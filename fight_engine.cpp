/*  
    THE FIGHTING GAME ENGINE
    Joey, Cordell, Matthew, and Owen
*/

#include <iostream>
#include <string>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

//https://www.sfml-dev.org/documentation/2.5.1/

#define create_window(name, h, w) sf::RenderWindow window(sf::VideoMode(h, w), name);
//#define RenderWindow sf::RenderWindow


struct sprite_data{
    sf::Texture imageSource;
    sf::Sprite imageSprite;
};


using namespace std;



//SPRITE HANDLING:

sprite_data draw_sprite(string sprite_path){
    sprite_data sprite;
    //sf::Texture imageSource;
    //sf::Sprite imageSprite;
    if(!sprite.imageSource.loadFromFile(sprite_path)){
        cout << "hi I'm error" << endl;
        return sprite;
    }
    sprite.imageSprite.setTexture(sprite.imageSource);
    return sprite;

    /*while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            //do nothing?
        }

        window.draw(imageSprite);
        window.display();
    }*/

}