#include "../../fight_engine.cpp"

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


int main(){
    //create_window("Joey's Game", 2560, 1440); // Fullscreen but at cost of sprites looking small and needing to make stage bigger with adjusting speed and jump vals.
    create_window("Joey's Game", 800, 800); // Small window but works same as in out test.cpp
    window.setKeyRepeatEnabled(false);

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
    orange_idle.addAnimationData(make_sprite("./images/orange/idle/idle1.png"));
    orange_idle.addAnimationData(make_sprite("./images/orange/idle/idle2.png"));
    orange_idle.addAnimationData(make_sprite("./images/orange/idle/idle3.png"));
    orange_walk.addAnimationData(make_sprite("./images/orange/walk/walk1.png"));
    orange_walk.addAnimationData(make_sprite("./images/orange/walk/walk2.png"));
    orange_leap.addAnimationData(make_sprite("./images/orange/leap.png"));
    orange_kick.addAnimationData(make_sprite("./images/orange/kick.png"));
    orange_punch.addAnimationData(make_sprite("./images/orange/punch.png"));
    // For blue:
    blue_idle.addAnimationData(make_sprite("./images/blue/idle/idle1.png"));
    blue_idle.addAnimationData(make_sprite("./images/blue/idle/idle2.png"));
    blue_idle.addAnimationData(make_sprite("./images/blue/idle/idle3.png"));
    blue_walk.addAnimationData(make_sprite("./images/blue/walk/walk1.png"));
    blue_walk.addAnimationData(make_sprite("./images/blue/walk/walk2.png"));
    blue_leap.addAnimationData(make_sprite("./images/blue/leap.png"));
    blue_kick.addAnimationData(make_sprite("./images/blue/kick.png"));
    blue_punch.addAnimationData(make_sprite("./images/blue/punch.png"));

    // Create orange object
    KinematicBody2D orange(50,100,64,64);
    orange.setSprite(orange_idle.getCurrentFrame());
    orange.initHitbox();
    orange.getHitbox()->initRectangle(); //Testing
    //orange.adjustHitbox(2,4,52,50);
    orange.a.y = 1;
    // Create blue object
    KinematicBody2D blue(550,100,64,64);
    blue.setSprite(blue_idle.getCurrentFrame());
    blue.initHitbox();
    blue.getHitbox()->initRectangle(); //Testing
    //blue.adjustHitbox(2,4,52,50);
    blue.a.y = 1;

    // Initialize game object and static bodies.
    NewGame Game(orange, blue, 6);
    Game.addStatic(KinematicBody2D(0,600,800,200));
    Game.getStatic(0)->initRectangle();
    Game.getStatic(0)->initHitbox();


    // Textbox attempt
    //Textbox test_text((50, 50), "", "Hello!"); // TODO NEED FONT PATH

    Event event;
    int orange_keys[5] = {0,0,0,0,0};
    int blue_keys[5] = {0,0,0,0,0};
    int *collide;
    bool orange_on_ground = false;
    bool blue_on_ground = false;
    Vector2D orange_f(1,0);
    Vector2D blue_f(1,0);

    // Framerate Control -Cordell King
    Framerate ticker(30);
    while (window.isOpen()) {
        ticker.next_frame();
        
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
                        orange_keys[0] = 1;
                        if (orange.isFlippedH()) {
                            orange.flipH();
                        }
                        break;
                    case Keyboard::D:
                        orange_keys[1] = 1;
                        if (!orange.isFlippedH()) {
                            orange.flipH();
                        }
                    break;
                    case Keyboard::W:
                        if (orange_keys[2] == 0 && orange_on_ground){
                            //pew_sound.play();
                            orange_keys[2] = 1;
                            orange.v.y = -20;
                        }
                        break;
                    case Keyboard::Left:
                        blue_keys[0] = 1;
                        if (blue.isFlippedH()) {
                            blue.flipH();
                        }
                        break;
                    case Keyboard::Right:
                        blue_keys[1] = 1;
                        if (!blue.isFlippedH()) {
                            blue.flipH();
                        }
                        break;
                    case Keyboard::Up:
                        if (blue_keys[2] == 0 && blue_on_ground){
                            //pew_sound.play();
                            blue_keys[2] = 1;
                            blue.v.y = -20;
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
        orange.p.x += 50*(orange_keys[1]-orange_keys[0]); // Speed multiplier was 10
        orange.tick();
        // blue:
        blue.p.x += 50*(blue_keys[1]-blue_keys[0]); // Speed multiplier was 10
        blue.tick();
        // Statics:

        //Physics Cleanup
        // orange:
        //orange.v.x -= 10*(keys[3]-keys[2]);
        orange.dampen(orange_f);
        // blue:
        //blue.v.x -= 10*(keys[3]-keys[2]);
        blue.dampen(blue_f);

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


        //Apply animation
        // orange:
        if (!orange_on_ground) {
            orange.setSprite(orange_leap.getCurrentFrame()); // Do leap animation
        }else if (orange_keys[0]^orange_keys[1]) {
            orange.setSprite(orange_walk.frameTick()); // Do walking animations
        }else {
            orange.setSprite(orange_idle.getCurrentFrame()); // Do idle animation
        }

        // blue:
        if (blue_keys[0]^blue_keys[1]) {
            blue.setSprite(blue_walk.frameTick()); // Do walking animations
        }else {
            blue.setSprite(blue_idle.getCurrentFrame()); // Do idle animation
        }
        

        //Draw the Screen
        window.clear(Color(42,42,42,255)); // Dark gray.
        window.draw(Game.getStatic(0)->getRectangle()); //Draw the Floor
        //window.draw(Game.getStatic(1)->getRectangle());
        //window.draw(Game.getStatic(2)->getRectangle());
        //window.draw(Game.getStatic(3)->getRectangle());
        window.draw(orange.getSprite());
        window.draw(orange.getHitbox()->getRectangle());
        window.draw(blue.getSprite());
        window.draw(blue.getHitbox()->getRectangle());

        // Text Test
        //window.draw(test_text.txt);

        window.display();
    }
    return 0;

}