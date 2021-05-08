# Fighting Game by Cordell King
# Art by Diego Arroyo
# Music from Ori and the Blind Forest

## The Engine
Our game engine called fight_engine is fairly robust and helps to handle physics, sprite loading and animation, sound, and music the last two had a big boost from SFML but we made some classes for convenience. Animation was the big thing for our game engine and that is handled very well by our system we use structs that hold all the animation data for say an Idle animation and we load all the nessecary images for that and then we tell the sprite to play and it does. 

## How Did I Use The Engine?
I used the engine to handle sprite information and physics. The physics allowed me to include moving platforms, collideable and non-collideable objects, clean jumping arcs, and even the ability to send characters flying with only a few lines of code! The Music and Text functions were also very nice, allowing me to easily create a title screen and add atmosphere.

## Screenshots
![Title](./title.png)
![Playing](./fight.png)
![End](./end.png)