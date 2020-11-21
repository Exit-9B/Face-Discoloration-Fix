#pragma once

class FaceGenManager
{
public:
	struct RequestModelFlags
	{
		std::int64_t flags_0 = 0x300000000;
		std::int32_t flags_8 = 0x1000100;
	};
	struct ValidateFaceGenData_Params
	{
		RE::BSFaceGenNiNode* faceGenNode;
		RE::TESNPC* actor;
		bool* isValid;
	};

	static void InstallFaceDiscolorationFix();
	static void EnableFaceGenLogging();
	static void IgnorePreprocessedFaceGen();
	static void SkipHeadPartsValidation();
	static void ScanOnStartup();

	static bool GetFaceGenModelPath(RE::TESNPC* a_actor, char* a_buffer);
	static bool LoadFaceGen(
		RE::TESNPC* a_actor,
		RE::BSFaceGenNiNode** a_node,
		void** a_arg3);
	static bool ModelExistsInDB(const char* a_path);
	static bool ValidateFaceGenData(RE::TESNPC* a_actor, ValidateFaceGenData_Params* a_data);

	static void ValidateAllFaceGenData();

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
