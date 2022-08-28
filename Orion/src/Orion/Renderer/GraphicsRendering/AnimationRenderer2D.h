#pragma once


#include"Orion/Core/TimeHelper.h"
#include"Orion/Core/AnimationLib2D/AnimationLibrary.h"	



namespace Orion {


	enum class AnimationTypes
	{
		Walk, Idle, Jump, Run, Shoot, Damage, Death, Reborn, Attack, Whoa
	};
	struct Animation2D
	{
		struct Specification
		{
			glm::vec3* Position;
			glm::vec2* Size;
			glm::vec4* Color;
	
			
		};
			
		Animation2D(const Shared<AnimationSource>& src, AnimationTypes type,  bool reverse = false) 
			: AnimSource(src), AnimationType(type), m_IsReverse(reverse)
		{
			if (reverse)
			{
				FrameIndex = src->TotalFrames - 1;
				AnimationName = src->AnimationName + "Reverse";
			}else
			{
				AnimationName = src->AnimationName;

			}

		};
	
		void Reset() 
		{
			if (m_IsReverse) 
			{
				if (IsFlip)
				{
					FrameIndex = 0;
				}
				else { FrameIndex = FrameIndex = AnimSource->TotalFrames - 1;; }
			}
			else 
			{
				if (IsFlip)
				{
					FrameIndex = AnimSource->TotalFrames - 1;
				}
				else { FrameIndex = 0; }
			}
			BindedState = nullptr;
		}

		void Flip(bool flip)
		{
			if (flip != IsFlip) {
				IsFlip = flip;
				FrameIndex = (AnimSource->TotalFrames - 1) - FrameIndex;
			}
		}

		bool IsReverse() { return m_IsReverse; }
		bool IsLooping() { return m_IsLooping; }
		std::string AnimationName;
		AnimationTypes AnimationType;
		int32_t FrameIndex = 0;
		float PlaybackSpeed = 1.0f;
		float Accumulator = 0;
		Specification Spec;
		bool* BindedState = nullptr;
		bool PlayToEnd = false;
		bool InitialBindedState;
		bool IsRunning = false;
		bool IsFlip = false;
		const Shared<AnimationSource> AnimSource;
	private: 
		bool m_IsLooping = false;
		bool m_IsReverse = false;
	};


	class AnimationStateMachine
	{

		using StateTemplateFunction = std::function<bool(const AnimationTypes&, const AnimationTypes&)>;
		typedef bool (RawStateFunction)(const AnimationTypes&, const AnimationTypes&);

	public:
		void UpdateActiveAnimation();
		void SetStateFunction(StateTemplateFunction fnc);
		void TryActivateAnimationNow(const Shared<Animation2D>& anim, bool playUntilEnd, bool* StateToBind = nullptr);
		void TryActivateAnimation(const Shared<Animation2D>& anim, bool playUntilEnd, bool* StateToBind = nullptr);
		void AddAnimation(Shared<Animation2D>& anim);
	
		void BindSpecToAnimation(glm::vec3* position, glm::vec2* size, glm::vec4* color);
		void SetFlipForAllAnimation(bool flip);




		std::string GetActiveAnimationName();
		Shared<Animation2D> GetActiveAnimation();
	private:
		bool FirstAnimation = true;
		StateTemplateFunction m_StateFunc;
		std::unordered_map <std::string, Shared<Animation2D>> m_AnimationStorage;
		std::string m_ActiveAnimation;
	};



	class AnimationRenderer2D
	{
	public:
	
		static void Play(const Shared<Animation2D>& animation);

		static void PlayReverse(const Shared<Animation2D>& animation);
		static void RenderAnimation(const Shared<Animation2D>& animation);


		static void SetDelta(Timestep ts) { deltaTime = ts.GetMilliseconds(); }
	private: 
		static float deltaTime;
	};

}

