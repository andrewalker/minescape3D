CC="gcc"

default: minespace

minespace:
	$(CC) -Wall -lGL -lGLU -lglut -lrply -o minescape minescape.c

clean:
	-rm minescape
	-rm keyboard
