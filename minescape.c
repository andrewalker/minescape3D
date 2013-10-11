#include <string.h>
#include <stdio.h>
#include <GL/glut.h>
#include <rply/rply.h>
#include "minescape.h"

#define TIMER_DELAY 30

double current_vertex[3];
int vertex_count;
int face_count;
double **vertices;
int **faces;
int v = 0;
int f = -1;
char filename[100];

float angle      = 0;
float x_vector   = 0;
float y_vector   = 0;
float z_vector   = 0;
float angle_step = 1;
float x_step     = 0.8;
float y_step     = 0.3;
float z_step     = 0.8;

void minescapeDisplay() {
	int i, j;
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(((int)angle) % 360, x_vector, y_vector, z_vector);
	glScalef(0.3f, 0.3f, 0.3f);

	for (i = 0; i < face_count; i++) {
		glBegin(GL_POLYGON);
		for (j = 1; j <= faces[i][0]; j++) {
			glColor3f(0.5f/j, 0.0f, (i*j)/0.5f);
			glVertex3d(vertices[faces[i][j]][0], vertices[faces[i][j]][1], vertices[faces[i][j]][2]);
		}
		glEnd();
	}

	glutSwapBuffers();
}

void minescapeReshape(GLsizei w, GLsizei h) {
	float windowHeight, windowWidth;

     // Evita a divisao por zero
     if(h == 0) h = 1;

     // Especifica as dimensões da Viewport
     glViewport(0, 0, w, h);

     // Inicializa o sistema de coordenadas
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();

     // Estabelece a janela de seleção (left, right, bottom, top)
     if (w <= h)  {
		windowHeight = 250.0f*h/w;
		windowWidth = 250.0f;
     }
     else  {
		windowWidth = 250.0f*w/h;
		windowHeight = 250.0f;
     }

     gluOrtho2D(0.0f, windowWidth, 0.0f, windowHeight);
}

void minescapeSetup() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	loadPlyFile();

	return;
}

static int vertex_cb(p_ply_argument argument) {
	long index;

	ply_get_argument_user_data(argument, NULL, &index);
	current_vertex[index] = ply_get_argument_value(argument);

	if (index == 2) {
		vertices[v] = (double *)malloc(3 * sizeof(double));
		vertices[v][0] = current_vertex[0];
		vertices[v][1] = current_vertex[1];
		vertices[v][2] = current_vertex[2];
		v++;
	}

	return 1;
}

static int face_cb(p_ply_argument argument) {
	long length, value_index;

	int position = (int) ply_get_argument_value(argument);

	ply_get_argument_property(argument, NULL, &length, &value_index);

	if (value_index < 0) {
		f++;
		faces[f] = (int *)malloc(position * sizeof(int));
		faces[f][0] = position;

		return 1;
	}

	faces[f][value_index+1] = position;

	return 1;
}

static int normal(p_ply_argument argument) { return 1; }

int loadPlyFile() {

	p_ply ply = ply_open(filename, NULL, 0, NULL);

	if (!ply) return -1;
	if (!ply_read_header(ply)) return -1;

	vertex_count = ply_set_read_cb(ply, "vertex", "x", vertex_cb, NULL, 0);
	ply_set_read_cb(ply, "vertex", "y", vertex_cb, NULL, 1);
	ply_set_read_cb(ply, "vertex", "z", vertex_cb, NULL, 2);

	ply_set_read_cb(ply, "vertex", "nx", normal, NULL, 0);
	ply_set_read_cb(ply, "vertex", "ny", normal, NULL, 0);
	ply_set_read_cb(ply, "vertex", "nz", normal, NULL, 0);

	face_count = ply_set_read_cb(ply, "face", "vertex_indices", face_cb, NULL, 0);

	vertices = (double **) malloc(vertex_count * sizeof(double *));
	faces    = (int **) malloc(face_count * sizeof(int *));

	if (!ply_read(ply)) return -1;

	ply_close(ply);

	return 1;
}

void minescapeTimer(int value) {
	angle += angle_step;
	x_vector += x_step;
	y_vector += y_step;
	z_vector += z_step;

	glRotatef(((int)angle) % 360, x_vector, y_vector, z_vector);

	glutPostRedisplay();
	glutTimerFunc(TIMER_DELAY, minescapeTimer, 1);
}

int main(int argc, char *argv[]) {
	strcpy(filename, argv[1]);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowSize(WINDOW_SIZE_X,WINDOW_SIZE_Y);
	glutInitWindowPosition(WINDOW_POSITION_X,WINDOW_POSITION_Y);
	glutCreateWindow(WINDOW_TITLE);

	glutDisplayFunc(minescapeDisplay);
//	glutReshapeFunc(minescapeReshape);
	glutTimerFunc(TIMER_DELAY, minescapeTimer, 1);

	minescapeSetup();

	glutMainLoop();

	return 0;
}
