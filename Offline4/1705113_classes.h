#ifndef HEADER_FILE_NAME
#define HEADER_FILE_NAME


using namespace std;
#define pi (2*acos(0.0))
#define INF INT_MAX

class Color{
public:
    double r,g,b;
    Color(double r=0, int g=0, int b=0){
        this->r = r;
        this->g = g;
        this->b = b;
    }

    double getR() {return r;}
    double getG() {return g;}
    double getB() {return b;}

    void operator = (const Color &color){
        this->r = color.r;
        this->g = color.g;
        this->b = color.b;
    }
    void print(string s=""){
        cout << s <<" "<<r <<" "<<g <<" "<<b <<endl;
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

    void normalize(){
        double magnitude = sqrt(x*x+y*y+z*z);
        x /= magnitude;
        y /= magnitude;
        z /= magnitude;
    }

    void operator = (const Point &point){
        this->x = point.x;
        this->y = point.y;
        this->z = point.z;
    }

    Point operator + (const Point &point){
        return Point(this->x + point.x,this->y + point.y,this->z + point.z);
    }

    Point operator - (const Point &p){
        return {x-p.x, y-p.y, z-p.z};
    }

    Point operator * (const double &d){
        return {x*d, y*d, z*d};
    }

    Point multiplyCosnt(double cons){
        return {x*cons, y*cons, z*cons};
    }

    Point cross(const Point &b) {
		return {y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x};
	}

	double dot(const Point &b) {
		return (x * b.x) + (y * b.y) + (z * b.z);
	}


    void print(string s=""){
        cout << s << endl;
        cout << "("<<x <<","<< y <<","<<z<<")"<<endl;
    }
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
};

class SpotLight{
public:
    PointLight pointLight;
    Point light_direction;
    double cutoff_angle;

    SpotLight(PointLight, Point light_direction, double cutoff_angle){
        this->pointLight = pointLight ;
        this->light_direction = light_direction;
        this->cutoff_angle = cutoff_angle;
    }
};


class Ray{
public:
    Point start, dir;

    Ray(Point start, Point dir){
        this->start = start;
        this->dir = dir;
    }
};

class Object{
public:
    Point reference_point;
    double height, width, length;
    Color color;
    double kd,ka,ks,kr;

    int shine;
    Object(){
        reference_point = {0,0,0};
        height = width = length = 0.0;
        color = {0,0,0};
        kd = ka = ks = kr = 0;
    }
    Object(Point reference_point, double height, double width, double length, Color color , double coEfficients[4]){
        this->reference_point = reference_point;
        this->height = height;
        this->width = width;
        this->length = length;
        this->color = color;

        ka = coEfficients[0];
        kd = coEfficients[1];
        ks = coEfficients[2];
        kr = coEfficients[3];
    }

    virtual void draw(){}

    virtual double intersect(Ray *r, double color, int level){ return -1.0; }

    void setColor(Color color){
        this->color = color;
        this->color.print();
    }
    void setShine(int shine){
        this->shine = shine;
    }
    void setCoEfficients(double coEfficients[4]){
        ka = coEfficients[0];
        kd = coEfficients[1];
        ks = coEfficients[2];
        kr = coEfficients[3];
    }
};

class Sphere: public Object{
public:
    double radius;
    int stacks, slices;

    Shpere(){
        radius = 0.0;
        stacks = slices = 0;
    }
    Sphere(Point center, double radius, int stacks, int slices){
        reference_point = center;
        this->radius = radius;

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
        for(i=0;i<stacks;i++)
        {
            //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
            glColor3f(color.r,color.g,color.b);
            //color.print();
            for(j=0;j<slices;j++)
            {
                glBegin(GL_QUADS);{
                    //upper hemisphere
                    Point temp1 = reference_point + points[i][j];
                    Point temp2 = reference_point + points[i][j+1];
                    Point temp3 = reference_point + points[i+1][j+1];
                    Point temp4 = reference_point + points[i+1][j];

                    glVertex3f(temp1.x, temp1.y, temp1.z);
                    glVertex3f(temp2.x, temp2.y, temp2.z);
                    glVertex3f(temp3.x, temp3.y, temp3.z);
                    glVertex3f(temp4.x, temp4.y, temp4.z);
                    //lower hemisphere
                    glVertex3f(temp1.x, temp1.y, temp1.z-2*points[i][j].z);
                    glVertex3f(temp2.x, temp2.y, temp2.z-2*points[i][j+1].z);
                    glVertex3f(temp3.x, temp3.y, temp3.z-2*points[i+1][j+1].z);
                    glVertex3f(temp4.x, temp4.y, temp4.z-2*points[i+1][j].z);
                }glEnd();
            }
        }
    }

    double interset(Ray *ray, Color color, int level){
        double a,b,c,t;
        a = 1;
        b = ray->start.dot(ray->dir) - (2.0) * (ray->dir.dot(reference_point));
        c = ray->start.dot(ray->start) + reference_point.dot(reference_point) - ray->start.dot(reference_point)*2.0 - radius*radius;

        double discriminant = b*b - 4.0*a*c;

        if(discriminant > 0.0){
            double m = (sqrt(discriminant) - b) / (2.0*a);
            t = (-sqrt(discriminant) - b) / (2.0*a);
            t = (t > 0.0)? t:m;
        }
        else if(discriminant == 0){
            t = -b / (2.0*a);
        }
        else{
            t = INF;
        }

        if(level == 0){
            return t;
        }


        return t;
    }
};

class Triangle: public Object{
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

class GeneralQuad: public Object{
public:
    double a,b,c,d,e,f,g,h,i,j;
    GeneralQuad(double a, double b, double c, double d, double e, double f, double g, double h, double i, double j, Point refP, double len, double wid, double height){
        this->a = a;
        this->b = b;
        this->c = c;
        this->d = d;
        this->e = e;
        this->f = f;
        this->g = g;
        this->h = h;
        this->i = i;
        this->j = j;

        this->reference_point = refP;
        this->length = len;
        this->width = wid;
        this->height = height;
    }

};

class Floor: public Object{
public:
    double width, sqWidth;
    Floor(){
        width = sqWidth = 0.0;
    }

    Floor(double width, double sqWidth){
        this->width = width;
        this->sqWidth = sqWidth;
    }

    void draw(){
        int row = width / sqWidth;
        int col = width / sqWidth;

        for(int i = 0; i < row; i++){
            for(int j = 0; j < col; j++){
                int color;
                if((i+j)%2 == 0){
                    color = 255;
                }
                else{
                    color = 0;
                }
                glColor3f(color, color, color);
                Point bottomLeft(-width/2.0+sqWidth*j, -width/2.0+sqWidth*i, 0.0);
                glBegin(GL_QUADS);
                {
                    glVertex3f(bottomLeft.x, bottomLeft.y, bottomLeft.z);
                    glVertex3f(bottomLeft.x+sqWidth, bottomLeft.y, bottomLeft.z);
                    glVertex3f(bottomLeft.x+sqWidth, bottomLeft.y+sqWidth, bottomLeft.z);
                    glVertex3f(bottomLeft.x, bottomLeft.y+sqWidth, bottomLeft.z);
                }
                glEnd();
            }
        }
    }

    bool intersect(Ray *ray, Color color, int level){
        double t = INF;
        ///checking if t parallel to plane
        if(ray->dir.z == 0) return t;

        t = (-1.0)* (ray->start.z)/(ray->dir.z);

        ///checking if the intersection point in front of camera
        if(t > 0.0){
            Point interSection = ray->start + ray->dir*t;

            ///checking if the intersection point is in floor
            if(!(interSection.x > -width/2.0 && interSection.x < width/2.0 && interSection.y > -width/2.0 && interSection.y < width/2.0)){
                t = INF;
            }
        }

        if(level == 0) return t;



        return t;
    }
};



#endif
