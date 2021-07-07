#ifndef STIM_GLNETWORK_H
#define STIM_GLNETWORK_H

#include <GL/glew.h>
#include <GL/glut.h>
#include "network.h"
#include <stim/visualization/cylinder.h>
#include <stim/math/vector.h>
#include <list>
#include <ANN/ANN.h>


namespace stim
{
template <typename T>
class glnetwork : public virtual network<T>
{
private:
	using stim::network<T>::E;

	GLuint dList;
	GLuint cList;

	void initl()
	{
		if(glIsList(dList))
			glDeleteLists(dList, 1);
		dList = glGenLists(1);
		glListBase(dList);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

	}

	void initc()
	{
		if(glIsList(cList))
			glDeleteLists(cList, 1);
		cList = glGenLists(1);

		glListBase(cList);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	void
	Create(GLenum mode)
	{
		glListBase(dList);
		glNewList(dList, GL_COMPILE);
		glLineWidth(3.5);
		for(int i = 0; i < E.size(); i++)
		{
			if(mode == GL_SELECT)
			{
//				glLineWidth(3.5);
				glLoadName(i);
			}
			else{
//				glLineWidth(1.0+1.0*i);
			}
			if(i < 1)
				glColor3f(1.0,0.0,0.0);
			else
				glColor3f(0.0, 1.0, 0.0);
			std::vector<stim::vec3<T> > line = getEdgeCenterLine(i);
			glBegin(GL_LINE_STRIP);
			for(int j = 0; j < line.size(); j++)
			{
				glVertex3f(line[j][0],
					line[j][1],
					line[j][2]);
			}
			glEnd();
		}
		glEndList();
//		CHECK_OPENGL_ERROR
	}
	
	void
	CreateCyl(GLenum mode, int sides = 8)
	{

		glListBase(cList);
		glNewList(cList, GL_COMPILE);

		for(int i = 0; i < E.size(); i++)
		{
			if(mode == GL_SELECT)
			{
				glLoadName(i);
			}
			glColor3f(1.0, 1.0, 0.0);
			std::vector<stim::vec3<T> > line = getEdgeCenterLine(i);
			std::vector< T > linemag = getEdgeCenterLineMag(i);
			for(int i = 0; i < linemag.size(); i++)
				linemag[i] = linemag[i]/3;
			stim::cylinder<T> cyl(line, linemag);
			std::vector<std::vector<stim::vec3<T > > > p = cyl.getPoints(sides);
			for(int i = 0; i < p.size()-1; i++)
			{
				for(int j = 0; j < p[0].size()-1; j++)
				{
					glColor4f(1.0, 1.0, 0.0, 0.5);
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glBegin(GL_QUADS);
						glVertex3f(p[i][j][0], p[i][j][1], p[i][j][2]);
						glVertex3f(p[i][j+1][0], p[i][j+1][1], p[i][j+1][2    ]);
						glVertex3f(p[i+1][j+1][0], p[i+1][j+1][1], p[i+1][    j+1][2]     );
						glVertex3f(p[i+1][j][0], p[i+1][j][1], p[i+1][j][2    ]);
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
				}

			}

			

		}                                                                                 
		glEndList();

	}
	
public:
	using stim::network<T>::sizeE;
	using stim::network<T>::getEdgeCenterLine;
	using stim::network<T>::getEdgeCenterLineMag;
	glnetwork()
	{

	}
	
	void
	createFromSelf(GLenum mode = GL_RENDER)
	{
		initl();
		Create(mode);
	}

	void
	createCylinders(GLenum mode = GL_RENDER, int sides = 8)
	{
		initc();
		CreateCyl(mode, sides);
	}

	void
	Render()
	{
		glCallList(dList);
//		CHECK_OPENGL_ERROR
	}

	void
	RenderCylinders()
	{
		glCallList(cList);
	}

	void
	RenderAll()
	{
		glCallList(dList);
		glCallList(cList);
	}

	void
	RenderLine(std::vector<stim::vec3<T> > l)
	{
                glColor3f(0.5, 1.0, 0.5);
                glLineWidth(3.0);
                glBegin(GL_LINE_STRIP);
                for(int j = 0; j < l.size(); j++){
                        glVertex3f(
                                l[j][0],
                                l[j][1],
                                l[j][2]
                                );
                }
                glEnd();
	}
	
};
}

#endif
