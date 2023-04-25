#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <iostream>
using namespace std;
//==========================================================================================
//Variables
//==========================================================================================
//Color of the shapes DEFAULT BLUE
float redColor = 0.0;
float greenColor = 0.0;
float blueColor = 1.0;
//Special xtranslation for letters to be space out.
float k_xtranslation = -0.75;
float e1_xtranslation = -0.55;
float n1_xtranslation = -0.35;
float n2_xtranslation = -0.17;
float e2_xtranslation = 0.00;
float t_xtranslation = 0.20;
float h_xtranslation = 0.40;
//Counter for color change.
int counter = 0;
//counter for idle animations
int counter2 = 0;
//Input for translation. Helping to move the name and boarder when user use WASD keys
float y_translation = 0.0;
float x_translation = 0.0;
//Input for Scaling. Helping increasing the size of the object when the user use the mouse button left, right, and middle 
float scaling = 1.0;
//For y idle animation
float y_place = 0; //record the last place y_translation is.
float y_animate_speed = 0.0001;
//=================================================================================================
// CALLBACKS
//=================================================================================================

//-----------------------------------------------------------------------------
// CALLBACK DOCUMENTATION
// https://www.opengl.org/resources/libraries/glut/spec3/node45.html
// http://freeglut.sourceforge.net/docs/api.php#WindowCallback
//-----------------------------------------------------------------------------

void idle_func()
{
    /* 
    //Test code for color changing idle animation. 
    //NOTE hurts eyes if you add very low value upwards.
    if (redColor > 1)
    {
        redColor = 0;
    }
    if (blueColor > 1) {
        blueColor = 0;
    }
    if (greenColor > 0) {
        greenColor = 1;
    }
    redColor = redColor + 0.5;
    greenColor = greenColor - 0.5;
    blueColor = blueColor + 0.5;
    glutPostRedisplay();
    */

    
    y_translation += y_animate_speed;
    if (y_translation >= y_place+0.5 || y_translation <= y_place-0.5) {
        y_animate_speed = -y_animate_speed;
    }

    //0.6 is upper boarder
    // -0.9 is lower boarder
    //-0.15 for left x boarder
    //0.4 for right x boarder 
    
    //Reading for X and Y coordinated for the boarder of name 
    //cout << "Y: " << y_translation<< endl;
    //cout << "X: " << x_translation << endl;
    
    glutPostRedisplay();
}

void reshape_func( int width, int height )
{
	glViewport( 0, 0, width, height );
	glutPostRedisplay();
}

void keyboard_func(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
	{
        //movement for the y direction UPWARDS
		y_translation = y_translation + 0.01;

        //Recording last Y_cord for idle animation
        y_place = y_translation;

        //Boarder LIMIT for orignal size of the Boarder and Name 
        if (y_translation >= (0.6 * (1/scaling)) )
        {
            y_translation = 0.6 * (1/scaling);
        }
		break;
	}

	case 'a':
	{
        //movement for the x direction to the LEFT <---
		x_translation = x_translation - 0.01;

        //Boarder LIMIT for orignal size of the Boarder and Name 
        if (x_translation <= -0.15 * (1 / scaling))
        {
            x_translation = -0.15 * (1 / scaling);
        }
		break;
	}

	case 's':
	{
        //movement for the y direction DOWNWARDS
		y_translation = y_translation - 0.01;

        //Recording last Y_cord for idle animation
        y_place = y_translation;

        //Boarder LIMIT for orignal size of the Boarder and Name 
        if (y_translation <= - 0.93 * (1 / scaling))
        {
            y_translation = -0.93 * (1 / scaling);
        }
		break;
	}

	case 'd':
	{
        //movement for the x direction to the RIGHT --->
		x_translation = x_translation + 0.01;
        //Boarder LIMIT for orignal size of the Boarder and Name
        if (x_translation >= 0.4 * (1 / scaling))
        {
            x_translation = 0.4 * (1 / scaling);
        }
		break;
	}

	// Exit on escape key press
	case '\x1B':
	{
		exit(EXIT_SUCCESS);
		break;
	}

	case 'c':
	{
		counter++;
		if (counter == 1)
		{
			redColor = 1;
			greenColor = 0;
			blueColor = 0;
		}
		if (counter == 2)
		{
			redColor = 0;
			greenColor = 1;
			blueColor = 0;
		}
		if (counter == 3)
		{
			redColor = 1;
			greenColor = 1;
			blueColor = 0;
		}
		if (counter == 4)
		{
			redColor = 1;
			greenColor = 0;
			blueColor = 1;
		}
		if (counter == 5)
		{
			redColor = 0;
			greenColor = 1;
			blueColor = 1;
		}
		if (counter == 6)
		{
			redColor = 1;
			greenColor = 1;
			blueColor = 1;
		}
		if (counter == 7)
		{
			redColor = 0;
			greenColor = 0;
			blueColor = 0;
		}
		if (counter == 8)
		{
			redColor = 0;
			greenColor = 0;
			blueColor = 1;
			counter = 0;
		}
		break;
	}
	case 'h':
	{
        cout << "==============================================================" << endl;
        cout << "Movement control for the name: " << endl;
        cout << "w to move up" << endl;
		cout << "s to move down" << endl;
		cout << "a to move left" << endl;
		cout << "d to move right" << endl;
        cout << "==============================================================" << endl;
        cout << "Changing the color of the name: " << endl;
		cout << "c to change color" << endl;
        cout << "==============================================================" << endl;
        cout << "Zooming the name: " << endl;
        cout << "Left mouse button to zoom in" << endl;
        cout << "Right mouse button to zoom out" << endl;
        cout << "==============================================================" << endl;
        cout << "Flip the name: " << endl;
        cout << "middle mouse button to Flip" << endl;
        cout << "==============================================================" << endl;
        cout << "Exiting the system: " << endl;
		cout << "esc to exit" << endl;
        cout << "==============================================================" << endl;
        cout << "Idle animation is automatic. The name and boarder will go up and down." << endl;
        cout << "==============================================================" << endl;
		break;
	}
	}
	glutPostRedisplay();
}

void mouse_func( int button, int state, int x, int y )
{
    //Noting if the GLUT_LEFT_BUTTON isnt defined use the number
    // 0 = Left mouse button
    // 1 = Middle mouse button
    // 2 = Right mouse button
    if (button == GLUT_LEFT_BUTTON) {
        //Note if state is when button is pressed or not
        //When button press = 0
        //When button release = 1
        if (state == 0) {
            scaling = scaling* 2;
        }
       
    }
    if (button == GLUT_RIGHT_BUTTON) {
        if (state == 0) {
            scaling = scaling / 2;
        }

    }
    if (button == GLUT_MIDDLE_BUTTON)
    {
        if (state == 0) {
            scaling = scaling * -1;
        }
    }
}

//=================================================================================================
// RENDERING
//=================================================================================================

void display_func(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Begin top boarder for name
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (- 0.80 + x_translation) * scaling, 
        (0.35 + y_translation) * scaling );
    glVertex2f( (- 0.80 + x_translation) * scaling, 
        (0.4 + y_translation) * scaling);
    glVertex2f( (0.55 + x_translation) * scaling, 
        (0.35 + y_translation) * scaling);
    glEnd();

    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.55 + x_translation) * scaling, 
        (0.35 + y_translation) * scaling);
    glVertex2f( (0.55 + x_translation) * scaling, 
        (0.4 + y_translation) * scaling);
    glVertex2f( (- 0.80 + x_translation) * scaling, 
        (0.4 + y_translation) * scaling);
    glEnd();
    //=====END of creating top of the boarder for name

    //Begin top boarder for name
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (- 0.80 + x_translation) * scaling, 
        (- 0.02 + y_translation) * scaling);
    glVertex2f( (- 0.80 + x_translation) * scaling, 
        (- 0.07 + y_translation) * scaling);
    glVertex2f( (0.55 + x_translation) * scaling, 
        (- 0.02 + y_translation) * scaling);
    glEnd();

    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.55 + x_translation) * scaling, 
        (- 0.02 + y_translation) * scaling);
    glVertex2f( (0.55 + x_translation) * scaling, 
        (-0.07 + y_translation)* scaling);
    glVertex2f( (- 0.80 + x_translation) * scaling, 
        (- 0.07 + y_translation) * scaling);
    glEnd();
    //===== END for creating bottom of the boarder for name

    //Begin Right boarder for name
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (- 0.80 + x_translation) * scaling, 
        (- 0.07 + y_translation) * scaling);
    glVertex2f( (- 0.85 + x_translation) * scaling, 
        (- 0.07 + y_translation) * scaling);
    glVertex2f( (- 0.80 + x_translation) * scaling, 
        (0.4 + y_translation) * scaling);
    glEnd();

    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (- 0.80 + x_translation) * scaling, 
        (0.4 + y_translation) * scaling);
    glVertex2f( (- 0.85 + x_translation) * scaling, 
        (0.4 + y_translation) * scaling);
    glVertex2f( (- 0.85 + x_translation) * scaling, 
        (- 0.07 + y_translation) * scaling);
    glEnd();
    //===== END for creating right of the boarder for name

    //Begin left boarder for name
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.60 + x_translation) * scaling, 
        (- 0.07 + y_translation) * scaling);
    glVertex2f( (0.55 + x_translation) * scaling, 
        (- 0.07 + y_translation) * scaling);
    glVertex2f( (0.60 + x_translation) * scaling, 
        (0.4 + y_translation) * scaling);
    glEnd();

    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.55 + x_translation) * scaling, 
        (0.4 + y_translation) * scaling);
    glVertex2f( (0.60 + x_translation) * scaling, 
        (0.4 + y_translation) * scaling);
    glVertex2f( (0.55 + x_translation) * scaling, 
        (- 0.07 + y_translation) * scaling);
    glEnd();
    //===== END for creating left of the boarder for name

    //=====BEGIN of K Creating=========
    //Straight line for K Part one Bottom Triangle
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.0 + k_xtranslation + x_translation) * scaling, 
        (0.0 + y_translation) * scaling);
    glVertex2f( (0.025 + k_xtranslation + x_translation) * scaling, 
        (0.3 + y_translation) * scaling);
    glVertex2f( (0.025 + k_xtranslation + x_translation) * scaling, 
        (0.0 + y_translation)* scaling);
    glEnd();

    //Straight line for K Part 2 Upper triangle
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.0 + k_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glVertex2f( (0.0 + k_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glVertex2f( (0.025 + k_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glEnd();

    //triangle for line angle upper K
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.025 + k_xtranslation + x_translation)* scaling, 
        (0.15 + y_translation)* scaling);
    glVertex2f( (0.15 + k_xtranslation + x_translation)* scaling, 
        (0.30 + y_translation)* scaling);
    glVertex2f( (0.15 + k_xtranslation + x_translation)* scaling, 
        (0.20 + y_translation)* scaling);
    glEnd();

    //triangle for line angle downward K
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.15 + k_xtranslation + x_translation)* scaling, 
        (0.09 + y_translation)* scaling);
    glVertex2f( (0.15 + k_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glVertex2f( (0.025 + k_xtranslation + x_translation)* scaling, 
        (0.15 + y_translation)* scaling);
    glEnd();

    //====END of K creating====
    //=======================================================================

    //=============BEGIN E number 1 Creating===========
    //triangle for bottom triangle straight E
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.0 + e1_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glVertex2f( (0.025 + e1_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glVertex2f( (0.025 + e1_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glEnd();
    //triangle for top triangle straight E
    glColor3f(redColor, greenColor, blueColor + y_translation);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.0 + e1_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glVertex2f( (0.0 + e1_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glVertex2f( (0.025 + e1_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glEnd();

    //triangle for upper line E Part 1 TOP
    glColor3f(redColor, greenColor, blueColor + y_translation);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.025 + e1_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glVertex2f( (0.025 + e1_xtranslation + x_translation)* scaling, 
        (0.275 + y_translation)* scaling);
    glVertex2f( (0.15 + e1_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glEnd();

    //triangle for upper line E PART 2 Bottom
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.025 + e1_xtranslation + x_translation)* scaling, 
        (0.275 + y_translation)* scaling);
    glVertex2f( (0.15 + e1_xtranslation + x_translation)* scaling, 
        (0.275 + y_translation)* scaling);
    glVertex2f( (0.15 + e1_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glEnd();

    //triangle for bottom line E Part 1 TOP
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.025 + e1_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glVertex2f( (0.025 + e1_xtranslation + x_translation)* scaling, 
        (0.025 + y_translation)* scaling);
    glVertex2f( (0.15 + e1_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glEnd();

    //triangle for bottom line E PART 2 Bottom
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.025 + e1_xtranslation + x_translation)* scaling, 
        (0.025 + y_translation)* scaling);
    glVertex2f( (0.15 + e1_xtranslation + x_translation)* scaling, 
        (0.025 + y_translation)* scaling);
    glVertex2f( (0.15 + e1_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glEnd();

    //triangle for MIDDLE line E Part 1 TOP
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.025 + e1_xtranslation + x_translation)* scaling, 
        (0.175 + y_translation)* scaling);
    glVertex2f( (0.025 + e1_xtranslation + x_translation)* scaling, 
        (0.150 + y_translation)* scaling);
    glVertex2f( (0.15 + e1_xtranslation + x_translation)* scaling, 
        (0.175 + y_translation)* scaling);
    glEnd();

    //triangle for MIDDLE line E PART 2 Bottom
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.025 + e1_xtranslation + x_translation)* scaling, 
        (0.150 + y_translation)* scaling);
    glVertex2f( (0.15 + e1_xtranslation + x_translation)* scaling, 
        (0.150 + y_translation)* scaling);
    glVertex2f( (0.15 + e1_xtranslation + x_translation)* scaling, 
        (0.175 + y_translation)* scaling);
    glEnd();
    //====== END of Creating E number 1 =======
    //=================================================================

    //============BEGIN N one Creating ============
    //triangle for bottom triangle straight Part one for N Left side
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.0 + n1_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glVertex2f( (0.025 + n1_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glVertex2f( (0.025 + n1_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glEnd();

    //triangle for top triangle straight Part two for N Left side
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.0 + n1_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glVertex2f( (0.0 + n1_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glVertex2f( (0.025 + n1_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glEnd();

    //triangle for bottom triangle straight Part one for N right side
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.1 + n1_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glVertex2f( (0.125 + n1_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glVertex2f( (0.125 + n1_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glEnd();

    //triangle for top triangle straight Part two for N right side
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.1 + n1_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glVertex2f( (0.1 + n1_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glVertex2f( (0.125 + n1_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glEnd();

    //triangle for bottom angle Part one for N middle
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.025 + n1_xtranslation + x_translation)* scaling, 
        (0.25 + y_translation)* scaling);
    glVertex2f( (0.025 + n1_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glVertex2f( (0.1 + n1_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glEnd();

    //triangle for top angle part two for N middle
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.1 + n1_xtranslation + x_translation)* scaling, 
        (0.05 + y_translation)* scaling);
    glVertex2f( (0.025 + n1_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glVertex2f( (0.1 + n1_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glEnd();

    //======= END OF N one Creating =======
    //======================================================================================


    //==========BEGIN N two CREATING =====
    //triangle for bottom triangle straight Part one for N Left side
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.0 + n2_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glVertex2f( (0.025 + n2_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glVertex2f( (0.025 + n2_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glEnd();

    //triangle for top triangle straight Part two for N Left side
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.0 + n2_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glVertex2f( (0.0 + n2_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glVertex2f( (0.025 + n2_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glEnd();

    //triangle for bottom triangle straight Part one for N right side
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.1 + n2_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glVertex2f( (0.125 + n2_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glVertex2f( (0.125 + n2_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glEnd();

    //triangle for top triangle straight Part two for N right side
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.1 + n2_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glVertex2f( (0.1 + n2_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glVertex2f( (0.125 + n2_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glEnd();

    //triangle for bottom angle Part one for N middle
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.025 + n2_xtranslation + x_translation)* scaling, 
        (0.25 + y_translation)* scaling);
    glVertex2f( (0.025 + n2_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glVertex2f( (0.1 + n2_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glEnd();

    //triangle for top angle part two for N middle
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.1 + n2_xtranslation + x_translation)* scaling, 
        (0.05 + y_translation)* scaling);
    glVertex2f( (0.025 + n2_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glVertex2f( (0.1 + n2_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glEnd();
    //================ END of N two Creating ==============
    //===========================================================================


    //========BEGIN E two Creating ==========
    //triangle for bottom triangle straight E two
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.0 + e2_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glVertex2f( (0.025 + e2_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glVertex2f( (0.025 + e2_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glEnd();
    //triangle for top triangle straight E two
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.0 + e2_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glVertex2f( (0.0 + e2_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glVertex2f( (0.025 + e2_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glEnd();

    //triangle for upper line E two Part 1 TOP
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.025 + e2_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glVertex2f( (0.025 + e2_xtranslation + x_translation)* scaling, 
        (0.275 + y_translation)* scaling);
    glVertex2f( (0.15 + e2_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glEnd();

    //triangle for upper line E two PART 2 Bottom
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.025 + e2_xtranslation + x_translation)* scaling, 
        (0.275 + y_translation)* scaling);
    glVertex2f( (0.15 + e2_xtranslation + x_translation)* scaling, 
        (0.275 + y_translation)* scaling);
    glVertex2f( (0.15 + e2_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glEnd();

    //triangle for bottom line E two Part 1 TOP
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.025 + e2_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glVertex2f( (0.025 + e2_xtranslation + x_translation)* scaling, 
        (0.025 + y_translation)* scaling);
    glVertex2f( (0.15 + e2_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glEnd();

    //triangle for bottom line E two PART 2 Bottom
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.025 + e2_xtranslation + x_translation)* scaling, 
        (0.025 + y_translation)* scaling);
    glVertex2f( (0.15 + e2_xtranslation + x_translation)* scaling, 
        (0.025 + y_translation)* scaling);
    glVertex2f( (0.15 + e2_xtranslation + x_translation)* scaling,
        (0.0 + y_translation)* scaling);
    glEnd();

    //triangle for MIDDLE line E two Part 1 TOP
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.025 + e2_xtranslation + x_translation)* scaling, 
        (0.175 + y_translation)* scaling);
    glVertex2f( (0.025 + e2_xtranslation + x_translation)* scaling, 
        (0.150 + y_translation)* scaling);
    glVertex2f( (0.15 + e2_xtranslation + x_translation)* scaling, 
        (0.175 + y_translation)* scaling);
    glEnd();

    //triangle for MIDDLE line E two PART 2 Bottom
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.025 + e2_xtranslation + x_translation)* scaling, 
        (0.150 + y_translation)* scaling);
    glVertex2f( (0.15 + e2_xtranslation + x_translation)* scaling, 
        (0.150 + y_translation)* scaling);
    glVertex2f( (0.15 + e2_xtranslation + x_translation)* scaling, 
        (0.175 + y_translation)* scaling);
    glEnd();
    //===============END OF E two Creating ==========
    //========================================================================================

    //BEGIN T
    //triangle for bottom triangle straight t part 1
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.075 + t_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glVertex2f( (0.1 + t_xtranslation + x_translation)* scaling, 
        (0.275 + y_translation)* scaling);
    glVertex2f( (0.1 + t_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glEnd();

    //triangle for top triangle straight t part 2
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.075 + t_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glVertex2f( (0.075 + t_xtranslation + x_translation)* scaling, 
        (0.275 + y_translation)* scaling);
    glVertex2f( (0.1 + t_xtranslation + x_translation)* scaling, 
        (0.275 + y_translation)* scaling);
    glEnd();

    //Top of the T part one
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.0 + t_xtranslation + x_translation)* scaling, 
        (0.275 + y_translation)* scaling);
    glVertex2f( (0.0 + t_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glVertex2f( (0.175 + t_xtranslation + x_translation)* scaling, 
        (0.275 + y_translation)* scaling);
    glEnd();
    //Top of the T part two
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.175 + t_xtranslation + x_translation)* scaling, 
        (0.275 + y_translation)* scaling);
    glVertex2f( (0.175 + t_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glVertex2f( (0.0 + t_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glEnd();

    //============End of creating T ========
    //=======================================================================================

    //=======BEGIN Creating H =======
     //triangle for bottom triangle straight t part one right
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.0 + h_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glVertex2f( (0.025 + h_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glVertex2f( (0.025 + h_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glEnd();
    //triangle for top triangle straight t part two left
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.0 + h_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glVertex2f( (0.0 + h_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glVertex2f( (0.025 + h_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glEnd();

    //triangle for bottom triangle straight h part one right
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.1 + h_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glVertex2f( (0.125 + h_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glVertex2f( (0.125 + h_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glEnd();
    //triangle for top triangle straight h part two right
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.1 + h_xtranslation + x_translation)* scaling, 
        (0.0 + y_translation)* scaling);
    glVertex2f( (0.1 + h_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glVertex2f( (0.125 + h_xtranslation + x_translation)* scaling, 
        (0.3 + y_translation)* scaling);
    glEnd();

    //triangle for MIDDLE line H two Part 1 TOP
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.025 + h_xtranslation + x_translation)* scaling, 
        (0.175 + y_translation)* scaling);
    glVertex2f( (0.025 + h_xtranslation + x_translation)* scaling, 
        (0.150 + y_translation)* scaling);
    glVertex2f( (0.1 + h_xtranslation + x_translation)* scaling, 
        (0.175 + y_translation)* scaling);
    glEnd();

    //triangle for MIDDLE line H two PART 2 Bottom
    glColor3f(redColor, greenColor, blueColor);
    glBegin(GL_TRIANGLES);
    glVertex2f( (0.025 + h_xtranslation + x_translation)* scaling, 
        (0.150 + y_translation)* scaling);
    glVertex2f( (0.1 + h_xtranslation + x_translation)* scaling, 
        (0.150 + y_translation)* scaling);
    glVertex2f( (0.1 + h_xtranslation + x_translation)* scaling, 
        (0.175 + y_translation)* scaling);
    glEnd();

    //======== END of creating H ======
    //===============================================================================================


    glFlush();

    glutSwapBuffers();
}

//=================================================================================================
// INIT
//=================================================================================================

void init( void )
{
	// Print some info
	std::cout << "Vendor:         " << glGetString( GL_VENDOR   ) << "\n";
	std::cout << "Renderer:       " << glGetString( GL_RENDERER ) << "\n";
	std::cout << "OpenGL Version: " << glGetString( GL_VERSION  ) << "\n\n";

	// Set the background color
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	std::cout << "Finished initializing...\n\n";
}

//=================================================================================================
// MAIN
//=================================================================================================

int main( int argc, char** argv )
{
	glutInit( &argc, argv );

	glutInitWindowPosition( 100, 100 );
	glutInitWindowSize( 800, 600 );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

	glutCreateWindow( "CSE-170 Computer Graphics" );

	glutDisplayFunc( display_func );
	glutIdleFunc( idle_func );
	glutReshapeFunc( reshape_func );
	glutKeyboardFunc( keyboard_func );
	glutMouseFunc( mouse_func );

	init();

	glutMainLoop();

	return EXIT_SUCCESS;
}
