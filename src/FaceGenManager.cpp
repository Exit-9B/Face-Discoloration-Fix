#include "FaceGenManager.h"
#include "Offsets.h"

inline static std::array<uint8_t, 2> nop2{ 0x66, 0x90 };
inline static std::array<uint8_t, 6> nop6{ 0x66, 0x0F, 0x1F, 0x44, 0x00, 0x00 };
inline static std::array<uint8_t, 1> jcc2_to_jmp{ 0xEB };
inline static std::array<uint8_t, 2> jcc6_to_jmp{ 0x90, 0xE9 };

bool FaceGenManager::DataLoad_CheckRace(RE::TESNPC* a_actor)
{
	auto race = a_actor->race;
	if (!race)
	{
		logger::warn(
			"NPC '{}' ({:8x}) has no race.",
			a_actor->fullName,
			a_actor->formID);

		return false;
	}

	return a_actor->tintLayers && a_actor->tintLayers->size() > 0;
}

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
	static REL::Relocation<std::uintptr_t> hook_tint{ Offset::BSFaceGenDB_GenerateHeadPartModel, 0x3A4 };
	REL::safe_write(hook_tint.address(), nop2);

	static REL::Relocation<std::uintptr_t> hook_dataload{ Offset::TESNPC_InitializeAfterLoad, 0x730 };

	auto& trampoline = SKSE::GetTrampoline();

	struct Patch : Xbyak::CodeGenerator
	{
		Patch(std::uintptr_t addr)
		{
			Xbyak::Label label;
			L(label);

			mov(rcx, r15);
			mov(rax, addr);
			call(rax);
			test(al, al);
			jz(label.getAddress() + 0x106);
			mov(r12d, r14d);
			nop(6);
		}
	};

	Patch patch{ SKSE::unrestricted_cast<std::uintptr_t>(DataLoad_CheckRace) };
	patch.ready();
	assert(patch.getSize() == 0x20);

	REL::safe_write(hook_dataload.address(), patch.getCode(), patch.getSize());

	logger::info("Installed hooks for face discoloration fix");
}

void FaceGenManager::InstallIgnorePreprocessedFaceGen()
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
