#include "memory.h"
#include <iostream>
#include <chrono>
#include <thread>

namespace offsets
{
	constexpr ::std::ptrdiff_t dwLocalPlayer = 0xDEA964;
	constexpr ::std::ptrdiff_t m_iEntityQuality = 0x2FBC;
	constexpr ::std::ptrdiff_t dwForceAttack = 0x322DDFC;
	constexpr ::std::ptrdiff_t m_iHealth = 0x100;
	constexpr ::std::ptrdiff_t m_iTeamNum = 0xF4;
	constexpr ::std::ptrdiff_t m_iCrosshairId = 0x11838;
	constexpr ::std::ptrdiff_t dwGlowObjectManager = 0x535AA70;
}


int main()
{
	const auto memory = Memory{ "csgo.exe" };
	const auto client = memory.GetModuleAddress("client.dll");

	std::cout <<std::hex << "client.dll -> 0x" << client << std::dec << std::endl;
	//hack loop
	while (true)
	{
		//sleep loop for 1 millisecond
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

		//skup if thrigger jet is not down
		if (!GetAsyncKeyState(VK_SHIFT)) {
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, NULL);
			continue;
			}

		//get local player
		const auto localPlayer = memory.Read<std::uintptr_t>(client + offsets::dwLocalPlayer); 
		// const auto means that the variable is constant and cannot be changed and the value and variable type is determined at compile time (it probobly a pointer type)
		// std::uintptr_t is a type that can hold a memory address (it is an unsigned integer type)            <--
		// memory.Read<std::uintptr_t> is a function that reads a memory address from the process memory  relates^
		// client + offsets::dwLocalPlayer is the address of the local player (uses namespace offsets)
		
		//get crosshair id
		const auto crosshairId = memory.Read<std::int32_t>(localPlayer + offsets::m_iCrosshairId);

		if (!crosshairId || crosshairId > 64) {
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, NULL);
			continue;
			}

		//send mouse click down using windows api
	    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, NULL);
		std::cout << "mouse down" << std::endl;
	}


	return 0;
}