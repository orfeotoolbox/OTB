/* SCCS_ID[] = @(#) vpfio.h 1.6 8/13/91  */

/* ========================================================================

   Environmental Systems Research Institute (ESRI) Applications Programming

       Project: 		Conversion from ARC/INFO to VPF 
       Original Coding:		David Flinn     July 1991
       Modifications:	      

   ======================================================================== */

#ifndef _VPF_IO_
#define _VPF_IO_

#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif
#include <ossim/base/ossimConstants.h>
/* These are all the metacharacters used to parse out input text files */

#define	 	COMPONENT_SEPERATOR	';'
#define		LINE_CONTINUE		'\\'
#define		SPACE			' '
#define		TEXT_NULL		"-"
#define		COMMENT_CHAR		'"'
#define		FIELD_COUNT		'='
#define		FIELD_SEPERATOR		','
#define		END_OF_FIELD		':'
#define		FIELD_PARTITION		'^'
#define		NEXT_ELEMENT		'|'
#define		COMMENT			'#'
#define		NEW_LINE		'\n'
#define		VARIABLE_COUNT		'*'
#define		TAB			'\t'

/* Now set up the macros to read in data from disk and to write */

/* #if __MSDOS__ */
#if 0

#define   Read_Vpf_Char(tobuffer,fromtable,count)\
	     fread(tobuffer,sizeof(char),count,fromtable)

#define   Read_Vpf_Short(tobuffer,fromtable,count)\
	     fread(tobuffer,sizeof(short int),count,fromtable)

#define   Read_Vpf_Int(tobuffer,fromtable,count)\
	     fread(tobuffer,sizeof(ossim_int32),count,fromtable)

#define   Read_Vpf_Float(tobuffer,fromtable,count)\
	     fread(tobuffer,sizeof(float),count,fromtable)

#define   Read_Vpf_Double(tobuffer,fromtable,count)\
	     fread(tobuffer,sizeof(double),count,fromtable)

#define   Read_Vpf_Date(tobuffer,fromtable,count)\
	     fread(tobuffer,sizeof(date_type)-1,count,fromtable)

#define   Read_Vpf_Coordinate(tobuffer,fromtable,count)\
	     fread(tobuffer,sizeof(coordinate_type),count,fromtable)

#define   Read_Vpf_CoordinateZ(tobuffer,fromtable,count)\
	     fread(tobuffer,sizeof(tri_coordinate_type),count,fromtable)

#define   Read_Vpf_DoubleCoordinate(tobuffer,fromtable,count)\
	     fread(tobuffer,sizeof(double_coordinate_type),count,fromtable)

#define   Read_Vpf_DoubleCoordinateZ(tobuffer,fromtable,count)\
	     fread(tobuffer,sizeof(double_tri_coordinate_type), \
	     count,fromtable)

#define   Write_Vpf_Char(tobuffer,fromfile,count)\
	     fwrite(tobuffer,sizeof(char),count,fromfile)

#define   Write_Vpf_Short(tobuffer,fromfile,count)\
	     fwrite(tobuffer,sizeof(short int),count,fromfile)

#define   Write_Vpf_Int(tobuffer,fromfile,count)\
	     fwrite(tobuffer,sizeof(ossim_int32),count,fromfile)

#define   Write_Vpf_Float(tobuffer,fromfile,count)\
	     fwrite(tobuffer,sizeof(float),count,fromfile)

#define   Write_Vpf_Double(tobuffer,fromfile,count)\
	     fwrite(tobuffer,sizeof(double),count,fromfile)

#define   Write_Vpf_Date(tobuffer,fromfile,count)\
	     fwrite(tobuffer,sizeof(char)*20,count,fromfile)

#define   Write_Vpf_Coordinate(tobuffer,fromfile,count)\
	     fwrite(tobuffer,sizeof(float)*2,count,fromfile)

#define   Write_Vpf_CoordinateZ(tobuffer,fromfile,count)\
	     fwrite(tobuffer,sizeof(float)*3,count,fromfile)

#define   Write_Vpf_DoubleCoordinate(tobuffer,fromfile,count)\
	     fwrite(tobuffer,sizeof(double)*2,count,fromfile)

#define   Write_Vpf_DoubleCoordinateZ(tobuffer,fromfile,count)\
	     fwrite(tobuffer,sizeof(double)*3,count,fromfile)

#else   /* Now the UNIX subroutine calls */


/* types */
typedef enum {
  VpfNull,
  VpfChar,
  VpfShort,
  VpfInteger,
  VpfFloat,
  VpfDouble,
  VpfDate,
  VpfKey,
  VpfCoordinate,
  VpfTriCoordinate,
  VpfDoubleCoordinate,
  VpfDoubleTriCoordinate,
  VpfUndefined
} VpfDataType ;

#define   Read_Vpf_Char(tobuffer,fromfile,count)\
	     VpfRead(tobuffer,VpfChar,count,fromfile)

#define   Read_Vpf_Short(tobuffer,fromfile,count)\
	     VpfRead(tobuffer,VpfShort,count,fromfile)

#define   Read_Vpf_Int(tobuffer,fromfile,count)\
	     VpfRead(tobuffer,VpfInteger,count,fromfile)

#define   Read_Vpf_Float(tobuffer,fromfile,count)\
	     VpfRead(tobuffer,VpfFloat,count,fromfile)

#define   Read_Vpf_Double(tobuffer,fromfile,count)\
	     VpfRead(tobuffer,VpfDouble,count,fromfile)

#define   Read_Vpf_Date(tobuffer,fromfile,count)\
	     VpfRead(tobuffer,VpfDate,count,fromfile)

#define   Read_Vpf_Coordinate(tobuffer,fromfile,count)\
	     VpfRead(tobuffer,VpfCoordinate,count,fromfile)

#define   Read_Vpf_CoordinateZ(tobuffer,fromfile,count)\
	     VpfRead(tobuffer,VpfTriCoordinate,count,fromfile)

#define   Read_Vpf_DoubleCoordinate(tobuffer,fromfile,count)\
	     VpfRead(tobuffer,VpfDoubleCoordinate,count,fromfile)

#define   Read_Vpf_DoubleCoordinateZ(tobuffer,fromfile,count)\
	     VpfRead(tobuffer,VpfDoubleTriCoordinate,count,fromfile)

#define   Write_Vpf_Char(tobuffer,fromfile,count)\
	     VpfWrite(tobuffer,VpfChar,count,fromfile)

#define   Write_Vpf_Short(tobuffer,fromfile,count)\
	     VpfWrite(tobuffer,VpfShort,count,fromfile)

#define   Write_Vpf_Int(tobuffer,fromfile,count)\
	     VpfWrite(tobuffer,VpfInteger,count,fromfile)

#define   Write_Vpf_Float(tobuffer,fromfile,count)\
	     VpfWrite(tobuffer,VpfFloat,count,fromfile)

#define   Write_Vpf_Double(tobuffer,fromfile,count)\
	     VpfWrite(tobuffer,VpfDouble,count,fromfile)

#define   Write_Vpf_Date(tobuffer,fromfile,count)\
	     VpfWrite(tobuffer,VpfDate,count,fromfile)

#define   Write_Vpf_Coordinate(tobuffer,fromfile,count)\
	     VpfWrite(tobuffer,VpfCoordinate,count,fromfile)

#define   Write_Vpf_CoordinateZ(tobuffer,fromfile,count)\
	     VpfWrite(tobuffer,VpfTriCoordinate,count,fromfile)

#define   Write_Vpf_DoubleCoordinate(tobuffer,fromfile,count)\
	     VpfWrite(tobuffer,VpfDoubleCoordinate,count,fromfile)

#define   Write_Vpf_DoubleCoordinateZ(tobuffer,fromfile,count)\
	     VpfWrite(tobuffer,VpfDoubleTriCoordinate,count,fromfile)

/*  subroutines */

ossim_int32 VpfRead ( void *to, VpfDataType type, ossim_int32 count, FILE *from ) ;

ossim_int32 VpfWrite( void *from, VpfDataType type, ossim_int32 count, FILE *to );

#ifdef __cplusplus
}
#endif

#endif          /* if __MSDOS__    */
#endif		/* ifndef _VPF_IO_ */








