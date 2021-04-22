/*  
    THE FIGHTING GAME ENGINE
    Joey, Cordell, Matthew, and Owen
*/

#include <iostream>
#include <string>
#include <vector>
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
    vector<sprite_data*> animations;
    int cur_frame = 0;
    int frame_tick = 1;
    int max_frame_tick = 10;
    void createAnimationData(){
        /* This does nothing */
    }
    void addAnimationData(sprite_data* frame){
        this->animations.push_back(frame);
    }
    int getSize(){
        return (int) this->animations.size();
    }
    sprite_data* getCurrentFrame() {
        return this->animations.at(this->cur_frame);
    }
    sprite_data* getNextFrame() {
        this->cur_frame += 1;
        if (this->cur_frame >= this->getSize()) {
            this->cur_frame = 0;
        }
        return this->animations.at(cur_frame);
    }
    sprite_data* getFrame(int index){
        if(index < this->getSize()){
            return this->animations.at(index);
        }
        return this->animations.front();
    }
    sprite_data* frameTick() {
        if (this->frame_tick > 1) { 
            this->frame_tick--; 
            return this->getCurrentFrame();
        };
        this->frame_tick = this->max_frame_tick;
        return this->getNextFrame();
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
            this->last_time = (clock()*this->fps)-CLOCKS_PER_SEC;
            this->current_time = clock()*this->fps;
        };
        void next_frame() {
            while ((this->current_time - this->last_time) <= CLOCKS_PER_SEC) {
                this->current_time = clock()*this->fps;
            }
            this->last_time = this->last_time + CLOCKS_PER_SEC;
        }
};

//Sound
class SFX {                                                     // The biggest mistake you can make is assuming any of this is gonna actually work -owen
    public:
        SFX(string soundEffectPath) {
            if (!this->buffer.loadFromFile(soundEffectPath)) {
                cerr << "ERROR: Invalid file path" << endl;
            }
            this->soundEffect.setBuffer(this->buffer);
        } 

        void play() {
            this->soundEffect.play();
        }

        void pause() {
            this->soundEffect.pause();
        }

        void setPlayingOffset(float sec) {
            Time t = sf::seconds(sec);
            this->soundEffect.setPlayingOffset(t);
        }

        void stop() {
            this->soundEffect.stop();
        }

        void setPitch(float pitch) {
            this->soundEffect.setPitch(pitch); // 1 = normal pitch, 1.2 = higher pitch, 0.8 = lower pitch, etc
        }

        void setVolume(float vol) {
            this->soundEffect.setVolume(vol); //0 = mute, 100 = full volume
        }

        void setLoop(bool isLooping) {
            this->soundEffect.setLoop(isLooping);
        }

    private:
        SoundBuffer buffer;
        Sound soundEffect;
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
        Hitbox(int px, int py, int pw, int ph) {
            this->x = px; this->y = py; this->w = pw; this->h = ph;
        };
        Hitbox() {
            this->x = 0; this->y = 0; this->w = 0; this->h = 0;
        };
        void setPosition(int px, int py) {
            this->x = px;
            this->y = py;
        }
        bool collides(Hitbox rect) {
            //cout << this->x << ' ' << this->y << ' ' << this->w << ' ' << this->h << "   " << rect.x << ' ' << rect.y << ' ' << rect.w << ' ' << rect.h << '\n';
            return (this->x < (rect.x + rect.w) && (this->x + this->w) > rect.x && this->y < (rect.y + rect.h) && (this->y + this->h) > rect.y);
        }
        //* Directional Collision
        int* collidesDir(Hitbox rect) {
            int* side = new int[3];
            side[0] = 0;
            side[1] = 0;
            side[2] = 0;
            if (this->collides(rect)) {
                side[2] = 1;
                if (this->y < rect.y){
                    side[1] += 1;
                }
                if ((this->y + this->h) > (rect.y + rect.h)) {
                    side[1] -= 1;
                }
                if (this->x < rect.x) {
                    side[0] += 1;
                }
                if ((this->x + this->w) > (rect.x + rect.w)) {
                    side[0] -= 1;
                }
            }
            return side;
        }
        //*/
};


//An object that holds a Sprite and a Hitbox
class KinematicBody2D {
    public:
        KinematicBody2D(int px, int py, int pw, int ph) {
            this->x = px; this->y = py; this->w = pw; this->h = ph;
            this->hbx = 0; this->hby = 0; //Allow the Hitbox's position to be different from the KB2D -Cordell King
            this->spx = 0; this->spy = 0; //Allow the Sprite's position to be different from the KB2D -Cordell King
            this->hbset = false; this->spset = false;
        };
        KinematicBody2D() {
            this->x = 0; this->y = 0; this->w = 0; this->h = 0;
            this->hbx = 0; this->hby = 0;
            this->spx = 0; this->spy = 0;
            this->hbset = false; this->spset = false;
        };

        void setPos(Vector2D v) {
            this->x = v.x;
            this->y = v.y;
            if (this->spset) {(this->sprite)->imageSprite.setPosition(this->x+this->spx,this->y+this->spy); };
            if (this->hbset) {(this->hitbox)->setPosition(this->x+this->hbx,this->y+this->hby); };
            if (this->rcset) {(this->rectangle).setPosition(this->x,this->y); };
        }
        void move(Vector2D v) {
            this->x += v.x;
            this->y += v.y;
            if (this->spset) {(this->sprite)->imageSprite.setPosition(this->x+this->spx,this->y+this->spy); };
            if (this->hbset) {(this->hitbox)->setPosition(this->x+this->hbx,this->y+this->hby); };
            if (this->rcset) {(this->rectangle).setPosition(this->x,this->y); };
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

        void flipH() {
            //Flips the sprite, but the origin is now at the top-right corner. This throws off collision.
            this->sprite->imageSprite.setScale(-1,1);
        }
        
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
        int* collidesDir(KinematicBody2D *hostile) {
            return this->hitbox->collidesDir(*(hostile->getHitbox()));
        }
        bool blocks(KinematicBody2D *hostile) {
            int* side = this->collidesDir(hostile);
            if (side[2] == 1) {
                if (side[1] == -1) {
                    hostile->setPos(Vector2D(hostile->x,this->y-hostile->h));
                    hostile->v.y = 0;
                } else if (side[0] == -1) {
                    hostile->setPos(Vector2D(this->x-hostile->w,hostile->y));
                    hostile->v.x = 0;
                } else if (side[0] == 1) {
                    hostile->setPos(Vector2D(this->x+this->w,hostile->y));
                    hostile->v.x = 0;
                } else if (side[1] == 1) {
                    hostile->setPos(Vector2D(hostile->x,this->y+this->h));
                    hostile->v.y = 0;
                }
                return true;
            }
            return false;
        }

        void initRectangle() {
            //cout << this->x << ' ' << this->y << ' ' << this->w << ' ' << this->h << '\n';
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
        //bool hflip;
        Hitbox* hitbox;
        sprite_data* sprite;
        RectangleShape rectangle;
};

//LINKED LIST IMPLEMENTATION


// std::list<sprite_data> l;
// l.push_front();
