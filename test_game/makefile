INCLUDE_SFML = -lsfml-graphics -lsfml-window -lsfml-system -I/usr/include/SFML/
INCLUDE_SFML_EXTRAS = -lsfml-audio -lsfml-network
#INCLUDE_GL = -lGL -lGLU -lglut -I/usr/include/GL/


test: test.cpp ../fight_engine.cpp
	g++ -o game test.cpp $(INCLUDE_SFML) $(INCLUDE_SFML_EXTRAS) -Wall -Werror
	./game; rm game
	rm game

clean:
	rm -f game

.PHONY: test clean
