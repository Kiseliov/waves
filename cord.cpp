#include <vector>
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <stdlib.h>
#define PI 3.1415926535
// git test
using namespace std;

///////////////////////////////////////////////////// MAGIC!!!

int left_dot = 50, right_dot = 600;

struct Imp
{
	int heights[1660];
	/*const */int height = 100;

	float x, speed;
	int width;
	bool to_right;

		Imp(int x, bool to_right, int width)
		{
		    this->width = width;

		    for (int i = 0; i <= width; i++)
            {
                float delta = i / (float)width * PI - PI / 2;
                heights[i] = height * cos(delta);
		    }

			this->x = x;
			speed = 0.3;
			this->to_right = to_right;

			if (!to_right)
				speed *= -1;
		}
};

vector<Imp>vector_imp;							// vector of perturbations
int dots[1660];									// vector of dots

int compare(const void* a, const void* b)
{
    return int(0.5 + ((Imp*)a) -> x) - int(0.5 + ((Imp*)b) -> x);
}
//////////////////////////////////////////////////// enter
void keyb(unsigned char key, int x, int y)
{
	if (key == 27) exit(0);
};

void mouse_button_click(int button, int state, int x, int y )
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		Imp temp(x, true, 40);
		vector_imp.push_back(temp);
	};
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		Imp temp(x, false, 40);
		vector_imp.push_back(temp);
	};
	if(button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)exit(0);
}

void sum_imps( )
{
	for(int i = 0; i<=1660; i++)
	{
		dots[i]=0;									// Clear dots
	}
	for (vector<Imp>::iterator it = vector_imp.begin(); it != vector_imp.end(); it++)
	{
	    for (int i = 0; i < it -> width; i++)
        {
            int dot = (int) (it->x + 0.5 + i - it->width / 2);
            if (dot < 0) return;

            dots[dot] += (it->heights)[i] * ((it -> speed) > 0 == it -> to_right ? 1 : -1);
        }
	}
}

void draw_wave()     						// TODO DRAW TOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOODOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOootooooooo
{


	for(int i = left_dot; i <= right_dot; i++)
	{
		glVertex2f(i, dots[i]);
	}
}
/////////////////////////////////////////////////// paint
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0, 200, 0);
	glBegin(GL_LINE_STRIP);
        glColor3f(0, 1, 1);    // colorising
        glVertex2f(left_dot, 0); // left dot
        // Drawing
        draw_wave();
        glVertex2f(right_dot, 0); // right dot
    glEnd();

	glutSwapBuffers();
}

void timf(int value)
{
	glutPostRedisplay();
	   // Move imps
   for (vector<Imp>::iterator it = vector_imp.begin(); it != vector_imp.end(); it++)
	{
		float ix = it -> x;
		float is = it -> speed;

	    if ((is > 0 && ix + is < right_dot)
			|| (is < 0 && ix + is > left_dot))
		{
	    	ix += is;
	    } else
		{
	        is *= -1;
    	}

    	it -> speed = is;
    	it -> x = ix;
    }
    void* v = (void*) &vector_imp[0];
       qsort(v, vector_imp.size(), sizeof(Imp), compare); 											// sort

    sum_imps();

	glutTimerFunc(1, timf, 0);
}



int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(800,800);
	glutCreateWindow("D&B");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);


	glutTimerFunc(1, timf, 0);
	glutKeyboardFunc(keyb);
	glutMouseFunc(mouse_button_click);

	vector_imp.clear();

	glutMainLoop();
	return 0;
}
