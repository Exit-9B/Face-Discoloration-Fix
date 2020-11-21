#include "FaceGenManager.h"
#include "Offsets.h"

inline static std::array<uint8_t, 2> nop2{ 0x66, 0x90 };
inline static std::array<uint8_t, 6> nop6{ 0x66, 0x0F, 0x1F, 0x44, 0x00, 0x00 };
inline static std::array<uint8_t, 1> jcc2_to_jmp{ 0xEB };
inline static std::array<uint8_t, 2> jcc6_to_jmp{ 0x90, 0xE9 };

void FaceGenManager::InstallFaceDiscolorationFix()
{
	// Load tint layers for all NPCs
	static REL::Relocation<std::uintptr_t> hook_TINC{ Offset::TESNPC_ReadFromFileStream, 0x2EB };
	static REL::Relocation<std::uintptr_t> hook_TINI{ Offset::TESNPC_ReadFromFileStream, 0x81A };
	static REL::Relocation<std::uintptr_t> hook_TIAS{ Offset::TESNPC_ReadFromFileStream, 0x11E1 };
	static REL::Relocation<std::uintptr_t> hook_TINV{ Offset::TESNPC_ReadFromFileStream, 0x1594 };

	REL::safe_write(hook_TIAS.address(), nop6);
	REL::safe_write(hook_TINV.address(), nop6);
	REL::safe_write(hook_TINC.address(), nop6);
	REL::safe_write(hook_TINI.address(), nop6);

	// Fix face discoloration
	static REL::Relocation<std::uintptr_t> hook_init{ Offset::TESNPC_FinishInit, 0x2E };
	REL::safe_write(hook_init.address(), jcc6_to_jmp);

	// Fix dark face
	static REL::Relocation<std::uintptr_t> hook_tint
	{ Offset::BSFaceGenDB_GenerateHeadPartModel, 0x3A4 };

	REL::safe_write(hook_tint.address(), nop2);

	// Workaround for immediate crash with broken mods with bad race (RNAM) data
	static REL::Relocation<std::uintptr_t> hook_dataload
	{ Offset::TESNPC_InitializeAfterLoad, 0x730 };

	struct Nop32 : Xbyak::CodeGenerator {
		Nop32() {
			nop(32, false);
		}
	};

	Nop32 nop32;
	nop32.ready();
	assert(nop32.getSize() == 0x20);

	struct Patch : Xbyak::CodeGenerator {
		Patch(std::uintptr_t addr) : Xbyak::CodeGenerator(0x20) {
			Xbyak::Label label;
			L(label);

			mov(rcx, r15);
			mov(rax, addr);
			call(rax);
			test(al, al);
			jz(label.getAddress() + 0x106);
			mov(r12d, r14d);
		}
	};

	Patch patch{ SKSE::unrestricted_cast<std::uintptr_t>(DataLoad_CheckRace) };
	patch.ready();

	REL::safe_write(hook_dataload.address(), nop32.getCode(), nop32.getSize());
	REL::safe_write(hook_dataload.address(), patch.getCode(), patch.getSize());

	logger::info("Installed hooks for face discoloration fix");
}

void FaceGenManager::EnableFaceGenLogging()
{
	auto& trampoline = SKSE::GetTrampoline();
	static REL::Relocation<std::uintptr_t> hook_facegen{ Offset::TESNPC_GetHeadModel, 0x115 };
	_GenerateFaceGen = trampoline.write_call<5>(hook_facegen.address(), Hook_GenerateFaceGen);

	logger::info("Installed hooks for logging head generation");
}

void FaceGenManager::IgnorePreprocessedFaceGen()
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

void FaceGenManager::SkipHeadPartsValidation()
{
	static REL::Relocation<std::uintptr_t> hook_verify{ Offset::TESNPC_LoadFaceGen, 0x4A9 };

	REL::safe_write(hook_verify.address(), nop6);

	logger::info("Installed hooks for disabling FaceGen verification");
}

void FaceGenManager::ScanOnStartup()
{
	auto& trampoline = SKSE::GetTrampoline();

	static REL::Relocation<std::uintptr_t> hook_postload
	{ Offset::DataHandler_InitializeActorAfterLoad, 0x1DA };

	_FinishInitNPC = trampoline.write_call<5>(hook_postload.address(), Hook_FinishInitNPC);

	logger::info("Installed hooks for scanning on startup");
}

bool FaceGenManager::GetFaceGenModelPath(RE::TESNPC* a_actor, char* a_buffer)
{
	using func_t = decltype(GetFaceGenModelPath);
	static REL::Relocation<func_t> func{ Offset::TESNPC_GetFaceGenModelPath };
	return func(a_actor, a_buffer);
}

bool FaceGenManager::LoadFaceGen(
	RE::TESNPC* a_actor,
	RE::BSFaceGenNiNode** a_node,
	void** a_arg3)
{
	using func_t = decltype(LoadFaceGen);
	static REL::Relocation<func_t> func{ Offset::TESNPC_LoadFaceGen };
	return func(a_actor, a_node, a_arg3);
}

bool FaceGenManager::ModelExistsInDB(const char* a_path)
{
	using func_t = decltype(ModelExistsInDB);
	static REL::Relocation<func_t> func{ Offset::ModelExistsInDB };
	return func(a_path);
}

bool FaceGenManager::ValidateFaceGenData(
	RE::TESNPC* a_actor,
	ValidateFaceGenData_Params* a_data)
{
	using func_t = decltype(ValidateFaceGenData);
	static REL::Relocation<func_t> func{ Offset::TESNPC_ValidateFaceGenData };
	return func(a_actor, a_data);
}

void FaceGenManager::ValidateAllFaceGenData()
{
	auto dataHandler = RE::TESDataHandler::GetSingleton();
	auto npcArray = dataHandler->GetFormArray<RE::TESNPC>();

	for (auto npc : npcArray)
	{
		ValidateFaceGen(npc);
	}
}

bool FaceGenManager::DataLoad_CheckRace(RE::TESNPC* a_actor)
{
	auto race = a_actor->race;
	if (!race)
	{
		logger::error(
			"NPC '{}' ({:08x}) has no race.",
			a_actor->GetFullName(),
			a_actor->GetFormID());

		return false;
	}

	return a_actor->tintLayers && a_actor->tintLayers->size() > 0;
}

char FaceGenManager::Hook_GenerateFaceGen(
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

bool FaceGenManager::Hook_FinishInitNPC(RE::TESNPC* a_actor)
{
	ValidateFaceGen(a_actor);
	return _FinishInitNPC(a_actor);
}

bool FaceGenManager::ValidateFaceGen(RE::TESNPC* a_actor)
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
		auto filename = a_actor->GetFile()->fileName;
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
	void* a3 = nullptr;

	bool isFaceGenValid = LoadFaceGen(a_actor, &faceNode, &a3);

	if (faceNode)
	{
		faceNode->DeleteThis();
	}

	if (!isFaceGenValid)
	{
		logger::warn(
			"Invalid FaceGen data for NPC '{}' ({:08x})",
			a_actor->GetFullName(),
			a_actor->GetFormID());

		return false;
	}

	return true;
}

