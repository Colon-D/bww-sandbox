#include <UE4SSProgram.hpp>
#include <Mod/CppUserModBase.hpp>
#include <iostream>
#include <array>
#include <filesystem>
#include <windows.h>
#undef ensure
#include <polyhook2/Detour/x64Detour.hpp>
#include <sol/sol.hpp>

constexpr std::uint8_t nop{ 0x90 };

std::uint64_t base_offset{};

template<typename type>
void write(const std::uint64_t offset, const type value) {
	const auto address = reinterpret_cast<void*>(base_offset + offset);
	DWORD old_protect{};
	DWORD new_protect{ PAGE_READWRITE };
	VirtualProtect(address, sizeof(type), new_protect, &old_protect);
	*reinterpret_cast<type*>(address) = value;
	VirtualProtect(address, sizeof(type), old_protect, &new_protect);
}

uint64_t orig_set_sound_driver_category_volume;
std::unordered_map<std::string, float> volume_multipliers{};
__int64 __fastcall hook_set_sound_driver_category_volume(
	const __int64 sound_driver_category,
	float volume,
	const float a3,
	const int a4
) {
	std::string sound_driver_category_name{
		*reinterpret_cast<char**>(sound_driver_category + 0xD8)
	};

	volume *= volume_multipliers.at(sound_driver_category_name);

	return PLH::FnCast(
		orig_set_sound_driver_category_volume, 
		&hook_set_sound_driver_category_volume
	)(
		sound_driver_category, volume, a3, a4
	);
}

class Sandbox : public RC::CppUserModBase {
public:
	std::unique_ptr<PLH::x64Detour> detour_set_sound_driver_category_volume;

	Sandbox() : CppUserModBase() {
		ModName = STR("Sandbox");
		ModVersion = STR("0.2.0");
		ModDescription = STR("Where is this Description used?");
		ModAuthors = STR("Lily");
		// Do not change this unless you want to target a UE4SS version
		// other than the one you're currently building with somehow.
		//ModIntendedSDKVersion = STR("3.0.1");

		base_offset =
			reinterpret_cast<std::uint64_t>(GetModuleHandleW(nullptr));
		
		// config dependant code is set in on_lua_start()'s on_config_loaded().
		// this is called after the main lua script has loaded config.lua

		detour_set_sound_driver_category_volume =
			std::make_unique<PLH::x64Detour>(
				std::uint64_t{ base_offset + 0x34C77E0 },
				reinterpret_cast<std::uint64_t>(
					hook_set_sound_driver_category_volume
				),
				&orig_set_sound_driver_category_volume
			);
		detour_set_sound_driver_category_volume->hook();

		// force CanJump to work on ground on forced Jumps for ALL costumes
		write(0x2A5970A, std::array{ nop, nop, nop, nop, nop, nop });
	}

	~Sandbox() override {
	}

	void on_update() override {
	}

	void on_lua_start(
		LuaMadeSimple::Lua& lms_lua,
		LuaMadeSimple::Lua& lms_main_lua,
		LuaMadeSimple::Lua& lms_async_lua,
		std::vector<LuaMadeSimple::Lua*>& hook_luas
	) override {
		sol::state_view lua{ lms_lua.get_lua_state() };

		lua["config_filepath"] =
			RC::UE4SSProgram::get_program().get_mods_directory()
			+ STR("\\Sandbox\\config.lua");

		lua["on_config_loaded"] = [](const sol::table& config) {
			if (config["infinite_costumes"]) {
				// NOP costume decrement
				write(0x114A4AA, std::array{ nop, nop, nop });
				// Don't increment above 1
				write<std::uint8_t>(0x11435AC + 0x2, 1);
			}

			const sol::table& lua_volume_multipliers
				= config["volume_multipliers"];
			for (const auto& [key, value] : lua_volume_multipliers) {
				volume_multipliers.insert(
					{ key.as<std::string>(), value.as<float>() }
				);
			}
		};
	}
};

extern "C" {
	__declspec(dllexport) RC::CppUserModBase* start_mod() {
		return new Sandbox();
	}

	__declspec(dllexport) void uninstall_mod(RC::CppUserModBase* mod) {
		delete mod;
	}
}
