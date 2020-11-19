#include "Patches.h"
#include "Offsets.h"

inline static std::array<uint8_t, 2> nop2{ 0x66, 0x90 };
inline static std::array<uint8_t, 6> nop6{ 0x66, 0x0F, 0x1F, 0x44, 0x00, 0x00 };
inline static std::array<uint8_t, 1> jcc2_to_jmp{ 0xEB };
inline static std::array<uint8_t, 2> jcc6_to_jmp{ 0x90, 0xE9 };

RE::TESRace::FaceRelatedData::TintAsset* FindTintAssetData(RE::TESNPC* a_actor, std::uint16_t a_index)
{
	auto race = a_actor->race;
	if (!race)
	{
		logger::warn("{} [NPC_:{:x}] has face tint data but no race.", a_actor->fullName, a_actor->formID);
		return nullptr;
	}

	for (int i = 0; i < RE::SEXES::kTotal; i++)
	{
		auto faceData = race->faceRelatedData[i];
		if (faceData && faceData->tintMasks)
		{
			auto tintMasks = *faceData->tintMasks;
			for (RE::BSTArrayBase::size_type j = 0; j < tintMasks.size(); j++)
			{
				auto tintAsset = tintMasks[j];
				if (tintMasks[j]->texture.index == a_index)
				{
					return tintAsset;
				}
			}
		}
	}

	logger::warn("{} [NPC_:{:x}] has invalid face tint data.", a_actor->fullName, a_actor->formID);

	return nullptr;
}

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
	static REL::Relocation<std::uintptr_t> hook_tint{ Offset::BSFaceGenDB_GenerateHeadPartModel, 0x3A4 };
	REL::safe_write(hook_tint.address(), nop2);

	constexpr REL::ID TESNPC_InitializeAfterAllFormsAreReadFromFile{ 24215 };
	static REL::Relocation<std::uintptr_t> hook_findtintasset{ TESNPC_InitializeAfterAllFormsAreReadFromFile, 0x766 };

	auto& trampoline = SKSE::GetTrampoline();
	std::array<std::uint8_t, 6> mov_rcx_r15{ 0x4C, 0x89, 0xF9, 0x0F, 0x1F, 0x00 };
	REL::safe_write(hook_findtintasset.address(), mov_rcx_r15);
	trampoline.write_call<6>(hook_findtintasset.address() + 6, FindTintAssetData);

	logger::info("Applied patches for face discoloration fix");
}

void ApplyIgnorePreprocessedFacegen()
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