#include "../fight_engine.cpp"
#include "tree_classes.cpp"


int main(int argc, char* argv[]){
    create_window("Up the Tree", 1600, 1200);
    window.setKeyRepeatEnabled(false);
    
    //Create an Animation
    animation_data idle;
    animation_data walk;
    animation_data leap;

    Player* P1 = new Player(250,450);
    Player* P2 = new Player(1200,750);

    //Create Sprite Data
    NewGame Game(P1, P2, 6);
    Game.buildLevel();
    Game.ON = false;

    //Main Menu Stuff
    P2->body->flipH();
    P1->body->setSprite(P1->idle.getCurrentFrame());
    P2->body->setSprite(P2->idle.getCurrentFrame());

    //Title Screen
    TextBox title(800, 200, "./text/good-times-rg.ttf", "Up the Tree", true);
    title.setColor(255,255,255);
    title.setCharacterSize(200);
    TextBox creator(800,400, "./text/good-times-rg.ttf", "Made by Cordell King", true);
    creator.setColor(255,255,255);
    creator.setCharacterSize(25);
    TextBox artist(800,425, "./text/good-times-rg.ttf", "Art by Diego Arroyo", true);
    artist.setColor(255,255,255);
    artist.setCharacterSize(25);
    TextBox music(800,450, "./text/good-times-rg.ttf", "Music from Ori and the Blind Forest", true);
    music.setColor(255,255,255);
    music.setCharacterSize(25);

    TextBox P1controls(800,700, "./text/good-times-rg.ttf", "P1: WASD & E", true);
    P1controls.setColor(255,255,255);
    P1controls.setCharacterSize(100);
    TextBox P2controls(800,800, "./text/good-times-rg.ttf", "P2: UHJK & I", true);
    P2controls.setColor(255,255,255);
    P2controls.setCharacterSize(100);
    TextBox controls(800,900, "./text/good-times-rg.ttf", "SPACE to Start", true);
    controls.setColor(255,255,255);
    controls.setCharacterSize(150);
    
    Song my_music("sounds/Restoring the Light, Facing the Dark.wav");
    //my_music.setPitch(1.2f);
    //my_music.setVolume(10.0f);
    SFX pew_sound("sounds/pew.wav");
    //pew_sound.setVolume(100.0f);
            
    Event event;
    //Left, Right, Jump, Attack
    Vector2D f(1,0);
    int worldshifty = 0;
    int seconds = 0;
    int st = 0;
    int fps = 60;

    int t = 0;
    int falldelay = 5;
    Framerate ticker(fps);
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
                            case Keyboard::Space:
                                Game.ON = true;
                                ticker = Framerate(60);
                                seconds = 0;
                                my_music.setPlayingOffset(0); //117
                                my_music.play();
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
            for (int i = 0; i < Game.getFlowersCount(); i++) {
                window.draw(Game.getFlower(i)->getRectangle());
            }
            
            window.draw(title.text);
            window.draw(creator.text);
            window.draw(artist.text);
            window.draw(music.text);
            window.draw(P1controls.text);
            window.draw(P2controls.text);
            window.draw(controls.text);
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
                for (int i = 0; i < Game.getFlowersCount(); i++) {
                    Game.getFlower(i)->p.y += 1;
                }
                worldshifty += 1;
            }
            st += 1;
            if (st >= fps) {
                st = 0;
                seconds += 1;
                if (seconds >= 77) {
                    my_music.setPlayingOffset(29);
                    seconds = 29;
                }
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
                                P1->keys[0] = true;
                                if (!P1->body->isFlippedH()) {
                                    P1->body->flipH();
                                }
                                break;
                            case Keyboard::D:
                                P1->keys[1] = true;
                                if (P1->body->isFlippedH()) {
                                    P1->body->flipH();
                                }
                                break;
                            case Keyboard::W:
                                P1->jump(Game.getFlowers(), Game.getFlowersCount(), P2);
                                break;
                            case Keyboard::E:
                                P1->keys[3] = true;
                                if (P1->punch(P2)) {
                                    P2->takeHit(Vector2D(0,0), P1->body->isFlippedH());
                                }
                                break;
                            case Keyboard::H:
                            case Keyboard::Left:
                                P2->keys[0] = true;
                                if (!P2->body->isFlippedH()) {
                                    P2->body->flipH();
                                }
                                break;
                            case Keyboard::K:
                            case Keyboard::Right:
                                P2->keys[1] = true;
                                if (P2->body->isFlippedH()) {
                                    P2->body->flipH();
                                }
                                break;
                            case Keyboard::U:
                            case Keyboard::Up:
                                P2->jump(Game.getFlowers(), Game.getFlowersCount(), P1);
                                break;
                            case Keyboard::I:
                                P2->keys[3] = true;
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
                                P1->keys[0] = false;
                                break;
                            case Keyboard::D:
                                P1->keys[1] = false;
                                break;
                            case Keyboard::W:
                                P1->keys[2] = false;
                                break;
                            case Keyboard::E:
                                P1->keys[3] = false;
                                break;
                            case Keyboard::H:
                            case Keyboard::Left:
                                P2->keys[0] = false;
                                break;
                            case Keyboard::K:
                            case Keyboard::Right:
                                P2->keys[1] = false;
                                break;
                            case Keyboard::U:
                            case Keyboard::Up:
                                P2->keys[2] = false;
                                break;
                            case Keyboard::I:
                                P2->keys[3] = false;
                                break;
                            default:
                                break;
                        }
                        break;
                    default:
                        break;
                }
            }

            if (Game.getStatic(2)->posX() <= 200) {
                Game.getStatic(2)->v.x = 5;
            } else if (Game.getStatic(2)->posX() >= 800) {
                Game.getStatic(2)->v.x = -5;
            }
            if (Game.getStatic(3)->posY() <= worldshifty+200) {
                Game.getStatic(3)->v.y = 5;
            } else if (Game.getStatic(3)->posY() >= worldshifty+700) {
                Game.getStatic(3)->v.y = -5;
            }

            //Apply Physics
            P1->body->p.x += (P1->keys[1]-P1->keys[0])*10;
            P2->body->p.x += (P2->keys[1]-P2->keys[0])*10;
            P1->body->p.y -= (P1->keys[2])*3;
            P2->body->p.y -= (P2->keys[2])*3;
            Game.runPhysics();
            P1->body->dampen(f);
            P2->body->dampen(f);

            //Apply animation
            /*
            if (!P1->on_ground) {
                P1->body->setSprite(P1->leap.getCurrentFrame()); // Do leap animation
            }
            else 
            //*/
            if (P1->keys[1]^P1->keys[0]) {
                P1->body->setSprite(P1->walk.frameTick()); // Do walking animations
            }
            else {
                P1->body->setSprite(P1->idle.frameTick()); // Do idle animation
            }
            /*
            if (!P2->on_ground) {
                P2->body->setSprite(P2->leap.getCurrentFrame()); // Do leap animation
            }
            else 
            //*/
            if (P2->keys[1]^P2->keys[0]) {
                P2->body->setSprite(P2->walk.frameTick()); // Do walking animations
            }
            else {
                P2->body->setSprite(P2->idle.frameTick()); // Do idle animation
            }

            //Process death
            if (P1->body->posY() >= 1300 || P2->body->posY() >= 1300) {
                Game.ON = false;
                my_music.setPlayingOffset(78);
                seconds = 78;
                cout << "Game Over";
            }
            

            //Draw the Screen
            window.clear(Color(42,42,42,255)); // Dark gray.

            window.draw(P1->body->getSprite());
            window.draw(P2->body->getSprite());

            for (int i = 0; i < Game.getStaticsCount(); i++) {
                window.draw(Game.getStatic(i)->getRectangle());
            }
            for (int i = 0; i < Game.getFlowersCount(); i++) {
                window.draw(Game.getFlower(i)->getRectangle());
            }

            window.draw(P1->punchbox.getRectangle());
            window.draw(P2->punchbox.getRectangle());

            window.display();
        }
        while (!Game.ON && window.isOpen()) {
            ticker.next_frame();
            st += 1;
            if (st >= fps) {
                st = 0;
                seconds += 1;
                if (seconds >= 100) {
                    my_music.stop();
                }
            }
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
                            case Keyboard::Space:
                                Game.ON = true;
                                ticker = Framerate(60);
                                //my_music.setPlayingOffset(seconds); //117
                                my_music.play();
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
            for (int i = 0; i < Game.getFlowersCount(); i++) {
                window.draw(Game.getFlower(i)->getRectangle());
            }
            
            window.draw(title.text);
            window.draw(creator.text);
            window.draw(artist.text);
            window.draw(music.text);
            /*
            window.draw(P1controls.text);
            window.draw(P2controls.text);
            window.draw(controls.text);
            //*/
            window.display();
        }
        break;
    }
    return 0;
}
