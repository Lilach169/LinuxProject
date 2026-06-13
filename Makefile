milestone1:
	gcc dijkstra.c -o dijkstra

milestone2:
	gcc main.c -o sim -lraylib -lm -lpthread -ldl -lrt -lX11

milestone3:
	gcc sim_animation.c -o sim -lraylib -lm -lpthread -ldl -lrt -lX11

milestone4:
	gcc milestone4.c -o sim -lraylib -lm -lpthread -ldl -lrt -lX11
	
milestone5:
	gcc milestone5.c -o sim -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

clean:
	rm -f dijkstra sim
