#pragma once

namespace Offset
{
#ifndef SKYRIMVR
	constexpr REL::ID TESNPC_ReadFromFileStream{ 24159 };
	constexpr REL::ID TESNPC_InitializeAfterLoad{ 24215 };
	constexpr REL::ID TESNPC_GetHeadModel{ 24227 };
	constexpr REL::ID TESNPC_FinishInit{ 24272 };
	constexpr REL::ID BSFaceGenDB_GenerateHeadPartModel{ 26259 };
	constexpr REL::ID BSFaceGenDB_LoadFromDB{ 26263 };
	constexpr REL::ID HighProcessData_Unload{ 38692 };
#else
	constexpr REL::Offset TESNPC_ReadFromFileStream{ 0x0036C150 };
	constexpr REL::Offset TESNPC_InitializeAfterLoad{ 0x003714D0 };
	constexpr REL::Offset TESNPC_GetHeadModel{ 0x00373740 };
	constexpr REL::Offset TESNPC_FinishInit{ 0x00378240 };
	constexpr REL::Offset BSFaceGenDB_GenerateHeadPartModel{ 0x003E23D0 };
	constexpr REL::Offset BSFaceGenDB_LoadFromDB{ 0x003E2C30 };
	constexpr REL::Offset HighProcessData_Unload{ 0x0067F550 };
#endif
}
