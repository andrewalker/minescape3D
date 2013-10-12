#include <string.h>
#include <stdio.h>
#include <GL/glut.h>
#include <rply/rply.h>
#include "minescape.h"

double current_vertex[3];
int vertex_count;
int face_count;
double **vertices;
int **faces;
int v = 0;
int f = -1;
char filename[100];

float trans_x = 0.0f;
float trans_y = 0.0f;
float trans_z = 0.0f;

int angle        = 0;
float x_vector   = 0;
float y_vector   = 0;
float z_vector   = 0;
int angle_step   = 1;
float crt_angle  = 0;
int z_rotation   = 0;

void minescapeDisplay() {
	int i, j;
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();

	glRotatef(angle, 0.f, 0.f, 1.f);
	glTranslatef(trans_x, trans_y, trans_z);

	for (i = 0; i < face_count; i++) {
		glBegin(GL_POLYGON);
		for (j = 1; j <= faces[i][0]; j++) {
			glColor3f(0.5f/j, 0.0f, (i*j)/0.5f);
			glVertex3d(vertices[faces[i][j]][0], vertices[faces[i][j]][1], vertices[faces[i][j]][2]);
		}
		glEnd();
	}

	glPopMatrix();
	glutSwapBuffers();
}

void minescapeKeyboard(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_UP:    trans_y += -1.0f;  break;
		case GLUT_KEY_DOWN:  trans_y += 1.0f; break;
		case GLUT_KEY_RIGHT: trans_x += 1.0f;  break;
		case GLUT_KEY_LEFT:  trans_x += -1.0f; break;
	}

	glutPostRedisplay();
}

void minescapeSetup() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScalef(0.1f, 0.1f, 0.1f);

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

	//if(z_rotation == 1)
	//{
		angle += 2;
		z_rotation = 0;
	/*}
	else if(z_rotation == -1)
	{
		angle -= 1;
		z_rotation = 0;
	}*/

	glutPostRedisplay();
	glutTimerFunc(TIMER_DELAY, minescapeTimer, 1);
}

int main(int argc, char *argv[]) {
	if (argc == 1) {
		fprintf(stderr, "Uso: ./minespace <caminho para arquivo.ply>\n");
		return 1;
	}

	strcpy(filename, argv[1]);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowSize(WINDOW_SIZE_X,WINDOW_SIZE_Y);
	glutInitWindowPosition(WINDOW_POSITION_X,WINDOW_POSITION_Y);
	glutCreateWindow(WINDOW_TITLE);

	glutDisplayFunc(minescapeDisplay);
	glutSpecialFunc(minescapeKeyboard);
	glutTimerFunc(TIMER_DELAY, minescapeTimer, 1);

	minescapeSetup();

	glutMainLoop();

	return 0;
}
