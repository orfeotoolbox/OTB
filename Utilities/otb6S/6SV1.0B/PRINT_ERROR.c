/* PRINT_ERROR.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif
/* OTB patches: replace "f2c.h" by "otb_6S.h" */
/*#include "f2c.h"*/
#include "otb_6S.h"

/* Common Block Declarations */

Extern struct {
    integer iwr;
    logical ier;
} sixs_ier__;

#define sixs_ier__1 sixs_ier__

/* Table of constant values */

static integer c__1 = 1;

/*<       subroutine print_error(tex) >*/
/* Subroutine */ int print_error__(char *tex, ftnlen tex_len)
{
    /* System generated locals */
    cilist ci__1;

    /* Builtin functions */
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe();

/*<       character *(*) tex >*/
/*<       logical ier >*/
/*<       integer iwr >*/
/*<       common/sixs_ier/iwr,ier >*/
/*<       ier = .TRUE. >*/
    sixs_ier__1.ier = TRUE_;
/*<       write(iwr,'(a)')tex >*/
    ci__1.cierr = 0;
    ci__1.ciunit = sixs_ier__1.iwr;
    ci__1.cifmt = "(a)";
    s_wsfe(&ci__1);
    do_fio(&c__1, tex, tex_len);
    e_wsfe();
/*<       return >*/
    return 0;
/*<       end >*/
} /* print_error__ */

#ifdef __cplusplus
	}
#endif
