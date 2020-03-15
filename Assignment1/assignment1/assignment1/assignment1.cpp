#include <gl/glut.h>
#include <math.h>
#include <iostream>
#include "cubicRender.h"

cubicRender *cubic;

void renderWindow(void)
{   
    cubic->draw();
}

int main(int argc, char* argv[])
{
    std::cout << "Input the value of a, b, c, d. One value per line." << std::endl;
    double a, b, c, d;
    std::cin >> a >> b >> c >> d;

    cubic = new cubicRender(a, b, c, d);
    std::cout << "Draw curve for:" << std::endl;
    cubic->printFunction();
    cubic->printInfo();
    double height = cubic->ymax - cubic->ymin;
    int windowHeight = (int(height) + 1) > 720 ? 720 : (int(height) + 1);

    glutInit(&argc, argv);                                  
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(102, windowHeight);
    glutCreateWindow("Hello, OpenGL!");

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 101.0, 0.0, windowHeight-1);

    glutDisplayFunc(&renderWindow);
    glutMainLoop();
    return 0;
}
