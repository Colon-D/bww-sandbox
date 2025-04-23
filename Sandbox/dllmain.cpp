#include <stdio.h>
#include <Mod/CppUserModBase.hpp>
#include <iostream>
#include <array>
#include <windows.h>

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
		ModVersion = STR("1.0");
		ModDescription = STR("Where is this Description used?");
		ModAuthors = STR("Lily");
		// Do not change this unless you want to target a UE4SS version
		// other than the one you're currently building with somehow.
		//ModIntendedSDKVersion = STR("3.0.1");

		base_offset = GetModuleHandleW(nullptr);
		
		constexpr std::uint8_t nop{ 0x90 };
		// NOP costume decrement
		write(0x114A4AA, std::array{ nop, nop, nop });
		// Don't increment above 1
		write<std::uint8_t>(0x11435AC + 0x2, 1);

		// force CanJump to work on ground on forced Jumps for ALL costumes
		write(0x2A5970A, std::array{ nop, nop, nop, nop, nop, nop });
	}

	~Sandbox() override {
	}

	auto on_update() -> void override {
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
