#pragma once

namespace Offset
{
#ifndef SKYRIMVR
	namespace TESNPC
	{
		// SkyrimSE 1.6.318.0: 0x373570
		inline constexpr REL::ID ReadFromFileStream(24663);
		// SkyrimSE 1.6.318.0: 0x3789B0
		inline constexpr REL::ID InitializeAfterLoad(24719);
		// SkyrimSE 1.6.318.0: 0x37AFB0
		inline constexpr REL::ID GetHeadModel(24731);
		// SkyrimSE 1.6.318.0: 0x37C100
		inline constexpr REL::ID LoadModel(24740);
		// SkyrimSE 1.6.318.0: 0x37FE00
		inline constexpr REL::ID FinishInit(24787);
	}

	namespace BSFaceGenDB
	{
		// SkyrimSE 1.6.318.0: 0x3EA3D0
		inline constexpr REL::ID GenerateHeadPartModel(26838);
		// SkyrimSE 1.6.318.0: 0x3EAD60
		inline constexpr REL::ID LoadFromDB(26842);
	}

	namespace HighProcessData
	{
		// SkyrimSE 1.6.318.0: 0x69D810
		inline constexpr REL::ID Unload(39727);
	}

#else

	namespace TESNPC
	{
		inline constexpr REL::Offset ReadFromFileStream(0x0036C150);
		inline constexpr REL::Offset InitializeAfterLoad(0x003714D0);
		inline constexpr REL::Offset GetHeadModel(0x00373740);
		inline constexpr REL::Offset FinishInit(0x00378240);
	}

	namespace BSFaceGenDB
	{
		inline constexpr REL::Offset GenerateHeadPartModel(0x003E23D0);
		inline constexpr REL::Offset LoadFromDB(0x003E2C30);
	}

	namespace HighProcessData
	{
		inline constexpr REL::Offset Unload(0x0067F550);
	}
#endif
}
