#include <GL/glut.h>
#include <rply/rply.h>
#include "minescape.h"

int **points;
int *face[8];
int p = 0;

void minescapeDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);

	glScalef(0.3f, 0.3f, 0.3f);
	glRotatef(90, 0.0, 1.0, 0.5);

	glEnable(GL_NORMALIZE);
	loadPlyFile("minescape_cart.ply");

	glFlush();
}

void minescapeReshape(GLsizei w, GLsizei h) {
	return;
}

void minescapeSetup() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	return;
}

static int vertex_cb(p_ply_argument argument) {
	int *vertex;
	long index;

	ply_get_argument_user_data(argument, (void **)&vertex, &index);
	vertex[index] = ply_get_argument_value(argument);

	if (index == 2) {
		points[p] = (int *)malloc(3 * sizeof(int));
		points[p][0] = vertex[0];
		points[p][1] = vertex[1];
		points[p][2] = vertex[2];
		p++;
	}

	return 1;
}

static int normal_cb(p_ply_argument argument) {
	int *normal;
	long index;

	ply_get_argument_user_data(argument, (void **)&normal, &index);
	normal[index] = ply_get_argument_value(argument);

	if (index == 2) {
		points[p] = (int *)malloc(3 * sizeof(int));
		points[p][0] = normal[0];
		points[p][1] = normal[1];
		points[p][2] = normal[2];
		p++;
	}

	return 1;
}

static int face_cb(p_ply_argument argument) {
	int i;
	long length, value_index;

	int position = (int) 2 * ply_get_argument_value(argument);

	ply_get_argument_property(argument, NULL, &length, &value_index);

	if (value_index < 0) {
		return 1;
	}

	face[value_index] = (int *)malloc(6 * sizeof(int));

	face[value_index][0] = points[position][0];
	face[value_index][1] = points[position][1];
	face[value_index][2] = points[position][2];

	face[value_index][3] = points[position+1][0];
	face[value_index][4] = points[position+1][1];
	face[value_index][5] = points[position+1][2];

	if (length == value_index + 1) {
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_TRIANGLES);
		for (i = 0; i < length; i++) {
			glVertex3d(face[i][0], face[i][1], face[i][2]);
			glNormal3d(face[i][3], face[i][4], face[i][5]);
		}
		glEnd();
	}

	return 1;
}

int loadPlyFile(const char *filename) {
	int vertex[3], normal[3];
	int vertex_count;

	p_ply ply = ply_open(filename, NULL, 0, NULL);

	if (!ply) return -1;
	if (!ply_read_header(ply)) return -1;

	vertex_count = ply_set_read_cb(ply, "vertex", "x", vertex_cb, vertex, 0);
	ply_set_read_cb(ply, "vertex", "y", vertex_cb, vertex, 1);
	ply_set_read_cb(ply, "vertex", "z", vertex_cb, vertex, 2);

	ply_set_read_cb(ply, "vertex", "nx", normal_cb, normal, 0);
	ply_set_read_cb(ply, "vertex", "ny", normal_cb, normal, 1);
	ply_set_read_cb(ply, "vertex", "nz", normal_cb, normal, 2);

	ply_set_read_cb(ply, "face", "vertex_indices", face_cb, NULL, 0);

	points = (int **) malloc(2 * vertex_count * sizeof(int *));

	if (!ply_read(ply)) return -1;

	ply_close(ply);

	return 1;
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
