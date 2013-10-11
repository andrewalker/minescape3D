#ifndef MINESCAPE_H
#define MINESCAPE_H

#define WINDOW_SIZE_X 400
#define WINDOW_SIZE_Y 350

#define WINDOW_POSITION_X 10
#define WINDOW_POSITION_Y 10

#define WINDOW_TITLE "Minescape"

void minescapeDisplay();
void minescapeReshape(GLsizei w, GLsizei h);
void minescapeSetup();
int loadPlyFile(const char *filename);

static int face_cb(p_ply_argument argument);
static int vertex_cb(p_ply_argument argument);
static int normal_cb(p_ply_argument argument);

#endif
