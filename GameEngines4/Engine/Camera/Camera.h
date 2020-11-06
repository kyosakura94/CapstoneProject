
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../FX/LightSource.h"
#include <vector>
#include "Plane.h"
#include "../Math/Randomizer.h"

using namespace glm;
using namespace MATH;

class Camera
{
public:
	Camera();
	~Camera();

	void SetPosition(vec3 position_);
	vec3 GetPosition();
	void SetRotation(float yaw_, float pitch_);
	void Addlightsources(LightSource* light);
	void AddPlane(Plane* plane_);
	std::vector<LightSource*> GetLightSources();
	std::vector<Plane*> GetPlane();
	vec2 GetClippingPlanes() const;

	mat4 GetView() const;
	mat4 GetPerspective() const;
	mat4 GetOrthographic() const;
	
	void ExtractPlane() const;

	void ProcessMouseMovement(vec2 offset_);
	void ProcessMoouseScroll(int y_);

private:
	void UpdateCameraVectors();
	void OnDestroy();
	std::vector<LightSource*> lightSources;
	std::vector<Plane*> plane;


	vec3 position;
	mat4 perspective, orthographic;
	float fieldOfView;
	float yaw, pitch;
	float nearPlane, farPlane;
	vec3 forward, up, right, worldUp;

};
#endif // !CAMERA_H


