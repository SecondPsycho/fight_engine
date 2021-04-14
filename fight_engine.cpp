/*  
    THE FIGHTING GAME ENGINE
    Joey, Cordell, Matthew, and Owen
*/

#include <string>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

//https://www.sfml-dev.org/documentation/2.5.1/

#define create_window(name, h, w) sf::RenderWindow window(sf::VideoMode(800, 800), "Awesome Game");
//#define RenderWindow sf::RenderWindow

/*
void create_window (string name, int h, int w) {
    //We may have to Malloc this.
    sf::RenderWindow window(sf::VideoMode(h, w), name);
}//*/

using namespace std;



//SPRITE HANDLING:
