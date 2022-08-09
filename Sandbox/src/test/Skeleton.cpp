#include "Skeleton.h"

#define ORI_BIND_ANIMATION_FN(fn) [&](const auto animType1, const auto animType2) { return fn(animType1, animType2); }

namespace Test
{
	Skeleton::Skeleton(glm::vec3 spawnPoint, glm::vec2 size, glm::vec4 color) : m_Color(color), m_Position(spawnPoint), m_Size(size)
	{
		InitiliazeAnimations();
	}
	void Skeleton::Update(Orion::Timestep ts, glm::vec4& color)
	{
		m_Color = color;
		Move(ts);
		m_StateMachine.UpdateActiveAnimation();
		//CheckStates();
	}
	void Skeleton::InitiliazeAnimations()
	{
		m_SkeletonReborn = Orion::CreateShared<Orion::Animation2D>
			(
				Orion::AnimationLibrary::Get("SkeletonDeath"),
				Orion::AnimationTypes::Reborn,
				true
				);
		m_SkeletonDeath = Orion::CreateShared<Orion::Animation2D>
			(
				Orion::AnimationLibrary::Get("SkeletonDeath"),
				Orion::AnimationTypes::Death
				);
		m_SkeletonWalk = Orion::CreateShared<Orion::Animation2D>
			(
				Orion::AnimationLibrary::Get("SkeletonWalk"),
				Orion::AnimationTypes::Walk
				);
		m_SkeletonAttack = Orion::CreateShared<Orion::Animation2D>
			(
				Orion::AnimationLibrary::Get("SkeletonAttack"),
				Orion::AnimationTypes::Attack
				);
		m_SkeletonWhoa = Orion::CreateShared<Orion::Animation2D>
			(
				Orion::AnimationLibrary::Get("SkeletonWhoa"),
				Orion::AnimationTypes::Whoa
				);
		m_SkeletonIdle = Orion::CreateShared<Orion::Animation2D>
			(
				Orion::AnimationLibrary::Get("SkeletonIdle"),
				Orion::AnimationTypes::Idle
				);

		//m_StateMachine.SetStateFunction(ORI_BIND_ANIMATION_FN(Skeleton::StatePattern));

		m_StateMachine.AddAnimation(m_SkeletonDeath);
		m_StateMachine.AddAnimation(m_SkeletonReborn);
		m_StateMachine.AddAnimation(m_SkeletonWalk);
		m_StateMachine.AddAnimation(m_SkeletonAttack);
		m_StateMachine.AddAnimation(m_SkeletonWhoa);
		m_StateMachine.AddAnimation(m_SkeletonIdle);

		m_StateMachine.BindSpecToAnimation(&m_Position, &m_Size, &m_Color);
	}

	//Old system with pattern
	/*bool Skeleton::StatePattern(const Orion::AnimationTypes& activeAnim, const Orion::AnimationTypes& outgoingAnim)
	{
		switch (activeAnim)
		{
		case Orion::AnimationTypes::Reborn:
			switch (outgoingAnim)
			{
			case Orion::AnimationTypes::Death: return true;
			case Orion::AnimationTypes::Walk: return true;
			case Orion::AnimationTypes::Idle: return true;
			}
			return false;
			break;
		case Orion::AnimationTypes::Death:
			switch (outgoingAnim)
			{
			case Orion::AnimationTypes::Reborn: return true;
			}
			return false;
			break;
		case Orion::AnimationTypes::Idle:
			switch (outgoingAnim)
			{
			case Orion::AnimationTypes::Walk: return true;
			case Orion::AnimationTypes::Death: return true;
			case Orion::AnimationTypes::Jump: return true;
			case Orion::AnimationTypes::Idle: return false;
			case Orion::AnimationTypes::Reborn: return false;
			}
			break;
		case Orion::AnimationTypes::Jump:
			switch (outgoingAnim)
			{
			case Orion::AnimationTypes::Reborn: return false;
				return true;
			}
			break;
		case Orion::AnimationTypes::Run:

			break;

		case Orion::AnimationTypes::Attack:
			switch (outgoingAnim)
			{
			case Orion::AnimationTypes::Walk: return true;
			case Orion::AnimationTypes::Death: return true;
			case Orion::AnimationTypes::Jump: return true;
			case Orion::AnimationTypes::Idle: return true;
			}
			break;

		case Orion::AnimationTypes::Walk:
			switch (outgoingAnim)
			{
			case Orion::AnimationTypes::Death: return true;
			case Orion::AnimationTypes::Idle: return true;
			case Orion::AnimationTypes::Reborn: return false;
			}
			break;
		}
	}*/

	void Skeleton::Move(Orion::Timestep ts)
	{
		float speed = 2.0f * ts;
		//accumulator += ts.GetMilliseconds();
		//ORI_CORE_INFO(accumulator);
		//m_SkeletonWalk->PlaybackSpeed = 3.0f;
		static float accumulator = 0;
		if (Orion::Input::IsKeyPressed(ORI_KEY_E) && !m_IsDead && !m_SkeletonReborn->IsRunning)
		{
			m_IsDead = true;
			m_StateMachine.TryActivateAnimationNow(m_SkeletonDeath, true);
		}
		else if (!m_IsDead && !m_SkeletonReborn->IsRunning)
		{

			if (m_IsGrounded) 
			{
					if (!m_IsAttacking && !m_IsWalking) {
						accumulator += ts.GetMilliseconds();
					}
					else 
					{
						m_IsWaiting = false;
						accumulator = 0;
					}

					if (Orion::Input::IsMouseButtonPressed(ORI_MOUSE_BUTTON_1))
					{
						m_IsAttacking = true;
						m_StateMachine.TryActivateAnimationNow(m_SkeletonAttack, true, &m_IsAttacking);
					}
					if(!m_IsAttacking) {

						if (Orion::Input::IsKeyPressed(ORI_KEY_RIGHT))
						{
							m_IsWalking = true;
							m_StateMachine.SetFlipForAllAnimation(false);
							m_StateMachine.TryActivateAnimation(m_SkeletonWalk, false, &m_IsWalking);
							m_Position.x += speed;
						}
						else if (Orion::Input::IsKeyPressed(ORI_KEY_LEFT))
						{
							m_IsWalking = true;
							m_StateMachine.SetFlipForAllAnimation(true);
							m_StateMachine.TryActivateAnimation(m_SkeletonWalk, false, &m_IsWalking);
							m_Position.x -= speed;
						}
						else
						{
							m_IsWalking = false;

						}
					}

					if(accumulator > 3000.f)
					{
						m_IsWaiting = true;
						m_StateMachine.TryActivateAnimation(m_SkeletonIdle, false, &m_IsWaiting);
					}
			}
		}
		else if (Orion::Input::IsKeyPressed(ORI_KEY_Q) && m_IsDead && !m_SkeletonDeath->IsRunning)
		{
			m_IsDead = false;
			m_StateMachine.TryActivateAnimation(m_SkeletonReborn, true);
		}
		


		

	}

}