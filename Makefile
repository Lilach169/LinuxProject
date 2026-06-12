milestone1:
	gcc dijkstra.c -o dijkstra

milestone2:
	gcc main.c -o sim -lraylib -lm -lpthread -ldl -lrt -lX11

milestone3:
	gcc sim_animation.c -o sim -lraylib -lm -lpthread -ldl -lrt -lX11

clean:
	rm -f dijkstra sim