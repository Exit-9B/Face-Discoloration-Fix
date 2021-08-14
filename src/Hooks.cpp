#include "Hooks.h"

namespace Hooks
{
	void Install()
	{
		FaceGenManager::InstallFaceDiscolorationFix();

		// Don't use this, it basically just disables the INI setting to use preprocessed facegen
		//FaceGenManager::InstallIgnorePreprocessedFaceGen();
	}
}
