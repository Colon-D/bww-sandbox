#include <stdio.h>
#include <UE4SSProgram.hpp>
#include <Mod/CppUserModBase.hpp>
#include <iostream>
#include <array>
#include <filesystem>
#include <windows.h>

constexpr std::uint8_t nop{ 0x90 };

void* base_offset{};

template<typename type>
void write(const std::uint64_t offset, const type value) {
	const auto address = reinterpret_cast<void*>(
		reinterpret_cast<std::uint64_t>(base_offset) + offset
	);
	DWORD old_protect{};
	DWORD new_protect{ PAGE_READWRITE };
	VirtualProtect(address, sizeof(type), new_protect, &old_protect);
	*reinterpret_cast<type*>(address) = value;
	VirtualProtect(address, sizeof(type), old_protect, &new_protect);
}

class Sandbox : public RC::CppUserModBase {
public:
	Sandbox() : CppUserModBase() {
		ModName = STR("Sandbox");
		ModVersion = STR("0.2.0");
		ModDescription = STR("Where is this Description used?");
		ModAuthors = STR("Lily");
		// Do not change this unless you want to target a UE4SS version
		// other than the one you're currently building with somehow.
		//ModIntendedSDKVersion = STR("3.0.1");

		base_offset = GetModuleHandleW(nullptr);
		
		// config dependant code is set in on_lua_start's set_config function.
		// this is called after the main lua script has loaded config.lua

		// force CanJump to work on ground on forced Jumps for ALL costumes
		write(0x2A5970A, std::array{ nop, nop, nop, nop, nop, nop });
	}

	~Sandbox() override {
	}

	void on_update() override {
	}

	void on_lua_start(
		LuaMadeSimple::Lua& lua,
		LuaMadeSimple::Lua& main_lua,
		LuaMadeSimple::Lua& async_lua,
		std::vector<LuaMadeSimple::Lua*>& hook_luas
	) override {
		lua.register_function("get_config_filepath", [](const LuaMadeSimple::Lua& lua) {
			lua.set_string(RC::to_string(get_config_filepath()));
			return 1;
		});

		lua.register_function("set_config", [](const LuaMadeSimple::Lua& config) {
			const auto infinite_costumes =
				get_config_value<bool>(config, "infinite_costumes");
			if (infinite_costumes) {
				// NOP costume decrement
				write(0x114A4AA, std::array{ nop, nop, nop });
				// Don't increment above 1
				write<std::uint8_t>(0x11435AC + 0x2, 1);
			}

			return 1;
		});
	}

	static RC::StringType get_config_filepath() {
		return RC::UE4SSProgram::get_program().get_mods_directory()
			+ STR("\\Sandbox\\config.lua");
	}

	template<typename type>
	static type get_value(
		const LuaMadeSimple::Lua& lua,
		LuaMadeSimple::Lua::Table table,
		const std::string_view key
	) {
		lua.set_string(key);
		table.find_value();
		if constexpr (std::is_same_v<type, bool>) {
			return lua.get_bool();
		}
		else if constexpr (std::is_same_v<type, int>) {
			return lua.get_integer();
		}
		else if constexpr (std::is_same_v<type, float>) {
			return lua.get_float();
		}
		else if constexpr (std::is_same_v<type, double>) {
			return lua.get_number();
		}
		else if constexpr (std::is_same_v<type, std::string_view>) {
			return lua.get_string();
		}
		else if constexpr (std::is_same_v<type, LuaMadeSimple::Lua::Table>) {
			return lua.get_table();
		}
		else {
			static_assert(false, "Unsupported type for get_value");
		}
	}

	template<typename type>
	static type get_config_value(
		const LuaMadeSimple::Lua& config,
		const std::string_view key
	) {
		return get_value<type>(config, config.get_table(), key);
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
