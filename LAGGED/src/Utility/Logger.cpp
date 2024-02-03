#include "Logger.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "Platform/Base/PlatformIncludes.h"

#include "Core/Defines.h"


namespace LAG::Logger
{
    struct LoggerData
    {
        std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> consoleSink;
        std::shared_ptr<spdlog::sinks::basic_file_sink_mt> fileSink;

    } *loggerData = nullptr;
}

void LAG::Logger::Initialize()
{
    //Set up the basics
    spdlog::set_pattern("[%H:%M:%S %z][%^%l%$] %v");

    loggerData = new LoggerData();
    loggerData->consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    loggerData->consoleSink->set_level(spdlog::level::trace);

    loggerData->fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("Logs/LatestLog.txt", true);
    loggerData->fileSink->set_level(spdlog::level::info);
    loggerData->fileSink->set_pattern("[%H:%M:%S %z][%^%l%$] %v");

    spdlog::set_default_logger(std::make_shared<spdlog::logger>("DefaultLogger", spdlog::sinks_init_list({ loggerData->consoleSink, loggerData->fileSink })));
    spdlog::flush_every(std::chrono::seconds(5));
}

void LAG::Logger::Shutdown()
{
    //TODO: Causes a crash. Look into this at some point
    //spdlog::shutdown();
}