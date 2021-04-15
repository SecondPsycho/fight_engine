/*  
    THE FIGHTING GAME ENGINE
    Joey, Cordell, Matthew, and Owen
*/

#include <string>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

//https://www.sfml-dev.org/documentation/2.5.1/

struct sprite_data {
    sf::Texture imageSource;
    sf::Sprite imageSprite;
};

class Vector2D {
    public:
        int x,y;
        Vector2D(int px, int py) {
            this->x = px; this->y = py;
        }
};

class Rect {
    public:
        int x,y,h,w;
        Rect(int px, int py, int ph, int pw) {
            this->x = px; this->y = py; this->h = ph; this->w = pw;
        };
        Rect() {
            this->x = 0; this->y = 0; this->h = 0; this->w = 0;
        };
        bool collides(Rect rect) {
            return (this->x < (rect.x + rect.w) && (this->x + this->w) > rect.x && this->y < (rect.y + rect.h) && (this->y + this->h) > rect.y);
        }
};

class KinematicBody2D {
    public:
        KinematicBody2D(int px, int py, int ph, int pw) {
            this->x = px; this->y = py; this->h = ph; this->w = pw;
            this->hbx = 0; this->hby = 0;
        };
        void move(Vector2D v) {
            this->x += v.x;
            this->y += v.y;
            this->hitbox.x = this->x + this->hbx;
            this->hitbox.y = this->y + this->hby;
        }
        int pos() {
            return this->hitbox.y;
        }
    private:
        int x, y, h, w, hbx, hby;
        Rect hitbox;
        sprite_data sprite;
};

int main() {
    KinematicBody2D player1(0,0,4,2);
    Vector2D g(0,1);
    cout << player1.pos() << '\n';
    player1.move(g);
    cout << player1.pos() << '\n';
    return 0;
}