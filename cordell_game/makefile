INCLUDE_SFML = -lsfml-graphics -lsfml-window -lsfml-system -I/usr/include/SFML/
INCLUDE_SFML_EXTRAS = -lsfml-audio -lsfml-network


tree: tree.cpp ../fight_engine.cpp
	g++ -o cordell tree.cpp $(INCLUDE_SFML) $(INCLUDE_SFML_EXTRAS) -Wall -Werror
#	./game;

clean:
	rm -f cordell

.PHONY: tree clean
