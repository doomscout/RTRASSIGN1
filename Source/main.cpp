#define DEBUG
#define GL_GLEXT_PROTOTYPES
#include <iostream>
#include <memory>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include <SDL2/SDL.h>
#include <time.h>
#include "Point3f.hpp"
#include "SineWave3D.hpp"
#include <cstdlib> 


static int wantRedisplay = 1;
void postRedisplay();
void systemShutdown();
/*Color set from assignment 1 code sample*/
typedef struct { float r, g, b; } color3f;
typedef struct vec3f { float x, y, z; } vec3f;

color3f cyan = { 1.0, 0.0, 1.0 };
color3f white = { 1.0, 1.0, 1.0 };
color3f grey = { 0.8, 0.8, 0.8 };
color3f black = { 1.0, 1.0, 1.0 };

//I need this for init GlenBuffer after SDL2 Initialization
// because GlenBuffer can't be Initialization before SDL2 setting up
bool isSineWaveInitBuffer = false;


//Render/Draw Option with VBO or Immediation
typedef enum {
	IMMEDI_MODE,
	VERTEX_BUFFER_OBJECT
}DrawMethod;

//PolygonMode enum
typedef enum
{
	gl_line,
	gl_fill
	
}PolygonMode;

/*Global variable structure*/
typedef struct
{
	int witdhOfWindow;
	int heightOfWindow;
	bool isMultiScreen;
	int tess; //tessellation
	float currentFramerate;
	float t;
	bool animate;
	float lastT;
	float deltTime;
	PolygonMode polygonMode;
	bool lighting;
	bool normal;
	int waveDim;
	bool isDisplayOSD;
	bool isPrintConsole;
	bool isFPS30;
	int lightNumber;
}Global;

//Initialization global variable
Global g = {1920, 1080,false,32, 0.0f, 0.0f, false, 0,0, gl_line, false, false, 2, true, false, false, 1};
DrawMethod drawMethod = IMMEDI_MODE;
bool exit_main = false;
GLfloat prevTime = 0;
SineWave3D sineWave3D(g.tess);

/* Basic camera struct */
typedef struct {
    bool rotating;
    bool zooming;
    float rotX, rotY;
    float zoom;
    float sensitivity;
    float lastMouseX;
    float lastMouseY;
} Camera;
Camera camera = {false,false,0,0, 1.0f,0.3f,0,0};

using namespace std;

static SDL_Window *mainWindow = 0;

void initLighting();
void checkForGLerrors(int lineno);
void reshape (int w, int h);
void drawAxis(GLfloat length);
void renderingScene();
void display();
void viewMultiDisplay();
void renderOSD();
void update(float time);
void printConsole();
//SDL2
bool handleKeyDown(SDL_Keysym *key);
bool mouseDown(SDL_MouseButtonEvent *mouseEvent, bool pressure);
void mouseMotion(int x, int y);
bool handleEvents();


//sample code from sineWave code
void drawVector(vec3f *r, vec3f *v, float s, bool normalize, color3f *c);
void drawSineWave(int tess);


int main()
{

	 /* fake glutInit args for OSD */
	int argc = 0;   
    char *argv = "";
    glutInit(&argc, &argv);

   /*Initialization SDL2 setting*/	
 	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		return EXIT_SUCCESS;
	}

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	mainWindow = SDL_CreateWindow("Assignment1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, g.witdhOfWindow, g.heightOfWindow, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!mainWindow)
	{
		fprintf(stderr, "Failed to create a window: %s\n", SDL_GetError());
		return EXIT_SUCCESS;
	}

	SDL_GLContext mainGLContext = SDL_GL_CreateContext(mainWindow);
	SDL_GL_MakeCurrent(mainWindow, mainGLContext);
	reshape(g.witdhOfWindow, g.heightOfWindow);
	initLighting();


	bool loop = true;
	
	
	//Event Loop
	while(loop)
	{
		g.deltTime = SDL_GetTicks() - prevTime;
		prevTime =  SDL_GetTicks();
		handleEvents();
		    
		if (wantRedisplay) {
			if(!g.isMultiScreen)
				display();
			else
				viewMultiDisplay();
				
			wantRedisplay = 0;
		}
		update(g.deltTime * 0.001);
		
		if(exit_main)
		{
			loop = false;
		}		
	}

	sineWave3D.cleanUp();
	atexit(systemShutdown);

	return EXIT_SUCCESS;
}


/* 
 * We reimplement GLUT's glutPostRedisplay() function.  Why? Because
 * it allows us to easily request a redraw, without having to worry
 * about doing redundant redraws of the scene. We use the
 * wantRedisplay variable as a latch that is cleared when a redraw is
 * done.
 */
void postRedisplay()
{
  wantRedisplay = 1;
}
//SDL shutdown function
void systemShutdown()
{
	SDL_Quit();
}


//Initialization lighting function
void initLighting()
{
	const float lightAmbient[] = {0.1f, 0.1f, 0.1f, 1.0f};
   const float lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f}; // can be white color if used for Diffuse Light
    
    

	//const float LightDiffuse[] = {0.0f, 0.5f, 0.5f, 1.0f};
	
	const GLfloat lightPos[9][4] = {{0.0f, 1.0f, 0.0f, 1.0f}, //middle
                                    {-1.0f, 1.0f, 0.0f, 1.0f},// west
                                    {1.0f, 1.0f, 0.0f, 1.0f},//east
                                    {0.0f, 1.0f, 1.0f, 1.0f}, // north
                                    {0.0f, 1.0f, -1.0f, 1.0f},//south
                                    {-1.0f, 1.0f, 1.0f, 1.0f}, //north west
                                    {1.0f, 1.0f, 1.0f, 1.0f},//north east
                                    {1.0f, 1.0f, -1.0f, 1.0f},//south east
                                    {-1.0f, 1.0f, -1.0f, 1.0f} //south west
                                    };
                                    	

								
	//Initialization 9 different lighting
	for(int i = 0; i < 9; i++)
	{		
		glLightfv(GL_LIGHT0 + i, GL_POSITION, lightPos[i]);
		glLightfv(GL_LIGHT0 + i, GL_AMBIENT, lightAmbient);
		glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, lightSpecular);
		glLightfv(GL_LIGHT0 + i, GL_SPECULAR, lightSpecular);
	}

}

//Debug GL function with file number of line
void checkForGLerrors(int lineno)
{
  GLenum error;
  while ((error = glGetError()) != GL_NO_ERROR)
    printf("%d: %s\n", lineno, gluErrorString(error));
}

//Modifty/Change window size function
void reshape (int w, int h)
{
  glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  glOrtho(-1.0, 1.0, -1.0, 1.0, -100.0, 100.0);
 // gluPerspective(100.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
 // glTranslatef (0.0, 0.0, -5.0);
}


//Draw Axis Function with setting length
void drawAxis(GLfloat length)
{
	
	glPushAttrib(GL_CURRENT_BIT);
	glBegin(GL_LINES);
	//Red Line on x axis
	glColor3f(1,0,0);
	glVertex3f(-length,0,0);
	glVertex3f(length,0,0);

	//green Line on y axis
	glColor3f(0,1,0);
	glVertex3f(0,-length,0);
	glVertex3f(0,length,0);

	//Blue Line on z axis
	glColor3f(0,0,1);
	glVertex3f(0,0,-length);
	glVertex3f(0,0,length);
	
	glEnd();
	glPopAttrib();
}

//Drawing everything function
void renderingScene()
{

	drawAxis(5);

	if(g.lighting)
	{
		glEnable(GL_LIGHTING);
		//glEnable(GL_LIGHT0);
		#if 1
		for (int i = 0; i < 9; i++) {
			if(i < g.lightNumber)
				glEnable(GL_LIGHT0 + i);
			else
				glDisable(GL_LIGHT0 + i);		
		}	
		#endif
		glEnable(GL_NORMALIZE);
		glMaterialfv(GL_FRONT, GL_SPECULAR, (GLfloat *) &white);
		glMaterialf(GL_FRONT, GL_SHININESS, 25.0);
		//Nvidia don't like glLightModeli when rendering VBO and make massive lag ....
		//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	}
	else
	{
		 glDisable(GL_LIGHTING);
	}	
		
	if(drawMethod == IMMEDI_MODE)
	{
		//cout << "Draw immedation" << endl;
		drawSineWave(g.tess);
	}
	else if(drawMethod == VERTEX_BUFFER_OBJECT)
	{
		//cout << "Draw VBO" << endl;
		if(g.animate)
		{
			sineWave3D.animatSineWave(g.t);
		}
		sineWave3D.renderingWithVBO(g.lighting);

		if(g.normal)
		{
			//have to put disbland lighting for prevent lighting effect on draw vector 
			glDisable(GL_LIGHTING);
			sineWave3D.drawNormVec();
		}
	}
	
	if (g.lighting) {
		glDisable(GL_LIGHTING);
	}
}


//Display Function
void display()
{
	//only 1 time call this once and no more update after change
	if(!isSineWaveInitBuffer)
	{
		sineWave3D.initGlenBuffers();
		isSineWaveInitBuffer = true;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glEnable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, g.witdhOfWindow, g.heightOfWindow);
	
	if(g.polygonMode == gl_line)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    /* Camera transformations */
 
  
  /*General view*/ 
	glPushMatrix();  
  
	glRotatef(camera.rotX, 1.0f, 0.0f, 0.0f);
	glRotatef(camera.rotY, 0.0f, 1.0f, 0.0f);
    //sineWave3D.changeGlenBuffer();
	glScalef(camera.zoom, camera.zoom, camera.zoom);
	glDisable(GL_LIGHTING);
 	/* Draw here */
	renderingScene();
	glPopMatrix();
	
	if(g.isDisplayOSD) 
		renderOSD();
	
	if(g.isPrintConsole)
		printConsole();
	SDL_GL_SwapWindow(mainWindow);
}

//Multi Display view
void viewMultiDisplay()
{
	
	//sineWave3D.initGlenBuffers();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if(g.polygonMode == gl_line)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//FrontView
	glPushMatrix();
	glViewport(g.witdhOfWindow / 16.0, g.heightOfWindow * 9.0 / 16.0, g.witdhOfWindow * 6.0 / 16.0, g.heightOfWindow * 6.0 / 16.0);
	renderingScene();
	
	glPopMatrix();
	
	//Top View
	glPushMatrix();
	
	glViewport(g.witdhOfWindow / 16.0, g.heightOfWindow / 16.0, g.witdhOfWindow * 6.0 / 16.0, g.heightOfWindow * 6.0 / 16.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	renderingScene();
	
	glPopMatrix();	
	
	
	// Left view
	glPushMatrix();
	
	glViewport(g.witdhOfWindow * 9.0 / 16.0, g.heightOfWindow * 9.0/ 16.0, g.witdhOfWindow * 6.0 / 16.0, g.heightOfWindow * 6.0 / 16.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	renderingScene();

	glPopMatrix();	
	
	// General view
	
	glPushMatrix();
	glViewport(g.witdhOfWindow * 9.0 / 16.0, g.heightOfWindow / 16.0, g.witdhOfWindow * 6.0 / 16.0, g.heightOfWindow * 6.0 / 16.0);
   
    glRotatef(camera.rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(camera.rotY, 0.0f, 1.0f, 0.0f);
    glScalef(camera.zoom, camera.zoom, camera.zoom);
	
	renderingScene();
	
	
	if(g.isDisplayOSD) 
		renderOSD();
	
	if(g.isPrintConsole)
		printConsole();
	
	glPopMatrix();	
	
	
	SDL_GL_SwapWindow(mainWindow);

}

//Dispaly OSD function
void renderOSD()
{
	char buffer[30];
	glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);

    /* Create a temporary orthographic projection, matching
     * window dimensions, and push it onto the stack */
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, g.witdhOfWindow, 0, g.heightOfWindow, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    /* Clear current modelview (ie. from display) */
    glLoadIdentity();
    
        
    /* Tesselation */
	glColor3f(1.0, 1.0, 0);
	glRasterPos2i(10, 20);
	snprintf(buffer, sizeof (buffer), "tesselation:       %5d", g.tess);
	for (char *bufp = buffer; *bufp; bufp++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *bufp);
    
    /* Display framerate */
    snprintf(buffer, sizeof(buffer), "Frame rate (f/s): %d", (int)g.currentFramerate);
    glRasterPos2i(10, 30);
    for (char *bufp = buffer; *bufp; bufp++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *bufp);
    
        /* Display frame time */
    snprintf(buffer, sizeof(buffer), "Frame time(ms): %d", (int)(1000.0f / g.currentFramerate));
    glRasterPos2i(10, 50);
    for (char *bufp = buffer; *bufp; bufp++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *bufp);

    	//cout << "triangle rate (t/s)" :  << (int)g.currentFramerate * 2 * (g.tess * g.tess) << endl;
    /* Display frame time */
    snprintf(buffer, sizeof(buffer), "triangle rate (t/s): %d", (int)((int)g.currentFramerate * 2 * (g.tess * g.tess)));
    glRasterPos2i(10, 70);
    for (char *bufp = buffer; *bufp; bufp++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *bufp);
    
    
    
    glPopMatrix();

    /* Pop projection */
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    /* Restore "enable" state */
    glMatrixMode(GL_MODELVIEW);
    glPopAttrib();
}


//Update time for animation function
void update(float time)
{
	float dt;
	//const float fps 30
	const float fps30 = 30.0f;
	/* Update positions here */
    static float fpsTime = 0.0f;
    static int fpsFrames = 0;
    
    //30 fps mode
    /**
     * 1 second = 1000 millisecond = 1 000 000 000 nanosecond
     * 1 millisecond = 1 000 000 nanosecond
     * http://timmurphy.org/2009/09/29/nanosleep-in-c-c/
     */
     //store as nanosecond format
    static long sleepTime = 0; 
    
    struct timespec delay;
		if(g.isFPS30)
		{
			//calucation under millisecond format and 
			// to get FrameTime around 31 to 32 in frame/millseconds after reach on 30 fps
			long calucFrameTime = (sleepTime / 1000000L) + (1000.0f / fps30)
		                             - (time * 1000.0f);
		    //  cout << "calucMax" << calucFrameTime << endl;
		 
		 //convert To Nanosecond format from millisecond for return value on sleepTime
		     sleepTime = fmax(0.0f, calucFrameTime) * 1000000L;
		     delay.tv_sec = 0;
		     delay.tv_nsec = sleepTime;
		     nanosleep(&delay, (struct timespec*)NULL);
		 }
		 
    //animating
	if(g.animate)
	{
		dt = time - g.lastT;
		g.t += dt;
	}
    	
	fpsTime += time;
	fpsFrames += 1;
	
	//calucation frame per seconds
    if (fpsTime > 1.0f) {
        g.currentFramerate = fpsFrames / fpsTime;
        fpsTime = 0.0f;
        fpsFrames = 0;
    }
	postRedisplay();
}


//Printout on Console for peformance data
void printConsole()
{
	cout << "tessellation: " << g.tess << endl;
	cout << "Frame per Seconds: " << g.currentFramerate << endl;	
	cout << "triangle rate (t/s) : "  << (int)g.currentFramerate * 2 * (g.tess * g.tess) << endl;
	cout << "Frame time (millisecond / frame ) : " << 1000.0f / g.currentFramerate << endl;
	cout << "Light Numbers : " << g.lightNumber << endl;
	switch(g.polygonMode)
	{
		case gl_fill:
			cout << "Polygon Mode : fill" << endl;
			break;
		default:
			cout << "Polygon Mode : wireframe" << endl;
			break;
	}

	switch(drawMethod)
	{
		case IMMEDI_MODE:
			cout << " DrawMethod: IMMEDI_MODE" << endl;
			break;
		default:
			cout << " DrawMethod: VERTEX_BUFFER_OBJECT" << endl;
			break;
	}

	if(g.lighting)
		cout << "Light is ON" << endl;
	else
		cout << "Light is OFF" << endl;

}


bool handleKeyDown(SDL_Keysym *key)
{
	/* Handle key press events here */
	switch (key->sym)
	{
	case SDLK_ESCAPE:
		exit_main = true;
		return true;
		break;
		
	case 'f':
	g.isFPS30 = !g.isFPS30;
	return true;
	break;
	/*change DrawMethod hotkey*/	
	case 'd':
	switch(drawMethod)
	{
		case IMMEDI_MODE:
			drawMethod = VERTEX_BUFFER_OBJECT;
			break;
		default:
			drawMethod = IMMEDI_MODE;
			break;
	}	
	return true;
	break;		
	
	
	
	/*display OSD hotkey*/	
	case 'o':
	g.isDisplayOSD = !g.isDisplayOSD;
	return true;
	break;
	
	/*display single/multi Screen hotkey*/	
	case 's':
	g.isMultiScreen = !g.isMultiScreen;
	return true;
	break;
		
	/*print console hotkey*/	
	case 'c':
	g.isPrintConsole = !g.isPrintConsole;
	return true;
	break;
	/*Draw Normal hotkey*/
	case 'n':
		g.normal = !g.normal;
		return true;
		break;
	/*Change polygonMode hotkey*/
	case 'm':
		if(g.polygonMode == gl_fill)
			g.polygonMode = gl_line;
		else
			g.polygonMode = gl_fill;
		return true;
		break;	

		/*Change waveLength hotkey*/
	case 'w':
		g.waveDim++;
		sineWave3D.waveDim++;
		if(g.waveDim > 3)
		{
			sineWave3D.waveDim = 2;
			g.waveDim =2;
		}
		sineWave3D.animatSineWave(g.t);
		return true;
		break;	
	/*Animation hotkey*/
	case 'a':
		g.animate = !g.animate;
		return true;
		break;	
	/*Lighting turn on/off hotkey*/
	case 'l':
		g.lighting = !g.lighting;
		return true;
		break;	
	/*increase Lighting number hotkey*/
	case 'k':
		g.lightNumber++;
		if(g.lightNumber >= 9)
			g.lightNumber = 9;
		return true;
		break;
		
	/*decrease Lighting number hotkey*/
	case 'j':
		g.lightNumber--;
		if(g.lightNumber <= 1)
			g.lightNumber = 1;
		return true;
		break;
	/*Increase Tesselation hotkey*/
	case SDLK_KP_PLUS:
		g.tess *= 2;
		if(g.tess > 1024)
			g.tess = 1024;
			sineWave3D.reSize(g.tess);
		return true;
		break;
		
	/*Decrease Tesselation hotkey*/
	case SDLK_KP_MINUS:
		g.tess /= 2;
		if(g.tess < 8)
			g.tess = 8;
			sineWave3D.reSize(g.tess);
		return true;
		break;
		
	default:
		break;
	}

	return false;
}

//Press Mouse button function
bool mouseDown(SDL_MouseButtonEvent *mouseEvent, bool pressure)
{
	switch(mouseEvent->button)
	{
		case SDL_BUTTON_LEFT:
		if(pressure)
		{
			camera.rotating = pressure;
			#if 0
			cout << "Pressured right" << endl;
			#endif
		}
		else
		{
			camera.rotating = pressure;
			#if 0
			cout << "Release" << endl;
			#endif
		}
		return true;
		break;
		
		case SDL_BUTTON_RIGHT:
		if(pressure)
		{
			camera.zooming = pressure;
			#if 0
			cout << "Pressure left" << endl;
			#endif
		}
		else
		{
			camera.zooming = pressure;
			#if 0
			cout << "Release" << endl;
			#endif
		}
		return true;
		break;
	}
	return false;
}

//mouse motion function
void mouseMotion(int x, int y)
{
	int dx = x - camera.lastMouseX;
    int dy = y - camera.lastMouseY;

    if (camera.rotating) {
        camera.rotY += dx * camera.sensitivity;
        camera.rotX += dy * camera.sensitivity;
    }
    
    if(camera.zooming)
    {
		camera.zoom += dy * camera.sensitivity * .1f;
	}
    camera.lastMouseX = x;
    camera.lastMouseY = y;
}

//SDL2 event Dispatcher Function
bool handleEvents()
{
	static int lastX = 0;
    static int lastY = 0;
	SDL_Event event;
	
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			exit(EXIT_SUCCESS);
		case SDL_MOUSEMOTION:
			lastX = event.motion.x;
			lastY = event.motion.y;
			mouseMotion(lastX,lastY);
			postRedisplay();
			break;
		case SDL_MOUSEBUTTONDOWN:
			//cout << "left mouseClick" << endl;
			//return true;
			if(mouseDown(&event.button, true))
			postRedisplay();
			break;
		case SDL_MOUSEBUTTONUP:
			if(mouseDown(&event.button, false))
			postRedisplay();
			break;
		case SDL_KEYDOWN:
			if (handleKeyDown(&event.key.keysym))
			{
				postRedisplay();
			}
			return true;
			break;
		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
				if (event.window.windowID == SDL_GetWindowID(mainWindow))
				{
					SDL_SetWindowSize(mainWindow, event.window.data1, event.window.data2);
					reshape(event.window.data1, event.window.data2);
					g.witdhOfWindow = event.window.data1;
					g.heightOfWindow = event.window.data2;
					postRedisplay();
				};
				break;
			case SDL_WINDOWEVENT_CLOSE:
				return true;
				break;
			default:
				break;
			}
			break;

		default:
			break;
		}
	}

	return false;
}

//Draw immediate mode Sinewave3D Normal Vector from sample code
void drawVector(vec3f *r, vec3f *v, float s, bool normalize, color3f *c)
{
  glPushAttrib(GL_CURRENT_BIT);
  glColor3fv((GLfloat *)c);
  glBegin(GL_LINES);
  if (normalize) {
    float mag = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
    v->x /= mag;
    v->y /= mag;
    v->z /= mag;
  }
  glVertex3fv((GLfloat *)r);
  glVertex3f(r->x + s * v->x, r->y + s * v->y, r->z + s * v->z);
  glEnd();
  glPopAttrib();
}

//Draw immediate mode mode SineWave3D
void drawSineWave(int tess)
{
  const float A1 = 0.25, k1 = 2.0 * M_PI, w1 = 0.25;
  const float A2 = 0.25, k2 = 2.0 * M_PI, w2 = 0.25;
  float stepSize = 2.0 / tess;
  vec3f r, n;
  int i, j;
  float t = g.t;

  if (g.polygonMode == gl_line)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


  for (j = 0; j < tess; j++) {

    // Sine wave
   //glBegin(GL_QUAD_STRIP);
	glBegin(GL_TRIANGLE_STRIP);
    for (i = 0; i <= tess; i++) {
      r.x = -1.0 + i * stepSize;
      r.z = -1.0 + j * stepSize;

      if (g.waveDim == 2) {
	r.y = A1 * sinf(k1 * r.x + w1 * t);
	if (g.lighting) {
	  n.x = - A1 * k1 * cosf(k1 * r.x + w1 * t);
	  n.y = 1.0;
	  n.z = 0.0;
	}
      } else if (g.waveDim == 3) {
	r.y = A1 * sinf(k1 * r.x + w1 * t) + A2 * sinf(k2 * r.z + w2 * t);
	if (g.lighting) {
	  n.x = - A1 * k1 * cosf(k1 * r.x + w1 * t);
	  n.y = 1.0;
	  n.z = - A2 * k2 * cosf(k2 * r.z + w2 * t);
	}
      }

      if (g.lighting) 
	glNormal3fv((GLfloat *)&n);
      glVertex3fv((GLfloat *)&r);

      r.z += stepSize;

      if (g.waveDim == 3) {
	r.y = A1 * sinf(k1 * r.x + w1 * t) + A2 * sinf(k2 * r.z + w2 * t);
	if (g.lighting) {
	  n.z = - A2 * k2 * cosf(k2 * r.z + w2 * t);
	}
      }

      if (g.lighting) 
	glNormal3fv((GLfloat *)&n);
      glVertex3fv((GLfloat *)&r);
    }
    glEnd();
  }

  if (g.lighting) {
    glDisable(GL_LIGHTING);
  }

  // Normals
  if (g.normal) {
    for (j = 0; j <= tess; j++) {
      for (i = 0; i <= tess; i++) {
	r.x = -1.0 + i * stepSize;
	r.z = -1.0 + j * stepSize;

	n.y = 1.0;
	n.x = - A1 * k1 * cosf(k1 * r.x + w1 * t);
	if (g.waveDim == 2) {
	  r.y = A1 * sinf(k1 * r.x + w1 * t);
	  n.z = 0.0;
	} else {
	  r.y = A1 * sinf(k1 * r.x + w1 * t) + A2 * sinf(k2 * r.z + w2 * t);
	  n.z = - A2 * k2 * cosf(k2 * r.z + w2 * t);
	}

	drawVector(&r, &n, 0.05, true, &cyan);
      }
    }
  }
}


