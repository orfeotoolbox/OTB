/****************************************************************************
 * NCSA HDF                                                                 *
 * Software Development Group                                               *
 * National Center for Supercomputing Applications                          *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                        *
 *                                                                          *
 ****************************************************************************/

/* $Id$ */

#ifndef _H_PROTO
#define _H_PROTO

/* Usefull macros, which someday might become actual functions */
/* Wrappers for Hinquire. feb-2-92 */
#define HQueryfileid(aid, fileid)     \
  (Hinquire ((int32)   aid,  (int32*) fileid, (uint16*) NULL,\
             (uint16*) NULL, (int32*) NULL,   (int32*)  NULL,\
             (int32*)  NULL, (int16*) NULL,   (int16*)  NULL))

#define HQuerytagref(aid, tag, ref) \
  (Hinquire ((int32)   aid,  (int32*) NULL,   (uint16*) tag,\
             (uint16*) ref,  (int32*) NULL,   (int32*)  NULL,\
             (int32*)  NULL, (int16*) NULL,   (int16*)  NULL))

#define HQuerylength(aid, length)     \
  (Hinquire ((int32)    aid, (int32*) NULL, (uint16*) NULL,   \
             (uint16*) NULL, (int32*) length,   (int32*)  NULL,   \
             (int32*)  NULL, (int16*) NULL, (int16*)  NULL))

#define HQueryoffset(aid, offset)     \
  (Hinquire ((int32)    aid, (int32*) NULL, (uint16*) NULL,   \
             (uint16*) NULL, (int32*) NULL,     (int32*)  offset, \
             (int32*)  NULL, (int16*) NULL,     (int16*)  NULL))

#define HQueryposition(aid, position) \
  (Hinquire ((int32)    aid, (int32*) NULL, (uint16*) NULL,   \
             (uint16*) NULL, (int32*) NULL, (int32*)  NULL,   \
             (int32*) position, (int16*) NULL,  (int16*)  NULL))

#define HQueryaccess(aid, access)     \
  (Hinquire ((int32)    aid,    (int32*) NULL,   (uint16*) NULL,  \
             (uint16*) NULL,    (int32*) NULL,   (int32*)  NULL,  \
             (int32*)   NULL,   (int16*) access, (int16*)  NULL))

#define HQueryspecial(aid, special) \
  (Hinquire ((int32)    aid,    (int32*) NULL,  (uint16*) NULL,   \
             (uint16*) NULL,    (int32*) NULL,  (int32*)  NULL,   \
             (int32*)   NULL,   (int16*) NULL,  (int16*)  special))


#if defined c_plusplus || defined __cplusplus
extern      "C"
{
#endif                          /* c_plusplus || __cplusplus */

/*
   ** from hfile.c
 */
    HDFLIBAPI int32 Hopen
                (const char *path, intn acc_mode, int16 ndds);

    HDFLIBAPI intn Hclose
                (int32 file_id);

    HDFLIBAPI int32 Hstartread
                (int32 file_id, uint16 tag, uint16 ref);

    HDFLIBAPI intn Hnextread
                (int32 access_id, uint16 tag, uint16 ref, intn origin);

    HDFLIBAPI intn Hexist
                (int32 file_id, uint16 search_tag, uint16 search_ref);

    HDFLIBAPI intn Hinquire
                (int32 access_id, int32 * pfile_id, uint16 * ptag,
          uint16 * pref, int32 * plength, int32 * poffset,
        int32 * pposn, int16 * paccess, int16 * pspecial);

    HDFLIBAPI int32 Hstartwrite
                (int32 file_id, uint16 tag, uint16 ref, int32 length);

    HDFLIBAPI int32 Hstartaccess
                (int32 file_id, uint16 tag, uint16 ref, uint32 flags);

    HDFLIBAPI intn Hsetlength
                (int32 file_id, int32 length);

    HDFLIBAPI intn Happendable
                (int32 aid);

    HDFLIBAPI intn HPisappendable
                (int32 aid);

    HDFLIBAPI intn HPregister_term_func
                (hdf_termfunc_t term_func);

    HDFLIBAPI intn Hseek
                (int32 access_id, int32 offset, intn origin);

    HDFLIBAPI int32 Htell
                (int32 access_id);

    HDFLIBAPI int32 Hread
                (int32 access_id, int32 length, void * data);

    HDFLIBAPI int32 Hwrite
                (int32 access_id, int32 length, const void * data);

    HDFLIBAPI int32 Htrunc
                (int32 access_id, int32 trunc_len);

    HDFLIBAPI intn Hendaccess
                (int32 access_id);

    HDFLIBAPI intn HDgetc
                (int32 access_id);

    HDFLIBAPI intn HDputc
                (uint8 c, int32 access_id);

    HDFLIBAPI int32 Hgetelement
                (int32 file_id, uint16 tag, uint16 ref, uint8 * data);

    HDFLIBAPI int32 Hputelement
                (int32 file_id, uint16 tag, uint16 ref, const uint8 * data, int32 length);

    HDFLIBAPI int32 Hlength
                (int32 file_id, uint16 tag, uint16 ref);

    HDFLIBAPI int32 Hoffset
                (int32 file_id, uint16 tag, uint16 ref);

    HDFLIBAPI intn Hsync
                (int32 file_id);

    HDFLIBAPI intn Hcache
                (int32 file_id, intn cache_on);

    HDFLIBAPI intn Hgetlibversion
                (uint32 * majorv, uint32 * minorv,
                 uint32 * releasev, char * string);

    HDFLIBAPI intn Hgetfileversion
                (int32 file_id, uint32 * majorv, uint32 * minorv,
                 uint32 * release, char * string);

    HDFLIBAPI intn Hsetaccesstype(int32 access_id, uintn accesstype);

    HDFLIBAPI uint16 HDmake_special_tag
                (uint16 tag);

    HDFLIBAPI intn HDis_special_tag
                (uint16 tag);

    HDFLIBAPI uint16 HDbase_tag
                (uint16 tag);

    HDFLIBAPI int  HDerr
                (int32 file_id);

    HDFLIBAPI intn HDvalidfid
                (int32 file_id);

    HDFLIBAPI const char *HDgettagdesc
                (uint16 tag);

    HDFLIBAPI char *HDgettagsname
                (uint16 tag);

    HDFLIBAPI intn HDgettagnum
                (const char *tag_name);

    HDFLIBAPI char *HDgetNTdesc
                (int32 nt);

    HDFLIBAPI const char *HDfidtoname
                (int32 fid);

    HDFLIBAPI intn Hishdf
                (const char * filename);

    HDFLIBAPI intn Hfidinquire
                (int32 file_id, char ** fname, intn * acc_mode,
                 intn * attach);
    
    HDFLIBAPI intn Hshutdown(void);

    HDFLIBAPI void HPend(void);

    HDFLIBAPI intn HDdont_atexit(void);

/*
   ** from hfiledd.c
 */
/******************************************************************************
 NAME
     Hdupdd - Duplicate a data descriptor

 DESCRIPTION
    Duplicates a data descriptor so that the new tag/ref points to the
    same data element pointed to by the old tag/ref.  Return FAIL if
    the given tag/ref are already in use.

 RETURNS
    returns SUCCEED (0) if successful, FAIL (-1) otherwise

*******************************************************************************/
HDFLIBAPI intn Hdupdd(int32 file_id,      /* IN: File ID the tag/refs are in */
        uint16 tag,             /* IN: Tag of new tag/ref */
        uint16 ref,             /* IN: Ref of new tag/ref */
        uint16 old_tag,         /* IN: Tag of old tag/ref */
        uint16 old_ref          /* IN: Ref of old tag/ref */
);

/******************************************************************************
 NAME
     Hnumber - Determine the number of objects of a given type

 DESCRIPTION
    Determine how many objects of the given tag are in the file.
    tag may be set to DFTAG_WILDCARD to get back the total number
    of objects in the file.

    Note, a return value of zero is not a fail condition.

 RETURNS
    the number of objects of type 'tag' else FAIL

*******************************************************************************/
HDFLIBAPI int32 Hnumber(int32 file_id,    /* IN: File ID the tag/refs are in */
        uint16 tag              /* IN: Tag to count */
);

/******************************************************************************
 NAME
     Hnewref - Returns a ref that is guaranteed to be unique in the file

 DESCRIPTION
    Returns a ref number that can be used with any tag to produce a
    unique tag/ref.  Successive calls to Hnewref will generate a
    strictly increasing sequence until the highest possible ref had been
    returned, then Hnewref will return unused ref's starting from 1.

 RETURNS
    returns the ref number, 0 otherwise

*******************************************************************************/
HDFLIBAPI uint16 Hnewref(int32 file_id        /* IN: File ID the tag/refs are in */
);

/******************************************************************************
 NAME
    Htagnewref  - returns a ref that is unique in the file for a given tag

 DESCRIPTION
    Returns a ref number that can be used with any tag to produce a
    unique tag/ref.  Successive calls to Hnewref will generate a
    strictly increasing sequence until the highest possible ref had been
    returned, then Hnewref will return unused ref's starting from 1.

 RETURNS
    returns the ref number, 0 otherwise

*******************************************************************************/
HDFLIBAPI uint16 Htagnewref(int32 file_id,    /* IN: File ID the tag/refs are in */
        uint16 tag                  /* IN: Tag to search for a new ref for */
);

/******************************************************************************
 NAME
    Hfind - locate the next object of a search in an HDF file

 DESCRIPTION
    Searches for the `next' DD that fits the search tag/ref.  Wildcards
    apply.  If origin is DF_FORWARD, search from current position forwards
    in the file, otherwise DF_BACKWARD searches backward from the current
    position in the file.  If *find_tag and *find_ref are both set to
    0, this indicates the beginning of a search, and the search will
    start from the beginning of the file if the direction is DF_FORWARD
    and from the and of the file if the direction is DF_BACKWARD.

 RETURNS
    returns SUCCEED (0) if successful and FAIL (-1) otherwise

*******************************************************************************/
HDFLIBAPI intn Hfind(int32 file_id,       /* IN: file ID to search in */
        uint16 search_tag,      /* IN: the tag to search for (can be DFTAG_WILDCARD) */
        uint16 search_ref,      /* IN: ref to search for (can be DFREF_WILDCARD) */
        uint16 *find_tag,       /* IN: if (*find_tag==0) and (*find_ref==0) then start search */
                                /* OUT: tag matching the search tag */
        uint16 *find_ref,       /* IN: if (*find_tag==0) and (*find_ref==0) then start search */
                                /* OUT: ref matching the search ref */
        int32 *find_offset,     /* OUT: offset of the data element found */
        int32 *find_length,     /* OUT: length of the data element found */
        intn direction          /* IN: Direction to search in: */
                                /*  DF_FORWARD searches forward from the current location */
                                /*  DF_BACKWARD searches backward from the current location */
);


/******************************************************************************
 NAME
     HDcheck_tagref - Checks to see if tag/ref is in DD list i.e. created already

 DESCRIPTION
     Routine checks to see if tag/ref exists in the DD list i.e. has
     been created.

 RETURNS
     0-> tag/ref does not exist
     1-> tag/ref exists
    -1-> function failed

*******************************************************************************/
HDFLIBAPI intn 
HDcheck_tagref(int32  file_id, /* IN: id of file */
               uint16 tag,     /* IN: Tag to check */
               uint16 ref      /* IN: ref to check */);

/************************************************************************
NAME
   HDreuse_tagref -- reuse a data descriptor preserving tag/ref

DESCRIPTION
   Reuses the data descriptor of tag/ref in the dd list of the file.
   The tag/ref must already exist in the DD list.
   This routine is unsafe and may leave a file in a condition that is
   not usable by some routines.  Use with care. Not valid for
   special elments right now. Used for allowing the data to change
   and move somewhere else in the file for non-special elements.
   Must be carefully if apply to higher-level objects like GR's and SDS
   that are comprised of other objects. 
   Usefull when re-writing simple elements whose size changes while
   preserving the original tag/ref of the element since other elements
   might refer to this element by tag/ref e.g. in a Vgroup.

RETURNS
   returns SUCCEED (0) if successful, FAIL (-1) otherwise
************************************************************************/
HDFLIBAPI intn 
HDreuse_tagref(int32 file_id, /* IN: id of file */
               uint16 tag,    /* IN: tag of data descriptor to reuse */
               uint16 ref     /* IN: ref of data descriptor to reuse */ );

/******************************************************************************
 NAME
     Hdeldd - Delete a data descriptor

 DESCRIPTION
    Deletes a data descriptor of tag/ref from the dd list of the file.
    This routine is unsafe and may leave a file in a condition that is
    not usable by some routines.  Use with care.
    For example, if this element is contained in a Vgroup, that group
    will *NOT* get updated to reflect that this element has been deleted.

 RETURNS
    returns SUCCEED (0) if successful, FAIL (-1) otherwise

*******************************************************************************/
HDFLIBAPI intn Hdeldd(int32 file_id,      /* IN: File ID the tag/refs are in */
        uint16 tag,             /* IN: Tag of tag/ref to delete */
        uint16 ref              /* IN: Ref of tag/ref to delete */
);

/*
   ** from hdfalloc.c
 */

    HDFLIBAPI void * HDmemfill
                (void * dest, const void * src, uint32 item_size, uint32 num_items);

    HDFLIBAPI char *HIstrncpy
                (char * dest, const char * source, int32 len);

    HDFLIBAPI int32 HDspaceleft
                (void);

#if defined(MALLOC_CHECK) || defined(_HDFDLL_)
    HDFPUBLIC extern void * HDmalloc
                (uint32 qty);

    HDFPUBLIC extern void * HDrealloc
                (void * where, uint32 qty);

    HDFPUBLIC extern void * HDcalloc
                (uint32 n, uint32 size);

    HDFPUBLIC extern void HDfree
                (void * ptr);

#endif /* defined MALLOC_CHECK */

#if defined VMS || defined macintosh || defined MAC || defined SYMANTEC_C || defined MIPSEL || defined NEXT || defined CONVEX || defined IBM6000 || defined SUN || defined IRIX || defined _HDFDLL_
    HDFPUBLIC extern char *HDstrdup
                (const char *s);

#endif

    HDFLIBAPI intn HDc2fstr
                (char * str, intn len);

    HDFLIBAPI char *HDf2cstring
                (_fcd fdesc, intn len);

    HDFLIBAPI intn HDflush
                (int32 file_id);

    HDFLIBAPI intn HDpackFstring
                (char * src, char * dest, intn len);

/*
   ** from hblocks.c
 */
    HDFLIBAPI int32 HLcreate
                (int32 file_id, uint16 tag, uint16 ref, int32 block_length,
                 int32 number_blocks);

    HDFLIBAPI intn HLconvert
                (int32 aid, int32 block_length, int32 number_blocks);

    HDFLIBAPI int  HDinqblockinfo
                (int32 aid, int32 *length, int32 *first_length, int32 *block_length,
                 int32 *number_blocks);

    HDFLIBAPI intn HLsetblockinfo
                (int32 aid, int32 block_size, int32 num_blocks);

    HDFLIBAPI intn HLgetblockinfo
                (int32 aid, int32* block_size, int32* num_blocks);

/*
   ** from hextelt.c
 */
    HDFLIBAPI int32 HXcreate
                (int32 file_id, uint16 tag, uint16 ref, const char * extern_file_name,
                 int32 offset, int32 start_len);

    HDFLIBAPI intn HXsetcreatedir
                (const char *dir);

    HDFLIBAPI intn HXsetdir
                (const char *dir);

/*
   ** from hcomp.c
 */
    HDFLIBAPI int32 HCcreate
                (int32 file_id, uint16 tag, uint16 ref,
                 comp_model_t model_type, model_info * m_info,
                 comp_coder_t coder_type, comp_info * c_info);

    HDFLIBAPI intn HCPgetcompress
                (int32 file_id, uint16 data_tag, uint16 data_ref, 
		 comp_coder_t *coder_type, comp_info * c_info);

    HDFPUBLIC intn HCget_config_info ( comp_coder_t coder_type, uint32 *compression_config_info);

    HDFLIBAPI int32 HCPquery_encode_header(comp_model_t model_type, model_info * m_info,
             comp_coder_t coder_type, comp_info * c_info);

    HDFLIBAPI intn HCPencode_header(uint8 *p, comp_model_t model_type, model_info * m_info,
             comp_coder_t coder_type, comp_info * c_info);

    HDFLIBAPI intn HCPdecode_header(uint8 *p, comp_model_t *model_type, model_info * m_info,
             comp_coder_t *coder_type, comp_info * c_info);

/*
   ** from cszip.c
 */

     HDFLIBAPI intn HCPsetup_szip_parms ( comp_info *c_info, int32 nt, int32 ncomp, int32 ndims, int32 *dims, int32 *cdims);
/*
   ** from hbuffer.c
 */
    HDFLIBAPI intn HBconvert
                (int32 aid);

/*
   ** from hcompri.c
 */
    HDFLIBAPI int32 HRPconvert
                (int32 fid, uint16 tag, uint16 ref, int32 xdim, int32 ydim,int16 scheme, comp_info *cinfo, uintn pixel_size);

/*
   ** from herr.c
 */
    HDFLIBAPI const char *HEstring
                (hdf_err_code_t error_code);

    HDFLIBAPI void HEpush
                (hdf_err_code_t error_code, const char * function_name,
                 const char * file_name, intn line);

    HDFLIBAPI void HEreport
                (const char *,...);

    HDFLIBAPI void HEprint
                (FILE * stream, int32 print_level);

    HDFLIBAPI int16 HEvalue
                (int32 level);

    HDFLIBAPI void HEPclear
                (void);

 	HDFLIBAPI intn HEshutdown(void);

/*
   ** from hbitio.c
 */
    HDFLIBAPI int32 Hstartbitread
                (int32 file_id, uint16 tag, uint16 ref);

    HDFLIBAPI int32 Hstartbitwrite
                (int32 file_id, uint16 tag, uint16 ref, int32 length);

    HDFLIBAPI intn Hbitappendable
                (int32 bitid);

    HDFLIBAPI intn Hbitwrite
                (int32 bitid, intn count, uint32 data);

    HDFLIBAPI intn Hbitread
                (int32 bitid, intn count, uint32 *data);

    HDFLIBAPI intn Hbitseek
                (int32 bitid, int32 byte_offset, intn bit_offset);

    HDFLIBAPI intn Hgetbit
                (int32 bitid);

    HDFLIBAPI int32 Hendbitaccess
                (int32 bitfile_id, intn flushbit);

    HDFLIBAPI intn HPbitshutdown(void);

/*
   ** from dfcomp.c
 */
    HDFLIBAPI intn DFputcomp
                (int32 file_id, uint16 tag, uint16 ref, const uint8 * image,
        int32 xdim, int32 ydim, uint8 * palette, uint8 * newpal,
                 int16 scheme, comp_info * cinfo);

    HDFLIBAPI int  DFgetcomp
                (int32 file_id, uint16 tag, uint16 ref, uint8 * image,
                 int32 xdim, int32 ydim, uint16 scheme);

/*
   ** from dfrle.c
 */
    HDFLIBAPI int32 DFCIrle
                (const void * buf, void * bufto, int32 len);

    HDFLIBAPI int32 DFCIunrle
                (uint8 * buf, uint8 *bufto, int32 outlen, int resetsave);

/*
   ** from dfimcomp.c
 */
    HDFLIBAPI void DFCIimcomp
                (int32 xdim, int32 ydim, const uint8 *in, uint8 out[],
                 uint8 in_pal[], uint8 out_pal[], int mode);

    HDFLIBAPI void DFCIunimcomp
                (int32 xdim, int32 ydim, uint8 in[], uint8 out[]);

/*
   ** from dfjpeg.c
 */

    HDFLIBAPI intn DFCIjpeg
                (int32 file_id, uint16 tag, uint16 ref, int32 xdim, int32 ydim,
                 const void * image, int16 scheme, comp_info * scheme_info);

/*
   ** from dfunjpeg.c
 */

    HDFLIBAPI intn DFCIunjpeg
                (int32 file_id, uint16 tag, uint16 ref, void * image, int32 xdim,
                 int32 ydim, int16 scheme);

/*
   ** from dfgroup.c
 */
    HDFLIBAPI int32 DFdiread
                (int32 file_id, uint16 tag, uint16 ref);

    HDFLIBAPI intn DFdiget
                (int32 list, uint16 * ptag, uint16 * pref);

    HDFLIBAPI intn DFdinobj
                (int32 list);

    HDFLIBAPI int32 DFdisetup
                (int maxsize);

    HDFLIBAPI intn DFdiput
                (int32 list, uint16 tag, uint16 ref);

    HDFLIBAPI intn DFdiwrite
                (int32 file_id, int32 list, uint16 tag, uint16 ref);
                
    HDFLIBAPI void DFdifree
    			(int32 groupID);

/*
   ** from dfp.c
 */
    HDFLIBAPI intn DFPgetpal
                (const char * filename, void * palette);

    HDFLIBAPI intn DFPputpal
                (const char * filename, const void * palette, intn overwrite, const char * filemode);

    HDFLIBAPI intn DFPaddpal
                (const char * filename, const void * palette);

    HDFLIBAPI intn DFPnpals
                (const char * filename);

    HDFLIBAPI intn DFPreadref
                (const char * filename, uint16 ref);

    HDFLIBAPI intn DFPwriteref
                (const char * filename, uint16 ref);

    HDFLIBAPI intn DFPrestart
                (void);

    HDFLIBAPI uint16 DFPlastref
                (void);

/*
   ** from dfr8.c
 */
    HDFLIBAPI int  DFR8setcompress
                (int32 scheme, comp_info * cinfo);

    HDFLIBAPI intn DFR8getdims
                (const char * filename, int32 * pxdim, int32 * pydim,
                 int * pispal);

    HDFLIBAPI intn DFR8getimage
                (const char * filename, uint8 * image, int32 xdim, int32 ydim,
                 uint8 * pal);

    HDFLIBAPI intn DFR8setpalette
                (uint8 * pal);

    HDFLIBAPI intn DFR8putimage
                (const char * filename, const void * image, int32 xdim, int32 ydim, uint16 compress);

    HDFLIBAPI intn DFR8addimage
                (const char * filename, const void * image, int32 xdim, int32 ydim, uint16 compress);

    HDFLIBAPI intn DFR8nimages
                (const char * filename);

    HDFLIBAPI intn DFR8readref
                (const char * filename, uint16 ref);

    HDFLIBAPI intn DFR8writeref
                (const char * filename, uint16 ref);

    HDFLIBAPI intn DFR8restart
                (void);

    HDFLIBAPI uint16 DFR8lastref
                (void);

    HDFLIBAPI intn DFR8getpalref(uint16 *pal_ref);

    HDFLIBAPI intn DFR8Pshutdown(void);

/*
   ** from dfgr.c
 */
    HDFLIBAPI intn DFGRgetlutdims
                (const char * filename, int32 * pxdim, int32 * pydim,
                 intn * pncomps, intn * pil);

    HDFLIBAPI intn DFGRreqlutil
                (intn il);

    HDFLIBAPI intn DFGRgetlut
                (const char * filename, void * lut, int32 xdim, int32 ydim);

    HDFLIBAPI intn DFGRgetimdims
                (const char * filename, int32 * pxdim, int32 * pydim,
                 intn * pncomps, intn * pil);

    HDFLIBAPI intn DFGRreqimil
                (intn il);

    HDFLIBAPI intn DFGRgetimage
                (const char * filename, void * image, int32 xdim, int32 ydim);

    HDFLIBAPI intn DFGRsetcompress
                (int32 scheme, comp_info * cinfo);

    HDFLIBAPI intn DFGRsetlutdims
                (int32 xdim, int32 ydim, intn ncomps, intn il);

    HDFLIBAPI intn DFGRsetlut
                (void * lut, int32 xdim, int32 ydim);

    HDFLIBAPI intn DFGRaddlut
                (const char * filename, void * lut, int32 xdim, int32 ydim);

    HDFLIBAPI intn DFGRsetimdims
                (int32 xdim, int32 ydim, intn ncomps, intn il);

    HDFLIBAPI intn DFGRaddimage
                (const char * filename, void * image, int32 xdim, int32 ydim);

    HDFLIBAPI intn DFGRputimage
                (const char * filename, void * image, int32 xdim, int32 ydim);

    HDFLIBAPI intn DFGRreadref
                (const char * filename, uint16 ref);

    HDFLIBAPI uint16 DFGRIlastref
                (void);

    HDFLIBAPI intn DFGRIgetdims
                (const char * filename, int32 * pxdim, int32 * pydim,
                 intn * pncomps, intn * pil, intn type);

    HDFLIBAPI intn DFGRIreqil
                (intn il, intn type);

    HDFLIBAPI intn DFGRIgetimlut
                (const char * filename, void * imlut, int32 xdim, int32 ydim, intn type,
                 intn isfortran, int *compressed, uint16 *compr_type, int *has_pal);

    HDFLIBAPI intn DFGRIsetdims
                (int32 xdim, int32 ydim, intn ncomps, intn type);

    HDFLIBAPI intn DFGRIsetil
                (intn il, intn type);

    HDFLIBAPI intn DFGRIrestart
                (void);

    HDFLIBAPI intn DFGRIaddimlut
                (const char * filename, const void * imlut, int32 xdim, int32 ydim, intn type,
                 intn isfortran, intn newfile);

    HDFLIBAPI intn DFGRPshutdown(void);

/*
   ** from df24.c
 */
    HDFLIBAPI intn DF24getdims
                (const char * filename, int32 * pxdim, int32 * pydim,
                 intn * pil);

    HDFLIBAPI intn DF24reqil
                (intn il);

    HDFLIBAPI intn DF24getimage
                (const char * filename, void * image, int32 xdim, int32 ydim);

    HDFLIBAPI intn DF24setdims
                (int32 xdim, int32 ydim);

    HDFLIBAPI intn DF24setil
                (intn il);

    HDFLIBAPI intn DF24setcompress
                (int32 type, comp_info * cinfo);

    HDFLIBAPI intn DF24restart
                (void);

    HDFLIBAPI intn DF24addimage
                (const char * filename, const void * image, int32 xdim, int32 ydim);

    HDFLIBAPI intn DF24putimage
                (const char * filename, const void * image, int32 xdim, int32 ydim);

    HDFLIBAPI intn DF24nimages
                (const char * filename);

    HDFLIBAPI intn DF24readref
                (const char * filename, uint16 ref);

    HDFLIBAPI uint16 DF24lastref
                (void);

/*
   ** from dfan.c
 */

    HDFLIBAPI int32 DFANgetlablen
                (const char * filename, uint16 tag, uint16 ref);

    HDFLIBAPI intn DFANgetlabel
                (const char * filename, uint16 tag, uint16 ref, char * label,
                 int32 maxlen);

    HDFLIBAPI int32 DFANgetdesclen
                (const char * filename, uint16 tag, uint16 ref);

    HDFLIBAPI intn DFANgetdesc
                (const char * filename, uint16 tag, uint16 ref, char * desc,
                 int32 maxlen);

    HDFLIBAPI int32 DFANgetfidlen
                (int32 file_id, intn isfirst);

    HDFLIBAPI int32 DFANgetfid
                (int32 file_id, char * id, int32 maxlen, intn isfirst);

    HDFLIBAPI int32 DFANgetfdslen
                (int32 file_id, intn isfirst);

    HDFLIBAPI int32 DFANgetfds
                (int32 file_id, char * desc, int32 maxlen, intn isfirst);

    HDFLIBAPI intn DFANputlabel
                (const char * filename, uint16 tag, uint16 ref, char * label);

    HDFLIBAPI intn DFANputdesc
                (const char * filename, uint16 tag, uint16 ref, char * desc,
                 int32 desclen);

    HDFLIBAPI intn DFANaddfid
                (int32 file_id, char * id);

    HDFLIBAPI intn DFANaddfds
                (int32 file_id, char * desc, int32 desclen);

    HDFLIBAPI uint16 DFANlastref
                (void);

    HDFLIBAPI intn DFANlablist
                (const char * filename, uint16 tag, uint16 reflist[],
         char * labellist, intn listsize, intn maxlen, intn startpos);

    HDFLIBAPI intn DFANclear
                (void);

    HDFLIBAPI intn DFANIclear
                (void);

    HDFLIBAPI uint16 DFANIlocate
                (int32 file_id, int type, uint16 tag, uint16 ref);

    HDFLIBAPI int  DFANIaddentry
                (int type, uint16 annref, uint16 datatag, uint16 dataref);

    HDFLIBAPI int32 DFANIgetannlen
                (const char * filename, uint16 tag, uint16 ref, int type);

    HDFLIBAPI intn DFANIgetann
                (const char * filename, uint16 tag, uint16 ref, uint8 * ann,
                 int32 maxlen, int type);

    HDFLIBAPI intn DFANIputann
                (const char * filename, uint16 tag, uint16 ref, uint8 * ann,
                 int32 annlen, int type);

    HDFLIBAPI int  DFANIlablist
                (const char * filename, uint16 tag, uint16 reflist[],
            uint8 * labellist, int listsize, int maxlen, int startpos,
                 int isfortran);

    HDFLIBAPI int  DFANIaddfann
                (int32 file_id, char * ann, int32 annlen, int type);

    HDFLIBAPI int32 DFANIgetfannlen
                (int32 file_id, int type, int isfirst);

    HDFLIBAPI int32 DFANIgetfann
                (int32 file_id, char * ann, int32 maxlen, int type, int isfirst);

    HDFLIBAPI intn DFANPshutdown(void);

/*
   ** from dfsd.c
 */

    HDFLIBAPI int  DFSDgetdims
                (const char * filename, intn * prank, int32 sizes[], intn maxrank);

    HDFLIBAPI int  DFSDgetdatastrs
                (char * label, char * unit, char * format, char * coordsys);

    HDFLIBAPI int  DFSDgetdimstrs
                (int dim, char * label, char * unit, char * format);

    HDFLIBAPI int  DFSDgetdatalen
                (int * llabel, int * lunit, int * lformat, int * lcoordsys);

    HDFLIBAPI int  DFSDgetdimlen
                (int dim, int * llabel, int * lunit, int * lformat);

    HDFLIBAPI int  DFSDgetdimscale
                (intn dim, int32 maxsize, void * scale);

    HDFLIBAPI int  DFSDgetrange
                (void * pmax, void * pmin);

    HDFLIBAPI int  DFSDgetdata
                (const char * filename, intn rank, int32 maxsizes[], void * data);

    HDFLIBAPI int  DFSDsetlengths
                (int maxlen_label, int maxlen_unit, int maxlen_format,
                 int maxlen_coordsys);

    HDFLIBAPI int  DFSDsetdims
                (intn rank, int32 dimsizes[]);

    HDFLIBAPI int  DFSDsetdatastrs
                (const char * label, const char * unit, const char * format, const char * coordsys);

    HDFLIBAPI int  DFSDsetdimstrs
                (int dim, const char * label, const char * unit, const char * format);

    HDFLIBAPI int  DFSDsetdimscale
                (intn dim, int32 dimsize, void * scale);

    HDFLIBAPI int  DFSDsetrange
                (void * maxi, void * mini);

    HDFLIBAPI int  DFSDputdata
                (const char * filename, intn rank, int32 dimsizes[], void * data);

    HDFLIBAPI int  DFSDadddata
                (const char * filename, intn rank, int32 dimsizes[], void * data);

    HDFLIBAPI int  DFSDrestart
                (void);

    HDFLIBAPI int32 DFSDndatasets
                (char * filename);

    HDFLIBAPI int  DFSDclear
                (void);

    HDFLIBAPI uint16 DFSDlastref
                (void);

    HDFLIBAPI int  DFSDreadref
                (char * filename, uint16 ref);

    HDFLIBAPI int  DFSDgetslice
                (const char * filename, int32 winst[], int32 windims[], void * data,
                 int32 dims[]);

    HDFLIBAPI int  DFSDstartslice
                (const char * filename);

    HDFLIBAPI int  DFSDputslice
                (int32 winend[], void * data, int32 dims[]);

    HDFLIBAPI int  DFSDendslice
                (void);

    HDFLIBAPI int  DFSDsetNT
                (int32 numbertype);

    HDFLIBAPI int  DFSDsetorder
                (int arrayorder);

    HDFLIBAPI int  DFSDgetNT
                (int32 * pnumbertype);

    HDFLIBAPI intn DFSDpre32sdg
                (char * filename, uint16 ref, intn * ispre32);

    HDFLIBAPI int  DFSDsetcal
                (float64 cal, float64 cal_err, float64 ioff,
                 float64 ioff_err, int32 cal_nt);

    HDFLIBAPI int  DFSDgetcal
                (float64 * pcal, float64 * pcal_err, float64 * pioff,
                 float64 * pioff_err, int32 * cal_nt);

    HDFLIBAPI int  DFSDwriteref
                (const char * filename, uint16 ref);

    HDFLIBAPI int  DFSDsetfillvalue
                (void * fill_value);

    HDFLIBAPI int  DFSDgetfillvalue
                (void * fill_value);

    HDFLIBAPI int  DFSDstartslab
                (const char * filename);

    HDFLIBAPI int  DFSDwriteslab
                (int32 start[], int32 stride[], int32 count[],
                 void * data);

    HDFLIBAPI int  DFSDendslab
                (void);

    HDFLIBAPI int  DFSDreadslab
                (const char *filename, int32 start[], int32 slab_size[],
             int32 stride[], void * buffer, int32 buffer_size[]);

    HDFLIBAPI intn DFSDPshutdown(void);

/*
   ** from dfconv.c
 */

    HDFLIBAPI int  DFKNTsize
                (int32 number_type);

    HDFLIBAPI int32 DFKisnativeNT
                (int32 numbertype);

    HDFLIBAPI int32 DFKislitendNT
                (int32 numbertype);

    HDFLIBAPI int8 DFKgetPNSC
                (int32 numbertype, int32 machinetype);

    HDFLIBAPI intn DFKsetNT
                (int32 ntype);

    HDFLIBAPI int32 DFKconvert
                (void * source, void * dest, int32 ntype, int32 num_elm,
                 int16 acc_mode, int32 source_stride, int32 dest_stride);

/*
   ** from dfknat.c
 */

    HDFLIBAPI intn DFKnb1b
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKnb2b
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKnb4b
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKnb8b
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

/*
   ** from dfkswap.c
 */

    HDFLIBAPI intn DFKsb2b
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKsb4b
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKsb8b
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

/*
   ** from dfkcray.c
 */

    HDFLIBAPI intn DFKui2i
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKui2s
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKuo2i
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKuo2s
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKui4i
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKui4s
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKuo4i
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKuo4s
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKui4f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKuo4f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKui8f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKuo8f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKlui2i
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKlui2s
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKluo2i
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKluo2s
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKlui4i
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKlui4s
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKluo4i
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKluo4s
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKlui4f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKluo4f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKlui8f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKluo8f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

/* CRAY-MPP */
    HDFLIBAPI intn DFKmi2i
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKmi2s
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKmo2b
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKlmi2i
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKlmi2s
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKlmo2b
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);


/*
   ** from dfkvms.c
 */

    HDFLIBAPI intn DFKvi4f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKvo4f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKvi8f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKvo8f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKlvi4f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKlvo4f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKlvi8f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKlvo8f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

/*
   ** from dfkconv.c
 */

    HDFLIBAPI intn DFKci4f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKco4f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKci8f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKco8f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKlci4f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKlco4f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKlci8f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKlco8f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

/*
   ** from dfkfuji.c
 */

    HDFLIBAPI intn DFKpi4f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKpo4f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKpi8f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKpo8f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKlpi4f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKlpo4f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKlpi8f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

    HDFLIBAPI intn DFKlpo8f
                (void * s, void * d, uint32 num_elm, uint32 source_stride, uint32 dest_stride);

/*
   ** from dfanF.c
 */
#ifndef DFAN_FNAMES
#   define  DFAN_FNAMES
#ifdef DF_CAPFNAMES
#   define ndaiganl  FNAME(DAIGANL)
#   define ndaigann  FNAME(DAIGANN)
#   define ndaipann  FNAME(DAIPANN)
#   define ndailist  FNAME(DAILIST)
#   define ndalref   FNAME(DALREF)
#   define ndaclear  FNAME(DACLEAR)
#   define ndfanlastref     FNAME(DFANLASTREF)

#   define ndfanaddfds      FNAME(DFANADDFDS)
#   define ndfangetfidlen   FNAME(DFANGETFIDLEN)
#   define ndfangetfdslen   FNAME(DFANGETFDSLEN)
#   define ndfangetfid      FNAME(DFANGETFID)
#   define ndfangetfds      FNAME(DFANGETFDS)
#   define ndaafds          FNAME(DAAFDS)
#   define ndagfidl         FNAME(DAGFIDL)
#   define ndagfdsl         FNAME(DAGFDSL)
#   define ndagfid          FNAME(DAGFID)
#   define ndagfds          FNAME(DAGFDS)
#   define ndaiafid         FNAME(DAIAFID)
#else                           /* DF_CAPFNAMES */
#   define ndaiganl  FNAME(daiganl)
#   define ndaigann  FNAME(daigann)
#   define ndaipann  FNAME(daipann)
#   define ndailist  FNAME(dailist)
#   define ndalref   FNAME(dalref)
#   define ndaclear  FNAME(daclear)
#   define ndfanlastref     FNAME(dfanlastref)

#   define ndfanaddfds      FNAME(dfanaddfds)
#   define ndfangetfidlen   FNAME(dfangetfidlen)
#   define ndfangetfdslen   FNAME(dfangetfdslen)
#   define ndfangetfid      FNAME(dfangetfid)
#   define ndfangetfds      FNAME(dfangetfds)
#   define ndaafds          FNAME(daafds)
#   define ndagfidl         FNAME(dagfidl)
#   define ndagfdsl         FNAME(dagfdsl)
#   define ndagfid          FNAME(dagfid)
#   define ndagfds          FNAME(dagfds)
#   define ndaiafid         FNAME(daiafid)
#endif                          /* DF_CAPFNAMES */
#endif                          /* DFAN_FNAMES */

    HDFLIBAPI      FRETVAL(intf) ndaiganl
                (_fcd filename, intf * tag, intf * ref, intf * type,
                 intf * fnlen);

    HDFLIBAPI      FRETVAL(intf) ndaigann
                (_fcd filename, intf * tag, intf * ref, _fcd annotation,
                 intf * maxlen, intf * type, intf * fnlen);

    HDFLIBAPI      FRETVAL(intf) ndaipann
                (_fcd filename, intf * tag, intf * ref, _fcd annotation,
                 intf * annlen, intf * type, intf * fnlen);

    HDFLIBAPI      FRETVAL(intf) ndailist
                (_fcd filename, intf * tag, intf reflist[], _fcd labellist,
          intf * listsize, intf * maxlen, intf * startpos,
                 intf * fnlen);

    HDFLIBAPI      FRETVAL(intf) ndalref
                (void);

    HDFLIBAPI      FRETVAL(intf) ndaclear
                (void);

    HDFLIBAPI      FRETVAL(intf) ndfanlastref
                (void);

    HDFLIBAPI      FRETVAL(intf) ndfanaddfds
                (intf * dfile, _fcd desc, intf * desclen);

    HDFLIBAPI      FRETVAL(intf) ndfangetfidlen
                (intf * dfile, intf * isfirst);

    HDFLIBAPI      FRETVAL(intf) ndfangetfdslen
                (intf * dfile, intf * isfirst);

    HDFLIBAPI      FRETVAL(intf) ndfangetfid
                (intf * dfile, _fcd id, intf * maxlen, intf * isfirst);

    HDFLIBAPI      FRETVAL(intf) ndfangetfds
                (intf * dfile, _fcd id, intf * maxlen, intf * isfirst);

    HDFLIBAPI      FRETVAL(intf) ndaafds
                (intf * dfile, _fcd desc, intf * desclen);

    HDFLIBAPI      FRETVAL(intf) ndagfidl
                (intf * dfile, intf * isfirst);

    HDFLIBAPI      FRETVAL(intf) ndagfdsl
                (intf * dfile, intf * isfirst);

    HDFLIBAPI      FRETVAL(intf) ndagfid
                (intf * dfile, _fcd id, intf * maxlen, intf * isfirst);

    HDFLIBAPI      FRETVAL(intf) ndagfds
                (intf * dfile, _fcd id, intf * maxlen, intf * isfirst);

    HDFLIBAPI      FRETVAL(intf) ndaiafid
                (intf * dfile, _fcd id, intf * idlen);

/*
   ** from dfr8F.c
 */
#ifndef DFR8_FNAMES
#   define DFR8_FNAMES
#ifdef DF_CAPFNAMES
#   define nd8spal   FNAME(D8SPAL)
#   define nd8first  FNAME(D8FIRST)
#   define nd8igdim  FNAME(D8IGDIM)
#   define nd8igimg  FNAME(D8IGIMG)
#   define nd8ipimg  FNAME(D8IPIMG)
#   define nd8iaimg  FNAME(D8IAIMG)
#   define nd8irref  FNAME(D8IRREF)
#   define nd8iwref  FNAME(D8IWREF)
#   define nd8inims  FNAME(D8INIMS)
#   define nd8lref   FNAME(D8LREF)
#   define ndfr8lastref      FNAME(DFR8LASTREF)
#   define ndfr8setpalette   FNAME(DFR8SETPALETTE)
#   define ndfr8restart  FNAME(DFR8RESTART)
#   define nd8scomp  FNAME(D8SCOMP)
#   define ndfr8scompress FNAME(DFR8SCOMPRESS)
#   define nd8sjpeg  FNAME(D8SJPEG)
#   define ndfr8sjpeg FNAME(DFR8SJPEG)
#else                           /* !DF_CAPFNAMES */
#   define nd8spal   FNAME(d8spal)
#   define nd8first  FNAME(d8first)
#   define nd8igdim  FNAME(d8igdim)
#   define nd8igimg  FNAME(d8igimg)
#   define nd8ipimg  FNAME(d8ipimg)
#   define nd8iaimg  FNAME(d8iaimg)
#   define nd8irref  FNAME(d8irref)
#   define nd8iwref  FNAME(d8iwref)
#   define nd8inims  FNAME(d8inims)
#   define nd8lref   FNAME(d8lref)
#   define ndfr8lastref      FNAME(dfr8lastref)
#   define ndfr8setpalette   FNAME(dfr8setpalette)
#   define ndfr8restart  FNAME(dfr8restart)
#   define nd8scomp  FNAME(d8scomp)
#   define ndfr8scompress FNAME(dfr8scompress)
#   define nd8sjpeg  FNAME(d8sjpeg)
#   define ndfr8sjpeg FNAME(dfr8sjpeg)
#endif                          /* DF_CAPFNAMES */
#endif                          /* DFR8_FNAMES */

    HDFLIBAPI      FRETVAL(intf) nd8spal
                (_fcd pal);

    HDFLIBAPI      FRETVAL(intf) nd8first
                (void);

    HDFLIBAPI      FRETVAL(intf) nd8igdim
                (_fcd filename, intf * xdim, intf * ydim, intf * ispal,
                 intf * lenfn);

    HDFLIBAPI      FRETVAL(intf) nd8igimg
                (_fcd filename, _fcd image, intf * xdim, intf * ydim,
                 _fcd pal, intf * lenfn);

    HDFLIBAPI      FRETVAL(intf) nd8ipimg
                (_fcd filename, _fcd image, intf * xdim, intf * ydim,
                 intf * compress, intf * lenfn);

    HDFLIBAPI      FRETVAL(intf) nd8iaimg
                (_fcd filename, _fcd image, intf * xdim, intf * ydim,
                 intf * compress, intf * lenfn);

    HDFLIBAPI      FRETVAL(intf) nd8irref
                (_fcd filename, intf * ref, intf * fnlen);

    HDFLIBAPI      FRETVAL(intf) nd8iwref
                (_fcd filename, intf * ref, intf * fnlen);

    HDFLIBAPI      FRETVAL(intf) nd8inims
                (_fcd filename, intf * fnlen);

    HDFLIBAPI      FRETVAL(intf) nd8lref
                (void);

    HDFLIBAPI      FRETVAL(intf) ndfr8lastref
                (void);

    HDFLIBAPI      FRETVAL(intf) ndfr8setpalette
                (_fcd pal);

    HDFLIBAPI      FRETVAL(intf) ndfr8restart
                (void);

    HDFLIBAPI      FRETVAL(intf) nd8scomp
                (intf * scheme);

    HDFLIBAPI      FRETVAL(intf) ndfr8scompress
                (intf * scheme);

    HDFLIBAPI      FRETVAL(intf) nd8sjpeg
                (intf * quality, intf * force_baseline);

    HDFLIBAPI      FRETVAL(intf) ndfr8sjpeg
                (intf * quality, intf * force_baseline);

/*
   ** from dfsdF.c
 */
#ifndef DFSD_FNAMES
#   define DFSD_FNAMES
#ifdef DF_CAPFNAMES
#   define ndsgdast  FNAME(DSGDAST)
#   define ndsgdisc  FNAME(DSGDISC)
#   define ndsgrang  FNAME(DSGRANG)
#   define ndssdims  FNAME(DSSDIMS)
#   define ndssdisc  FNAME(DSSDISC)
#   define ndssrang  FNAME(DSSRANG)
#   define ndsclear  FNAME(DSCLEAR)
#   define ndsslens  FNAME(DSSLENS)
#   define ndsgdiln  FNAME(DSGDILN)
#   define ndsgdaln  FNAME(DSGDALN)
#   define ndsfirst  FNAME(DSFIRST)
#   define ndspslc   FNAME(DSPSLC)
#   define ndseslc   FNAME(DSESLC)
#   define ndsgnt    FNAME(DSGNT)
#   define ndssnt    FNAME(DSSNT)
#   define ndsigdim  FNAME(DSIGDIM)
#   define ndsigdat  FNAME(DSIGDAT)
#   define ndsipdat  FNAME(DSIPDAT)
#   define ndsiadat  FNAME(DSIADAT)
#   define ndsigdas  FNAME(DSIGDAS)
#   define ndsigslc  FNAME(DSIGSLC)
#   define ndsigdis  FNAME(DSIGDIS)
#   define ndsisslc  FNAME(DSISSLC)
#   define ndsisdas  FNAME(DSISDAS)
#   define ndsisdis  FNAME(DSISDIS)
#   define ndsirref  FNAME(DSIRREF)
#   define ndslref   FNAME(DSLREF)
#   define ndsinum   FNAME(DSINUM)
#   define ndsip32s  FNAME(DSIP32S)
#   define ndsscal   FNAME(DSSCAL)
#   define ndsgcal   FNAME(DSGCAL)
#   define ndfsdgetdatastrs  FNAME(DFSDGETDATASTRS)
#   define ndfsdgetdimscale  FNAME(DFSDGETDIMSCALE)
#   define ndfsdgetrange     FNAME(DFSDGETRANGE)
#   define ndfsdsetdims      FNAME(DFSDSETDIMS)
#   define ndfsdsetdimscale  FNAME(DFSDSETDIMSCALE)
#   define ndfsdsetrange     FNAME(DFSDSETRANGE)
#   define ndfsdclear        FNAME(DFSDCLEAR)
#   define ndfsdsetlengths   FNAME(DFSDSETLENGTHS)
#   define ndfsdgetdimlen    FNAME(DFSDGETDIMLEN)
#   define ndfsdgetdatalen   FNAME(DFSDGETDATALEN)
#   define ndfsdrestart      FNAME(DFSDRESTART)
#   define ndfsdputslice     FNAME(DFSDPUTSLICE)
#   define ndfsdendslice     FNAME(DFSDENDSLICE)
#   define ndfsdsetnt        FNAME(DFSDSETNT)
#   define ndfsdgetnt        FNAME(DFSDGETNT)
#   define ndfsdlastref      FNAME(DFSDLASTREF)
#   define ndsiwref          FNAME(DSIWREF)
#   define ndssfill          FNAME(DSSFILL)
#   define ndsgfill          FNAME(DSGFILL)
#   define ndsisslab         FNAME(DSISSLAB)
#   define ndswslab          FNAME(DSWSLAB)
#   define ndseslab          FNAME(DSESLAB)
#   define ndsirslab         FNAME(DSIRSLAB)
#else
#   define ndsgdast  FNAME(dsgdast)
#   define ndsgdisc  FNAME(dsgdisc)
#   define ndsgrang  FNAME(dsgrang)
#   define ndssdims  FNAME(dssdims)
#   define ndssdisc  FNAME(dssdisc)
#   define ndssrang  FNAME(dssrang)
#   define ndsclear  FNAME(dsclear)
#   define ndsslens  FNAME(dsslens)
#   define ndsgdiln  FNAME(dsgdiln)
#   define ndsgdaln  FNAME(dsgdaln)
#   define ndsfirst  FNAME(dsfirst)
#   define ndspslc   FNAME(dspslc)
#   define ndseslc   FNAME(dseslc)
#   define ndsgnt    FNAME(dsgnt)
#   define ndssnt    FNAME(dssnt)
#   define ndsigdim  FNAME(dsigdim)
#   define ndsigdat  FNAME(dsigdat)
#   define ndsipdat  FNAME(dsipdat)
#   define ndsiadat  FNAME(dsiadat)
#   define ndsigdas  FNAME(dsigdas)
#   define ndsigslc  FNAME(dsigslc)
#   define ndsigdis  FNAME(dsigdis)
#   define ndsisslc  FNAME(dsisslc)
#   define ndsisdas  FNAME(dsisdas)
#   define ndsisdis  FNAME(dsisdis)
#   define ndsirref  FNAME(dsirref)
#   define ndslref   FNAME(dslref)
#   define ndsinum   FNAME(dsinum)
#   define ndsip32s  FNAME(dsip32s)
#   define ndsscal   FNAME(dsscal)
#   define ndsgcal   FNAME(dsgcal)
#   define ndfsdgetdatastrs  FNAME(dfsdgetdatastrs)
#   define ndfsdgetdimscale  FNAME(dfsdgetdimscale)
#   define ndfsdgetrange     FNAME(dfsdgetrange)
#   define ndfsdsetdims      FNAME(dfsdsetdims)
#   define ndfsdsetdimscale  FNAME(dfsdsetdimscale)
#   define ndfsdsetrange     FNAME(dfsdsetrange)
#   define ndfsdclear        FNAME(dfsdclear)
#   define ndfsdsetlengths   FNAME(dfsdsetlengths)
#   define ndfsdgetdimlen    FNAME(dfsdgetdimlen)
#   define ndfsdgetdatalen   FNAME(dfsdgetdatalen)
#   define ndfsdrestart      FNAME(dfsdrestart)
#   define ndfsdputslice     FNAME(dfsdputslice)
#   define ndfsdendslice     FNAME(dfsdendslice)
#   define ndfsdsetnt        FNAME(dfsdsetnt)
#   define ndfsdgetnt        FNAME(dfsdgetnt)
#   define ndfsdlastref      FNAME(dfsdlastref)
#   define ndsiwref          FNAME(dsiwref)
#   define ndssfill          FNAME(dssfill)
#   define ndsgfill          FNAME(dsgfill)
#   define ndsisslab         FNAME(dsisslab)
#   define ndswslab          FNAME(dswslab)
#   define ndseslab          FNAME(dseslab)
#   define ndsirslab         FNAME(dsirslab)
#endif                          /* DF_CAPFNAMES */
#endif                          /* DFSD_FNAMES */

    HDFLIBAPI      FRETVAL(intf) ndsgdisc
                (intf * dim, intf * maxsize, void * scale);

    HDFLIBAPI      FRETVAL(intf) ndsgrang
                (void * pmax, void * pmin);

    HDFLIBAPI      FRETVAL(intf) ndssdims
                (intf * rank, intf dimsizes[]);

    HDFLIBAPI      FRETVAL(intf) ndssdisc
                (intf * dim, intf * dimsize, void * scale);

    HDFLIBAPI      FRETVAL(intf) ndssrang
                (void * max, void * min);

    HDFLIBAPI      FRETVAL(intf) ndsclear
                (void);

    HDFLIBAPI      FRETVAL(intf) ndsslens
                (intf * maxlen_label, intf * maxlen_unit,
                 intf * maxlen_format, intf * maxlen_coordsys);

    HDFLIBAPI      FRETVAL(intf) ndsgdiln
                (intf * dim, intf * llabel, intf * lunit,
                 intf * lformat);

    HDFLIBAPI      FRETVAL(intf) ndsgdaln
                (intf * llabel, intf * lunit, intf * lformat,
                 intf * lcoordsys);

    HDFLIBAPI      FRETVAL(intf) ndsfirst
                (void);

    HDFLIBAPI      FRETVAL(intf) ndspslc
                (intf windims[], void * data, intf dims[]);

    HDFLIBAPI      FRETVAL(intf) ndseslc
                (void);

    HDFLIBAPI      FRETVAL(intf) ndssnt
                (intf * numbertype);

    HDFLIBAPI      FRETVAL(intf) ndsgnt
                (intf * pnumbertype);

    HDFLIBAPI      FRETVAL(intf) ndsigdim
                (_fcd filename, intf * prank, intf sizes[],
                 intf * maxrank, intf * lenfn);

    HDFLIBAPI      FRETVAL(intf) ndsigdat
                (_fcd filename, intf * rank, intf maxsizes[],
                 void * data, intf * fnlen);

    HDFLIBAPI      FRETVAL(intf) ndsipdat
                (_fcd filename, intf * rank, intf dimsizes[],
                 void * data, intf * fnlen);

    HDFLIBAPI      FRETVAL(intf) ndsiadat
                (_fcd filename, intf * rank, intf dimsizes[],
                 void * data, intf * fnlen);

    HDFLIBAPI      FRETVAL(intf) ndsigslc
                (_fcd filename, intf winst[], intf windims[],
                 void * data, intf dims[], intf * fnlen);

    HDFLIBAPI      FRETVAL(intf) ndsisslc
                (_fcd filename, intf * fnlen);

    HDFLIBAPI      FRETVAL(intf) ndsirref
                (_fcd filename, intf * ref, intf * fnlen);

    HDFLIBAPI      FRETVAL(intf) ndslref
                (void);

    HDFLIBAPI      FRETVAL(intf) ndsinum
                (_fcd filename, intf * len);

    HDFLIBAPI      FRETVAL(intf) ndsip32s
                (_fcd filename, intf * ref, intf * ispre32, intf * len);

    HDFLIBAPI      FRETVAL(intf) ndfsdgetdatastrs
                (_fcd label, _fcd unit, _fcd format, _fcd coordsys);

    HDFLIBAPI      FRETVAL(intf) ndfsdgetdimstrs
                (intf * dim, _fcd label, _fcd unit, _fcd format);

    HDFLIBAPI      FRETVAL(intf) ndfsdgetdimscale
                (intf * dim, intf * maxsize, void * scale);

    HDFLIBAPI      FRETVAL(intf) ndfsdgetrange
                (void * pmax, void * pmin);

    HDFLIBAPI      FRETVAL(intf) ndfsdsetdims
                (intf * rank, intf dimsizes[]);

    HDFLIBAPI      FRETVAL(intf) ndfsdsetdimscale
                (intf * dim, intf * dimsize, void * scale);

    HDFLIBAPI      FRETVAL(intf) ndfsdsetrange
                (void * max, void * min);

    HDFLIBAPI      FRETVAL(intf) ndfsdclear
                (void);

    HDFLIBAPI      FRETVAL(intf) ndfsdsetlengths
                (intf * maxlen_label, intf * maxlen_unit,
                 intf * maxlen_format, intf * maxlen_coordsys);

    HDFLIBAPI      FRETVAL(intf) ndfsdgetdimlen
                (intf * dim, intf * llabel, intf * lunit,
                 intf * lformat);

    HDFLIBAPI      FRETVAL(intf) ndfsdgetdatalen
                (intf * llabel, intf * lunit, intf * lformat,
                 intf * lcoordsys);

    HDFLIBAPI      FRETVAL(intf) ndfsdrestart
                (void);

    HDFLIBAPI      FRETVAL(intf) ndfsdputslice
                (intf windims[], void * data, intf dims[]);

    HDFLIBAPI      FRETVAL(intf) ndfsdendslice
                (void);

    HDFLIBAPI      FRETVAL(intf) ndfsdsetnt
                (intf * numbertype);

    HDFLIBAPI      FRETVAL(intf) ndfsdgetnt
                (intf * pnumbertype);

    HDFLIBAPI      FRETVAL(intf) ndfsdlastref
                (void);

    HDFLIBAPI      FRETVAL(intf) ndsisdis
                (intf * dim, _fcd flabel, _fcd funit, _fcd fformat,
             intf * llabel, intf * lunit, intf * lformat);

    HDFLIBAPI      FRETVAL(intf) ndsigdis
                (intf * dim, _fcd label, _fcd unit, _fcd format,
             intf * llabel, intf * lunit, intf * lformat);

    HDFLIBAPI      FRETVAL(intf) ndsisdas
                (_fcd flabel, _fcd funit, _fcd fformat, _fcd fcoordsys,
            intf * isfortran, intf * llabel, intf * lunit,
                 intf * lformat, intf * lcoordsys);

    HDFLIBAPI      FRETVAL(intf) ndsigdas
                (_fcd label, _fcd unit, _fcd format, _fcd coordsys, intf * llabel,
             intf * lunit, intf * lformat, intf * lcoord);

    HDFLIBAPI      FRETVAL(intf) ndsscal
                (float64 * cal, float64 * cal_err, float64 * ioff,
                 float64 * ioff_err, intf * cal_type);

    HDFLIBAPI      FRETVAL(intf) ndsgcal
                (float64 * cal, float64 * cal_err, float64 * ioff,
                 float64 * ioff_err, intf * cal_type);

    HDFLIBAPI      FRETVAL(intf) ndswref
                (_fcd filename, intf * fnlen, intf * ref);

    HDFLIBAPI      FRETVAL(intf) ndssfill
                (void * fill_value);

    HDFLIBAPI      FRETVAL(intf) ndsgfill
                (void * fill_value);

    HDFLIBAPI      FRETVAL(intf) ndssslab
                (_fcd filename, intf * fnlen);

    HDFLIBAPI      FRETVAL(intf) ndswslab
                (intf start[], intf  stride[],
                 intf  cont[], void * data);

    HDFLIBAPI      FRETVAL(intf) ndseslab
                (void);

    HDFLIBAPI	FRETVAL(intf) ndsiwref
		(_fcd filename, intf * fnlen, intf * ref);

    HDFLIBAPI	FRETVAL(intf) ndsisslab
		(_fcd filename, intf * fnlen);

    HDFLIBAPI	FRETVAL(intf) ndsirslab
		(_fcd filename, intf * fnlen, intf start[], intf slab_size[],
	         intf stride[], void * buffer, intf buffer_size[]);

/*
   ** from dfpF.c
 */

#ifndef DFP_FNAMES
#   define DFP_FNAMES
#ifdef DF_CAPFNAMES
#   define ndpigpal  FNAME(DPIGPAL)
#   define ndpippal  FNAME(DPIPPAL)
#   define ndpinpal  FNAME(DPINPAL)
#   define ndpiwref  FNAME(DPIWREF)
#   define ndpirref  FNAME(DPIRREF)
#   define ndprest   FNAME(DPREST)
#   define ndplref   FNAME(DPLREF)
#   define ndfprestart   FNAME(DFPRESTART)
#   define ndfplastref   FNAME(DFPLASTREF)
#else                           /* !DF_CAPNAMES */
#   define ndpigpal  FNAME(dpigpal)
#   define ndpippal  FNAME(dpippal)
#   define ndpinpal  FNAME(dpinpal)
#   define ndpiwref  FNAME(dpiwref)
#   define ndpirref  FNAME(dpirref)
#   define ndprest   FNAME(dprest)
#   define ndplref   FNAME(dplref)
#   define ndfprestart   FNAME(dfprestart)
#   define ndfplastref   FNAME(dfplastref)
#endif                          /* DF_CAPFNAMES */
#endif                          /* DFP_FNAMES */

    HDFLIBAPI      FRETVAL(intf) ndpigpal
                (_fcd filename, _fcd pal, intf  * fnlen);

    HDFLIBAPI      FRETVAL(intf) ndpippal
                (_fcd filename, _fcd pal, intf  * overwrite, _fcd filemode,
                 intf  * fnlen);

    HDFLIBAPI      FRETVAL(intf) ndpinpal
                (_fcd filename, intf  * fnlen);

    HDFLIBAPI      FRETVAL(intf) ndpirref
                (_fcd filename, intf  * ref, intf  * fnlen);

    HDFLIBAPI      FRETVAL(intf) ndpiwref
                (_fcd filename, intf  * ref, intf  * fnlen);

    HDFLIBAPI      FRETVAL(intf) ndprest
                (void);

    HDFLIBAPI      FRETVAL(intf) ndplref
                (void);

    HDFLIBAPI      FRETVAL(intf) ndfprestart
                (void);

    HDFLIBAPI      FRETVAL(intf) ndfplastref
                (void);

/*
   ** from df24F.c
 */
#ifndef DF24_FNAMES
#   define DF24_FNAMES
#ifdef DF_CAPFNAMES
#   define nd2reqil  FNAME(D2REQIL)
#   define ndf24reqil    FNAME(DF24REQIL)
#   define nd2sdims  FNAME(D2SDIMS)
#   define ndf24setdims  FNAME(DF24SETDIMS)
#   define nd2setil  FNAME(D2SETIL)
#   define ndf24setil    FNAME(DF24SETIL)
#   define nd2first  FNAME(D2FIRST)
#   define ndf24restart  FNAME(DF24RESTART)
#   define nd2igdim  FNAME(D2IGDIM)
#   define nd2igimg  FNAME(D2IGIMG)
#   define nd2iaimg  FNAME(D2IAIMG)
#   define nd2irref  FNAME(D2IRREF)
#   define nd2inimg  FNAME(D2INIMG)
#   define nd2lref   FNAME(D2LREF)
#   define nd2scomp  FNAME(D2SCOMP)
#   define ndf24scompress FNAME(DF24SCOMPRESS)
#   define nd2sjpeg  FNAME(D2SJPEG)
#   define ndf24sjpeg FNAME(DF24SJPEG)
#else
#   define nd2reqil  FNAME(d2reqil)
#   define ndf24reqil    FNAME(df24reqil)
#   define nd2sdims  FNAME(d2sdims)
#   define ndf24setdims  FNAME(df24setdims)
#   define nd2setil  FNAME(d2setil)
#   define ndf24setil    FNAME(df24setil)
#   define nd2first  FNAME(d2first)
#   define ndf24restart  FNAME(df24restart)
#   define nd2igdim  FNAME(d2igdim)
#   define nd2igimg  FNAME(d2igimg)
#   define nd2iaimg  FNAME(d2iaimg)
#   define nd2irref  FNAME(d2irref)
#   define nd2inimg  FNAME(d2inimg)
#   define nd2lref   FNAME(d2lref)
#   define nd2scomp  FNAME(d2scomp)
#   define ndf24scompress FNAME(df24scompress)
#   define nd2sjpeg  FNAME(d2sjpeg)
#   define ndf24sjpeg FNAME(df24sjpeg)
#endif                          /* DF_CAPFNAMES */
#endif                          /* DF24_FNAMES */

    HDFLIBAPI      FRETVAL(intf) nd2reqil
                (intf  * il);

    HDFLIBAPI      FRETVAL(intf) nd2sdims
                (intf  * xdim, intf  * ydim);

    HDFLIBAPI      FRETVAL(intf) nd2igdim
                (_fcd filename, intf  * pxdim, intf  * pydim, intf  * pil,
                 intf  * fnlen);

    HDFLIBAPI      FRETVAL(intf) nd2igimg
                (_fcd filename, _fcd image, intf  * xdim, intf  * ydim,
                 intf  * fnlen);

    HDFLIBAPI      FRETVAL(intf) nd2iaimg
                (_fcd filename, _fcd image, intf  * xdim, intf  * ydim,
                 intf  * fnlen, intf  * newfile);

    HDFLIBAPI      FRETVAL(intf) nd2setil
                (intf  * il);

    HDFLIBAPI      FRETVAL(intf) nd2first
                (void);

    HDFLIBAPI      FRETVAL(intf) ndf24reqil
                (intf  * il);

    HDFLIBAPI      FRETVAL(intf) ndf24setdims
                (intf  * xdim, intf  * ydim);

    HDFLIBAPI      FRETVAL(intf) ndf24setil
                (intf  * il);

    HDFLIBAPI      FRETVAL(intf) ndf24restart
                (void);

    HDFLIBAPI      FRETVAL(intf) nd2irref
                (_fcd filename, intf  * ref, intf  * fnlen);

    HDFLIBAPI      FRETVAL(intf) nd2inimg
                (_fcd filename, intf  * fnlen);

    HDFLIBAPI      FRETVAL(intf) nd2lref
                (void);

    HDFLIBAPI      FRETVAL(intf) nd2scomp
                (intf * scheme);

    HDFLIBAPI      FRETVAL(intf) ndf24scompress
                (intf * scheme);

    HDFLIBAPI      FRETVAL(intf) nd2sjpeg
                (intf * quality, intf * force_baseline);

    HDFLIBAPI      FRETVAL(intf) ndf24sjpeg
                (intf * quality, intf * force_baseline);

/*
   ** from dfF.c
 */
#ifndef DF_FNAMES
#   define DF_FNAMES
#ifdef DF_CAPFNAMES
#   define ndfiaccess FNAME(DFIACCESS)
#   define ndfiopen  FNAME(DFIOPEN)
#   define ndfclose  FNAME(DFCLOSE)
#   define ndfdesc   FNAME(DFDESC)
#   define ndfdup    FNAME(DFDUP)
#   define ndfdel    FNAME(DFDEL)
#   define ndfstart  FNAME(DFSTART)
#   define ndfread   FNAME(DFREAD)
#   define ndfseek   FNAME(DFSEEK)
#   define ndfwrite  FNAME(DFWRITE)
#   define ndfupdate FNAME(DFUPDATE)
#   define ndfget    FNAME(DFGET)
#   define ndfput    FNAME(DFPUT)
#   define ndfsfind  FNAME(DFSFIND)
#   define ndffind   FNAME(DFFIND)
#   define ndferrno  FNAME(DFERRNO)
#   define ndfnewref FNAME(DFNEWREF)
#   define ndfnumber FNAME(DFNUMBER)
#   define ndfstat   FNAME(DFSTAT)
#   define ndfiishdf FNAME(DFIISHDF)
#else                           /* !DF_CAPFNAMES */
#   define ndfiaccess FNAME(dfiaccess)
#   define ndfiopen  FNAME(dfiopen)
#   define ndfclose  FNAME(dfclose)
#   define ndfdesc   FNAME(dfdesc)
#   define ndfdup    FNAME(dfdup)
#   define ndfdel    FNAME(dfdel)
#   define ndfstart  FNAME(dfstart)
#   define ndfread   FNAME(dfread)
#   define ndfseek   FNAME(dfseek)
#   define ndfwrite  FNAME(dfwrite)
#   define ndfupdate FNAME(dfupdate)
#   define ndfget    FNAME(dfget)
#   define ndfput    FNAME(dfput)
#   define ndfsfind  FNAME(dfsfind)
#   define ndffind   FNAME(dffind)
#   define ndferrno  FNAME(dferrno)
#   define ndfnewref FNAME(dfnewref)
#   define ndfnumber FNAME(dfnumber)
#   define ndfstat   FNAME(dfstat)
#   define ndfiishdf FNAME(dfiishdf)
#endif                          /* DF_CAPFNAMES */
#endif                          /* DF_FNAMES */

    HDFLIBAPI      FRETVAL(intf) ndfiopen
                (_fcd name, intf  * acc_mode, intf  * defdds, intf  * namelen);

    HDFLIBAPI      FRETVAL(intf) ndfclose
                (intf  * dfile);

    HDFLIBAPI      FRETVAL(intf) ndfdesc
                (intf  * dfile, intf  ptr[][4], intf  * begin,
                 intf  * num);

    HDFLIBAPI      FRETVAL(intf) ndfdup
                (intf  * dfile, intf  * tag, intf  * ref, intf  * otag,
                 intf  * oref);

    HDFLIBAPI      FRETVAL(intf) ndfdel
                (intf  * dfile, intf  * tag, intf  * ref);

    HDFLIBAPI      FRETVAL(intf) ndfiaccess
                (intf  * dfile, intf  * tag, intf  * ref, _fcd acc_mode, intf  * acclen);

    HDFLIBAPI      FRETVAL(intf) ndfstart
                (intf  * dfile, intf  * tag, intf  * ref, char  * acc_mode);

    HDFLIBAPI      FRETVAL(intf) ndfread
                (intf  * dfile, _fcd ptr, intf  * len);

    HDFLIBAPI      FRETVAL(intf) ndfseek
                (intf  * dfile, intf  * offset);

    HDFLIBAPI      FRETVAL(intf) ndfwrite
                (intf  * dfile, _fcd ptr, intf  * len);

    HDFLIBAPI      FRETVAL(intf) ndfupdate
                (intf  * dfile);

    HDFLIBAPI      FRETVAL(intf) ndfget
                (intf  * dfile, intf  * tag, intf  * ref, _fcd ptr);

    HDFLIBAPI      FRETVAL(intf) ndfput
                (intf  * dfile, intf  * tag, intf  * ref, _fcd ptr, intf  * len);

    HDFLIBAPI      FRETVAL(intf) ndfsfind
                (intf  * dfile, intf  * tag, intf  * ref);

    HDFLIBAPI      FRETVAL(intf) ndffind
                (intf  * dfile, intf  * itag, intf  * iref, intf  * len);

    HDFLIBAPI      FRETVAL(intf) ndferrno
                (void);

    HDFLIBAPI      FRETVAL(intf) ndfnewref
                (intf  * dfile);

    HDFLIBAPI      FRETVAL(intf) ndfnumber
                (intf  * dfile, intf  * tag);

    HDFLIBAPI      FRETVAL(intf) ndfiishdf
                (_fcd name, intf  * namelen);

/*
   ** from dfutil.c
 */
    HDFLIBAPI uint16 DFfindnextref
                (int32 file_id, uint16 tag, uint16 lref);

/*
   ** from dfutilF.c
 */
#ifndef DFUTIL_FNAMES
#   define DFUTIL_FNAMES
#ifdef DF_CAPFNAMES
#   define ndfindnr          FNAME(DFINDNR)
#   define ndffindnextref    FNAME(DFFINDNEXTREF)
#else
#   define ndfindnr          FNAME(dfindnr)
#   define ndffindnextref    FNAME(dffindnextref)
#endif                          /* DF_CAPFNAMES */
#endif                          /* DFUTIL_FNAMES */

    HDFLIBAPI      FRETVAL(intf) ndfindnr
                (intf  * dfile, intf  * tag, intf  * lref);

    HDFLIBAPI      FRETVAL(intf) ndffindnextref
                (intf  * dfile, intf  * tag, intf  * lref);

/*
   ** from herrF.c
 */
#ifndef HERR_FNAMES
#   define HERR_FNAMES
#ifdef DF_CAPFNAMES
#   define nheprnt   FNAME(HEPRNT)
#   define nheprntc   FNAME(HEPRNTC)
#   define nhestringc FNAME(HESTRINGC)
#else
#   define nheprnt   FNAME(heprnt)
#   define nheprntc   FNAME(heprntc)
#   define nhestringc FNAME(hestringc)
#endif                          /* DF_CAPFNAMES */
#endif                          /* HERR_FNAMES */

    HDFLIBAPI      FRETVAL(void) nheprnt
                (intf  * print_levels);

    HDFLIBAPI      FRETVAL(intf) nheprntc
                (_fcd filename, intf  * print_levels, intf *namelen);

    HDFLIBAPI      FRETVAL(intf) nhestringc
				(intf *error_code,_fcd error_message, intf *len);
/*
   ** from hfileF.c
 */
#ifndef HFILE_FNAMES
#   define HFILE_FNAMES
#ifdef DF_CAPFNAMES
# if defined(UNIX386) || (!(defined INTEL86) && !(defined WIN32))
#   define nhiopen   FNAME(HIOPEN)
#   define nhclose   FNAME(HCLOSE)
#   define nhnumber  FNAME(HNUMBER)
#   define nhxisdir  FNAME(HXISDIR)
#   define nhxiscdir FNAME(HXISCDIR)
#   define nhddontatexit FNAME(HDDONTATEXIT)
#   define nhglibverc FNAME(HGLIBVERC)
#   define nhgfilverc FNAME(HGFILVERC)
#   define nhiishdf   FNAME(HIISHDF)
#  else                                   /* !sl */
#   define nhiopen   FNAME(HIOPEN)
#   define nhiclose   FNAME(HICLOSE)
#   define nhinumbr  FNAME(HINUMBR)
#   define nhxisdir  FNAME(HXISDIR)
#   define nhxiscdir FNAME(HXISCDIR)
#   define nhddontatexit FNAME(HDDONTATEXIT)
#   define nhglibverc FNAME(HGLIBVERC)
#   define nhgfilverc FNAME(HGFILVERC)
#   define nhiishdf   FNAME(HIISHDF)
#  endif

#else
# if defined(UNIX386) || (!(defined INTEL86) && !(defined WIN32))
#   define nhiopen   FNAME(hiopen)
#   define nhclose   FNAME(hclose)
#   define nhnumber  FNAME(hnumber)
#   define nhxisdir  FNAME(hxisdir)
#   define nhxiscdir FNAME(hxiscdir)
#   define nhddontatexit FNAME(hddontatexit)
#   define nhglibverc FNAME(hglibverc)
#   define nhgfilverc FNAME(hgfilverc)
#   define nhiishdf   FNAME(hiishdf)
#  else                                   /* !sl */
#   define nhiopen   FNAME(hiopen)
#   define nhiclose   FNAME(hiclose)
#   define nhinumbr  FNAME(hinumbr)
#   define nhxisdir  FNAME(hxisdir)
#   define nhxiscdir FNAME(hxiscdir)
#   define nhddontatexit FNAME(hddontatexit)
#   define nhglibverc FNAME(hglibverc)
#   define nhgfilverc FNAME(hgfilverc)
#   define nhiishdf   FNAME(hiishdF)
#  endif

#endif                          /* DF_CAPFNAMES */
#endif                          /* HFILE_FNAMES */

    HDFLIBAPI      FRETVAL(intf) nhiopen
                (_fcd name, intf  * acc_mode, intf  * defdds, intf  * namelen);

    HDFLIBAPI      FRETVAL(intf) nhclose
                (intf  * file_id);

    HDFLIBAPI	FRETVAL(intf) nhnumber
		(intf  * file_id, intf  * tag);

    HDFLIBAPI	FRETVAL(intf) nhxisdir
		(_fcd dir, intf * dirlen);

    HDFLIBAPI	FRETVAL(intf) nhxiscdir
		(_fcd dir, intf * dirlen);
 
    HDFLIBAPI      FRETVAL(intf)
                nhddontatexit(void);

    HDFLIBAPI      FRETVAL(intf)   
                nhglibverc(intf *major_v, intf *minor_v, intf *release,
                _fcd string, intf *len);

    HDFLIBAPI      FRETVAL(intf)
                nhgfilverc(intf *file_id, intf *major_v, intf *minor_v,
                intf *release, _fcd string, intf *len);

    HDFLIBAPI      FRETVAL(intf) nhiishdf
                (_fcd name, intf  * namelen);


    
/*
   ** from dfufp2im.c
 */
#ifndef DFUFP2I_FNAMES
#   define DFUFP2I_FNAMES
#ifdef DF_CAPFNAMES
#   define nduif2i       FNAME(DUIF2I)
#else
#   define nduif2i       FNAME(duif2i)
#endif                          /* DF_CAPFNAMES */
#endif                          /* DFUFP2I_FNAMES */

    HDFLIBAPI      FRETVAL(int) nduif2i
                (int32  * hdim, int32  * vdim, float32  * max,
        float32  * min, float32  hscale[], float32  vscale[],
                 float32  data[], _fcd palette, _fcd outfile,
              int  * ct_method, int32  * hres, int32  * vres,
                 int  * compress, int  * lenfn);

    HDFLIBAPI int  DFUfptoimage
                (int32 hdim, int32 vdim, float32 max, float32 min,
       float32  * hscale, float32  * vscale, float32  * data,
                 uint8  * palette, char  * outfile, int ct_method,
                 int32 hres, int32 vres, int compress);

/* for Multi-file fortran Annotation inteface */
#ifndef MFAN_FNAMES
#   define  MFAN_FNAMES
#ifdef DF_CAPFNAMES
# if defined(UNIX386) || (!(defined INTEL86) && !(defined WIN32))
#  define nafstart      FNAME(AFSTART)
#  define naffileinfo   FNAME(AFFILEINFO)
#  define nafend        FNAME(AFEND)
#  define nafcreate     FNAME(AFCREATE)
#  define naffcreate    FNAME(AFFCREATE)
#  define nafselect     FNAME(AFSELECT)
#  define nafnumann     FNAME(AFNUMANN)
#  define nafannlist    FNAME(AFANNLIST)
#  define nafannlen     FNAME(AFANNLEN)
#  define nafwriteann   FNAME(AFWRITEANN)
#  define nafreadann    FNAME(AFREADANN)
#  define nafendaccess  FNAME(AFENDACCESS)
#  define nafgettagref  FNAME(AFGETTAGREF)
#  define nafidtagref   FNAME(AFIDTAGREF)
#  define naftagrefid   FNAME(AFTAGREFID)
#  define nafatypetag   FNAME(AFATYPETAG)
#  define naftagatype   FNAME(AFTAGATYPE)
# else                                     /* !sl */
#  define nafistart     FNAME(AFISTART)
#  define nafifinf      FNAME(AFIFINF)
#  define nafiend       FNAME(AFIEND)
#  define naficreat     FNAME(AFICREAT)
#  define nafifcreat    FNAME(AFIFCREAT)
#  define nafiselct     FNAME(AFISELCT)
#  define nafinann      FNAME(AFINANN)
#  define nafialst      FNAME(AFIALST)
#  define nafialen      FNAME(AFIALEN)
#  define nafiwann      FNAME(AFIWANN)
#  define nafirann      FNAME(AFIRANN)
#  define nafiendac     FNAME(AFIENDAC)
#  define nafigtr       FNAME(AFIGTR)
#  define nafiid2tr     FNAME(AFIID2TR)
#  define nafitr2id     FNAME(AFITR2ID)
#  define nafitp2tg     FNAME(AFITP2TG)
#  define nafitg2tp     FNAME(AFITG2TP)
# endif
#else  /* !DF_CAPFNAMES */
# if defined(UNIX386) || (!(defined INTEL86) && !(defined WIN32))
#  define nafstart      FNAME(afstart)
#  define naffileinfo   FNAME(affileinfo)
#  define nafend        FNAME(afend)
#  define nafcreate     FNAME(afcreate)
#  define naffcreate    FNAME(affcreate)
#  define nafselect     FNAME(afselect)
#  define nafnumann     FNAME(afnumann)
#  define nafannlist    FNAME(afannlist)
#  define nafannlen     FNAME(afannlen)
#  define nafwriteann   FNAME(afwriteann)
#  define nafreadann    FNAME(afreadann)
#  define nafendaccess  FNAME(afendaccess)
#  define nafgettagref  FNAME(afgettagref)
#  define nafidtagref   FNAME(afidtagref)
#  define naftagrefid   FNAME(aftagrefid)
#  define nafatypetag   FNAME(afatypetag)
#  define naftagatype   FNAME(aftagatype)
# else                                       /* !sl */
#  define nafistart     FNAME(afistart)
#  define nafifinf      FNAME(afifinf)
#  define nafiend       FNAME(afiend)
#  define naficreat     FNAME(aficreat)
#  define nafifcreat    FNAME(afifcreat)
#  define nafiselct     FNAME(afiselct)
#  define nafinann      FNAME(afinamm)
#  define nafialst      FNAME(afialst)
#  define nafialen      FNAME(afialen)
#  define nafiwann      FNAME(afiwann)
#  define nafirann      FNAME(afirann)
#  define nafiendac     FNAME(afiendac)
#  define nafigtr       FNAME(afigtr)
#  define nafiid2tr     FNAME(afiid2tr)
#  define nafitr2id     FNAME(afitr2id)
#  define nafitp2tg     FNAME(afitp2tg)
#  define nafitg2tp     FNAME(afitg2tp)
# endif
#endif /* DF_CAPFNAMES */
#endif /* MFAN_FNAMES */

/* Multi-file Annotation C-stubs for fortan interface found in mfanf.c */

HDFLIBAPI FRETVAL(intf)
nafstart(intf *file_id);

HDFLIBAPI FRETVAL(intf)
naffileinfo(intf *an_id, intf *num_flabel, intf *num_fdesc, intf *num_olabel,
            intf *num_odesc);

HDFLIBAPI FRETVAL(intf)
nafend(intf *an_id);

HDFLIBAPI FRETVAL(intf)
nafcreate(intf *an_id, intf *etag, intf *eref, intf *atype);

HDFLIBAPI FRETVAL(intf)
naffcreate(intf *an_id, intf *atype);

HDFLIBAPI FRETVAL(intf)
nafselect(intf *an_id, intf *idx, intf *atype);

HDFLIBAPI FRETVAL(intf)
nafnumann(intf *an_id, intf *atype, intf *etag, intf *eref);

HDFLIBAPI FRETVAL(intf)
nafannlist(intf *an_id, intf *atype, intf *etag, intf *eref, intf alist[]);

HDFLIBAPI FRETVAL(intf)
nafannlen(intf *ann_id);

HDFLIBAPI FRETVAL(intf)
nafwriteann(intf *ann_id,_fcd ann, intf *annlen);

HDFLIBAPI FRETVAL(intf)
nafreadann(intf *ann_id,_fcd ann, intf *maxlen);

HDFLIBAPI FRETVAL(intf)
nafendaccess(intf *ann_id);

HDFLIBAPI FRETVAL(intf)
nafgettagref(intf *an_id, intf *idx, intf *type, intf *tag, intf *ref);

HDFLIBAPI FRETVAL(intf)
nafidtagref(intf *ann_id, intf *tag, intf *ref);

HDFLIBAPI FRETVAL(intf)
naftagrefid(intf *an_id, intf *tag, intf *ref);

HDFLIBAPI FRETVAL(intf)
nafatypetag(intf *atype);

HDFLIBAPI FRETVAL(intf)
naftagatype(intf *tag);

/* Multi-file Annotation C-routines found in mfan.c */
HDFLIBAPI int32 ANstart(int32 file_id);

HDFLIBAPI intn  ANfileinfo(int32 an_id, int32 *n_file_label, int32 *n_file_desc,
                        int32 *n_obj_label, int32 *n_obj_desc);

HDFLIBAPI int32 ANend(int32 an_id);

HDFLIBAPI int32 ANcreate(int32 an_id, uint16 elem_tag, uint16 elem_ref, 
                      ann_type type);

HDFLIBAPI int32 ANcreatef(int32 an_id, ann_type type);

HDFLIBAPI int32 ANselect(int32 an_id, int32 idx, ann_type type);

HDFLIBAPI intn  ANnumann(int32 an_id, ann_type type, uint16 elem_tag, 
                      uint16 elem_ref);

HDFLIBAPI intn  ANannlist(int32 an_id, ann_type type, uint16 elem_tag, 
                       uint16 elem_ref, int32 ann_list[]);

HDFLIBAPI int32 ANannlen(int32 ann_id);

HDFLIBAPI int32 ANwriteann(int32 ann_id, const char *ann, int32 annlen);

HDFLIBAPI int32 ANreadann(int32 ann_id, char *ann, int32 maxlen);

HDFLIBAPI intn  ANendaccess(int32 ann_id);

HDFLIBAPI int32 ANget_tagref(int32 an_id, int32 idx, ann_type type,
                          uint16 *ann_tag, uint16 *ann_ref);

HDFLIBAPI int32 ANid2tagref(int32 an_id, uint16 *ann_tag, uint16 *ann_ref);

HDFLIBAPI int32 ANtagref2id(int32 an_id, uint16 ann_tag, uint16 ann_ref);

HDFLIBAPI uint16 ANatype2tag(ann_type atype);

HDFLIBAPI ann_type ANtag2atype(uint16 atag);

HDFLIBAPI intn ANdestroy(void);

/* for Multi-file fortran GR interface */
#ifndef MFGR_FNAMES
#   define  MFGR_FNAMES
#ifdef DF_CAPFNAMES
# if defined(UNIX386) || (!(defined INTEL86) && !(defined WIN32))
#  define nmgstart      FNAME(MGSTART)
#  define nmgfinfo      FNAME(MGFINFO)
#  define nmgend        FNAME(MGEND)
#  define nmgicreat     FNAME(MGICREAT)
#  define nmgselct      FNAME(MGSELCT)
#  define nmgin2ndx     FNAME(MGIN2NDX)
#  define nmggiinf      FNAME(MGGIINF)
#  define nmgwcimg      FNAME(MGWCIMG)
#  define nmgrcimg      FNAME(MGRCIMG)
#  define nmgwrimg      FNAME(MGWRIMG)
#  define nmgrdimg      FNAME(MGRDIMG)
#  define nmgendac      FNAME(MGENDAC)
#  define nmgid2rf      FNAME(MGID2RF)
#  define nmgr2idx      FNAME(MGR2IDX)
#  define nmgrltil      FNAME(MGRLTIL)
#  define nmgrimil      FNAME(MGRIMIL)
#  define nmggltid      FNAME(MGGLTID)
#  define nmgglinf      FNAME(MGGLINF)
#  define nmgwrlut      FNAME(MGWRLUT)
#  define nmgwclut      FNAME(MGWCLUT)
#  define nmgrdlut      FNAME(MGRDLUT)
#  define nmgrclut      FNAME(MGRCLUT)
#  define nmgisxfil     FNAME(MGISXFIL)
#  define nmgssctp      FNAME(MGSACTP)
#  define nmgiscatt     FNAME(MGISCATT)
#  define nmgisattr     FNAME(MGISATTR)
#  define nmgatinf      FNAME(MGATINF)
#  define nmggcatt      FNAME(MGGCATT)
#  define nmggnatt      FNAME(MGGNATT)
#  define nmggattr      FNAME(MGGATTR)
#  define nmgifndat     FNAME(MGIFNDAT)
# define nmgcgichnk        FNAME(MGCGICHNK)
# define nmgcrcchnk        FNAME(MGCRCCHNK)
# define nmgcrchnk         FNAME(MGCRCHNK)
# define nmgcscchnk        FNAME(MGCSCCHNK)
# define nmgcschnk         FNAME(MGCSCHNK)
# define nmgcwcchnk        FNAME(MGCWCCHNK)
# define nmgcwchnk         FNAME(MGCWCHNK)
# define nmgcscompress     FNAME(MGCSCOMPRESS)
# define nmgcgcompress     FNAME(MGCGCOMPRESS)
# define nmglt2rf         FNAME(MGLT2RF)
# define nmgcgnluts       FNAME(MGCGNLUTS)
# else                                  /* !sl */
#  define nmgistrt      FNAME(MGISTRT)
#  define nmgifinf      FNAME(MGIFINF)
#  define nmgiend       FNAME(MGIEND)
#  define nmgicreat     FNAME(MGICREAT)
#  define nmgislct      FNAME(MGISLCT)
#  define nmgin2ndx     FNAME(MGIN2NDX)
#  define nmgigiinf     FNAME(MGIGIINF)
#  define nmgiwcim      FNAME(MGIWCIM)
#  define nmgircim      FNAME(MGIRCIM)
#  define nmgiwimg      FNAME(MGIWIMG)
#  define nmgirimg      FNAME(MGIRIMG)
#  define nmgiendac     FNAME(MGIENDAC)
#  define nmgiid2r      FNAME(MGIID2R)
#  define nmgir2dx      FNAME(MGIR2DX)
#  define nmgiltil      FNAME(MGILTIL)
#  define nmgiimil      FNAME(MGIIMIL)
#  define nmgiglid      FNAME(MGIGLID)
#  define nmgiglinf     FNAME(MGIGLINF)
#  define nmgiwrlt      FNAME(MGIWRLT)
#  define nmgiwclt      FNAME(MGIWCLT)
#  define nmgirdlt      FNAME(MGIRDLT)
#  define nmgirclt      FNAME(MGIRCLT)
#  define nmgisxfil     FNAME(MGISXFIL)
#  define nmgiactp      FNAME(MGIACTP)
#  define nmgiscatt     FNAME(MGISCATT)
#  define nmgisattr     FNAME(MGISATTR)
#  define nmgiainf      FNAME(MGIAINF)
#  define nmgigcat      FNAME(MGIGCAT)
#  define nmgignat      FNAME(MGIGNAT)
#  define nmgigatt      FNAME(MGIGATT)
#  define nmgifndat     FNAME(MGIFNDAT)
# define nmgcgichnk        FNAME(MGCGICHNK)
# define nmgcrcchnk        FNAME(MGCRCCHNK)
# define nmgcrchnk         FNAME(MGCRCHNK)
# define nmgcscchnk        FNAME(MGCSCCHNK)
# define nmgcschnk         FNAME(MGCSCHNK)
# define nmgcwcchnk        FNAME(MGCWCCHNK)
# define nmgcwchnk         FNAME(MGCWCHNK)
# define nmgcscompress     FNAME(MGCSCOMPRESS)
# define nmgcgcompress     FNAME(MGCGCOMPRESS)
# define nmglt2rf         FNAME(MGLT2RF)
# define nmgcgnluts       FNAME(MGCGNLUTS)
# endif
#else  /* !DF_CAPFNAMES */
# if defined(UNIX386) || (!(defined INTEL86) && !(defined WIN32))
#  define nmgstart      FNAME(mgstart)
#  define nmgfinfo      FNAME(mgfinfo)
#  define nmgend        FNAME(mgend)
#  define nmgicreat     FNAME(mgicreat)
#  define nmgselct      FNAME(mgselct)
#  define nmgin2ndx     FNAME(mgin2ndx)
#  define nmggiinf      FNAME(mggiinf)
#  define nmgwcimg      FNAME(mgwcimg)
#  define nmgrcimg      FNAME(mgrcimg)
#  define nmgwrimg      FNAME(mgwrimg)
#  define nmgrdimg      FNAME(mgrdimg)
#  define nmgendac      FNAME(mgendac)
#  define nmgid2rf      FNAME(mgid2rf)
#  define nmgr2idx      FNAME(mgr2idx)
#  define nmgrltil      FNAME(mgrltil)
#  define nmgrimil      FNAME(mgrimil)
#  define nmggltid      FNAME(mggltid)
#  define nmgglinf      FNAME(mgglinf)
#  define nmgwrlut      FNAME(mgwrlut)
#  define nmgwclut      FNAME(mgwclut)
#  define nmgrdlut      FNAME(mgrdlut)
#  define nmgrclut      FNAME(mgrclut)
#  define nmgisxfil     FNAME(mgisxfil)
#  define nmgssctp      FNAME(mgsactp)
#  define nmgiscatt     FNAME(mgiscatt)
#  define nmgisattr     FNAME(mgisattr)
#  define nmgatinf      FNAME(mgatinf)
#  define nmggcatt      FNAME(mggcatt)
#  define nmggnatt      FNAME(mggnatt)
#  define nmggattr      FNAME(mggattr)
#  define nmgifndat     FNAME(mgifndat)
# define nmgcgichnk        FNAME(mgcgichnk)
# define nmgcrcchnk        FNAME(mgcrcchnk)
# define nmgcrchnk         FNAME(mgcrchnk)
# define nmgcscchnk        FNAME(mgcscchnk)
# define nmgcschnk         FNAME(mgcschnk)
# define nmgcwcchnk        FNAME(mgcwcchnk)
# define nmgcwchnk         FNAME(mgcwchnk)
# define nmgcscompress     FNAME(mgcscompress)
# define nmgcgcompress     FNAME(mgcgcompress)
# define nmglt2rf         FNAME(mglt2rf)
# define nmgcgnluts       FNAME(mgcgnluts)
# else                                    /* !sl */
#  define nmgistrt      FNAME(mgistrt)
#  define nmgifinf      FNAME(mgifinf)
#  define nmgiend        FNAME(mgiend)
#  define nmgicreat     FNAME(mgicreat)
#  define nmgislct      FNAME(mgislct)
#  define nmgin2ndx     FNAME(mgin2ndx)
#  define nmgigiinf      FNAME(mgigiinf)
#  define nmgiwcim      FNAME(mgiwcim)
#  define nmgircim      FNAME(mgircim)
#  define nmgwimg      FNAME(mgiwimg)
#  define nmgrimg      FNAME(mgirimg)
#  define nmgiendac      FNAME(mgiendac)
#  define nmgiid2r      FNAME(mgiid2r)
#  define nmgir2dx      FNAME(mgir2dx)
#  define nmgiltil      FNAME(mgiltil)
#  define nmgiimil      FNAME(mgiimil)
#  define nmgiglid      FNAME(mgiglid)
#  define nmgiglinf     FNAME(mgiglinf)
#  define nmgiwrlt      FNAME(mgiwrlt)
#  define nmgiwclt      FNAME(mgiwclt)
#  define nmgirdlt      FNAME(mgirdlt)
#  define nmgirclt      FNAME(mgirclt)
#  define nmgisxfil     FNAME(mgisxfil)
#  define nmgiactp      FNAME(mgiactp)
#  define nmgiscatt     FNAME(mgiscatt)
#  define nmgisattr     FNAME(mgisattr)
#  define nmgiainf      FNAME(mgiainf)
#  define nmgigcat      FNAME(mgigcat)
#  define nmgignat      FNAME(mgignat)
#  define nmgigatt      FNAME(mgigatt)
#  define nmgifndat     FNAME(mgifndat)
# define nmgcgichnk        FNAME(mgcgichnk)
# define nmgcrcchnk        FNAME(mgcrcchnk)
# define nmgcrchnk         FNAME(mgcrchnk)
# define nmgcscchnk        FNAME(mgcscchnk)
# define nmgcschnk         FNAME(mgcschnk)
# define nmgcwcchnk        FNAME(mgcwcchnk)
# define nmgcwchnk         FNAME(mgcwchnk)
# define nmgcscompress     FNAME(mgcscompress)
# define nmgcgcompress     FNAME(mgcgcompress)
# define nmglt2rf         FNAME(mglt2rf)
# define nmgcgnluts       FNAME(mgcgnluts)
# endif

#endif /* DF_CAPFNAMES */
#endif /* MFGR_FNAMES */

/* Multi-file GR C-stubs for FORTRAN interface found in mfgrf.c */
HDFLIBAPI FRETVAL(intf)     /* !sl */
nmgiwimg(intf * riid, intf *start, intf *stride, intf *count, VOIDP data);

HDFLIBAPI FRETVAL(intf)     /* !sl */
nmgirimg(intf * riid, intf *start, intf *stride, intf *count, VOIDP data);

HDFLIBAPI FRETVAL(intf)     /* !sl */
nmgignat(intf * riid, intf *idx, VOIDP data);

HDFLIBAPI FRETVAL(intf)
nmgstart(intf * fid);

HDFLIBAPI FRETVAL(intf)
nmgfinfo(intf * grid,intf *n_datasets,intf *n_attrs);

HDFLIBAPI FRETVAL(intf)
nmgend(intf * grid);

HDFLIBAPI FRETVAL(intf)
nmgicreat(intf * grid, _fcd name, intf *ncomp, intf *nt, intf *il, intf dimsizes[2], intf *nlen);

HDFLIBAPI FRETVAL(intf)
nmgselct(intf * grid, intf *idx);

HDFLIBAPI FRETVAL(intf)
nmgin2ndx(intf * grid, _fcd name, intf *nlen);

HDFLIBAPI FRETVAL(intf)
nmggiinf(intf * riid, _fcd name, intf *ncomp, intf *nt, intf *il, intf *dimsizes, intf *nattr);


HDFLIBAPI FRETVAL(intf)
nmgwcimg(intf * riid, intf *start, intf *stride, intf *count, _fcd data);

HDFLIBAPI FRETVAL(intf)
nmgrcimg(intf * riid, intf *start, intf *stride, intf *count, _fcd data);

HDFLIBAPI FRETVAL(intf)
nmgwrimg(intf * riid, intf *start, intf *stride, intf *count, void * data);

HDFLIBAPI FRETVAL(intf)
nmgrdimg(intf * riid, intf *start, intf *stride, intf *count, void * data);

HDFLIBAPI FRETVAL(intf)
nmgendac(intf * riid);

HDFLIBAPI FRETVAL(intf)
nmgid2rf(intf * riid);

HDFLIBAPI FRETVAL(intf)
nmgr2idx(intf * grid, intf *ref);

HDFLIBAPI FRETVAL(intf)
nmgrltil(intf * riid, intf *il);

HDFLIBAPI FRETVAL(intf)
nmgrimil(intf * riid, intf *il);

HDFLIBAPI FRETVAL(intf)
nmggltid(intf * riid, intf *lut_index);

HDFLIBAPI FRETVAL(intf)
nmgglinf(intf * lutid, intf *ncomp, intf *nt, intf *il, intf *nentries);

HDFLIBAPI FRETVAL(intf)
nmgwrlut(intf * lutid, intf *ncomp, intf *nt, intf *il, intf *nentries, void * data);

HDFLIBAPI FRETVAL(intf)
nmgwclut(intf * lutid, intf *ncomp, intf *nt, intf *il, intf *nentries, _fcd data);

HDFLIBAPI FRETVAL(intf)
nmgrdlut(intf * lutid, void * data);

HDFLIBAPI FRETVAL(intf)
nmgrclut(intf * lutid, _fcd data);

HDFLIBAPI FRETVAL(intf)
nmgisxfil(intf * riid, _fcd filename, intf *offset, intf *nlen);

HDFLIBAPI FRETVAL(intf)
nmgsactp(intf * riid, intf *accesstype);

HDFLIBAPI FRETVAL(intf)
nmgiscatt(intf * riid, _fcd name, intf *nt, intf *count, _fcd data, intf *nlen);

HDFLIBAPI FRETVAL(intf)
nmgisattr(intf * riid, _fcd name, intf *nt, intf *count, void * data, intf *nlen);

HDFLIBAPI FRETVAL(intf)
nmgatinf(intf * riid, intf *idx, _fcd name, intf *nt, intf *count);

HDFLIBAPI FRETVAL(intf)
nmggcatt(intf * riid, intf *idx, _fcd data);

HDFLIBAPI FRETVAL(intf)
nmggnatt(intf * riid, intf *idx, void * data);

HDFLIBAPI FRETVAL(intf)
nmggattr(intf * riid, intf *idx, void * data);

HDFLIBAPI FRETVAL(intf)
nmgifndat(intf * riid, _fcd name, intf *nlen);

     HDFLIBAPI FRETVAL (intf)
     nmgcgichnk(intf *id, intf *dim_length, intf *flags);

    HDFLIBAPI FRETVAL (intf)
       nmgcrcchnk(intf *id, intf *start, _fcd char_data);

    HDFLIBAPI FRETVAL (intf)
       nmgcrchnk(intf *id, intf *start, VOIDP num_data);

    HDFLIBAPI FRETVAL (intf)
       nmgcscchnk(intf *id, intf *maxcache, intf *flags);

    HDFLIBAPI FRETVAL (intf)
      nmgcschnk(intf *id, intf *dim_length, intf *comp_type,
                intf *comp_prm);
    HDFLIBAPI FRETVAL (intf)
       nmgcwcchnk(intf *id, intf *start, _fcd char_data);

    HDFLIBAPI FRETVAL (intf)
       nmgcwchnk(intf *id, intf *start, VOIDP num_data);

    HDFLIBAPI FRETVAL (intf)
       nmgcscompress(intf *id, intf *comp_type, intf *comp_prm);

    HDFLIBAPI FRETVAL (intf)
       nmgcgcompress(intf *id, intf *comp_type, intf *comp_prm);

    HDFLIBAPI FRETVAL (intf)
       nmglt2rf(intf *id);

    HDFLIBAPI FRETVAL (intf)
       nmgcgnluts(intf *id);

/* Multi-file Raster C-routines found in mfgr.c */
HDFLIBAPI intn rigcompare(void * k1, void * k2, intn cmparg);

HDFLIBAPI int32 GRstart(int32 hdf_file_id);

HDFLIBAPI intn GRfileinfo(int32 grid,int32 *n_datasets,int32 *n_attrs);

HDFLIBAPI intn GRend(int32 grid);

HDFLIBAPI int32 GRcreate(int32 grid,const char *name,int32 ncomp,int32 nt,int32 il,
    int32 dimsizes[2]);

HDFLIBAPI int32 GRselect(int32 grid,int32 idx);

HDFLIBAPI int32 GRnametoindex(int32 grid,const char *name);

HDFLIBAPI intn GRgetiminfo(int32 riid,char *name,int32 *ncomp,int32 *nt,int32 *il,
    int32 dimsizes[2],int32 *n_attr);

HDFLIBAPI intn GRgetnluts(int32 riid);

HDFLIBAPI intn GRwriteimage(int32 riid,int32 start[2],int32 stride[2],
    int32 count[2],void * data);

HDFLIBAPI intn GRreadimage(int32 riid,int32 start[2],int32 stride[2],
    int32 count[2],void * data);

HDFLIBAPI intn GRendaccess(int32 riid);

HDFLIBAPI uint16 GRidtoref(int32 riid);

HDFLIBAPI int32 GRreftoindex(int32 grid,uint16 ref);

HDFLIBAPI intn GRreqlutil(int32 riid,intn il);

HDFLIBAPI intn GRreqimageil(int32 riid,intn il);

HDFLIBAPI int32 GRgetlutid(int32 riid,int32 idx);

HDFLIBAPI uint16 GRluttoref(int32 lutid);

HDFLIBAPI intn GRgetlutinfo(int32 riid,int32 *ncomp,int32 *nt,
    int32 *il,int32 *nentries);

HDFLIBAPI intn GRwritelut(int32 riid,int32 ncomps,int32 nt,
    int32 il,int32 nentries,void * data);

HDFLIBAPI intn GRreadlut(int32 lutid,void * data);

HDFLIBAPI intn GRsetexternalfile(int32 riid,const char *filename,int32 offset);

HDFLIBAPI intn GRsetaccesstype(int32 riid,uintn accesstype);

HDFLIBAPI intn GRsetcompress(int32 riid,comp_coder_t comp_type,comp_info *cinfo);

HDFLIBAPI intn GRgetcompress(int32 riid,comp_coder_t* comp_type,comp_info *cinfo);

HDFLIBAPI intn GRsetattr(int32 id,const char *name,int32 attr_nt,int32 count,const void * data);

HDFLIBAPI intn GRattrinfo(int32 id,int32 idx,char *name,int32 *attr_nt,int32 *count);

HDFLIBAPI intn GRgetattr(int32 id,int32 idx,void * data);

HDFLIBAPI int32 GRfindattr(int32 id,const char *name);

HDFLIBAPI intn GRPshutdown(void);

/*=== HDF_CHUNK_DEF same as in mfhdf.h - moved here  ====*/

/* Bit flags used for SDsetchunk(), SDgetchunkinfo() 
   GRsetchunk() and GRgetchunkinfo(). Note that GRs do not support NBIT */
#define HDF_NONE    0x0
#define HDF_CHUNK   0x1
#define HDF_COMP    0x3
#define HDF_NBIT    0x5

/* Cache flags */
#define HDF_CACHEALL 0x1

/* Chunk Defintion, Note that GRs need only 2 dimensions for the chunk_lengths */
typedef union hdf_chunk_def_u
{
    /* Chunk Lengths only */
    int32   chunk_lengths[MAX_VAR_DIMS]; /* chunk lengths along each dimension */

    struct 
    {   /* For Compression info */
        int32      chunk_lengths[MAX_VAR_DIMS]; /* chunk lengths along each dimension */
        int32      comp_type;    /* Compression type */
        int32      model_type;   /* Compression model type */
        comp_info  cinfo;        /* Compression info struct */
        model_info minfo;        /* Compression model info struct */
    }comp;
        
    struct 
    { /* For NBIT, Used by SDS and not by GR */
        int32 chunk_lengths[MAX_VAR_DIMS]; /* chunk lengths along each dimension */
        intn  start_bit; /* offset of the start bit in the data */
        intn  bit_len;   /* number of bits to store */
        intn  sign_ext;  /* whether to sign extend or not */
        intn  fill_one;  /* whether to fill with 1's or not (0's) */
    } nbit;

} HDF_CHUNK_DEF;

/*=== GR Chunking Routines  ====*/

/******************************************************************************
 NAME
      GRsetchunk   -- make GR a chunked GR

 DESCRIPTION
      This routine makes the GR a chunked GR according to the chunk
      definition passed in.

      The image currently cannot be special already.  i.e. NBIT,
      COMPRESSED, or EXTERNAL. This is an Error.

      The defintion of the HDF_CHUNK_DEF union with relvant fields is:

      typedef union hdf_chunk_def_u
      {
         int32   chunk_lengths[2];  Chunk lengths along each dimension

         struct 
          {   
            int32     chunk_lengths[2]; Chunk lengths along each dimension
            int32     comp_type;                   Compression type 
            comp_info cinfo;                       Compression info struct 
          }comp;

      } HDF_CHUNK_DEF

      The simplist is the 'chunk_lengths' array specifiying chunk 
      lengths for each dimension where the 'flags' argument set to 
      'HDF_CHUNK';

      COMPRESSION is set by using the 'HDF_CHUNK_DEF' structure to set the
      appropriate compression information along with the required chunk lengths
      for each dimension. The compression information is the same as 
      that set in 'SDsetcompress()'. The bit-or'd'flags' argument' is set to 
      'HDF_CHUNK | HDF_COMP'.

      See the example in pseudo-C below for further usage.

      The maximum number of Chunks in an HDF file is 65,535.

      The performance of the GRxxx interface with chunking is greatly
      affected by the users access pattern over the image and by
      the maximum number of chunks set in the chunk cache. The cache contains 
      the Least Recently Used(LRU cache replacment policy) chunks. See the
      routine GRsetchunkcache() for further info on the chunk cache and how 
      to set the maximum number of chunks in the chunk cache. A default chunk 
      cache is always created.

      The following example shows the organization of chunks for a 2D iamge.
      e.g. 4x4 image with 2x2 chunks. The array shows the layout of
           chunks in the chunk array.

            4 ---------------------                                           
              |         |         |                                                 
        Y     |  (0,1)  |  (1,1)  |                                       
        ^     |         |         |                                      
        |   2 ---------------------                                       
        |     |         |         |                                               
        |     |  (0,0)  |  (1,0)  |                                      
        |     |         |         |                                           
        |     ---------------------                                         
        |     0         2         4                                       
        ---------------> X                                                       
                                                                                
        --Without compression--:
        {                                                                    
        HDF_CHUNK_DEF chunk_def;
                                                                            
        .......                                                                    
        -- Set chunk lengths --                                                    
        chunk_def.chunk_lengths[0]= 2;                                                     
        chunk_def.chunk_lengths[1]= 2; 

        -- Set Chunking -- 
        GRsetchunk(riid, chunk_def, HDF_CHUNK);                      
         ......                                                                  
        }                                                                           

        --With compression--:
        {                                                                    
        HDF_CHUNK_DEF chunk_def;
                                                                            
        .......                
        -- Set chunk lengths first --                                                    
        chunk_def.chunk_lengths[0]= 2;                                                     
        chunk_def.chunk_lengths[1]= 2;

        -- Set compression --
        chunk_def.comp.cinfo.deflate.level = 9;
        chunk_def.comp.comp_type = COMP_CODE_DEFLATE;

        -- Set Chunking with Compression --
        GRsetchunk(riid, chunk_def, HDF_CHUNK | HDF_COMP);                      
         ......                                                                  
        }                                                                           

 RETURNS
        SUCCEED/FAIL
******************************************************************************/
HDFLIBAPI intn GRsetchunk
    (int32 riid,             /* IN: raster access id */
     HDF_CHUNK_DEF chunk_def, /* IN: chunk definition */
     int32 flags              /* IN: flags */);

/******************************************************************************
 NAME
     GRgetchunkinfo -- get Info on GR

 DESCRIPTION
     This routine gets any special information on the GR. If its chunked,
     chunked and compressed or just a regular GR. Currently it will only
     fill the array of chunk lengths for each dimension as specified in
     the 'HDF_CHUNK_DEF' union. You can pass in a NULL for 'chunk_def'
     if don't want the chunk lengths for each dimension.
     Additionaly if successfull it will return a bit-or'd value in 'flags' 
     indicating if the GR is:

     Chunked                  -> flags = HDF_CHUNK
     Chunked and compressed   -> flags = HDF_CHUNK | HDF_COMP 
     Non-chunked              -> flags = HDF_NONE
  
     e.g. 4x4 array - Pseudo-C
     {
     int32   rcdims[3];
     HDF_CHUNK_DEF rchunk_def;
     int32   cflags;
     ...
     rchunk_def.chunk_lengths = rcdims;
     GRgetchunkinfo(riid, &rchunk_def, &cflags);
     ...
     }

 RETURNS
        SUCCEED/FAIL
******************************************************************************/
HDFLIBAPI intn GRgetchunkinfo
    (int32 riid,              /* IN: Raster access id */
     HDF_CHUNK_DEF *chunk_def, /* IN/OUT: chunk definition */
     int32 *flags              /* IN/OUT: flags */);


/******************************************************************************
 NAME
     GRwritechunk  -- write the specified chunk to the GR

 DESCRIPTION
     This routine writes a whole chunk of data to the chunked GR 
     specified by chunk 'origin' for the given GR and can be used
     instead of GRwriteimage() when this information is known. This
     routine has less overhead and is much faster than using GRwriteimage().

     Origin specifies the co-ordinates of the chunk according to the chunk
     position in the overall chunk array.

     'datap' must point to a whole chunk of data.

     See GRsetchunk() for a description of the organization of chunks in an GR.

 RETURNS
        SUCCEED/FAIL
******************************************************************************/
HDFLIBAPI intn GRwritechunk
    (int32 riid,      /* IN: raster access id */
     int32 *origin,    /* IN: origin of chunk to write */
     const void *datap /* IN: buffer for data */);

/******************************************************************************
 NAME
     GRreadchunk   -- read the specified chunk to the GR

 DESCRIPTION
     This routine reads a whole chunk of data from the chunked GR
     specified by chunk 'origin' for the given GR and can be used
     instead of GRreadimage() when this information is known. This
     routine has less overhead and is much faster than using GRreadimage().

     Origin specifies the co-ordinates of the chunk according to the chunk
     position in the overall chunk array.

     'datap' must point to a whole chunk of data.

     See GRsetchunk() for a description of the organization of chunks in an GR.

 RETURNS
        SUCCEED/FAIL
******************************************************************************/
HDFLIBAPI intn GRreadchunk
    (int32 riid,      /* IN: raster access id */
     int32 *origin,    /* IN: origin of chunk to read */
     void  *datap      /* IN/OUT: buffer for data */);


/******************************************************************************
NAME
     GRsetchunkcache -- maximum number of chunks to cache 

DESCRIPTION
     Set the maximum number of chunks to cache.

     The cache contains the Least Recently Used(LRU cache replacment policy) 
     chunks. This routine allows the setting of maximum number of chunks that 
     can be cached, 'maxcache'.

     The performance of the GRxxx interface with chunking is greatly
     affected by the users access pattern over the image and by
     the maximum number of chunks set in the chunk cache. The number chunks 
     that can be set in the cache is process memory limited. It is a good 
     idea to always set the maximum number of chunks in the cache as the 
     default heuristic does not take into account the memory available for 
     the application.

     By default when the GR is promoted to a chunked element the 
     maximum number of chunks in the cache 'maxcache' is set to the number of
     chunks along the last dimension.

     The values set here affects the current object's caching behaviour.

     If the chunk cache is full and 'maxcache' is greater then the
     current 'maxcache' value, then the chunk cache is reset to the new
     'maxcache' value, else the chunk cache remains at the current
     'maxcache' value.

     If the chunk cache is not full, then the chunk cache is set to the
     new 'maxcache' value only if the new 'maxcache' value is greater than the
     current number of chunks in the cache.

     Use flags argument of 'HDF_CACHEALL' if the whole object is to be cached 
     in memory, otherwise pass in zero(0). Currently you can only
     pass in zero.

    See GRsetchunk() for a description of the organization of chunks in an GR.

RETURNS
     Returns the 'maxcache' value for the chunk cache if successful 
     and FAIL otherwise
******************************************************************************/
HDFLIBAPI intn GRsetchunkcache
    (int32 riid,     /* IN: raster access id */
     int32 maxcache,  /* IN: max number of chunks to cache */
     int32 flags      /* IN: flags = 0, HDF_CACHEALL */);


#ifdef HAVE_FMPOOL
/******************************************************************************
NAME
     Hmpset - set pagesize and maximum number of pages to cache on next open/create

DESCRIPTION
     Set the pagesize and maximum number of pages to cache on the next 
     open/create of a file. A pagesize that is a power of 2 is recommended.

     The values set here only affect the next open/creation of a file and
     do not change a particular file's paging behaviour after it has been
     opened or created. This maybe changed in a later release.

     Use flags arguement of 'MP_PAGEALL' if the whole file is to be cached 
     in memory otherwise passs in zero.

RETURNS
     Returns SUCCEED if successful and FAIL otherwise

NOTE
     This calls the real routine MPset().
     Currently 'maxcache' has to be greater than 1. Maybe use special 
     case of 0 to specify you want to turn page buffering off or use
     the flags arguement. 

******************************************************************************/
HDFLIBAPI int  Hmpset(int pagesize, /* IN: pagesize to use for next open/create */
                   int maxcache, /* IN: max number of pages to cache */
                   int flags     /* IN: flags = 0, MP_PAGEALL */);

/******************************************************************************
NAME
     Hmpget - get last pagesize and max number of pages cached for open/create

DESCRIPTION
     This gets the last pagesize and maximum number of pages cached for 
     the last open/create of a file.

RETURNS
     Returns SUCCEED.

NOTES
     This routine calls the real routine MPget().
******************************************************************************/
HDFLIBAPI int  Hmpget(int *pagesize, /*OUT: pagesize to used in last open/create */
                   int *maxcache, /*OUT: max number of pages cached in last open/create */
                   int flags      /* IN: */);

#endif /* HAVE_FMPOOL */

/* Vset interface functions (used to be in vproto.h) */

/* Useful macros, which someday might become actuall functions */
/*
 * macros for VSinquire
 * all these macros should be public for users
 */
#define VSQuerycount(vs, count) \
        (VSinquire (vs, (int32 *) count, (int32*) NULL, (char*) NULL, (int32*) NULL, (char*) NULL))

#define VSQueryinterlace(vs, intr) \
        (VSinquire (vs, (int32 *) NULL, (int32*) intr, (char*) NULL, (int32*) NULL, (char*) NULL))

#define VSQueryfields(vs, flds) \
        (VSinquire (vs, (int32 *) NULL, (int32*) NULL, (char*) flds, (int32*) NULL, (char*) NULL))

#define VSQueryvsize(vs, size) \
        (VSinquire (vs, (int32 *) NULL, (int32*) NULL, (char*) NULL, (int32*) size, (char*) NULL))

#define VSQueryname(vs, name) \
        (VSinquire (vs, (int32 *) NULL, (int32*) NULL, (char*) NULL, (int32*) NULL, (char*) name))

/* 
   ** from vattr.c
 */
   HDFLIBAPI intn Vsetattr
                (int32 vgid,  const char *attrname, int32 datatype,
                 int32 count, const void * values);
   HDFLIBAPI intn Vnattrs
                (int32 vgid);
   HDFLIBAPI intn Vfindattr
                (int32 vgid, const char *attrname);
   HDFLIBAPI intn Vattrinfo
                (int32 vgid, intn attrindex, char *name, 
                 int32 *datatype, int32 *count, int32 *size);
   HDFLIBAPI intn Vgetattr
                (int32 vgid, intn attrindex, void * values);
   HDFLIBAPI int32 Vgetversion
                (int32 vgid);
   HDFLIBAPI intn VSfindex
                 (int32 vsid, const char *fieldname, int32 *fldindex);
   HDFLIBAPI intn VSsetattr
                (int32 vsid, int32 findex, const char *attrname,
                 int32 datatype, int32 count, const void * values);
   HDFLIBAPI intn VSnattrs
                (int32 vsid);
   HDFLIBAPI intn VSfnattrs
                (int32 vsid, int32 findex);
   HDFLIBAPI intn VSfindattr
                (int32 vsid, int32 findex, const char *attrname);
   HDFLIBAPI intn VSattrinfo
                (int32 vsid, int32 findex, intn attrindex,
                 char *name, int32 *datatype, int32 *count, 
                 int32 *size);
   HDFLIBAPI intn VSgetattr
                (int32 vsid, int32 findex, intn attrindex,
                  void * values);
   HDFLIBAPI intn VSisattr
                (int32 vsid);
/*
   ** from vconv.c
 */
    HDFLIBAPI int32 vicheckcompat
                (HFILEID f);

    HDFLIBAPI int32 vimakecompat
                (HFILEID f);

    HDFLIBAPI int32 vcheckcompat
                (char  * fs);

    HDFLIBAPI int32 vmakecompat
                (char  * fs);

/*
   ** from vg.c
 */
    HDFLIBAPI int32 VSelts
                (int32 vkey);

    HDFLIBAPI int32 VSgetinterlace
                (int32 vkey);

    HDFLIBAPI intn VSsetinterlace
                (int32 vkey, int32 interlace);

    HDFLIBAPI int32 VSgetfields
                (int32 vkey, char  * fields);

    HDFLIBAPI intn VSfexist
                (int32 vkey, char  * fields);

    HDFLIBAPI int32 VSsizeof
                (int32 vkey, char  * fields);

    HDFLIBAPI void VSdump
                (int32 vkey);

    HDFLIBAPI int32 VSsetname
                (int32 vkey, const char  * vsname);

    HDFLIBAPI int32 VSsetclass
                (int32 vkey, const char  * vsclass);

    HDFLIBAPI int32 VSgetname
                (int32 vkey, char  * vsname);

    HDFLIBAPI int32 VSgetclass
                (int32 vkey, char  * vsclass);

    HDFLIBAPI intn VSinquire
                (int32 vkey, int32  * nelt, int32  * interlace,
           char  * fields, int32  * eltsize, char  * vsname);

    HDFLIBAPI int32 VSlone
                (HFILEID f, int32  * idarray, int32 asize);

    HDFLIBAPI int32 Vlone
                (HFILEID f, int32  * idarray, int32 asize);

    HDFLIBAPI int32 Vfind
                (HFILEID f, const char  * vgname);

    HDFLIBAPI int32 VSfind
                (HFILEID f, const char  * vsname);

    HDFLIBAPI int32 Vfindclass
                (HFILEID f, const char  * vgclass);

    HDFLIBAPI int32 VSfindclass
                (HFILEID f, const char  * vsclass);

    HDFLIBAPI intn VSsetblocksize
                (int32 vkey, int32 block_size);

    HDFLIBAPI intn VSsetnumblocks
                (int32 vkey, int32 num_blocks);

    HDFLIBAPI intn VSgetblockinfo
                (int32 vkey, int32* block_size, int32* num_blocks);

    HDFLIBAPI void Vsetzap
                (void);

/*
   ** from vgp.c
 */
    HDFLIBAPI intn vcompare
                (void * k1, void * k2, intn cmparg);

    HDFLIBAPI intn vcompareref
                (void * k1, void * k2, intn cmparg);

    HDFLIBAPI void vdestroynode
                (void * n);

    HDFLIBAPI void vtfreekey
                (void * k);

    HDFLIBAPI intn Vinitialize
                (HFILEID f);

    HDFLIBAPI intn Vfinish
                (HFILEID f);

    HDFLIBAPI HFILEID Vopen
                (char *path, intn acc_mode, int16 ndds);

    HDFLIBAPI intn Vclose
                (HFILEID f);

    HDFLIBAPI int32 vexistvg
                (HFILEID f, uint16 vgid);

    HDFLIBAPI int32 Vattach
                (HFILEID f, int32 vgid, const char  * accesstype);

    HDFLIBAPI int32 Vdetach
                (int32 vkey);

    HDFLIBAPI int32 Vinsert
                (int32 vkey, int32 vskey);
    /* note: 2nd arg of Vinsert can also be (VGROUP *) */

    HDFLIBAPI int32 Vflocate
                (int32 vkey, char  * field);

    HDFLIBAPI intn Vinqtagref
                (int32 vkey, int32 tag, int32 ref);

    HDFLIBAPI int32 Vntagrefs
                (int32 vkey);

    HDFLIBAPI int32 Vnrefs
                (int32 vkey,int32 tag);

    HDFLIBAPI int32 Vgettagrefs
                (int32 vkey, int32  tagarray[], int32  refarray[], int32 n);

    HDFLIBAPI intn Vgettagref
                (int32 vkey, int32 which, int32  * tag, int32  * ref);

    HDFLIBAPI int32 VQueryref
                (int32 vkey);

    HDFLIBAPI int32 VQuerytag
                (int32 vkey);

    HDFLIBAPI int32 Vaddtagref
                (int32 vkey, int32 tag, int32 ref);

    HDFLIBAPI int32 Ventries
                (HFILEID f, int32 vgid);

    HDFLIBAPI int32 Vsetname
                (int32 vkey, const char  * vgname);

    HDFLIBAPI int32 Vsetclass
                (int32 vkey, const char  * vgclass);

    HDFLIBAPI intn Visvg
                (int32 vkey, int32 id);

    HDFLIBAPI intn Visvs
                (int32 vkey, int32 id);

    HDFLIBAPI int32 Vgetid
                (HFILEID f, int32 vgid);

    HDFLIBAPI int32 Vgetnext
                (int32 vkey, int32 id);

    HDFLIBAPI int32 Vgetname
                (int32 vkey, char  * vgname);

    HDFLIBAPI int32 Vgetclass
                (int32 vkey, char  * vgclass);

    HDFLIBAPI intn Vinquire
                (int32 vkey, int32  * nentries, char  * vgname);

    HDFLIBAPI int32 Vdelete
                (int32 f, int32 ref);

/*******************************************************************************
NAME
   Vdeletetagref - delete tag/ref pair in Vgroup

DESCRIPTION
    Deletes the given tag/ref pair from the Vgroup.  If the given tag/ref pair 
    does not exist in the vgroup the routine will return FAIL. Users should use 
    Vinqtagref() to check if the tag/ref pair exists before deleting.

RETURNS
    Returns SUCCEED if the tag/ref pair is deleted from Vgroup and
    FAIL if unsuccessful.

*******************************************************************************/
HDFLIBAPI intn
Vdeletetagref(int32 vkey, /* IN: vgroup key */ 
              int32 tag,  /* IN: tag to delete in vgroup */
              int32 ref   /* IN: ref to delete in vgroup */);

    HDFLIBAPI intn VPshutdown(void);

/*
   ** from vparse.c
 */
    HDFLIBAPI int32 scanattrs
                (const char  * attrs, int32  * attrc, char  *** attrv);

/*
   ** from vhi.c
 */
    HDFLIBAPI int32 VHstoredata
                (HFILEID f, const char  * field, const uint8  *buf, int32 n, int32 datatype,
                 const char  * vsname, const char  * vsclass);

    HDFLIBAPI int32 VHstoredatam
                (HFILEID f, const char * field, const uint8  *buf, int32 n, int32 datatype,
                 const char  * vsname, const char  * vsclass, int32 order);

    HDFLIBAPI int32 VHmakegroup
                (HFILEID f, int32  tagarray[], int32  refarray[], int32 n,
                    const char * vgname, const char  * vgclass);

/*
   ** from vio.c
 */

    HDFLIBAPI intn VSPhshutdown(void);

    HDFLIBAPI int32 vexistvs
                (HFILEID f, uint16 vsref);

    HDFLIBAPI void vsdestroynode
                (void * n);

    HDFLIBAPI void vfdestroynode
                (void * n);

    HDFLIBAPI int32 VSattach
                (HFILEID f, int32 vsref, const char  * accesstype);

    HDFLIBAPI int32 VSdetach
                (int32 vkey);

    HDFLIBAPI int32 VSQuerytag
                (int32 vkey);

    HDFLIBAPI int32 VSQueryref
                (int32 vkey);
 
    HDFLIBAPI int32 VSgetid
                (HFILEID f, int32 vsref);

    HDFLIBAPI int32 VSgetversion
                (int32 vkey);

    HDFLIBAPI int32 VSdelete
                (int32 f, int32 ref);

    HDFLIBAPI int32 VSappendable
                (int32 vkey, int32 blk);

/*
   ** from vsfld.c
 */

    HDFLIBAPI intn VSsetfields
                (int32 vkey, const char  * fields);

    HDFLIBAPI intn VSfdefine
                (int32 vkey, const char  * field, int32 localtype, int32 order);

    HDFLIBAPI int32 VFnfields
                (int32 vkey);

    HDFLIBAPI char *VFfieldname
                (int32 vkey, int32 idx);

    HDFLIBAPI int32 VFfieldtype
                (int32 vkey, int32 idx);

    HDFLIBAPI int32 VFfieldisize
                (int32 vkey, int32 idx);

    HDFLIBAPI int32 VFfieldesize
                (int32 vkey, int32 idx);

    HDFLIBAPI int32 VFfieldorder
                (int32 vkey, int32 idx);

    HDFLIBAPI intn VSsetexternalfile
		(int32 vkey, const char *filename, int32 offset);

    HDFLIBAPI intn VSfpack
                (int32 vsid, intn packtype, const char *fields_in_buf,
                void * buf, intn bufsz, intn n_records, 
                const char *fields, void * fldbufpt[]);

/*
   ** from vrw.c
 */
    HDFLIBAPI intn VSPshutdown(void);

    HDFLIBAPI int32 VSseek
                (int32 vkey, int32 eltpos);

    HDFLIBAPI int32 VSread
                (int32 vkey, uint8  buf[], int32 nelt, int32 interlace);

    HDFLIBAPI int32 VSwrite
                (int32 vkey, const uint8  buf[], int32 nelt, int32 interlace);

/*
   ** from vgF.c
 */
#ifndef VG_FNAMES
#   define VG_FNAMES
#ifdef DF_CAPFNAMES
# if defined(UNIX386) || (!(defined INTEL86) && !(defined WIN32))
#   define  ndfivopn FNAME(DFIVOPN)
#   define  ndfvclos FNAME(DFVCLOS)
#   define  nvatchc  FNAME(VATCHC)
#   define  nvdtchc  FNAME(VDTCHC)
#   define  nvgnamc  FNAME(VGNAMC)
#   define  nvgclsc  FNAME(VGCLSC)
#   define  nvinqc   FNAME(VINQC)
#   define  nvdeletec FNAME(VDELETEC)
#   define  nvgidc   FNAME(VGIDC)
#   define  nvgnxtc  FNAME(VGNXTC)
#   define  nvsnamc  FNAME(VSNAMC)
#   define  nvsclsc  FNAME(VSCLSC)
#   define  nvinsrtc FNAME(VINSRTC)
#   define  nvisvgc  FNAME(VISVGC)
#   define  nvisvsc  FNAME(VISVSC)
#   define  nvsatchc FNAME(VSATCHC)
#   define  nvsdtchc FNAME(VSDTCHC)
#   define  nvsqref  FNAME(VSQREF)
#   define  nvsqtag  FNAME(VSQTAG)
#   define  nvsgver  FNAME(VSGVER)
#   define  nvsseekc FNAME(VSSEEKC)
#   define  nvsgnamc FNAME(VSGNAMC)
#   define  nvsgclsc FNAME(VSGCLSC)
#   define  nvsinqc  FNAME(VSINQC)
#   define  nvsfexc  FNAME(VSFEXC)
#   define  nvsfndc  FNAME(VSFNDC)
#   define  nvsgidc  FNAME(VSGIDC)
#   define  nvsdltc  FNAME(VSDLTC)
#   define  nvsapp   FNAME(VSAPP)
#   define  nvssnamc FNAME(VSSNAMC)
#   define  nvssclsc FNAME(VSSCLSC)
#   define  nvssfldc FNAME(VSSFLDC)
#   define  nvssintc FNAME(VSSINTC)
#   define  nvsfdefc FNAME(VSFDEFC)
#   define  nvssextfc FNAME(VSSEXTFC)
#   define  nvfnflds FNAME(VFNFLDS)
#   define  nvffnamec FNAME(VFFNAMEC)
#   define  nvfftype FNAME(VFFTYPE)
#   define  nvffisiz FNAME(VFFISIZ)
#   define  nvffesiz FNAME(VFFESIZ)
#   define  nvffordr FNAME(VFFORDR)
#   define  nvsfrdc  FNAME(VSFRDC)
#   define  nvsfrd   FNAME(VSFRD)
#   define  nvsreadc FNAME(VSREADC)
#   define  nvsfwrt  FNAME(VSFWRT)
#   define  nvsfwrtc FNAME(VSFWRTC)
#   define  nvswritc FNAME(VSWRITC)
#   define  nvsgintc FNAME(VSGINTC)
#   define  nvseltsc FNAME(VSELTSC)
#   define  nvsgfldc FNAME(VSGFLDC)
#   define  nvssizc  FNAME(VSSIZC)
#   define  nventsc  FNAME(VENTSC)
#   define  nvlonec  FNAME(VLONEC)
#   define  nvslonec FNAME(VSLONEC)
#   define  nvfindc  FNAME(VFINDC)
#   define  nvfndclsc FNAME(VFNDCLSC)
#   define  nvhscdc  FNAME(VHSCDC)
#   define  nvhsdc   FNAME(VHSDC)
#   define  nvhscdmc FNAME(VHSCDMC)
#   define  nvhsdmc  FNAME(VHSDMC)
#   define  nvhmkgpc FNAME(VHMKGPC)
#   define  nvflocc  FNAME(VFLOCC)
#   define  nvinqtrc FNAME(VINQTRC)
#   define  nvntrc   FNAME(VNTRC)
#   define  nvnrefs  FNAME(VNREFS)
#   define  nvgttrsc FNAME(VGTTRSC)
#   define  nvqref   FNAME(VQREF)
#   define  nvqtag   FNAME(VQTAG)
#   define  nvgttrc  FNAME(VGTTRC)
#   define  nvadtrc  FNAME(VADTRC)
#   define  nvfstart FNAME(VFSTART)
#   define  nvfend   FNAME(VFEND)
#   define  nvsqfnelt   FNAME(VSQFNELT)
#   define  nvsqfintr   FNAME(VSQFINTR)
#   define  nvsqfldsc   FNAME(VSQFLDSC)
#   define  nvsqfvsiz   FNAME(VSQFVSIZ)
#   define  nvsqnamec   FNAME(VSQNAMEC)
#   define  nvsfccpk    FNAME(VSFCCPK)
#   define  nvsfncpk    FNAME(VSFNCPK)
#   define  nvdtrc      FNAME(VDTRC)
#   define  nvscfcls    FNAME(VSCFCLS)
#   define  nvscsetblsz FNAME(VSCSETBLSZ)
#   define  nvscsetnmbl FNAME (VSCSETNMBL)
#   define  nvscgblinfo FNAME(VSCGBLINFO)
#  else                                /* !sl */
#   define  ndfivopn FNAME(DFIVOPN)
#   define  ndfvclos FNAME(DFVCLOS)
#   define  nvatchc  FNAME(VATCHC)
#   define  nvdtchc  FNAME(VDTCHC)
#   define  nvgnamc  FNAME(VGNAMC)
#   define  nvgclsc  FNAME(VGCLSC)
#   define  nvinqc   FNAME(VINQC)
#   define  nvdeletec FNAME(VDELETEC)
#   define  nvgidc   FNAME(VGIDC)
#   define  nvgnxtc  FNAME(VGNXTC)
#   define  nvsnamc  FNAME(VSNAMC)
#   define  nvsclsc  FNAME(VSCLSC)
#   define  nvinsrtc FNAME(VINSRTC)
#   define  nvisvgc  FNAME(VISVGC)
#   define  nvfistart FNAME(VFISTART)
#   define  nvfiend  FNAME(VFIEND)
#   define  nvisvsc  FNAME(VISVSC)
#   define  nvsatchc FNAME(VSATCHC)
#   define  nvsdtchc FNAME(VSDTCHC)
#   define  nvsiqref FNAME(VSIQREF)
#   define  nvsiqtag FNAME(VSIQTAG)
#   define  nvsigver FNAME(VSIGVER)
#   define  nvsseekc FNAME(VSSEEKC)
#   define  nvsgnamc FNAME(VSGNAMC)
#   define  nvsgclsc FNAME(VSGCLSC)
#   define  nvsinqc  FNAME(VSINQC)
#   define  nvsfexc  FNAME(VSFEXC)
#   define  nvsfndc  FNAME(VSFNDC)
#   define  nvsgidc  FNAME(VSGIDC)
#   define  nvsdltc  FNAME(VSDLTC)
#   define  nvsapp   FNAME(VSAPP)
#   define  nvssnamc FNAME(VSSNAMC)
#   define  nvssclsc FNAME(VSSCLSC)
#   define  nvssfldc FNAME(VSSFLDC)
#   define  nvssintc FNAME(VSSINTC)
#   define  nvsfdefc FNAME(VSFDEFC)
#   define  nvssextfc FNAME(VSSEXTFC)
#   define  nvfinflds FNAME(VFINFLDS)
#   define  nvfifnm  FNAME(VFIFNM)
#   define  nvfiftp  FNAME(VFIFTP)
#   define  nvfifisz FNAME(VFIFISZ)
#   define  nvfifesz FNAME(VFIFESZ)
#   define  nvfifodr FNAME(VFIFODR)
#   define  nvsfirdc FNAME(VSFIRDC)
#   define  nvsfird  FNAME(VSFIRD)
#   define  nvsreadc FNAME(VSREADC)
#   define  nvsfiwrc FNAME(VSFIWRC)
#   define  nvsfiwr  FNAME(VSFIWR)
#   define  nvswritc FNAME(VSWRITC)
#   define  nvsgintc FNAME(VSGINTC)
#   define  nvseltsc FNAME(VSELTSC)
#   define  nvsgfldc FNAME(VSGFLDC)
#   define  nvssizc  FNAME(VSSIZC)
#   define  nventsc  FNAME(VENTSC)
#   define  nvlonec  FNAME(VLONEC)
#   define  nvslonec FNAME(VSLONEC)
#   define  nvfindc  FNAME(VFINDC)
#   define  nvfndclsc FNAME(VFNDCLSC)
#   define  nvhsdc   FNAME(VHSDC)
#   define  nvhscdc  FNAME(VHSCDC)
#   define  nvhscdmc FNAME(VHSCDMC)
#   define  nvhsdmc  FNAME(VHSDMC)
#   define  nvhmkgpc FNAME(VHMKGPC)
#   define  nvflocc  FNAME(VFLOCC)
#   define  nvfirefs FNAME(VFIREFS)
#   define  nvfiqref  FNAME(VFIQREF)
#   define  nvfiqtag  FNAME(VFIQTAG)
#   define  nvinqtrc FNAME(VINQTRC)
#   define  nvntrc   FNAME(VNTRC)
#   define  nvgttrsc FNAME(VGTTRSC)
#   define  nvgttrc  FNAME(VGTTRC)
#   define  nvadtrc  FNAME(VADTRC)
#   define  nvsiqintr FNAME(VSIQINTR)
#   define  nvsiqnelt FNAME(VSIQNELT)
#   define  nvsqfldsc   FNAME(VSQFLDSC)
#   define  nvsiqvsz    FNAME(VSIQVSZ)
#   define  nvsqnamec   FNAME(VSQNAMEC)
#   define  nvsfccpk    FNAME(VSFCCPK)
#   define  nvsfncpk    FNAME(VSFNCPK)
#   define  nvdtrc      FNAME(VDTRC)
#   define  nvscfcls    FNAME(VSCFCLS)
#   define  nvscsetblsz FNAME(VSCSETBLSZ)
#   define  nvscsetnmbl FNAME (VSCSETNMBL)
#   define  nvscgblinfo FNAME(VSCGBLINFO)
#  endif
#else                           /* !DF_CAPFNAMES */
# if defined(UNIX386) || (!(defined INTEL86) && !(defined WIN32))
#   define  ndfivopn FNAME(dfivopn)
#   define  ndfvclos FNAME(dfvclos)
#   define  nvatchc  FNAME(vatchc)
#   define  nvdtchc  FNAME(vdtchc)
#   define  nvgnamc  FNAME(vgnamc)
#   define  nvgclsc  FNAME(vgclsc)
#   define  nvinqc   FNAME(vinqc)
#   define  nvdeletec FNAME(vdeletec)
#   define  nvgidc   FNAME(vgidc)
#   define  nvgnxtc  FNAME(vgnxtc)
#   define  nvsnamc  FNAME(vsnamc)
#   define  nvsclsc  FNAME(vsclsc)
#   define  nvinsrtc FNAME(vinsrtc)
#   define  nvisvgc  FNAME(visvgc)
#   define  nvisvsc  FNAME(visvsc)
#   define  nvsatchc FNAME(vsatchc)
#   define  nvsdtchc FNAME(vsdtchc)
#   define  nvsqref  FNAME(vsqref)
#   define  nvsqtag  FNAME(vsqtag)
#   define  nvsgver  FNAME(vsgver)
#   define  nvsseekc FNAME(vsseekc)
#   define  nvsgnamc FNAME(vsgnamc)
#   define  nvsgclsc FNAME(vsgclsc)
#   define  nvsinqc  FNAME(vsinqc)
#   define  nvsfexc  FNAME(vsfexc)
#   define  nvsfndc  FNAME(vsfndc)
#   define  nvsgidc  FNAME(vsgidc)
#   define  nvsdltc  FNAME(vsdltc)
#   define  nvsapp   FNAME(vsapp)
#   define  nvssnamc FNAME(vssnamc)
#   define  nvssclsc FNAME(vssclsc)
#   define  nvssfldc FNAME(vssfldc)
#   define  nvssintc FNAME(vssintc)
#   define  nvsfdefc FNAME(vsfdefc)
#   define  nvssextfc FNAME(vssextfc)
#   define  nvfnflds FNAME(vfnflds)
#   define  nvffnamec FNAME(vffnamec)
#   define  nvfftype FNAME(vfftype)
#   define  nvffisiz FNAME(vffisiz)
#   define  nvffesiz FNAME(vffesiz)
#   define  nvffordr FNAME(vffordr)
#   define  nvsfrdc  FNAME(vsfrdc)
#   define  nvsfrd   FNAME(vsfrd)
#   define  nvsreadc FNAME(vsreadc)
#   define  nvsfwrtc FNAME(vsfwrtc)
#   define  nvsfwrt  FNAME(vsfwrt)
#   define  nvswritc FNAME(vswritc)
#   define  nvsgintc FNAME(vsgintc)
#   define  nvseltsc FNAME(vseltsc)
#   define  nvsgfldc FNAME(vsgfldc)
#   define  nvssizc  FNAME(vssizc)
#   define  nventsc  FNAME(ventsc)
#   define  nvlonec  FNAME(vlonec)
#   define  nvslonec FNAME(vslonec)
#   define  nvfindc  FNAME(vfindc)
#   define  nvfndclsc FNAME(vfndclsc)
#   define  nvhscdc  FNAME(vhscdc)
#   define  nvhsdc   FNAME(vhsdc)
#   define  nvhscdmc  FNAME(vhscdmc)
#   define  nvhsdmc  FNAME(vhsdmc)
#   define  nvhmkgpc FNAME(vhmkgpc)
#   define  nvflocc  FNAME(vflocc)
#   define  nvinqtrc FNAME(vinqtrc)
#   define  nvntrc   FNAME(vntrc)
#   define  nvnrefs  FNAME(vnrefs)
#   define  nvgttrsc FNAME(vgttrsc)
#   define  nvqref   FNAME(vqref)
#   define  nvqtag   FNAME(vqtag)
#   define  nvgttrc  FNAME(vgttrc)
#   define  nvadtrc  FNAME(vadtrc)
#   define  nvfstart FNAME(vfstart)
#   define  nvfend   FNAME(vfend)
#   define  nvsqfnelt   FNAME(vsqfnelt)
#   define  nvsqfintr   FNAME(vsqfintr)
#   define  nvsqfldsc   FNAME(vsqfldsc)
#   define  nvsqfvsiz   FNAME(vsqfvsiz)
#   define  nvsqnamec   FNAME(vsqnamec)
#   define  nvsfccpk    FNAME(vsfccpk)
#   define  nvsfncpk    FNAME(vsfncpk)
#   define  nvdtrc      FNAME(vdtrc)
#   define  nvscfcls    FNAME(vscfcls)
#   define  nvscsetblsz FNAME(vscsetblsz)
#   define  nvscsetnmbl FNAME (vscsetnmbl)
#   define  nvscgblinfo FNAME(vscgblinfo)
#  else                            /* !sl */
#   define  ndfivopn FNAME(dfivopn)
#   define  ndfvclos FNAME(dfvclos)
#   define  nvatchc  FNAME(vatchc)
#   define  nvdtchc  FNAME(vdtchc)
#   define  nvgnamc  FNAME(vgnamc)
#   define  nvgclsc  FNAME(vgclsc)
#   define  nvinqc   FNAME(vinqc)
#   define  nvdeletec FNAME(vdeletec)
#   define  nvgidc   FNAME(vgidc)
#   define  nvgnxtc  FNAME(vgnxtc)
#   define  nvsnamc  FNAME(vsnamc)
#   define  nvsclsc  FNAME(vsclsc)
#   define  nvinsrtc FNAME(vinsrtc)
#   define  nvisvgc  FNAME(visvgc)
#   define  nvfistart FNAME(vfistart)
#   define  nvfiend  FNAME(vfiend)
#   define  nvisvsc  FNAME(visvsc)
#   define  nvsatchc FNAME(vsatchc)
#   define  nvsdtchc FNAME(vsdtchc)
#   define  nvsiqref FNAME(vsiqref)
#   define  nvsiqtag FNAME(vsiqtag)
#   define  nvsigver FNAME(vsigver)
#   define  nvsseekc FNAME(vsseekc)
#   define  nvsgnamc FNAME(vsgnamc)
#   define  nvsgclsc FNAME(vsgclsc)
#   define  nvsinqc  FNAME(vsinqc)
#   define  nvsfexc  FNAME(vsfexc)
#   define  nvsfndc  FNAME(vsfndc)
#   define  nvsgidc  FNAME(vsgidc)
#   define  nvsdltc  FNAME(vsdltc)
#   define  nvsapp   FNAME(vsapp)
#   define  nvssnamc FNAME(vssnamc)
#   define  nvssclsc FNAME(vssclsc)
#   define  nvssfldc FNAME(vssfldc)
#   define  nvssintc FNAME(vssintc)
#   define  nvsfdefc FNAME(vsfdefc)
#   define  nvssextfc FNAME(vssextfc)
#   define  nvfinflds FNAME(vfinflds)
#   define  nvfifnm  FNAME(vfifnm)
#   define  nvfiftp  FNAME(vfiftp)
#   define  nvfifisz FNAME(vfifisz)
#   define  nvfifesz FNAME(vfifesz)
#   define  nvfifodr FNAME(vfifodr)
#   define  nvsfirdc FNAME(vsfirdc)
#   define  nvsfird  FNAME(vsfird)
#   define  nvsreadc FNAME(vsreadc)
#   define  nvsfiwrc FNAME(vsfiwrc)
#   define  nvsfiwr  FNAME(vsfiwr)
#   define  nvswritc FNAME(vswritc)
#   define  nvsgintc FNAME(vsgintc)
#   define  nvseltsc FNAME(vseltsc)
#   define  nvsgfldc FNAME(vsgfldc)
#   define  nvssizc  FNAME(vssizc)
#   define  nventsc  FNAME(ventsc)
#   define  nvlonec  FNAME(vlonec)
#   define  nvslonec FNAME(vslonec)
#   define  nvfindc  FNAME(vfindc)
#   define  nvfndclsc FNAME(vfndclsc)
#   define  nvhsdc   FNAME(vhsdc)
#   define  nvhscdc  FNAME(vhscdc)
#   define  nvhscdmc FNAME(vhscdmc)
#   define  nvhsdmc  FNAME(vhsdmc)
#   define  nvhmkgpc FNAME(vhmkgpc)
#   define  nvflocc  FNAME(vflocc)
#   define  nvfirefs FNAME(vfirefs)
#   define  nvfiqref  FNAME(vfiqref)
#   define  nvfiqtag  FNAME(vfiqtag)
#   define  nvinqtrc FNAME(vinqtrc)
#   define  nvntrc   FNAME(vntrc)
#   define  nvgttrsc FNAME(vgttrsc)
#   define  nvgttrc  FNAME(vgttrc)
#   define  nvadtrc  FNAME(vadtrc)
#   define  nvsiqintr FNAME(vsiqintr)
#   define  nvsiqnelt FNAME(vsiqnelt)
#   define  nvsqfldsc   FNAME(vsqfldsc)
#   define  nvsiqvsz   FNAME(vsiqvsz)
#   define  nvsqnamec   FNAME(vsqnamec)
#   define  nvsfccpk    FNAME(vsfccpk)
#   define  nvsfncpk    FNAME(vsfncpk)
#   define  nvdtrc      FNAME(vdtrc)
#   define  nvscfcls    FNAME(vscfcls)
#   define  nvscsetblsz FNAME(vscsetblsz)
#   define  nvscsetnmbl FNAME (vscsetnmbl)
#   define  nvscgblinfo FNAME(vscgblinfo)
#  endif
#endif                          /* DF_CAPFNAMES */
#endif                          /* VG_FNAMES */

    HDFLIBAPI      FRETVAL(intf) ndfivopn
                (_fcd filename, intf  * acc_mode, intf  * defdds, intf  * namelen);

    HDFLIBAPI      FRETVAL(intf) ndfvclos
                (intf  * file_id);

    HDFLIBAPI      FRETVAL(intf) nvatchc
                (intf  * f, intf  * vgid, _fcd accesstype);

    HDFLIBAPI      FRETVAL(intf) nvdtchc
                (intf  * vkey);

    HDFLIBAPI      FRETVAL(intf) nvgnamc
                (intf  * vkey, _fcd vgname);

    HDFLIBAPI      FRETVAL(intf) nvgclsc
                (intf  * vkey, _fcd vgclass);

    HDFLIBAPI      FRETVAL(intf) nvinqc
                (intf  * vkey, intf  * nentries, _fcd vgname);

    HDFLIBAPI      FRETVAL(intf) nvdeletec
                (intf  *f, intf  * vkey);

    HDFLIBAPI      FRETVAL(intf) nvgidc
                (intf  * f, intf  * vgid);

    HDFLIBAPI      FRETVAL(intf) nvgnxtc
                (intf  * vkey, intf  * id);

    HDFLIBAPI      FRETVAL(intf) nvsnamc
                (intf  * vkey, _fcd vgname, intf  * vgnamelen);

    HDFLIBAPI      FRETVAL(intf) nvsclsc
                (intf  * vkey, _fcd vgclass, intf  * vgclasslen);

    HDFLIBAPI      FRETVAL(intf) nvinsrtc
                (intf  * vkey, intf  * vobjptr);

    HDFLIBAPI      FRETVAL(intf) nvisvgc
                (intf  * vkey, intf  * id);

    HDFLIBAPI      FRETVAL(intf) nvfstart
                (intf  * f);

    HDFLIBAPI      FRETVAL(intf) nvfend
                (intf  * f);

    HDFLIBAPI      FRETVAL(intf) nvisvsc
                (intf  * vkey, intf  * id);

    HDFLIBAPI      FRETVAL(intf) nvsatchc
                (intf  * f, intf  * vsref, _fcd accesstype);

    HDFLIBAPI      FRETVAL(intf) nvsdtchc
                (intf  * vkey);

    HDFLIBAPI      FRETVAL(intf) nvsqref
                (intf  * vkey);

    HDFLIBAPI      FRETVAL(intf) nvsqtag
                (intf  * vkey);
  
    HDFLIBAPI      FRETVAL(intf) nvsqnfld
                (intf * vkey);

    HDFLIBAPI      FRETVAL(intf) nvsgver
                (intf  * vkey);

    HDFLIBAPI      FRETVAL(intf) nvsseekc
                (intf  * vkey, intf  * eltpos);

    HDFLIBAPI      FRETVAL(intf) nvsgnamc
                (intf  * vkey, _fcd vsname, intf *vsnamelen);

    HDFLIBAPI      FRETVAL(intf) nvsgclsc
                (intf  * vkey, _fcd vsclass, intf *vsclasslen);

    HDFLIBAPI      FRETVAL(intf) nvsinqc
                (intf  * vkey, intf  * nelt, intf  * interlace, _fcd fields,
		intf  * eltsize, _fcd vsname, intf *fieldslen, intf *vsnamelen);

    HDFLIBAPI      FRETVAL(intf) nvsfexc
                (intf  * vkey, _fcd fields, intf  * fieldslen);

    HDFLIBAPI      FRETVAL(intf) nvsfndc
                (intf  * f, _fcd name, intf  * namelen);

    HDFLIBAPI      FRETVAL(intf) nvsgidc
                (intf  * f, intf  * vsref);

    HDFLIBAPI      FRETVAL(intf) nvsdltc
                (intf  * f, intf  * vsref);

    HDFLIBAPI      FRETVAL(intf) nvsapp
                (intf  * vkey, intf  *blk);

    HDFLIBAPI      FRETVAL(intf) nvssnamc
                (intf  * vkey, _fcd vsname, intf  * vsnamelen);

    HDFLIBAPI      FRETVAL(intf) nvssclsc
                (intf  * vkey, _fcd vsclass, intf  * vsclasslen);

    HDFLIBAPI      FRETVAL(intf) nvssfldc
                (intf  * vkey, _fcd fields, intf  * fieldslen);

    HDFLIBAPI      FRETVAL(intf) nvssintc
                (intf  * vkey, intf  * interlace);

    HDFLIBAPI      FRETVAL(intf) nvsfdefc
                (intf  * vkey, _fcd field, intf  * localtype,
                 intf  * order, intf  * fieldlen);

    HDFLIBAPI      FRETVAL(intf) nvssextfc
                (intf  * vkey, _fcd fname, intf  * offset,
                 intf  * fnamelen);

    HDFLIBAPI      FRETVAL(intf) nvfnflds
                (intf  * vkey);

    HDFLIBAPI      FRETVAL(intf) nvffnamec
                (intf  * vkey, intf  *idx, _fcd fname, intf *len);

    HDFLIBAPI      FRETVAL(intf) nvfftype
                (intf  * vkey, intf  *idx);

    HDFLIBAPI      FRETVAL(intf) nvffisiz
                (intf  * vkey, intf  *idx);

    HDFLIBAPI      FRETVAL(intf) nvffesiz
                (intf  * vkey, intf  *idx);

    HDFLIBAPI      FRETVAL(intf) nvffordr
                (intf  * vkey, intf  *idx);

    HDFLIBAPI      FRETVAL(intf) nvsfrdc
                (intf  * vkey, _fcd  cbuf, intf  * nelt,
                 intf  * interlace);

    HDFLIBAPI      FRETVAL(intf) nvsfrd
                (intf  * vkey, intf  * buf, intf  * nelt,
                 intf  * interlace);

    HDFLIBAPI      FRETVAL(intf) nvsreadc
                (intf  * vkey, uint8  * buf, intf  * nelt,
                 intf  * interlace);

    HDFLIBAPI      FRETVAL(intf) nvsfwrtc
                (intf  * vkey, _fcd  cbuf, intf  * nelt,
                 intf  * interlace);

    HDFLIBAPI      FRETVAL(intf) nvsfwrt
                (intf  * vkey, intf  * buf, intf  * nelt,
                 intf  * interlace);

    HDFLIBAPI      FRETVAL(intf) nvswritc
                (intf  * vkey, uint8  * buf, intf  * nelt,
                 intf  * interlace);

    HDFLIBAPI      FRETVAL(intf) nvsgintc
                (intf  * vkey);

    HDFLIBAPI      FRETVAL(intf) nvseltsc
                (intf  * vkey);

    HDFLIBAPI      FRETVAL(intf) nvsgfldc
                (intf  * vkey, _fcd fields);

    HDFLIBAPI      FRETVAL(intf) nvssizc
                (intf  * vkey, _fcd fields, intf  * fieldslen);

    HDFLIBAPI      FRETVAL(intf) nventsc
                (intf  * f, intf  * vgid);

    HDFLIBAPI      FRETVAL(intf) nvlonec
                (intf  * f, intf  * idarray, intf  * asize);

    HDFLIBAPI      FRETVAL(intf) nvslonec
                (intf  * f, intf  * idarray, intf  * asize);

    HDFLIBAPI      FRETVAL(intf) nvfindc
                (intf  * f, _fcd name, intf  * namelen);

    HDFLIBAPI      FRETVAL(intf) nvfndclsc
                (intf  * f, _fcd vgclass, intf  * classlen);

    HDFLIBAPI      FRETVAL(intf) nvhscdc
                (intf  * f, _fcd field, _fcd  cbuf, intf  * n, 
                 intf  * datatype, _fcd vsname, _fcd vsclass, 
                 intf  * fieldlen, intf  * vsnamelen,
                 intf  * vsclasslen);

    HDFLIBAPI      FRETVAL(intf) nvhsdc
                (intf  * f, _fcd field, uint8  * buf, intf  * n, 
                 intf  * datatype, _fcd vsname, _fcd vsclass, 
                 intf  * fieldlen, intf  * vsnamelen,
                 intf  * vsclasslen);

    HDFLIBAPI      FRETVAL(intf) nvhscdmc
                (intf  * f, _fcd field, _fcd  cbuf, intf  * n,
                 intf  * datatype, _fcd vsname, _fcd vsclass, 
                 intf  * order, intf  * fieldlen, intf * vsnamelen,
                 intf  * vsclasslen);

    HDFLIBAPI      FRETVAL(intf) nvhsdmc
                (intf  * f, _fcd field, uint8  * buf, intf  * n,
                 intf  * datatype, _fcd vsname, _fcd vsclass, 
                 intf  * order, intf  * fieldlen, intf * vsnamelen,
                 intf  * vsclasslen);

    HDFLIBAPI      FRETVAL(intf) nvhmkgpc
                (intf  * f, intf  * tagarray, intf  * refarray, intf  * n,
                 _fcd vgname, _fcd vgclass, intf  * vgnamelen, intf  * vgclasslen);

    HDFLIBAPI      FRETVAL(intf) nvflocc
                (intf  * vkey, _fcd field, intf  * fieldlen);

    HDFLIBAPI      FRETVAL(intf) nvinqtrc
                (intf  * vkey, intf  * tag, intf  * ref);

    HDFLIBAPI      FRETVAL(intf) nvntrc
                (intf  * vkey);

    HDFLIBAPI      FRETVAL(intf) nvnrefs
                (intf  * vkey, intf  *tag);

    HDFLIBAPI      FRETVAL(intf) nvqref
                (intf  * vkey);

    HDFLIBAPI      FRETVAL(intf) nvqtag
                (intf  * vkey);

    HDFLIBAPI      FRETVAL(intf) nvgttrsc
                (intf  * vkey, intf  * tagarray, intf  * refarray, intf  * n);

    HDFLIBAPI      FRETVAL(intf) nvgttrc
                (intf  * vkey, intf  * which, intf  * tag, intf  * ref);

    HDFLIBAPI      FRETVAL(intf) nvadtrc
                (intf  * vkey, intf  * tag, intf  * ref);

    HDFLIBAPI      FRETVAL(intf) nvdtrc
                (intf  * vkey, intf  * tag, intf  * ref);

    HDFLIBAPI      FRETVAL(intf) nvsqfnelt
                (intf * vkey, intf * nelt);

    HDFLIBAPI      FRETVAL(intf) nvsqfintr
                (intf * vkey, intf * interlace);

    HDFLIBAPI      FRETVAL(intf) nvsqfldsc
                (intf * vkey, _fcd fields, intf *fieldslen);

    HDFLIBAPI      FRETVAL(intf) nvsqfvsiz
                (intf * vkey, intf * size);

    HDFLIBAPI      FRETVAL(intf) nvsqnamec  
                (intf * vkey, _fcd name, intf *namelen);

    HDFLIBAPI      FRETVAL(intf) nvsfccpk
                (intf *vs, intf *packtype, _fcd buflds, intf *buf, intf *bufsz,
        intf *nrecs, _fcd pckfld, _fcd fldbuf, intf *buflds_len, intf *fld_len);

    HDFLIBAPI      FRETVAL(intf) nvsfncpk
                (intf *vs, intf *packtype, _fcd buflds, intf *buf, intf *bufsz,
        intf *nrecs, _fcd pckfld, intf *fldbuf, intf *buflds_len, intf *fld_len);

    HDFLIBAPI      FRETVAL(intf) nvscsetblsz
                (intf *id, intf *block_size);

    HDFLIBAPI      FRETVAL(intf) nvscsetnmbl
                (intf *id, intf *num_blocks);

    HDFLIBAPI      FRETVAL(intf) nvscgblinfo
                (intf *id, intf *block_size, intf *num_blocks);

/* 
  ** from vattrf.c
 */
#ifndef VATTR_FNAMES
#  define VATTR_FNAMES
#ifdef DF_CAPFNAMES
# if defined(UNIX386) || (!(defined INTEL86) && !(defined WIN32))
#  define nvsfcfdx   FNAME(VSFCFDX)
#  define nvsfcsat   FNAME(VSFCSAT)
#  define nvsfcsca   FNAME(VSFCSCA)
#  define nvsfnats   FNAME(VSFNATS)
#  define nvsffnas   FNAME(VSFFNAS)
#  define nvsfcfda   FNAME(VSFCFDA)
#  define nvsfcain   FNAME(VSFCAIN)
#  define nvsfgnat   FNAME(VSFGNAT)
#  define nvsfgcat   FNAME(VSFGCAT)
#  define nvsfisat   FNAME(VSFISAT)
#  define nvfcsatt   FNAME(VFCSATT)
#  define nvfcscat   FNAME(VFCSCAT)
#  define nvfnatts   FNAME(VFNATTS)
#  define nvfcfdat   FNAME(VFCFDAT)
#  define nvfainfo   FNAME(VFAINFO)
#  define nvfgnatt   FNAME(VFGNATT)
#  define nvfgcatt   FNAME(VFGCATT)
#  define nvfgver    FNAME(VFGVER)
# else                          /* !sl */
#  define nvsfcfdx   FNAME(VSFCFDX)
#  define nvsfcsat   FNAME(VSFCSAT)
#  define nvsfcsca   FNAME(VSFCSCA)
#  define nvsfcnats  FNAME(VSFCNATS)
#  define nvsfcfnas  FNAME(VSFCFNAS)
#  define nvsfcfda   FNAME(VSFCFDA)
#  define nvsfcainf  FNAME(VSFCAINF)
#  define nvsfcgna   FNAME(VSFCGNA)
#  define nvsfcgca   FNAME(VSFCGCA)
#  define nvsfcisa   FNAME(VSFCISA)
#  define nvfcsatt   FNAME(VFCSATT)
#  define nvfcscat   FNAME(VFCSCAT)
#  define nvfcnats   FNAME(VFCNATS)
#  define nvfcfdat   FNAME(VFCFDAT)
#  define nvfcainf   FNAME(VFCAINF)
#  define nvfcgnat   FNAME(VFCGNAT)
#  define nvfcgcat   FNAME(VFCGCAT)
#  define nvfcgver   FNAME(VFCGVER)
# endif
#else       /* !DF_CAPFNAMES  */
# if defined(UNIX386) || (!(defined INTEL86) && !(defined WIN32))
#  define nvsfcfdx   FNAME(vsfcfdx)
#  define nvsfcsat   FNAME(vsfcsat)
#  define nvsfcsca   FNAME(vsfcsca)
#  define nvsfnats   FNAME(vsfnats)
#  define nvsffnas   FNAME(vsffnas)
#  define nvsfcfda   FNAME(vsfcfda)
#  define nvsfcain   FNAME(vsfcain)
#  define nvsfgnat   FNAME(vsfgnat)
#  define nvsfgcat   FNAME(vsfgcat)
#  define nvsfisat   FNAME(vsfisat)
#  define nvfcsatt   FNAME(vfcsatt)
#  define nvfcscat   FNAME(vfcscat)
#  define nvfnatts   FNAME(vfnatts)
#  define nvfcfdat   FNAME(vfcfdat)
#  define nvfainfo   FNAME(vfainfo)
#  define nvfgnatt   FNAME(vfgnatt)
#  define nvfgcatt   FNAME(vfgcatt)
#  define nvfgver    FNAME(vfgver)
# else                           /* !sl */
#  define nvsfcfdx   FNAME(vsfcfdx)
#  define nvsfcsat   FNAME(vsfcsat)
#  define nvsfcsca   FNAME(vsfcsca)
#  define nvsfcnats  FNAME(vsfcnats)
#  define nvsfcfnas  FNAME(vsfcfnas)
#  define nvsfcfda   FNAME(vsfcfda)
#  define nvsfcainf  FNAME(vsfcainf)
#  define nvsfcgna   FNAME(vsfcgna)
#  define nvsfcgca   FNAME(vsfcgca)
#  define nvsfcisa   FNAME(vsfcisa)
#  define nvfcsatt   FNAME(vfcsatt)
#  define nvfcscat   FNAME(vfcscat)
#  define nvfcnats   FNAME(vfcnats)
#  define nvfcfdat   FNAME(vfcfdat)
#  define nvfcainf   FNAME(vfcainf)
#  define nvfcgnat   FNAME(vfcgnat)
#  define nvfcgcat   FNAME(vfcgcat)
#  define nvfcgver   FNAME(vfcgver)
# endif
#endif   /* DF_CAPFNAMES */
#endif   /* VATTR_FNAMES  */
   HDFLIBAPI   FRETVAL(intf) nvsfcfdx
            (intf *vsid, _fcd fldnm, intf *findex, intf *fldnmlen);
   HDFLIBAPI   FRETVAL(intf) nvsfcsat
            (intf *vsid, intf *findex, _fcd attrnm, intf *dtype,
             intf *count, intf *values, intf *attrnmlen);
   HDFLIBAPI   FRETVAL(intf) nvsfcsca
            (intf *vsid, intf *findex, _fcd attrnm, intf *dtype,
             intf *count, _fcd values, intf *attrnmlen);
   HDFLIBAPI   FRETVAL(intf) nvsfnats
            (intf *vsid);
   HDFLIBAPI   FRETVAL(intf) nvsffnas
            (intf *vsid, intf *findex);
   HDFLIBAPI   FRETVAL(intf) nvsfcfda
            (intf *vsid, intf *findex, _fcd attrnm, intf *attrnmlen);
   HDFLIBAPI   FRETVAL(intf) nvsfcain
            (intf *vsid, intf *findex, intf *aindex, _fcd attrname,
             intf *dtype, intf *count, intf *size, intf *attrnamelen);
   HDFLIBAPI   FRETVAL(intf) nvsfgnat
            (intf *vsid, intf *findex, intf *aindex, intf *values);
   HDFLIBAPI   FRETVAL(intf) nvsfgcat
            (intf *vsid, intf *findex, intf *aindex, _fcd values);
   HDFLIBAPI   FRETVAL(intf) nvsfisat
            (intf *vsid);
   HDFLIBAPI   FRETVAL(intf) nvfcsatt
            (intf *vgid, _fcd attrnm, intf *dtype,
             intf *count, intf *values, intf *attrnmlen);
   HDFLIBAPI   FRETVAL(intf) nvfcscat
            (intf *vgid, _fcd attrnm, intf *dtype, intf *count,
             _fcd values, intf *attrnmlen);
   HDFLIBAPI   FRETVAL(intf) nvfnatts
            (intf *vgid);
   HDFLIBAPI   FRETVAL(intf) nvfcfdat
            (intf *vgid, _fcd attrnm, intf *attrnmlen);
   HDFLIBAPI   FRETVAL(intf) nvfainfo
            (intf *vgid, intf *aindex, _fcd attrname,
             intf *dtype, intf *count, intf *size);
   HDFLIBAPI   FRETVAL(intf) nvfgnatt
            (intf *vgid, intf *aindex, intf *values);
   HDFLIBAPI   FRETVAL(intf) nvfgcatt
            (intf *vgid, intf *aindex, _fcd values);
   HDFLIBAPI   FRETVAL(intf) nvfgver
            (intf *vgid);

#if defined c_plusplus || defined __cplusplus
}
#endif                          /* c_plusplus || __cplusplus */

#endif                          /* _H_PROTO */

