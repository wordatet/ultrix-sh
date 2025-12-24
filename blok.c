#ifndef lint
static char sccsid[] = "@(#)blok.c	4.1 (Ultrix) 7/2/90";
/* Original ID:  "@(#)blok.c	4.2 8/11/83" */
#endif

#include	"defs.h"
#include    <stdlib.h>

/*
 *	storage allocator
 *	(standard malloc/free wrappers for 64-bit stability)
 */

#undef free

#define MAGIC 0xDEADC0DE64624CULL

ADDRESS	alloc(nbytes)
	POS		nbytes;
{
	size_t req = (size_t)nbytes + sizeof(uint64_t);
	uint64_t *p = (uint64_t *)malloc(req);
	if (p)
	{
		*p = MAGIC;
		return (ADDRESS)(p + 1);
	}
	return NULL;
}

VOID	sh_free(ap)
	void		*ap;
{
	if (ap)
	{
		uint64_t *p = (uint64_t *)ap - 1;
		if (*p == MAGIC)
		{
			*p = 0; /* Clear magic to prevent double free */
			free(p);
		}
	}
}

VOID	addblok(reqd)
	POS		reqd;
{
	/* No-op: merged into alloc/malloc */
}
