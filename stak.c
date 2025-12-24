#ifndef lint
static char sccsid[] = "@(#)stak.c	4.1 (Ultrix) 7/2/90";
/* Original ID:  "@(#)stak.c	4.2 8/11/83" */
#endif

#include	"defs.h"
#include <sys/mman.h>

#define STAK_MAX_SIZE (8 * 1024 * 1024) /* 8MB stack limit */

VOID growstak();

STKPTR		stakbot=nullstr;

/* ========	storage allocation	======== */

STKPTR	getstak(asize)
	INT		asize;
{	/* allocate requested stack */
	REG STKPTR	oldstak;
	REG INT		size;

	size=round(asize,BYTESPERWORD);
	
	if (stakbas == NULL)
	{
		growstak(NULL);
	}

	if (staktop + size >= brkend)
	{
		error(nospace);
	}
	
	oldstak=stakbot;
	staktop = stakbot += size;
	return(oldstak);
}

/* growstak: Initial allocation of the fixed-base stack via mmap */
VOID growstak(ptr)
    STKPTR ptr;
{
    if (stakbas != NULL) return; /* Already initialized */

    /* 
     * We allocate a large fixed-base region via mmap to avoid the 
     * use-after-realloc issues inherent in the original Bourne shell's 
     * assumption that the stack never moves.
     */
    stakbas = (STKPTR)mmap(NULL, STAK_MAX_SIZE, PROT_READ|PROT_WRITE, 
                           MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    
    if (stakbas == MAP_FAILED)
    {
        stakbas = NULL;
        error(nospace);
    }
    
    stakbot = staktop = stakbas;
    brkend = stakbas + STAK_MAX_SIZE;
}

STKPTR	locstak()
{
	if (stakbas == NULL)
	{
		growstak(NULL);
	}
	return(stakbot);
}

STKPTR	savstak()
{
	return(stakbot);
}

STKPTR	endstak(argp)
	REG STRING	argp;
{	/* tidy up after `locstak' */
	REG STKPTR	oldstak;
	*argp++=0;
	oldstak=stakbot; 
	stakbot=staktop=(STKPTR)round(argp,BYTESPERWORD);
	return(oldstak);
}

VOID	tdystak(x)
	REG STKPTR 	x;
{
	/* 
	 * Original tdystak freed stakbsy blocks.
	 * In our malloc version, we don't cover the stack with the heap,
	 * but we might still have stakbsy if we implement that.
	 * For now, just reset the pointers.
	 */
	staktop=stakbot=(STKPTR)max((char *)(x),(char *)(stakbas));
	rmtemp(x);
}

VOID	stakchk()
{
	/* No-op in malloc version */
}

STKPTR	cpystak(x)
	STKPTR		x;
{
	return(endstak(movstr(x,locstak())));
}

/* 
 * setbrk shim: 
 * main.c and other modules call this.
 */
BYTPTR	setbrk(incr)
{
    if (stakbas == NULL)
    {
        growstak(NULL);
    }
	return (BYTPTR)stakbot;
}
