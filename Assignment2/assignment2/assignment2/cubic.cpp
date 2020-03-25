#include "cubic.h"
#include <iostream>

CubicFunc::CubicFunc(double _a1, double _a2, double _a3, double _d) {
	this->a1 = _a1;
	this->a2 = _a2;
	this->a3 = _a3;
	this->d = _d;
	this->isLimit = false;
}

CubicFunc::CubicFunc(double _a1, double _a2, double _a3, double _d, double _tmin, double _tmax) {
	this->a1 = _a1;
	this->a2 = _a2;
	this->a3 = _a3;
	this->d = _d;
	this->tmin = _tmin;
	this->tmax = _tmax;
	this->isLimit = true;
}

CubicFunc::CubicFunc() {

}

double CubicFunc::get(double t) {
	double res= a1* t* t* t + a2 * t * t + a3 * t + d;
	if (isLimit) {
		if (t >= tmin && t < tmax) {
			return res;
		}
		else {
			return 0;
		}
	}
	else {
		return res;
	}
}

void::CubicFunc::setLimit(double _tmin, double _tmax) {
	tmin = _tmin;
	tmax = _tmax;
	isLimit = true;
}

void CubicFunc::printOut() {
	//char out[200];
	printf_s("f -> %gx^3 + %gx^2 + %gx + %g\n", a1, a2, a3, d);
}


void CurveCache::add(Coor2d c) {
	data.push_back(c);
	if (isEmpty) {
		xmax = c.x; ymax = c.y; xmin = c.x; ymin = c.y;
		isEmpty = false;
	}
	else {
		xmax = c.x > xmax ? c.x : xmax;
		ymax = c.y > ymax ? c.y : ymax;
		xmin = c.x < xmin ? c.x : xmin;
		ymin = c.y < ymin ? c.y : ymin;
	}
}

CubicCurve::CubicCurve(CubicFunc f, CubicFunc g, double _tmin, double _tmax) {
	funcF = f;
	funcG = g;
	tmin = _tmin;
	tmax = _tmax;
	//funcF.setLimit(tmin, tmax);
	//funcG.setLimit(tmin, tmax);
}

Coor2d CubicCurve::get(double t) {
	return Coor2d(funcF.get(t), funcG.get(t));
}

void CubicCurve::compute(double interval, CurveCache &emptyCache) {
	for (double t = tmin; t <= tmax; t = t + interval) {
		Coor2d coor = get(t);
		emptyCache.add(coor);
	}
}

CurveDrawer::CurveDrawer(int num_curves, double width, double height) {
	caches = std::vector<CurveCache>();
	for (int i = 0; i < num_curves; i++) {
		caches.push_back(CurveCache());
	}
	screenWidth = width;
	screenHeight = height;
}

CurveDrawer::CurveDrawer() {
	caches = std::vector<CurveCache>();
}

void CurveDrawer::Init(int num_curves, double width, double height) {
	for (int i = 0; i < num_curves; i++) {
		caches.push_back(CurveCache());
	}
	screenWidth = width;
	screenHeight = height;
}

void CurveDrawer::draw() {
	if (caches.size() == 0) {
		return;
	}
	bool isFirst = true;
	double xmin, ymin, xmax, ymax;
	for (std::vector<CurveCache>::iterator iter = caches.begin(); iter != caches.end(); iter++) {
		if (isFirst) {
			xmin = iter->xmin;
			xmax = iter->xmax;
			ymin = iter->ymin;
			ymax = iter->ymax;
			isFirst = false;
		}
		else {
			xmin = xmin < iter->xmin ? xmin : iter->xmin;
			xmax = xmax > iter->xmax ? xmax : iter->xmax;
			ymin = ymin < iter->ymin ? ymin : iter->ymin;
			ymax = ymax > iter->ymax ? ymax : iter->ymax;
		}
	}

	double ratio;
	double xlen = xmax - xmin;
	double ylen = ymax - ymin;
	if (xlen < 0.00001 && ylen < 0.00001) {
		ratio = 1.0;
	}
	else if (xlen < 0.00001) {
		ratio = screenHeight / ylen;
	}
	else if (ylen < 0.00001) {
		ratio = screenWidth / xlen;
	}
	else {
		double r1 = screenWidth / xlen; 
		double r2 = screenHeight / ylen;
		ratio = r1 < r2 ? r1 : r2;
	}
	double dy = (screenHeight - ratio * ylen) / 2;
	double dx = (screenWidth - ratio * xlen) / 2;

	// start to draw.
	glClear(GL_COLOR_BUFFER_BIT);
	
	for (std::vector<CurveCache>::iterator iter = caches.begin(); iter != caches.end(); iter++) {
		glColor3f(iter->r, iter->g, iter->b);
		glBegin(GL_LINE_STRIP);
		for (std::vector<Coor2d>::iterator j = iter->data.begin(); j != iter->data.end(); j++) {
			double x = ratio * (j->x - xmin) + dx;
			double y = ratio * (j->y - ymin) + dy;
			glVertex2d(x, y);
		}
		glEnd();
	}
	
	glFlush();
}

BCurve::BCurve(double k[], int num, double _tmin, double _tmax, double _interval) {
	tmin = _tmin;
	tmax = _tmax;
	interval = _interval;
	knots = std::vector<double>();
	for (int i = 0; i < num; i++) {
		knots.push_back(k[i]);
	}
}

void BCurve::compute(CurveCache& cache, int i, int deg) {
	if (deg == 1) {
		for (double t = tmin; t < tmax; t += interval) {
			if (t >= knots[i] && t < knots[i + 1]) {
				cache.add(Coor2d(t, 1.0));
			}
			else {
				cache.add(Coor2d(t, 0));
			}
		}
	}
	else {
		CurveCache cacheB1;
		CurveCache cacheB2;
		compute(cacheB1, i, deg-1);
		compute(cacheB2, i + 1, deg - 1);
		int intNum = cacheB1.data.size() < cacheB2.data.size() ? cacheB1.data.size() : cacheB2.data.size();
		for (int j = 0; j < intNum; j++) {
			double b1 = cacheB1.data[j].y;
			double b2 = cacheB2.data[j].y;
			double t = cacheB1.data[j].x;
			double b = (t - knots[i]) / (knots[i + deg - 1] - knots[i]) * b1 + (knots[i + deg] - t) / (knots[i + deg] - knots[i + 1]) * b2;
			cache.add(Coor2d(t, b));
		}
	}
}

void CurveCache::draw(double screenWidth, double screenHeight) {
	double ratio;
	double xlen = xmax - xmin;
	double ylen = ymax - ymin;
	if (xlen < 0.00001 && ylen < 0.00001) {
		ratio = 1.0;
	}
	else if (xlen < 0.00001) {
		ratio = screenHeight / ylen;
	}
	else if (ylen < 0.00001) {
		ratio = screenWidth / xlen;
	}
	else {
		double r1 = screenWidth / xlen;
		double r2 = screenHeight / ylen;
		ratio = r1 < r2 ? r1 : r2;
	}
	double dy = (screenHeight - ratio * ylen) / 2;
	double dx = (screenWidth - ratio * xlen) / 2;

	// start to draw.
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(r, g, b);
	//glBegin(GL_LINE_STRIP);
	glBegin(GL_LINES);
	double yPre;
	double xPre;
	if (data.size() > 0) {
		yPre = ratio * (data[0].y - ymin) + dy;
		xPre = ratio * (data[0].x - xmin) + dx;
	}
	for (std::vector<Coor2d>::iterator j = data.begin(); j != data.end(); j++) {
		double x = ratio * (j->x - xmin) + dx;
		double y = ratio * (j->y - ymin) + dy;
		
		if (abs(y - yPre) < 0.2) {
			glVertex2d(xPre, yPre);
			glVertex2d(x, y);
		}
		xPre = x;
		yPre = y;
	}
	glEnd();
	glFlush();
}
