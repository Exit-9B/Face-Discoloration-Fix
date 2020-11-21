#pragma once

class FaceGenLoggingManager
{
public:
	FaceGenLoggingManager() = delete;

	static void InstallHooks();

private:
	static char Hook_GenerateFaceGen(
		RE::TESRace* a_race,
		RE::BSFaceGenNiNode** a_node,
		RE::TESNPC* a_actor,
		int a_arg4,
		int a_arg5);

	inline static REL::Relocation<decltype(Hook_GenerateFaceGen)> _GenerateFaceGen;
};
