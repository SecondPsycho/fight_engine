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

/*
struct animation_data{
    string animation_frames[];
    public:
        createAnimationData(int size){
            animation_frames = new Array(size);
        }
    
};
//*/

sprite_data make_sprite(string sprite_path){
    sprite_data sprite;
    if(!sprite.imageSource.loadFromFile(sprite_path)){
        cerr << "sprite creation error error" << endl;
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
        void inc(Vector2D vector) {
            this->x += vector.x;
            this->y += vector.y;
        }
};

class Hitbox {
    public:
        int x,y,h,w;
        Hitbox(int px, int py, int ph, int pw) {
            this->x = px; this->y = py; this->h = ph; this->w = pw;
        };
        Hitbox() {
            this->x = 0; this->y = 0; this->h = 0; this->w = 0;
        };
        void setOrigin(int px, int py) {
            this->x = px;
            this->y = py;
        }
        bool collides(Hitbox rect) {
            return (this->x < (rect.x + rect.w) && (this->x + this->w) > rect.x && this->y < (rect.y + rect.h) && (this->y + this->h) > rect.y);
        }
};

//An object that holds a Sprite and a Hitbox
class KinematicBody2D {
    public:
        KinematicBody2D(int px, int py, int ph, int pw) {
            this->x = px; this->y = py; this->h = ph; this->w = pw;
            this->hbx = 0; this->hby = 0; //Allow the Hitbox's position to be different from the KB2D
            this->spx = 0; this->spy = 0; //Allow the Sprite's position to be different from the KB2D
            this->hbset = false; this->spset = false;
        };
        KinematicBody2D() {
            this->x = 0; this->y = 0; this->h = 0; this->w = 0;
            this->hbx = 0; this->hby = 0;
            this->spx = 0; this->spy = 0;
            this->hbset = false; this->spset = false;
        };
        void setSprite(sprite_data* newsprite) {
            this->spset = true;
            this->sprite = newsprite;
            (this->sprite)->imageSprite.setOrigin(this->x+this->spx,this->y+this->spy);
        };
        sprite_data getSpriteData() {
            return *(this->sprite);
        };
        Sprite getSprite() {
            return (this->sprite)->imageSprite;
        };

        void setHitbox(Hitbox* newhitbox) {
            this->hbset = true;
            this->hitbox = newhitbox;
            (this->hitbox)->setOrigin(this->x+this->hbx,this->y+this->spx);
        };
        
        void move(Vector2D v) {
            this->x += v.x;
            this->y += v.y;
            if (this->spset) { (this->sprite)->imageSprite.setOrigin(this->x+this->spx,this->y+this->spy); };
            if (this->hbset) {(this->hitbox)->setOrigin(this->x+this->hbx,this->y+this->spx); };
        };
        void tick() {
            this->v.inc(this->a);
            this->move(this->v);
        };

        void setAcceleration(Vector2D pa) {
            this->a = pa;
        };
        void incAcceleration(Vector2D pa) {
            this->a.inc(pa);
        };

        void setSpeed(Vector2D pv) {
            this->v = pv;
        };
        void incSpeed(Vector2D pv) {
            this->v.inc(pv);
        };

        int pos() {
            return this->y;
        };

    private:
        int x, y, h, w, hbx, hby, spx, spy;
        Vector2D v; //Save a velocity
        Vector2D a; //Save an acceleration
        bool hbset; bool spset;
        Hitbox* hitbox;
        sprite_data* sprite;
};

int main() {
    KinematicBody2D player1(0,0,4,2);
    player1.setAcceleration(Vector2D(0,-1));
    player1.tick();
    cout << player1.pos() << '\n';
    player1.tick();
    cout << player1.pos() << '\n';
    player1.tick();
    cout << player1.pos() << '\n';
    return 0;
}