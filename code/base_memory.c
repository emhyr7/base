#include "base_memory.h"

#include "base_system.h"

// heap

static VOID *default_heap_interface__allocate  (UWORD size , VOID *state             ) { (VOID)state; return __builtin_malloc (size       ); }
static VOID *default_heap_interface__reallocate(UWORD size , VOID *block, VOID *state) { (VOID)state; return __builtin_realloc(block, size); }
static VOID  default_heap_interface__deallocate(VOID *block, VOID *state             ) { (VOID)state; return __builtin_free   (block      ); }

static HEAP_INTERFACE default_heap_interface =
{
	.state      = 0,
	.allocate   = default_heap_interface__allocate,
	.reallocate = default_heap_interface__reallocate,
	.deallocate = default_heap_interface__deallocate,
};

HEAP_INTERFACE *global_heap_interface = &default_heap_interface;

// stack

static BUFFER default_stack_interface__state =
{
	.reservation_size = 0,
	.commission_rate  = 0,
	.data             = 0,
	.commission_size  = 0,
};

static STACK_INTERFACE default_stack_interface =
{
	.state = &default_stack_interface__state,
	.push  = (PUSH *)&push_into_buffer,
	.pop   = (POP  *)&pop_from_buffer,
};

STACK_INTERFACE *global_stack_interface = &default_stack_interface;

// buffer

VOID *push_into_buffer(UWORD size, UWORD alignment, BUFFER *buffer)
{
	assert(is_alignment(alignment));

	if(!buffer->data)
	{
		if(!buffer->reservation_size) buffer->reservation_size = GIBIBYTES(1);
		if(!buffer->commission_rate)  buffer->commission_rate = get_system_info()->page_size;
		buffer->data = reserve_memory(buffer->reservation_size);
		commit_memory(buffer->data, buffer->commission_rate);
		buffer->commission_size = buffer->commission_rate;
		buffer->data->ending = buffer->data->beginning;
	}
	UINTPTR forward_alignment = forward_alignment_of((UINTPTR)buffer->data->beginning, alignment);
	UINTPTR mass = buffer->data->ending - buffer->data->beginning;
	if(mass + forward_alignment + size >= buffer->commission_size)
	{
		assert(buffer->commission_size + buffer->commission_rate <= buffer->reservation_size);
		commit_memory((BYTE *)buffer->data + buffer->commission_size, buffer->commission_rate);
		buffer->commission_size += buffer->commission_rate;
	}
	buffer->data->ending += forward_alignment;
	VOID *result = buffer->data->ending;
	buffer->data->ending += size;
	fill(result, 0, size);
	return result;
}

VOID pop_from_buffer(UWORD size, UWORD alignment, BUFFER *buffer)
{
	buffer->data->ending = (BYTE *)align_backwards((UINTPTR)buffer->data->ending, alignment);
	buffer->data->ending -= size;
	if(buffer->data->ending < buffer->data->beginning) buffer->data->ending = buffer->data->beginning;
}
