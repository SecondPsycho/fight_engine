/*  
    THE FIGHTING GAME ENGINE
    Joey, Cordell, Matthew, and Owen
*/
/**
 * @file fight_engine.cpp
 * @brief This is the file containing all our code for our game engine built for fighting games.
 * 
 * @author Cordell
 * @author Joey
 * @author Matthew
 * @author Owen
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
/**
 * @struct sprite_data
 *
 * @brief The struct which contains all of the data necessary for a single sprite.
 * 
 */
struct sprite_data{
    Texture imageSource;
    Sprite imageSprite;
    Sprite flippedSprite;
};

/**
 * @struct animation_data
 *
 * @brief The struct which contains all of the data and functions for an animation. 
 * 
 */
struct animation_data{
    vector<sprite_data*> animations;
    int cur_frame = 0;
    int frame_tick = 1;
    int max_frame_tick = 10;
    /**
    * @brief Enqueues the provided sprite data into the 'animations' vector.
    * 
    * @param frame The pointer to the sprite_data to add to the back of the animation.
    */
    void addAnimationData(sprite_data* frame){
        this->animations.push_back(frame);
    }
    /**
     * @brief Get the size of the animations vector.
     * 
     * @return Return the size of the animations vector.
     */
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
/**
 * @brief Creates a sprite struct from a sprite path.
 * 
 * @param sprite_path Pass in a string of the path to the sprite image.
 * @return sprite_data* A pointer to the sprite_data struct that is created from the sprite_path that was passed in is returned.
 */
sprite_data* make_sprite(string sprite_path){
    sprite_data* sprite = new sprite_data;
    if(!sprite->imageSource.loadFromFile(sprite_path)){
        cerr << "ERROR: Sprite creation failed" << endl;
        return sprite;
    }
    sprite->imageSprite.setTexture(sprite->imageSource);

    //Store the flipped sprite
    sprite->flippedSprite.setTexture(sprite->imageSource); 
    sprite->flippedSprite.setScale(-1,1);
    sprite->flippedSprite.setOrigin(sprite->imageSprite.getTexture()->getSize().x,0);
    return sprite;
}

// https://www.tutorialspoint.com/How-can-I-get-the-list-of-files-in-a-directory-using-C-Cplusplus
/* print out all entries in a directory
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
//*/


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

// SOUND AND MUSIC
class SFX { //use this for quick clips, not running soundtracks, and .wav works best for these
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

/**
 * @class Song
 * @brief The Song class that can play music.
 * 
 */
class Song { //use this for running soundtracks, not for quick clips, and .ogg works best for these
    public:
        Song(string musicPath) {
            if (!this->music.openFromFile(musicPath)) {
                cerr << "ERROR: Invalid file path" << endl;
            }
            this->music.setLoop(true);
        } 

        void play() {
            this->music.play();
        }

        void pause() {
            this->music.pause();
        }

        void setPlayingOffset(float sec) {
            Time t = sf::seconds(sec);
            this->music.setPlayingOffset(t);
        }

        void stop() {
            this->music.stop();
        }

        void setPitch(float pitch) {
            this->music.setPitch(pitch); // 1 = normal pitch, 1.2 = higher pitch, 0.8 = lower pitch, etc
        }

        void setVolume(float vol) {
            this->music.setVolume(vol); //0 = mute, 100 = full volume
        }

        void setLoop(bool isLooping) {
            this->music.setLoop(isLooping);
        }

    private:
        Music music;
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
        };
        void setSize(int pw, int ph) {
            this->w = pw;
            this->h = ph;
        }
        bool collides(Hitbox rect) {
            //cout << this->x << ' ' << this->y << ' ' << this->w << ' ' << this->h << "   " << rect.x << ' ' << rect.y << ' ' << rect.w << ' ' << rect.h << '\n';
            return (this->x <= (rect.x + rect.w) && (this->x + this->w) >= rect.x && this->y <= (rect.y + rect.h) && (this->y + this->h) >= rect.y);
        }
        // Directional Collision
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
        void initRectangle() {
            this->rectangle.setSize(Vector2f(this->w,this->h));
            this->rectangle.setOutlineColor(Color(255,255,0,255));
            this->rectangle.setOutlineThickness(1);
            this->rectangle.setFillColor(Color(0,0,0,0));
            this->rcset = true;
        }
        RectangleShape getRectangle() {
            this->rectangle.setPosition(this->x,this->y);
            return this->rectangle;
        }
    private:
    bool rcset;
        RectangleShape rectangle;
};


//An object that holds a Sprite and a Hitbox
class KinematicBody2D {
    public:
        /**
        * @brief Initialize a KinematicBody2D. Parameters optional.
        * 
        * @param px x-position (int)
        * @param py y-position (int)
        * @param pw width (int)
        * @param ph height (int)
        */
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
        
        /**
        * @brief Set the Position of the KinematicBody2D.
        * 
        * @param v A Vector2D holding the new coordinates.
        */
        void setPos(Vector2D v) {
            this->x = v.x;
            this->y = v.y;
            if (this->hbset) {(this->hitbox)->setPosition(this->x+this->hbx,this->y+this->hby); };
        }
        /**
        * @brief Move the KinematicBody2D relative to its current coordinates.
        * 
        * @param v A Vector2D holding the distances to move.
        */
        void move(Vector2D v) {
            this->x += v.x;
            this->y += v.y;
            if (this->hbset) {(this->hitbox)->setPosition(this->x+this->hbx,this->y+this->hby); };
        };
        /**
        * @brief Update the Physics. Run every frame.
        */
        void tick() {
            //This function is designed to be called every frame to make the physics work. -Cordell King
            this->v.inc(this->a);
            this->p.inc(this->v);
            this->move(this->p);
            this->p = Vector2D(0,0);
        };
        
        /**
        * @brief Change the Current Sprite. Useful for animations.
        * 
        * @param newsprite A pointer to the sprite_data object which holds the sprite.
        */
        void setSprite(sprite_data* newsprite) {
            //this->spset = true; Test This -Cordell King
            this->sprite = newsprite;
        };
        /**
        * @brief Get the sprite_data from the KinematicBody2D. Do NOT use for drawing. Use getSprite() instead.
        * @return A pointer to the sprite_data from the KinematicBody2D.
        */
        //I may remove this function. Are we using it? -Cordell King
        sprite_data* getSpriteData() {
            return (this->sprite);
        };
        /**
        * @brief Get the Sprite. Use for drawing.
        * @return The Sprite to be drawn.
        */
        Sprite getSprite() {
            if (this->hflip) {
                (this->sprite)->flippedSprite.setPosition(this->x+this->spx,this->y+this->spy);
                return (this->sprite)->flippedSprite;
            } else {
                (this->sprite)->imageSprite.setPosition(this->x+this->spx,this->y+this->spy);
                return (this->sprite)->imageSprite;
            }
        };

        /**
        * @brief Flip the KinematicBody2D horizontally for display purposes.
        */
        void flipH() {
            if (this->hflip) {
                this->hflip = false;
            } else {
                this->hflip = true;
            }
        }
        /**
        * @brief Set the horizontal flip direction for display purposes.
        * @param toFlip Whether we want it flipped from default or not.
        */
        /* Test this Later
        void flipH(bool toFlip) {
            this->hflip = toFlip;
        }
        //*/
        /**
        * @brief Determine if the KinematicBody2D is already flipped.
        */
        bool isFlippedH() {
            return this->hflip;
        }
        
        /**
        * @brief Initialize the Hitbox. Use if you intend to use a Hitbox for this KinematicBody2D.
        */
        void initHitbox() {
            this->hitbox = new Hitbox(this->x, this->y, this->w, this->h);
            this->hbset = true;
        };
        /**
        * @brief Set up the Hitbox to a pre-existing Hitbox. Can be used instead of initHitbox().
        * @param newhitbox A pointer to the new Hitbox.
        */
        void setHitbox(Hitbox* newhitbox) {
            this->hbset = true;
            this->hitbox = newhitbox;
            (this->hitbox)->setPosition(this->x+this->hbx,this->y+this->spx);
        };
        /**
        * @brief Adjust the location and size of the Hitbox. Doesn't work yet.
        * @param px The Hitbox's new x location, relative to the origin of the KinematicBody2D.
        * @param py The Hitbox's new y location, relative to the origin of the KinematicBody2D.
        * @param pw The Hitbox's new width.
        * @param ph The Hitbox's new height.
        */
        /*
        void adjustHitbox(int px, int py, int pw, int ph) {
            this->hitbox->setSize(pw,ph);
            this->hbx = px;
            this->hby = py;
        };
        //*/
        /**
        * @brief The Hitbox.
        * @return A pointer to the Hitbox.
        */
        Hitbox* getHitbox() {
            return this->hitbox;
        };
        
        /**
        * @brief See if this KinematicBody2D collides with another.
        * 
        * @param hostile A pointer to the other KinematicBody2D
        * @return A boolean indicating whether or not they collide.
        */
        bool collides(KinematicBody2D *hostile) {
            return this->hitbox->collides(*(hostile->getHitbox()));
        };
        /**
        * @brief See if this KinematicBody2D collides with another.
        * 
        * @param hostile A pointer to the other KinematicBody2D
        * @return A boolean indicating whether or not they collide.
        */
        int* collidesDir(KinematicBody2D *hostile) {
            return this->hitbox->collidesDir(*(hostile->getHitbox()));
        }
        /**
        * @brief See if this KinematicBody2D collides with another, and on which side.
        * 
        * @param hostile A pointer to the other KinematicBody2D
        * @return A pointer to an array of 3 integers. The first indicates collision on the left or right side. The second indicates collision on the top or bottom. The third indicates collision in general.
        */
        int* blocks(KinematicBody2D *hostile) {
            int* side = this->collidesDir(hostile);
            if (side[2] == 1) {
                if (side[1] == -1) {
                    hostile->setPos(Vector2D(hostile->x,this->y-hostile->h));
                    hostile->v.y = this->v.y;
                    hostile->p.x += this->v.x;
                } else if (side[0] == -1) {
                    hostile->setPos(Vector2D(this->x-hostile->w,hostile->y));
                    hostile->v.x = this->v.x;
                    hostile->p.y += this->v.y;
                } else if (side[0] == 1) {
                    hostile->setPos(Vector2D(this->x+this->w,hostile->y));
                    hostile->v.x = this->v.x;
                    hostile->p.y += this->v.y;
                } else if (side[1] == 1) {
                    hostile->setPos(Vector2D(hostile->x,this->y+this->h));
                    hostile->v.y = this->v.y;
                    hostile->p.x += this->v.x;
                }
            }
            return side;
        }
    
        /**
        * @brief Slow down the current velocity by a set amount.
        * 
        * @param f a Vector2D indicating how much to slow.
        */
        void dampen(Vector2D f) {
            if (this->v.x > 0) {
                this->v.x -= f.x;
            } else if (this->v.x < 0) {
                this->v.x += f.x;
            }
            if (this->v.y > 0) {
                this->v.y -= f.y;
            } else if (this->v.y < 0) {
                this->v.y += f.y;
            }
        }

        /**
        * @brief Set up the Rectangle. Use if you intend to draw a Rectangle over the KinematicBody2D.
        */
        void initRectangle() {
            this->rectangle.setSize(Vector2f(this->w,this->h));
            this->rectangle.setPosition(this->x,this->y);
            this->rectangle.setFillColor(Color(0,0,0,255));
            this->rcset = true;
        };
        
        /**
        * @brief Get the Rectangle for drawing purposes. Make sure you've called initRectangle() first.
        * 
        * @return The Rectangle of the KinematicBody2D.
        */
        RectangleShape getRectangle(){
            (this->rectangle).setPosition(this->x,this->y); //Test this -Cordell King
            return this->rectangle;
        };

        /**
        * @brief Get the current x-position of the KinematicBody2D.
        * 
        * @return An integer with the current x-position.
        */
        int posX() { 
            return this->x;
        };
        /**
        * @brief Get the current y-position of the KinematicBody2D.
        * 
        * @return An integer with the current y-position.
        */
        int posY() {
            return this->y;
        };
        /**
        * @brief Get the current position of the KinematicBody2D.
        * 
        * @return A Vector2D with the current x and y-positions.
        */
        Vector2D pos() {
            return Vector2D(this->x,this->y);
        };

        Vector2D p; //Save movement to apply next frame
        Vector2D v; //Save a velocity
        Vector2D a; //Save an acceleration
        
    private:
        int x, y, w, h, hbx, hby, spx, spy;
        bool hbset, spset, rcset;
        bool hflip = false;
        Hitbox* hitbox;
        sprite_data* sprite;
        RectangleShape rectangle;
};
