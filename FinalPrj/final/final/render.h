#pragma once
#include "ray.h"
#include "light.h"
#include "material.h"
#define mini 0.001

/*
Implementation of rendering pipeline:
Input a ray:
	for all the objects in the scene
		get the intersection that is closest to the origin of ray
	for all the lights in the scene
		compute the color of intersection lighted by these lights
		merge these color together
	if reflective:
		generate a reflected ray and rendering recursively
		merge color
	return color
*/
Color rend(Ray& ray, objUnion& scene, lightUnion& lights, bool isRef = false, bool isTrans = false, int maxReflect = 0) {

	Intersection result = scene.intersect(ray);
	if (result.isHit == 0) {
		return Color::black();
	}
	GVector3 highray = ray.getDirection();
	Color color = lights.intersect(scene, result, highray);
	Color transColor(0,0,0);
	Color reflectedColor(0, 0, 0);
	if (isTrans && maxReflect > mini) {
		double transparency = result.object->material->getTrans();
		if (transparency > mini ) {
			color = color * (1 - transparency);
			
			GVector3 transDirection = ray.getDirection();
			double ni_over_nt = 1.1;
			if (result.isInside) {
				ni_over_nt = 1 / ni_over_nt;
			}
			double dt = ray.getDirection().dotMul(result.normal);
			double discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
			if (discriminant > 0) {
				transDirection = (ray.getDirection() - result.normal * dt) * ni_over_nt - result.normal * sqrt(discriminant);	
			}
			//else {
			//	transColor = Color::black();
			//}
			Ray transRay = Ray(result.position + ray.getDirection() * mini, transDirection);
			//Ray transRay = Ray(result.position, transDirection);
			transColor = rend(transRay, scene, lights, isRef, isTrans, maxReflect - 1).multiply(transparency);
		}
	}
	if (isRef && maxReflect > mini)
	{
		float reflectivity = result.object->material->getRef();
		if (reflectivity > mini){
			GVector3 r = result.normal * (-2 * result.normal.dotMul(ray.getDirection())) + ray.getDirection();
			Ray reflectRay = Ray(result.position, r);
			reflectedColor = rend(reflectRay, scene, lights, isRef, isTrans, maxReflect - 1).multiply(reflectivity);
		}
	}
	color = color.add(transColor);
	color = color.add(reflectedColor);
	return color;
}

