#include <GL/glut.h>
#include "minescape.h"

void minescapeDisplay() {
	return;
}
void minescapeReshape(GLsizei w, GLsizei h) {
	return;
}

void minescapeSetup() {
	return;
}

int main(int argc, char *argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_INDEX);

	glutInitWindowSize(WINDOW_SIZE_X,WINDOW_SIZE_Y);
	glutInitWindowPosition(WINDOW_POSITION_X,WINDOW_POSITION_Y);
	glutCreateWindow(WINDOW_TITLE);

	glutDisplayFunc(minescapeDisplay);
	glutReshapeFunc(minescapeReshape);

	minescapeSetup();

	glutMainLoop();

	return 0;
}
