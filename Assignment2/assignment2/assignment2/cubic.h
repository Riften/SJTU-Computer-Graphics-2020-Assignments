#pragma once

#include <vector>
#include <iostream>
#include "GL/glut.h"
// Cubic function y=f(x)
// Represents a cubic function. It contains 4 coefficients and the domain of definition.
class CubicFunc {
private:
	double a1, a2, a3, d;
	double tmax, tmin;
	bool isLimit;
public:
	CubicFunc(double _a1, double _a2, double _a3, double _d);
	CubicFunc(double _a1, double _a2, double _a3, double _d, double _tmin, double _tmax);
	CubicFunc();
	double get(double t);
	void setLimit(double _tmin, double _tmax);
	void printOut();
};

class Coor2d {
public:
	double x; double y;
	Coor2d(double _x, double _y) { x = _x; y = _y; }
	void printOut() { printf_s("(%g, %g)\n", x, y); }
};

// CurveCache is used to save the coordinates computed by CubicCurve. 
// With a CurveCache, we can draw a curve by connecting the coordinates using line strips.
// CurveCache.draw() also design the method to resize the coordinates so that we can draw it properly on the canvas.
class CurveCache {
public:
	std::vector<Coor2d> data;
	double xmax, ymax, xmin, ymin;
	float r, g, b; //Used to set color of line.
	bool isEmpty;
	CurveCache() { data = std::vector<Coor2d>(); isEmpty = true; r = 0; g = 0; b = 0; }
	void add(Coor2d);
	void setColor(float _r, float _g, float _b) { r = _r; g = _g; b = _b; };
	void draw(double screenWidth, double screenHeight);
};

// CubicCurve
// Each CubicCurve contains two CubicFunc, x(t) and y(t).
// CubicCurve can compute several coordinates based on domain of t and an interval.
class CubicCurve {
private:
	CubicFunc funcF;
	CubicFunc funcG;
	double tmax, tmin;
public:
	CubicCurve(CubicFunc f, CubicFunc g, double _tmin, double _tmax);
	//CubicCurve();
	Coor2d get(double t);
	double tMax() { return tmax; }
	double tMin() { return tmin; }
	void compute(double interval, CurveCache &emptyCache);
};

// Used to draw a list of curves according to a list of CurveCache.
class CurveDrawer {
private:
	double screenWidth, screenHeight;
	//double xmin, ymin, xmax, ymax;
public:
	std::vector<CurveCache> caches;
	CurveDrawer();
	void Init(int num_curves, double width, double height);
	CurveDrawer(int num_curves, double width, double height);
	void draw();
};

class BCurve {
private:
	std::vector<double> knots;
	double tmin, tmax, interval;
public:
	BCurve(double t[], int num, double _tmin, double _tmax, double _interval);
	void compute(CurveCache &cache, int i, int deg);
};