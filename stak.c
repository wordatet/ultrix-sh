#ifndef lint
static char sccsid[] = "@(#)stak.c	4.1 (Ultrix) 7/2/90";
/* Original ID:  "@(#)stak.c	4.2 8/11/83" */
#endif

#include	"defs.h"
#include    <stdlib.h>

VOID growstak();

STKPTR		stakbot=nullstr;
static size_t stak_size = 0;

/* ========	storage allocation	======== */

STKPTR	getstak(asize)
	INT		asize;
{	/* allocate requested stack */
	REG STKPTR	oldstak;
	REG INT		size;

	size=round(asize,BYTESPERWORD);
	
	/* In a malloc-based port, small stack allocations can just be malloced
	   if they are intended to be permanent, or we can use our buffer. 
	   Bourne shell's getstak() is usually for long-term stack items. */
	
	if (stakbas == NULL || (staktop + size >= brkend))
	{
		growstak(staktop + size);
	}
	
	oldstak=stakbot;
	staktop = stakbot += size;
	return(oldstak);
}

/* growstak: expand the stack buffer and update all pointers */
VOID growstak(ptr)
    STKPTR ptr;
{
    size_t old_size = stak_size;
    size_t new_size = old_size + (BRKINCR * 16); 
    size_t bot_off = (stakbas) ? (size_t)(stakbot - stakbas) : 0;
    size_t top_off = (stakbas) ? (size_t)(staktop - stakbas) : 0;
    
    /* Ensure new_size is enough for ptr if provided */
    if (ptr && stakbas && (size_t)(ptr - stakbas) + BRKINCR > new_size)
    {
        new_size = (size_t)(ptr - stakbas) + (BRKINCR * 16);
    }

    STKPTR new_bas = (STKPTR)realloc(stakbas, new_size);
    if (new_bas == NULL) error(nospace);
    
    stakbas = new_bas;
    stakbot = stakbas + bot_off;
    staktop = stakbas + top_off;
    brkend = stakbas + new_size;
    stak_size = new_size;
    
    if (old_size == 0)
    {
        stakbot = staktop = stakbas;
    }
}

STKPTR	locstak()
{
	if (stakbas == NULL || (staktop + BRKINCR >= brkend))
	{
		growstak(staktop);
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
	return (BYTPTR)stakbas;
}
