#include "fight_engine.cpp"


class NewGame {
  public:
    NewGame(KinematicBody2D player, int n) {
      this->statics_count = 0;
      this->statics_max = n;
      this->statics = new KinematicBody2D[this->statics_max];
      this->player = player;
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
  private:
    KinematicBody2D player;
    KinematicBody2D* statics;
    int statics_count;
    int statics_max;
};

int main(int argc, char* argv[]){
    create_window("Awesome Game", 800, 800);
    
    KinematicBody2D wolf;
    wolf.setSprite((make_sprite("./images/monster_idle.png")));
    sprite_data* temp_sprite = wolf.getSpriteData();

    NewGame Game(wolf, 1);
    Game.addStatic(KinematicBody2D(0,600,800,200));
    Game.getStatic(0)->initRectangle();
    
    list_dir("./images");
    //Create a Sprite
    animation_data new_animation;
    new_animation.createAnimationData(4);
    new_animation.addAnimationData(temp_sprite);
    
    //Vector2u size = temp_sprite.imageSource.getSize(); //Does anybody actually want this line? -Cordell King
    
    Event event;

    // Framerate Control -Cordell King
    Framerate ticker(30);
    int t = 0;
    int seconds = 0;
    while (window.isOpen()) {
      // Framerate Control -Cordell King
      ticker.next_frame();
      t += 1;
      if (t >= 30) {
        seconds += 1;
        cout << seconds << " seconds\n";
        t -= 30;
      };
      
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
              case Keyboard::W:
                wolf.move(Vector2D(0,10));
                break;
              case Keyboard::S:
                wolf.move(Vector2D(0,-10));
                break;
              case Keyboard::A:
                wolf.move(Vector2D(10,0));
                break;
              case Keyboard::D:
                wolf.move(Vector2D(-10,0));
                break;
              default:
                break;
            }
          default:
            break;
        }
      }

      //Draw the Screen
      window.clear(Color(42,42,42,255)); // Dark gray.
      window.draw(Game.getStatic(0)->getRectangle()); //Draw the Floor
      window.draw(wolf.getSprite());
      window.display();
    }
    return 0;
}


/* Does ANYBODY want this code? -Cordell King
void main(int argc, char** argv[]){
  sf::RenderWindow window(sf::VideoMode(640,480),
    "Bouncing mushroom.");

  sf::Texture mushroomTexture;
  mushroomTexture.loadFromFile("Mushroom.png");
  sf::Sprite mushroom(mushroomTexture);
  sf::Vector2u size = mushroomTexture.getSize();
  mushroom.setOrigin(size.x / 2, size.y / 2);
  sf::Vector2f increment(0.4f, 0.4f);

  while(window.isOpen()){
    sf::Event event;
    while(window.pollEvent(event)){
      if(event.type == sf::Event::Closed){
        window.close();
      }
    }

    if((mushroom.getPosition().x + (size.x / 2) >
      window.getSize().x && increment.x > 0) ||
      (mushroom.getPosition().x - (size.x / 2) < 0 &&
      increment.x < 0))
    {
        // Reverse the direction on X axis.
        increment.x = -increment.x;
    }

    if((mushroom.getPosition().y + (size.y / 2) >
      window.getSize().y && increment.y > 0) ||
      (mushroom.getPosition().y - (size.y / 2) < 0 &&
      increment.y < 0))
    {
         // Reverse the direction on Y axis.
        increment.y = -increment.y;
    }

    mushroom.setPosition(mushroom.getPosition() + increment);

    window.clear(sf::Color(16,16,16,255)); // Dark gray.
    window.draw(mushroom); // Drawing our sprite.
    window.display();
  }
}
*/