pi: 
	gcc pong.c -g -o pong -lpthread -lGLU -lGL -lGL -lglut -lm -std=gnu99 -w
	./pong