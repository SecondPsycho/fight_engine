//#include "../fight_engine.cpp"

class NewGame {
  public:
    NewGame(KinematicBody2D player1, KinematicBody2D player2, int n) {
      this->statics_count = 0;
      this->statics_max = n;
      this->statics = new KinematicBody2D[this->statics_max];
      this->player1 = player1;
      this->player2 = player2;
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
    KinematicBody2D player1;
    KinematicBody2D player2;
    KinematicBody2D* statics;
    int statics_count;
    int statics_max;
};


class swordPlayer {
    public:
        animation_data running;
        animation_data attack;
        animation_data idle;
        animation_data jump;
        KinematicBody2D sword_player;
        Vector2D f;
        int *collide;
        int keys[5] = {0,0,0,0,0};
        bool on_ground = false;

        swordPlayer(){
            KinematicBody2D sword_player(50,100,64,64);
            Vector2D f(1,0);
        }

        void initializeAndReset(){
            sword_player.setSprite(this->idle.getFrame(0));
            //Vector2D vec(50, 100);
            //sword_player.setPos(vec);
            sword_player.initHitbox();
            sword_player.getHitbox()->initRectangle(); //Testing
            sword_player.a.y = 1;
        }

        void physicsProcess(){
            this->sword_player.p.x += 10*((this->keys[1])-(this->keys[0])); // Speed multiplier was 10
            this->sword_player.tick();
            this->sword_player.dampen(this->f);
        }


        void animate(){

        }

        void addAllAnimations(){
            // Idle Animations:
            idle.addAnimationData(make_sprite("./images/Sword_Player/Idle/idle0.png"));
            idle.addAnimationData(make_sprite("./images/Sword_Player/Idle/idle1.png"));
            idle.addAnimationData(make_sprite("./images/Sword_Player/Idle/idle2.png"));
            idle.addAnimationData(make_sprite("./images/Sword_Player/Idle/idle3.png"));
            // Jump Animations:
            jump.addAnimationData(make_sprite("./images/Sword_Player/Jump/jump0.png"));
            jump.addAnimationData(make_sprite("./images/Sword_Player/Jump/jump1.png"));
            jump.addAnimationData(make_sprite("./images/Sword_Player/Jump/jump2.png"));
            jump.addAnimationData(make_sprite("./images/Sword_Player/Jump/jump3.png"));
            jump.addAnimationData(make_sprite("./images/Sword_Player/Jump/jump4.png"));
            jump.addAnimationData(make_sprite("./images/Sword_Player/Jump/jump5.png"));
            jump.addAnimationData(make_sprite("./images/Sword_Player/Jump/jump6.png"));
            jump.addAnimationData(make_sprite("./images/Sword_Player/Jump/jump7.png"));
            jump.addAnimationData(make_sprite("./images/Sword_Player/Jump/jump8.png"));
            jump.addAnimationData(make_sprite("./images/Sword_Player/Jump/jump9.png"));
        }
};