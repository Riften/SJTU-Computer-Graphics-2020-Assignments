#pragma once

/**
 * cubicRender is used to draw the curve y=ax^3+bx^2+cx+d.
 * One global cubicRender object is used to draw the curve.
 */
class cubicRender {
private:
    void computeLimit();
    double getFunctionValue(double x);  // Compute f(x);
    void drawPixels(int x1, double y1, int x2, double y2);  // Draw pixels between (x1, y1) and (x2, y2).
    void drawPixel(int x, double y); // Draw pixel (x, y).
    //void drawPixel(int x, int y);
    double getNext(double y1, double y2, double y3);   // yi = 3y{i-1}-3y{i-2}+y{i-3}+6a
    double a, b, c, d;
    double a6;
    
public:
    double ymin, ymax;  // range of y
    double xmin, xmax;  // range of x
    cubicRender(double _a, double _b, double _c, double _d);
    void printFunction();   // Print out function.
    void printInfo();       // Print info about the curve.
    void draw();
    void naiveDraw();       // Show the wrong method.
};



