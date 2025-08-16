#include "camera.h"


Camera::Camera() :
	m_cameraPos(glm::vec3(0.0f, 1.8f, 0.0f)),
	m_cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)),
	m_cameraUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_velocity(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_yaw(270.0f),
	m_pitch(0.0f)
{ 
	m_oldPosition = m_cameraPos;
}

glm::mat4 Camera::getLookAt() {

	return glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}

glm::vec3 Camera::getCameraFront() {
	return m_cameraFront;
}

glm::vec3 Camera::getCameraUp() {
	return m_cameraUp;
}

glm::vec3 Camera::getCameraPos()
{
	return m_cameraPos;
}

float Camera::getYaw()
{
	return m_yaw;
}

float Camera::getPitch()
{
	return m_pitch;
}

void Camera::warpCamera(Portal p) {

	float angleDiff = 
		p.getDestinationPortal()->getAngle() * p.getDestinationPortal()->getRotationAxis().y - 
		p.getAngle() * p.getRotationAxis().y;

	if (angleDiff > 180) {
		angleDiff = angleDiff - 360.0f;
	}
	else if (angleDiff < -180) {
		angleDiff = angleDiff + 360;
	}
	else if (abs(angleDiff) == 180) angleDiff = 0;
	else if (angleDiff == 0) angleDiff = 180;

	m_yaw += angleDiff;

	glm::vec3 posDiff = m_cameraPos - p.getPosition();
	
	//rotate vector
	//thats why rotationAxis in Camera class constructor is set to y=1.0f
	posDiff = glm::vec3(glm::rotate(
		glm::mat4(1.0f),
		glm::radians(-angleDiff),
		p.getDestinationPortal()->getRotationAxis()
	) * glm::vec4(posDiff, 1.0f));
	m_cameraPos = p.getDestinationPortal()->getPosition() + posDiff;
}

bool Camera::collisionDetection(Portal m_portals) {

	if (m_cameraPos == m_oldPosition) {
		return 0;
	}

	std::vector<Vertex> portalVertices = m_portals.getVertices();
	std::vector<unsigned int> indc = m_portals.indices;
	for (int j = 0; j < 2; j++) {
		glm::vec3
			p0 = portalVertices[indc[j * 3 + 0]].position,
			p1 = portalVertices[indc[j * 3 + 1]].position,
			p2 = portalVertices[indc[j * 3 + 2]].position;

		glm::vec3 tuv =
			glm::inverse(glm::mat3(
				glm::vec3(
					m_oldPosition.x - m_cameraPos.x,
					m_oldPosition.y - m_cameraPos.y,
					m_oldPosition.z - m_cameraPos.z
				),
				glm::vec3(p1.x - p0.x, p1.y - p0.y, p1.z - p0.z),
				glm::vec3(p2.x - p0.x, p2.y - p0.y, p2.z - p0.z)
			)) * glm::vec3(m_oldPosition.x - p0.x, m_oldPosition.y - p0.y, m_oldPosition.z - p0.z);

		float t = tuv.x, u = tuv.y, v = tuv.z;

		if (t >= 0 - 1e-6 && t <= 1 + 1e-6) {
			if (u >= 0 - 1e-6 && u <= 1 + 1e-6 && v >= 0 - 1e-6 && v <= 1 + 1e-6 && (u + v) <= 1 + 1e-6) {
				return 1;
			}
		}
	}
	return 0;
}

void Camera::updateCameraPosition(glm::vec3 velocity) {

	//save current position
	m_oldPosition = m_cameraPos;

	m_cameraPos.x += velocity.x;
	m_cameraPos.z += velocity.z;
}

void Camera::updateCameraDirection(glm::vec3 front, float newYaw, float newPitch) {

	m_yaw = newYaw;
	m_pitch = newPitch;
	m_cameraFront = front;
}
