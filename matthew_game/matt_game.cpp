#include "../fight_engine.cpp"
#include "./NewGameClass.cpp"
#include "./SwordPlayerClass.cpp"
#include "./ScythePlayerClass.cpp"
#include <map>
#include <bits/stdc++.h>
#include <fstream>
#include <sstream>

void handle_game_controls(Event event);
void handle_title_controls(Event event);
void handle_death_controls(Event event);
void initializeGame();
void resetGame();
void playing_game();
void death_screen();
void title_screen();
void leader_board();
void handle_leader_board_controls(Event event, string &text, int &current_player_typing, string other_text);
void loadLeaderBoard(string file_path);
void saveLeaderBoard(string file_path);


bool cmp(pair<string, int>& a, pair<string, int>& b);
void sortMap(map<string, int>& M);


#define TITLE_SCREEN 0
#define PLAYING_GAME 1
#define DEATH_SCREEN 2
#define QUIT_GAME 3
#define LEADER_BOARD 4



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
TextBox titleControls((window_w / 2), (window_h / 2) - 150, "./font/good-times-rg.ttf", "Press Space to Play Press Q to Quit, Press L for Leaderboard!!!", true);
TextBox titleP1Controls(150, (window_h / 2) + 50, "./font/good-times-rg.ttf", "Press A and D to move,\nW to jump,\nand G to attack.", true);
TextBox titleP2Controls((window_w - 190), (window_h / 2) + 50, "./font/good-times-rg.ttf", "Press Left and Right to move,\nUp to jump,\nand 2 to attack.", true);
TextBox deathScreenText((window_w / 2), (window_h / 2) - 300, "./font/good-times-rg.ttf", "Player X Wins!!!", true);
TextBox deathScreenControls((window_w / 2), (window_h / 2) - 150, "./font/good-times-rg.ttf", "Press Space to Return to Title!!!", true);

// Leaderboard Stuff
TextBox leaderBoardText((window_w / 2), (window_h / 2) - 500, "./font/good-times-rg.ttf", "Leaderboard", true);
TextBox leaderBoardControls((window_w / 2), (window_h / 2) + 200, "./font/good-times-rg.ttf", "Press \"[\" to select player 1,\nPress \"]\" to select player 2,\nand Escape to go to title.", true);
TextBox p1PreText(150, (window_h / 2) - 200, "./font/good-times-rg.ttf", "Player 1 Name: ", true);
TextBox currentPlayer1Name(290, (window_h / 2) - 200, "./font/good-times-rg.ttf", "", false);
string p1StringName = "";
TextBox p2PreText(150, (window_h / 2) - 100, "./font/good-times-rg.ttf", "Player 2 Name: ", true);
TextBox currentPlayer2Name(290, (window_h / 2) - 100, "./font/good-times-rg.ttf", "", false);
string p2StringName = "";
int current_player_typing = 0;
string randStr = "";
TextBox leader1Box((window_w/2), (window_h / 2) - 300, "./font/good-times-rg.ttf", "1. ", true);
TextBox leader2Box((window_w/2), (window_h / 2) - 250, "./font/good-times-rg.ttf", "2. ", true);
TextBox leader3Box((window_w/2), (window_h / 2) - 200, "./font/good-times-rg.ttf", "3. ", true);
TextBox leader4Box((window_w/2), (window_h / 2) - 150, "./font/good-times-rg.ttf", "4. ", true);
TextBox leader5Box((window_w/2), (window_h / 2) - 100, "./font/good-times-rg.ttf", "5. ", true);


TextBox titleP1PreName(200, (window_h / 2) -400, "./font/good-times-rg.ttf", "Player 1 Name: ", true);
TextBox titleP2PreName((window_w - 500), (window_h / 2) - 400, "./font/good-times-rg.ttf", "Player 2 Name: ", true);
TextBox titleP1Name(340, (window_h / 2) -400, "./font/good-times-rg.ttf", "", false);
TextBox titleP2Name((window_w - 360), (window_h / 2) - 400, "./font/good-times-rg.ttf", "", false);


map<string, int> leaderBoardRanking;
map<string, int> leaderBoardWins;
map<string, int> leaderBoardLosses;


int main(int argc, char** argv){
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
    leaderBoardText.setCharacterSize(75);
    leaderBoardControls.setCharacterSize(30);
    p1PreText.setCharacterSize(25);
    currentPlayer1Name.setCharacterSize(25);
    p2PreText.setCharacterSize(25);
    currentPlayer2Name.setCharacterSize(25);
    titleP1PreName.setCharacterSize(25);
    titleP2PreName.setCharacterSize(25);
    titleP1Name.setCharacterSize(25);
    titleP2Name.setCharacterSize(25);
    leader1Box.setCharacterSize(25);
    leader2Box.setCharacterSize(25);
    leader3Box.setCharacterSize(25);
    leader4Box.setCharacterSize(25);
    leader5Box.setCharacterSize(25);


    loadLeaderBoard("./Leader-Board.txt");


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
        if (gameState == LEADER_BOARD){
            leader_board();
        }
    }

    saveLeaderBoard("./Leader-Board.txt");

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
            if (p1StringName != "" && p2StringName != ""){
                leaderBoardWins[p2StringName] += 1;
                leaderBoardLosses[p1StringName] += 1;
                deathScreenText.setText(p2StringName + " Wins!!!");

            }
            gameState = DEATH_SCREEN;
        }
        else if (player2.healthBar == 0){
            player1.endGame();
            player2.dead = true;
            player2.endGame();
            deathScreenText.setText("Player 1 Wins!!!");
            if (p1StringName != "" && p2StringName != ""){
                leaderBoardWins[p1StringName] += 1;
                leaderBoardLosses[p2StringName] += 1;
                deathScreenText.setText(p1StringName + " Wins!!!");
            }
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
        
        window.draw(titleP1PreName.text);
        window.draw(titleP2PreName.text);
        window.draw(titleP1Name.text);
        window.draw(titleP2Name.text);

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
            if ((p1StringName == "" && p2StringName == "") || (p1StringName.length() > 0 && p2StringName.length() > 0)){
                gameState = PLAYING_GAME;
            }
            break;
            case Keyboard::Q:
            gameState = QUIT_GAME;
            break;
            case Keyboard::L:
            gameState = LEADER_BOARD;
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
            default:
            break;
        }
        break;
        default:
        break;
    }
}


void leader_board(){
    
    for (auto& it : leaderBoardRanking) {
        if (leaderBoardWins[it.first] == 0 && leaderBoardLosses[it.first] == 0){
            leaderBoardRanking[it.first] = -10000;
        }
        else {
            leaderBoardRanking[it.first] = leaderBoardWins[it.first] - leaderBoardLosses[it.first];
        }
    }
    sortMap(leaderBoardRanking);

    // Framerate Control -Cordell King
    Framerate ticker(60);
    while (gameState == LEADER_BOARD && window.isOpen()) {
        ticker.next_frame();
        
        // Do Controls:
        while(window.pollEvent(event)){
            if (current_player_typing == 0){
                randStr = "";
                handle_leader_board_controls(event, randStr, current_player_typing, randStr);
            }
            else if (current_player_typing == 1){
                handle_leader_board_controls(event, p1StringName, current_player_typing, p2StringName);
            }
            else if (current_player_typing == 2){
                handle_leader_board_controls(event, p2StringName, current_player_typing, p1StringName);
            }

        }

        currentPlayer1Name.setText(p1StringName);
        currentPlayer2Name.setText(p2StringName);

        //Draw to the Screen
        window.clear(Color(42,42,42,255)); // Dark gray.

        // Draw Leaderboard Text:
        window.draw(leaderBoardText.text);
        window.draw(leaderBoardControls.text);
        window.draw(p1PreText.text);
        window.draw(currentPlayer1Name.text);
        window.draw(p2PreText.text);
        window.draw(currentPlayer2Name.text);
        window.draw(leader1Box.text);
        window.draw(leader2Box.text);
        window.draw(leader3Box.text);
        window.draw(leader4Box.text);
        window.draw(leader5Box.text);

        window.display();
    }
    
    titleP1Name.setText(p1StringName);
    titleP2Name.setText(p2StringName);
    if (p1StringName != "" && p2StringName != ""){
        if (leaderBoardRanking[p1StringName] == 0){
            leaderBoardRanking[p1StringName] = 0;
            leaderBoardWins[p1StringName] = 0;
            leaderBoardLosses[p1StringName] = 0;
        }
        if (leaderBoardRanking[p2StringName] == 0){
            leaderBoardRanking[p2StringName] = 0;
            leaderBoardWins[p2StringName] = 0;
            leaderBoardLosses[p2StringName] = 0;
        }
    }

}

void handle_leader_board_controls(Event event, string &text, int &current_player_typing, string other_text){
    string new_char = "";
    switch (event.type) {
        case Event::Closed:
        window.close();
        break;
        case Event::KeyPressed:
        switch (event.key.code) {
            case Keyboard::Escape:
            current_player_typing = 0;
            gameState = TITLE_SCREEN;
            break;
            case Keyboard::End:
            current_player_typing = 0;
            break;
            case Keyboard::LBracket:
            current_player_typing = 1;
            break;
            case Keyboard::RBracket:
            current_player_typing = 2;
            break;
            case Keyboard::Backspace:
            if (text.length() > 0){
                string temp = text;
                temp.pop_back();
                if (temp != other_text || temp == ""){
                    text.pop_back();
                }
            }
            break;
            case Keyboard::Q:
            new_char = "Q";
            break;
            case Keyboard::W:
            new_char = "W";
            break;
            case Keyboard::E:
            new_char = "E";
            break;
            case Keyboard::R:
            new_char = "R";
            break;
            case Keyboard::T:
            new_char = "T";
            break;
            case Keyboard::Y:
            new_char = "Y";
            break;
            case Keyboard::U:
            new_char = "U";
            break;
            case Keyboard::I:
            new_char = "I";
            break;
            case Keyboard::O:
            new_char = "O";
            break;
            case Keyboard::P:
            new_char = "P";
            break;
            case Keyboard::A:
            new_char = "A";
            break;
            case Keyboard::S:
            new_char = "S";
            break;
            case Keyboard::D:
            new_char = "D";
            break;
            case Keyboard::F:
            new_char = "F";
            break;
            case Keyboard::G:
            new_char = "G";
            break;
            case Keyboard::H:
            new_char = "H";
            break;
            case Keyboard::J:
            new_char = "J";
            break;
            case Keyboard::K:
            new_char = "K";
            break;
            case Keyboard::L:
            new_char = "L";
            break;
            case Keyboard::Z:
            new_char = "Z";
            break;
            case Keyboard::X:
            new_char = "X";
            break;
            case Keyboard::C:
            new_char = "C";
            break;
            case Keyboard::V:
            new_char = "V";
            break;
            case Keyboard::B:
            new_char = "B";
            break;
            case Keyboard::N:
            new_char = "N";
            break;
            case Keyboard::M:
            new_char = "M";
            break;
            default:
            break;
        }
        break;
        default:
        break;
    }
    if ((text + new_char) != other_text){
        text += new_char;
    }
}


void loadLeaderBoard(string file_path){
    ifstream loadFile(file_path);
    string line;
    if(loadFile.is_open()){
        while(getline(loadFile, line)){
            stringstream lineStream(line);
            string pName;
            string winStr;
            string lossStr;
            int wins;
            int losses;
            getline(lineStream, pName, ' ');
            getline(lineStream, winStr, ' ');
            getline(lineStream, lossStr, ' ');
            wins = stoi(winStr);
            losses = stoi(lossStr);
            //cout << pName << " " << wins << " " << losses << endl;
            if (wins == 0 && losses == 0){
                leaderBoardRanking[pName] = -10000;
            }
            else {
                leaderBoardRanking[pName] = wins - losses;
            }
            leaderBoardWins[pName] = wins;
            leaderBoardLosses[pName] = losses;
        }
    }
    loadFile.close();
}

void saveLeaderBoard(string file_path){
    ofstream saveFile(file_path);
    if(saveFile.is_open()){
        for (auto& it : leaderBoardRanking) {
            //cout << it.first << " " << leaderBoardWins[it.first] << " " << leaderBoardLosses[it.first] << endl;
            saveFile << it.first << " " << leaderBoardWins[it.first] << " " << leaderBoardLosses[it.first] << endl;
        }
    }
    saveFile.close();
}


// Below Functions ripped from https://www.geeksforgeeks.org/sorting-a-map-by-value-in-c-stl/

// Comparator function to sort pairs
// according to second value
bool cmp(pair<string, int>& a, pair<string, int>& b) {
    return a.second > b.second;
}
  
// Function to sort the map according
// to value in a (key-value) pairs
void sortMap(map<string, int>& M) {
  
    // Declare vector of pairs
    vector<pair<string, int> > A;
  
    // Copy key-value pair from Map
    // to vector of pairs
    for (auto& it : M) {
        A.push_back(it);
    }
  
    // Sort using comparator function
    sort(A.begin(), A.end(), cmp);
  
    // Print the sorted value
    int counter = 1;
    for (auto& it : A) {
        //cout << it.first << ' ' << it.second << endl;
        string temp = "";
        if(counter == 1){
            temp = "1. ";
            temp += it.first;
            temp += "   Win-Loss Record: ";
            temp += to_string(leaderBoardWins[it.first]);
            temp += "-";
            temp += to_string(leaderBoardLosses[it.first]);
            leader1Box.setText(temp);
        }
        if(counter == 2){
            temp = "2. ";
            temp += it.first;
            temp += "   Win-Loss Record: ";
            temp += to_string(leaderBoardWins[it.first]);
            temp += "-";
            temp += to_string(leaderBoardLosses[it.first]);
            leader2Box.setText(temp);
        }
        if(counter == 3){
            temp = "3. ";
            temp += it.first;
            temp += "   Win-Loss Record: ";
            temp += to_string(leaderBoardWins[it.first]);
            temp += "-";
            temp += to_string(leaderBoardLosses[it.first]);
            leader3Box.setText(temp);
        }
        if(counter == 4){
            temp = "4. ";
            temp += it.first;
            temp += "   Win-Loss Record: ";
            temp += to_string(leaderBoardWins[it.first]);
            temp += "-";
            temp += to_string(leaderBoardLosses[it.first]);
            leader4Box.setText(temp);
        }
        if(counter == 5){
            temp = "5. ";
            temp += it.first;
            temp += "   Win-Loss Record: ";
            temp += to_string(leaderBoardWins[it.first]);
            temp += "-";
            temp += to_string(leaderBoardLosses[it.first]);
            leader5Box.setText(temp);
        }

        if (counter >= 5){
            return;
        }
        counter++;
    }
    
}