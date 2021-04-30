class swordPlayer {
    public:
        animation_data running;
        animation_data attack;
        animation_data idle;
        animation_data jump;
        animation_data death;
        KinematicBody2D sword_player;
        Vector2D startingPos;
        Vector2D f;
        int *collide;
        int keys[5] = {0,0,0,0,0};
        bool on_ground = false;
        bool dead = false;

        swordPlayer(){
            this->sword_player = *(new KinematicBody2D(50,100,64,64));
            this->f = *(new Vector2D(1,0));
            this->startingPos = *(new Vector2D(50, 100));
        }

        void initialize(){
            this->sword_player.setSprite(this->idle.getFrame(0));
            this->sword_player.setPos(startingPos);
            this->sword_player.initHitbox();
            this->sword_player.getHitbox()->initRectangle(); //Testing
            this->sword_player.a.y = 1;
        }

        void reset(){
            this->sword_player.setSprite(this->idle.getFrame(0));
            this->sword_player.setPos(startingPos);
            this->sword_player.a.y = 1;
            this->sword_player.p.x = 0;
            this->dead = false;
            this->running.cur_frame = 0;
            this->idle.cur_frame = 0;
            this->death.cur_frame = 0;
            this->jump.cur_frame = 0;
        }

        void die(){
            this->dead = true;
            for (int i=0; i<5; i++){
                keys[i] = 0;
            }
        }

        void physicsProcess(){
            this->sword_player.p.x += 10*((this->keys[1])-(this->keys[0])); // Speed multiplier was 10
            this->sword_player.tick();
            this->sword_player.dampen(this->f);
        }

        void collision(NewGame Game){
            this->on_ground = false;
            for (int i = 0; i < Game.getStaticsCount(); i++) {
            collide = Game.getStatic(i)->blocks(&(this->sword_player));
                if (collide[2] && collide[1] <= -1) {
                    this->on_ground = true;
                };
            };
        }


        void animate(){
            if (dead){
                if (death.cur_frame == 4){
                    sword_player.setSprite(death.getFrame(4));
                }
                else {
                    sword_player.setSprite(death.frameTick()); // Do death animation
                }
            }
            else if (!on_ground) {
                sword_player.setSprite(jump.frameTick()); // Do jump animation
            }
            else if (keys[0]^keys[1]) {
                sword_player.setSprite(running.frameTick()); // Do walking animations
            }
            else {
                sword_player.setSprite(idle.frameTick()); // Do idle animation
            }
        }


        void addAllAnimations(){
            // Idle Animations:
            idle.addAnimationData(make_sprite("./images/Sword_Player/Idle/idle0.png", 2));
            idle.addAnimationData(make_sprite("./images/Sword_Player/Idle/idle1.png", 2));
            idle.addAnimationData(make_sprite("./images/Sword_Player/Idle/idle2.png", 2));
            idle.addAnimationData(make_sprite("./images/Sword_Player/Idle/idle3.png", 2));
            // Jump Animations:
            jump.addAnimationData(make_sprite("./images/Sword_Player/Jump/jump0.png", 2));
            jump.addAnimationData(make_sprite("./images/Sword_Player/Jump/jump1.png", 2));
            jump.addAnimationData(make_sprite("./images/Sword_Player/Jump/jump2.png", 2));
            jump.addAnimationData(make_sprite("./images/Sword_Player/Jump/jump3.png", 2));
            jump.addAnimationData(make_sprite("./images/Sword_Player/Jump/jump4.png", 2));
            jump.addAnimationData(make_sprite("./images/Sword_Player/Jump/jump5.png", 2));
            jump.addAnimationData(make_sprite("./images/Sword_Player/Jump/jump6.png", 2));
            jump.addAnimationData(make_sprite("./images/Sword_Player/Jump/jump7.png", 2));
            jump.addAnimationData(make_sprite("./images/Sword_Player/Jump/jump8.png", 2));
            jump.addAnimationData(make_sprite("./images/Sword_Player/Jump/jump9.png", 2));
            // Running Animations:
            running.addAnimationData(make_sprite("./images/Sword_Player/Running/running0.png", 2));
            running.addAnimationData(make_sprite("./images/Sword_Player/Running/running1.png", 2));
            running.addAnimationData(make_sprite("./images/Sword_Player/Running/running2.png", 2));
            running.addAnimationData(make_sprite("./images/Sword_Player/Running/running3.png", 2));
            running.addAnimationData(make_sprite("./images/Sword_Player/Running/running4.png", 2));
            running.addAnimationData(make_sprite("./images/Sword_Player/Running/running5.png", 2));
            // Death Animations:
            death.addAnimationData(make_sprite("./images/Sword_Player/Death/death0.png", 2));
            death.addAnimationData(make_sprite("./images/Sword_Player/Death/death1.png", 2));
            death.addAnimationData(make_sprite("./images/Sword_Player/Death/death2.png", 2));
            death.addAnimationData(make_sprite("./images/Sword_Player/Death/death3.png", 2));
            death.addAnimationData(make_sprite("./images/Sword_Player/Death/death4.png", 2));
        }
};