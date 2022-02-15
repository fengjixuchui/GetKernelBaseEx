#pragma once
#include <ntifs.h>
#include <cstdint>

#define Log(x,...)  DbgPrint("[meh] " x, __VA_ARGS__)



namespace FindPatter
{

	/*
	original	https://www.unknowncheats.me/forum/3238153-post14.html

	VfPowerDumpIrpStack signature:
	48 8D 3D ? ? ? FF 48 8B 8C ? ? ? ? ? E8

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
				Windows 7 - 11 support

				VfPowerDumpIrpStack sig:
				48 8D 3D ? ? ? FF 48 8B 8C ? ? ? ? ? E8
				
				*/
				

				if (*reinterpret_cast<uint8_t*>(current_address) == 0x48
					&& *reinterpret_cast<uint8_t*>(current_address + 1) == 0x8D
					&& *reinterpret_cast<uint8_t*>(current_address + 2) == 0x3D
					&& *reinterpret_cast<uint8_t*>(current_address + 6) == 0xFF
					&& *reinterpret_cast<uint8_t*>(current_address + 7) == 0x48
					&& *reinterpret_cast<uint8_t*>(current_address + 8) == 0x8B
					&& *reinterpret_cast<uint8_t*>(current_address + 9) == 0x8C
					&& *reinterpret_cast<uint8_t*>(current_address + 15) == 0xE8

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
