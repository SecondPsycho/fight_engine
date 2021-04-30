#include "../fight_engine.cpp"
#include "tree_classes.cpp"


int main(int argc, char* argv[]){
    create_window("Up the Tree", 1600, 1200);
    window.setKeyRepeatEnabled(false);
    
    //Create an Animation
    animation_data idle;
    animation_data walk;
    animation_data leap;

    Player* P1 = new Player(50,80);
    Player* P2 = new Player(450,80);

    //Create Sprite Data
    NewGame Game(P1, P2, 6);
    Game.buildLevel();
    Game.ON = false;

    //Main Menu Stuff
    P1->body->flipH();
    P1->body->setSprite(P1->idle.getCurrentFrame());
    P2->body->setSprite(P2->idle.getCurrentFrame());
    TextBox title(0, 0, "./text/good-time-rg.ttf", "Up the Tree");
    
    Song my_music("sounds/Restoring the Light, Facing the Dark.wav");
    //my_music.setPitch(1.2f);
    //my_music.setVolume(10.0f);
    my_music.play();
    SFX pew_sound("sounds/pew.wav");
    pew_sound.setVolume(100.0f);
            
    Event event;
    //Left, Right, Jump, Attack
    bool P1keys[5] = {false,false,false,false};
    bool P2keys[5] = {false,false,false,false};
    Vector2D f(1,0);
    int worldshifty = 0;

    int t = 0;
    int falldelay = 5;
    Framerate ticker(60);
    while (window.isOpen()) {
        while (!Game.ON && window.isOpen()) {
            while(window.pollEvent(event)) {
                switch (event.type) {
                    case Event::Closed:
                        window.close();
                        break;
                    case Event::KeyPressed:
                        switch(event.key.code) {
                            case Keyboard::Escape:
                                window.close();
                                break;
                            default:
                                break;
                        }
                    default:
                        break;
                }
            }
            window.clear(Color(42,42,42,255)); // Dark gray.

            window.draw(P1->body->getSprite());
            window.draw(P2->body->getSprite());

            for (int i = 0; i < Game.getStaticsCount(); i++) {
                window.draw(Game.getStatic(i)->getRectangle());
            }

            window.display();
        }
        while (Game.ON && window.isOpen()) {
            ticker.next_frame();
            t += 1;
            if (t >= falldelay) {
                t = 0;
                for (int i = 0; i < Game.getStaticsCount(); i++) {
                    Game.getStatic(i)->p.y += 1;
                }
                worldshifty += 1;
            }
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
                                P1keys[0] = true;
                                if (P1->body->isFlippedH()) {
                                    P1->body->flipH();
                                }
                                break;
                            case Keyboard::D:
                                P1keys[1] = true;
                                if (!P1->body->isFlippedH()) {
                                    P1->body->flipH();
                                }
                                break;
                            case Keyboard::W:
                                if (!P1keys[2]) {
                                    if (P1->on_ground) {
                                        P1->body->v.y = -15;
                                        P1keys[2] = true;
                                    } else if (P1->double_jump) {
                                        P1->body->v.y = -15;
                                        P1keys[2] = true;
                                        P1->double_jump = false;
                                    }
                                }
                                break;
                            case Keyboard::E:
                                P1keys[3] = true;
                                if (P1->punch(P2)) {
                                    P2->takeHit(Vector2D(0,0), P1->body->isFlippedH());
                                }
                                break;
                            case Keyboard::H:
                            case Keyboard::Left:
                                P2keys[0] = true;
                                if (P2->body->isFlippedH()) {
                                    P2->body->flipH();
                                }
                                break;
                            case Keyboard::K:
                            case Keyboard::Right:
                                P2keys[1] = true;
                                if (!P2->body->isFlippedH()) {
                                    P2->body->flipH();
                                }
                                break;
                            case Keyboard::U:
                            case Keyboard::Up:
                                if (!P2keys[2]) {
                                    if (P2->on_ground) {
                                        P2->body->v.y = -15;
                                        P2keys[2] = true;
                                    } else if (P2->double_jump) {
                                        P2->body->v.y = -15;
                                        P2keys[2] = true;
                                        P2->double_jump = false;
                                    }
                                }
                                break;
                            case Keyboard::I:
                                P2keys[3] = true;
                                if (P2->punch(P1)) {
                                    P1->takeHit(Vector2D(0,0), P2->body->isFlippedH());
                                }
                                break;
                            default:
                                break;
                        }
                        break;
                    case Event::KeyReleased:
                        switch (event.key.code) {
                            case Keyboard::A:
                                P1keys[0] = false;
                                break;
                            case Keyboard::D:
                                P1keys[1] = false;
                                break;
                            case Keyboard::W:
                                P1keys[2] = false;
                                break;
                            case Keyboard::E:
                                P1keys[3] = false;
                                break;
                            case Keyboard::H:
                            case Keyboard::Left:
                                P2keys[0] = false;
                                break;
                            case Keyboard::K:
                            case Keyboard::Right:
                                P2keys[1] = false;
                                break;
                            case Keyboard::U:
                            case Keyboard::Up:
                                P2keys[2] = false;
                                break;
                            case Keyboard::I:
                                P2keys[3] = false;
                                break;
                            default:
                                break;
                        }
                        break;
                    default:
                        break;
                }
            }

            if (Game.getStatic(2)->posX() <= 0) {
                Game.getStatic(2)->v.x = 5;
            } else if (Game.getStatic(2)->posX() >= 400) {
                Game.getStatic(2)->v.x = -5;
            }
            if (Game.getStatic(3)->posY() <= worldshifty+200) {
                Game.getStatic(3)->v.y = 5;
            } else if (Game.getStatic(3)->posY() >= worldshifty+500) {
                Game.getStatic(3)->v.y = -5;
            }

            //Apply Physics
            P1->body->p.x += (P1keys[1]-P1keys[0])*10;
            P2->body->p.x += (P2keys[1]-P2keys[0])*10;
            P1->body->p.y -= (P1keys[2])*3;
            P2->body->p.y -= (P2keys[2])*3;
            Game.runPhysics();
            P1->body->dampen(f);
            P2->body->dampen(f);

            //cout << P1->body->posX() << ' ' << P1->body->posY() << "     " << P2->body->posX() << ' ' << P2->body->posY() << endl;
            //Apply animation
            if (!P1->on_ground) {
                P1->body->setSprite(P1->leap.getCurrentFrame()); // Do leap animation
            }
            else if (P1keys[1]^P1keys[0]) {
                P1->body->setSprite(P1->walk.frameTick()); // Do walking animations
            }
            else {
                P1->body->setSprite(P1->idle.getCurrentFrame()); // Do idle animation
            }
            
            if (!P2->on_ground) {
                P2->body->setSprite(P2->leap.getCurrentFrame()); // Do leap animation
            }
            else if (P2keys[1]^P2keys[0]) {
                P2->body->setSprite(P2->walk.frameTick()); // Do walking animations
            }
            else {
                P2->body->setSprite(P2->idle.getCurrentFrame()); // Do idle animation
            }
            

            //Draw the Screen
            window.clear(Color(42,42,42,255)); // Dark gray.

            window.draw(P1->body->getSprite());
            window.draw(P2->body->getSprite());

            for (int i = 0; i < Game.getStaticsCount(); i++) {
                window.draw(Game.getStatic(i)->getRectangle());
            }
            //window.draw(wolf.getHitbox()->getRectangle());

            window.draw(P1->punchbox.getRectangle());
            window.draw(P2->punchbox.getRectangle());

            window.display();
        }
    }
    return 0;
}
