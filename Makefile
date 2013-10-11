CC="gcc"

default:
	$(CC) -Wall -lGL -lGLU -lglut -lrply -o minescape minescape.c

clean:
	-rm minescape
