//#include "../fight_engine.cpp"

class Player {
  public:
    //Methods
    Player(int px, int py) {
        float scale = 2;

        this->w = 64;
        this->h = 64;
        this->body = new KinematicBody2D(px,py,64*scale,64*scale);
        this->body->initHitbox();
        this->punchbox = Hitbox(px,py,32*scale,48*scale);
        this->punchbox.initRectangle();

        this->idle.addAnimationData(make_sprite("./images/wolf_idle.png", scale));
        this->walk.addAnimationData(make_sprite("./images/wolf_walk/wolf_walk1.png", scale));
        this->walk.addAnimationData(make_sprite("./images/wolf_walk/wolf_walk2.png", scale));
        this->leap.addAnimationData(make_sprite("./images/wolf_leap.png", scale));
        this->body->a.y = 1;
    }
    bool punch(Player* PN) {
        float scale = 2;
        //cout << this->punchbox.x << ' ' << this->punchbox.y << ' ' << this->punchbox.w << ' ' << this->punchbox.h << "   ";
        //cout << PN->body->getHitbox()->x << ' ' << PN->body->getHitbox()->y << ' ' << PN->body->getHitbox()->w << ' ' << PN->body->getHitbox()->h << "   ";
        if (this->body->isFlippedH()) {
            this->punchbox.setPosition(this->body->posX()+(48*scale), this->body->posY()+(8*scale));
        } else {
            this->punchbox.setPosition(this->body->posX()-(16*scale), this->body->posY()+(8*scale));
        }
        return this->punchbox.collides(*(PN->body->getHitbox()));
    }
    bool jump(KinematicBody2D* Flowers, int flowercount, Player* PN) {
        if (!this->keys[2]) {
            if (this->on_ground) {
                this->body->v.y = -20;
                this->keys[2] = true;
                return true;
            } else if (this->body->collides(PN->body)) {
                this->body->v.y = -20;
                this->keys[2] = true;
                PN->body->v.y = 20;
                PN->flying = true;
                this->double_jump = true;
                return true;
            } 
            for (int i = 0; i < flowercount; i++) {
                if (this->body->collides(&(Flowers[i]))) {
                    this->body->v.y = -20;
                    this->keys[2] = true;
                    this->double_jump = true;
                    return true;
                }
            }
            if (this->double_jump) {
                this->body->v.y = -20;
                this->keys[2] = true;
                this->double_jump = false;
                return true;
            }
        }
        return false;
    }
    void takeHit(Vector2D dir, bool flipped) {
        this->flying = true;
        this->dmg += 1;
        if (flipped) {
            this->body->v.x += this->dmg;
        } else {
            this->body->v.x -= this->dmg;
        }
        this->body->v.y -= this->dmg;
    }
    void onGround() {
        this->on_ground = true;
        this->double_jump = true;
        this->flying = false;
    }
    //Attributes
    bool on_ground = false;
    bool double_jump = false;
    bool flying = false;
    bool keys[5] = {false,false,false,false,false};
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
    KinematicBody2D* getFlowers() {
        return this->flowers;
    }
    void runPhysics() {
      P1->body->tick();
      P2->body->tick();
      
      int* collide;
      this->P1->on_ground = false;
      this->P2->on_ground = false;
      
      for (int i = 0; i < this->statics_count; i++) {
        this->statics[i].tick();
        collide = this->statics[i].blocks(this->P1->body, P1->flying);
        if (collide[2] && collide[1] <= -1) {
          this->P1->onGround();
        };
        collide = this->statics[i].blocks(this->P2->body, P2->flying);
        if (collide[2] && collide[1] <= -1) {
          this->P2->onGround();
        };
      };
    }
    void buildLevel() {
      this->addStatic(KinematicBody2D(200,900,1200,200));
      this->getStatic(0)->initRectangle();
      this->getStatic(0)->initHitbox();

      this->addStatic(KinematicBody2D(200,600,300,300));
      this->getStatic(1)->initRectangle();
      this->getStatic(1)->initHitbox();

      this->addStatic(KinematicBody2D(200,300,300,50));
      this->getStatic(2)->initRectangle();
      this->getStatic(2)->initHitbox();
      this->getStatic(2)->v.x = 5;

      this->addStatic(KinematicBody2D(1100,200,300,50));
      this->getStatic(3)->initRectangle();
      this->getStatic(3)->initHitbox();
      this->getStatic(3)->v.y = 5;
    }
    bool ON = true;
  private:
    Player* P1;
    Player* P2;
    KinematicBody2D* statics;
    KinematicBody2D* flowers;
    int statics_count;
    int statics_max;
};