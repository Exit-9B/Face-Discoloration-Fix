#pragma once

class FaceGenManager
{
public:
	static void InstallFaceDiscolorationFix();
	static void InstallIgnorePreprocessedFaceGen();
	static void InstallPerformanceTimers();

private:
	FaceGenManager() = default;
	static bool DataLoad_CheckRace(RE::TESNPC* a_actor);

	static char PerformanceTimer_GetHeadModel(
		RE::TESNPC* a_actor, void* a_arg2, void* a_arg3);

	inline static REL::Relocation<decltype(PerformanceTimer_GetHeadModel)> _GetHeadModel;
};
