#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/string_cast.hpp>

#include <vector>

#include "portal.h"


class Camera {

	friend class Application;

	glm::vec3 m_cameraPos;
	glm::vec3 m_cameraFront;
	glm::vec3 m_cameraUp;
	glm::vec3 m_velocity;
	glm::vec3 m_oldPosition;
	float m_yaw;
	float m_pitch;

public:

	Camera();
	glm::mat4 getLookAt();
	glm::vec3 getCameraFront();
	glm::vec3 getCameraUp();
	glm::vec3 getCameraPos();
	float getYaw();
	float getPitch();
	void updateCameraPosition(glm::vec3 velocity);
	void updateCameraDirection(glm::vec3 front, float newYaw, float newPitch);
	bool collisionDetection(Portal m_portals);
	void warpCamera(Portal p);
};