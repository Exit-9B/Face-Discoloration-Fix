#pragma once

#include "FaceGenManager.h"

namespace Hooks
{
	inline static bool EnableFaceDiscolorationFix = true;
	inline static bool IgnorePreprocessedFacegen = false;

	void Install();
}
