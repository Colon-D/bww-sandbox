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

// macros to make code a bit shorter for detours.
// requires the original function to be stored as `std::uint64_t orig_FN_NAME`,
// and the replacement function to be stored as `FN_SIGNATURE hook_FN_NAME`,
// and the detour object to be stored as `detour detour_FN_NAME`
#define hook(function_name, offset)\
	detour_##function_name = std::make_unique<PLH::x64Detour>(\
		std::uint64_t{ base_offset + offset },\
		reinterpret_cast<std::uint64_t>(hook_##function_name),\
		&orig_##function_name\
	);\
	detour_##function_name->hook()
#define orig(function_name)\
	PLH::FnCast(orig_##function_name, hook_##function_name)
using detour = std::unique_ptr<PLH::x64Detour>;

std::uint64_t orig_set_sound_driver_category_volume{};
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

	return orig(set_sound_driver_category_volume)(
		sound_driver_category, volume, a3, a4
	);
}

constexpr std::int32_t costume_locked{ -1 };
std::int32_t get_costume_count(
	const std::uint64_t game, const std::uint8_t costume_index
) {
	const auto* const costume_counts =
		*reinterpret_cast<std::uint32_t**>(game + 0x1F8);
	return costume_counts[costume_index];
}

std::uint64_t orig_get_costume_count{};
bool infinite_costumes{};
bool unlock_all_costumes{};
std::int64_t __fastcall hook_get_costume_count(
	const std::uint64_t game, const std::uint8_t costume_index
) {
	auto costume_count = get_costume_count(game, costume_index);
	if (unlock_all_costumes and costume_count == costume_locked) {
		costume_count = 0;
	}
	if (infinite_costumes and costume_count == 0) {
		costume_count = 1;
	}
	return costume_count;
}

std::uint64_t orig_decrement_costume_count{};
std::uint32_t* __fastcall hook_decrement_costume_count(
	const std::uint64_t game, const std::uint8_t costume_index
) {
	auto* const costume_counts =
		*reinterpret_cast<std::uint32_t**>(game + 0x1F8);
	const auto costume_count = costume_counts[costume_index];
	if (
		infinite_costumes
		and (costume_count == 0 or costume_count == costume_locked)
	) {
		return costume_counts;
	}
	--costume_counts[costume_index];
	return costume_counts;
}

class Sandbox : public RC::CppUserModBase {
public:
	detour detour_set_sound_driver_category_volume;
	detour detour_get_costume_count;
	detour detour_decrement_costume_count;

	Sandbox() : CppUserModBase() {
		ModName = STR("Sandbox");
		ModVersion = STR("0.2.2");
		ModDescription = STR("Where is this Description used?");
		ModAuthors = STR("Lily");
		// Do not change this unless you want to target a UE4SS version
		// other than the one you're currently building with somehow.
		//ModIntendedSDKVersion = STR("3.0.1");

		base_offset =
			reinterpret_cast<std::uint64_t>(GetModuleHandleW(nullptr));
		
		// config dependant code is set in on_lua_start()'s on_config_loaded().
		// this is called after the main lua script has loaded config.lua

		hook(get_costume_count, 0x1146530);
		hook(decrement_costume_count, 0x114A4A0);
		hook(set_sound_driver_category_volume, 0x34C77E0);

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

		lua["on_config_loaded"] = [&](const sol::table& config) {
			infinite_costumes = config["infinite_costumes"];
			unlock_all_costumes = config["unlock_all_costumes"];

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
