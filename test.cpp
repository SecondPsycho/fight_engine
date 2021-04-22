#include "fight_engine.cpp"


class NewGame {
  public:
    NewGame(KinematicBody2D player, int n) {
      this->statics_count = 0;
      this->statics_max = n;
      this->statics = new KinematicBody2D[this->statics_max];
      this->player = player;
    }
    void addStatic(KinematicBody2D newstatic) {
      if (this->statics_count < this->statics_max) {
        this->statics[this->statics_count] = newstatic;
        this->statics_count++;
      } else {
        cout << "Cannot add another static.\n";
      }
    }
    KinematicBody2D* getStatic(int n) {
      if (n < this->statics_max && n >= 0) {
        return &(this->statics[n]);
      }
      cout << "Invalid Static Index.\n";
      return nullptr;
    }
    int getStaticsCount() {
      return this->statics_count;
    }
  private:
    KinematicBody2D player;
    KinematicBody2D* statics;
    int statics_count;
    int statics_max;
};




int main(int argc, char* argv[]){
    create_window("Awesome Game", 800, 800);
    window.setKeyRepeatEnabled(false);
    
    //Create an Animation
    animation_data idle;
    animation_data walk;
    animation_data leap;

    //Create Sprite Data
    idle.addAnimationData(make_sprite("./images/monster_idle.png"));
    walk.addAnimationData(make_sprite("./images/walk/monster_walk1.png"));
    walk.addAnimationData(make_sprite("./images/walk/monster_walk2.png"));
    leap.addAnimationData(make_sprite("./images/monster_leap.png"));

    KinematicBody2D wolf(100,300,64,64);
    wolf.setSprite(idle.getCurrentFrame());
    wolf.initHitbox();
    wolf.a.y = 1;

    NewGame Game(wolf, 6);
    Game.addStatic(KinematicBody2D(0,600,800,200));
    Game.getStatic(0)->initRectangle();
    Game.getStatic(0)->initHitbox();

    Game.addStatic(KinematicBody2D(0,400,200,200));
    Game.getStatic(1)->initRectangle();
    Game.getStatic(1)->initHitbox();

    Game.addStatic(KinematicBody2D(0,200,200,50));
    Game.getStatic(2)->initRectangle();
    Game.getStatic(2)->initHitbox();
    Game.getStatic(2)->v.x = 5;

    Game.addStatic(KinematicBody2D(600,200,200,50));
    Game.getStatic(3)->initRectangle();
    Game.getStatic(3)->initHitbox();
    Game.getStatic(3)->v.y = 5;
    
    //list_dir("./images");
    //cout << "walk.getSize() = " << walk.getSize() << endl;

    //* Test to the Music
    SFX my_music("The_Last_Encounter_Original_Version (online-audio-converter.com).wav");
    my_music.play();
    //*/
        
    Event event;
    int keys[5] = {0,0,0,0,0};
    int *collide;
    bool wolf_on_ground = false;

    // Framerate Control -Cordell King
    Framerate ticker(30);
    while (window.isOpen()) {
      ticker.next_frame();
      
      while(window.pollEvent(event)){
        switch (event.type) {
          case Event::Closed:
            window.close();
            break;
          case Event::KeyPressed:
            switch (event.key.code) {
              case Keyboard::Escape:
                window.close();
                break;
              case Keyboard::F:
                //wolf.flipH();
                break;
              case Keyboard::A:
                keys[2] = 1;
                if (wolf.isFlippedH()) {
                  wolf.flipH();
                }
                break;
              case Keyboard::D:
                keys[3] = 1;
                if (!wolf.isFlippedH()) {
                  wolf.flipH();
                }
                break;
              case Keyboard::Space:
                if (keys[4] == 0 && wolf_on_ground){
                  keys[4] = 1;
                  wolf.v.y = -20;
                }
                break;
              default:
                break;
            }
            break;
          case Event::KeyReleased:
            switch (event.key.code) {
              case Keyboard::A:
                keys[2] = 0;
                break;
              case Keyboard::D:
                keys[3] = 0;
                break;
              case Keyboard::Space:
                keys[4] = 0;
                break;
              default:
                break;
            }
            break;
          default:
            break;
        }
      }

      if (Game.getStatic(2)->posX() <= 0) {
        Game.getStatic(2)->v.x = 5;
      } else if (Game.getStatic(2)->posX() >= 400) {
        Game.getStatic(2)->v.x = -5;
      }
      if (Game.getStatic(3)->posY() <= 200) {
        Game.getStatic(3)->v.y = 5;
      } else if (Game.getStatic(3)->posY() >= 500) {
        Game.getStatic(3)->v.y = -5;
      }

      //Apply physics
      wolf.v.x = 10*(keys[3]-keys[2]);
      wolf.tick();
      Game.getStatic(2)->tick();
      Game.getStatic(3)->tick();
      wolf_on_ground = false;
      for (int i = 0; i < Game.getStaticsCount(); i++) {
        collide = Game.getStatic(i)->blocks(&wolf);
        if (collide[2] && collide[1]==-1) {
          wolf_on_ground = true;
        }
      };

      //Apply animation
      if (!wolf_on_ground) {
        wolf.setSprite(leap.getCurrentFrame()); // Do leap animation
      }
      else if (wolf.v.x != 0) {
        wolf.setSprite(walk.frameTick()); // Do walking animations
      }
      else {
        wolf.setSprite(idle.getCurrentFrame()); // Do idle animation
      }
      

      //Draw the Screen
      window.clear(Color(42,42,42,255)); // Dark gray.
      window.draw(Game.getStatic(0)->getRectangle()); //Draw the Floor
      window.draw(Game.getStatic(1)->getRectangle());
      window.draw(Game.getStatic(2)->getRectangle());
      window.draw(Game.getStatic(3)->getRectangle());
      window.draw(wolf.getSprite());

      window.display();
    }
    return 0;
}