#include "ScanOnStartupManager.h"
#include "Offsets.h"

void ScanOnStartupManager::InstallHooks()
{
	auto& trampoline = SKSE::GetTrampoline();

	static REL::Relocation<std::uintptr_t> hook_postload
	{ Offset::DataHandler_InitializeActorAfterLoad, 0x1DA };

	_FinishInitNPC = trampoline.write_call<5>(hook_postload.address(), Hook_FinishInitNPC);

	logger::info("Installed hooks for scanning on startup");
}

bool ScanOnStartupManager::GetFaceGenModelPath(RE::TESNPC* a_actor, char* a_buffer)
{
	using func_t = decltype(GetFaceGenModelPath);
	static REL::Relocation<func_t> func{ Offset::TESNPC_GetFaceGenModelPath };
	return func(a_actor, a_buffer);
}

bool ScanOnStartupManager::LoadFaceGen(
	RE::TESNPC* a_actor,
	RE::BSFaceGenNiNode** a_node,
	RE::BSResource::ID** a_resourceID)
{
	using func_t = decltype(LoadFaceGen);
	static REL::Relocation<func_t> func{ Offset::TESNPC_LoadFaceGen };
	return func(a_actor, a_node, a_resourceID);
}

bool ScanOnStartupManager::ModelExistsInDB(const char* a_path)
{
	using func_t = decltype(ModelExistsInDB);
	static REL::Relocation<func_t> func{ Offset::ModelExistsInDB };
	return func(a_path);
}

bool ScanOnStartupManager::ValidateFaceGen(RE::TESNPC* a_actor)
{
	if (a_actor->faceNPC)
	{
		return true;
	}

	if (a_actor->actorData.actorBaseFlags.any(
		RE::ACTOR_BASE_DATA::Flag::kIsChargenFacePreset))
	{
		return true;
	}

	if (!a_actor->race || !a_actor->race->data.flags.all(RE::RACE_DATA::Flag::kFaceGenHead))
	{
		return true;
	}

	char path[MAX_PATH];
	if (!GetFaceGenModelPath(a_actor, path))
	{
		logger::critical(
			"Failed to get FaceGen model path for NPC '{}' ({:08x})",
			a_actor->GetFullName(),
			a_actor->GetFormID());

		return false;
	}

	if (!ModelExistsInDB(path))
	{
		auto filename = a_actor->GetFile(0)->fileName;
		bool silent =
			strcmp(filename, "Skyrim.esm") == 0 ||
			strcmp(filename, "Dawnguard.esm") == 0 ||
			strcmp(filename, "Dragonborn.esm") == 0;

		if (!silent)
		{
			logger::warn(
				"Missing FaceGen model for NPC '{}' ({:08x})",
				a_actor->GetFullName(),
				a_actor->GetFormID());
		}

		return false;
	}

	RE::BSFaceGenNiNode* faceNode = nullptr;
	RE::BSResource::ID* id = nullptr;

	bool isFaceGenValid = LoadFaceGen(a_actor, &faceNode, &id);

	if (!isFaceGenValid)
	{
		logger::warn(
			"Invalid FaceGen data for NPC '{}' ({:08x})",
			a_actor->GetFullName(),
			a_actor->GetFormID());

		return false;
	}

	if (faceNode)
	{
		faceNode->DeleteThis();
	}

	return true;
}

bool ScanOnStartupManager::Hook_FinishInitNPC(RE::TESNPC* a_actor)
{
	ValidateFaceGen(a_actor);
	return _FinishInitNPC(a_actor);
}