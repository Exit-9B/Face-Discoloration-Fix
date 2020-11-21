#pragma once

class ScanOnStartupManager
{
public:
	ScanOnStartupManager() = delete;

	static void InstallHooks();

	static bool GetFaceGenModelPath(RE::TESNPC* a_actor, char* a_buffer);

	static bool LoadFaceGen(
		RE::TESNPC* a_actor,
		RE::BSFaceGenNiNode** a_node,
		RE::BSResource::ID** a_resourceID);

	static bool ModelExistsInDB(const char* a_path);

private:
	static bool ValidateFaceGen(RE::TESNPC* a_actor);

	static bool Hook_FinishInitNPC(RE::TESNPC* a_actor);

	inline static REL::Relocation<bool(RE::TESNPC*)> _FinishInitNPC;
};
