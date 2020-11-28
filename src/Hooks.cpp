#include "Hooks.h"
#include "Settings.h"
#include "FaceDiscolorationManager.h"
#include "FaceGenLoggingManager.h"
#include "IgnorePreprocessedFaceGenManager.h"
#include "HeadPartsValidationManager.h"
#include "ScanOnStartupManager.h"

namespace Hooks
{
	void Install()
	{
		FaceDiscolorationManager::InstallHooks();

		if (*Settings::bEnableFaceGenLogging)
		{
			FaceGenLoggingManager::InstallHooks();
		}

		if (*Settings::bIgnorePreprocessedFaceGen)
		{
			IgnorePreprocessedFaceGenManager::InstallHooks();
		}

		if (*Settings::bSkipHeadPartsValidation)
		{
			HeadPartsValidationManager::InstallHooks();
		}

		if (*Settings::bScanOnStartup)
		{
			ScanOnStartupManager::InstallHooks();
		}
	}
}
