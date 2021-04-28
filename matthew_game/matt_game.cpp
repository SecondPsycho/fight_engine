#include "../fight_engine.cpp"

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
    //create_window("Matthew's Game", 2560, 1440); // Fullscreen but at cost of sprites looking small and needing to make stage bigger with adjusting speed and jump vals.
    create_window("Matthew's Game", 800, 800); // Small window but works same as in out test.cpp
    window.setKeyRepeatEnabled(false);

    //Create Animation Data
    // For wolf:
    animation_data wolf_idle;
    animation_data wolf_walk;
    animation_data wolf_leap;
    // For human:
    animation_data human_idle;
    animation_data human_walk;

    //Create Sprite Data
    // For wolf:
    wolf_idle.addAnimationData(make_sprite("./images/wolf_idle.png"));
    wolf_walk.addAnimationData(make_sprite("./images/wolf_walk/wolf_walk1.png"));
    wolf_walk.addAnimationData(make_sprite("./images/wolf_walk/wolf_walk2.png"));
    wolf_leap.addAnimationData(make_sprite("./images/wolf_leap.png"));
    // For human:
    human_idle.addAnimationData(make_sprite("./images/human_idle.png"));
    human_walk.addAnimationData(make_sprite("./images/human_walk/human_walk1.png"));
    human_walk.addAnimationData(make_sprite("./images/human_walk/human_walk2.png"));
    human_walk.addAnimationData(make_sprite("./images/human_walk/human_walk3.png"));

    // Create wolf object
    KinematicBody2D wolf(50,100,64,64);
    wolf.setSprite(wolf_idle.getCurrentFrame());
    wolf.initHitbox();
    wolf.getHitbox()->initRectangle(); //Testing
    //wolf.adjustHitbox(2,4,52,50);
    wolf.a.y = 1;
    // Create human object
    KinematicBody2D human(550,100,64,64);
    human.setSprite(human_idle.getCurrentFrame());
    human.initHitbox();
    human.getHitbox()->initRectangle(); //Testing
    //human.adjustHitbox(2,4,52,50);
    human.a.y = 1;

    // Initialize game object and static bodies.
    NewGame Game(wolf, human, 6);
    Game.addStatic(KinematicBody2D(0,600,800,200));
    Game.getStatic(0)->initRectangle();
    Game.getStatic(0)->initHitbox();


    // Textbox attempt
    //Textbox test_text((50, 50), "", "Hello!"); // TODO NEED FONT PATH

    Event event;
    int wolf_keys[5] = {0,0,0,0,0};
    int human_keys[5] = {0,0,0,0,0};
    int *collide;
    bool wolf_on_ground = false;
    bool human_on_ground = false;
    Vector2D wolf_f(1,0);
    Vector2D human_f(1,0);

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
                wolf_keys[0] = 1;
                if (wolf.isFlippedH()) {
                    wolf.flipH();
                }
                break;
                case Keyboard::D:
                wolf_keys[1] = 1;
                if (!wolf.isFlippedH()) {
                    wolf.flipH();
                }
                break;
                case Keyboard::W:
                if (wolf_keys[2] == 0 && wolf_on_ground){
                    //pew_sound.play();
                    wolf_keys[2] = 1;
                    wolf.v.y = -20;
                }
                break;
                case Keyboard::Left:
                human_keys[0] = 1;
                if (human.isFlippedH()) {
                    human.flipH();
                }
                break;
                case Keyboard::Right:
                human_keys[1] = 1;
                if (!human.isFlippedH()) {
                    human.flipH();
                }
                break;
                case Keyboard::Up:
                if (human_keys[2] == 0 && human_on_ground){
                    //pew_sound.play();
                    human_keys[2] = 1;
                    human.v.y = -20;
                }
                break;
                default:
                break;
            }
            break;
            case Event::KeyReleased:
            switch (event.key.code) {
                case Keyboard::A:
                wolf_keys[0] = 0;
                break;
                case Keyboard::D:
                wolf_keys[1] = 0;
                break;
                case Keyboard::W:
                wolf_keys[2] = 0;
                break;
                case Keyboard::Left:
                human_keys[0] = 0;
                break;
                case Keyboard::Right:
                human_keys[1] = 0;
                break;
                case Keyboard::Up:
                human_keys[2] = 0;
                break;
                default:
                break;
            }
            break;
            default:
            break;
        }
        }

        /*
        if (Game.getStatic(2)->posX() <= 0) {
        Game.getStatic(2)->v.x = 5;
        } else if (Game.getStatic(2)->posX() >= 400) {
        Game.getStatic(2)->v.x = -5;
        }
        if (Game.getStatic(3)->posY() <= 200) {
        Game.getStatic(3)->v.y = 5;
        } else if (Game.getStatic(3)->posY() >= 500) {
        Game.getStatic(3)->v.y = -5;
        }
        */

        //Apply Physics
        // Wolf:
        wolf.p.x += 50*(wolf_keys[1]-wolf_keys[0]); // Speed multiplier was 10
        wolf.tick();
        // Human:
        human.p.x += 50*(human_keys[1]-human_keys[0]); // Speed multiplier was 10
        human.tick();
        // Statics:
        //Game.getStatic(2)->tick();
        //Game.getStatic(3)->tick();

        //Physics Cleanup
        // Wolf:
        //wolf.v.x -= 10*(keys[3]-keys[2]);
        wolf.dampen(wolf_f);
        // Human:
        //human.v.x -= 10*(keys[3]-keys[2]);
        human.dampen(human_f);

        // Wolf: 
        wolf_on_ground = false;
        for (int i = 0; i < Game.getStaticsCount(); i++) {
        collide = Game.getStatic(i)->blocks(&wolf);
            if (collide[2] && collide[1] <= -1) {
                wolf_on_ground = true;
            };
        };
        // Human:
        human_on_ground = false;
        for (int i = 0; i < Game.getStaticsCount(); i++) {
        collide = Game.getStatic(i)->blocks(&human);
            if (collide[2] && collide[1] <= -1) {
                human_on_ground = true;
            };
        };


        //Apply animation
        // Wolf:
        if (!wolf_on_ground) {
        wolf.setSprite(wolf_leap.getCurrentFrame()); // Do leap animation
        }
        else if (wolf_keys[0]^wolf_keys[1]) {
        wolf.setSprite(wolf_walk.frameTick()); // Do walking animations
        }
        else {
        wolf.setSprite(wolf_idle.getCurrentFrame()); // Do idle animation
        }
        // Human:
        if (human_keys[0]^human_keys[1]) {
        human.setSprite(human_walk.frameTick()); // Do walking animations
        }
        else {
        human.setSprite(human_idle.getCurrentFrame()); // Do idle animation
        }
        

        //Draw the Screen
        window.clear(Color(42,42,42,255)); // Dark gray.
        window.draw(Game.getStatic(0)->getRectangle()); //Draw the Floor
        //window.draw(Game.getStatic(1)->getRectangle());
        //window.draw(Game.getStatic(2)->getRectangle());
        //window.draw(Game.getStatic(3)->getRectangle());
        window.draw(wolf.getSprite());
        window.draw(wolf.getHitbox()->getRectangle());
        window.draw(human.getSprite());
        window.draw(human.getHitbox()->getRectangle());

        // Text Test
        //window.draw(test_text.txt);

        window.display();
    }
    return 0;

}