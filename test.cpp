//#include "fight_engine.cpp"
#include <string>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#define create_window(name, h, w) sf::RenderWindow(sf::VideoMode(h, w), name)
#define RenderWindow sf::RenderWindow
/*
sf::RenderWindow create_window (string name, int h, int w) {
    //We may have to Malloc this.
    sf::RenderWindow window(sf::VideoMode(h, w), name);
    window.setKeyRepeatEnabled(false);
    return window;
}
//*/

int main(int argc, char* argv[]){
    //sf::RenderWindow window = create_window("Awesome Game", 800, 800);
    RenderWindow window(sf::VideoMode(800, 800), "Awesome Game");
    //sf::RenderWindow willow = window;
    //create_window("Awesome Game", 800, 800);
    while (window.isOpen()) {
    
    }
    return 0;
}
