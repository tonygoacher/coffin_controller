g++ -o coffin *.o -L/usr/lib/arm-linux-gnueabihf/ -lSDL_mixer -lpthread -lpigpiod_if2 $(sdl-config --libs)

