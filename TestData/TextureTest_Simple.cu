#include "glnetwork.h"  //must be first
#include <math.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stim/math/vector.h>
#include <stim/gl/gl_spider.h>
#include <stim/gl/gl_texture.h>
#include <stim/gl/error.h>
#include <stim/visualization/camera.h>
#include <stim/visualization/glObj.h>

#include <stim/parser/arguments.h>
#ifdef TIMING
	#include <cstdio>
	#include <ctime>
#endif
#define _USE_MATH_DEFINES

//Vis and Coord Variables
#include<stim/math/constants.h>
	const float degtorad		= 360.0/(stim::TAU);
	stim::vec3<float> v_size(1.0, 1.0, 5.0);
	stim::vec3<float> i_size(1024.0, 1024.0, 98.0);
	stim::vec3<float> size(v_size[0]*i_size[0], v_size[1]*i_size[1], v_size[2]*i_size[2]);
	stim::vec<int> w_size(900,900);
	float GL[3] = {4000.0, 4000.0, 20000.0};
	stim::vec3<float> p;
	stim::vec3<float> up; 
	stim::vec3<float> d;  
	bool gui 			= false;
	int cyl = 0;
	


//Glut Variables
	float a = size[0]; 		//size of the volume.
        float b = size[1];		//y
        float c = size[2];		//z
        float z = 0.0;
	float curPos = 0.0;
	stim::camera cam;
	static bool button1		= false;
	static float mousePos[2]	= {0,0};
	static float prevmousePos[2]	= {0,0};
	
	stim::camera camSel;
        GLint lines[24] 	= {0,1,1,2,2,3,0,3,4,5,5,7,7,6,6,4,3,6,2,7,1,5,0,4};
	GLfloat vertexDrw[8][3];
	

//Runtime Variables
	
	GLuint texID;
        bool started = false;
	stim::gl_spider<float> spidey(200,100,50);
	std::vector<stim::vec3<float> > sl;
	std::vector<stim::vec3<float> > current;
        stim::vec3<float> temp(0.0, 0.0, 1.0);
	stim::vec3<float> ps;
	stim::vec3<float> ups; 
	stim::vec3<float> ds;  
	std::string sfname = "skel_new.txt";

//Testing Variables
#ifdef TIMING
	std::clock_t start;
	double duration;
#endif

	stim::glObj<float> skeleton; // glObject.
	stim::glnetwork<float> net;


//--------------------HELPER METHODS--------------------------------//
//------------------------------------------------------------------//
//Given a file name, load all the seedpoints.
//Depreciated
void
loadSeedPoints(std::string file)
{
	 
         std::ifstream myfile(file.c_str());
         string line;
         if(myfile.is_open())
         {
                 while (getline(myfile, line))
                 {
                         float x, y, z;
                         myfile >> x >> y >> z;
             		 sl.push_back(stim::vec<float>(
				((float) x)*0.6, 
				((float) y)*0.6, 
				((float) z)*1.0));
                 }
                 myfile.close();
        } else {
			std::cerr<<"failed" << std::endl;
	}
}
///updateVertices
///float a -- the new x coordinate for the basis of the cube.
///float b -- the new y coordinate for the basis of the cube.
///float c -- the new z coordinate for the basis of the cube.
void
updateCubeVertices(float aa, float bb, float cc)
{
	GLfloat temp[8][3] = {{bb, bb, z}, {z, bb, z}, {z, z, z},
                             {aa, z, z}, {aa, bb, cc}, {z, bb, cc},
                             {aa, z, cc}, {z, z, cc}};
	memcpy(&vertexDrw, &temp, sizeof(temp));
}


//Init 
//GLsizei xs -- (size in the x direction
//GLsizei xy -- (size in the y direction)
//Initializes all matrix and camera objects for GL, as well as loads the
//Seed points.

void 
glInit(GLsizei xs, GLsizei ys, int x = 512, int y = 512, int z = 298,
		 float dx = 0.6, float dy = 0.7, float dz = 1.0)
{

	 //Set all of thei global visuzalization variables.
	 ::v_size = stim::vec3<float>(dx, dy, dz);
	 ::i_size = stim::vec3<float>(x, y, z); 		
 	 ::size = stim::vec3<float>(v_size[0]*i_size[0], v_size[1]*i_size[1], v_size[2]*i_size[2]);

	 ::a = size[0]; 		//size of the volume.
         ::b = size[1];		//y
         ::c = size[2];		//z
	 updateCubeVertices(::a, ::b, ::c);
	 if(x >= 1024 || y >= 1024 || z >= 1024)
		 {GL[0] = 8000; GL[1] = 8000; GL[2] = 40000;}
	 

	 //Enable all the necessary GL options
	 //And clear all matrices for visualization.
         glEnable(GL_TEXTURE_3D);
         glEnable(GL_DEPTH_TEST);
         glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	 glClearColor(0.0, 0.0, 0.0, 0.0);
	 glColor4d(0.0, 0.0, 0.0, 1.0);
   	 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         glShadeModel(GL_FLAT);
         glViewport(0, 0, xs, ys);
         glMatrixMode(GL_PROJECTION);
         glLoadIdentity();
	 glOrtho(0.0, GL[0], 0.0, GL[1], 0.0, GL[2]);
	 glMatrixMode(GL_TEXTURE);
	 glLoadIdentity();
         glMatrixMode(GL_MODELVIEW);
         glLoadIdentity();

	 //Attach the spider, to the texture and set all the properties of the searched volume.
         spidey.attachSpider(texID);
	 spidey.setSize(x, y, z);
	 spidey.setDims(dx,dy, dz);
         spidey.initCuda();
	 cam.setPosition(0.0, 0.0, 0.0);
	 cam.setFocalDistance(4000.0);
}


//---------------------------DRAW METHODS---------------------------//
//------------------------------------------------------------------//


//Draws a cube that surrounds the volume
void
DrawCube()
{
        glLineWidth(2.5);
        glColor3f(1.0,1.0,1.0);
        glBegin(GL_LINES);
        
        for (int i = 0; i < 24; i++)
        {
                glVertex3f(vertexDrw[lines[i]][0],
                           vertexDrw[lines[i]][1],
                           vertexDrw[lines[i]][2]);     
        }
        glEnd();
}

//Draws a plane in the Z direction using the texture input texID
void
DrawTexturePlane()
{
	glEnable(GL_TEXTURE_3D);
	glBindTexture(GL_TEXTURE_3D, texID);
	glBegin(GL_QUADS);
		glTexCoord3f(
			z,
			z,
			curPos/::c
			);
		glVertex3f(
			z,
			z,
			curPos
			);
		glTexCoord3f(
			z,
			1.0,
			curPos/::c
			);
		glVertex3f(
			z,
			b,
			curPos
			);
		glTexCoord3f(
			1.0,
			1.0,
			curPos/::c
			);
		glVertex3f(
			a,
			b,
			curPos
			);
		glTexCoord3f(
			1.0,
			z,
			curPos/::c
			);
		glVertex3f(
			a,
			z,
			curPos
			);
		glEnd();
		glBindTexture(GL_TEXTURE_3D, 0);
		glDisable(GL_TEXTURE_3D);
}	

//Either loads the geometry from the trace (once complete)
//Or from file(if the trace is cancelled).
void
CreateList(bool fromSelf)
{
	if(fromSelf)
		net.createFromSelf();
	else
		skeleton.createFromFile(sfname);
}

//glut render method
void
renderScene()
{	
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glViewport(0,0, w_size[0], w_size[1]);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
  	glOrtho(0.0, GL[0], 0.0, GL[1], 0.0, GL[2]);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTranslatef(GL[0]/2, GL[1]/2, 0.0);
	glScalef(6.0, 6.0, 6.0);
	cam.LookAt((::a)/2.0, (::b)/2.0, (::c)/2.0);
	p	= cam.getPosition();
	up	= cam.getUp();
	d	= cam.getLookAt();
	gluLookAt(p[0], p[1], p[2], d[0], d[1], d[2], up[0], up[1], up[2]);
	CHECK_OPENGL_ERROR
	DrawCube();
	if(cyl == 0)
		net.Render();
	else if(cyl == 1)
		net.RenderCylinders();
	else
		net.RenderAll();
	DrawTexturePlane();
	CHECK_OPENGL_ERROR
	glutSwapBuffers();
}	


//--------------------------RUN TIME/TRACING------------------------//
//------------------------------------------------------------------//

//Runs the trace based on the seedpoints loaded in the Init.
//Stores the result in filename.
void
run(float min_cost, std::string filename)
{
	spidey.trace(min_cost);
	net = spidey.getGLNetwork();
	spidey.saveNetwork(filename);
}

//--------------------CONTROL AND VISUALIZATION---------------------//
//------------------------------------------------------------------//

//change size
void
changeSize(int w, int h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, GL[0], 0.0, GL[1], 0.0, GL[2]);
	glMatrixMode(GL_MODELVIEW);
}
	
//Process mouse button input
void
MouseButton(int button, int state, int x, int y)
{

        if (button == GLUT_LEFT_BUTTON)
        {
                button1          = (state == GLUT_DOWN) ? true : false;
               // button_shift     = glutGetModifiers();
                prevmousePos[0]  = (float)x;
                prevmousePos[1]  = (float)y; 
        }
}
//Process mouse input
void
MouseMotion(int x, int y)
{
        if(button1)
                {       
                        mousePos[0] = (prevmousePos[0] - (float) x)*0.00005;
                        mousePos[1] = ((float)y - prevmousePos[1])*0.00005;
                        prevmousePos[0] = (float)x;
                        prevmousePos[1] = (float)y;
                        cam.OrbitFocus(mousePos[0]*degtorad, mousePos[1]*degtorad);
                }
}

//Process Arrow Key presses.
void
processSpecialKeys(int key, int xx, int yy)
{
	switch(key)
	{
		case GLUT_KEY_UP:
			curPos = curPos + 1.0;		
			break;
		case GLUT_KEY_DOWN:
			curPos = curPos - 1.0;		
			break;
	}
	glutPostRedisplay();
}

//process keys.
void
processKeys(unsigned char key, int x, int y)
{

	if (key == 27)
		exit(0);
	///space
	if (key == 32)
	{
		cyl++;
		if(cyl > 3)
			cyl = 0;
	}
}

//Redraw
void
idleFunction()
{
	glutPostRedisplay();
}

void output_usage(){
	std::cout<<std::endl<<"Usage: volumespider --options [value(s)]" << std::endl;
	std::cout<<std::endl<<"Example usage:" << std::endl;
	std::cout<<"./volumespider --data /home/pavel/Documents/Test_Data/ --voxelsize 0.6 0.6 1.0 --seeds /Documents/VS/volume-spider/mainSeeds.txt --cost 195 --numtemplates 250 125 50 --gui" << std::endl;
}

//-------------------------------MAIN-------------------------------//
//------------------------------------------------------------------//

int
main(int argc, char **argv)
{
	std::vector<std::string>::iterator j;
	int cost = 61;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(w_size[0], w_size[1]);

	glutCreateWindow("Full_Run_From_Seed_Points");

	GLenum err = glewInit();

	//glut commands for rendering and control.
	glutKeyboardFunc(processKeys);
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutIdleFunc(idleFunction);	
	glutSpecialFunc(processSpecialKeys);


	stim::arglist args;

#ifdef _WIN32
	args.set_ansi(false);
#endif
	args.add("help", "Prints the usage information");
	args.add("load", "The file path to the data file (.obj) to load for visualization as a string", "", "[string]");
	args.add("seeds", "The file path to the seeds file in .txt format as a string", "./seeds.txt", "[string");
	args.add("savefile", "The name of the save file that will be generated after execution", "network.txt", "[string]");
	args.add("voxelsize", "Three float values representing the size of each voxel in x, y and z. Most likely in microns per voxel", "0.6 0.6 1.0", "[float], [float], [float]");
	args.add("numtemplates", "Three integer values representing the number of direction, position and size templates the algorithm will use for segmentation", "200, 100, 50","[int], [int], [int]");
	args.add("cost", "The cost value to use as a termination criterion for the segmentation","195", "[positive int]");
	args.add("gui", "Run the gui after or before segmentation.");
	
	args.parse(argc, argv);
	
	if(args["help"].is_set()){															//if the user asks for help, provide it
		output_usage();																	//output usage parameters
		std::cout << args.str()<< std::endl;											//output all arguments
		exit(1);																		//end execution
	}
	if(args.nargs() != 1){
		std::cout<<"Enter a path containing the images to be segmented."<<std::endl;
		output_usage();
		std::cout << args.str()<< std::endl;
		exit(1);
	}

	std::vector<std::string> commands;
	std::vector<int> sz;
	sz.resize(3);sz[0] = 512; sz[1] = 512; sz[2] = 228;

	std::vector<float> dm;
	dm.resize(3); dm[0] = 1.0; dm[1] = 1.0; dm[2] = 1.0;
	dm[0] = args["voxelsize"].as_float(0);
	dm[1] = args["voxelsize"].as_float(1);
	dm[2] = args["voxelsize"].as_float(2);

	std::vector<int> templates;
	templates.resize(3);
	templates[0] = args["numtemplates"].as_int(0);
	templates[1] = args["numtemplates"].as_int(1);
	templates[2] = args["numtemplates"].as_int(2);

	spidey.setNumberDirectionTemplates(templates[0]);
	spidey.setNumberPositionTemplates(templates[1]);
	spidey.setNumberSizeTemplates(templates[2]);

	cost = args["cost"].as_int();

	std::string image_path = args.arg(0);

	if (GLEW_OK != err)
	{
		std::cerr << "Failed" << std::endl;
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	std::cout<<"Loading images...";
	stim::gl_texture<unsigned char> stack (image_path + "/*.jpg", GL_LINEAR, GL_CLAMP_TO_EDGE)    ;
    	stack.attach();
	std::cout<<"done."<<std::endl;

	texID = stack.getTexture();
	sz = stack.getSize();
	
	if(args["seeds"].is_set())
	{
		std::string seedName = args["seeds"].as_string();
		spidey.setSeeds(seedName);
	}

	if(args["savefile"].is_set())
	{
		sfname = args["savefile"].as_string();
	}

	if(args["gui"].is_set())
	{
		gui=true;
	}

	

	#ifdef TIMING
		start = std::clock(); 
	#endif
	
	glInit(w_size[0], w_size[1], sz[0], sz[1], sz[2], dm[0], dm[1], dm[2]);
	spidey.trace(cost);
	#ifdef TIMING
		duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
		std::cerr << "Segmentation Took: " << duration << std::endl;
	#endif
//			spidey.saveNetwork(sfname);
	spidey.saveNetwork("Run_6_7_1024_ii_4_4.obj");
	#ifdef TIMING
		std::vector<double> times = spidey.getTimings();
		std::cout << "Branch detection took " << times[0]/1000 << " seconds" << std::endl;
		std::cout << "Finding optimal direction took " << times[1]/1000 << " seconds" << std::endl;
		std::cout << "Finding optimal position took " << times[2]/1000 << " seconds" << std::endl;
		std::cout << "Finding optimal size took " << times[3]/1000 << " seconds" << std::endl;
		std::cout << "Cost calculation took " << times[4]/1000 << " seconds" << std::endl;
		std::cout << "Network Manipulation took " << times[5]/1000 << " seconds" << std::endl;
		std::cout << "Collision Detection took " << times[6]/1000 << " seconds" << std::endl;

	#endif

	if(gui)
	{
		CHECK_OPENGL_ERROR
		net = spidey.getGLNetwork();
		net.createFromSelf();
		net.createCylinders();
		CHECK_OPENGL_ERROR
		glutMainLoop();
		return 0;
	} else {
		return 0;
	}
}



