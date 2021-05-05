#include "../fight_engine.cpp"
#include "./NewGameClass.cpp"
#include "./SwordPlayerClass.cpp"
#include "./ScythePlayerClass.cpp"

void handle_game_controls(Event event);
void handle_title_controls(Event event);
void handle_death_controls(Event event);
void initializeGame();
void resetGame();
void playing_game();
void death_screen();
void title_screen();


#define TITLE_SCREEN 0
#define PLAYING_GAME 1
#define DEATH_SCREEN 2
#define QUIT_GAME 3



const int window_w = 1920;
const int window_h = 1080;


create_window("Dash And Slash", window_w, window_h);


// Globals:
swordPlayer player1;
scythePlayer player2;
NewGame Game(player1.sword_player, player2.scythe_player, 6);
Event event;
int gameState = TITLE_SCREEN;
SFX hitSound("./sounds/Hit_Hurt6.wav");
TextBox titleText((window_w / 2), (window_h / 2) - 300, "./font/good-times-rg.ttf", "Dash And Slash", true);
TextBox titleControls((window_w / 2), (window_h / 2) - 150, "./font/good-times-rg.ttf", "Press Space to Play Press Q to Quit!!!", true);
TextBox titleP1Controls(150, (window_h / 2) + 50, "./font/good-times-rg.ttf", "Press A and D to move,\nW to jump,\nand G to attack.", true);
TextBox titleP2Controls((window_w - 190), (window_h / 2) + 50, "./font/good-times-rg.ttf", "Press Left and Right to move,\nUp to jump,\nand 2 to attack.", true);
TextBox deathScreenText((window_w / 2), (window_h / 2) - 300, "./font/good-times-rg.ttf", "Player X Wins!!!", true);
TextBox deathScreenControls((window_w / 2), (window_h / 2) - 150, "./font/good-times-rg.ttf", "Press Space to Return to Title Press Q to Quit!!!", true);


int main(){
    // Turn key repeat off:
    window.setKeyRepeatEnabled(false);
    initializeGame();
    Song song("./music/The_Last_Encounter_Original_Version (online-audio-converter.com).wav");
    song.setVolume(75.0f);
    song.play();
    titleText.setCharacterSize(75);
    titleControls.setCharacterSize(30);
    titleP1Controls.setCharacterSize(15);
    titleP2Controls.setCharacterSize(15);
    deathScreenText.setCharacterSize(75);
    deathScreenControls.setCharacterSize(30);

    while(gameState != QUIT_GAME && window.isOpen()){
        if (gameState == TITLE_SCREEN){
            resetGame();
            title_screen();
        }
        if (gameState == PLAYING_GAME) {
            playing_game();
        }
        if (gameState == DEATH_SCREEN){
            death_screen();
        }
    }

    return 0;
}


void initializeGame(){
    // Initialize Player 1:
    player1.addAllAnimations();
    player1.initialize();
    // Initialize Player 2:
    player2.addAllAnimations();
    player2.initialize();

    // Initialize the floor:
    Game.addStatic(KinematicBody2D(0,(window_h - 200),window_w,200));
    Game.getStatic(0)->initRectangle();
    Game.getStatic(0)->initHitbox();

    // Make some platforms:
    Game.addStatic(KinematicBody2D(0, (window_h - 400), 300, 10));
    Game.getStatic(1)->initRectangle();
    Game.getStatic(1)->initHitbox();

    Game.addStatic(KinematicBody2D((window_w - 300), (window_h - 400), 300, 10));
    Game.getStatic(2)->initRectangle();
    Game.getStatic(2)->initHitbox();

    Game.addStatic(KinematicBody2D(((window_w/2)-400), (window_h - 550), 800, 15));
    Game.getStatic(3)->initRectangle();
    Game.getStatic(3)->initHitbox();

}


void resetGame(){
    player1.reset();
    player2.reset();
}


void playing_game(){
    // Framerate Control -Cordell King
    Framerate ticker(60);
    while (gameState == PLAYING_GAME && window.isOpen()) {
        ticker.next_frame();
        
        // Do Controls:
        while(window.pollEvent(event)){
            handle_game_controls(event);
        }

        
        // Apply Physics:
        player1.physicsProcess();
        player1.collision(Game);
        player2.physicsProcess();
        player2.collision(Game);



        // Apply Animations:
        player1.animate();
        player2.animate();


        // Apply Timers:
        player1.timers();
        player2.timers();


        if (player1.attack_timer > 0 && player2.attack_timer > 0){
            
        }
        else if (player1.sword_player.collides(&(player2.scythe_player)) && player1.attack_timer > 0 && player2.invicibility_timer == 0){
            player2.updateHealthBar();
            player2.onHit();
            hitSound.play();
        }
        else if (player2.scythe_player.collides(&(player1.sword_player)) && player2.attack_timer > 0 && player1.invicibility_timer == 0){
            player1.updateHealthBar();
            player1.onHit();
            hitSound.play();
        }

        if (player1.healthBar == 0){
            player2.endGame();
            player1.dead = true;
            player1.endGame();
            deathScreenText.setText("Player 2 Wins!!!");
            gameState = DEATH_SCREEN;
        }
        else if (player2.healthBar == 0){
            player1.endGame();
            player2.dead = true;
            player2.endGame();
            deathScreenText.setText("Player 1 Wins!!!");
            gameState = DEATH_SCREEN;
        }


        //Draw to the Screen
        window.clear(Color(42,42,42,255)); // Dark gray.
        window.draw(Game.getStatic(0)->getRectangle()); //Draw the Floor
        // Draw Platforms:
        window.draw(Game.getStatic(1)->getRectangle());
        window.draw(Game.getStatic(2)->getRectangle());
        window.draw(Game.getStatic(3)->getRectangle()); 
        // Draw Player1:
        window.draw(player1.sword_player.getSprite());
        //window.draw(player1.sword_player.getHitbox()->getRectangle());// Only for debugging
        window.draw(player1.healthBody.getSprite());
        // Draw Player2:
        window.draw(player2.scythe_player.getSprite());
        //window.draw(player2.scythe_player.getHitbox()->getRectangle());// Only for debugging
        window.draw(player2.healthBody.getSprite());

        window.display();
    }
}


void title_screen(){
    // Framerate Control -Cordell King
    Framerate ticker(60);
    while (gameState == TITLE_SCREEN && window.isOpen()) {
        ticker.next_frame();
        
        // Do Controls:
        while(window.pollEvent(event)){
            handle_title_controls(event);
        }

        
        // Apply Physics:
        player1.physicsProcess();
        player1.collision(Game);
        player2.physicsProcess();
        player2.collision(Game);



        // Apply Animations:
        player1.animate();
        player2.animate();


        //Draw to the Screen
        window.clear(Color(42,42,42,255)); // Dark gray.
        window.draw(Game.getStatic(0)->getRectangle()); //Draw the Floor
        // Draw Platforms:
        window.draw(Game.getStatic(1)->getRectangle());
        window.draw(Game.getStatic(2)->getRectangle());
        window.draw(Game.getStatic(3)->getRectangle()); 
        // Draw Player1:
        window.draw(player1.sword_player.getSprite());
        //window.draw(player1.sword_player.getHitbox()->getRectangle());// Only for debugging
        window.draw(player1.healthBody.getSprite());
        // Draw Player2:
        window.draw(player2.scythe_player.getSprite());
        //window.draw(player2.scythe_player.getHitbox()->getRectangle());// Only for debugging
        window.draw(player2.healthBody.getSprite());
        // Draw Title Text:
        window.draw(titleText.text);
        window.draw(titleControls.text);
        window.draw(titleP1Controls.text);
        window.draw(titleP2Controls.text);

        window.display();
    }
}


void death_screen(){
    // Framerate Control -Cordell King
    Framerate ticker(60);
    while (gameState == DEATH_SCREEN && window.isOpen()) {
        ticker.next_frame();
        
        // Do Controls:
        while(window.pollEvent(event)){
            handle_death_controls(event);
        }

        
        // Apply Physics:
        player1.physicsProcess();
        player1.collision(Game);
        player2.physicsProcess();
        player2.collision(Game);



        // Apply Animations:
        player1.animate();
        player2.animate();


        //Draw to the Screen
        window.clear(Color(42,42,42,255)); // Dark gray.
        window.draw(Game.getStatic(0)->getRectangle()); //Draw the Floor
        // Draw Platforms:
        window.draw(Game.getStatic(1)->getRectangle());
        window.draw(Game.getStatic(2)->getRectangle());
        window.draw(Game.getStatic(3)->getRectangle()); 
        // Draw Player1:
        window.draw(player1.sword_player.getSprite());
        //window.draw(player1.sword_player.getHitbox()->getRectangle());// Only for debugging
        window.draw(player1.healthBody.getSprite());
        // Draw Player2:
        window.draw(player2.scythe_player.getSprite());
        //window.draw(player2.scythe_player.getHitbox()->getRectangle());// Only for debugging
        window.draw(player2.healthBody.getSprite());
        // Draw Winning Text:
        window.draw(deathScreenText.text);
        window.draw(deathScreenControls.text);

        window.display();
    }
}


void handle_game_controls(Event event){
    switch (event.type) {
        case Event::Closed:
        window.close();
        break;
        case Event::KeyPressed:
        switch (event.key.code) {
            case Keyboard::A:
            player1.keys[0] = 1;
            if (!player1.sword_player.isFlippedH()) {
                player1.sword_player.flipH();
            }
            break;
            case Keyboard::D:
            player1.keys[1] = 1;
            if (player1.sword_player.isFlippedH()) {
                player1.sword_player.flipH();
            }
            break;
            case Keyboard::W:
            if (player1.keys[2] == 0 && player1.on_ground){
                player1.keys[2] = 1;
                player1.sword_player.v.y = -22;
            }
            break;
            case Keyboard::G:
            if (player1.attack_timer == 0 && player1.dash_timer == 0){
                player1.startAttack();
            }
            break;
            case Keyboard::Left:
            player2.keys[0] = 1;
            if (!player2.scythe_player.isFlippedH()) {
                player2.scythe_player.flipH();
            }
            break;
            case Keyboard::Right:
            player2.keys[1] = 1;
            if (player2.scythe_player.isFlippedH()) {
                player2.scythe_player.flipH();
            }
            break;
            case Keyboard::Up:
            if (player2.keys[2] == 0 && player2.on_ground){
                player2.keys[2] = 1;
                player2.scythe_player.v.y = -22;
            }
            break;
            case Keyboard::Numpad2:
            if (player2.attack_timer == 0 && player2.dash_timer == 0){
                player2.startAttack();
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
            player2.keys[0] = 0;
            break;
            case Keyboard::Right:
            player2.keys[1] = 0;
            break;
            case Keyboard::Up:
            player2.keys[2] = 0;
            break;
            default:
            break;
        }
        break;
        default:
        break;
    }
}

void handle_title_controls(Event event){
    switch (event.type) {
        case Event::Closed:
        window.close();
        break;
        case Event::KeyPressed:
        switch (event.key.code) {
            case Keyboard::Space:
            gameState = PLAYING_GAME;
            break;
            case Keyboard::Q:
            gameState = QUIT_GAME;
            break;
            default:
            break;
        }
        break;
        default:
        break;
    }
}


void handle_death_controls(Event event){
    switch (event.type) {
        case Event::Closed:
        window.close();
        break;
        case Event::KeyPressed:
        switch (event.key.code) {
            case Keyboard::Space:
            gameState = TITLE_SCREEN;
            break;
            case Keyboard::Q:
            gameState = QUIT_GAME;
            break;
            default:
            break;
        }
        break;
        default:
        break;
    }
}