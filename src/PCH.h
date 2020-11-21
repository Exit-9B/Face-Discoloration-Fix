#pragma once

#define XBYAK64
#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

struct LPNLSVERSIONINFO;
#include <stringapiset.h>
#include "AutoTOML.hpp"

#ifdef NDEBUG
#include <spdlog/sinks/basic_file_sink.h>
#else
#include <spdlog/sinks/msvc_sink.h>
#endif

using namespace std::literals;

namespace logger = SKSE::log;

#define DLLEXPORT __declspec(dllexport)
