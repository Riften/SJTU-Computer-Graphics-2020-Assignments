#include "cubicRender.h"
#include <gl/glut.h>
#include <iostream>
#include <math.h>

cubicRender::cubicRender(double _a, double _b, double _c, double _d) {
    a = _a; b = _b; c = _c; d = _d;
    xmin = 1.0;
    xmax = 100.0;
    a6 = 6 * a;
    computeLimit();
}

void cubicRender::printFunction() {
    //char str[100];
    //sprintf_s(str, "%.3fx^3 + %.3fx^2 + %.3fx + %.3f", a, b, c, d);
    printf_s("%gx^3 + %gx^2 + %gx + %g\n", a, b, c, d);
    return;
}

void cubicRender::printInfo() {
    printf_s("x within [%g, %g]\ny within [%g, %g]\n", xmin, xmax, ymin, ymax);
    return;
}

double cubicRender::getFunctionValue(double x) {
    return a * x * x * x + b * x * x + c * x + d;
}

double cubicRender::getNext(double y1, double y2, double y3) {   // yi = 3y{i-1}-3y{i-2}+y{i-3}+6a
    return y3 + y3 + y3 - y2 - y2 - y2 + y1 + a6;
}

void cubicRender::computeLimit() {
    double y1 = getFunctionValue(xmin); //a + b + c + d;  // f(1)
    double y2 = getFunctionValue(xmax); //1000000 * a + 10000 * b + 100 * c + d;  // f(100)
    ymin = (y1 < y2) ? y1 : y2;
    ymax = (y1 < y2) ? y2 : y1;


    double delta = 4 * b * b - 12 * a * c;
    if (delta > 0) { // compute extremum at the root of derivation
        delta = sqrt(delta);
        double x1 = (-2 * b + delta) / (6 * a);
        double x2 = (-2 * b - delta) / (6 * a);
        double y3 = getFunctionValue(x1);
        double y4 = getFunctionValue(x2);
        ymin = (ymin < y3) ? ymin : y3; ymin = (ymin < y4) ? ymin : y4;
        ymax = (ymax > y3) ? ymax : y3; ymax = (ymax > y4) ? ymax : y4;
    }
    else { // Function is monotonous
        return;
    }
}

void cubicRender::drawPixel(int x, double y) {
    glVertex2i(x, int(y+0.5));
}

void cubicRender::drawPixels(int x1, double y1, int x2, double y2) {
    int y1r = int(y1 + 0.5);
    int y2r = int(y2 + 0.5);
    if (y1r == y2r) {
        drawPixel(x1, y1r);
        drawPixel(x2, y1r);
        return;
    }
    int mid = (y1r + y2r) >> 1;
    int dy = y1r < y2r ? 1 : -1;
    //int err = abs(2 * (y1r - y2r));
    //int derr = 2;
    for (int y = y1r; y != mid; y+=dy) {
        glVertex2i(x1, y);
    }
    for (int y = mid; y != y2r; y+=dy) {
        glVertex2i(x2, y);
    }
    return;
}

void cubicRender::draw() {
    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(1.0f);
    glColor3f(1, 0, 0);
    glBegin(GL_POINTS);
    double ys[101];
    ys[1] = getFunctionValue(1);
    ys[2] = getFunctionValue(2);
    ys[3] = getFunctionValue(3);
    
    drawPixels(1, ys[1]-ymin, 2, ys[2]-ymin);
    drawPixels(2, ys[2]-ymin, 3, ys[3]-ymin);
    for (int x = 4; x <= 100; x++) {
        ys[x] = getNext(ys[x - 3], ys[x - 2], ys[x - 1]);
        drawPixels(x - 1, ys[x - 1]-ymin, x, ys[x]-ymin);
    }
    drawPixel(100, ys[100]);
    glEnd();
    glFlush();
}

void cubicRender::naiveDraw() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(1.0f);
    glColor3f(1, 0, 0);
    glBegin(GL_POINTS);
    double ys[101];
    ys[1] = getFunctionValue(1);
    ys[2] = getFunctionValue(2);
    ys[3] = getFunctionValue(3);

    drawPixel(1, ys[1] - ymin);
    drawPixel(2, ys[2] - ymin);
    drawPixel(3, ys[3] - ymin);
    for (int x = 4; x <= 100; x++) {
        ys[x] = getNext(ys[x - 3], ys[x - 2], ys[x - 1]);
        drawPixel(x, ys[x] - ymin);
    }
    glEnd();
    glFlush();
}