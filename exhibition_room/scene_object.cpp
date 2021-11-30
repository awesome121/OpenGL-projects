/**
 * Scene object class
 * This is an abstract class for Sphere, Plane, Cylinder...
 * 
 * Implementation of  intersect() and normal()
 * needs to be provided
 * 
 */
 
#include "scene_object.h"


glm::vec3 SceneObject::Lighting(glm::vec3 lightPos, glm::vec3 viewVec, glm::vec3 hit)
{
	float ambient_term = 0.2;
	float diffuse_term  = 0;
	float specular_term  = 0;
	glm::vec3 v_normal = GetNormal(hit);
	glm::vec3 v_light = glm::normalize(lightPos - hit);
	float lDotn = glm::dot(v_light, v_normal);
	if (mIsSpecular)
	{
		glm::vec3 v_refl = glm::reflect(-v_light, v_normal);
		float rDotv = glm::dot(v_refl, viewVec);
		if (rDotv > 0) specular_term = pow(rDotv, mShininess);
	}
	return ambient_term * mColor + lDotn * mColor\
	 + specular_term * glm::vec3(1);
}

glm::vec3 SceneObject::GetColor()
{
	return mColor;
}

float SceneObject::GetReflectionCoeff()
{
	return mReflectivity;
}

float SceneObject::GetRefractionCoeff()
{
	return mRefractivity;
}

float SceneObject::GetTransparencyCoeff()
{
	return mTransparency;
}

float SceneObject::GetRefractiveIndex()
{
	return mRefractivityIndex;
}

float SceneObject::GetShininess()
{
	return mShininess;
}

bool SceneObject::IsReflective()
{
	return mIsReflective;
}

bool SceneObject::IsRefractive()
{
	return mIsRefractive;
}


bool SceneObject::IsSpecular()
{
	return mIsSpecular;
}


bool SceneObject::IsTransparent()
{
	return mIsTransparent;
}

void SceneObject::SetColor(glm::vec3 col)
{
	mColor = col;
}

void SceneObject::SetReflectivity(bool flag)
{
	mIsReflective = flag;
}

void SceneObject::SetReflectivity(bool flag, float refl_coeff)
{
	mIsReflective = flag;
	mReflectivity = refl_coeff;
}

void SceneObject::SetRefractivity(bool flag)
{
	mIsRefractive = flag;
}

void SceneObject::SetRefractivity(bool flag, float refr_coeff, float refr_index)
{
	
	mIsRefractive = flag;
	mRefractivity = refr_coeff;
	mRefractivityIndex = refr_index;
}

void SceneObject::SetShininess(float shininess)
{
	mShininess = shininess;
}

void SceneObject::SetSpecularity(bool flag)
{
	mIsSpecular = flag;
}

void SceneObject::SetTransparency(bool flag)
{
	mIsTransparent  = flag;
}

void SceneObject::SetTransparency(bool flag, float tran_coeff)
{
	mIsTransparent = flag;
	mTransparency = tran_coeff;
}
