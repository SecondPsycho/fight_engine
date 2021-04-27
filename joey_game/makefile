INCLUDE_SFML = -lsfml-graphics -lsfml-window -lsfml-system -I/usr/include/SFML/
INCLUDE_SFML_EXTRAS = -lsfml-audio -lsfml-network
#INCLUDE_GL = -lGL -lGLU -lglut -I/usr/include/GL/


joey: ./src/joey.cpp ../fight_engine.cpp
	g++ -o game joey.cpp $(INCLUDE_SFML) $(INCLUDE_SFML_EXTRAS) -Wall -Werror
	./joey
	rm -f joey

clean:
	rm -f joey

.PHONY: test clean
