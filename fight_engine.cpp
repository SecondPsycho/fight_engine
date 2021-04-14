/*  
    THE FIGHTING GAME ENGINE
    Joey, Cordell, Matthew, and Owen
*/

#include <string>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

//https://www.sfml-dev.org/documentation/2.5.1/


using namespace std;


sf::RenderWindow create_window (string name, int h, int w) {
    //We may have to Malloc this.
    sf::RenderWindow window(sf::VideoMode(h, w), name);
    window.setKeyRepeatEnabled(false);
    return window;
}


//SPRITE HANDLING:
