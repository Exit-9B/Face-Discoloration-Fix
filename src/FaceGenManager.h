#pragma once

class FaceGenManager
{
public:
	static void InstallFaceDiscolorationFix();
	static void InstallIgnorePreprocessedFaceGen();

private:
	FaceGenManager() = default;
	static bool DataLoad_CheckRace(RE::TESNPC* a_actor);

};
