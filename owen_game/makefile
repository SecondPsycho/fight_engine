INCLUDE_SFML = -lsfml-graphics -lsfml-window -lsfml-system -I/usr/include/SFML/
INCLUDE_SFML_EXTRAS = -lsfml-audio -lsfml-network
#INCLUDE_GL = -lGL -lGLU -lglut -I/usr/include/GL/

test: fighting_game.cpp ../fight_engine.cpp
	g++ -o owen fighting_game.cpp $(INCLUDE_SFML) $(INCLUDE_SFML_EXTRAS)
#	./game; rm game;

clean:
	rm -f game owen

.PHONY: test clean
