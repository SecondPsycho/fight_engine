#include "../fight_engine.cpp"


class Player {
  public:
    //Methods
    Player(int px, int py) {
      this->w = 64;
      this->h = 64;
      this->body = new KinematicBody2D(px,py,64,64);
      this->body->initHitbox();
      this->punchbox = Hitbox(px,py,32,32);
      this->punchbox.initRectangle();
    }
    bool punch(Player* PN) {
      //cout << this->punchbox.x << ' ' << this->punchbox.y << ' ' << this->punchbox.w << ' ' << this->punchbox.h << "   ";
      //out << PN->body->getHitbox()->x << ' ' << PN->body->getHitbox()->y << ' ' << PN->body->getHitbox()->w << ' ' << PN->body->getHitbox()->h << "   ";
      if (this->body->isFlippedH()) {
        this->punchbox.setPosition(this->body->posX()+48, this->body->posY()+16);
      } else {
        this->punchbox.setPosition(this->body->posX()-16, this->body->posY()+16);
      }
      return this->punchbox.collides(*(PN->body->getHitbox()));
    }
    void takeHit(Vector2D dir) {

    }
    //Attributes
    bool on_ground = false;
    bool double_jump = false;
    //Body
    KinematicBody2D* body;
    Hitbox punchbox;
    animation_data idle;
    animation_data walk;
    animation_data leap;
  private:
    int w, h;
    int dmg = 0;
};

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
      P1->body->tick();
      P2->body->tick();
      
      int* collide;
      this->P1->on_ground = false;
      this->P2->on_ground = false;
      
      for (int i = 0; i < this->statics_count; i++) {
        this->statics[i].tick();
        collide = this->statics[i].blocks(this->P1->body);
        if (collide[2] && collide[1] <= -1) {
          this->P1->on_ground = true;
          this->P1->double_jump = true;
        };
        collide = this->statics[i].blocks(this->P2->body);
        if (collide[2] && collide[1] <= -1) {
          this->P2->on_ground = true;
          this->P2->double_jump = true;
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


int main(int argc, char* argv[]){
    create_window("Up the Tree", 800, 800);
    window.setKeyRepeatEnabled(false);
    
    //Create an Animation
    animation_data idle;
    animation_data walk;
    animation_data leap;

    Player* P1 = new Player(50,100);
    Player* P2 = new Player(450,100);

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
    P1->body->a.y = 1;

    P2->idle.addAnimationData(make_sprite("./images/wolf_idle.png"));
    P2->walk.addAnimationData(make_sprite("./images/wolf_walk/wolf_walk1.png"));
    P2->walk.addAnimationData(make_sprite("./images/wolf_walk/wolf_walk2.png"));
    P2->leap.addAnimationData(make_sprite("./images/wolf_leap.png"));
    P2->body->a.y = 1;

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
    //Left, Right, Jump, Attack
    bool P1keys[5] = {false,false,false,false};
    bool P2keys[5] = {false,false,false,false};
    int P1dmg = 0, P2dmg = 0;
    Vector2D f(1,0);
    int worldshifty = 0;
    //int frame, seconds = 0;

    int t = 0;
    int falldelay = 5;
    Framerate ticker(30);
    while (window.isOpen()) {
      ticker.next_frame();
      /*
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
      //*/
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
              case Keyboard::A:
                P1keys[0] = true;
                if (P1->body->isFlippedH()) {
                  P1->body->flipH();
                }
                break;
              case Keyboard::D:
                P1keys[1] = true;
                if (!P1->body->isFlippedH()) {
                  P1->body->flipH();
                }
                break;
              case Keyboard::W:
                if (!P1keys[2]) {
                  if (P1->on_ground) {
                    P1->body->v.y = -15;
                    P1keys[2] = true;
                  } else if (P1->double_jump) {
                    P1->body->v.y = -15;
                    P1keys[2] = true;
                    P1->double_jump = false;
                  }
                }
                break;
              case Keyboard::E:
                P1keys[3] = true;
                if (P1->punch(P2)) {
                  P2dmg += 1;
                  if (P1->body->isFlippedH()) {
                    P2->body->v.x += P2dmg;
                  } else {
                    P2->body->v.x -= P2dmg;
                  }
                  P2->body->v.y -= P2dmg;
                }
                break;
              case Keyboard::H:
              case Keyboard::Left:
                P2keys[0] = true;
                if (P2->body->isFlippedH()) {
                  P2->body->flipH();
                }
                break;
              case Keyboard::K:
              case Keyboard::Right:
                P2keys[1] = true;
                if (!P2->body->isFlippedH()) {
                  P2->body->flipH();
                }
                break;
              case Keyboard::U:
              case Keyboard::Up:
                if (!P2keys[2]) {
                  if (P2->on_ground) {
                    P2->body->v.y = -15;
                    P2keys[2] = true;
                  } else if (P2->double_jump) {
                    P2->body->v.y = -15;
                    P2keys[2] = true;
                    P2->double_jump = false;
                  }
                }
                break;
              case Keyboard::I:
                P2keys[3] = true;
                if (P2->punch(P1)) {
                  P1dmg += 1;
                  if (P2->body->isFlippedH()) {
                    P1->body->v.x += P1dmg;
                  } else {
                    P1->body->v.x -= P1dmg;
                  }
                  P1->body->v.y -= P1dmg;
                }
                break;
              default:
                break;
            }
            break;
          case Event::KeyReleased:
            switch (event.key.code) {
              case Keyboard::A:
                P1keys[0] = false;
                break;
              case Keyboard::D:
                P1keys[1] = false;
                break;
              case Keyboard::W:
                P1keys[2] = false;
                break;
              case Keyboard::E:
                P1keys[3] = false;
                break;
              case Keyboard::H:
              case Keyboard::Left:
                P2keys[0] = false;
                break;
              case Keyboard::K:
              case Keyboard::Right:
                P2keys[1] = false;
                break;
              case Keyboard::U:
              case Keyboard::Up:
                P2keys[2] = false;
                break;
              case Keyboard::I:
                P2keys[3] = false;
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
      P1->body->p.x += (P1keys[1]-P1keys[0])*10;
      P2->body->p.x += (P2keys[1]-P2keys[0])*10;
      P1->body->p.y -= (P1keys[2])*3;
      P2->body->p.y -= (P2keys[2])*3;
      Game.runPhysics();
      P1->body->dampen(f);
      P2->body->dampen(f);

      //cout << P1->body->posX() << ' ' << P1->body->posY() << "   " << P2->body->posX() << ' ' << P2->body->posY() << endl;
      //Apply animation
      if (!P1->on_ground) {
        P1->body->setSprite(P1->leap.getCurrentFrame()); // Do leap animation
      }
      else if (P1keys[1]^P1keys[0]) {
        P1->body->setSprite(P1->walk.frameTick()); // Do walking animations
      }
      else {
        P1->body->setSprite(P1->idle.getCurrentFrame()); // Do idle animation
      }
      
      if (!P2->on_ground) {
        P2->body->setSprite(P2->leap.getCurrentFrame()); // Do leap animation
      }
      else if (P2keys[1]^P2keys[0]) {
        P2->body->setSprite(P2->walk.frameTick()); // Do walking animations
      }
      else {
        P2->body->setSprite(P2->idle.getCurrentFrame()); // Do idle animation
      }
      

      //Draw the Screen
      window.clear(Color(42,42,42,255)); // Dark gray.

      window.draw(P1->body->getSprite());
      window.draw(P2->body->getSprite());

      for (int i = 0; i < Game.getStaticsCount(); i++) {
        window.draw(Game.getStatic(i)->getRectangle());
      }
      //window.draw(wolf.getHitbox()->getRectangle());
      window.draw(P1->punchbox.getRectangle());
      window.draw(P2->punchbox.getRectangle());
      window.display();
    }
    return 0;
}
