#include "base_system.h"

typedef struct
{
	union
	{
		UWORD dwOemId;
		struct
		{
			UHALF wProcessorArchitecture;
			UHALF wReserved;
		} DUMMYSTRUCTNAME;
	} DUMMYUNIONNAME;
	UWORD   dwPageSize;
	VOID   *lpMinimumApplicationAddress;
	VOID   *lpMaximumApplicationAddress;
	UINTPTR dwActiveProcessorMask;
	UWORD   dwNumberOfProcessors;
	UWORD   dwProcessorType;
	UWORD   dwAllocationGranularity;
	UHALF   wProcessorLevel;
	UHALF   wProcessorRevision;
} SYSTEM_INFO_;

_IMPORTED_ VOID  _STDCALL_ GetSystemInfo(SYSTEM_INFO_ *);
_IMPORTED_ VOID *_STDCALL_ VirtualAlloc (VOID *, SIZE, UWORD, UWORD);
_IMPORTED_ BIT32 _STDCALL_ VirtualFree  (VOID *, SIZE, UWORD);

SYSTEM_INFO *get_system_info(VOID)
{
	static SYSTEM_INFO system_info = {};
	if(!system_info.page_size)
	{
		SYSTEM_INFO_ system_info_;
		GetSystemInfo(&system_info_);
		system_info.page_size = system_info_.dwPageSize;
	}
	return &system_info;
}

VOID *reserve_memory(ULONG size)
{
	VOID *memory = VirtualAlloc(0, size, 0x00002000 /* MEM_RESERVE */, 0x04 /* PAGE_READWRITE */);
	assert(memory);
	return memory;
}

VOID commit_memory(VOID *memory, ULONG size)
{
	VOID *result = VirtualAlloc(memory, size, 0x00001000 /* MEM_COMMIT */, 0x04 /* PAGE_READWRITE */);
	assert(result);
}

VOID decommit_memory(VOID *memory, ULONG size)
{
	BIT32 result = VirtualFree(memory, size, 0x00004000 /* MEM_DECOMMIT */);
	assert(result);
}

VOID release_memory(VOID *memory)
{
	BIT32 result = VirtualFree(memory, 0, 0x00008000 /* MEM_RELEASE */);
	assert(result);
}
