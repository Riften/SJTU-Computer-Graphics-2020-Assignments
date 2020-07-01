#include "light.h"
#include <math.h>
#include <iostream>
using namespace std;
Light::Light() {
	//cout << "Basic constructor of Light\n";
}
Light::~Light() {

}
Color Light::intersect(objUnion& scence, Intersection& rayResult, GVector3 &highray) {
	cout << "Basic intersect of Light\n";
	return Color::white();
}

PointLight::PointLight()
{
}

PointLight::~PointLight()
{
}

PointLight::PointLight(const Color& _color, const double& _intensity, const GVector3& _position, const bool& _isShadow = 1)
{
	color = _color;
	intensity = _intensity;
	position = _position;
	isShadow = _isShadow;
}

Color PointLight::intersect(objUnion& scence, Intersection& result, GVector3& highray)
{
	double factor = 0.05;
	GVector3 v = result.position - position;
	GVector3 vDir = v;
	vDir.normalize();
	double DdotV = result.normal.dotMul(vDir);
	if (DdotV >= 0) {
		return Color::black();
	}
	double distance = v.getLength();
	double fadeIntensity = -(intensity / (distance * distance)) * DdotV;
	//shadowRay can be seen as a reflected ray at intersection
	//The reason I add a (vDir.negate()*factor) is avoid intersecting with this object again.
	Ray shadowRay = Ray(result.position + (vDir.negate() * factor), v.negate());
	Intersection lightResult = scence.intersect(shadowRay);
	Color diffuseColor = Color::black();
	if (lightResult.isHit && (lightResult.distance <= distance)) {
		return Color::black();
	}
	else {
		fadeIntensity = -(intensity / (distance * distance)) * DdotV;
		diffuseColor = ((color * fadeIntensity) * result.object->material->getColor().multiply(result.object->material->getDif())).saturate();
	}

	// Specular highlight
	Color highlight = Color::black();
	if (result.object->material->getSpecularK() > 0.00001) {
		GVector3 halfway = (highray.negate() + vDir.negate()).normalize();
		double highlightF = halfway.dotMul(result.normal);
		highlightF = highlightF > 0 ? highlightF : 0;
		highlightF = pow(highlightF, result.object->material->getSpecularN()) * result.object->material->getSpecularK();
		highlight = (color * fadeIntensity) * highlightF;
	}
	return diffuseColor.add(highlight);
}
AmbientLight::AmbientLight() {
	isShadow = false;
	color = Color::white().multiply(0.2);
}

AmbientLight::~AmbientLight() {
}

AmbientLight::AmbientLight(Color _color) {
	isShadow = false;
	color = _color;
	color.saturate();
}

Color AmbientLight::intersect(objUnion& scene, Intersection& result, GVector3& highray) {
	if (result.isHit) {
		return color * (result.object->material->getColor());
	}
	else {
		return Color::black();
	}
}


lightUnion::lightUnion() {}

lightUnion::~lightUnion() {}

void lightUnion::push(Light* l) {
	lights.push_back(l);
}
Color lightUnion::intersect(objUnion& scence, Intersection& result, GVector3& highray) {
	long size = lights.size();
	Color resultColor = Color::black();
	for (int i = 0; i < size; i++) {
		resultColor = resultColor.add(lights[i]->intersect(scence, result, highray));
	}
	resultColor.saturate();
	return resultColor;
}

Light* lightUnion::getLight(int i) {
	return lights[i];
}