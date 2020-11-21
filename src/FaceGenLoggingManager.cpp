#include "FaceGenLoggingManager.h"
#include "Offsets.h"

void FaceGenLoggingManager::InstallHooks()
{
	auto& trampoline = SKSE::GetTrampoline();
	static REL::Relocation<std::uintptr_t> hook_facegen{ Offset::TESNPC_GetHeadModel, 0x115 };
	_GenerateFaceGen = trampoline.write_call<5>(hook_facegen.address(), Hook_GenerateFaceGen);

	logger::info("Installed hooks for logging head generation");
}

char FaceGenLoggingManager::Hook_GenerateFaceGen(
	RE::TESRace* a_race,
	RE::BSFaceGenNiNode** a_node,
	RE::TESNPC* a_actor,
	int a_arg4,
	int a_arg5)
{
	if (!a_actor->IsPlayer())
	{
		logger::info(
			"Generating FaceGen data for NPC '{}' ({:08x})",
			a_actor->GetFullName(),
			a_actor->GetFormID());
	}

	return _GenerateFaceGen(a_race, a_node, a_actor, a_arg4, a_arg5);
}