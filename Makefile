CC="gcc"

default:
	$(CC) -Wall -lGL -lGLU -lglut -o minescape minescape.c

clean:
	-rm minescape
