#if !defined(__INCLUDED_BASE_MEMORY_H__)
#define __INCLUDED_BASE_MEMORY_H__

#include "base.h"

#define BYTES(n)     (LONG)(n)
#define KIBIBYTES(n) (BYTES(n)     << 10)
#define MEBIBYTES(n) (KIBIBYTES(n) << 10)
#define GIBIBYTES(n) (MEBIBYTES(n) << 10)

_IMPORTED_ VOID *_CDECL_ memcpy (VOID *, const VOID *, SIZE);
_IMPORTED_ VOID *_CDECL_ memmove(VOID *, const VOID *, SIZE);
_IMPORTED_ WORD  _CDECL_ memcmp (const VOID *, const VOID *, SIZE);
_IMPORTED_ VOID *_CDECL_ memset (VOID *, WORD, SIZE);
_IMPORTED_ VOID *_CDECL_ memchr (const VOID *, WORD, SIZE);

#define copy(a, b, n) memcpy (a, b, n)
#define move(a, b, n) memmove(a, b, n) 
#define test(a, b, n) memcmp (a, b, n)
#define fill(a, b, n) memset (a, b, n)
#define find(a, b, n) memchr (a, b, n)

_INLINE_ BIT     is_alignment         (SIZE alignment)                  { return alignment % 2 == 0; }
_INLINE_ SIZE    backward_alignment_of(UINTPTR address, SIZE alignment) { assert(is_alignment(alignment)); return alignment ? address & (alignment - 1) : 0; }
_INLINE_ SIZE    forward_alignment_of (UINTPTR address, SIZE alignment) { SIZE remainder = backward_alignment_of(address, alignment); return remainder ? alignment - remainder : 0; }
_INLINE_ UINTPTR align_forwards       (UINTPTR address, SIZE alignment) { return address + forward_alignment_of(address, alignment); }
_INLINE_ UINTPTR align_backwards      (UINTPTR address, SIZE alignment) { return address - backward_alignment_of(address, alignment); }
_INLINE_ BIT     is_aligned           (UINTPTR address, SIZE alignment) { return !backward_alignment_of(address, alignment); }

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

_INLINE_ VOID *allocate  (UWORD size)              { return global_heap_interface->allocate(size, global_heap_interface->state); }
_INLINE_ VOID *reallocate(UWORD size, VOID *block) { return global_heap_interface->reallocate(size, block , global_heap_interface->state); }
_INLINE_ VOID  deallocate(VOID *block)             { global_heap_interface->deallocate(block, global_heap_interface->state); }

typedef VOID *PUSH(UWORD size, UWORD alignment, VOID *state);
typedef VOID  POP (UWORD size, UWORD alignment, VOID *state);

typedef struct
{
	VOID *state;
	PUSH *push;
	POP  *pop;
} STACK_INTERFACE;

extern STACK_INTERFACE *global_stack_interface;

_INLINE_ VOID *push(UWORD size, UWORD alignment) { return global_stack_interface->push(size, alignment, global_stack_interface->state); }
_INLINE_ VOID  pop (UWORD size, UWORD alignment) { global_stack_interface->pop(size, alignment, global_stack_interface->state); }

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
