#if !defined(__INCLUDED_BASE_MEMORY_H__)
#define __INCLUDED_BASE_MEMORY_H__

#include "base.h"

#define BYTES(n)     (LONG)(n)
#define KIBIBYTES(n) (BYTES(n)     << 10)
#define MEBIBYTES(n) (KIBIBYTES(n) << 10)
#define GIBIBYTES(n) (MEBIBYTES(n) << 10)

#define copy(a, b, n) __builtin_memcpy(a, b, n)
#define move(a, b, n) __builtin_memmove(a, b, n) 
#define test(a, b, n) __builtin_memcmp(a, b, n)
#define fill(a, b, n) __builtin_memset(a, b, n)
#define find(a, b, n) __builtin_memchr(a, b, n)

#define align_forwards(address, alignment)  __builtin_align_up(address, alignment)
#define align_backwards(address, alignment) __builtin_align_down(address, alignment)
#define is_aligned(address, alignment)      __builtin_is_aligned(address, alignment)
#define is_alignment(alignment)             ((alignment) % 2 == 0)

#define backward_alignment_of(address, alignment) ((address) - align_backwards(address, alignment))
#define forward_alignment_of(address, alignment)  (align_forwards(address, alignment) - (address))

typedef struct
{
	BYTE *ending;
	BYTE  beginning[];
} DATA;

typedef struct
{
	BYTE *ending;
	BYTE *beginning;
} VIEW;

typedef VOID *ALLOCATE  (UWORD size, VOID *state);
typedef VOID *REALLOCATE(UWORD size, VOID *block, VOID *state);
typedef VOID  DEALLOCATE(VOID *block, VOID *state);

typedef struct
{
	VOID       *state;
	ALLOCATE   *allocate;
	REALLOCATE *reallocate;
	DEALLOCATE *deallocate;
} HEAP_INTERFACE;

extern HEAP_INTERFACE *global_heap_interface;

inline VOID *allocate  (UWORD size)              { return global_heap_interface->allocate(size, global_heap_interface->state); }
inline VOID *reallocate(UWORD size, VOID *block) { return global_heap_interface->reallocate(size, block , global_heap_interface->state); }
inline VOID  deallocate(VOID *block)             { global_heap_interface->deallocate(block, global_heap_interface->state); }

typedef VOID *PUSH(UWORD size, UWORD alignment, VOID *state);
typedef VOID  POP (UWORD size, UWORD alignment, VOID *state);

typedef struct
{
	VOID *state;
	PUSH *push;
	POP  *pop;
} STACK_INTERFACE;

extern STACK_INTERFACE *global_stack_interface;

inline VOID *push(UWORD size, UWORD alignment) { return global_stack_interface->push(size, alignment, global_stack_interface->state); }
inline VOID  pop (UWORD size, UWORD alignment) { global_stack_interface->pop(size, alignment, global_stack_interface->state); }

// buffer

typedef struct
{
	ULONG reservation_size;
	ULONG commission_rate;
	DATA *data;
	ULONG commission_size;
} BUFFER;

VOID *push_into_buffer(UWORD size, UWORD alignment, BUFFER *buffer);
VOID  pop_from_buffer (UWORD size, UWORD alignment, BUFFER *buffer);

#endif
