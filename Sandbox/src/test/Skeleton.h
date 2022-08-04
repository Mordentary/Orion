#pragma once
#include<Orion.h>



namespace Test {

	class Skeleton
	{
	public:
		Skeleton(glm::vec3 spawnPoint, glm::vec2 size, glm::vec4 color);
		Skeleton() = default;

		void Update(Orion::Timestep ts, glm::vec4& color);
	private:
		void InitiliazeAnimations();
		//bool StatePattern(const Orion::AnimationTypes& activeAnim, const Orion::AnimationTypes& outgoingAnim);
		void Move(Orion::Timestep ts);
		void CheckStates() 
		{
			ORI_CORE_INFO("Grounded: {0}", m_IsGrounded);
			ORI_CORE_INFO("Walking: {0}", m_IsWalking);
			ORI_CORE_INFO("Waiting: {0}", m_IsWaiting);
			ORI_CORE_INFO("Attacking: {0}", m_IsAttacking);
			ORI_CORE_INFO("Dead: {0}", m_IsDead);





		}
		Orion::AnimationStateMachine m_StateMachine;
		glm::vec4 m_Color;
		glm::vec3 m_Position;
		glm::vec2 m_Size;
		bool m_IsGrounded = true;
		bool m_IsWalking = false;
		bool m_IsWaiting = false;
		bool m_IsAttacking = false;
		bool m_IsBusuWithAnimation = false;
		bool m_IsDead = false;

		Orion::Shared<Orion::Animation2D>
			m_SkeletonReborn,
			m_SkeletonDeath,
			m_SkeletonWalk,
			m_SkeletonAttack,
			m_SkeletonWhoa,
			m_SkeletonIdle;
	};


	
}

