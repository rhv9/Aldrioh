#include "pch.h"
#include "ImGuiEntityComponents.h"

#include <Scene/Entity.h>
#include <Scene/Components.h>

#include <imgui.h>

void ImGuiEntityComponents::Show(Entity e)
{
	
	if (ImGui::CollapsingHeader(e.GetComponent<NameComponent>().name.c_str()))
	{
		if (e.HasComponent<PhysicsMovementComponent>() && ImGui::CollapsingHeader("PhysicsMovementComponent"))
		{
			auto& pmc = e.GetComponent<PhysicsMovementComponent>();
			glm::vec2 velocity = pmc.managedVelocity + pmc.resultantVelocity;
			ImGui::Text(std::format("Velocity: ({:.2f},{:.2f})  {:.2f}", velocity.x, velocity.y, glm::length(velocity)).c_str());
		}

		if (e.HasComponent<MoveControllerComponent>() && ImGui::CollapsingHeader("MoveControllerComponent"))
		{
			auto& mcc = e.GetComponent<MoveControllerComponent>();
			ImGui::DragFloat("Speed", &mcc.speed);
			ImGui::DragFloat("Max Speed", &mcc.maxSpeed);
			ImGui::DragFloat("Falloff", &mcc.falloff);

		}

	}
}
