#include "oripch.h"
#include "AnimationRenderer2D.h"
#include "Renderer2D.h"

namespace Orion
{
	float AnimationRenderer2D::deltaTime = 0.f;

	void AnimationRenderer2D::Play(const Shared<Animation2D>& animation)
	{

		if (animation->IsRunning)
		{
			if (animation->IsReverse())
			{
				PlayReverse(animation);
				return;
			}
			if (animation->BindedState && *animation->BindedState != animation->InitialBindedState && !animation->PlayToEnd)
			{
				animation->IsRunning = false;
				animation->Reset();
			}
			
				int32_t& frameIndex = animation->FrameIndex;
				float& accumulator = animation->Accumulator;
				const uint32_t& totalFrames = animation->AnimSource->TotalFrames;
				accumulator += deltaTime;
				//ORI_CORE_ASSERT(!(frameIndex >= animation->FrameCount), "Iterator index is higher than frame count!!");

				float DELAY = 100.0f / animation->PlaybackSpeed; //100 ms

				if (animation->IsFlip)
				{
					if (accumulator > DELAY) {
						frameIndex--;
						accumulator = 0;
					}
					if (frameIndex <= 0)
					{
						frameIndex = 0;
						if (animation->BindedState) {
							*animation->BindedState = !animation->InitialBindedState;
						}
						animation->IsRunning = false;
					}
				}
				else
				{
					if (accumulator > DELAY) {
						frameIndex++;
						accumulator = 0;
					}

					if (frameIndex >= totalFrames - 1)
					{
						frameIndex = totalFrames - 1;
						if (animation->BindedState) {
							*animation->BindedState = !animation->BindedState;
						}
						animation->IsRunning = false;
					}
				}
			
			

			ORI_CORE_INFO("NORMAL: Name: {0}, ID: {1}", animation->AnimationName, frameIndex);
		}
	}

	void AnimationRenderer2D::PlayReverse(const Shared<Animation2D>& animation)
	{
		

		
			if (animation->IsRunning)
			{

				if (animation->BindedState && *animation->BindedState != animation->InitialBindedState && !animation->PlayToEnd)
				{
					animation->IsRunning = false;
					animation->Reset();
				}
			
				int32_t& frameIndex = animation->FrameIndex;
				float& accumulator = animation->Accumulator;
				const uint32_t& totalFrames = animation->AnimSource->TotalFrames;
				accumulator += deltaTime;
				//ORI_CORE_ASSERT(!(frameIndex >= animation->FrameCount), "Iterator index is higher than frame count!!");

				float DELAY = 100.0f / animation->PlaybackSpeed; //100 ms

				if (animation->IsFlip)
				{
					if (accumulator > DELAY) {
						frameIndex++;
						accumulator = 0;
					}

					if (frameIndex >= totalFrames - 1) {
						frameIndex = totalFrames - 1;
						if (animation->BindedState) {
							*animation->BindedState = !animation->BindedState;
						}
						animation->IsRunning = false;
					}
				}
				else
				{
					if (accumulator > DELAY) {
						frameIndex--;
						accumulator = 0;
					}
					if (frameIndex <= 0)
					{
						frameIndex = 0;
						if (animation->BindedState) {
							*animation->BindedState = !animation->BindedState;
						}
						animation->IsRunning = false;
					}
				}
			


			ORI_CORE_INFO("NORMAL: Name: {0}, ID: {1}", animation->AnimationName, frameIndex);

			//Orion::Renderer2D::DrawTexturedQuad(animP.Spec.Position, animP.Spec.Size, animation->Frames[frameIndex], animP.Spec.Color, animP.IsFlip);
		}
	}
	void AnimationRenderer2D::RenderAnimation(const Shared<Animation2D>& animation)
	{
		Orion::Renderer2D::DrawTexturedQuad
		(
			*animation->Spec.Position,
			*animation->Spec.Size,
			animation->AnimSource->Frames[animation->FrameIndex],
			*animation->Spec.Color,
			animation->IsFlip
		);
	}

	//Animation State Machine

	void AnimationStateMachine::TryActivateAnimation(const Shared<Animation2D>& anim, bool playUntilEnd, bool* StateToBind)
	{
		std::string& name = anim->AnimationName;

		if (FirstAnimation)
		{
			m_ActiveAnimation = name;
			anim->IsRunning = true;
			if (StateToBind)
			{
				anim->BindedState = StateToBind;
				anim->InitialBindedState = *StateToBind;
			}
			FirstAnimation = false;
			return;
		}



		if (!m_AnimationStorage[m_ActiveAnimation]->IsRunning)
		{
			anim->Reset();
			if (StateToBind)
			{
				anim->BindedState = StateToBind;
				anim->InitialBindedState = *StateToBind;
			}
			anim->IsRunning = true;
			anim->PlayToEnd = playUntilEnd;
			m_ActiveAnimation = name;
		}
	
	}

	void AnimationStateMachine::TryActivateAnimationNow(const Shared<Animation2D>& anim, bool playUntilEnd, bool* StateToBind)
	{
		std::string& name = anim->AnimationName;

		if (FirstAnimation)
		{
			m_ActiveAnimation = name;
			anim->IsRunning = true;
			if (StateToBind)
			{
				anim->BindedState = StateToBind;
				anim->InitialBindedState = *StateToBind;
			}
			FirstAnimation = false;
			return;
		}

		if (name != m_ActiveAnimation) 
		{
			anim->Reset();
			if (StateToBind)
			{
				anim->BindedState = StateToBind;
				anim->InitialBindedState = *StateToBind;
			}
			anim->IsRunning = true;
			anim->PlayToEnd = playUntilEnd;
			m_ActiveAnimation = name;
		}
		else 
		{
			if (!anim->IsRunning)
			{
				anim->Reset();
				if (StateToBind)
				{
					anim->BindedState = StateToBind;
					anim->InitialBindedState = *StateToBind;
				}
				anim->IsRunning = true;
				anim->PlayToEnd = playUntilEnd;
				anim->BindedState = StateToBind;
			}

		}
	}

	void AnimationStateMachine::BindSpecToAnimation(glm::vec3* position, glm::vec2* size, glm::vec4* color)
	{
		for (auto& anim : m_AnimationStorage)
		{
			anim.second->Spec.Position = position;
			anim.second->Spec.Size = size;
			anim.second->Spec.Color = color;
		}
	}

	void AnimationStateMachine::SetFlipForAllAnimation(bool flip)
	{
		for (auto& anim : m_AnimationStorage)
		{
			anim.second->IsFlip = flip;
		}
	}

	void AnimationStateMachine::AddAnimation(Shared<Animation2D>& anim)
	{
		const std::string& name = anim->AnimationName;

		m_AnimationStorage.insert({ name, anim });

	
	}

	void AnimationStateMachine::UpdateActiveAnimation()
	{
		if (m_AnimationStorage.find(m_ActiveAnimation) != m_AnimationStorage.end()) {
			auto& anim = m_AnimationStorage[m_ActiveAnimation];
			if (anim)
			{
				ORI_CORE_INFO("NORMAL: Name: {0}, ID: {1}, Running: {2}", m_ActiveAnimation, anim->FrameIndex, anim->IsRunning);
				AnimationRenderer2D::Play(anim);
				AnimationRenderer2D::RenderAnimation(anim);
			}
		}
	}

	void AnimationStateMachine::SetStateFunction(StateTemplateFunction fnc)
	{
		m_StateFunc = fnc;
	}

	std::string AnimationStateMachine::GetActiveAnimationName()
	{
		return m_ActiveAnimation;
	}

	Shared<Animation2D> AnimationStateMachine::GetActiveAnimation()
	{
		return m_AnimationStorage[m_ActiveAnimation];
	}
}