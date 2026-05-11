milestone1:
	gcc main.c -o dijkstra

milestone2:
	gcc sim.c -o sim

milestone3:
	gcc sim_animation.c -o sim -I"C:/raylib/include" -L"C:/raylib/lib" -lraylib -lopengl32 -lgdi32 -lwinmm

clean:
	rm -f dijkstra sim