#pragma once

namespace Patches
{
	inline static std::array<uint8_t, 2> nop2{ 0x66, 0x90 };
	inline static std::array<uint8_t, 6> nop6{ 0x66, 0x0F, 0x1F, 0x44, 0x00, 0x00 };
	inline static std::array<uint8_t, 1> jcc2_to_jmp{ 0xEB };
	inline static std::array<uint8_t, 2> jcc6_to_jmp{ 0x90, 0xE9 };
}

