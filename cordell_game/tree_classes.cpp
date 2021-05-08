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

            this->punchbox = Hitbox(px,py,32*scale,32*scale);
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

            this->kick_windup.addAnimationData(make_sprite("./images/Ailie/A_kick_1.png"));
            this->kick.addAnimationData(make_sprite("./images/Ailie/A_kick_2.png"));

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
                    if (!this->hurt && !this->attack) {
                        this->body->p.y += (this->keys[3]-this->keys[2])*5;
                    }
                }
            }
            this->body->tick();
            
            this->hanging = false;
        }
        void runCollision(int* collide) {
            if (collide[2]) {
                if (collide[1] >= 1 and collide[0] == 0) {
                    this->hanging = true;
                    this->on_ground = false;
                    //this->double_jump = true;
                } else if (collide[1] <= -1) {
                    this->onGround();
                } else {
                    this->on_ground = false;
                }
            };
        }
        void processAttack(Player* PN) {
            if (this->attackCooldown > 0 && !this->hit_target && this->punch(PN)) {
                this->hit_target = true;
                if (PN->blocking) {
                    this->takeHit(Vector2D(0,0), !this->body->isFlippedH());
                } else {
                    PN->takeHit(Vector2D(0,0), this->body->isFlippedH());
                }
            }

        }
        bool punch(Player* PN) {
            float scale = 2;
            if (this->on_ground) {
                if (this->body->isFlippedH()) {
                    this->punchbox.setPosition(this->body->posX()-(4*scale), this->body->posY()+(8*scale));
                } else {
                    this->punchbox.setPosition(this->body->posX()+(36*scale), this->body->posY()+(8*scale));
                }
            } else {
                if (this->body->isFlippedH()) {
                    this->punchbox.setPosition(this->body->posX()-(4*scale), this->body->posY()+(32*scale));
                } else {
                    this->punchbox.setPosition(this->body->posX()+(36*scale), this->body->posY()+(32*scale));
                }
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
            this->on_ground = false;
            this->body->v.y = -20;
            this->keys[2] = true;
            this->hurt = false;
            this->stomped = false;
        }
        void takeHit(Vector2D dir, bool flipped) {
            this->hurt = true;
            this->on_ground = false;
            if (this->attackCooldown > 0) {
                this->attackCooldown = 1;
            }

            this->dmg += 5;
            if (flipped) {
                this->body->v.x -= this->dmg;
            } else {
                this->body->v.x += this->dmg;
            }
            this->body->v.y -= this->dmg/3;
        }
        void onGround() {
            if (!this->on_ground) {
                this->attackCooldown = 0;
                this->hit_target = false;
            }
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
                if (this->attackCooldown == 0) {
                    this->hit_target = false;
                }
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
        bool hit_target = false;
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
        NewGame(Player* player1, Player* player2, int s = 30, int f = 30, int w = 2) {
            this->statics_count = 0;
            this->statics_max = s;
            this->flowers_count = 0;
            this->flowers_max = f;
            this->waters_count = 0;
            this->waters_max = w;
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
        void addStatic(int x, int y, int w = 300, int h = 50) {
            KinematicBody2D newstatic(x,y,w,h);
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
        void addFlower(int x, int y) {
            KinematicBody2D newflower(x,y,50,50);
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
        void addWater(int x, int y, int w = 2000, int h = 2000) {
            KinematicBody2D newwater(x,y,w,h);
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
        void runPhysics(int t, int fps, int worldshifty) {
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
            
            // Waters
            int waterspeed = -1;
            if (this->waters[0].pos().y >= 1205) {
                waterspeed = -3;
            }
            this->waters[0].tick();
            this->waters[1].tick();
            if (-4200 + worldshifty < this->waters[0].pos().y ) {
                this->waters[0].v.y = waterspeed;
                this->waters[1].v.y = waterspeed;
            } else {
                this->waters[0].v.y = 0;
                this->waters[1].v.y = 0;
            }

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
        void platformControlX(int index, int left, int right, int speed = 5) {
            KinematicBody2D* box = this->getStatic(index);
            if (box->posX() <= left) {
                box->v.x = speed;
            } else if (box->posX() >= right - box->getHitbox()->w) {
                box->v.x = -speed;
            }
        }
        void platformControlY(int worldshifty, int index, int top, int bottom, int speed = 5) {
            KinematicBody2D* box = this->getStatic(index);
            if (box->posY() <= worldshifty + top) {
                box->v.y = speed;
            } else if (box->posY() >= worldshifty + bottom - box->getHitbox()->h) {
                box->v.y = -speed;
            }
        }
        void movePlatforms(int worldshifty) {
            //Section 1
            int count = 5;
            platformControlX(count, 200, 1000); count++;
            platformControlY(worldshifty, count, 0, 475); count++;

            //Section 2
            count = 10;
            platformControlX(count, 500, 1400); count++;
            platformControlX(count, 300, 1000); count++;

            //Section 3
            count = 14;
            platformControlY(worldshifty, count, -2500, -800); count++;
            platformControlY(worldshifty, count, -1600, -900, 10); count++;

            //Section 4
            count = 20;
            platformControlY(worldshifty, count, -3400, -2600); count++;

            //Section 5
            count = 24;
            platformControlX(count, 800, 1400); count++;

            //Section 6
            count = 28;
            platformControlY(worldshifty, count, -5000, -4450); count++;
            platformControlY(worldshifty, count, -5000, -4450); count++;
        }
        void buildLevel() {

            //Section 0
            this->addStatic(200,900,1200,200);
            this->addStatic(200,600,300,300);
            this->addStatic(1100,600,300,300);

            //Wall 1
            this->addStatic(-100,-700,200,2200);
            this->addStatic(1500,-2600,200,4000);

            //Section 1
            this->addStatic(200,300); //5
            this->addStatic(1100,0); //6

            this->addFlower(75,450);
            this->addFlower(1475,500);
            this->addFlower(900,400);

            //Section 2
            this->addStatic(100,-100,350,50);
            this->addStatic(100,-500,200);
            this->addStatic(-100,-700,300);

            this->addStatic(500,-300,400); //10
            this->addStatic(300,-700,400,75); //11

            this->addFlower(725,75);
            this->addFlower(1300,-150);
            this->addFlower(200,-250);
            this->addFlower(125,-1025);
            this->addFlower(75,-850);
            this->addFlower(275,-875);

            //Section 3
            this->addStatic(0,-1300,300,100);
            this->addStatic(700,-1300,300,100);

            this->addStatic(1100,-2500,300,100); //14
            this->addStatic(400,-1600,200); //15

            this->addFlower(0,-1750);
            this->addFlower(750,-1525);
            this->addFlower(200,-1500);
            this->addFlower(675,-1275);
            this->addFlower(1000,-1000);

            //Section 4
            this->addStatic(100,-2000,900,200);
            this->addStatic(200,-2100,200,100);
            this->addStatic(800,-2450,200,100);
            this->addStatic(1400,-2600,300);

            this->addStatic(500,-3400,600,50); //20

            this->addFlower(550,-2500);
            this->addFlower(300,-2725);
            this->addFlower(1350,-3450);
            this->addFlower(375,-3050);
            this->addFlower(1350,-3175);
            this->addFlower(1125,-3300);
            this->addFlower(200,-3200);
            this->addFlower(225,-3425);
            this->addFlower(75,-2900);

            //Wall 2
            this->addStatic(-100,-4000,200,1700);
            this->addStatic(1500,-4000,200,1100);

            //Section 5
            this->addStatic(100,-3700,600,100);

            this->addStatic(800,-3700,300,100); //24

            //Section 6
            this->addStatic(300,-4300,1000,300);
            this->addStatic(400,-4600);
            this->addStatic(900,-4600);

            this->addStatic(0,-5000); //28
            this->addStatic(1300,-5000); //29

            this->addFlower(125,-4175);
            this->addFlower(1425,-4175);
            this->addFlower(775,-4800);
            this->addFlower(525,-5000);
            this->addFlower(1025,-5000);

            //Set 'em up
            for (int i = 0; i < this->statics_count; i ++) {
                this->getStatic(i)->initRectangle();
                this->getStatic(i)->initHitbox();
            }
            for (int i = 0; i < this->flowers_count; i ++) {
                this->getFlower(i)->initRectangle();
                this->getFlower(i)->setRectColor(255,255,0,255);
                this->getFlower(i)->initHitbox();
            }

            //Water
            this->addWater(0,1300);
            this->getWater(0)->initHitbox();
            this->addWater(0,1500);

            for (int i = 0; i < this->waters_count; i ++) {
                this->getWater(i)->initRectangle();
                this->getWater(i)->v.y = -1;
            }
            this->getWater(0)->setRectColor(0,192,255,127);
            this->getWater(1)->setRectColor(0,192,255,255);
            
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