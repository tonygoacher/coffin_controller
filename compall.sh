g++ -g -c *.cpp -L/usr/include/SDL -L/usr/include $(sdl-config --cflags) 
g++ -g -c *.c -L/usr/include/SDL -L/usr/include $(sdl-config --cflags) 

