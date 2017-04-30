// earth-simulation by Ohad Eder Pressman, 2001
// ohad@visual-i.com / ohad.visual-i.com

#define PI 3.14159265

#include <stdio.h>
#include <math.h>
#include <string.h>

#include "glut/glut.h"
#include "GLTexture.h"
#include "gltexfont/gltexfont.h"

#include "countries/countries.h"


// menu
enum MenuCommands {
	MENU_TEXTURE	= 0,
	MENU_WIREFRAME,
	MENU_LINEWIDTH,
	MENU_COUNTRY_NAMES,
	
	MENU_COAST,
	MENU_ISLAND,
	MENU_LAKE,
	MENU_RIVER,
	MENU_NATION,
	MENU_USSTATE,

	MENU_EXIT
};

// defines
#define EARTH_LON_RES	60	// Longitude Resolution (x)
#define EARTH_LAT_RES	60	// Latitude Resolution (y)

#define EARTH_RADIUS	6378	// in kilometers

#define WORLD_SCALE		0.01f	// scale of all the world

#define MIN_SCALE		0.5f	// how much can we zoom out
#define MAX_SCALE		1.9f	// and in


// structures
struct Vector {
	float x, y, z;
};

struct Mapping {
	float u, v;
};


// vector-file
struct VxpFeature {
	int		 nVerts;
	Vector	*pVerts;
};

struct Vxp {
	int			 nFeatures;
	VxpFeature	*pFeatures;
};


// variables
CGLTexture earthTexture;

Vector	vertices[EARTH_LON_RES+1][EARTH_LAT_RES+1];
Mapping mapping[EARTH_LON_RES+1][EARTH_LAT_RES+1];

Vector	country_names_pos[NUM_COUNTRIES];

int mouseX, mouseY;
int mouseButtonDown = 0, isCtrlDown = 0;

float rotX = 140, rotY = 0;
float autoRotX = 0, autoRotY = 0;
float scaleAll = 1;

int drawWireframe	= 1;
int drawTexture		= 1;
int showCountries	= 1;
int lineWidth		= 1;
int showVxp[6]		= {0,0,0,0,0,0};

Vxp	*vxp_data[6];

// forward
void LonLat2Point(float lon, float lat, Vector *pos);


// functions

// read binary-vector-data files (not a commercial format)
Vxp *load_bdv(char *pFileName)
{
	Vxp *vxp = new Vxp;

	FILE	*f;

	// read file
	f = fopen(pFileName, "rb");

	// read number of features
	fread(&vxp->nFeatures, 4, 1, f);
	// allocate features
	vxp->pFeatures = new VxpFeature[vxp->nFeatures];

	// start reading features
	for (int i=0; i< vxp->nFeatures; i++) {
		// read number of vertices
		fread(&vxp->pFeatures[i].nVerts, 4, 1, f);
		// allocate vertices
		vxp->pFeatures[i].pVerts = new Vector[ vxp->pFeatures[i].nVerts ];

		// scan vertices
		for (int j=0; j<vxp->pFeatures[i].nVerts; j++) {
			float lonlat[2];

			fread(lonlat, 8, 1, f);

			// project on sphere
			LonLat2Point(lonlat[1], lonlat[0], &vxp->pFeatures[i].pVerts[j]);
		}
	}

	fclose(f);

	return vxp;
}


void Resize( GLsizei iWidth, GLsizei iHeight ) 
{ 
	glViewport (0,0,iWidth,iHeight); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0,(GLfloat)iWidth/(GLfloat)iHeight, 0.1, 120.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Idle()
{
	glutPostRedisplay();
}

void Init()
{
	earthTexture.LoadTGA("images/earth.tga");

	// generate our sphere
	for (int x=0; x<=EARTH_LON_RES; x++) {
		for (int y=0; y<=EARTH_LAT_RES; y++) {
			// angle around y-axis (which is x-value)
			float	angX, angY;

			angX = (x * 360.f / EARTH_LON_RES) * PI / 180.f;
			angY = (-90.f + (y * 180.f / EARTH_LAT_RES)) * PI / 180.f;

			vertices[x][y].x = fabsf(cosf(angY)) * EARTH_RADIUS * sinf(angX);
			vertices[x][y].y = EARTH_RADIUS * sinf(angY);
			vertices[x][y].z = fabsf(cosf(angY)) * EARTH_RADIUS * cosf(angX);

			mapping[x][y].u = (float)x / EARTH_LON_RES;
			mapping[x][y].v = (float)y / EARTH_LAT_RES;
		}
	}

	// load bvd files
	vxp_data[0] = load_bdv("vector data/coast.bvd");
	vxp_data[1] = load_bdv("vector data/island.bvd");
	vxp_data[2] = load_bdv("vector data/lake.bvd");
	vxp_data[3] = load_bdv("vector data/river.bvd");
	vxp_data[4] = load_bdv("vector data/nation.bvd");
	vxp_data[5] = load_bdv("vector data/state.bvd");
}

void DrawEarth()
{
	if (drawTexture) {
		glEnable(GL_TEXTURE_2D);
		earthTexture.Use();
	}

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glColor3f(1,1,1);

	int x, y;

	for (y=0; y<EARTH_LAT_RES; y++) {

		glBegin(GL_QUAD_STRIP);
		
		for (x=0; x<EARTH_LON_RES; x++) {
			glTexCoord2fv((float*)&mapping[x][y]);
			glVertex3fv((float*)&vertices[x][y]);

			glTexCoord2fv((float*)&mapping[x][y+1]);
			glVertex3fv((float*)&vertices[x][y+1]);

			glTexCoord2fv((float*)&mapping[x+1][y]);
			glVertex3fv((float*)&vertices[x+1][y]);

			glTexCoord2fv((float*)&mapping[x+1][y+1]);
			glVertex3fv((float*)&vertices[x+1][y+1]);
		}

		glEnd();
	}

	if (drawWireframe) {

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glLineWidth(lineWidth);

		for (y=0; y<=EARTH_LAT_RES; y++) {
			glBegin(GL_LINE_STRIP);
			for (x=0; x<=EARTH_LON_RES; x++) {
				glColor4f(0,0,0,0.2f);
				glVertex3fv((float*)&vertices[x][y]);
			}
			glEnd();
		}

		for (x=0; x<=EARTH_LON_RES; x++) {
			glBegin(GL_LINE_STRIP);
			for (y=0; y<=EARTH_LAT_RES; y++) {
				glColor4f(0,0,0,0.2f);
				glVertex3fv((float*)&vertices[x][y]);
			}
			glEnd();
		}

		glDisable(GL_BLEND);
	}

	glDisable(GL_TEXTURE_2D);
}

void DrawVxp(Vxp *vxp)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int i=0; i<vxp->nFeatures; i++) {
		glBegin(GL_LINE_STRIP);

		if (drawTexture) {
			glColor4f(1,1,1,0.5f);
		} else {
			glColor4f(0,0,0,0.5f);
		}

		// run over all vertices in this feature
		for (int j=0; j<vxp->pFeatures[i].nVerts; j++) {
			glVertex3fv((float*)&vxp->pFeatures[i].pVerts[j]);
		}
		
		glEnd();
	}

	glDisable(GL_BLEND);
}


void LonLat2Point(float lon, float lat, Vector *pos)
{
	// lon -90..90
	// lat -180..180

	float	angX, angY;

	angX = (180.f+lat) * PI / 180.f;
	angY = lon * PI / 180.f;

	pos->x = fabsf(cosf(angY)) * EARTH_RADIUS * sinf(angX);
	pos->y = EARTH_RADIUS * sinf(angY);
	pos->z = fabsf(cosf(angY)) * EARTH_RADIUS * cosf(angX);
}


void DrawScene() 
{ 	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 

	glPushMatrix(); 

	static float time = 0;
	time += 0.1;

	// if the mouse-button is not down, auto-rotate
	if (!mouseButtonDown) {
		rotX += autoRotX*scaleAll*0.1f;
		rotY += autoRotY*scaleAll*0.1f;
	}

	glTranslatef(0,0,-125);
	glScalef(WORLD_SCALE, WORLD_SCALE, WORLD_SCALE);
	glScalef(scaleAll, scaleAll, scaleAll);
	glRotatef(rotY, 1,0,0);
	glRotatef(rotX, 0,1,0);

	GLdouble modelview_matrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix);


	DrawEarth();

	// darw state vector-data
	glPushMatrix();
	glScalef(1.001f, 1.001f, 1.001f);
	glLineWidth(lineWidth);

	for (int v=0; v<6; v++) {
		if (showVxp[v]) {
			DrawVxp(vxp_data[v]);
		}
	}

	glPopMatrix();


	// draw countries as solid red cubes
	if (showCountries) {
		// draw countries
		for (int i=0; i<NUM_COUNTRIES-1; i++) {

			LonLat2Point(countries[i].lon, countries[i].lat, &country_names_pos[i]);

			glPushMatrix();

			glTranslatef(country_names_pos[i].x, country_names_pos[i].y, country_names_pos[i].z);
			glColor3f(1,0,0);
			float radius;
			radius = 20+10*sinf(i+time);
			glutSolidCube(radius);

			glPopMatrix();
		}
	}

	glPopMatrix();

	if (showCountries) {
		// draw country-names
		GLdouble projection_matrix[16];
		GLint	 viewport[4];
		glGetDoublev(GL_PROJECTION_MATRIX, projection_matrix);
		glGetIntegerv(GL_VIEWPORT, viewport);

		// goto ortho
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, 1, 0, 1, 1, 129);
		glMatrixMode(GL_MODELVIEW);

		for (int i=0; i<NUM_COUNTRIES-1; i++) {
			GLdouble winX, winY, winZ;

			gluProject(country_names_pos[i].x, country_names_pos[i].y, country_names_pos[i].z,
						modelview_matrix,
						projection_matrix,
						viewport,
						&winX, &winY, &winZ);

			// now isn't this the nastiest trick
			if (winZ < 0.999f) {
				glColor4f(1,1,1,.5f);
				fontShadow();
				fontShadowColor(0,0,0);
				fontSize(10+4000*(0.999-winZ));
				fontDrawString(winX, winY, countries[i].name);
			}
		}

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}


	glutSwapBuffers(); 

	glutPostRedisplay();
}

void MouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			mouseX = x;
			mouseY = y;
			mouseButtonDown = 1;

			isCtrlDown = glutGetModifiers() & GLUT_ACTIVE_CTRL;
		} else {
			mouseButtonDown = 0;
		}
	}
}

void MouseMotion(int x, int y)
{
	int	deltX, deltY;

	// calculate mouse movement since click
	deltX = x - mouseX;
	deltY = y - mouseY;

	// store new mouse position
	mouseX = x;
	mouseY = y;

	if (mouseButtonDown) {
		// is Cotrl Down ?
		if (isCtrlDown) {
			// scale
			float addition;
			addition = ((deltX+deltY) / 200.f);
			
			if (addition < 0 && scaleAll+addition > MIN_SCALE) {
				scaleAll += addition;
			}

			if (addition > 0 && scaleAll+addition < MAX_SCALE) {
				scaleAll += addition;
			}
		} else {
			// rotate
			rotX += deltX*0.25f/scaleAll;
			rotY += deltY*0.25f/scaleAll;

			// save values for auto rotation
			autoRotX = deltX*0.25f;
			autoRotY = deltY*0.25f;
		}

		glutPostRedisplay();
	}
}

void SelectFromMenu(int idCommand)
{
	switch (idCommand) {
	case MENU_WIREFRAME:
		drawWireframe = !drawWireframe;
		break;
	case MENU_TEXTURE:
		drawTexture = !drawTexture;
		break;
	case MENU_LINEWIDTH:
		if (lineWidth == 1) {
			lineWidth = 2;
		} else {
			lineWidth = 1;
		}
		break;
	case MENU_COUNTRY_NAMES:
		showCountries = !showCountries;
		break;

	case MENU_COAST:
	case MENU_ISLAND:
	case MENU_LAKE:
	case MENU_NATION:
	case MENU_RIVER:
	case MENU_USSTATE:
		showVxp[idCommand-MENU_COAST] = !showVxp[idCommand-MENU_COAST];
		break;

	case MENU_EXIT:
		exit(0);
		break;
	}

	glutPostRedisplay();
}

int BuildPopupMenu (void)
{
	int menu;
	menu = glutCreateMenu(SelectFromMenu);
	glutAddMenuEntry ("Toggle Texturing\tt", MENU_TEXTURE);
	glutAddMenuEntry ("Toggle Wireframe\tw", MENU_WIREFRAME);
	glutAddMenuEntry ("Switch Line-Width\tl", MENU_LINEWIDTH);
	glutAddMenuEntry ("Toggle Country Names\tc", MENU_COUNTRY_NAMES);

	glutAddMenuEntry ("Toggle Coast-Line\t1", MENU_COAST);
	glutAddMenuEntry ("Toggle Islands\t2", MENU_ISLAND);
	glutAddMenuEntry ("Toggle Lakes\t3", MENU_LAKE);
	glutAddMenuEntry ("Toggle Rivers\t4", MENU_RIVER);
	glutAddMenuEntry ("Toggle Nation Boundaries\t5", MENU_NATION);
	glutAddMenuEntry ("Toggle US-States\t6", MENU_USSTATE);

	glutAddMenuEntry ("Exit\tEsc", MENU_EXIT);
	return menu;
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
		SelectFromMenu(MENU_COAST+key-'1');
		break;

	case 't':
		SelectFromMenu(MENU_TEXTURE);
		break;

	case 'w':
		SelectFromMenu(MENU_WIREFRAME);
		break;

	case 'l':
		SelectFromMenu(MENU_LINEWIDTH);
		break;

	case 'c':
		SelectFromMenu(MENU_COUNTRY_NAMES);
		break;

	case 27:	// escape
		SelectFromMenu(MENU_EXIT);
		break;
	}
}

void main(int argc, char **argv) 
{ 
	GLsizei iWidth = 640.0; 
	GLsizei iHeight = 480.0; 

	glutInit(&argc, argv);
	glutInitWindowPosition( 0, 0);
	glutInitWindowSize(iWidth, iHeight);
	glutInitDisplayMode( GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE );
	glutCreateWindow( "Earth Simulation" );
	BuildPopupMenu();
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutDisplayFunc(DrawScene );
	glutIdleFunc(Idle );
	glutReshapeFunc(Resize );
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutKeyboardFunc(Keyboard);

	Init();
 
	glClearColor(0.13f, 0.34f, 0.83f,0); // blue
	glEnable(GL_DEPTH_TEST); 
	glDepthFunc(GL_LEQUAL);

	Resize(iWidth, iHeight); 

    fontLoad("gltexfont/font.tga");

	glutMainLoop(); 
}