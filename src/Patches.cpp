#include "Patches.h"
#include "Offsets.h"

inline static std::array<uint8_t, 2> nop2{ 0x66, 0x90 };
inline static std::array<uint8_t, 6> nop6{ 0x66, 0x0F, 0x1F, 0x44, 0x00, 0x00 };
inline static std::array<uint8_t, 1> jcc2_to_jmp{ 0xEB };
inline static std::array<uint8_t, 2> jcc6_to_jmp{ 0x90, 0xE9 };

void ApplyFaceDiscolorationFix()
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
	static REL::Relocation<std::uintptr_t> hook_texture{ Offset::BSFaceGenDB_GenerateHeadPartModel, 0x3A4 };
	REL::safe_write(hook_texture.address(), nop2);

	// Try loading from DB
	static REL::Relocation<std::uintptr_t> hook_queue{ Offset::BSFaceGenDB_LoadFromDB, 0x17D };
	REL::safe_write(hook_queue.address(), nop2);

	logger::info("Applied patches for face discoloration fix");
}

void ApplyIgnorePreprocessedFacegen()
{
	// Always generate facegen
	static REL::Relocation<std::uintptr_t> hook_npc1{ Offset::TESNPC_GetHeadModel, 0x61 };
	static REL::Relocation<std::uintptr_t> hook_npc2{ Offset::TESNPC_GetHeadModel, 0x8A };
	REL::safe_write(hook_npc1.address(), jcc6_to_jmp);
	REL::safe_write(hook_npc2.address(), jcc2_to_jmp);

	// Nothing to unload
	static REL::Relocation<std::uintptr_t> hook_free{ Offset::HighProcessData_Unload, 0x26C };
	REL::safe_write(hook_free.address(), jcc2_to_jmp);

	logger::info("Applied patches for ignoring preprocessed FaceGen");
}

void Patches::Apply()
{
	if (EnableFaceDiscolorationFix)
	{
		ApplyFaceDiscolorationFix();

		if (IgnorePreprocessedFacegen)
		{
			ApplyIgnorePreprocessedFacegen();
		}
	}
}