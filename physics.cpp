/*  
    THE FIGHTING GAME ENGINE
    Joey, Cordell, Matthew, and Owen
*/

#include <iostream>
#include <string>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <ctime>

//https://www.sfml-dev.org/documentation/2.5.1/

#define create_window(name, h, w) sf::RenderWindow window(sf::VideoMode(h, w), name);
using namespace sf;
using namespace std;


//SPRITE HANDLING:
struct sprite_data{
    Texture imageSource;
    Sprite imageSprite;
};

struct animation_data{
    sprite_data *animation_frames;
    int MAX_SIZE = 0;
    int counter = 0;
    public:
        void createAnimationData(int size){
            MAX_SIZE = size;
            animation_frames = new sprite_data[size];
        }
        void addAnimationData(sprite_data sprite){
            if(counter>=MAX_SIZE){
                cerr << "Too many frames loaded" << endl;
                return;
            }
            animation_frames[counter] = sprite;
        }
};



//FUNCTIONS
//create a sprite
sprite_data make_sprite(string sprite_path){
    sprite_data sprite;
    if(!sprite.imageSource.loadFromFile(sprite_path)){
        cerr << "sprite creation error error" << endl;
        return sprite;
    }
    sprite.imageSprite.setTexture(sprite.imageSource);
    return sprite;
}

// https://www.tutorialspoint.com/How-can-I-get-the-list-of-files-in-a-directory-using-C-Cplusplus
//print out all entries in a directory
/*
void list_dir(const char *path) {
    struct dirent *entry;
    DIR *dir = opendir(path);

    if (dir == NULL) {
        return;                                                                         //WIP
    }
    while ((entry = readdir(dir)) != NULL) {
        cout << entry->d_name << endl;
    }
    closedir(dir);
}
//*/

//Game Time
class Framerate {
    public:
        time_t start_up;
        time_t last_time;
        time_t current_time;
        int fps;
        Framerate(int pfps) {
            this->fps = pfps;
            this->start_up = time(NULL);
            this->last_time = start_up*this->fps;
            this->current_time = time(NULL)*this->fps;
        };
        void setFPS(int pfps) {
            this->fps = pfps;
        };
        void next_frame() {
            while ((this->current_time - this->last_time) < 1) {
                this->current_time = time(NULL)*this->fps;
            }
            this->last_time = this->last_time + 1;
        }
};

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
    Framerate ticker(30);
    int t = 0;
    int seconds = 0;
    while (1 == 1) {
        ticker.next_frame();
        t += 1;
        if (t >= 30) {
            seconds += 1;
            cout << seconds << '\n';
            t -= 30;
        }
    }
    return 0;
}