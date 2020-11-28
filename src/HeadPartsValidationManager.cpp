#include "HeadPartsValidationManager.h"
#include "Offsets.h"
#include "Patches.h"

using namespace Patches;

void HeadPartsValidationManager::InstallHooks()
{
	static REL::Relocation<std::uintptr_t> hook_verify{ Offset::TESNPC_LoadFaceGen, 0x4A9 };

	REL::safe_write(hook_verify.address(), nop6);

	logger::info("Installed hooks for disabling FaceGen verification");
}

