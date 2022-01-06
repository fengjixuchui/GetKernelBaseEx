#pragma once
#include <ntifs.h>
#include <cstdint>

#define Log(x,...)  DbgPrint("[meh] " x, __VA_ARGS__)



namespace FindPatter
{

	/*
	original	https://www.unknowncheats.me/forum/3238153-post14.html
	*/

	uint64_t GetNtoskrnlbase()
	{
		typedef unsigned char uint8_t;
		auto Idt_base = reinterpret_cast<uintptr_t>(KeGetPcr()->IdtBase);
		auto align_page = *reinterpret_cast<uintptr_t*>(Idt_base + 4) >> 0xc << 0xc;

		for (; align_page; align_page -= PAGE_SIZE)
		{
			for (int index = 0; index < PAGE_SIZE - 0x7; index++)
			{
				auto current_address = static_cast<intptr_t>(align_page) + index;

				/*

				KiShadowProcessorAllocation

				48 8D ? ? ? ? FF 48 8B ? E8 ? ? ?  FF 4C 8D 05  ? ? ? ?  48 8B ?  44 ? ? 48 8B C8 E8
				work 7 sp1 - 11 windows pattern unique and should  be work ?
				*/
				if (*reinterpret_cast<uint8_t*>(current_address) == 0x48
					&& *reinterpret_cast<uint8_t*>(current_address + 1) == 0x8D
					&& *reinterpret_cast<uint8_t*>(current_address + 6) == 0xFF
					&& *reinterpret_cast<uint8_t*>(current_address + 7) == 0x48
					&& *reinterpret_cast<uint8_t*>(current_address + 8) == 0x8B
					&& *reinterpret_cast<uint8_t*>(current_address + 10) == 0xE8
					&& *reinterpret_cast<uint8_t*>(current_address + 14) == 0xFF
					&& *reinterpret_cast<uint8_t*>(current_address + 15) == 0x4C
					&& *reinterpret_cast<uint8_t*>(current_address + 16) == 0x8D
					&& *reinterpret_cast<uint8_t*>(current_address + 17) == 0x5
					&& *reinterpret_cast<uint8_t*>(current_address + 22) == 0x48
					&& *reinterpret_cast<uint8_t*>(current_address + 23) == 0x8B
					&& *reinterpret_cast<uint8_t*>(current_address + 25) == 0x44
					&& *reinterpret_cast<uint8_t*>(current_address + 28) == 0x48
					&& *reinterpret_cast<uint8_t*>(current_address + 29) == 0x8B
					&& *reinterpret_cast<uint8_t*>(current_address + 30) == 0xC8
					&& *reinterpret_cast<uint8_t*>(current_address + 31) == 0xE8

					)
				{

					auto nto_base_offset = *reinterpret_cast<int*>(current_address + 3);
					auto nto_base_ = (current_address + nto_base_offset + 7);
					if (!(nto_base_ & 0xfff))
					{
						return nto_base_;
					}
				}
			}
		}

		return NULL;
	}

}