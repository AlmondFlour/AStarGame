g++ -c gameloop.cpp -I../../../SFML-2.2/include 
g++ -c handler.cpp
g++ -c interface.cpp
g++ -c entity.cpp
g++ -c menu.cpp
g++ -c camera.cpp
g++ -c animation.cpp
g++ gameloop.o handler.o interface.o entity.o menu.o camera.o animation.o -o AStarGame -L../../../SFML-2.2/lib -lsfml-graphics -lsfml-window -lsfml-system