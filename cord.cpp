#include <vector>
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <stdlib.h>
#define PI 3.1415926535
// git test
using namespace std;

///////////////////////////////////////////////////// CONSTATS
int dx1 = 0, dx2 = 0, dx3 = 0, dx4 = 0, dy = 0;								// two constants for marker, pls dont touch them I know they looking ugly, but it works
int left_dot = 200, right_dot = 1400, height_of_cord = 400;
bool pause = false, key0 = false;
struct Imp
{
	int heights[1660];
	int height;
	float x, speed;
	float frequency;
	bool to_right;
	int width;

		Imp(int x, bool to_right, float frequency, int height)
		{
		    this -> frequency = frequency;
		    speed = 0.3;
			this -> width = speed / frequency;
		    for (int i = 0; i <= width; i++)
            {
                float delta = i / (float)width * PI - PI / 2;
                heights[i] = height * cos(delta);
		    }
			this -> height = height;
			this -> x = x;
			this -> to_right = to_right;

			if (!to_right)
				speed *= -1;
		}
};

vector<Imp>vector_imp;							// vector of perturbations
vector<Imp>vector_standing;						// temp vector while generating standing wave
int dots[1660];									// vector of dots

int compare(const void* a, const void* b)
{
    return int(0.5 + ((Imp*)a) -> x) - int(0.5 + ((Imp*)b) -> x);
}
//////////////////////////////////////////////////// keyboard
void keyb(unsigned char key, int x, int y)
{
	if (key == 27) exit(0);
	if (key == 13) vector_imp.clear();
	if (key == 32) pause = !pause;
	cout << key;
	if (key0 && key != '0') vector_standing.clear();
	if (key0 = (key == '0'))
	{
		if( vector_standing.empty() ||
		(vector_standing.end() - 1)  -> x - left_dot >= (vector_standing.end() - 1)-> width)
		{
			Imp temp(x, true, 0.005, 100);
			vector_standing.push_back(temp);
			vector_imp.push_back(temp);
		}
	}	
};

void mouse_button_click(int button, int state, int x, int y )    /////// mouse clicks| generating waves
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)	// left button
	{
		if(y > height_of_cord) 
		{
			Imp temp(x, true, 0.00075, -y + 750 - height_of_cord);
			vector_imp.push_back(temp);
		}
		if(y <= height_of_cord) 
		{
			Imp temp(x, true, 0.00075, -y + 750 - height_of_cord);
			vector_imp.push_back(temp);
		}
	};
	
		if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) // right button
	{
		if(y > height_of_cord) 
		{
			Imp temp(x, false, 0.00075, -100);
			vector_imp.push_back(temp);
		}
		if(y <= height_of_cord) 
		{
			Imp temp(x, false, 0.00075, 100);
			vector_imp.push_back(temp);
		}
	};
}

void mouseMove(int x, int y) 									//////// MOUSE MOVING
{ 	
	dx1 = x - (0.3/0.00075)/2;
	dx2 = x + (0.3/0.00075)/2;
	
	dx3 = x - 20;
	dx4 = x + 20;
	dy = - y;	
}
//////////////////////////////////////////////////////////////////////// MAGIC starts here
void sum_imps( )										//////////////// summaraising imps
{
	for(int i = 0; i<=1660; i++)
	{
		dots[i]=0;									// Clear dots							
	}
	for (vector<Imp>::iterator it = vector_imp.begin(); it != vector_imp.end(); it++)		
	{
	    for (int i = 0; i < it -> width; i++)
        {
        	int dot = 0; 
        	if( i > right_dot)
        	{
            	dot = (int) (it->x + 0.5 + i - it->width / 2);
            	if (dot < 0) return;
            	dots[2*right_dot - dot] -= (it -> heights)[i] * ((it -> speed) );//> 0 == it -> to_right ? 1 : -1);
        	}//else if (i < left_dot){
		//	}
			else
			{
				dot = (int) (it->x + 0.5 + i - it->width / 2);
				if (dot < 0) return;
            	dots[dot] += (it -> heights)[i] * ((it -> speed) > 0 == it -> to_right ? 1 : -1);
			};

            //dots[dot] += (it -> heights)[i] * ((it -> speed) > 0 == it -> to_right ? 1 : -1);
        }
	}
}																								 

void draw_wave()     							/// drawing wave					
{


	for(int i = left_dot; i <= right_dot; i++)
	{
		glVertex2f(i, dots[i]);
	}
}
///////////////////////////////////////////////////  just reshape
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display()							///////////////// display 
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0, height_of_cord, 0);
	glBegin(GL_LINES);
		glColor3f(1,0,0);
		glVertex2f(dx1, 50);
		glVertex2f(dx1, -50);
		glVertex2f(dx2, 50);
		glVertex2f(dx2, -50);
	glEnd();
	
	glTranslatef(0, 750 - height_of_cord, 0 );
	glBegin(GL_LINES);
		glColor3f(1,0,0);
		glVertex2f(dx3, dy);
		glVertex2f(dx4, dy);
	glEnd();
	
	glTranslatef(0, -750 + height_of_cord, 0 );
	
	glBegin(GL_LINE_STRIP);					// Drawing wave
        glColor3f(0, 1, 1);    // colorising
        glVertex2f(left_dot, 0); // left dot
        draw_wave();
        glVertex2f(right_dot, 0); // right dot
    glEnd();

	glutSwapBuffers();
}

void timf(int value)					/////////// timf + a piece of MAGIC about moving imps for next step
{
	glutPostRedisplay();
	if(!pause)
	{
	   for (vector<Imp>::iterator it = vector_imp.begin(); it != vector_imp.end(); it++)		 // Move imps
		{																							///////////////////////// ERROR ABOUT ENDINGS HERE !!!!! I MUST FIX IT!!! FIIIIIIIIIIIIIIIIIIIIIIIIX
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
    	}																							///////// I SAID FIX!!! DONT IGNORE THIS PROBLEM!!!!!
    	void* v = (void*) &vector_imp[0];
    	qsort(v, vector_imp.size(), sizeof(Imp), compare); 											// sort
	}
	sum_imps();
	glutTimerFunc(1, timf, 0);
}



int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(1500,750);
	glutCreateWindow("D&B");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);


	glutTimerFunc(1, timf, 0);
	glutKeyboardFunc(keyb);
	glutMouseFunc(mouse_button_click);
	glutPassiveMotionFunc(mouseMove);

	vector_imp.clear();

	glutMainLoop();
	return 0;
}
