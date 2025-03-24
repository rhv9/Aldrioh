#include "pch.h"
#include "Camera.h"


OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
	: m_ProjectionMatrix(glm::ortho(left, right, bottom, top)), m_ViewMatrix(1.0f)
{
	RecalculateProjectionMatrix();
}

void OrthographicCamera::SetProjectionMatrix(float left, float right, float bottom, float top)
{
	m_ProjectionMatrix = glm::ortho(left, right, bottom, top);
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void OrthographicCamera::RecalculateProjectionMatrix()
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), { 0.0f, 0.0f, 1.0f });
	m_ViewMatrix = glm::inverse(transform);

	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

