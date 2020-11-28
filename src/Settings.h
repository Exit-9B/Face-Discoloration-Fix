#pragma once

struct Settings
{
	using ISetting = AutoTOML::ISetting;
	using bSetting = AutoTOML::bSetting;

	inline static void load()
	{
		try
		{
			const auto table = toml::parse_file("Data/SKSE/Plugins/FaceGenFixes.toml"s);
			for (const auto& setting : ISetting::get_settings())
			{
				setting->load(table);
			}
		}
		catch (const toml::parse_error& e)
		{
			std::ostringstream ss;
			ss << "Error parsing file \'" << *e.source().path << "\':" << std::endl
				<< '\t' << e.description() << std::endl
				<< "\t\t(" << e.source().begin << ')';
			logger::error(ss.str());
		}
	}

	inline static bSetting bEnableFaceGenLogging
	{ "FaceGen"s, "bEnableFaceGenLogging"s, false };

	inline static bSetting bIgnorePreprocessedFaceGen
	{ "FaceGen"s, "bIgnorePreprocessedFaceGen"s, false };

	inline static bSetting bSkipHeadPartsValidation
	{ "FaceGen"s, "bSkipHeadPartsValidation"s, false };

	inline static bSetting bScanOnStartup
	{ "FaceGen"s, "bScanOnStartup"s, false };
};
