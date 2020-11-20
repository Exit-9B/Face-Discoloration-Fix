#include "Hooks.h"

namespace Hooks
{
	void Install()
	{
		if (EnableFaceDiscolorationFix)
		{
			FaceGenManager::InstallFaceDiscolorationFix();

			if (IgnorePreprocessedFacegen)
			{
				FaceGenManager::InstallIgnorePreprocessedFaceGen();
			}

			FaceGenManager::InstallPerformanceTimers();
		}
	}
}
