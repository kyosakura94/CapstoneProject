#include "Camera.h"
#include "../Core/CoreEngine.h"

Camera::Camera() : position(vec3())
{
	fieldOfView = 45.0f;
	forward = vec3(0.0f, 0.0f, 1.0f);
	up = vec3(0.0f, 1.0f, 0.0f);
	worldUp = up;
	nearPlane = 0.10f;
	farPlane = 100.0f;
	yaw = -90.0f;
	pitch = 0.0f;
	lightSources.reserve(10);
	plane.reserve(10);

	perspective = glm::perspective(fieldOfView, CoreEngine::GetInstance()->GetWindowSize().x /
		CoreEngine::GetInstance()->GetWindowSize().y, nearPlane, farPlane);

	orthographic = glm::ortho(0.0f, CoreEngine::GetInstance()->GetWindowSize().x, 0.0f, CoreEngine::GetInstance()->GetWindowSize().y, -1.0f, 1.0f);

	UpdateCameraVectors();
}

Camera::~Camera()
{
	OnDestroy();
}

void Camera::SetPosition(vec3 position_)
{
	position = position_;
	UpdateCameraVectors();
}

vec3 Camera::GetPosition()
{
	return position;
}

void Camera::SetRotation(float yaw_, float pitch_)
{
	yaw = yaw_;
	pitch = pitch_;
	UpdateCameraVectors();
}

void Camera::Addlightsources(LightSource* light)
{
	lightSources.push_back(light);
}

void Camera::AddPlane(Plane * plane_)
{
	plane.push_back(plane_);
}

vector<LightSource*> Camera::GetLightSources()
{
	return lightSources;
}

std::vector<Plane*> Camera::GetPlane()
{
	return plane;
}

vec2 Camera::GetClippingPlanes() const
{
	return vec2(nearPlane, farPlane);
}

mat4 Camera::GetView() const
{
	return glm::lookAt(position, position + forward, up);
}

mat4 Camera::GetPerspective() const
{
	return perspective;
}

mat4 Camera::GetOrthographic() const
{
	return orthographic;
}

void Camera::ExtractPlane() const
{
	//mat4 invertMat = inverse(GetPerspective() * GetView());
	mat4 invertMat = GetView() * GetPerspective();

	//LEFT
	plane[0]->x = invertMat[3][0] + invertMat[0][0];
	plane[0]->y = invertMat[3][1] + invertMat[0][1];
	plane[0]->z = invertMat[3][2] + invertMat[0][2];
	plane[0]->d = invertMat[3][3] + invertMat[0][3];

	//RIGHT
	plane[1]->x = invertMat[3][0] - invertMat[0][0];
	plane[1]->y = invertMat[3][1] - invertMat[0][1];
	plane[1]->z = invertMat[3][2] - invertMat[0][2];
	plane[1]->d = invertMat[3][3] - invertMat[0][3];
	
	//TOP
	plane[2]->x = invertMat[3][0] - invertMat[1][0];
	plane[2]->y = invertMat[3][1] - invertMat[1][1];
	plane[2]->z = invertMat[3][2] - invertMat[1][2];
	plane[2]->d = invertMat[3][3] - invertMat[1][3];
	
	//BOTTOM
	plane[3]->x = invertMat[3][0] + invertMat[1][0];
	plane[3]->y = invertMat[3][1] + invertMat[1][1];
	plane[3]->z = invertMat[3][2] + invertMat[1][2];
	plane[3]->d = invertMat[3][3] + invertMat[1][3];
	
	//NEAR
	plane[4]->x = invertMat[3][0] + invertMat[2][0];
	plane[4]->y = invertMat[3][1] + invertMat[2][1];
	plane[4]->z = invertMat[3][2] + invertMat[2][2];
	plane[4]->d = invertMat[3][3] + invertMat[2][3];
	
	//FAR
	plane[5]->x = invertMat[3][0] - invertMat[2][0];
	plane[5]->y = invertMat[3][1] - invertMat[2][1];
	plane[5]->z = invertMat[3][2] - invertMat[2][2];
	plane[5]->d = invertMat[3][3] - invertMat[2][3];

	/*plane[0]->normalize();
	plane[1]->normalize();
	plane[2]->normalize();
	plane[3]->normalize();
	plane[4]->normalize();
	plane[5]->normalize();*/
}


void Camera::ProcessMouseMovement(vec2 offset_)
{
	offset_ *= 0.05f;
	yaw += offset_.x;
	pitch += offset_.y;
	if (yaw < 0.0f)
	{
		yaw += 360.0f;
	}
	if (yaw > 360.0f)
	{
		yaw -= 360.0f;
	}
	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	SetRotation(yaw, pitch);
	UpdateCameraVectors();
}

void Camera::ProcessMoouseScroll(int y_)
{
	if (y_ < 0 || y_ > 0)
	{
		position += static_cast<float>(y_) * (forward * 2.0f);
		
	}

	UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
	forward.x = cos(radians(yaw)) * cos(radians(pitch));
	forward.y = sin(radians(pitch));
	forward.z = sin(radians(yaw)) * cos(radians(pitch));

	forward = normalize(forward);

	right = normalize(cross(forward, worldUp));

	up = normalize(cross(right, forward));
	cout << yaw << "," << pitch << endl;
}

void Camera::OnDestroy()
{
	if (lightSources.size() > 0)
	{
		for (auto t : lightSources)
		{
			delete t;
			t = nullptr;
		}
		lightSources.clear();
	}
}
