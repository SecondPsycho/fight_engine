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
using namespace sf;
using namespace std;


//SPRITE HANDLING:
struct sprite_data{
    Texture imageSource;
    Sprite imageSprite;
};

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

//PHYSICS
class Vector2D {
    public:
        int x,y;
        Vector2D() {
            this->x = 0; this->y = 0;
        }
        Vector2D(int px, int py) {
            this->x = px; this->y = py;
        }
};

//An object that holds a Sprite and a Hitbox
class KinematicBody2D {
    public:
        KinematicBody2D(int px, int py, int ph, int pw) {
            this->x = px; this->y = py; this->h = ph; this->w = pw;
            this->hbx = 0; this->hby = 0; //Allow the Hitbox's position to be different from the KB2D
            this->spx = 0; this->spy = 0; //Allow the Sprite's position to be different from the KB2D
        };
        KinematicBody2D() {
            this->x = 0; this->y = 0; this->h = 0; this->w = 0;
            this->hbx = 0; this->hby = 0;
            this->spx = 0; this->spy = 0;
        }
        void setSprite(sprite_data* newsprite) {
            this->sprite = newsprite;
            (this->sprite)->imageSprite.setOrigin(this->x-this->spx,this->y-this->spy);
        }
        sprite_data getSpriteData() {
            return *(this->sprite);
        }
        Sprite getSprite() {
            return (this->sprite)->imageSprite;
        }
        
        void move(Vector2D v) {
            this->x += v.x;
            this->y += v.y;
            (this->sprite)->imageSprite.setOrigin(this->x-this->spx,this->y-this->spy);
            //(this->hitbox)->x = this->x + this->hbx;
            //(this->hitbox)->y = this->y + this->hby;
        }

    private:
        int x, y, h, w, hbx, hby, spx, spy;
        Vector2D v; //Save a velocity
        Vector2D a; //Save an acceleration
        //Rect* hitbox;
        sprite_data* sprite;
};