#include "IgnorePreprocessedFaceGenManager.h"
#include "Offsets.h"
#include "Patches.h"

using namespace Patches;

void IgnorePreprocessedFaceGenManager::InstallHooks()
{
	// Always generate facegen
	static REL::Relocation<std::uintptr_t> hook_getmodel1{ Offset::TESNPC_GetHeadModel, 0x61 };
	static REL::Relocation<std::uintptr_t> hook_getmodel2{ Offset::TESNPC_GetHeadModel, 0x8A };
	REL::safe_write(hook_getmodel1.address(), jcc6_to_jmp);
	REL::safe_write(hook_getmodel2.address(), jcc2_to_jmp);

	// Loading from DB
	static REL::Relocation<std::uintptr_t> hook_load{ Offset::BSFaceGenDB_LoadFromDB, 0x17D };
	REL::safe_write(hook_load.address(), nop2);

	// No model unload
	static REL::Relocation<std::uintptr_t> hook_unload{ Offset::HighProcessData_Unload, 0x26C };
	REL::safe_write(hook_unload.address(), jcc2_to_jmp);

	logger::info("Installed hooks for ignoring preprocessed FaceGen");
}