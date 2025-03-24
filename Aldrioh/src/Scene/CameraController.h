#pragma once

#include "Events/Events.h"
#include "Graphics/Camera.h"

struct OrthographicCameraBounds
{
	float Left, Right;
	float Bottom, Top;

	float GetWidth() { return Right - Left; }
	float GetHeight() { return Top - Bottom; }
};


class CameraController
{
public:

	CameraController(const float aspectRatio, const float zoomLevel = 1.0f);

	virtual ~CameraController() {}
	virtual void OnUpdate(Timestep delta) = 0;


	void SetPosition(const glm::vec2& pos);
	void OnResize(uint32_t width, uint32_t height);

	void SetAspectRatio(const float aspectRatio) { this->aspectRatio = aspectRatio; CalculateView(); }
	void SetRotation(const float rotation) { this->rotation = rotation; CalculateView(); }
	void SetZoomLevel(const float zoomLevel) { this->zoomLevel = zoomLevel; CalculateView(); }

	const glm::vec2& GetPosition() const { return m_Position; }
	OrthographicCamera& GetCamera() { return camera; }
	const float& GetZoomLevel() { return zoomLevel;}

	const OrthographicCameraBounds& GetBounds() const { return bounds; }


private:
	void CalculateView();

protected:
	glm::vec2 m_Position = { 0.0f, 0.0f };
	float zoomLevel, aspectRatio;
	float rotation = 0.0f;
	float cameraSpeed = 1.0f, rotationSpeed = 360.0f;

	// Mouse dragging
	glm::vec2 mousePressedPoint { 0.0f };
	glm::vec2 initialCameraPos { 0.0f };
	bool mouseHeld = false;

	OrthographicCameraBounds bounds;
	OrthographicCamera camera;
};