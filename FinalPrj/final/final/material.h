#pragma once
#include "ray.h"

class Material {
private:
	Color color;
	double transparency;
	double reflectivity;
	double diffuse;
	double specular_n;
	double specular_k;
public:
	double getRef();
	double getTrans();
	double getDif();
	double getSpecularN();
	double getSpecularK();
	Color getColor();
	void setRef(const double& _reflectivity);
	void setColor(const Color& _color);
	void setTrans(const double& _transparency);
	void setDif(const double& _diffuse);
	void setSpecularN(const double& _specular);
	void setSpecularK(const double& _specular_k);
	Material();
	Material(const Color& _color, const double& _reflectivity = 0, const double& _transparency = 0, const double& _diffuse = 1, const double& _specular = 0);
	virtual ~Material();
};
