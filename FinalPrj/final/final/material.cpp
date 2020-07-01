#include "material.h"  

Material::Material() {}
Material::Material(const Color& _color, const double& _reflectivity, const double& _transparency, const double& _diffuse, const double& _specular) {
	color = _color;
	reflectivity = _reflectivity;
	transparency = _transparency;
	diffuse = _diffuse;
	specular_n = _specular;
	specular_k = 0;
}
Material::~Material() {}

double Material::getRef() {
	return reflectivity;
}
double Material::getTrans() {
	return transparency;
}
double Material::getDif() {
	return diffuse;
}
Color Material::getColor() {
	return color;
}
double Material::getSpecularN() {
	return specular_n;
}

double Material::getSpecularK() {
	return specular_k;
}
void Material::setRef(const double& _reflectivity) {
	reflectivity = _reflectivity;
}
void Material::setTrans(const double& _transparency) {
	transparency = _transparency;
}
void Material::setColor(const Color& _color) {
	color = _color;
}

void Material::setDif(const double& _diffuse) {
	diffuse = _diffuse;
}

void Material::setSpecularN(const double& _specular) {
	specular_n = _specular;
}

void Material::setSpecularK(const double& _specular_k) {
	specular_k = _specular_k;
}
