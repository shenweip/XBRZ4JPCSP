// Copyright (C) 2003 Dolphin Project.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0 or later versions.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

// Official SVN repository and contact information can be found at
// http://code.google.com/p/dolphin-emu/

#include "MemoryUtil.h"

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#else
#include <errno.h>
#include <stdio.h>
#endif

#include <stdlib.h>


#if !defined(_WIN32) && defined(__x86_64__) && !defined(MAP_32BIT)
#include <unistd.h>
#ifdef __APPLE__
#define PAGE_MASK (4096-1)
#else
#define PAGE_MASK     (getpagesize() - 1)
#endif
#define round_page(x) ((((unsigned long)(x)) + PAGE_MASK) & ~(PAGE_MASK))
#endif

void* AllocateMemoryPages(size_t size)
{
	size = (size + 4095) & (~4095);
#ifdef _WIN32
	void* ptr = VirtualAlloc(0, size, MEM_COMMIT, PAGE_READWRITE);
#elif defined(__SYMBIAN32__)
	void* ptr = new u8[size];
#else
	void* ptr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
#endif

	// printf("Mapped memory at %p (size %ld)\n", ptr,
	//	(unsigned long)size);
	if (ptr == NULL)
		//PanicAlert("Failed to allocate raw memory");

	return ptr;
}

void FreeMemoryPages(void* ptr, size_t size)
{
	size = (size + 4095) & (~4095);
	if (ptr)
	{
#ifdef _WIN32
	
		if (!VirtualFree(ptr, 0, MEM_RELEASE))
			//PanicAlert("FreeMemoryPages failed!\n%s", GetLastErrorMsg());
		ptr = NULL; // Is this our responsibility?
#else
		munmap(ptr, size);
#endif
	}
}









