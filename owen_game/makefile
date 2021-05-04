INCLUDE_SFML = -lsfml-graphics -lsfml-window -lsfml-system -I/usr/include/SFML/
INCLUDE_SFML_EXTRAS = -lsfml-audio -lsfml-network
#INCLUDE_GL = -lGL -lGLU -lglut -I/usr/include/GL/


<<<<<<< HEAD
test: fighting_game.cpp fight_engine.cpp
	g++ -o game ../fighting_game.cpp $(INCLUDE_SFML) $(INCLUDE_SFML_EXTRAS) -Wall -Werror
=======
test: fighting_game.cpp ../fight_engine.cpp
	g++ -o game fighting_game.cpp $(INCLUDE_SFML) $(INCLUDE_SFML_EXTRAS) -Wall -Werror
>>>>>>> 09342e9c56e019dedb49087dd905d406a09520b8
	./game; rm game;

clean:
	rm -f game

.PHONY: test clean
