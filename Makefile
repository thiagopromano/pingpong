pi: 
	gcc pong.c -o pong -lpthread -lGLU -lGL -lGL -lglut -lm -std=c99 -w
	./pong