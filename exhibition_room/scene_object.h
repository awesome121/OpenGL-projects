/**
 * Header file for abstract class SceneObject
 * This is an abstract class for Sphere, Plane, Cylinder...
 * 
 * Implementation of  intersect() and normal()
 * needs to be provided
 * 
 */

#pragma once
#include <glm/glm.hpp>


class SceneObject
{
protected:
	glm::vec3  mColor = glm::vec3(1);
	bool mIsReflective = false;
	bool mIsRefractive = false;
	bool mIsSpecular = true;
	bool mIsTransparent = false;
	float mReflectivity = 0.8;
	float mRefractivity = 0.8;
	float mTransparency = 0.8;
	float mRefractivityIndex = 1;
	float mShininess = 50;
	
public:
	SceneObject() {}
	// virtual = 0: pure virtual makes this class abstract
	virtual float Intersect(glm::vec3 p0, glm::vec3 dir) = 0;
	// Assume p0 is on the surface of the object
	virtual glm::vec3 GetNormal(glm::vec3 p0) = 0;  
	virtual ~SceneObject() {}
	
	glm::vec3 Lighting(glm::vec3 lightPos, glm::vec3 viewVec, glm::vec3 hit);
	void SetColor(glm::vec3 col);
	void SetReflectivity(bool flag);
	void SetReflectivity(bool flag, float refl_coeff);
	void SetRefractivity(bool flag);
	void SetRefractivity(bool flag, float refr_coeff, float refr_indx);
	void SetShininess(float shininess);
	void SetSpecularity(bool flag);
	void SetTransparency(bool flag);
	void SetTransparency(bool flag, float tran_coeff);
	
	glm::vec3 GetColor();
	float GetReflectionCoeff();
	float GetRefractionCoeff();
	float GetTransparencyCoeff();
	float GetRefractiveIndex();
	float GetShininess();
	bool IsReflective();
	bool IsRefractive();
	bool IsSpecular();
	bool IsTransparent();
};
