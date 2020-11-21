#pragma once

class FaceDiscolorationManager
{
public:
	FaceDiscolorationManager() = delete;

	static void InstallHooks();

private:
	static bool DataLoad_CheckRace(RE::TESNPC* a_actor);
};
