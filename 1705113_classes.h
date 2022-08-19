#ifndef HEADER_FILE_NAME
#define HEADER_FILE_NAME

using namespace std;
#define pi (2*acos(0.0))

class Color{
    int r,g,b;
public:
    Color(int r=0, int g=0, int b=0){
        this->r = r;
        this->g = g;
        this->b = b;
    }

    int getR() {return r;}
    int getG() {return g;}
    int getB() {return b;}

    void operator = (const Color &color){
        this->r = color.r;
        this->g = color.g;
        this->b = color.b;
    }
};

class Point{
public:
    double x,y,z;
    Point(double x=0, double y=0, double z=0){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Point(const Point &point){
        this->x = point.x;
        this->y = point.y;
        this->z = point.z;
    }

    void operator = (const Point &point){
        this->x = point.x;
        this->y = point.y;
        this->z = point.z;
    }

    void print(string s=""){
        cout << s << endl;
        cout << "("<<x <<","<< y <<","<<z<<")"<<endl;
    }
};

class Object{
public:
    Point reference_point;
    double height, width, length;
    Color color;
    double coEfficients[4];

    int shine;
    Object(){
        reference_point = {0,0,0};
        height = width = length = 0.0;
        color = {0,0,0};
        coEfficients[0] = coEfficients[1] = coEfficients[2] = coEfficients[3] = 0;
    }
    Object(Point reference_point, double height, double width, double length, Color color , double coEfficients[4]){
        this->reference_point = reference_point;
        this->height = height;
        this->width = width;
        this->length = length;
        this->color = color;
        for(int i = 0; i < 3; i++) this->coEfficients[i] = coEfficients[i];
    }

    virtual void draw(){}

    void setColor(Color color){
        this->color = color;
    }
    void setShine(int shine){
        this->shine = shine;
    }
    void setCoEfficients(double coEfficients[4]){
        for(int i = 0; i < 3; i++) this->coEfficients[i] = coEfficients[i];
    }
};

class Sphere: Object{
    double radius;
    int stacks, slices;

    Shpere(){
        radius = 0.0;
        stacks = slices = 0;
    }
    Sphere(Point center, double radius, int stacks, int slices){
        reference_point = center;
        length = radius;

        this->stacks = stacks;
        this->slices = slices;
    }

    void draw(){
        Point points[stacks+1][slices+1];
        int i,j;
        double h,r;
        //generate points
        for(i=0;i<=stacks;i++)
        {
            h=radius*sin(((double)i/(double)stacks)*(pi/2));
            r=radius*cos(((double)i/(double)stacks)*(pi/2));
            for(j=0;j<=slices;j++)
            {
                points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
                points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
                points[i][j].z=h;
            }
        }
        //draw quads using generated points
        glColor3f(color.getR(), color.getG(), color.getB());
        for(i=0;i<stacks;i++)
        {
            for(j=0;j<slices;j++)
            {
                glBegin(GL_QUADS);{
                    //upper hemisphere
                    glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                    //lower hemisphere
                    glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
                }glEnd();
            }
        }

    }

    double interset(){
        return 1.0;
    }
};

class Triangle: Object{
public:
    Point p[3];

    Triangle(Point p1, Point p2, Point p3){
        p[0] = p1;
        p[1] = p2;
        p[2] = p3;
    }

    void draw(){
        glColor3f(color.getR(), color.getG(), color.getB());

        glBegin(GL_TRIANGLES);
        {
            for(int i = 0; i < 3; i++)
                glVertex3f(p[i].x, p[i].y, p[i].z);
        }
        glEnd();
    }

    double intersect(){
        return 1.0;
    }

};

class Floor:Object{
    double length, width;

};


class PointLight{
public:
    Point lightPos;
    Color color;

    PointLight(){
        this->lightPos = {0.0,0.0,0.0};
        this->color = {0,0,0};
    }

    PointLight(Point pos, Color color){
        this->lightPos = pos;
        this->color = color;
    }

    void operator = (const PointLight &pointLight){
        this->lightPos = pointLight.lightPos;
        this->color = pointLight.color;
    }
};

class SpotLight{
    PointLight pointLight;
    Point direction;
    double cutoffAngle;

    SpotLight(){
        ///default constructor
    }

    SpotLight(PointLight pointLight, Point direction, double angle){
        this->pointLight = pointLight;
        this->direction = direction;
        this->cutoffAngle = angle;
    }
};


#endif
