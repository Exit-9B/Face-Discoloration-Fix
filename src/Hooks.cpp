#include "Hooks.h"
#include "Settings.h"

namespace Hooks
{
	void Install()
	{
		FaceGenManager::InstallFaceDiscolorationFix();

		if (*Settings::bEnableFaceGenLogging)
		{
			FaceGenManager::EnableFaceGenLogging();
		}

		if (*Settings::bIgnorePreprocessedFaceGen)
		{
			FaceGenManager::IgnorePreprocessedFaceGen();
		}

		if (*Settings::bSkipHeadPartsValidation)
		{
			FaceGenManager::SkipHeadPartsValidation();
		}

		if (*Settings::bScanOnStartup)
		{
			FaceGenManager::ScanOnStartup();
		}
	}
}
