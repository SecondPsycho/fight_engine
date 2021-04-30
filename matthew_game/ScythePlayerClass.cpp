class scythePlayer {
    public:
        animation_data attack;
        animation_data idle;
        animation_data jump;
        animation_data death;
        KinematicBody2D scythe_player;
        Vector2D f;
        Vector2D startingPos;
        int *collide;
        int keys[5] = {0,0,0,0,0};
        bool on_ground = false;
        bool dead = false;

        scythePlayer(){
            this->scythe_player = *(new KinematicBody2D(1500,100,64,64));
            this->f = *(new Vector2D(1,0));
            this->startingPos = *(new Vector2D(1500, 100));
        }

        void initialize(){
            scythe_player.setSprite(this->idle.getFrame(0));
            scythe_player.setPos(startingPos);
            scythe_player.initHitbox();
            scythe_player.getHitbox()->initRectangle(); //Testing
            scythe_player.a.y = 1;
            this->scythe_player.flipH();
        }

        void reset(){
            scythe_player.setSprite(this->idle.getFrame(0));
            scythe_player.setPos(startingPos);
            this->scythe_player.a.y = 1;
            this->scythe_player.p.x = 0;
            this->dead = false;
            this->idle.cur_frame = 0;
            this->death.cur_frame = 0;
        }

        void die(){
            this->dead = true;
            for (int i=0; i<5; i++){
                keys[i] = 0;
            }
        }

        void physicsProcess(){
            this->scythe_player.p.x += 10*((this->keys[1])-(this->keys[0])); // Speed multiplier was 10
            this->scythe_player.tick();
            this->scythe_player.dampen(this->f);
        }

        void collision(NewGame Game){
            this->on_ground = false;
            for (int i = 0; i < Game.getStaticsCount(); i++) {
            collide = Game.getStatic(i)->blocks(&(this->scythe_player));
                if (collide[2] && collide[1] <= -1) {
                    this->on_ground = true;
                };
            };
        }


        void animate(){
            if (dead){
                if (death.cur_frame == 17){
                    scythe_player.setSprite(death.getFrame(17));
                }
                else {
                    scythe_player.setSprite(death.frameTick()); // Do death animation
                }
            }
            else {
                scythe_player.setSprite(idle.frameTick()); // Do idle animation
            }
        }

        void addAllAnimations(){
            // Idle Animations:
            idle.addAnimationData(make_sprite("./images/Scythe_Player/Idle/idle0.png"));
            idle.addAnimationData(make_sprite("./images/Scythe_Player/Idle/idle1.png"));
            idle.addAnimationData(make_sprite("./images/Scythe_Player/Idle/idle2.png"));
            idle.addAnimationData(make_sprite("./images/Scythe_Player/Idle/idle3.png"));
            idle.addAnimationData(make_sprite("./images/Scythe_Player/Idle/idle4.png"));
            idle.addAnimationData(make_sprite("./images/Scythe_Player/Idle/idle5.png"));
            idle.addAnimationData(make_sprite("./images/Scythe_Player/Idle/idle6.png"));
            idle.addAnimationData(make_sprite("./images/Scythe_Player/Idle/idle7.png"));
            // Death Animations:
            death.addAnimationData(make_sprite("./images/Scythe_Player/Death/death0.png", 2));
            death.addAnimationData(make_sprite("./images/Scythe_Player/Death/death1.png", 2));
            death.addAnimationData(make_sprite("./images/Scythe_Player/Death/death2.png", 2));
            death.addAnimationData(make_sprite("./images/Scythe_Player/Death/death3.png", 2));
            death.addAnimationData(make_sprite("./images/Scythe_Player/Death/death4.png", 2));
            death.addAnimationData(make_sprite("./images/Scythe_Player/Death/death5.png", 2));
            death.addAnimationData(make_sprite("./images/Scythe_Player/Death/death6.png", 2));
            death.addAnimationData(make_sprite("./images/Scythe_Player/Death/death7.png", 2));
            death.addAnimationData(make_sprite("./images/Scythe_Player/Death/death8.png", 2));
            death.addAnimationData(make_sprite("./images/Scythe_Player/Death/death9.png", 2));
            death.addAnimationData(make_sprite("./images/Scythe_Player/Death/death10.png", 2));
            death.addAnimationData(make_sprite("./images/Scythe_Player/Death/death11.png", 2));
            death.addAnimationData(make_sprite("./images/Scythe_Player/Death/death12.png", 2));
            death.addAnimationData(make_sprite("./images/Scythe_Player/Death/death13.png", 2));
            death.addAnimationData(make_sprite("./images/Scythe_Player/Death/death14.png", 2));
            death.addAnimationData(make_sprite("./images/Scythe_Player/Death/death15.png", 2));
            death.addAnimationData(make_sprite("./images/Scythe_Player/Death/death16.png", 2));
            death.addAnimationData(make_sprite("./images/Scythe_Player/Death/death17.png", 2));
        }
};