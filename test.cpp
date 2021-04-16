#include "fight_engine.cpp"


int main(int argc, char* argv[]){
    create_window("Awesome Game", 800, 800);
    
    KinematicBody2D wolf;
    wolf.setSprite((make_sprite("./images/monster_idle.png")));
    sprite_data* temp_sprite = wolf.getSpriteData();

    list_dir("./images");
    //Create a Sprite
    animation_data new_animation;
    new_animation.createAnimationData(4);
    new_animation.addAnimationData(temp_sprite);
    
    //Vector2u size = temp_sprite.imageSource.getSize();
      
    while (window.isOpen()) {
        Event event;
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
        window.draw(wolf.getSprite());
        window.display();
    }
    return 0;
}


/*
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