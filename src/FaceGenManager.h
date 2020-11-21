#pragma once

class FaceGenManager
{
public:
	static void InstallFaceDiscolorationFix();
	static void EnableFaceGenLogging();
	static void IgnorePreprocessedFaceGen();
	static void SkipHeadPartsValidation();
	static void ScanOnStartup();

	static bool GetFaceGenModelPath(RE::TESNPC* a_actor, char* a_buffer);
	static bool LoadFaceGen(
		RE::TESNPC* a_actor,
		RE::BSFaceGenNiNode** a_node,
		RE::BSResource::ID** a_resourceID);
	static bool ModelExistsInDB(const char* a_path);

private:
	FaceGenManager() = default;
	static bool DataLoad_CheckRace(RE::TESNPC* a_actor);
	static bool ValidateFaceGen(RE::TESNPC* a_actor);

	static char Hook_GenerateFaceGen(
		RE::TESRace* a_race,
		RE::BSFaceGenNiNode** a_node,
		RE::TESNPC* a_actor,
		int a_arg4,
		int a_arg5);

	static bool Hook_FinishInitNPC(RE::TESNPC* a_actor);

	inline static REL::Relocation<decltype(Hook_GenerateFaceGen)> _GenerateFaceGen;
	inline static REL::Relocation<bool(RE::TESNPC*)> _FinishInitNPC;

};
