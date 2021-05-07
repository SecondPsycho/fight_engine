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

            this->body->getHitbox()->initRectangle();
            this->body->adjustHitbox(30,10,68,108);

            this->punchbox = Hitbox(px,py,32*scale,48*scale);
            this->punchbox.initRectangle();

            this->ouch.addAnimationData(make_sprite("./images/Ailie/A_hurt_1.png"));
            this->stop.addAnimationData(make_sprite("./images/Ailie/A_stop_1.png"));

            this->fist_windup.addAnimationData(make_sprite("./images/Ailie/A_fist_1.png"));
            this->fist.addAnimationData(make_sprite("./images/Ailie/A_fist_2.png"));

            this->walk.addAnimationData(make_sprite("./images/Ailie/A_walk_1.png"));
            this->walk.addAnimationData(make_sprite("./images/Ailie/A_walk_2.png"));
            this->walk.addAnimationData(make_sprite("./images/Ailie/A_walk_3.png"));
            this->walk.addAnimationData(make_sprite("./images/Ailie/A_walk_2.png"));
            this->walk.setMaxFrameTick(8);

            this->idle.addAnimationData(make_sprite("./images/Ailie/A_idle_1.png"));
            this->idle.addAnimationData(make_sprite("./images/Ailie/A_idle_2.png"));
            this->idle.addAnimationData(make_sprite("./images/Ailie/A_idle_3.png"));
            this->idle.addAnimationData(make_sprite("./images/Ailie/A_idle_2.png"));
            this->idle.setMaxFrameTick(10);

            this->knocked.addAnimationData(make_sprite("./images/Ailie/A_hurt_2.png"));

            this->kick.addAnimationData(make_sprite("./images/Ailie/A_kick_1.png"));
            this->kick_windup.addAnimationData(make_sprite("./images/Ailie/A_kick_2.png"));

            this->hang.addAnimationData(make_sprite("./images/Ailie/A_hang_s.png"));

            this->rise.addAnimationData(make_sprite("./images/Ailie/A_rise_s.png"));
            this->fall.addAnimationData(make_sprite("./images/Ailie/A_fall_s.png"));

            this->body->a.y = 1;
        }
        void animate() {
            if (this->hurt) {
                if (this->stomped) {
                    this->body->setSprite(this->knocked.frameTick()); 
                } else {
                    this->body->setSprite(this->ouch.frameTick()); 
                }
            } else if (this->on_ground) {
                if (this->blocking) {
                    this->body->setSprite(this->stop.frameTick());
                } else if (this->attackCooldown > 0) {
                    this->body->setSprite(this->fist.frameTick());
                } else if (this->keys[1]^this->keys[0]) {
                    this->body->setSprite(this->walk.frameTick()); // Do walking animations
                } else {
                    this->body->setSprite(this->idle.frameTick()); // Do idle animation
                }
            } else {
                if (this->attackCooldown > 0) {
                    this->body->setSprite(this->kick.frameTick());
                } else if (this->hanging) {
                    this->body->setSprite(this->hang.frameTick());
                } else if (this->body->p.y + this->body->v.y < 0) {
                    this->body->setSprite(this->rise.frameTick());
                } else {
                    this->body->setSprite(this->fall.frameTick());
                }
            }
        }
        void Tick() {
            if (this->attackCooldown == 0) {
                if (!this->blocking) {
                    this->body->p.x += (this->keys[1]-this->keys[0])*10;
                    this->body->p.y += (this->keys[3]-this->keys[2])*5;
                }
            }
            this->body->tick();
            this->on_ground = false;
            this->hanging = false;
        }
        void runCollision(int* collide) {
            if (collide[2]) {
                if (collide[1] >= 1) {
                    this->hanging = true;
                } else if (collide[1] <= -1) {
                    this->onGround();
                }
            };
        }
        bool punch(Player* PN) {
            float scale = 2;
            this->attackCooldown += 3;
            if (this->body->isFlippedH()) {
                this->punchbox.setPosition(this->body->posX()-(16*scale), this->body->posY()+(8*scale));
            } else {
                this->punchbox.setPosition(this->body->posX()+(48*scale), this->body->posY()+(8*scale));
            }
            return this->punchbox.collides(*(PN->body->getHitbox()));
        }
        bool jump(KinematicBody2D* Flowers, int flowercount, Player* PN) {
            if (!this->keys[2]) {
                if (this->on_ground) {
                    this->make_jump();
                    return true;
                } else if (this->body->collides(PN->body)) {
                    this->make_jump();
                    PN->body->v.y = 20;
                    PN->stomped = true;
                    PN->hurt = true;
                    this->double_jump = true;
                    return true;
                } 
                for (int i = 0; i < flowercount; i++) {
                    if (this->body->collides(&(Flowers[i]))) {
                        this->make_jump();
                        this->double_jump = true;
                        return true;
                    }
                }
                if (this->double_jump) {
                    this->make_jump();
                    this->double_jump = false;
                    return true;
                }
            }
            return false;
        }
        bool block() {
            this->keys[3] = true;
            if (this->on_ground) {
                this->blocking = true;
            }
            return this->blocking;
        }
        void unblock() {
            this->keys[3] = false;
            this->blocking = false;
        }
        void make_jump() {
            this->body->v.y = -20;
            this->keys[2] = true;
            this->hurt = false;
            this->stomped = false;
        }
        void takeHit(Vector2D dir, bool flipped) {
            this->hurt = true;
            this->dmg += 5;
            if (flipped) {
                this->body->v.x -= this->dmg;
            } else {
                this->body->v.x += this->dmg;
            }
            this->body->v.y -= this->dmg;
        }
        void onGround() {
            this->on_ground = true;
            this->double_jump = true;
            this->hurt = false;
            this->stomped = false;
        }
        void faceRight(bool right) {
            if (this->body->isFlippedH() == right && !this->blocking) {
                this->body->flipH();
            }
        }
        void cooldowns() {
            if (this->attackCooldown > 0) {
                this->attackCooldown -= 1;
            }
        }

        //Attributes
        bool hurt = false;
        bool on_ground = false;
        bool windup = false;
        bool attack = false;
        bool hanging = false;
        bool stomped = false;
        bool blocking = false;

        bool double_jump = false;
        int attackCooldown = 0;
        
        //Left, Right, Up, Down, Punch
        bool keys[5] = {false,false,false,false,false};
        //Body
        KinematicBody2D* body;
        Hitbox punchbox;
        animation_data ouch;
        animation_data stop;
        animation_data fist_windup;
        animation_data fist;
        animation_data walk;
        animation_data idle;
        animation_data knocked;
        animation_data kick_windup;
        animation_data kick;
        animation_data hang;
        animation_data rise;
        animation_data fall;
    private:
        int w, h;
        int dmg = 0;
};

class NewGame {
    public:
        NewGame(Player* player1, Player* player2, int n = 10, int f = 10, int w = 10) {
            this->statics_count = 0;
            this->statics_max = n;
            this->flowers_count = 0;
            this->flowers_max = n;
            this->waters_count = 0;
            this->waters_max = n;
            this->statics = new KinematicBody2D[this->statics_max];
            this->flowers = new KinematicBody2D[this->flowers_max];
            this->waters = new KinematicBody2D[this->waters_max];
            this->P1 = player1;
            this->P2 = player2;
        }
        KinematicBody2D* getStatics() {
            return this->statics;
        }
        KinematicBody2D* getStatic(int n) {
            if (n < this->statics_max && n >= 0) {
                return &(this->statics[n]);
            }
            cout << "Invalid Static Index.\n";
            return nullptr;
        }
        void addStatic(KinematicBody2D newstatic) {
            if (this->statics_count < this->statics_max) {
                this->statics[this->statics_count] = newstatic;
                this->statics_count++;
            } else {
                cout << "Cannot add another static.\n";
            }
        }
        int getStaticsCount() {
            return this->statics_count;
        }
        KinematicBody2D* getFlowers() {
            return this->flowers;
        }
        KinematicBody2D* getFlower(int n) {
            if (n < this->flowers_max && n >= 0) {
                return &(this->flowers[n]);
            }
            cout << "Invalid Flower Index.\n";
            return nullptr;
        }
        void addFlower(KinematicBody2D newflower) {
            if (this->flowers_count < this->flowers_max) {
                this->flowers[this->flowers_count] = newflower;
                this->flowers_count++;
            } else {
                cout << "Cannot add another flower.\n";
            }
        }
        int getFlowersCount() {
            return this->flowers_count;
        }
        KinematicBody2D* getWaters() {
            return this->waters;
        }
        KinematicBody2D* getWater(int n) {
            if (n < this->waters_max && n >= 0) {
                return &(this->waters[n]);
            }
            cout << "Invalid Water Index.\n";
            return nullptr;
        }
        void addWater(KinematicBody2D newwater) {
            if (this->waters_count < this->waters_max) {
                this->waters[this->waters_count] = newwater;
                this->waters_count++;
            } else {
                cout << "Cannot add another water.\n";
            }
        }
        int getWatersCount() {
            return this->waters_count;
        }
        void movePlatforms(int worldshifty) {
            if (this->getStatic(2)->posX() <= 200) {
                this->getStatic(2)->v.x = 5;
            } else if (this->getStatic(2)->posX() >= 800) {
                this->getStatic(2)->v.x = -5;
            }
            if (this->getStatic(3)->posY() <= worldshifty+200) {
                this->getStatic(3)->v.y = 5;
            } else if (this->getStatic(3)->posY() >= worldshifty+700) {
                this->getStatic(3)->v.y = -5;
            }
        }
        void runPhysics(int t, int fps) {
            fps = fps / 10;
            if (t % fps == 0) {
                this->P1->cooldowns();
                this->P2->cooldowns();
            }
            this->P1->Tick();
            this->P2->Tick();

            for (int i = 0; i < this->flowers_count; i++) {
                this->flowers[i].tick();
            }
            
            /* Stop the Waters for testing purposes
            int waterspeed = -1;
            if (this->waters[0].pos().y >= 1200) {
                waterspeed = -3;
            }
            for (int i = 0; i < this->waters_count; i++) {
                this->waters[i].tick();
                this->waters[i].v.y = waterspeed;
            }
            //*/
            /* Change Player Movement Underwater
            if (this->P1->body->collides(&this->waters[0])) {
                this->P1->body->a.y = 0;
                //this->P1->body->dampen(Vector2D(1,1));
            } else {
                this->P1->body->a.y = 1;
            }
            if (this->P2->body->collides(&this->waters[0])) {
                this->P2->body->a.y = 0;
                //this->P1->body->dampen(Vector2D(1,1));
            } else {
                this->P2->body->a.y = 1;
            }
            //*/

            int* collide;
            for (int i = 0; i < this->statics_count; i++) {
                this->statics[i].tick();
                collide = this->statics[i].blocks(this->P1->body, P1->hurt);
                this->P1->runCollision(collide);
                
                collide = this->statics[i].blocks(this->P2->body, P2->hurt);
                this->P2->runCollision(collide);
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

            this->addFlower(KinematicBody2D(75,500,50,50));
            this->getFlower(0)->initRectangle();
            this->getFlower(0)->setRectColor(255,255,0,255);
            this->getFlower(0)->initHitbox();

            this->addFlower(KinematicBody2D(1475,500,50,50));
            this->getFlower(1)->initRectangle();
            this->getFlower(1)->setRectColor(255,255,0,255);
            this->getFlower(1)->initHitbox();

            this->addWater(KinematicBody2D(0,1300,2000,2000));
            this->getWater(0)->initRectangle();
            this->getWater(0)->setRectColor(0,192,255,192);
            this->getWater(0)->initHitbox();
            this->getWater(0)->v.y = -1;

            this->addWater(KinematicBody2D(0,1500,2000,2000));
            this->getWater(1)->initRectangle();
            this->getWater(1)->setRectColor(0,192,255,255);
            this->getWater(1)->v.y = -1;
            //this->getWater(0)->initHitbox();
        }
        bool ON = true;
    private:
        Player* P1;
        Player* P2;
        KinematicBody2D* statics;
        int statics_count;
        int statics_max;
        KinematicBody2D* flowers;
        int flowers_count;
        int flowers_max;
        KinematicBody2D* waters;
        int waters_count;
        int waters_max;
};