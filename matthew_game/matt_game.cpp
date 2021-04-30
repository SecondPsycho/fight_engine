#include "../fight_engine.cpp"
#include "./NewGameClass.cpp"
#include "./SwordPlayerClass.cpp"
#include "./ScythePlayerClass.cpp"

void handle_game_controls(Event event);
void handle_death_controls(Event event);
void initializeGame();
void resetGame();
void playing_game();
void death_screen();


create_window("Matthew's Game", 2560, 1440); // Fullscreen but at cost of sprites looking small and needing to make stage bigger with adjusting speed and jump vals.


// Globals:
swordPlayer player1;
scythePlayer player2;
NewGame Game(player1.sword_player, player2.scythe_player, 6);
Event event;
int gameState = 1;

int main(){
    // Turn key repeat off:
    window.setKeyRepeatEnabled(false);
    initializeGame();
    Song song("./music/The_Last_Encounter_Original_Version (online-audio-converter.com).wav");
    song.play();
    song.setLoop(true);

    while(gameState != 4 && window.isOpen()){
        if (gameState == 1) {
            resetGame();
            gameState = 2;
        }
        if (gameState == 2) {
            playing_game();
        }
        if (gameState == 3){
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
    Game.addStatic(KinematicBody2D(0,1140,2560,200));
    Game.getStatic(0)->initRectangle();
    Game.getStatic(0)->initHitbox();
}


void resetGame(){
    player1.reset();
    player2.reset();
}


void playing_game(){
    // Framerate Control -Cordell King
    Framerate ticker(60);
    while (gameState == 2 && window.isOpen()) {
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

        // Current Death Condition
        if (player1.sword_player.collides(&(player2.scythe_player))){
            player1.die();
            player2.die();
            gameState = 3;
        }


        //Draw to the Screen
        window.clear(Color(42,42,42,255)); // Dark gray.
        window.draw(Game.getStatic(0)->getRectangle()); //Draw the Floor
        window.draw(player1.sword_player.getSprite());
        window.draw(player1.sword_player.getHitbox()->getRectangle());
        window.draw(player2.scythe_player.getSprite());
        window.draw(player2.scythe_player.getHitbox()->getRectangle());

        window.display();
    }
}

void death_screen(){
    // Framerate Control -Cordell King
    Framerate ticker(60);
    while (gameState == 3 && window.isOpen()) {
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
        window.draw(player1.sword_player.getSprite());
        window.draw(player1.sword_player.getHitbox()->getRectangle());
        window.draw(player2.scythe_player.getSprite());
        window.draw(player2.scythe_player.getHitbox()->getRectangle());

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
            case Keyboard::Escape:
            window.close();
            break;
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
                player1.sword_player.v.y = -20;
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
                player2.scythe_player.v.y = -20;
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

void handle_death_controls(Event event){
    switch (event.type) {
        case Event::Closed:
        window.close();
        break;
        case Event::KeyPressed:
        switch (event.key.code) {
            case Keyboard::Escape:
            window.close();
            break;
            case Keyboard::R:
            gameState = 1;
            break;
            case Keyboard::Q:
            gameState = 4;
            break;
            default:
            break;
        }
        break;
        default:
        break;
    }
}