#include "MusePCH.h"

#include "Log.h"
#include "Core/Instrumentor.h"

namespace Muse
{
	std::shared_ptr<spdlog::logger> Log::ms_EngineLogger;
	std::shared_ptr<spdlog::logger> Log::ms_ApplicationLogger;

	void Log::Init()
	{
		MUSE_PROFILE_FUNCTION();

		spdlog::set_pattern("%^[%T] %n: %v%$");
		ms_EngineLogger = spdlog::stdout_color_mt("ENGINE");
		ms_EngineLogger->set_level(spdlog::level::trace);

		ms_ApplicationLogger = spdlog::stdout_color_mt("APP");
		ms_ApplicationLogger->set_level(spdlog::level::trace);
	}
}
