#pragma once

#include "FaceGenManager.h"

namespace Hooks
{
	inline static bool EnableFaceGenLogging = false;
	inline static bool IgnorePreprocessedFacegen = false;
	inline static bool SkipHeadPartsValidation = false;
	inline static bool ScanOnStartup = false;

	void Install();
}
