#include "FaceDiscolorationManager.h"
#include "Offsets.h"
#include "Patches.h"

using namespace Patches;

void FaceDiscolorationManager::InstallHooks()
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

bool FaceDiscolorationManager::DataLoad_CheckRace(RE::TESNPC* a_actor)
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