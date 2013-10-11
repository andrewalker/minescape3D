#include <GL/glut.h>
#include "minescape.h"

void minescapeDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_POLYGON);
	glVertex3f(-0.6, -0.75, 0.5);
	glVertex3f(0.6, -0.75, 0);
	glVertex3f(0, 0.75, 0);
	glEnd();


	glFlush();
}

void minescapeReshape(GLsizei w, GLsizei h) {
	return;
}

void minescapeSetup() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	return;
}

int main(int argc, char *argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowSize(WINDOW_SIZE_X,WINDOW_SIZE_Y);
	glutInitWindowPosition(WINDOW_POSITION_X,WINDOW_POSITION_Y);
	glutCreateWindow(WINDOW_TITLE);

	glutDisplayFunc(minescapeDisplay);
//	glutReshapeFunc(minescapeReshape);

	minescapeSetup();

	glutMainLoop();

	return 0;
}
