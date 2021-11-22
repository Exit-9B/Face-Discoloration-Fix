#include "FaceGenManager.h"
#include "Offsets.h"

inline static std::array<std::uint8_t, 2> nop2{ 0x66, 0x90 };
inline static std::array<std::uint8_t, 6> nop6{ 0x66, 0x0F, 0x1F, 0x44, 0x00, 0x00 };
inline static std::array<std::uint8_t, 1> jcc2_to_jmp{ 0xEB };
inline static std::array<std::uint8_t, 2> jcc6_to_jmp{ 0x90, 0xE9 };

#ifndef SKYRIMVR
void FaceGenManager::InstallFaceDiscolorationFix()
{
	// Load tint layers for all NPCs
	static REL::Relocation<std::uintptr_t> hook_TINC{
		Offset::TESNPC::ReadFromFileStream.address() + 0x43C
	};
	static REL::Relocation<std::uintptr_t> hook_TINI{
		Offset::TESNPC::ReadFromFileStream.address() + 0x972
	};
	static REL::Relocation<std::uintptr_t> hook_TIAS{
		Offset::TESNPC::ReadFromFileStream.address() + 0x1637
	};
	static REL::Relocation<std::uintptr_t> hook_TINV{
		Offset::TESNPC::ReadFromFileStream.address() + 0x1A60
	};

	REL::safe_write<std::uint8_t>(hook_TIAS.address(), nop6);
	REL::safe_write<std::uint8_t>(hook_TINV.address(), nop6);
	REL::safe_write<std::uint8_t>(hook_TINC.address(), nop6);
	REL::safe_write<std::uint8_t>(hook_TINI.address(), nop6);

	// Fix face discoloration
	static REL::Relocation<std::uintptr_t> hook_init{ Offset::TESNPC::FinishInit.address() + 0x36 };
	REL::safe_write<std::uint8_t>(hook_init.address(), jcc6_to_jmp);

	// Fix dark face
	static REL::Relocation<std::uintptr_t> hook_tint{
		Offset::BSFaceGenDB::GenerateHeadPartModel.address() + 0x4D4
	};

	REL::safe_write<std::uint8_t>(hook_tint.address(), nop2);

	// Workaround for immediate crash with broken mods with bad race (RNAM) data
	static REL::Relocation<std::uintptr_t> hook_dataload{
		Offset::TESNPC::InitializeAfterLoad.address() + 0x73F
	};

	struct Patch : Xbyak::CodeGenerator
	{
		Patch(std::uintptr_t a_funcAddr)
		{
			Xbyak::Label hookLbl;
			L(hookLbl);

			mov(rcx, r15);
			mov(rax, a_funcAddr);
			call(rax);
			test(al, al);
			jz(hookLbl.getAddress() + 0x10B);
			mov(r12d, esi);
			mov(rax, ptr[r15 + 0x260]);
		}
	};

	Patch patch{ reinterpret_cast<std::uintptr_t>(DataLoad_CheckRace) };
	patch.ready();
	if (patch.getSize() > 0x21) {
		logger::critical("Patch was too large, failed to install"sv);
		// rest of the mod can still work though
	}
	else {
		REL::safe_fill(hook_dataload.address(), REL::NOP, 0x21);
		REL::safe_write(hook_dataload.address(), patch.getCode(), patch.getSize());
	}

	logger::info("Installed hooks for face discoloration fix"sv);
}

#else

void FaceGenManager::InstallFaceDiscolorationFix()
{
	// Load tint layers for all NPCs
	static REL::Relocation<std::uintptr_t> hook_TINC{
		Offset::TESNPC::ReadFromFileStream.address() + 0x2EB
	};
	static REL::Relocation<std::uintptr_t> hook_TINI{
		Offset::TESNPC::ReadFromFileStream.address() + 0x81A
	};
	static REL::Relocation<std::uintptr_t> hook_TIAS{
		Offset::TESNPC::ReadFromFileStream.address() + 0x11E1
	};
	static REL::Relocation<std::uintptr_t> hook_TINV{
		Offset::TESNPC::ReadFromFileStream.address() + 0x1594
	};

	REL::safe_write<std::uint8_t>(hook_TIAS.address(), nop6);
	REL::safe_write<std::uint8_t>(hook_TINV.address(), nop6);
	REL::safe_write<std::uint8_t>(hook_TINC.address(), nop6);
	REL::safe_write<std::uint8_t>(hook_TINI.address(), nop6);

	// Fix face discoloration
	static REL::Relocation<std::uintptr_t> hook_init{ Offset::TESNPC::FinishInit.address() + 0x2E };
	REL::safe_write<std::uint8_t>(hook_init.address(), jcc6_to_jmp);

	// Fix dark face
	static REL::Relocation<std::uintptr_t> hook_tint{
		Offset::BSFaceGenDB::GenerateHeadPartModel.address() + 0x3A4
	};

	REL::safe_write<std::uint8_t>(hook_tint.address(), nop2);

	// Workaround for immediate crash with broken mods with bad race (RNAM) data
	static REL::Relocation<std::uintptr_t> hook_dataload{
		Offset::TESNPC::InitializeAfterLoad.address() + 0x730
	};

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
		}
	};

	Patch patch{ reinterpret_cast<std::uintptr_t>(DataLoad_CheckRace) };
	patch.ready();

	if (patch.getSize() > 0x20) {
		logger::critical("Patch was too large, failed to install"sv);
		// rest of the mod can still work though
	}
	else {
		REL::safe_fill(hook_dataload.address(), REL::NOP, 0x20);
		REL::safe_write(hook_dataload.address(), patch.getCode(), patch.getSize());
	}

	logger::info("Installed hooks for face discoloration fix"sv);
}
#endif

#ifndef SKYRIMVR
void FaceGenManager::InstallIgnorePreprocessedFaceGen()
{
	// Always generate facegen
	static REL::Relocation<std::uintptr_t> hook_getmodel1{
		Offset::TESNPC::GetHeadModel.address() + 0x61
	};
	static REL::Relocation<std::uintptr_t> hook_getmodel2{
		Offset::TESNPC::GetHeadModel.address() + 0x8A
	};
	// (Same function but inlined)
	static REL::Relocation<std::uintptr_t> hook_loadmodel1{
		Offset::TESNPC::GetHeadModel.address() + 0x2B7
	};
	static REL::Relocation<std::uintptr_t> hook_loadmodel2{
		Offset::TESNPC::GetHeadModel.address() + 0x2E3
	};
	REL::safe_write<std::uint8_t>(hook_getmodel1.address(), jcc6_to_jmp);
	REL::safe_write<std::uint8_t>(hook_getmodel2.address(), jcc2_to_jmp);
	REL::safe_write<std::uint8_t>(hook_loadmodel1.address(), jcc6_to_jmp);
	REL::safe_write<std::uint8_t>(hook_loadmodel2.address(), jcc2_to_jmp);

	// Loading from DB
	static REL::Relocation<std::uintptr_t> hook_load{
		Offset::BSFaceGenDB::LoadFromDB.address() + 0x18F
	};
	REL::safe_write<std::uint8_t>(hook_load.address(), nop6);

	// No model unload
	static REL::Relocation<std::uintptr_t> hook_unload{
		Offset::HighProcessData::Unload.address() + 0x294
	};
	REL::safe_write<std::uint8_t>(hook_unload.address(), jcc2_to_jmp);

	logger::info("Installed hooks for ignoring preprocessed FaceGen"sv);
}
#else
void FaceGenManager::InstallIgnorePreprocessedFaceGen()
{
	// Always generate facegen
	static REL::Relocation<std::uintptr_t> hook_getmodel1{
		Offset::TESNPC::GetHeadModel.address() + 0x61
	};
	static REL::Relocation<std::uintptr_t> hook_getmodel2{
		Offset::TESNPC::GetHeadModel.address() + 0x8A
	};
	REL::safe_write<std::uint8_t>(hook_getmodel1.address(), jcc6_to_jmp);
	REL::safe_write<std::uint8_t>(hook_getmodel2.address(), jcc2_to_jmp);

	// Loading from DB
	static REL::Relocation<std::uintptr_t> hook_load{
		Offset::BSFaceGenDB::LoadFromDB.address() + 0x17D
	};
	REL::safe_write<std::uint8_t>(hook_load.address(), nop2);

	// No model unload
	static REL::Relocation<std::uintptr_t> hook_unload{
		Offset::HighProcessData::Unload.address() + 0x26C
	};
	REL::safe_write<std::uint8_t>(hook_unload.address(), jcc2_to_jmp);

	logger::info("Installed hooks for ignoring preprocessed FaceGen"sv);
}
#endif

bool FaceGenManager::DataLoad_CheckRace(RE::TESNPC* a_actor)
{
	auto race = a_actor->race;
	if (!race) {
		logger::error("NPC '{}' ({:8x}) has no race."sv, a_actor->fullName, a_actor->formID);
		return false;
	}

	return a_actor->tintLayers && a_actor->tintLayers->size() > 0;
}
