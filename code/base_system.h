#if !defined(__INCLUDED_BASE_SYSTEM_H__)
#define __INCLUDED_BASE_SYSTEM_H__

#include "base.h"

typedef struct
{
	UWORD page_size;
} SYSTEM_INFO;

SYSTEM_INFO *get_system_info(VOID);

VOID *reserve_memory (ULONG size);
VOID  commit_memory  (VOID *memory, ULONG size);
VOID  decommit_memory(VOID *memory, ULONG size);
VOID  release_memory (VOID *memory);

#endif
