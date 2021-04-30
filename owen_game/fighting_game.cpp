#include "../fight_engine.cpp"

// fighting game
// by owen

// globals



int main(int argc, char* argv[]) {
    printf("now playing: fighting game\n");

    // declare variables and objects

    Event event;
    int p1keys[5] = {0,0,0,0,0};
    string state = "game";
    TextBox title(0, -8, "assets/pixer.ttf", "fighting game demo");
    title.setColor(0, 0, 0);

    animation_data p1_idle;
    animation_data p1_dead;

    sprite_data* idle = make_sprite("assets/p1_idle.png");
    idle->imageSprite.setScale(2,2);
    idle->flippedSprite.setScale(2,2);

    p1_dead.addAnimationData(make_sprite("assets/p1_dead.png"));
    p1_idle.addAnimationData(idle);

    p1_idle.setMaxFrameTick(1);
    p1_dead.setMaxFrameTick(1);

    KinematicBody2D p1(100,100,128,128);
    p1.setSprite(p1_idle.getCurrentFrame());
    p1.initHitbox();
    p1.getHitbox()->initRectangle();
    p1.a.y = 1;

    KinematicBody2D ground(0,600,800,200);
    ground.initHitbox();
    ground.initRectangle();

    bool p1_grounded = false;
    int* p1_ground_test;

    // create window

    create_window("fighting game", 800, 800);
    window.setKeyRepeatEnabled(false);

    // main game loop

    Framerate ticker(60);
    while (window.isOpen()) {
        ticker.next_frame();
        
        if (state == "game") {

            // update game

            // player controls

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
                                p1keys[0] = 1;
                                break;
                            case Keyboard::D:
                                p1keys[1] = 1;
                                break;
                            case Keyboard::W:
                                if (p1keys[2] == 0 && p1_grounded){
                                    p1keys[2] = 1;
                                    p1.v.y = -20;
                                }
                                break;
                            case Keyboard::BackSpace:
                                p1.setSprite(p1_dead.getCurrentFrame());
                                break;
                            case Keyboard::LAlt:
                                cout << p1keys[2] << endl;
                            default:
                                break;
                        }
                        break;
                    case Event::KeyReleased:
                        switch (event.key.code) {
                            case Keyboard::A:
                                p1keys[0] = 0;
                                break;
                            case Keyboard::D:
                                p1keys[1] = 0;
                                break;
                            case Keyboard::W:
                                p1keys[2] = 0;
                                break;
                            default:
                                break;
                        }
                        break;
                    default:
                        break;
                }
            }

            // handle inputs

            p1.p.x += 10*(p1keys[1]-p1keys[0]);

            // physics

            p1.tick();

            p1_grounded = false;
            p1_ground_test = ground.blocks(&p1);
            if (p1_ground_test[2] && p1_ground_test[1] <= -1) {
                p1_grounded = true;
            }
            

            /*
            if (p1_grounded) {
                p1.a.y = 0;
            } else {
                p1.a.y = 1;
            } */

            // draw game

            window.clear(Color(255,255,255,255));

            window.draw(p1.getSprite());
            window.draw(ground.getRectangle());
            window.draw(title.text);

            window.display();

        }
    }
}