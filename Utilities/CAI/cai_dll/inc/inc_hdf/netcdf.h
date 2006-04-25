/* Generated automatically from netcdf.h.in by configure. */
/*
 *	Copyright 1993, University Corporation for Atmospheric Research
 *
 *  Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose without fee is hereby granted, provided
 * that the above copyright notice appear in all copies, that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of UCAR/Unidata not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  UCAR makes no
 * representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.  It is
 * provided with no support and without obligation on the part of UCAR
 * Unidata, to assist in its use, correction, modification, or enhancement.
 *
 */
/* "$Id$" */

#ifndef _NETCDF_
#define _NETCDF_

//RWR Modification Start 07/14/98
//#include "api_adpt.h"
#include "hdfi.h"
//RWR Modification End

/*
 * The definitions ncvoid, USE_ENUM, and MAX_NC_OPEN, may need to be set
 * properly for your installation.
 */

/*
 * Argument type in user functions (deprecated, backward compatibility)
 */
#ifndef UD_NO_VOID
#define ncvoid    void
#else
/* system doesn't have void type */
#define ncvoid    char
#endif


/*
 *   If xdr_enum works properly on your system, you can define 
 * USE_ENUM so that nc_type is an enum. 
 * Otherwise, delete this definition so that the nc_type is
 * an int and the valid values are #defined.
 */
#define USE_ENUM


/*
 * The following macro is provided for backward compatibility only.  If you
 * are a new user of netCDF, then you may safely ignore it.  If, however,
 * you have an existing archive of netCDF files that use default
 * floating-point fill values, then you should know that the definition of
 * the default floating-point fill values changed with version 2.3 of the
 * netCDF package.  Prior to this release, the default floating-point fill
 * values were not very portable:  their correct behavior depended not only
 * upon the particular platform, but also upon the compilation
 * environment.  This led to the definition of new, default floating-point
 * fill values that are portable across all platforms and compilation
 * environments.  If you wish, however, to obtain the old, non-portable
 * floating-point fill values, then the following macro should have a true
 * value PRIOR TO BUILDING THE netCDF LIBRARY.
 *
 * Implementation details are contained in the section below on fill values.
 */
#define NC_OLD_FILLVALUES	0

/*
 * 	Fill values
 * These values are stuffed into newly allocated space as appropriate.
 * The hope is that one might use these to notice that a particular datum
 * has not been set.
 */

#define FILL_BYTE	((char)-127)		/* Largest Negative value */
#define FILL_CHAR	((char)0)
#define FILL_SHORT	((short)-32767)
#define FILL_LONG	((long)-2147483647)

#if !NC_OLD_FILLVALUES

#   define FILL_FLOAT	9.9692099683868690e+36 /* near 15 * 2^119 */
#   define FILL_DOUBLE	9.9692099683868690e+36

#else	/* NC_OLD_FILLVALUES below */

/*
 * This section is provided for backward compatibility only.  Using
 * XDR infinities for floating-point fill values has caused more problems
 * than it has solved.  We encourage you to define your own data-specific
 * fill values rather than use default ones (see `_FillValue' below).
 * If, however, you *must* use default fill values, then you should use
 * the above fill values rather than the ones in this section.
 */

/*
 * XDR_F_INFINITY is a float value whose EXTERNAL (xdr)
 * represention is ieee floating infinity.
 * XDR_D_INFINITY is a double value whose EXTERNAL (xdr)
 * represention is ieee double floating point infinity.
 * These are used as default fill values below.
 *
 * This section shows three techniques for setting these:
 *  Direct assignment (vax, cray) - works for non IEEE machines
 *		Doesn't work when IEEE machines don't allow
 *      float or double constants whose values are infinity.
 *  Use of a union (preferred portable method) - should work on
 *      any ANSI compiler with IEEE floating point representations,
 *      modulo byte order and sizeof() considerations.
 *  Use of pointer puns - may work with many older compilers
 *      which don't allow intialization of unions.
 *      Often doesn't work with compilers which have strict
 *      alignment rules.
 */ 

    /* Direct assignment. All cases should be mutually exclusive */

#ifdef vax
#define	XDR_D_INFINITY	1.7014118346046923e+38
#define	XDR_F_INFINITY	1.70141173e+38
#endif /* vax */

#ifdef cray
#define	XDR_D_INFINITY	1.797693134862313000e+308
#define	XDR_F_INFINITY	XDR_D_INFINITY
#endif /* cray */

#ifdef notdef /* you might want to try these, on an IEEE machine */
#define XDR_D_INFINITY	1.797693134862315900e+308
#define XDR_F_INFINITY	3.40282357e+38
#endif

#ifdef __STDC__
    /* Use of a union, assumes IEEE representation and 1 byte unsigned char */

#ifndef    XDR_D_INFINITY
#define USE_D_UNION
     union xdr_d_union {unsigned char bb[8]; double dd;} ;
     extern union xdr_d_union xdr_d_infs ;  /* instantiated in array.c */
#define XDR_D_INFINITY    (xdr_d_infs.dd)
#endif /* !XDR_D_INFINITY */

#ifndef    XDR_F_INFINITY
#define USE_F_UNION
     union xdr_f_union {unsigned char bb[4]; float ff;} ;
     extern union xdr_f_union xdr_f_infs ;  /* instantiated in array.c */
#define  XDR_F_INFINITY    (xdr_f_infs.ff)
#endif /* !XDR_F_INFINITY */


#else /* __STDC__ */
    /* Use of a pointer pun, assumes IEEE representation, 4 byte long */

#ifndef    XDR_D_INFINITY
#define USE_D_LONG_PUN
     extern long xdr_d_infinity[] ;  /* instantiated in array.c */
#define XDR_D_INFINITY *(double *)xdr_d_infinity
#endif /* !XDR_D_INFINITY */

#ifndef    XDR_F_INFINITY
#define USE_F_LONG_PUN
     extern long xdr_f_infinity ;  /* instantiated in array.c */
#define XDR_F_INFINITY *((float *)&xdr_f_infinity)
#endif /* !XDR_F_INFINITY */

#endif /* __STDC__ */

/* End of INFINITY           section */

#define FILL_FLOAT	XDR_F_INFINITY	/* IEEE Infinity */
#define FILL_DOUBLE	XDR_D_INFINITY

#endif	/* NC_OLD_FILLVALUES above */

/*
 *  masks for the struct NC flags field; passed in as 'mode' arg to
 * nccreate and ncopen.
 *
 */
#define NC_RDWR  1		/* read/write, 0 => readonly */
#define NC_CREAT 2		/* in create phase, cleared by ncendef */
#define NC_EXCL  4		/* on create, don't destroy existing file */
#define NC_INDEF 8		/* in define mode, cleared by ncendef */
#define NC_NSYNC 0x10	/* synchronise numrecs on change */
#define NC_HSYNC 0x20	/* synchronise whole header on change */
#define NC_NDIRTY 0x40	/* numrecs has changed */
#define NC_HDIRTY 0x80  /* header info has changed */
#define NC_NOFILL 0x100	/* Don't fill vars on endef and increase of record */
#define NC_LINK 0x8000	/* isa link */

#define NC_FILL 0	/* argument to ncsetfill to clear NC_NOFILL */

/*
 * 'mode' arguments for nccreate and ncopen
 */
#define NC_NOWRITE   0
#define NC_WRITE     NC_RDWR
#define NC_CLOBBER   (NC_INDEF | NC_CREAT | NC_RDWR)
#define NC_NOCLOBBER (NC_INDEF | NC_EXCL | NC_CREAT | NC_RDWR)

/*
 * 'size' argument to ncdimdef for an unlimited dimension
 */
#define NC_UNLIMITED 0L

/*
 * attribute id to put/get a global attribute
 */
#define NC_GLOBAL -1

#ifndef HDF
/*
 * This can be as large as the maximum number of stdio streams
 * you can have open on your system.
 */
#define MAX_NC_OPEN 32

/*
 * These maximums are enforced by the interface, to facilitate writing
 * applications and utilities.  However, nothing is statically allocated to
 * these sizes internally.
 */
#define MAX_NC_DIMS 5000	 /* max dimensions per file */
#define MAX_NC_ATTRS 3000	 /* max global or per variable attributes */
#define MAX_NC_VARS 5000	 /* max variables per file */
#define MAX_NC_NAME 256		 /* max length of a name */
#define MAX_VAR_DIMS 32          /* max per variable dimensions */

/*
 * Added feature. 
 * If you wish a variable to use a different value than the above
 * defaults, create an attribute with the same type as the variable
 * and the following reserved name. The value you give the attribute
 * will be used as the fill value for that variable.
 */
#define _FillValue	"_FillValue"

#else /* HDF */

#include "hlimits.h"  /* Hard coded constants for HDF library */

#endif /* HDF */

#ifdef USE_ENUM
/*
 *  The netcdf data types
 */
typedef enum {
	NC_UNSPECIFIED, /* private */
	NC_BYTE,
	NC_CHAR,
	NC_SHORT,
	NC_LONG,
	NC_FLOAT,
	NC_DOUBLE,
	/* private */
	NC_BITFIELD,
	NC_STRING,
	NC_IARRAY,
	NC_DIMENSION,
	NC_VARIABLE,
	NC_ATTRIBUTE
} nc_type ;
#else
typedef int nc_type ;
#define	NC_UNSPECIFIED 0 /* private */
#define	NC_BYTE 1
#define	NC_CHAR 2
#define	NC_SHORT 3
#define	NC_LONG 4
#define	NC_FLOAT 5
#define	NC_DOUBLE 6
	/* private */
#define	NC_BITFIELD 7
#define	NC_STRING 8
#define	NC_IARRAY 9
#define	NC_DIMENSION 10
#define	NC_VARIABLE 11
#define	NC_ATTRIBUTE 12
#endif


/*
 * C data types corresponding to netCDF data types:
 */
/* Don't use these or the C++ interface gets confused
typedef char  ncchar;
typedef char  ncbyte;
typedef short ncshort;
typedef float ncfloat;
typedef double        ncdouble;
*/

/* 
 * Variables/attributes of type NC_LONG should use the C type 'nclong'
 */
#ifdef __alpha
typedef int     nclong;   
#else
typedef long    nclong;         /* default, compatible type */
#endif


/*
 * Global netcdf error status variable
 *  Initialized in error.c
 */
#define	NC_NOERR	0	/* No Error */
#define	NC_EBADID	1	/* Not a netcdf id */
#define	NC_ENFILE	2	/* Too many netcdfs open */
#define	NC_EEXIST	3	/* netcdf file exists && NC_NOCLOBBER */
#define	NC_EINVAL	4	/* Invalid Argument */
#define	NC_EPERM	5	/* Write to read only */
#define	NC_ENOTINDEFINE	6	/* Operation not allowed in data mode */
#define	NC_EINDEFINE	7	/* Operation not allowed in define mode */
#define	NC_EINVALCOORDS	8	/* Coordinates out of Domain */
#define	NC_EMAXDIMS	9	/* MAX_NC_DIMS exceeded */
#define	NC_ENAMEINUSE	10	/* String match to name in use */
#define NC_ENOTATT	11	/* Attribute not found */
#define	NC_EMAXATTS	12	/* MAX_NC_ATTRS exceeded */
#define NC_EBADTYPE	13	/* Not a netcdf data type */
#define NC_EBADDIM	14	/* Invalid dimension id */
#define NC_EUNLIMPOS	15	/* NC_UNLIMITED in the wrong index */
#define	NC_EMAXVARS	16	/* MAX_NC_VARS exceeded */
#define NC_ENOTVAR	17	/* Variable not found */
#define NC_EGLOBAL	18	/* Action prohibited on NC_GLOBAL varid */
#define NC_ENOTNC	19	/* Not a netcdf file */
#define NC_ESTS         20      /* In Fortran, string too short */
#define NC_EMAXNAME     21      /* MAX_NC_NAME exceeded */
#define NC_ENTOOL       NC_EMAXNAME   /* Backward compatibility */
#define NC_EUNLIMIT     22      /* NC_UNLIMITED size already in use */

#define	NC_EXDR		32	/* */
#define	NC_SYSERR	-1

extern int ncerr ;

/*
 * Global options variable. Used to determine behavior of error handler.
 *  Initialized in lerror.c
 */
#define	NC_FATAL	1
#define	NC_VERBOSE	2

HDFLIBAPI int ncopts ;	/* default is (NC_FATAL | NC_VERBOSE) */

/*
 * NB: The following feature-test line is too long in order to accomodate a 
 * bug in the VMS 5.3 C compiler.
 */
#ifndef HAVE_PROTOTYPES
#   if defined(__STDC__) || defined(__GNUC__) || defined(__cplusplus) || defined(c_plusplus)
#       define	HAVE_PROTOTYPES
#   endif
#endif

#undef PROTO
#ifdef HAVE_PROTOTYPES 
#   define	PROTO(x)	x
#else
#   define	PROTO(x)	()
#endif

#include "hdf2netcdf.h"

#ifdef __cplusplus
extern "C" {
#endif

HDFLIBAPI int nccreate	PROTO((
    const char*	path,
    int		cmode
));
HDFLIBAPI int ncopen	PROTO((
    const char*	path,
    int		mode
));
HDFLIBAPI int ncredef	PROTO((
    int		cdfid
));
HDFLIBAPI int ncendef	PROTO((
    int		cdfid
));
HDFLIBAPI int ncclose	PROTO((
    int		cdfid
));
HDFLIBAPI int ncinquire	PROTO((
    int		cdfid,
    int*	ndims,
    int*	nvars,
    int*	natts, 
    int*	recdim
));
HDFLIBAPI int ncsync	PROTO((
    int		cdfid
));
HDFLIBAPI int ncabort	PROTO((
    int		cdfid
));
HDFLIBAPI int ncdimdef	PROTO((
    int		cdfid,
    const char*	name,
    long	length
));
HDFLIBAPI int ncdimid	PROTO((
    int		cdfid,
    const char*	name
));
HDFLIBAPI int ncdiminq	PROTO((
    int		cdfid,
    int		dimid,
    char*	name,
    long*	length
));
HDFLIBAPI int ncdimrename	PROTO((
    int		cdfid,
    int		dimid,
    const char*	name
));
HDFLIBAPI int ncvardef	PROTO((
    int		cdfid,
    const char*	name,
    nc_type	datatype, 
    int		ndims,
    const int*	dim
));
HDFLIBAPI int ncvarid	PROTO((
    int		cdfid,
    const char*	name
));
HDFLIBAPI int ncvarinq	PROTO((
    int		cdfid,
    int		varid,
    char*	name,
    nc_type*	datatype,
    int*	ndims,
    int*	dim,
    int*	natts
));
HDFLIBAPI int ncvarput1	PROTO((
    int		cdfid,
    int		varid,
    const long*	coords,
    const void*	value
));
HDFLIBAPI int ncvarget1	PROTO((
    int		cdfid,
    int		varid,
    const long*	coords,
    void*	value
));
HDFLIBAPI int ncvarput	PROTO((
    int		cdfid,
    int		varid,
    const long*	start,
    const long*	count, 
    const void*	value
));
HDFLIBAPI int ncvarget	PROTO((
    int		cdfid,
    int		varid,
    const long*	start,
    const long*	count, 
    void*	value
));
HDFLIBAPI int ncvarputs	PROTO((
    int		cdfid,
    int		varid,
    const long*	start,
    const long*	count,
    const long*	stride,
    const void*	values
));
HDFLIBAPI int ncvargets	PROTO((
    int		cdfid,
    int		varid,
    const long*	start,
    const long*	count,
    const long*	stride,
    void*	values
));
HDFLIBAPI int ncvarputg	PROTO((
    int		cdfid,
    int		varid,
    const long*	start,
    const long*	count,
    const long*	stride,
    const long*	imap,
    const void* values
));
HDFLIBAPI int ncvargetg	PROTO((
    int		cdfid,
    int		varid,
    const long*	start,
    const long*	count,
    const long*	stride,
    const long*	imap,
    void*	values
));
HDFLIBAPI int ncvarrename	PROTO((
    int		cdfid,
    int		varid,
    const char*	name
));
HDFLIBAPI int ncattput	PROTO((
    int		cdfid,
    int		varid,
    const char*	name, 
    nc_type	datatype,
    int		len,
    const void*	value
));
HDFLIBAPI int ncattinq	PROTO((
    int		cdfid,
    int		varid,
    const char*	name, 
    nc_type*	datatype,
    int*	len
));
HDFLIBAPI int ncattget	PROTO((
    int		cdfid,
    int		varid,
    const char*	name, 
    void*	value
));
HDFLIBAPI int ncattcopy	PROTO((
    int		incdf,
    int		invar,
    const char*	name, 
    int		outcdf,
    int		outvar
));
HDFLIBAPI int ncattname	PROTO((
    int		cdfid,
    int		varid,
    int		attnum,
    char*	name
));
HDFLIBAPI int ncattrename	PROTO((
    int		cdfid,
    int		varid,
    const char*	name, 
    const char*	newname
));
HDFLIBAPI int ncattdel	PROTO((
    int		cdfid,
    int		varid,
    const char*	name
));
HDFLIBAPI int nctypelen	PROTO((
    nc_type	datatype
));
HDFLIBAPI int ncsetfill	PROTO((
    int		cdfid,
    int		fillmode
));
HDFLIBAPI int ncrecinq		PROTO((
    int		cdfid,
    int*	nrecvars,
    int*	recvarids,
    long*	recsizes
));
HDFLIBAPI int ncrecget		PROTO((
    int		cdfid,
    long	recnum,
    void**	datap
));
HDFLIBAPI int ncrecput		PROTO((
    int		cdfid,
    long	recnum,
    void* const* datap
));
#ifdef __cplusplus
}
#endif

#endif /* _NETCDF_ */
