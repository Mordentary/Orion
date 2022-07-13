#pragma once
#include <chrono>
#include <string>
#include <iostream>
namespace Orion
{
	class Timestep
	{
	public:
		Timestep(float time = 0.0f) : m_Time(time) {}
		float GetMilliseconds() const { return m_Time * 1000.0f; }
		float GetSeconds() const { return m_Time; }

        operator float() const { return m_Time; }
        operator std::string() const { return std::to_string(m_Time); }
	private:
		float m_Time;
	};


    class Timer {

    public:

        Timer(std::string&& description = "") : m_Description(description) 
        {

        };
        ~Timer() {
            End();
        };
        inline void Start() 
        {
            m_Started = true;
            m_Ended = false;
            m_StartTime = std::chrono::high_resolution_clock::now();
        }
        inline bool IsEnded() { return m_Ended; }
        inline void End()
        {
            if (!m_Started) return;

            auto endTime = std::chrono::high_resolution_clock::now();
            auto t_ms = std::chrono::duration<float, std::chrono::milliseconds::period>(endTime - m_StartTime).count();

  
            m_TimeDifference = t_ms;
            m_Ended = true;
            m_Started = false;

            //if (description != "") {
            //    std::cout << description << " took " << duration_us << "us (" << duration_ms << "ms)" << std::endl;
            //}

        }
        inline float GetTimeMilliseconds() { return (m_TimeDifference); }
        inline float GetTimeSeconds() { return (m_TimeDifference/1000); }
    private:
        bool m_Started = false, m_Ended = false;
        float m_TimeDifference = 0;
        std::string m_Description;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;

        
    };



}