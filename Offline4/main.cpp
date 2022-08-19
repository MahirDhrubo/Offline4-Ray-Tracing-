#include<stdio.h>
#include<bits/stdc++.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <GL/glut.h>

#include "1705113_classes.h"

using namespace std;

#define pi (2*acos(0.0))
#define UP_KEY

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double l_angle,u_angle,r_angle;
double one_by_root_two;
double edge,radius,max_len;

int imageWidth, recursionLevel;
int objectCount, pointLightCount, spotLightCount;
vector<Object*> objects;
vector<PointLight*> pointLights;
vector<SpotLight*> spotLights;


struct point
{
	double x,y,z;
};

struct point pos, u, r, l;


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}




void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}


void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '2':
		    l.x = l.x*cos(pi/180) + r.x*sin(pi/180);
            l.y = l.y*cos(pi/180) + r.y*sin(pi/180);
            l.z = l.z*cos(pi/180) + r.z*sin(pi/180);

            r.x = r.x*cos(pi/180) - l.x*sin(pi/180);
            r.y = r.y*cos(pi/180) - l.y*sin(pi/180);
            r.z = r.z*cos(pi/180) - l.z*sin(pi/180);
			break;
        case '1':
            l.x = l.x*cos(pi/180) - r.x*sin(pi/180);
            l.y = l.y*cos(pi/180) - r.y*sin(pi/180);
            l.z = l.z*cos(pi/180) - r.z*sin(pi/180);

            r.x = r.x*cos(pi/180) + l.x*sin(pi/180);
            r.y = r.y*cos(pi/180) + l.y*sin(pi/180);
            r.z = r.z*cos(pi/180) + l.z*sin(pi/180);
			break;
        case '3':
            l.x = l.x*cos(pi/180) + u.x*sin(pi/180);
            l.y = l.y*cos(pi/180) + u.y*sin(pi/180);
            l.z = l.z*cos(pi/180) + u.z*sin(pi/180);

            u.x = u.x*cos(pi/180) - l.x*sin(pi/180);
            u.y = u.y*cos(pi/180) - l.y*sin(pi/180);
            u.z = u.z*cos(pi/180) - l.z*sin(pi/180);
            break;
        case '4':
            l.x = l.x*cos(pi/180) - u.x*sin(pi/180);
            l.y = l.y*cos(pi/180) - u.y*sin(pi/180);
            l.z = l.z*cos(pi/180) - u.z*sin(pi/180);

            u.x = u.x*cos(pi/180) + l.x*sin(pi/180);
            u.y = u.y*cos(pi/180) + l.y*sin(pi/180);
            u.z = u.z*cos(pi/180) + l.z*sin(pi/180);
            break;
        case '5':
            u.x = u.x*cos(pi/180) + r.x*sin(pi/180);
            u.y = u.y*cos(pi/180) + r.y*sin(pi/180);
            u.z = u.z*cos(pi/180) + r.z*sin(pi/180);

            r.x = r.x*cos(pi/180) - u.x*sin(pi/180);
            r.y = r.y*cos(pi/180) - u.y*sin(pi/180);
            r.z = r.z*cos(pi/180) - u.z*sin(pi/180);
            break;
        case '6':
            u.x = u.x*cos(pi/180) - r.x*sin(pi/180);
            u.y = u.y*cos(pi/180) - r.y*sin(pi/180);
            u.z = u.z*cos(pi/180) - r.z*sin(pi/180);

            r.x = r.x*cos(pi/180) + u.x*sin(pi/180);
            r.y = r.y*cos(pi/180) + u.y*sin(pi/180);
            r.z = r.z*cos(pi/180) + u.z*sin(pi/180);
            break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			 pos.x -= l.x;
			 pos.y -= l.y;
			 pos.z -= l.z;
			break;
		case GLUT_KEY_UP:		// up arrow key
			pos.x += l.x;
			 pos.y += l.y;
			 pos.z += l.z;
			break;
		case GLUT_KEY_RIGHT:
			pos.x += r.x;
			pos.y += r.y;
			pos.z += r.z;
			break;
		case GLUT_KEY_LEFT:
            pos.x -= r.x;
			pos.y -= r.y;
			pos.z -= r.z;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos.x += u.x;
			pos.y += u.y;
			pos.z += u.z;
			break;
		case GLUT_KEY_PAGE_DOWN:
			pos.x -= u.x;
			pos.y -= u.y;
			pos.z -= u.z;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
		    if(radius <= max_len && edge > 0){
                radius++;
                edge--;
		    }
			break;
		case GLUT_KEY_END:
		    if(radius > 0 && edge <= max_len){
                edge++;
                radius--;
		    }
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
//	gluLookAt(0,0,200,	0,0,0,	0,1,0);

    //gluLookAt(pos.x, pos.y, pos.z,	0,0,0,	0,1,0);
	gluLookAt(pos.x, pos.y, pos.z, pos.x + l.x, pos.y + l.y, pos.z + l.z, u.x, u.y, u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();


    //glColor3f(1,0,0);
    //drawSquare(10);

    Object *floor = new Floor(1000,20);
    floor->draw();
    for(int i = 0; i < objectCount; i++){
        objects[i]->draw();
    }

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//angle+=0.1;
	//codes for any changes in Models, Camera


	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=0;
	cameraHeight=150.0;
	cameraAngle=1.0;
	l_angle = 180;
	u_angle = 0;
	r_angle = 0;
	one_by_root_two = 1 / sqrt(2);
	pos = {50,50,0};
	u = {0,0,1}; /// y axis
	r = {-one_by_root_two,one_by_root_two,0}; /// x axis
	l = {-one_by_root_two, -one_by_root_two, 0}; /// z axis

    max_len = 18;
	edge = 12; //square side
	radius = max_len-edge; //sphere radius

    objectCount = 0;
	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

void loadData(){
    ifstream input;
    input.open("D:\\Academics\\4-1\\graphics_sessional\\Ray Tracing\\input.txt");
    if(!input.is_open()){
        cout << "Failed to load file" << endl;
        return;
    }

    input >> recursionLevel;
    input >> imageWidth;

    ///objects...
    input >> objectCount;
    cout << objectCount << endl;
    string shape;
    Object *object;
    double ambient, diffuse, specular, recurReflxCo;
    int shininess;
    for(int i = 0; i < objectCount; i++){
        input >> shape;
        cout << shape << endl;
        if(shape == "sphere"){
            Point center;
            double radius;

            input >> center.x >> center.y >> center.z;
            input >> radius;
            center.print();

            object = new Sphere(center, radius, 50, 50);
        }
        else if(shape == "triangle"){
            Point p[3];
            for(int i = 0; i < 3; i++)
                input >> p[i].x >> p[i].y >> p[i].z;
            object = new Triangle(p[0], p[1], p[2]);
        }
        else if(shape == "general"){
            double a,b,c,d,e,f,g,h,i,j;
            double length, width, height;
            Point refP;

            input >> a >> b >> c >> d >> e >> f>> g >> h >> i >>j;
            input >> refP.x >> refP.y >> refP.z;
            input >> length >> width >> height;

            object = new GeneralQuad(a,b,c,d,e,f,g,h,i,j,refP,length,width,height);
        }
        double a,b,c;
        input >> a >> b >> c;
        Color color(a,b,c);
        input >> ambient >> diffuse >> specular >> recurReflxCo;
        input >> shininess;

        object->setColor(color);
        double coefficients[4] = {ambient, diffuse, specular, recurReflxCo};
        object->setCoEfficients(coefficients);
        object->setShine(shininess);

        objects.push_back(object);
        objects[i]->color.print();
        //else if(shape == "")
        object = NULL;
    }

    /// point lights...
    input >> pointLightCount;
    cout << pointLightCount << endl;
    PointLight *pointLight;
    for(int i = 0; i < pointLightCount; i++){
        Point pos;
        Color color;
        input >> pos.x >> pos.y >> pos.z;
        input >> color.r >> color.g >> color.b;

        pointLight = new PointLight(pos, color);
        pointLights.push_back(pointLight);

        pointLight = NULL;
    }

    ///spot lights...
    input >> spotLightCount;
    cout << spotLightCount << endl;
    SpotLight *spotLight;
    for(int i = 0; i < spotLightCount; i++){
        Point pos,dir;
        Color color;
        double cutOffAngle;

        input >> pos.x >> pos.y >> pos.z;
        input >> dir.x >> dir.y >> dir.z;
        input >> color.r >> color.g >> color.b;

        input >> cutOffAngle;

        spotLight = new SpotLight(PointLight(pos,color),dir,cutOffAngle);

    }
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	loadData();

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);



	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
