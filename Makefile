milestone1:
	gcc main.c -o dijkstra

milestone2:
	gcc sim.c -o sim

milestone3:
	gcc sim_animation.c -o sim -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

clean:
	rm -f dijkstra sim