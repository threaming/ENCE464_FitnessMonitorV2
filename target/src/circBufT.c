// *******************************************************
// 
// circBufT.c
//
// Support for a circular buffer of uint32_t values on the 
//  Tiva processor.
// P.J. Bones UCECE
// Last modified:  8.3.2017
// 
// *******************************************************

#include <stdint.h>
#include "stdlib.h"
#include "circBufT.h"

// *******************************************************
// initCircBuf: Initialise the circBuf instance. Reset both indices to
// the start of the buffer.  Dynamically allocate and clear the the 
// memory and return a pointer for the data.  Return NULL if 
// allocation fails. Return NULL if size is to small or to big.
uint32_t *
initCircBuf (circBuf_t *buffer, uint32_t size)
{
	if (!size || size>MAX_BUFFER_CAPACITY)
		return NULL;
	buffer->windex = 0;
	buffer->rindex = 0;
	buffer->nelem = 0;
	buffer->size = size;
	buffer->data = 
        (uint32_t *) calloc (size, sizeof(int32_t));
	return buffer->data;
}
   // Note use of calloc() to clear contents.

// *******************************************************
// writeCircBuf: insert entry at the current windex location,
// advance windex, modulo (buffer size). If the buffer is full
// nothing will be changed at the entries
void
writeCircBuf (circBuf_t *buffer, int32_t entry)
{
	if (buffer->nelem == buffer->size)
		return;
	buffer->data[buffer->windex] = entry;
	buffer->windex++;
	buffer->nelem++;
	if (buffer->windex >= buffer->size)
	   buffer->windex = 0;
}

// *******************************************************
// readCircBuf: return entry at the current rindex location,
// advance rindex, modulo (buffer size). 0 is returned if the
// buffer is empty.
int32_t
readCircBuf (circBuf_t *buffer)
{
	if (!buffer->nelem)
		return 0;
	int32_t entry;
	entry = buffer->data[buffer->rindex];
	buffer->rindex++;
	buffer->nelem--;
	if (buffer->rindex >= buffer->size)
	   buffer->rindex = 0;

    return entry;
}

// *******************************************************
// freeCircBuf: Releases the memory allocated to the buffer data,
// sets pointer to NULL and other fields to 0. The buffer can
// re-initialised by another call to initCircBuf().
void
freeCircBuf (circBuf_t * buffer)
{
	buffer->windex = 0;
	buffer->rindex = 0;
	buffer->size = 0;
	free (buffer->data);
	buffer->data = NULL;
}

