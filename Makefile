CC="gcc"

all: rotate keyboard

keyboard:
	$(CC) -Wall -lGL -lGLU -lglut -lrply -o keyboard minescape-keyboard.c

rotate:
	$(CC) -Wall -lGL -lGLU -lglut -lrply -o minescape minescape.c

clean:
	-rm minescape
	-rm keyboard
