#include "glnetwork.h"  //must be first
#include <math.h>
#include <cmath>
#include <iostream>
#include <vector>
//#include "stim/math/mathvec.h"
#include <stim/math/vector.h>
//#include <GL/gl.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
//#include <GL/glext.h>
#include "stim/gl/gl_spider.h"
#include "stim/gl/gl_texture.h"
#include "stim/gl/error.h"
#include "stim/visualization/camera.h"
#include <stim/visualization/glObj.h>
#include <stim/math/rect.h>
#include <stim/math/constants.h>

//#include <ANN/ANN.h>
//#include "fiber.h"
#define VERTICAL 	1
#define HORIZONTAL 	1
#define _USE_MATH_DEFINES
		stim::camera cam;
		stim::camera Parker;
		GLuint texID;
		GLuint texID2;
		int id = 0;
//		GLuint fboId;
		stim::vec<float> D(0,0,0);
//		GLuint rboId;
//		GLuint pbo;
		float pvalue = 0.0;
		std::vector<stim::vec<float > > pst;
//		float a = 512.0*0.6;
//		float b = 512.0*0.6;
//		float c = 298.0*1.0;
//		float DIMS[3] = {1024.0, 1024.0, 98.0};
	//	float DIMS[3] = {1024.0, 1024.0, 1024.0};
		float DIMS[3] = {512, 512, 298};
		float VOXDIMS[3] = {0.6, 0.6, 1.0};
		float a = DIMS[0]*VOXDIMS[0];
		float b = DIMS[1]*VOXDIMS[1];
		float c = DIMS[2]*VOXDIMS[2];
		float z = 0.0;
//		float GL[3] = {900.0, 900.0, 40000.0};
		float GL[3] = {1300.0, 1300.0, 40000.0};
//		GLsizei size[2] = {900,900};
		GLsizei size[2] = {1300,1300};
		GLfloat Normals[6][3] =
			{{ -1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
			{0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0}};
		GLint faces[6][4] =
			{{0, 1, 2, 3}, {4, 5, 7, 6}, {1, 5, 7, 2},
	 		 {3, 2, 7, 6}, {0, 4, 6, 3}, {0, 1, 5, 4}};
		GLfloat vertex[8][3] =
			{{0.0, 0.0, 1.0}, {1.0, 0.0, 1.0}, {1.0, 1.0, 1.0},
	 		{0.0, 1.0, 1.0}, {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0},
	 		{0.0, 1.0, 0.0}, {1.0, 1.0, 0.0}}; 
		GLint lines[24] = {0,1,1,2,2,3,0,3,4,5,5,7,7,6,6,4,3,6,2,7,1,5,0,4};
		//GLfloat vertexDrw[8][3] =
		//	{{a, a, b}, {b, a, b}, {b, b, b},
	 	//	{a, b, b}, {a, a, a}, {b, a, a},
	 	//	{a, b, a}, {b, b, a}};
		GLfloat vertexDrw[8][3] =
			{{b, b, z}, {z, b, z}, {z, z, z},
	 		{a, z, z}, {a, b, c}, {z, b, c},
	 		{a, z, c}, {z, z, c}};
		static float mousePos[2]        = {0,0};
		static float prevmousePos[2]        = {0,0};
		unsigned long tick 		= 0;
		stim::vec<float> p(0,0,0);
		stim::vec<float> up(0,0,0);
		stim::vec<float> d(0,0,0);
		static bool button1 		= false;
		static bool button_shift	= false;
		static float degtorad		= 360/(stim::TAU);
//		static GLfloat adjustTex	= 1.0/DIMS[2]/2;
//		static GLfloat adjustDrw	= 2.0/DIMS[2]/2;
//		static GLfloat oriTex[3]	= {adjustTex*213+adjustTex/2
//							,adjustTex*213+adjustTex/2
//								,adjustTex*213+adjustTex/2};
//		static GLfloat oriDrw[3]	= {adjustDrw/2
//							,adjustDrw/2
//								,adjustDrw/2};
//		static GLfloat org[2]		= {adjustTex/2,adjustTex/2+425*adjustTex};
		stim::gl_spider<float> spidey(1000, 200, 100);
		stim::vec<float>p1(0,0,0);
		stim::vec<float>p2(0,0,0);
		stim::vec<float>p3(0,0,0);
		stim::vec<float>p4(0,0,0);
		stim::glObj<float>skeleton;
		std::vector<stim::vec3<float> > 	sl;
		std::vector<stim::vec3<float> > 	sv;
		std::vector<float >	       	sm;
		static bool tracingLine 	= false;
		ofstream branches;
//		static bool bfound		= false;
		static bool minimize		= false;
		static bool artificial		= false;


//network stuff
		stim::glnetwork<float> net;
		std::clock_t start;
		double duration;

		std::vector<stim::vec3<float> > Ps;
		std::vector<stim::vec<float> > Ms;
		

void 
glInit()
{
	glEnable(GL_TEXTURE_3D);
	glEnable(GL_DEPTH_TEST);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor4d(0.0, 0.0, 0.0, 1.0);
	glShadeModel(GL_FLAT);
//	cam.setPosition(500.0,500.0,500.0);
	cam.setPosition(1000.0,1000.0,1000.0);
	cam.setFocalDistance(100.0);
	cam.LookAt(300.0, 300.0, 300.0);
	p = cam.getPosition();
	up = cam.getUp();
	d = cam.getLookAt();
	glViewport(0, 0, size[0], size[1]);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, GL[0], 0.0, GL[1], 0.0, GL[2]);
	glMatrixMode(GL_MODELVIEW);
//	spidey = stim::gl_spider<float>(0.515372, 0.564174, 0.494553,
//			0.130563, -0.98295, -0.129467,
//			 0.03);
	//spidey = stim::gl_spider<float>(93.0, 273.0, 78.0,
	//		0.0, 0.0, -1.0,
	//		 100.0);
	//spidey = stim::gl_spider<float>();
	//std::cout<< p1 << ":" << p2 << ":" << p3 <<":" << p4 << std::endl;
//spidey = stim::gl_spider<float>
//	(0.0641842,0.0172776,-0.997788,0.537789,0.587935,0.449583,0.0259658);
//	spidey = stim::gl_spider<float>(0.451825, 0.592827, 0.558536,
 //			0.159298, -0.11443, -0.980576,
//			 0.0367658);
	//spidey = stim::gl_spider<float>(0.3075257, 0.630329, 0.496995,
	//				-0.532453, 0.293348, -0.794993,
	//				0.03);
	spidey.attachSpider(texID);
	spidey.setSize(DIMS[0], DIMS[1], DIMS[2]);
	spidey.setDims(VOXDIMS[0],VOXDIMS[1], VOXDIMS[2]);
	Parker.setPosition(spidey.getPosition());
	Parker.LookAt(spidey.getDirection());
//	GenerateFBO(400, 200);
	spidey.initCuda();
//	std::ifstream myfile("SeedBranchVec.txt");
	std::ifstream myfile("mainSeeds.txt");
//	std::ifstream myfile("SeedBranchVec.txt");
	string line;
	if(myfile.is_open())
	{
		while (getline(myfile, line))
		{
			float x, y, z, u, v, w, m;
			//myfile >> x >> y;
			myfile >> x >> y >> z >> u >> v >> w >> m;
			sl.push_back(stim::vec3<float>(x, y, z));
			sv.push_back(stim::vec3<float>(u,v,w));
			sm.push_back(m);
		}
		myfile.close();
	} else { std::cerr<<"failed" << std::endl;}
//	skeleton = stim::obj<float>("Skeleton.obj");	
	branches.open("branches4.txt");
		pst.resize(8);
		for(int i = 0; i < pst.size(); i++)
			pst[i] = stim::vec<float>(0,0,0);

//	spidey.setPosition(78, 207, 178);
//	spidey.setDirection(-0.073569, 0.484641, 0.871614);
//	spidey.setMagnitude(14.0);

//starting
//	spidey.setPosition(150, 215, 210);
//	spidey.setDirection(0.0214, -0.0657, 0.995531);
//	spidey.setMagnitude(16.864);

//Branch
	spidey.setPosition(0.0, 0.0, 0.0);
//	spidey.setPosition(203.876, 148.344, 245.522);
	spidey.setDirection(0.0, 0.0, 1.0);
	spidey.setMagnitude(16);
	net = spidey.getGLNetwork();
	net.createFromSelf();
	net.createCylinders();
//	skeleton.Begin(stim::OBJ_LINE);

	//glGenRenderbuffers(1, &rboId);
	//glBindRenderbuffer(GL_RENDERBUFFER, rboId);
	//GLuint rboId;
	//glGenRenderbuffers(1, &rboId);
	//glBindRenderbuffer(GL_RENDERBUFFER, rboId);
	//glGenBuffers(1, &pbo);
	//glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, pbo);
	//glBufferData(GL_PIXEL_UNPACK_BUFFER_ARB, 426*426*sizeof(uchar4), NULL, GL_DYNAMIC_DRAW_ARB); 
	CHECK_OPENGL_ERROR
}


void
DrawCube()
{
	glLineWidth(2.5);
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_LINES);
	
	for (int i = 0; i < 24; i++)
	{
		glVertex3f(vertexDrw[lines[i]][0],
			   vertexDrw[lines[i]][1],
			   vertexDrw[lines[i]][2]);	
	}
	glEnd();
}

void
DrawCylinder(float r)
{			  
//			  stim::vec<float> mg = spidey.getMagnitude();
//                        float z0 = -r; float z1 = r; float r0 = r;
                          float z0 = -0.5; float z1 = 0.5; float r0 = 0.5;
                          float x,y;
//                        float xold = 0.0; float yold = 0.0;
                          float xold = 0.5; float yold = 0.0;
                          float step = 360.0/1089.0*36;
                          glEnable(GL_TEXTURE_3D);
                          glBindTexture(GL_TEXTURE_3D, texID);
                          glBegin(GL_QUAD_STRIP);
                                 for(float i = step; i <= 360.0; i += step)
                                 {
                                          x=r0*cos(i*stim::TAU/360.0);
                                          y=r0*sin(i*stim::TAU/360.0);
                                          glTexCoord3f(x,y,z0); 
//                                        glVertex2f(0.0, j*8.0+8.0); 
                                          glVertex3f(x,y,z0);
                                          glTexCoord3f(x,y,z1); 
//                                        glVertex2f(16.0, j*8.0+8.0);
                                          glVertex3f(x, y, z1);
                                          glTexCoord3f(xold,yold,z1); 
//                                        glVertex2f(16.0, j*8.0);
                                          glVertex3f(xold, yold, z1);
                                          glTexCoord3f(xold,yold,z0); 
//                                        glVertex2f(0.0, j*8.0);
                                          glVertex3f(xold, yold, z0);
                                          xold=x;
                                          yold=y;
                                 }                                                                 
                          glEnd();  
}

void
DrawCylinder(std::vector<stim::vec3<float> > inP, std::vector<stim::vec<float> > inM, int withTex = 1)
{			  

	stim::cylinder<float> cyl(inP, inM);
	std::vector<std::vector<stim::vec3<float> > > p = cyl.getPoints(8);
	if(withTex){
        glEnable(GL_TEXTURE_3D);
        glBindTexture(GL_TEXTURE_3D, texID);
	for(int i = 0; i < p.size()-1; i++)
	{
			for(int j = 0; j < p[0].size()-1; j++)
			{
//				glColor4f(1.0, 1.0, 0.0, 0.5);
//				glEnable(GL_BLEND);
//				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glBegin(GL_QUADS);
					glTexCoord3f(p[i][j][0], p[i][j][1], p[i][j][2]);
					glVertex3f(p[i][j][0], p[i][j][1], p[i][j][2]);

					glTexCoord3f(p[i][j+1][0], p[i][j+1][1], p[i][j+1][2]);
					glVertex3f(p[i][j+1][0], p[i][j+1][1], p[i][j+1][2]);

					glTexCoord3f(p[i+1][j+1][0], p[i+1][j+1][1], p[i+1][j+1][2]     );
					glVertex3f(p[i+1][j+1][0], p[i+1][j+1][1], p[i+1][j+1][2]     );

					glTexCoord3f(p[i+1][j][0], p[i+1][j][1], p[i+1][j][2]);
					glVertex3f(p[i+1][j][0], p[i+1][j][1], p[i+1][j][2]);
				glEnd();
//				glDisable(GL_BLEND);

//				glColor4f(1.0, 0.0, 1.0, 1.0);
//				glBegin(GL_LINES);
//					glVertex3f(p[i][j][0], p[i][j][1], p[i][j][2]);
//					glVertex3f(p[i][j+1][0], p[i][j+1][1], p[i][j+1][2]);
//					glVertex3f(p[i+1][j][0], p[i+1][j][1], p[i+1][j][2]);
//					glVertex3f(p[i+1][j+1][0], p[i+1][j+1][1], p[i+1][j+1][2]     );
//				glEnd();
			}
//	glDisable(GL_TEXTURE_3D);

	}

		pst[0] = cyl.surf(pvalue, 45.0);
		pst[1] = cyl.surf(pvalue, 90.0);
		pst[2] = cyl.surf(pvalue, 135.0);
		pst[3] = cyl.surf(pvalue, 180.0);
		pst[4] = cyl.surf(pvalue, 225.0);
		pst[5] = cyl.surf(pvalue, 270.0);
		pst[6] = cyl.surf(pvalue, 315.0);
		pst[7] = cyl.surf(pvalue, 360.0);
	
	} else {
	for(int i = 0; i < p.size()-1; i++)
	{
			for(int j = 0; j < p[0].size()-1; j++)
			{
//				glColor4f(1.0, 1.0, 0.0, 0.5);
//				glEnable(GL_BLEND);
//				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glColor4f(1.0, 1.0, 0.0, 0.5);
				glEnable(GL_BLEND);
				if(j == 0)
				{
				glPushMatrix();
					glTranslatef(p[i][j][0], p[i][j][1], p[i][j][2]);
//					rot = spidey.getRotation(dr);
//					glRotatef(rot[0], rot[1], rot[2], rot[3]);
					glScalef(10, 10, 10);
					glColor3f(0.0, 1.0, 0.0);
		//			glutSolidSphere(0.1,10,10);
				glPopMatrix();	
				}
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glBegin(GL_QUADS);
					glVertex3f(p[i][j][0], p[i][j][1], p[i][j][2]);

					glVertex3f(p[i][j+1][0], p[i][j+1][1], p[i][j+1][2]);

					glVertex3f(p[i+1][j+1][0], p[i+1][j+1][1], p[i+1][j+1][2]     );

					glVertex3f(p[i+1][j][0], p[i+1][j][1], p[i+1][j][2]);
				glEnd();
				glDisable(GL_BLEND);

				glColor4f(1.0, 0.0, 1.0, 1.0);                                                                                                                                   
				glLineWidth(2.0);
				glBegin(GL_LINES);
					glVertex3f(p[i][j][0], p[i][j][1], p[i][j][2]);
					glVertex3f(p[i][j+1][0], p[i][j+1][1], p[i][j+1][2    ]);
					glVertex3f(p[i][j][0], p[i][j][1], p[i][j][2]);
					glVertex3f(p[i+1][j][0], p[i+1][j][1], p[i+1][j][2    ]     );
				glEnd();

//				glColor4f(1.0, 0.0, 1.0, 1.0);
//				glBegin(GL_LINES);
//					glVertex3f(p[i][j][0], p[i][j][1], p[i][j][2]);
//					glVertex3f(p[i][j+1][0], p[i][j+1][1], p[i][j+1][2]);
//					glVertex3f(p[i+1][j][0], p[i+1][j][1], p[i+1][j][2]);
//					glVertex3f(p[i+1][j+1][0], p[i+1][j+1][1], p[i+1][j+1][2]     );
//				glEnd();
			}
//	glDisable(GL_TEXTURE_3D);

	}
//		cyl.print(0);
//		cyl.print(1);
//		cyl.print(2);
//		cyl.print(3);
//		cyl.print(4);

		glPushMatrix();
//			stim::vec<float> ps;
//			ps = cyl.surf(0.5, 90.0);
			stim::vec3<float> ps = cyl.surf(0.5, 90.0);
			glTranslatef(ps[0], ps[1], ps[2]);
			glScalef(10.0, 10.0, 10.0);
			glColor3f(1.0, 0.0, 0.0);
			glutSolidSphere(0.1,10,10);
		glPopMatrix();	

		glPushMatrix();
			ps = cyl.surf(1.0, 90.0);
			std::cout << ps << std::endl;
			glTranslatef(ps[0], ps[1], ps[2]);
			glScalef(10.0, 10.0, 10.0);
			glColor3f(1.0, 0.0, 0.0);
			glutSolidSphere(0.1,10,10);
		glPopMatrix();	

		glPushMatrix();
			ps = cyl.surf(pvalue, 45.0);
			glTranslatef(ps[0], ps[1], ps[2]);
			glScalef(10.0, 10.0, 10.0);
			glColor3f(1.0, 0.0, 0.0);
			glutSolidSphere(0.1,10,10);
		glPopMatrix();	

		glPushMatrix();
			ps = cyl.surf(pvalue, 90.0);
			glTranslatef(ps[0], ps[1], ps[2]);
			glScalef(10.0, 10.0, 10.0);
			glColor3f(1.0, 0.0, 0.0);
			glutSolidSphere(0.1,10,10);
		glPopMatrix();	

		glPushMatrix();
			ps = cyl.surf(pvalue, 135.0);
			glTranslatef(ps[0], ps[1], ps[2]);
			glScalef(10.0, 10.0, 10.0);
			glColor3f(1.0, 0.0, 0.0);
			glutSolidSphere(0.1,10,10);
		glPopMatrix();	

		glPushMatrix();
			ps = cyl.surf(pvalue, 180.0);
			glTranslatef(ps[0], ps[1], ps[2]);
			glScalef(10.0, 10.0, 10.0);
			glColor3f(1.0, 0.0, 0.0);
			glutSolidSphere(0.1,10,10);
		glPopMatrix();	

		glPushMatrix();
			ps = cyl.surf(pvalue, 225.0);
			glTranslatef(ps[0], ps[1], ps[2]);
			glScalef(10.0, 10.0, 10.0);
			glColor3f(1.0, 0.0, 0.0);
			glutSolidSphere(0.1,10,10);
		glPopMatrix();	

		glPushMatrix();
			ps = cyl.surf(pvalue, 270.0);
			glTranslatef(ps[0], ps[1], ps[2]);
			glScalef(10.0, 10.0, 10.0);
			glColor3f(1.0, 0.0, 0.0);
			glutSolidSphere(0.1,10,10);
		glPopMatrix();	

		glPushMatrix();
			ps = cyl.surf(pvalue, 315.0);
			glTranslatef(ps[0], ps[1], ps[2]);
			glScalef(10.0, 10.0, 10.0);
			glColor3f(1.0, 0.0, 0.0);
			glutSolidSphere(0.1,10,10);
		glPopMatrix();	

		glPushMatrix();
			ps = cyl.surf(pvalue, 360.0);
			glTranslatef(ps[0], ps[1], ps[2]);
			glScalef(10.0, 10.0, 10.0);
			glColor3f(1.0, 0.0, 0.0);
			glutSolidSphere(0.1,10,10);
		glPopMatrix();	
	}



}


void
DrawSphere()
{
	int vang;
	int ang;
	int delang = 5;
	float r0 = 0.2;
	float x0, y0, z0, x1, y1, z1, x2, z2;
	
	glEnable(GL_TEXTURE_3D);
	glBindTexture(GL_TEXTURE_3D, texID);
	glBegin(GL_QUADS);
	for (vang = 0; vang <= 180; vang+=delang){
		y0=r0*cos((double)(vang)*stim::TAU/360.0);
     		y1=r0*cos((double)(vang+delang)*stim::TAU/360.0);
     		x0=r0*sin((double)vang*stim::TAU/360.0);
     		z0=0.0; 	
		for (ang=0;ang<=360;ang+=delang)
		     {
			x1=r0*cos((double)ang*stim::TAU/360.0)*sin((double)vang*stim::TAU/360.0);
			x2=r0*cos((double)ang*stim::TAU/360.0)*sin((double)(vang+delang)*stim::TAU/360.0);
			z1=r0*sin((double)ang*stim::TAU/360.0)*sin((double)vang*stim::TAU/360.0);
			z2=r0*sin((double)ang*stim::TAU/360.0)*sin((double)(vang+delang)*stim::TAU/360.0);
	//		glTexCoord3f(x0,y0,z0);
			glVertex3f(x0,y0,z0);
	//		glTexCoord3f(x1,y0,z1);
			glVertex3f(x1,y0,z1);
	//		glTexCoord3f(x1,y0,z1);
			glVertex3f(x1,y0,z1);
	//		glTexCoord3f(x2,y1,z2);
			glVertex3f(x2,y1,z2);
			x0=x1;
			z0=z1;
		     }
		  }
     glEnd(); 
}

	
void
DrawPlanes()
{
	glEnable(GL_TEXTURE_3D);
	glBindTexture(GL_TEXTURE_3D, texID);
	p1 = spidey.ver.p(1,1);
	p2 = spidey.ver.p(1,0);
	p3 = spidey.ver.p(0,0);
	p4 = spidey.ver.p(0,1);
	//glPushName(VERTICAL);
	glBegin(GL_QUADS);
		glTexCoord3f(
			p1[0]/a,
			p1[1]/b,
			p1[2]/c
			);
		glVertex3f(
			p1[0],
			p1[1],
			p1[2]
			);
		glTexCoord3f(
			p2[0]/a,
			p2[1]/b,
			p2[2]/c
			);
		glVertex3f(
			p2[0],
			p2[1],
			p2[2]
			);
		glTexCoord3f(
			p3[0]/a,
			p3[1]/b,
			p3[2]/c
			);
		glVertex3f(
			p3[0],
			p3[1],
			p3[2]
			);
		glTexCoord3f(
			p4[0]/a,
			p4[1]/b,
			p4[2]/c
			);
		glVertex3f(
			p4[0],
			p4[1],
			p4[2]
			);
	glEnd();
	//glPopName();
	p1 = spidey.hor.p(1,1);
	p2 = spidey.hor.p(1,0);
	p3 = spidey.hor.p(0,0);
	p4 = spidey.hor.p(0,1);
	//glPushName(HORIZONTAL);
	glBegin(GL_QUADS);
		glTexCoord3f(
			p1[0]/a,
			p1[1]/b,
			p1[2]/c
			);
		glVertex3f(
			p1[0],
			p1[1],
			p1[2]
			);
		glTexCoord3f(
			p2[0]/a,
			p2[1]/b,
			p2[2]/c
			);
		glVertex3f(
			p2[0],
			p2[1],
			p2[2]
			);
		glTexCoord3f(
			p3[0]/a,
			p3[1]/b,
			p3[2]/c
			);
		glVertex3f(
			p3[0],
			p3[1],
			p3[2]
			);
		glTexCoord3f(
			p4[0]/a,
			p4[1]/b,
			p4[2]/c
			);
		glVertex3f(
			p4[0],
			p4[1],
			p4[2]
			);
	glEnd();
	//glPopName();
	glBindTexture(GL_TEXTURE_3D, 0);
	glDisable(GL_TEXTURE_3D);
}
void
DrawSpiders()
{
	glEnable(GL_TEXTURE_3D);
	glBindTexture(GL_TEXTURE_3D, texID);
	p1 = spidey.hor.p(1,1);
	p2 = spidey.hor.p(1,0);
	p3 = spidey.hor.p(0,0);
	p4 = spidey.hor.p(0,1);
	glBegin(GL_QUADS);
		glTexCoord3f(
			p1[0]/a,
			p1[1]/b,
			p1[2]/c
			);
		glVertex2f(0.0,0.0);
		glTexCoord3f(
			p2[0]/a,
			p2[1]/b,
			p2[2]/c
			);
		glVertex2f(1.0, 0.0);
		glTexCoord3f(
			p3[0]/a,
			p3[1]/b,
			p3[2]/c
			);
		glVertex2f(1.0, 2.0);
		glTexCoord3f(
			p4[0]/a,
			p4[1]/b,
			p4[2]/c
			);
		glVertex2f(0.0, 2.0);
	glEnd();
	p1 = spidey.ver.p(1,1);
	p2 = spidey.ver.p(1,0);
	p3 = spidey.ver.p(0,0);
	p4 = spidey.ver.p(0,1);
	
	glBegin(GL_QUADS);
		glTexCoord3f(
			p1[0]/a,
			p1[1]/b,
			p1[2]/c
			);
		glVertex2f(1.0, 0.0);
		glTexCoord3f(
			p2[0]/a,
			p2[1]/b,
			p2[2]/c
			);
		glVertex2f(2.0, 0.0);
		glTexCoord3f(
			p3[0]/a,
			p3[1]/b,
			p3[2]/c
			);
		glVertex2f(2.0, 2.0);
		glTexCoord3f(
			p4[0]/a,
			p4[1]/b,
			p4[2]/c
			);
		glVertex2f(1.0, 2.0);
	glEnd();
	glBindTexture(GL_TEXTURE_3D, 0);
	glDisable(GL_TEXTURE_3D);
	p1 = spidey.hor.p(1,1);
	p2 = spidey.hor.p(1,0);
	p3 = spidey.hor.p(0,0);
	p4 = spidey.hor.p(0,1);
	}

//render cube	

void
artificialCylinder()
{
	DrawCylinder(Ps, Ms, 0);
}

void
renderScene()
{	
	glViewport(0, 0, size[0], size[1]);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, GL[0],0.0, GL[1], 0.0, GL[2]);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_TEXTURE);          //these lines must be added in order to make the correct viewing of the texture matrix when getSample is called in gl_spider
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glInitNames();
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(450.0,450.0,0.0);
	glScalef(4.0, 4.0, 4.0);
	if(!artificial){
		stim::vec3<float> pos = spidey.getPosition();
		cam.LookAt(pos[0], pos[1], pos[2]);
		p = cam.getPosition();
		up = cam.getUp();
		d = cam.getLookAt();
		gluLookAt(p[0], p[1], p[2], d[0], d[1], d[2], up[0], up[1], up[2]);  
	} else {
		cam.LookAt(Ps[0][0], Ps[0][1], Ps[0][2]);
		p = cam.getPosition();
		up = cam.getUp();
		d = cam.getLookAt();
		gluLookAt(p[0], p[1], p[2], d[0], d[1], d[2], up[0], up[1], up[2]);  
	}
	DrawCube();
	//glTranslatef(78.0,273.0, 93.0);
	DrawPlanes();
	stim::vec3<float> ps = spidey.getPosition();
	stim::vec3<float> dr = spidey.getDirection();
	stim::vec3<float> mg = spidey.getMagnitude();
	stim::vec<float> rot = spidey.getRotation(dr);
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
		glScalef(1.0/a, 1.0/b, 1.0/c);	
		glTranslatef(ps[0], ps[1], ps[2]);
		glRotatef(rot[0], rot[1], rot[2], rot[3]);
		glScalef(mg[0], mg[0], mg[0]);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glTranslatef(ps[0], ps[1], ps[2]);
		glRotatef(rot[0], rot[1], rot[2], rot[3]);
		glScalef(mg[0], mg[0], mg[0]);
		DrawCylinder(14);
	glPopMatrix();
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	std::stack< stim::vec3<float> > sds = spidey.getSeeds();
//	net = spidey.getGLNetwork();
	for(int i = id; i < sl.size(); i++)
	{
		ps = sl[i];
		dr = sv[i];
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_3D);
		glDisable(GL_LIGHTING);
		glPushMatrix();
			glTranslatef(ps[0], ps[1], ps[2]);
			rot = spidey.getRotation(dr);
			glRotatef(rot[0], rot[1], rot[2], rot[3]);
			glScalef(mg[0], mg[0], mg[0]);
			glColor3f(1.0, 0.0, 0.0);
			glutSolidSphere(0.1,10,10);
		glPopMatrix();	
	}
	if(!minimize){	
		glDisable(GL_TEXTURE_3D);
		for(int i = 0; i < pst.size(); i++){
			glPushMatrix();
				glTranslatef(pst[i][0], pst[i][1], pst[i][2]);
				glScalef(10.0, 10.0, 10.0);
				glColor3f(0.0, 0.125*i, 1.0);
				glutSolidSphere(0.1,10,10);
			glPopMatrix();	
		}
	}
/*	for(int i = 0; i < id; i++)
	{
		ps = sl[i];
		dr = sv[i];
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_3D);
		glDisable(GL_LIGHTING);
		glPushMatrix();
			glTranslatef(ps[0], ps[1], ps[2]);
			rot = spidey.getRotation(dr);
			glRotatef(rot[0], rot[1], rot[2], rot[3]);
			glScalef(mg[0], mg[0], mg[0]);
			glColor3f(0.0, 1.0, 0.0);
			glutSolidSphere(0.1,10,10);
		glPopMatrix();	
	}
*/
	for(int i = 0; i < net.sizeV(); i++)
	{
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_3D);
		glDisable(GL_LIGHTING);
		glPushMatrix();
			stim::vec3<float> a = net.V[i].getPosition();
			glTranslatef(a[0], a[1], a[2]);
			rot = spidey.getRotation(dr);
			glRotatef(rot[0], rot[1], rot[2], rot[3]);
			glScalef(mg[0], mg[0], mg[0]);
			glColor3f(0.0, 0.0, 1.0);
			glutSolidSphere(0.1,10,10);

			//label the nodes with numbers.
			float curTrans[16];
			stim::matrix<float, 4> cT;
			glGetFloatv(GL_MODELVIEW_MATRIX, curTrans);
			cT.set(curTrans);
			stim::vec<float> loc(0.0,0.0,0.0);
			loc = cT*loc;
			ostringstream ss;
			ss<<i<<" "<<net.V[i].edges_to_str();// << " "<<net.V[i].str();
			glColor3f(1., 1., 0.0);
			glRasterPos3f(loc[0]+0.2, loc[1]+0.2, loc[2]+0.2);
			glutBitmapString(GLUT_BITMAP_HELVETICA_18,
			 (const unsigned char* )(ss.str().c_str()));
		glPopMatrix();	
	}

	for (int i = 0; i < net.sizeE(); i++)
	{
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_3D);
		glDisable(GL_LIGHTING);
		glPushMatrix();
			std::vector<stim::vec3<float> > tmp = net.getEdgeCenterLine(i);
			stim::vec3<float> a = tmp[tmp.size()/2];
			glTranslatef(a[0], a[1], a[2]);
			rot = spidey.getRotation(dr);
			glRotatef(rot[0], rot[1], rot[2], rot[3]);
			glScalef(mg[0], mg[0], mg[0]);
			glColor3f(1.0, 0.0, 1.0);

			float curTrans[16];
			stim::matrix<float, 4> cT;
			glGetFloatv(GL_MODELVIEW_MATRIX, curTrans);
			cT.set(curTrans);
			stim::vec<float> loc(0., 0., 0.);
			loc = cT*loc;
			ostringstream ss;
			ss<<i; // << net.nodes_to_str(i);
			glRasterPos3f(loc[0]+0.2, loc[1]+0.2, loc[2]+0.2);
			glutBitmapString(GLUT_BITMAP_HELVETICA_18, 
				(const unsigned char*)(ss.str().c_str()));
		glPopMatrix();

		glPushMatrix();
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0.0, GL[0],0.0, GL[1], 0.0, GL[2]);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity(); 
			glTranslatef(450.0,450.0,0.0);
			glScalef(4.0, 4.0, 4.0);
			stim::vec3<float> pos = spidey.getPosition();
			cam.LookAt(pos[0], pos[1], pos[2]);
			p = cam.getPosition();
			up = cam.getUp();
			d = cam.getLookAt();
			gluLookAt(p[0], p[1], p[2], d[0], d[1], d[2], up[0], up[1], up[2]);  
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			glScalef(1.0/::a, 1.0/b, 1.0/c); 
			if(!minimize && (i == net.sizeE()-1))
				DrawCylinder(net.getEdgeCenterLine(i), net.getEdgeCenterLineMag(i));
			CHECK_OPENGL_ERROR
			glDisable(GL_TEXTURE_3D);
			glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}
//	net.createFromSelf();
	net.Render();	
	if(!minimize){
		net.RenderCylinders();
	}
	if(artificial){
		artificialCylinder();
	}
//	skeleton.createFromSelf();
//	skeleton.Render();
//	for(int i = 0; i < net.E.size(); i++)
//	{
//		glColor3f(0.15*i, 0.0, 0.0); 
//		skeleton.RenderLine(net.E[i].centerline());
//	}	


	//DrawCylinder();
		//glEnable(GL_TEXTURE_2D);


		//glBindTexture(GL_TEXTURE_2D, texID);
		//glBindFramebuffer(GL_FRAMEBUFFER, fboId);
		//glFramebufferTexture2D(
		//		GL_FRAMEBUFFER,
		//		GL_COLOR_ATTACHMENT0,
		//		GL_TEXTURE_2D,
		//		texID2,
		//		0
		//		);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID2, 0);
		//glBindFramebuffer(GL_FRAMEBUFFER, fboId);
		//GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
		//glDrawBuffers(1, DrawBuffers);
		//if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		//	std::cout << "damn" << std::endl;
		//glBindTexture(GL_TEXTURE_2D, texID2);
		//glClearColor(1,1,1,1);
	//glClear(GL_COLOR_BUFFER_BIT);
	//glLoadIdentity();
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CHECK_OPENGL_ERROR
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if(minimize == false){
		glViewport(0,0,400,200);
		gluOrtho2D(0.0, 2.0, 0.0, 2.0);
	//glTranslatef(2.5,2.5,0.0);
	//glScalef(2.0, 2.0, 2.0);
		DrawSpiders();
	//glPopMatrix();
	//DrawSpiders2();
	//glFlush();
	//glFinish();
		CHECK_OPENGL_ERROR
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, spidey.getFB());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0,0,16,218, 0, 0, 16, 218, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}
	CHECK_OPENGL_ERROR
	//glBindFramebuffer(GL_FRAMEBUFFER,0);
	//glBindTexture(GL_TEXTURE_3D, texID);
	//glGenerateMipmap(GL_TEXTURE_3D);
	//glBindTexture(GL_TEXTURE_3D, 0);
	//glViewport(0,0, 800,800);

	//GLint curbuf;
	//glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &curbuf); 
	//std::cout << curbuf << std::endl;

	//glViewport(0,0,600,600);
	//glBindFramebuffer(GL_FRAMEBUFFER, fboId);
	
	//glDrawBuffer(GL_FRAMEBUFFER0);
	//glDrawBuffer(fboId);
	//glClearColor(1,1,1,1);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glLoadIdentity();
	//std::cout << fboId << std::endl;
	//DrawSpiders();
	
	//glBlitFramebuffer(0.0,0.0,1,1,0.0,0.0,1,1, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	//gluOrtho2D(-5.0, -3.0, -5.0, -1.0);
	//glClearColor(1,1,1,1);
	//glBindTexture(GL_TEXTURE_3D, texID);
	//glPushMatrix();
	//glGenerateMipmap(GL_TEXTURE_3D);
	//glBindTexture(GL_TEXTURE_3D, 0);
	//glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &curbuf); 
	//std::cout << curbuf << std::endl;

//	glCopyImageSubData(fboId, GL_FRAMEBUFFER, 0, 0, 0, 0, GL_FRONT_AND_BACK, GL_NONE, 0, 0, 0, 0, 20, 20, 0);
	
	
	glutSwapBuffers();
}


void
MouseButton(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON)
	{
	 	button1 	 = (state == GLUT_DOWN) ? true : false;
		button_shift     = glutGetModifiers();
		prevmousePos[0]	 = (float)x;
		prevmousePos[1]	 = (float)y; 
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		branches << "Branch" << "\n";
		branches << spidey.getPosition() << "\n"
			<< spidey.getDirection() << "\n"
			<< spidey.getMagnitude() << "\n";
		branches << "\n";
		
		
	}
}

void MouseMotion(int x, int y)
{
	if(button1 && !button_shift)
		{	
			mousePos[0] = (prevmousePos[0] - (float) x)*0.00005;
			mousePos[1] = ((float)y - prevmousePos[1])*0.00005;
			prevmousePos[0] = (float)x;
			prevmousePos[1] = (float)y;
			cam.OrbitFocus(mousePos[0]*degtorad, mousePos[1]*degtorad);
		}
	if(button1 && button_shift)
		{
			mousePos[0] = (prevmousePos[0] - (float) x)*0.00001;
			mousePos[1] = ((float)y - prevmousePos[1])*0.00001;
			prevmousePos[0] = (float)x;
			prevmousePos[1] = (float)y;
			Parker.Pan(mousePos[0]*degtorad);
			Parker.Tilt(mousePos[1]*degtorad);
			spidey.setDirection(Parker.getDirection());
			spidey.Update();
			std::cout << Parker.getLookAt() << std::endl;
		}
}

void
idleFunction()
{
	tick += 10;
	glutPostRedisplay();
}

void
printCost()
{
	
}


void
processSpecialKeys(int key, int xx, int yy)
{
switch(key) {
	case GLUT_KEY_UP:
//	    oriTex[2] = oriTex[2]+adjustTex;
//	    if (oriTex[2] > org[1]){
//		oriTex[2] = org[1];
//	    }
//	    else{
//		oriDrw[2] = oriDrw[2] + adjustDrw;
//	    }
		pvalue += 0.001;
		std::cerr << pvalue << std::endl;
	    if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
		{
			stim::vec3<float> temp =	spidey.getPosition();
			temp[1] += 1;
			spidey.setPosition(temp);
			spidey.Update();
		} 
	    if (glutGetModifiers() == GLUT_ACTIVE_CTRL)
		{
			//stim::vec<float> temp = Parker.getDirection();
			//temp[1] += 0.001;
			Parker.Pan(0.01);
			spidey.setDirection(Parker.getDirection());
			spidey.Update();
		}
	    break;
	case GLUT_KEY_DOWN:
//	    oriTex[2] = oriTex[2]-adjustTex;
//	    if (oriTex[2] < org[0]){
//              oriTex[2] = org[0];
//	    }
//	    else{
//		oriDrw[2] = oriDrw[2] - adjustDrw;
//	    }
		pvalue -= 0.01;
		std::cerr << pvalue << std::endl;
	    if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
		{
			stim::vec3<float> temp =	spidey.getPosition();
			temp[1] -= 1;
			spidey.setPosition(temp);
			spidey.Update();
		}
	    if (glutGetModifiers() == GLUT_ACTIVE_CTRL)
		{
			//stim::vec<float> temp = Parker.getDirection();
			//temp[1] -= 0.001;
			Parker.Pan(0.01);
			spidey.setDirection(Parker.getDirection());
			spidey.Update();
		}
	    break;
	case GLUT_KEY_LEFT:
//	    oriTex[1] = oriTex[1]+adjustTex;
//	    if (oriTex[1] > org[1]){
//		oriTex[1] = org[1];
//          }
//	    else{
//		oriDrw[1] = oriDrw[1] + adjustDrw;
//	    }
	    if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
		{
			stim::vec3<float> temp =	spidey.getPosition();
			temp[0] += 1;
			spidey.setPosition(temp);
			spidey.Update();
		} 
	    if (glutGetModifiers() == GLUT_ACTIVE_CTRL)
		{
			//stim::vec<float> temp = Parker.getDirection();
			//temp[0] += 0.001;
			Parker.Tilt(0.01);
			spidey.setDirection(Parker.getDirection());
			spidey.Update();
		}
	    break;
	case GLUT_KEY_RIGHT:
//	    oriTex[1] = oriTex[1]-adjustTex;
//	    if (oriTex[1] < org[0]){
//                oriTex[1] = org[0];
//            }
//	    else{
//		oriDrw[1] = oriDrw[1] - adjustDrw;
//	    }
	    if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
		{
			stim::vec3<float> temp =	spidey.getPosition();
			temp[0] -= 1;
			spidey.setPosition(temp);
			spidey.Update();
		} 
	    if (glutGetModifiers() == GLUT_ACTIVE_CTRL)
		{
			//stim::vec<float> temp = Parker.getDirection();
			//temp[0] -= 0.001;
			Parker.Tilt(-0.01);
			spidey.setDirection(Parker.getDirection());
			spidey.Update();
		}
	    break;
	}	
	glutPostRedisplay();
}


void
QUIT()
{
	skeleton.End();
	skeleton.save("Skeleton.obj");
	stim::glObj<float>skeleton_final;
	for(int i = 0; i < net.sizeE(); i++)
	{
		std::vector<stim::vec< float > > cm = net.getEdgeCenterLineMag(i);
		std::vector<stim::vec3< float > > ce = net.getEdgeCenterLine(i);
		skeleton_final.Begin(stim::OBJ_LINE);
		for(int j = 0; j < ce.size(); j++)
		{
			skeleton.TexCoord(cm[j][0]);
			skeleton.Vertex(ce[j][0], ce[j][1], ce[j][2]);
		}
		skeleton_final.End();
	}
	skeleton.save("Final.obj");
	branches.close();
	exit(0);
}

void
processKeys(unsigned char key, int x, int y)
{
	//ESC key
        if (key == 27)
	{
	     duration = (std::clock() - start)/ (double) CLOCKS_PER_SEC;
	     std::cout << duration << " seconds" << std::endl;
	     QUIT();
        //     exit(0);
	}
	//Space bar
	if (key == 32)
	{
			std::cout <<id<<":"<< sl[id][0] << ", " << sl[id][1]  << ", "  << sl[id][2] << std::endl;
			std::cout <<id<<":"<< sv[id][0] << ", " << sv[id][1]  << ", "  << sv[id][2] << std::endl;
			std::cout <<id<<":"<< sm[id] << std::endl;
		skeleton.Begin(stim::OBJ_LINE);
		tracingLine = true;
//		spidey.Bind();
		spidey.clearCurrent();
		spidey.traceLine(spidey.getPosition(),
			 spidey.getMagnitude(), 200);
//		spidey.printSizes();
//		std::cerr << "got here" << std::endl;
/*		if(tracingLine){
			std::vector<stim::vec<float> > ce = a.first.centerline();
			std::vector<stim::vec<float> > cm = a.first.centerlinemag();
			if(ce.size() > 2)
			{
				for(int i = 0; i < ce.size(); i++)
				{
					skeleton.TexCoord(cm[i][0]);
					skeleton.Vertex(ce[i][0], ce[i][1], ce[i][2]);
				}
				//add edge() to the network.
				if(a.second == -1)
				{
					std::cout << "got here" << std::endl;
					net.addEdge(ce, cm, -1, -1);
				}
				else if(a.second != -1)
				{
					std::cout << "got here1" << std::endl;
					net.addEdge(ce, cm, -1, a.second);
				}
			}
		}	*/
		while(!spidey.Empty()){
			sl.push_back(spidey.getLastSeed());
			sv.push_back(spidey.getLastSeedVec());
			sm.push_back(spidey.getLastSeedMag());
			spidey.popSeed();
		}
//		spidey.Update();	
		skeleton.End();
		tracingLine = false;
		id++;
		spidey.setPosition(sl[id][0], sl[id][1], sl[id][2]);
		spidey.setDirection(sv[id][0], sv[id][1], sv[id][2]);
		spidey.setMagnitude(sm[id]);
		net = spidey.getGLNetwork();
		net.createFromSelf();
		net.createCylinders();
		glutPostRedisplay();
		
//		spidey.setMagnitude(16.0);
//		spidey.Update();
//		std::cout << "stuff1" << std::endl;
			
		
	/*
//		spidey.Bind();
		int cost = spidey.StepP();
		stim::vec<float> m = spidey.getMagnitude();
		stim::vec<float> p = spidey.getPosition();
		if(tracingLine){
			skeleton.TexCoord(m[0]);
			skeleton.Vertex(p[0], p[1], p[2]);
		}
		stim::vec<float> pz = spidey.getPosition();
		spidey.Update();
		std::cerr << "got before" << std::endl;
		while(!spidey.Empty()){
			sl.push_back(spidey.getLastSeed());
			sv.push_back(spidey.getLastSeedVec());
			spidey.popSeed();
		}	
		std::cerr << "got after" << std::endl;
	     	std::cerr << cost << ":" << spidey.getMagnitude()  << ":" << pz << std::endl;
	*/
	}


	//ENTERKEY
	if(key == 13)
	{
		std::cout << "Took a step" << std::endl;
		stim::vec3<float> temp = spidey.getPosition();
		printf("P: %f %f %f \n", temp[0], temp[1], temp[2]);
		temp = spidey.getDirection();
		printf("D: %f %f %f \n", temp[0], temp[1], temp[2]);
		temp = spidey.getMagnitude();
		printf("P: %f \n", temp[0]);
		spidey.Step();
		spidey.Update();
	}

	//m key
	if(key == 109)
	{
		if(minimize == false)
			minimize = true;
		else
			minimize = false;	
	}
	
	// plus key	
	if (key == 43)
	{
		stim::vec<float> temp = spidey.getMagnitude();
		temp = temp + 0.1;
		spidey.setMagnitude(temp);
		spidey.Update();
	}   
	// minus key
	if (key == 45)
	{
		stim::vec<float> temp = spidey.getMagnitude();
		temp = temp - 0.1;
		spidey.setMagnitude(temp);
		spidey.Update();
	}
	if (key == 8)
	{
		//	std::cout <<id<<":"<< sl[id][0] << ", " << sl[id][1]  << ", "  << sl[id][2] << std::endl;
		//	std::cout <<id<<":"<< sv[id][0] << ", " << sv[id][1]  << ", "  << sv[id][2] << std::endl;
		//int cost = spidey.Step();
		spidey.setPosition(sl[id][0], sl[id][1], sl[id][2]);
		spidey.setDirection(sv[id][0], sv[id][1], sv[id][2]);
		spidey.setMagnitude(sm[id]);
//		spidey.setMagnitude(16.0);
		spidey.Update();
		id++;
	}

	if (key == 47)
	{
		if(!tracingLine){
			std::cout << "I have begin a line" << std::endl;
			skeleton.Begin(stim::OBJ_LINE);
			tracingLine = true;
		} else {
			std::cout << "Please End previous line" << std::endl;
		}
	}
	if (key == 92)
	{
		if(tracingLine){
			std::cout << "I have ended a line" << std::endl;
			skeleton.End();
			tracingLine = false;
			
		} else {
			std::cout << "No line being traced, please start a line" << std::endl;
		}
	}
	// "P" key
	if (key == 80)
	{
		net = spidey.getGLNetwork();
		net.to_csv();
		//net.to_gdf();
	}
	// "T" key
	if (key == 84)
	{
		while(id != sl.size())
		{
			skeleton.Begin(stim::OBJ_LINE);
			tracingLine = true;
			spidey.clearCurrent();
			spidey.traceLine(spidey.getPosition(),
				 spidey.getMagnitude(), 200);
//			spidey.printSizes();
			if(id == sl.size()-1)
				break;
			while(!spidey.Empty()){
				sl.push_back(spidey.getLastSeed());
				sv.push_back(spidey.getLastSeedVec());
				sm.push_back(spidey.getLastSeedMag());
				spidey.popSeed();
			}
			spidey.Update();	
			skeleton.End();
			tracingLine = false;
			id++;
			spidey.setPosition(sl[id][0], sl[id][1], sl[id][2]);
			spidey.setDirection(sv[id][0], sv[id][1], sv[id][2]);
			spidey.setMagnitude(sm[id]);
			spidey.Update();
//			std::cout <<" " << std::endl;
//			std::cout << sl.size() << ":" << id << std::endl;
//			std::cout <<" " << std::endl;
		}
		net = spidey.getGLNetwork();
		net.createFromSelf();
		net.createCylinders();
		glutPostRedisplay();
//		net.to_csv();
//		net.to_gdf();
	} 

	///a key
	if (key == 97)
	{
		if(!artificial == true && Ps.size() == 0)
		{		
/*
			Ps.push_back(stim::vec<float>(0.0,0.0,0.0));
			Ps.push_back(stim::vec<float>(0.0,40.0,0.0));
			Ps.push_back(stim::vec<float>(80.0,40.0,0.0));
			Ps.push_back(stim::vec<float>(80.0,0.0,0.0));
			Ms.push_back(stim::vec<float>(10,5));
			Ms.push_back(stim::vec<float>(5,6));
			Ms.push_back(stim::vec<float>(15,6));
			Ms.push_back(stim::vec<float>(5,6)); 
*/
			Ps.push_back(stim::vec<float>(0.0,0.0,0.0));
			Ps.push_back(stim::vec<float>(0.0,40.0,80.0));
			Ps.push_back(stim::vec<float>(40.0,80.0,40.0));
			Ps.push_back(stim::vec<float>(80.0,40.0,20.0));
			Ps.push_back(stim::vec<float>(80.0,0.0,0.0));
			Ms.push_back(stim::vec<float>(10,5));
			Ms.push_back(stim::vec<float>(5,6));
			Ms.push_back(stim::vec<float>(15,6));
			Ms.push_back(stim::vec<float>(5,6));
			Ms.push_back(stim::vec<float>(10,6));

			artificial = true;
		} else if(!artificial == true){
			artificial = true;
		} else {
			artificial = false;
		}
	}
/*	float S[4] = {0.0, 0.6, 0.6, 2.0};
	std::cout << "DEBUG: Main::338" << std::endl;                        
			    std::cout << "pos: " << spidey.getPosition() << std::endl;
			    std::cout << "dir: " << spidey.getDirection() << std::endl;
			    std::cout << "mag: " << spidey.getMagnitude() << std::endl;
			    std::cout << "After Transformation" << std::endl;
			std::cout << "[" << (p1[0])/512/S[1] << ", "
                                   <<(p1[1])/512/S[2] << ", "
                                   <<(p1[2])/426/S[3] << "]" << ":"
                                   << "[" << (p2[0])/512/S[1] << ", " 
                                   <<  (p2[1])/512/S[2] << ", "
                                   <<  (p2[2])/426/S[3] << "]" << ":"
                                   << "[" << (p3[0])/512/S[1] << ", " 
                                   <<  (p3[1])/512/S[2] << ", "
                                   <<  (p3[2])/426/S[3] << "]" << ":"
                                   << "[" << (p4[0])/512/S[1] << ", " 
                                   <<  (p4[1])/512/S[2] << ", "
                                   <<  (p4[2])/426/S[3] << "]" << std::endl;   */     
	//stim::vec<float> tempPos = spidey.getPosition();
	//stim::vec<float> tempDir = spidey.getDirection();
	//stim::vec<float> tempMag = spidey.getMagnitude();
	//spidey.setPosition(tempPos[0]+-tempDir[0]*tempMag[0]/2,
	//			tempPos[1]+-tempDir[1]*tempMag[0]/2,
	//			tempPos[2]+-tempDir[2]*tempMag[0]/2);
	     //spidey.findOptimalDirection();
	//spidey.Update();

	   
}


void 
changeSize(int w, int h)
{
        glViewport(0,0,w,h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
	glOrtho(-5.0, 2.0,-5.0, 2.0, -0.0, 1000.0);
	//gluPerspective(90, 4.0/3.0, 0.1, 100.0);
        glMatrixMode(GL_MODELVIEW);
}


int
main(int argc, char **argv)
{
	CHECK_OPENGL_ERROR
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(size[0], size[1]);
	glutCreateWindow("gl_texture");

	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);
        glutReshapeFunc(changeSize);
        glutDisplayFunc(renderScene);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);

	GLenum err = glewInit();
	if (GLEW_OK != err)
		{
			std::cerr << "Failed" << std::endl;
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		}
//	stim::gl_texture<unsigned char> stack ("/media/pavel/Storage/Data/Test_out/4_3/");
	stim::gl_texture<unsigned char> stack ("/home/pavel/Documents/Test_Data/");
//	stim::gl_texture<unsigned char> stack ("/media/pavel/Storage/Data/Partitioned/2008_ii_Clean_parts/4_4/a0-1023");
	stack.setTexParam(GL_LINEAR, GL_CLAMP_TO_EDGE, GL_UNSIGNED_BYTE, GL_LUMINANCE);
	//stack.setTexParam(GL_LINEAR, GL_CLAMP_TO_EDGE, GL_FLOAT, GL_LUMINANCE);
//	stack.setDimensions(VOXDIMS[0],VOXDIMS[1],VOXDIMS[2]);
	//D = stack.getDimensions();
	stack.createTexture();
	texID = stack.getTexture();
	start = std::clock();


	glInit();
	glutIdleFunc(idleFunction);
	glutMainLoop();
	return 0;
}


