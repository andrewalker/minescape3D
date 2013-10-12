#ifndef MINESCAPE_H
#define MINESCAPE_H

#define WINDOW_SIZE_X 800
#define WINDOW_SIZE_Y 600

#define WINDOW_POSITION_X 10
#define WINDOW_POSITION_Y 10

#define WINDOW_TITLE "Minescape3D"

#define TIMER_DELAY 30

void minescapeDisplay();
void minescapeReshape(GLsizei w, GLsizei h);
void minescapeSetup();
int loadPlyFile();

static int face_cb(p_ply_argument argument);
static int vertex_cb(p_ply_argument argument);

#endif
