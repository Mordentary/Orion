#include"oripch.h"
#include "TimeHelper.h"
#include <Orion/Core/Application.h>



namespace Orion
{
    int32_t CurrentTime::GetCurrentTimeInSec_Int()
    {
        return Orion::Application::Get().GetWindow().GetCurrentTimeInSec_Int();
    }
    float CurrentTime::GetCurrentTimeInSec()
    {
        return Orion::Application::Get().GetWindow().GetCurrentTimeInSec_Float();

    }

}