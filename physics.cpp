/*  
    THE FIGHTING GAME ENGINE
    Joey, Cordell, Matthew, and Owen
*/

//#include <string>
//#include <SFML/Audio.hpp>
//#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

//https://www.sfml-dev.org/documentation/2.5.1/


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
        KinematicBody2D() {
        }
    private:
        int x, y, hx, hy;
        Rect hitbox;
};

int main() {
    /*
    Rect rect0(2,2,4,4);
    Rect rect1(1,5,2,2);
    if (rect0.collides(rect1)) {
        cout << "Collision.\n";
    } else {
        cout << "Miss.\n";
    }
    //*/
    return 0;
}