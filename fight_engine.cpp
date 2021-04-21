/*  
    THE FIGHTING GAME ENGINE
    Joey, Cordell, Matthew, and Owen
*/

#include <iostream>
#include <string>
#include <list>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

//https://www.sfml-dev.org/documentation/2.5.1/

#define create_window(name, h, w) sf::RenderWindow window(sf::VideoMode(h, w), name);
using namespace sf;
using namespace std;


//STRUCTS:

struct sprite_data{
    Texture imageSource;
    Sprite imageSprite;
};

struct animation_data{
    list<sprite_data*> animations;
    void createAnimationData(){
        //do nothing?
    }
    void addAnimationData(sprite_data* frame){
        this->animations.push_back(frame);
    }
    int size(){
        return this->animations.size();
    }
    sprite_data* getFrame(int index){
        // if(index < this->size()){
        // }
        return this->animations.front();
    }
};



//FUNCTIONS
//create a sprite
sprite_data* make_sprite(string sprite_path){
    sprite_data* sprite = new sprite_data;
    if(!sprite->imageSource.loadFromFile(sprite_path)){
        cerr << "ERROR: Sprite creation failed" << endl;
        return sprite;
    }
    sprite->imageSprite.setTexture(sprite->imageSource);
    return sprite;
}

// https://www.tutorialspoint.com/How-can-I-get-the-list-of-files-in-a-directory-using-C-Cplusplus
//print out all entries in a directory
void list_dir(const char *path) {
    struct dirent *entry;
    DIR *dir = opendir(path);

    
    if (dir == NULL) {
        return;                                                                         //WIP
    }
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0){
            cout << entry->d_name << endl;
        }
    }
    closedir(dir);
}


//CLASSES

//Framerate
class Framerate {
    private:
        clock_t last_time;
        clock_t current_time;
        int fps;
    public:
        Framerate(int pfps) {
            this->fps = pfps;
            this->last_time = clock()*this->fps;
            this->current_time = clock()*this->fps;
        };
        void setFPS(int pfps) {
            this->fps = pfps;
        };
        void next_frame() {
            while ((this->current_time - this->last_time) <= CLOCKS_PER_SEC) {
                this->current_time = clock()*this->fps;
            }
            this->last_time = this->last_time + CLOCKS_PER_SEC;
        }
};

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

class TextBox { //"Man I hope no one expects this to work" -Owen
    public:
        TextBox(Vector2D newPosition, string newFontPath, string newText="") {
            this->position = newPosition;
            this->text = newText;

            if (!font.loadFromFile(newFontPath)) {
                cerr << "Error: Invalid font path" << endl;
            }

            self.setFont(font);
            self.setString(text);
        }

        void setText (string newText) {
            this->text = newText;
            self.setString(text);
        }
        
        void setPosition (Vector2D newPosition) {
            this->position = newPosition;
        }

        /*// I've tried to pass the 'window' as a parameter before. It's never worked. -Cordell King
        void draw(Window theWindow) {
            theWindow.draw(self);
        }
        //*/

    private:
        Vector2D position;
        string text;
        Font font;
        Text self;
};

//PHYSICS
class Hitbox {
    public:
        int x,y,h,w;
        Hitbox(int px, int py, int ph, int pw) {
            this->x = px; this->y = py; this->h = ph; this->w = pw;
        };
        Hitbox() {
            this->x = 0; this->y = 0; this->h = 0; this->w = 0;
        };
        void setPosition(int px, int py) {
            this->x = px;
            this->y = py;
        }
        bool collides(Hitbox rect) {
            cout << this->x << ' ' << this->y << ' ' << this->w << ' ' << this->h << "   " << rect.x << ' ' << rect.y << ' ' << rect.w << ' ' << rect.h << '\n';
            return (this->x < (rect.x + rect.w) && (this->x + this->w) > rect.x && this->y < (rect.y + rect.h) && (this->y + this->h) > rect.y);
        }
};


//An object that holds a Sprite and a Hitbox
class KinematicBody2D {
    public:
        KinematicBody2D(int px, int py, int pw, int ph) {
            this->x = px; this->y = py; this->h = ph; this->w = pw;
            this->hbx = 0; this->hby = 0; //Allow the Hitbox's position to be different from the KB2D -Cordell King
            this->spx = 0; this->spy = 0; //Allow the Sprite's position to be different from the KB2D -Cordell King
            this->hbset = false; this->spset = false;
        };
        KinematicBody2D() {
            this->x = 0; this->y = 0; this->h = 0; this->w = 0;
            this->hbx = 0; this->hby = 0;
            this->spx = 0; this->spy = 0;
            this->hbset = false; this->spset = false;
        };

        void move(Vector2D v) {
            this->x += v.x;
            this->y += v.y;
            if (this->spset) { (this->sprite)->imageSprite.setPosition(this->x+this->spx,this->y+this->spy); };
            if (this->hbset) {(this->hitbox)->setPosition(this->x+this->hbx,this->y+this->hby); };
        };
        void tick() {
            //This function is designed to be called every frame to make the physics work. -Cordell King
            this->v.inc(this->a);
            this->move(this->v);
        };

        /*
        void setAcceleration(Vector2D pa) {
            this->a = pa;
        };
        void incAcceleration(Vector2D pa) {
            this->a.inc(pa);
        };

        void setSpeed(Vector2D pv) {
            this->v = pv;
        };
        void setSpeedY(int pvy) {
            this->v.y = pvy;
        }
        void setSpeedX(int pvx) {
            this->v.x = pvx;
        }
        void incSpeed(Vector2D pv) {
            this->v.inc(pv);
        };
        //*/

        void setSprite(sprite_data* newsprite) {
            this->spset = true;
            this->sprite = newsprite;
            (this->sprite)->imageSprite.setPosition(this->x+this->spx,this->y+this->spy);
        };
        sprite_data* getSpriteData() {
            return (this->sprite);
        };
        Sprite getSprite() {
            return (this->sprite)->imageSprite;
        };
        
        void initHitbox() {
            this->hitbox = new Hitbox(this->x, this->y, this->w, this->h);
            this->hbset = true;
        };
        void setHitbox(Hitbox* newhitbox) {
            this->hbset = true;
            this->hitbox = newhitbox;
            (this->hitbox)->setPosition(this->x+this->hbx,this->y+this->spx);
        };
        Hitbox* getHitbox() {
            return this->hitbox;
        };

        bool collides(KinematicBody2D *hostile) {
            return this->hitbox->collides(*(hostile->getHitbox()));
        };

        void initRectangle() {
            cout << this->x << ' ' << this->y << ' ' << this->w << ' ' << this->h << '\n';
            this->rectangle.setSize(Vector2f(this->w,this->h));
            this->rectangle.setPosition(this->x,this->y);
            this->rectangle.setFillColor(Color(0,0,0,255));
            this->rcset = true;
        };

        RectangleShape getRectangle(){
            return this->rectangle;
        };

        int pos() { 
            //This function is a Diagnostic
            return this->y;
        };

        Vector2D v; //Save a velocity
        Vector2D a; //Save an acceleration
    private:
        int x, y, w, h, hbx, hby, spx, spy;
        bool hbset, spset, rcset;
        Hitbox* hitbox;
        sprite_data* sprite;
        RectangleShape rectangle;
};

//LINKED LIST IMPLEMENTATION


// std::list<sprite_data> l;
// l.push_front();
