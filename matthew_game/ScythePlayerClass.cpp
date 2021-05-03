class scythePlayer {
    public:
        animation_data attack;
        animation_data idle;
        animation_data death;
        KinematicBody2D scythe_player;
        Vector2D f;
        Vector2D startingPos;
        int *collide;
        int keys[5] = {0,0,0,0,0};
        bool on_ground = false;
        bool dead = false;
        int dash_timer = 0;
        int attack_timer = 0;
        int window_w;
        int window_h;
        //int healthBar = 100;

        scythePlayer(int window_W, int window_H){
            this->scythe_player = *(new KinematicBody2D(1500,100,64,64));
            this->f = *(new Vector2D(1,0));
            this->startingPos = *(new Vector2D(1500, 100));
            //this->attack.max_frame_tick = 5; Might not want for this char.
            this->window_w = window_W;
            this->window_h = window_H;
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
            this->attack.cur_frame = 0;
        }

        void endGame(){
            for (int i=0; i<5; i++){
                keys[i] = 0;
            }
            this->dash_timer = 0;
            this->attack_timer = 0;
        }

        void physicsProcess(){
            if (this->dash_timer > 0){
                if (!(this->scythe_player.isFlippedH())){
                    this->scythe_player.p.x += 60;
                }
                else if (this->scythe_player.isFlippedH()){
                    this->scythe_player.p.x -= 60;
                }
            }
            else {
                this->scythe_player.p.x += 10*((this->keys[1])-(this->keys[0])); // Speed multiplier was 10
            }

            this->scythe_player.tick();
            this->scythe_player.dampen(this->f);
            Vector2D current_pos(this->scythe_player.posX(), this->scythe_player.posY());
            //cout << "X: " << current_pos.x << " Y: " << current_pos.y << endl;
            // "Walls":
            if (current_pos.x <= 0){
                current_pos.x = 0;
            }
            else if (current_pos.x >= window_w - 60){
                current_pos.x = window_w - 60;
            }
            this->scythe_player.setPos(current_pos);
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

        void startAttack(){
            this->dash_timer = 10;
            this->attack_timer = 40;
        }

        void timers(){
            if (this->dash_timer > 0){
                this->dash_timer -= 1;
            }
            if (this->attack_timer > 0){
                this->attack_timer -= 1;
            }
            else {
                this->attack.cur_frame = 0;
            }
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
            else if (this->attack_timer > 0){
                scythe_player.setSprite(attack.frameTick()); // Do attack animation
            }
            else {
                scythe_player.setSprite(idle.frameTick()); // Do idle animation
            }
        }

        void updateHealthBar(){

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
            // Attack Animations:
            attack.addAnimationData(make_sprite("./images/Scythe_Player/Attack/attack0.png"));
            attack.addAnimationData(make_sprite("./images/Scythe_Player/Attack/attack1.png"));
            attack.addAnimationData(make_sprite("./images/Scythe_Player/Attack/attack2.png"));
            attack.addAnimationData(make_sprite("./images/Scythe_Player/Attack/attack3.png"));
            attack.addAnimationData(make_sprite("./images/Scythe_Player/Attack/attack4.png"));
            attack.addAnimationData(make_sprite("./images/Scythe_Player/Attack/attack5.png"));
            attack.addAnimationData(make_sprite("./images/Scythe_Player/Attack/attack6.png"));
            attack.addAnimationData(make_sprite("./images/Scythe_Player/Attack/attack7.png"));
            attack.addAnimationData(make_sprite("./images/Scythe_Player/Attack/attack8.png"));
            attack.addAnimationData(make_sprite("./images/Scythe_Player/Attack/attack9.png"));
            attack.addAnimationData(make_sprite("./images/Scythe_Player/Attack/attack10.png"));
            attack.addAnimationData(make_sprite("./images/Scythe_Player/Attack/attack11.png"));
            attack.addAnimationData(make_sprite("./images/Scythe_Player/Attack/attack12.png"));
        }
};