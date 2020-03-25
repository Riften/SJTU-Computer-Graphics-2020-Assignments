#include <windows.h>
#include <math.h>
#include <gl\GL.h>
#include <gl\glut.h>
#include <iostream>
#include "cubic.h"
const int screenWidth = 640;
const int screenHeight = 480;

CurveDrawer drawer; // For exercise 1.
CurveCache cache;   // For exercise 2.

void myInit() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(3.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)screenWidth, 0.0, (GLdouble)screenHeight);
}
void myDisplay() {
    drawer.draw();
}

void myDisplay2() {
    cache.draw(screenWidth, screenHeight);
}

void main(int argc, char** argv) {
    int quiz;
    std::cout << "Running for which question? (1 or 2)" << std::endl;
    std::cin >> quiz;
    

    if (quiz == 1) {
        drawer.Init(2, screenWidth, screenHeight);
        CubicFunc r1(0, 0, 1, 0);// t
        CubicFunc r2(0, 1, 0, 0);// t^2
        CubicCurve r(r1, r2, 0, 1);// (t, t^2) t \in [0, 1]
        CubicFunc n1(0, 0, 2, 1);// 2t+1
        CubicFunc n2(1, 0, 4, 1); // t^3+4t+1
        CubicCurve n(n1, n2, 0, 1); // (2t+1, t^3+4t+1) \in [0,1]
        
        r.compute(0.01, drawer.caches[0]);
        n.compute(0.01, drawer.caches[1]);
        
        drawer.caches[0].setColor(1,0,0);
        drawer.caches[1].setColor(0,0,1);

        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
        glutInitWindowSize(screenWidth, screenHeight);
        glutInitWindowPosition(100, 150);
        glutCreateWindow("Curve");
        glutDisplayFunc(myDisplay);
        myInit();
        glutMainLoop();
    }
    else {
        double knots[] = { 0, 1, 3, 4, 5 };
        BCurve bCurve(knots, 5, -3, 8, 0.001);
        std::cout << "Input the value of i and deg. (One value per line)" << std::endl;
        int i, deg;
        std::cin >> i;
        std::cin >> deg;
        if (i < 0 || deg < 1 || i + deg>4) {
            std::cout << "Invalid value. i > 0, deg > 1, i + deg <= 4" << std::endl;
        }
        else {
            bCurve.compute(cache, i, deg);
            glutInit(&argc, argv);
            glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
            glutInitWindowSize(screenWidth, screenHeight);
            glutInitWindowPosition(100, 150);
            glutCreateWindow("Curve");
            glutDisplayFunc(myDisplay2);
            myInit();
            glutMainLoop();
        }
        //bCurve.compute(cache, )
    }
    
}
