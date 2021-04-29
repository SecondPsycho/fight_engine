#include "../fight_engine.cpp"
#include "./matt_classes.cpp"

void handle_controls(Event event);


create_window("Matthew's Game", 2560, 1440); // Fullscreen but at cost of sprites looking small and needing to make stage bigger with adjusting speed and jump vals.
//create_window("Matthew's Game", 800, 800); // Small window but works same as in out test.cpp
//window.setKeyRepeatEnabled(false);


// Globals:
swordPlayer player1;

KinematicBody2D human(550,100,64,64);

int human_keys[5] = {0,0,0,0,0};
bool human_on_ground = false;




int main(){
    /*
    create_window("Matthew's Game", 2560, 1440); // Fullscreen but at cost of sprites looking small and needing to make stage bigger with adjusting speed and jump vals.
    //create_window("Matthew's Game", 800, 800); // Small window but works same as in out test.cpp
    window.setKeyRepeatEnabled(false);
    */
    window.setKeyRepeatEnabled(false);

    /*
    swordPlayer player1;
    */

    /*
    KinematicBody2D human(550,100,64,64);
    */
    //human.setSprite(human_idle.getCurrentFrame());
    //human.initHitbox();
    //human.getHitbox()->initRectangle(); //Testing
    //human.adjustHitbox(2,4,52,50);
    //human.a.y = 1;

    player1.addAllAnimations();
    player1.initializeAndReset();


    NewGame Game(player1.sword_player, human, 6);
    Game.addStatic(KinematicBody2D(0,600,800,200));
    Game.getStatic(0)->initRectangle();
    Game.getStatic(0)->initHitbox();

    // Textbox attempt
    //Textbox test_text((50, 50), "./font/good-times-rg.ttf", "Hello!"); // TODO NEED FONT PATH

    Event event;
    int wolf_keys[5] = {0,0,0,0,0};
    //int human_keys[5] = {0,0,0,0,0};
    int *collide;
    bool wolf_on_ground = false;
    //bool human_on_ground = false;
    Vector2D wolf_f(1,0);
    Vector2D human_f(1,0);


    // Framerate Control -Cordell King
    Framerate ticker(30);
    while (window.isOpen()) {
        ticker.next_frame();
        
        while(window.pollEvent(event)){
            handle_controls(event);
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
        player1.physicsProcess();
        player1.on_ground = false;
        for (int i = 0; i < Game.getStaticsCount(); i++) {
        collide = Game.getStatic(i)->blocks(&(player1.sword_player));
            if (collide[2] && collide[1] <= -1) {
                player1.on_ground = true;
            };
        };
        /*
        // Wolf:
        wolf.p.x += 10*(wolf_keys[1]-wolf_keys[0]); // Speed multiplier was 10
        wolf.tick();
        // Human:
        human.p.x += 10*(human_keys[1]-human_keys[0]); // Speed multiplier was 10
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
        
        */

        //Draw the Screen
        window.clear(Color(42,42,42,255)); // Dark gray.
        window.draw(Game.getStatic(0)->getRectangle()); //Draw the Floor
        //window.draw(Game.getStatic(1)->getRectangle());
        //window.draw(Game.getStatic(2)->getRectangle());
        //window.draw(Game.getStatic(3)->getRectangle());
        window.draw(player1.sword_player.getSprite());
        window.draw(player1.sword_player.getHitbox()->getRectangle());
        //window.draw(human.getSprite());
        //window.draw(human.getHitbox()->getRectangle());

        // Text Test
        //window.draw(test_text.txt);

        window.display();
    }


    return 0;
}



void handle_controls(Event event){
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
            player1.keys[0] = 1;
            if (player1.sword_player.isFlippedH()) {
                player1.sword_player.flipH();
            }
            break;
            case Keyboard::D:
            player1.keys[1] = 1;
            if (!player1.sword_player.isFlippedH()) {
                player1.sword_player.flipH();
            }
            break;
            case Keyboard::W:
            if (player1.keys[2] == 0 && player1.on_ground){
                player1.keys[2] = 1;
                player1.sword_player.v.y = -20;
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
            player1.keys[0] = 0;
            break;
            case Keyboard::D:
            player1.keys[1] = 0;
            break;
            case Keyboard::W:
            player1.keys[2] = 0;
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