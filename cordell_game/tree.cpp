#include "../fight_engine.cpp"


class NewGame {
  public:
    NewGame(Player* player1, Player* player2, int n) {
      this->statics_count = 0;
      this->statics_max = n;
      this->statics = new KinematicBody2D[this->statics_max];
      this->P1 = player1;
      this->P2 = player2;
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
    void runPhysics() {
      P1->self->tick();
      P2->self->tick();
      
      int* collide;
      this->P1->on_ground = false;
      this->P2->on_ground = false;
      
      for (int i = 0; i < this->statics_count; i++) {
        this->statics[i].tick();
        collide = this->statics[i].blocks(this->P1->self);
        if (collide[2] && collide[1] <= -1) {
          this->P1->on_ground = true;
        };
        collide = this->statics[i].blocks(this->P2->self);
        if (collide[2] && collide[1] <= -1) {
          this->P2->on_ground = true;
        };
      };
    }
  private:
    Player* P1;
    Player* P2;
    KinematicBody2D* statics;
    int statics_count;
    int statics_max;
};

class Player {
  public:
    Player() {

    }
    //Attributes
    bool on_ground = false;
    //Body
    KinematicBody2D* self;
    animation_data idle;
    animation_data walk;
    animation_data leap;
};


int main(int argc, char* argv[]){
    create_window("Up the Tree", 800, 800);
    window.setKeyRepeatEnabled(false);
    
    //Create an Animation
    animation_data idle;
    animation_data walk;
    animation_data leap;

    Player* P1 = new Player;
    P1->self = new KinematicBody2D(50,100,64,64);
    P1->self.initHitbox();

    Player* P2 = new Player;
    P2->self = new KinematicBody2D(450,100,64,64);
    P2->self.initHitbox();

    /*
    KinematicBody2D wolf(50,100,64,64);
    //wolf.setSprite(idle.getCurrentFrame());
    wolf.initHitbox();
    wolf.getHitbox()->initRectangle(); //Testing
    wolf.a.y = 1;
    //*/

    //Create Sprite Data
    P1->idle.addAnimationData(make_sprite("./images/wolf_idle.png"));
    P1->walk.addAnimationData(make_sprite("./images/wolf_walk/wolf_walk1.png"));
    P1->walk.addAnimationData(make_sprite("./images/wolf_walk/wolf_walk2.png"));
    P1->leap.addAnimationData(make_sprite("./images/wolf_leap.png"));

    P2->idle.addAnimationData(make_sprite("./images/wolf_idle.png"));
    P2->walk.addAnimationData(make_sprite("./images/wolf_walk/wolf_walk1.png"));
    P2->walk.addAnimationData(make_sprite("./images/wolf_walk/wolf_walk2.png"));
    P2->leap.addAnimationData(make_sprite("./images/wolf_leap.png"));

    NewGame Game(P1, P2, 6);
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
    

    Song my_music("sounds/Restoring the Light, Facing the Dark.wav");
    //my_music.setPitch(1.2f);
    //my_music.setVolume(10.0f);
    my_music.play();
    SFX pew_sound("sounds/pew.wav");
    pew_sound.setVolume(100.0f);
        
    Event event;
    bool keys[10] = {false,false,false,false,false,false,false,false,false,false};
    //bool wolf_on_ground = false;
    Vector2D f(1,0);
    int worldshifty = 0;
    int frame, seconds = 0;

    int t = 0;
    int falldelay = 5;
    Framerate ticker(30);
    while (window.isOpen()) {
      ticker.next_frame();
      frame += 1;
      if (frame >= 30) {
        frame = 0;
        seconds += 1;
        cout << seconds << endl;
        if (seconds >= 82) {
          falldelay = 1000000000;
        }
        if (seconds >= 32) {
          falldelay = 2;
        }
      }
      t += 1;
      if (t >= falldelay) {
        t = 0;
        for (int i = 0; i < Game.getStaticsCount(); i++) {
          Game.getStatic(i)->p.y += 1;
        }
        worldshifty += 1;
      }
      
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
                /*
                keys[2] = 1;
                if (wolf.isFlippedH()) {
                  wolf.flipH();
                }
                //*/
                break;
              case Keyboard::D:
                /*
                keys[3] = 1;
                if (!wolf.isFlippedH()) {
                  wolf.flipH();
                }
                break;
                //*/
              case Keyboard::Space:
                /*
                if (keys[4] == 0 && wolf_on_ground){
                  //pew_sound.play();
                  keys[4] = 1;
                  wolf.v.y = -20;
                }
                break;
                //*/
              default:
                break;
            }
            break;
          case Event::KeyReleased:
            switch (event.key.code) {
              case Keyboard::A:
                //keys[2] = 0;
                break;
              case Keyboard::D:
                //keys[3] = 0;
                break;
              case Keyboard::Space:
                //keys[4] = 0;
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
      if (Game.getStatic(3)->posY() <= worldshifty+200) {
        Game.getStatic(3)->v.y = 5;
      } else if (Game.getStatic(3)->posY() >= worldshifty+500) {
        Game.getStatic(3)->v.y = -5;
      }

      //Apply Physics
      Game.runPhysics();

      //Apply animation
      if (!P1->on_ground) {
        P1->self->setSprite(P1.leap.getCurrentFrame()); // Do leap animation
      }
      else if (keys[2]^keys[3]) {
        P1->self->setSprite(P1.walk.frameTick()); // Do walking animations
      }
      else {
        P1->self->setSprite(P1.idle.getCurrentFrame()); // Do idle animation
      }
      
      if (!P2->on_ground) {
        P2->self->setSprite(P2.leap.getCurrentFrame()); // Do leap animation
      }
      else if (keys[2]^keys[3]) {
        P2->self->setSprite(P2.walk.frameTick()); // Do walking animations
      }
      else {
        P2->self->setSprite(P2.idle.getCurrentFrame()); // Do idle animation
      }
      

      //Draw the Screen
      window.draw(P1->self->getSprite());
      window.draw(P2->self->getSprite());
      
      window.clear(Color(42,42,42,255)); // Dark gray.
      for (int i = 0; i < Game.getStaticsCount(); i++) {
        window.draw(Game.getStatic(i)->getRectangle())
      }
      //window.draw(wolf.getHitbox()->getRectangle());
      window.display();
    }
    return 0;
}
