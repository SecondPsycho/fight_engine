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
    int t = 0;
    int risespeed = 2;
    int fps = 30;

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
                                ticker = Framerate(fps);
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
            for (int i = 0; i < Game.getWatersCount(); i++) {
                window.draw(Game.getWater(i)->getRectangle());
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
            
            if (P1->body->pos().y < 400 || P2->body->pos().y < 400) {
                //t = 0;
                for (int i = 0; i < Game.getStaticsCount(); i++) {
                    Game.getStatic(i)->p.y += risespeed;
                }
                for (int i = 0; i < Game.getFlowersCount(); i++) {
                    Game.getFlower(i)->p.y += risespeed;
                }
                worldshifty += risespeed;
                P1->body->p.y += risespeed;
                P2->body->p.y += risespeed;
            }
            t += 1;
            if (t >= fps) {
                t = 0;
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
                                P1->faceRight(false);
                                break;
                            case Keyboard::D:
                                P1->keys[1] = true;
                                P1->faceRight(true);
                                break;
                            case Keyboard::W:
                                P1->jump(Game.getFlowers(), Game.getFlowersCount(), P2);
                                break;
                            case Keyboard::S:
                                P1->block();
                                break;
                            case Keyboard::E:
                                P1->keys[4] = true;
                                if (P1->attackCooldown == 0 && P1->punch(P2)) {
                                    P2->takeHit(Vector2D(0,0), P1->body->isFlippedH());
                                }
                                break;
                            case Keyboard::H:
                            case Keyboard::Left:
                                P2->keys[0] = true;
                                P2->faceRight(false);
                                break;
                            case Keyboard::K:
                            case Keyboard::Right:
                                P2->keys[1] = true;
                                P2->faceRight(true);
                                break;
                            case Keyboard::U:
                            case Keyboard::Up:
                                P2->jump(Game.getFlowers(), Game.getFlowersCount(), P1);
                                break;
                            case Keyboard::J:
                            case Keyboard::Down:
                                P2->block();
                                break;
                            case Keyboard::I:
                                P2->keys[4] = true;
                                if (P2->attackCooldown == 0 && P2->punch(P1)) {
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
                            case Keyboard::S:
                                P1->unblock();
                                break;
                            case Keyboard::E:
                                P1->keys[4] = false;
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
                            case Keyboard::J:
                            case Keyboard::Down:
                                P2->unblock();
                                break;
                            case Keyboard::I:
                                P2->keys[4] = false;
                                break;
                            default:
                                break;
                        }
                        break;
                    default:
                        break;
                }
            }

            Game.movePlatforms(worldshifty);

            //Apply Physics
            Game.runPhysics(t, fps);
            P1->body->dampen(f);
            P2->body->dampen(f);

            P1->animate();
            P2->animate();

            //Apply animation
            //*
            
            /*
            if (!P2->on_ground) {
                P2->body->setSprite(P2->leap.getCurrentFrame()); // Do leap animation
            }
            else 
            if (P2->keys[1]^P2->keys[0]) {
                P2->body->setSprite(P2->walk.frameTick()); // Do walking animations
            }
            else {
                P2->body->setSprite(P2->idle.frameTick()); // Do idle animation
            }//*/

            //Process death
            if (P1->body->posY() >= Game.getWater(1)->pos().y || P2->body->posY() >= Game.getWater(1)->pos().y) {
                Game.ON = false;
                my_music.setPlayingOffset(78);
                seconds = 78;
                cout << "Game Over" << endl;
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
            for (int i = 0; i < Game.getWatersCount(); i++) {
                window.draw(Game.getWater(i)->getRectangle());
            }

            //*
            window.draw(P1->punchbox.getRectangle());
            window.draw(P2->punchbox.getRectangle());
            //*/
            window.draw(P1->body->getHitbox()->getRectangle());
            window.draw(P2->body->getHitbox()->getRectangle());

            window.display();
        }
        while (!Game.ON && window.isOpen()) {
            ticker.next_frame();
            t += 1;
            if (t >= fps) {
                t = 0;
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
            for (int i = 0; i < Game.getWatersCount(); i++) {
                window.draw(Game.getWater(i)->getRectangle());
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
