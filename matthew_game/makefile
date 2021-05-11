INCLUDE_SFML = -lsfml-graphics -lsfml-window -lsfml-system -I/usr/include/SFML/
INCLUDE_SFML_EXTRAS = -lsfml-audio -lsfml-network
#INCLUDE_GL = -lGL -lGLU -lglut -I/usr/include/GL/


matt_play: matt_game.cpp ../fight_engine.cpp
	g++ -o game matt_game.cpp $(INCLUDE_SFML) $(INCLUDE_SFML_EXTRAS) -Wall #-Werror
	./game; rm game;

matt_compile: matt_game.cpp ../fight_engine.cpp
	g++ -o matt matt_game.cpp $(INCLUDE_SFML) $(INCLUDE_SFML_EXTRAS) -Wall #-Werror

clean:
	rm -f game matt

.PHONY: test clean
