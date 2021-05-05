#include "../fight_engine.cpp"

// fighting game
// by owen

// globals

int main(int argc, char* argv[]) {
    printf("now playing: fighting game\n");

    // declare variables and objects

    Event event;
    int p1keys[5] = {0,0,0,0,0};
    int p2keys[5] = {0,0,0,0,0};
    string state = "game";
    TextBox title(0, -8, "assets/pixer.ttf", "fighting game demo");
    title.setColor(0, 0, 0);
    int p1_attackTimer = -1;
    int p2_attackTimer = -1;
    Vector2D v(0, 0);
    int p1hp = 5;
    int p2hp = 5;

    animation_data p1_idle;
    animation_data p1_dead;
    animation_data p1_attack;

    animation_data p2_idle;
    animation_data p2_dead;
    animation_data p2_attack;


    p1_dead.addAnimationData(make_sprite("assets/p1_dead.png"));

    p1_idle.addAnimationData(make_sprite("assets/p1_idle0.png"));
    p1_idle.addAnimationData(make_sprite("assets/p1_idle1.png"));

    p1_attack.addAnimationData(make_sprite("assets/p1_attack2.png"));
    p1_attack.addAnimationData(make_sprite("assets/p1_attack0.png"));
    p1_attack.addAnimationData(make_sprite("assets/p1_attack1.png"));
    p1_attack.addAnimationData(make_sprite("assets/p1_attack2.png"));
    p1_attack.addAnimationData(make_sprite("assets/p1_attack3.png"));
    p1_attack.addAnimationData(make_sprite("assets/p1_attack4.png"));

    p1_idle.setMaxFrameTick(25);
    p1_dead.setMaxFrameTick(1);
    p1_attack.setMaxFrameTick(5);

    p2_dead.addAnimationData(make_sprite("assets/p1_dead.png"));

    p2_idle.addAnimationData(make_sprite("assets/p1_idle0.png"));
    p2_idle.addAnimationData(make_sprite("assets/p1_idle1.png"));

    p2_attack.addAnimationData(make_sprite("assets/p1_attack2.png"));
    p2_attack.addAnimationData(make_sprite("assets/p1_attack0.png"));
    p2_attack.addAnimationData(make_sprite("assets/p1_attack1.png"));
    p2_attack.addAnimationData(make_sprite("assets/p1_attack2.png"));
    p2_attack.addAnimationData(make_sprite("assets/p1_attack3.png"));
    p2_attack.addAnimationData(make_sprite("assets/p1_attack4.png"));

    p2_idle.setMaxFrameTick(25);
    p2_dead.setMaxFrameTick(1);
    p2_attack.setMaxFrameTick(5);

    KinematicBody2D p1(100,100,100,512);
    p1.setSprite(p1_idle.getCurrentFrame());
    p1.initHitbox();
    p1.getHitbox()->initRectangle();
    p1.a.y = 1;

    KinematicBody2D p2(450,100,100,512);
    p2.setSprite(p2_idle.getCurrentFrame());
    p2.initHitbox();
    p2.getHitbox()->initRectangle();
    p2.a.y = 1;

    p2.flipH();

    KinematicBody2D ground(0,600,800,200);
    ground.initHitbox();
    ground.initRectangle();

    KinematicBody2D lwall(0,0,5,600);
    lwall.initHitbox();
    lwall.initRectangle();

    KinematicBody2D rwall(680,0,5,600);
    rwall.initHitbox();
    rwall.initRectangle();

    bool p1_grounded = false;
    int* p1_ground_test;

    bool p2_grounded = false;
    int* p2_ground_test;

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
                            case Keyboard::Left:
                                p2keys[0] = 1;
                                break;
                            case Keyboard::Right:
                                p2keys[1] = 1;
                                break;
                            case Keyboard::W:
                                if (p1keys[2] == 0 && p1_grounded){
                                    p1keys[2] = 1;
                                    p1.v.y = -8;
                                }
                                break;
                            case Keyboard::Up:
                                if (p2keys[2] == 0 && p2_grounded){
                                    p2keys[2] = 1;
                                    p2.v.y = -8;
                                }
                                break;
                            case Keyboard::LShift:
                                p1keys[3] = 1;
                                if (p1_attackTimer == -1) {
                                    p1_attackTimer = 0;
                                }
                                break;
                            case Keyboard::Numpad0:
                                p2keys[3] = 1;
                                if (p2_attackTimer == -1) {
                                    p2_attackTimer = 0;
                                }
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
                            case Keyboard::LShift:
                                p1keys[3] = 0;
                                break;
                            case Keyboard::Left:
                                p2keys[0] = 0;
                                break;
                            case Keyboard::Right:
                                p2keys[1] = 0;
                                break;
                            case Keyboard::Up:
                                p2keys[2] = 0;
                                break;
                            case Keyboard::Numpad0:
                                p2keys[3] = 0;
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

            v.x = 2 * ((-1 * p1keys[0]) + p1keys[1]);
            p1.move(v);

            v.x = 2 * ((-1 * p2keys[0]) + p2keys[1]);
            p2.move(v);

            // physics
           
            p1.tick();
            p2.tick();

            p1.blocks(&p2);

            p1_grounded = false;
            p1_ground_test = ground.blocks(&p1);
            lwall.blocks(&p1);
            if (p1_ground_test[2] && p1_ground_test[1] <= -1) {
                p1_grounded = true;
            }

            if (p1_attackTimer >= 0) {
                p1_attackTimer++;
                if (p1_attackTimer >= 50) {
                    p1_attackTimer = -1;
                    p1_attack.cur_frame = 0;
                }
            }

            p2_grounded = false;
            p2_ground_test = ground.blocks(&p2);
            rwall.blocks(&p2);
            if (p2_ground_test[2] && p2_ground_test[1] <= -1) {
                p2_grounded = true;
            }

            if (p2_attackTimer >= 0) {
                p2_attackTimer++;
                if (p2_attackTimer >= 50) {
                    p2_attackTimer = -1;
                    p2_attack.cur_frame = 0;
                }
            }
            
            // game logic

            if (p1hp <= 0) {
                state = "gg2";
                p1.setSprite(p1_dead.getCurrentFrame());
            }

            if (p2hp <= 0) {
                state = "gg1";
                p2.setSprite(p2_dead.getCurrentFrame());
            }

            // animation
            
            if (p1_attackTimer >= 0) {
                if (p1_attack.cur_frame <= 4) {
                    p1.setSprite(p1_attack.frameTick());
                } else {
                    p1.setSprite(p1_attack.getCurrentFrame());
                }
            } else {
                p1.setSprite(p1_idle.frameTick());
            }

            if (p2_attackTimer >= 0) {
                if (p2_attack.cur_frame <= 4) {
                    p2.setSprite(p2_attack.frameTick());
                } else {
                    p2.setSprite(p2_attack.getCurrentFrame());
                }
            } else {
                p2.setSprite(p2_idle.frameTick());
            }

            // draw game

            window.clear(Color(255,255,255,255));

            window.draw(p1.getSprite());
            window.draw(p2.getSprite());
            window.draw(ground.getRectangle());
            window.draw(title.text);

            window.display();

        }
    }
}