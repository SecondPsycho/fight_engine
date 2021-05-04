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
        int dash_timer = 0;
        int attack_timer = 0;
        int invicibility_timer = 0;
        int window_w = 1920;
        int window_h = 1080;
        KinematicBody2D healthBody;
        animation_data healthSprite;
        int healthBar = 10;

        swordPlayer(){
            this->sword_player = *(new KinematicBody2D(100,100,64,64));
            this->f = *(new Vector2D(1,0));
            this->startingPos = *(new Vector2D(100, 100));
            this->attack.max_frame_tick = 5;
            this->healthBody = *(new KinematicBody2D(25, 10, 500, 64));
        }

        void initialize(){
            this->sword_player.setSprite(this->idle.getFrame(0));
            this->sword_player.setPos(startingPos);
            this->sword_player.initHitbox();
            this->sword_player.getHitbox()->initRectangle(); //Testing
            this->sword_player.a.y = 1;
            this->healthSprite.cur_frame = 10;
            this->healthBody.setSprite(this->healthSprite.getCurrentFrame());
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
            this->attack.cur_frame = 0;
            this->healthBar = 10;
            this->healthSprite.cur_frame = 10;
            this->healthBody.setSprite(this->healthSprite.getCurrentFrame());
        }


        void endGame(){
            for (int i=0; i<5; i++){
                keys[i] = 0;
            }
            this->dash_timer = 0;
            this->attack_timer = 0;
            this->invicibility_timer = 0;
        }

        void physicsProcess(){
            if (this->dash_timer > 0){
                if (!(this->sword_player.isFlippedH())){
                    this->sword_player.p.x += 60;
                }
                else if (this->sword_player.isFlippedH()){
                    this->sword_player.p.x -= 60;
                }
            }
            else {
                this->sword_player.p.x += 10*((this->keys[1])-(this->keys[0])); // Speed multiplier default: 10
            }

            this->sword_player.tick();
            this->sword_player.dampen(this->f);
            Vector2D current_pos(this->sword_player.posX(), this->sword_player.posY());
            //cout << "X: " << current_pos.x << " Y: " << current_pos.y << endl;
            // "Walls":
            if (current_pos.x <= 0){
                current_pos.x = 0;
            }
            else if (current_pos.x >= this->window_w - 60){
                current_pos.x = this->window_w - 60;
            }
            this->sword_player.setPos(current_pos);
        }

        void collision(NewGame Game){
            this->on_ground = false;
            for (int i = 0; i < Game.getStaticsCount(); i++) {
            collide = Game.getStatic(i)->blocks(&(this->sword_player), false, 'D');
                if (collide[2] && collide[1] <= -1) {
                    this->on_ground = true;
                };
            };
        }

        void startAttack(){
            this->dash_timer = 10;
            this->attack_timer = 40;
        }

        void onHit(){
            this->invicibility_timer = 40;
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
            if (this->invicibility_timer > 0){
                this->invicibility_timer -= 1;
            }
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
            else if (this->attack_timer > 0){
                sword_player.setSprite(attack.frameTick()); // Do attack animation
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

        void updateHealthBar(){
            this->healthBar--;
            this->healthBody.setSprite(healthSprite.getFrame(healthBar));
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
            // Attack Animations:
            attack.addAnimationData(make_sprite("./images/Sword_Player/Attack/attack0.png", 2));
            attack.addAnimationData(make_sprite("./images/Sword_Player/Attack/attack1.png", 2));
            attack.addAnimationData(make_sprite("./images/Sword_Player/Attack/attack2.png", 2));
            attack.addAnimationData(make_sprite("./images/Sword_Player/Attack/attack3.png", 2));
            attack.addAnimationData(make_sprite("./images/Sword_Player/Attack/attack4.png", 2));
            attack.addAnimationData(make_sprite("./images/Sword_Player/Attack/attack5.png", 2));
            attack.addAnimationData(make_sprite("./images/Sword_Player/Attack/attack6.png", 2));
            attack.addAnimationData(make_sprite("./images/Sword_Player/Attack/attack7.png", 2));
            attack.addAnimationData(make_sprite("./images/Sword_Player/Attack/attack8.png", 2));
            attack.addAnimationData(make_sprite("./images/Sword_Player/Attack/attack9.png", 2));
            // Health Sprites:
            healthSprite.addAnimationData(make_sprite("./images/healthbar0.png"));
            healthSprite.addAnimationData(make_sprite("./images/healthbar1.png"));
            healthSprite.addAnimationData(make_sprite("./images/healthbar2.png"));
            healthSprite.addAnimationData(make_sprite("./images/healthbar3.png"));
            healthSprite.addAnimationData(make_sprite("./images/healthbar4.png"));
            healthSprite.addAnimationData(make_sprite("./images/healthbar5.png"));
            healthSprite.addAnimationData(make_sprite("./images/healthbar6.png"));
            healthSprite.addAnimationData(make_sprite("./images/healthbar7.png"));
            healthSprite.addAnimationData(make_sprite("./images/healthbar8.png"));
            healthSprite.addAnimationData(make_sprite("./images/healthbar9.png"));
            healthSprite.addAnimationData(make_sprite("./images/healthbar10.png"));
        }
};