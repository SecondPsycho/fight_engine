#include "../../fight_engine.cpp"

#define TITLE 0
#define GAME 1
#define POST_GAME 2
#define GAME_OVER 3


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

struct pos{
    pos(int theH, int theW){
        this->thisH=theH;
        this->thisW=theW;
    }
    int h(){
        return this->thisH;
    }
    int w(){
        return this->thisW;
    }
    private:
        int thisH;
        int thisW;
};


const int WINW = 2500;
const int WINH = 1440;


int main(){
    create_window("Joey's Game", WINW, WINH); //previous: 1200, 800
    window.setKeyRepeatEnabled(false);
    int state = TITLE;
    string winner = "";
    

    while(window.isOpen()){
        if(state==TITLE){
            TextBox title(WINW/4, WINH/4-8, "./images/pixer.ttf", "Joey's Game");
            title.setColor(255, 255, 255);

            Event event;
            while(window.pollEvent(event)){
                // cout << "pollEvent while loop (TITLE)" << endl;
                switch (event.type) {
                    case Event::Closed:
                        window.close();
                        break;
                    case Event::KeyReleased:
                        state=GAME;
                        break;
                    default:
                        break;
                }
            }

            window.clear(Color(42,42,42,255)); // Dark gray.

            window.draw(title.text);

            window.display();
        }else if(state==GAME){
            //Create Animation Data
            // For orange:
            animation_data orange_idle;
            animation_data orange_walk;
            animation_data orange_leap;
            animation_data orange_kick;
            animation_data orange_punch;
            // For blue:
            animation_data blue_idle;
            animation_data blue_walk;
            animation_data blue_leap;
            animation_data blue_kick;
            animation_data blue_punch;

            //Create Sprite Data
            // For orange:
            orange_idle.addAnimationData(make_sprite("./images/orange/idle/idle_large1.png"));
            orange_idle.addAnimationData(make_sprite("./images/orange/idle/idle_large2.png"));
            orange_idle.addAnimationData(make_sprite("./images/orange/idle/idle_large3.png"));
            orange_idle.addAnimationData(make_sprite("./images/orange/idle/idle_large2.png"));
            orange_idle.setMaxFrameTick(8);
            orange_walk.addAnimationData(make_sprite("./images/orange/walk/walk_large1.png"));
            orange_walk.addAnimationData(make_sprite("./images/orange/walk/walk_large2.png"));
            orange_walk.addAnimationData(make_sprite("./images/orange/walk/walk_large3.png"));
            orange_walk.addAnimationData(make_sprite("./images/orange/walk/walk_large4.png"));
            orange_walk.setMaxFrameTick(4);
            orange_leap.addAnimationData(make_sprite("./images/orange/leap_large.png"));
            orange_kick.addAnimationData(make_sprite("./images/orange/kick_large.png"));
            orange_punch.addAnimationData(make_sprite("./images/orange/punch_large.png"));
            // For blue:
            blue_idle.addAnimationData(make_sprite("./images/blue/idle/idle_large1.png"));
            blue_idle.addAnimationData(make_sprite("./images/blue/idle/idle_large2.png"));
            blue_idle.addAnimationData(make_sprite("./images/blue/idle/idle_large3.png"));
            blue_idle.addAnimationData(make_sprite("./images/blue/idle/idle_large2.png"));
            blue_idle.setMaxFrameTick(8);
            blue_walk.addAnimationData(make_sprite("./images/blue/walk/walk_large1.png"));
            blue_walk.addAnimationData(make_sprite("./images/blue/walk/walk_large2.png"));
            blue_walk.addAnimationData(make_sprite("./images/blue/walk/walk_large3.png"));
            blue_walk.addAnimationData(make_sprite("./images/blue/walk/walk_large4.png"));
            blue_walk.setMaxFrameTick(4);
            blue_leap.addAnimationData(make_sprite("./images/blue/leap_large.png"));
            blue_kick.addAnimationData(make_sprite("./images/blue/kick_large.png"));
            blue_punch.addAnimationData(make_sprite("./images/blue/punch_large.png"));

            pos center = pos((int)WINH/2, (int)WINW/2);

            // Create orange object
            KinematicBody2D orange(center.w()-64-WINW/4,center.h()-64,64,64);
            orange.setSprite(orange_idle.getCurrentFrame());
            orange.initHitbox();
            orange.getHitbox()->initRectangle(); //Testing
            //orange.adjustHitbox(2,4,52,50);
            orange.a.y = 1;
            // Create blue object
            KinematicBody2D blue(center.w(),center.h()-64,64,64);
            blue.setSprite(blue_idle.getCurrentFrame());
            blue.initHitbox();
            blue.getHitbox()->initRectangle(); //Testing
            //blue.adjustHitbox(2,4,52,50);
            blue.a.y = 1;


            // Initialize game object and static bodies.
                        //xPos, yPos, width, height
            NewGame Game(orange, blue, 20);
            Game.addStatic(KinematicBody2D(center.w()-WINW/2+((WINW-center.w())/4),center.h(),WINW/2,75));
            Game.getStatic(0)->initRectangle();
            Game.getStatic(0)->initHitbox();

            Game.addStatic(KinematicBody2D(2120,0,900,1100));
            Game.getStatic(1)->initRectangle();
            Game.getStatic(1)->initHitbox();

            Game.addStatic(KinematicBody2D(-1100,0,900,1100));
            Game.getStatic(2)->initRectangle();
            Game.getStatic(2)->initHitbox();

            Game.addStatic(KinematicBody2D(-200,-1100,1400,900));
            Game.getStatic(3)->initRectangle();
            Game.getStatic(3)->initHitbox();

            Game.addStatic(KinematicBody2D(-200,1100,2900,900));
            Game.getStatic(4)->initRectangle();
            Game.getStatic(4)->initHitbox();


            Event event;
            int orange_keys[5] = {0,0,0,0,0};
            int blue_keys[5] = {0,0,0,0,0};
            int *collide;
            int orange_punch_counter=0;
            int blue_punch_counter=0;
            int orange_knockback=1;
            int blue_knockback=1;
            bool orange_on_ground = false;
            bool blue_on_ground = false;
            bool orange_touching = false;
            bool blue_touching = false;
            string orange_frame;
            string blue_frame;
            Vector2D orange_f(1,0);
            Vector2D blue_f(1,0);

            // Framerate Control -Cordell King
            Framerate ticker(30);
            while (window.isOpen() && state==GAME) {
                // cout << "isOpen while loop (GAME)" << endl;
                ticker.next_frame();
                
                while(window.pollEvent(event)){
                    // cout << "pollEvent while loop (GAME)" << endl;
                    int velocity_mod = -30;
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
                                    orange_keys[0] = 1;
                                    if (!orange.isFlippedH() && orange_punch_counter==0) {
                                        orange.flipH();
                                    }
                                    break;
                                case Keyboard::D:
                                    orange_keys[1] = 1;
                                    if (orange.isFlippedH() && orange_punch_counter==0) {
                                        orange.flipH();
                                    }
                                break;
                                case Keyboard::W:
                                    if (orange_keys[2] == 0 && orange_on_ground){
                                        //pew_sound.play();
                                        orange_keys[2] = 1;
                                        orange.v.y = velocity_mod;
                                    }
                                    break;
                                case Keyboard::S:
                                    if (orange_keys[3] == 0 && !orange_on_ground){
                                        orange_keys[3] = 1;
                                    }else if (orange_keys[3] == 0){
                                        orange_keys[3] = 2;
                                    }
                                    break;
                                case Keyboard::Left:
                                    blue_keys[0] = 1;
                                    if (!blue.isFlippedH()) {
                                        blue.flipH();
                                    }
                                    break;
                                case Keyboard::Right:
                                    blue_keys[1] = 1;
                                    if (blue.isFlippedH()) {
                                        blue.flipH();
                                    }
                                    break;
                                case Keyboard::Up:
                                    if (blue_keys[2] == 0 && blue_on_ground){
                                        //pew_sound.play();
                                        blue_keys[2] = 1;
                                        blue.v.y = velocity_mod;
                                    }
                                    break;
                                case Keyboard::Down:
                                    if (blue_keys[3] == 0 && !blue_on_ground){
                                        blue_keys[3] = 1;
                                    }else if (blue_keys[3] == 0){
                                        blue_keys[3] = 2;
                                    }
                                    break;
                                default:
                                    break;
                            }
                            break;
                        case Event::KeyReleased:
                            switch (event.key.code) {
                                case Keyboard::A:
                                    orange_keys[0] = 0;
                                    break;
                                case Keyboard::D:
                                    orange_keys[1] = 0;
                                    break;
                                case Keyboard::W:
                                    orange_keys[2] = 0;
                                    break;
                                case Keyboard::Left:
                                    blue_keys[0] = 0;
                                    break;
                                case Keyboard::Right:
                                    blue_keys[1] = 0;
                                    break;
                                case Keyboard::Up:
                                    blue_keys[2] = 0;
                                    break;
                                default:
                                    break;
                            }
                            break;
                        default:
                            break;
                    }
                }

                //Apply Physics
                // orange:
                if(orange_punch_counter==0){
                    orange.p.x += 10*(orange_keys[1]-orange_keys[0]);
                }
                orange.tick();
                // blue:
                if(blue_punch_counter==0){
                    blue.p.x += 10*(blue_keys[1]-blue_keys[0]);
                }
                blue.tick();
                // Statics:

                //Physics Cleanup
                // orange:
                //orange.v.x -= 10*(keys[3]-keys[2]);
                orange.dampen(orange_f);
                // blue:
                //blue.v.x -= 10*(keys[3]-keys[2]);
                blue.dampen(blue_f);

                // is player out of bounds?
                for (int i = 1; i < 5; i++) {
                    if (Game.getStatic(i)->collides(&orange) || Game.getStatic(i)->collides(&blue)) {
                        // cout << "ended game" << endl;
                        state=POST_GAME;
                        if(Game.getStatic(i)->collides(&blue)){
                            winner = "orange";
                        }else{
                            winner = "blue";
                        }
                        break;
                    };
                };

                // orange: 
                orange_on_ground = false;
                for (int i = 0; i < Game.getStaticsCount(); i++) {
                collide = Game.getStatic(i)->blocks(&orange);
                    if (collide[2] && collide[1] <= -1) {
                        orange_on_ground = true;
                    };
                };
                // blue:
                blue_on_ground = false;
                for (int i = 0; i < Game.getStaticsCount(); i++) {
                collide = Game.getStatic(i)->blocks(&blue);
                    if (collide[2] && collide[1] <= -1) {
                        blue_on_ground = true;
                    };
                };

                
                orange_frame = "";
                blue_frame = "";
                orange_touching = false;
                blue_touching = false;

                //blue and orange touch?
                if(orange.collides(&blue)){
                    orange_touching = true;
                }
                if(blue.collides(&orange)){
                    blue_touching = true;
                }

                //remove kick animation if on ground
                if(orange_on_ground && orange_keys[3]==1){
                    orange_keys[3]=0;
                }
                if(blue_on_ground && blue_keys[3]==1){
                    blue_keys[3]=0;
                }
                
                //Apply animation
                // orange:
                if (!orange_on_ground) {
                    if(orange_keys[3]==0){
                        orange.setSprite(orange_leap.getCurrentFrame()); // Do leap animation
                        orange_frame = "leap";
                    }else{
                        orange.setSprite(orange_kick.getCurrentFrame()); // Do kick animation
                        orange_frame = "kick";
                    }
                }else if (orange_punch_counter!=0 || orange_keys[3]==2) {
                    orange.setSprite(orange_punch.getCurrentFrame()); // Do punch animation
                    orange_frame = "punch";
                    orange_punch_counter++;
                    if(orange_punch_counter>=10){
                        orange_punch_counter=0;
                        orange_keys[3]=0;
                    }
                }else if (orange_keys[0]^orange_keys[1]) {
                    orange.setSprite(orange_walk.frameTick()); // Do walking animations
                    orange_frame = "walk";
                }else {
                    orange.setSprite(orange_idle.frameTick()); // Do idle animation
                    orange_frame = "idle";
                }

                // blue:
                if (!blue_on_ground) {
                    if(blue_keys[3]==0){
                        blue.setSprite(blue_leap.getCurrentFrame()); // Do leap animation
                        blue_frame = "leap";
                    }else{
                        blue.setSprite(blue_kick.getCurrentFrame()); // Do kick animation
                        blue_frame = "kick";
                    }
                }else if (blue_punch_counter!=0 || blue_keys[3]==2) {
                    blue.setSprite(blue_punch.getCurrentFrame()); // Do punch animation
                    blue_frame = "punch";
                    blue_punch_counter++;
                    if(blue_punch_counter>=10){
                        blue_punch_counter=0;
                        blue_keys[3]=0;
                    }
                }else if (blue_keys[0]^blue_keys[1]) {
                    blue.setSprite(blue_walk.frameTick()); // Do walking animations
                    blue_frame = "walk";
                }else {
                    blue.setSprite(blue_idle.frameTick()); // Do idle animation
                    blue_frame = "idle";
                }
                
                //handle attacks
                if((orange_frame=="punch" || orange_frame=="kick") && (orange_touching && blue_touching) && (blue_frame=="punch" || blue_frame=="kick")){
                    // cout << "Both collided" << endl;
                }else if((orange_frame=="punch" || orange_frame=="kick") && (orange_touching && blue_touching) && (blue_frame!="punch" && blue_frame!="kick")){
                    // cout << "ORANGE HIT BLUE" << endl;
                    if(orange.isFlippedH()){
                        blue.p.x -= 10*(blue_knockback);
                    }else{
                        blue.p.x += 10*(blue_knockback);
                    }
                    blue_knockback*=2;
                }else if((blue_frame=="punch" || blue_frame=="kick") && (orange_touching && blue_touching) && (orange_frame!="punch" && orange_frame!="kick")){
                    // cout << "BLUE HIT ORANGE" << endl;
                    if(blue.isFlippedH()){
                        orange.p.x -= 10*(orange_knockback);
                    }else{
                        orange.p.x += 10*(orange_knockback);
                    }
                    orange_knockback*=2;
                }

                TextBox orange_KB(center.w()-64-WINW/4, WINH/2, "./images/pixer.ttf", to_string(orange_knockback));
                orange_KB.setColor(255, 150, 0);
                TextBox blue_KB(center.w(), WINH/2, "./images/pixer.ttf", to_string(blue_knockback));
                blue_KB.setColor(50, 20, 255);
                

                //Draw the Screen
                window.clear(Color(42,42,42,255)); // Dark gray.
                window.draw(Game.getStatic(0)->getRectangle()); //Draw the floor
                window.draw(Game.getStatic(1)->getRectangle()); //Draw the right wall
                window.draw(Game.getStatic(2)->getRectangle()); //Draw the left wall
                window.draw(Game.getStatic(3)->getRectangle()); //Draw the ceiling
                window.draw(Game.getStatic(4)->getRectangle()); //Draw the crevice
                window.draw(orange.getSprite());
                // window.draw(orange.getHitbox()->getRectangle());
                window.draw(blue.getSprite());
                // window.draw(blue.getHitbox()->getRectangle());
                window.draw(orange_KB.text);
                window.draw(blue_KB.text);

                
                

                // Text Test
                //window.draw(test_text.txt);

                window.display();
            }
        }else if(state==POST_GAME){
            TextBox title(WINW/4, WINH/4-8, "./images/pixer.ttf", winner + " wins!");
            title.setColor(255, 255, 255);

            Event event;
            while(window.pollEvent(event)){
                // cout << "pollEvent while loop (POST_GAME)" << endl;
                switch (event.type) {
                    case Event::Closed:
                        window.close();
                        break;
                    case Event::KeyReleased:
                        state=TITLE;
                        break;
                    default:
                        break;
                }
            }

            window.clear(Color(42,42,42,255)); // Dark gray.

            window.draw(title.text);

            window.display();
        }
        
    }
    return 0;

}