/******************************************************************************
 * $Id: liblas.h 888 2008-09-27 23:12:14Z hobu $
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  Main prototypes for the libLAS C API
 * Author:   Howard Butler, hobu.inc@gmail.com
 *
 ******************************************************************************
 * Copyright (c) 2008, Howard Butler
 *
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following 
 * conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright 
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright 
 *       notice, this list of conditions and the following disclaimer in 
 *       the documentation and/or other materials provided 
 *       with the distribution.
 *     * Neither the name of the Martin Isenburg or Iowa Department 
 *       of Natural Resources nor the names of its contributors may be 
 *       used to endorse or promote products derived from this software 
 *       without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 ****************************************************************************/
 
#ifndef LIBLAS_H_INCLUDED
#define LIBLAS_H_INCLUDED



#define LIBLAS_C_API 1

#include "las_version.h"
#include "las_config.h"


#include <liblas/cstdint.hpp>


typedef struct LASWriterHS *LASWriterH;
typedef struct LASReaderHS *LASReaderH;
typedef struct LASPointHS *LASPointH;
typedef struct LASHeaderHS *LASHeaderH;
typedef struct LASGuidHS *LASGuidH;
typedef struct LASVLRHS *LASVLRH;


LAS_C_START


#define LAS_MODE_READ 0
#define LAS_MODE_WRITE 1
#define LAS_MODE_APPEND 2

/**
 * \todo to be documented
 */
typedef enum
{
    LE_None = 0,
    LE_Debug = 1,
    LE_Warning = 2,
    LE_Failure = 3,
    LE_Fatal = 4
} LASError;

/**
 * \todo to be documented
 */
typedef struct  {

    double t;
    double x, y, z;
    uint16_t intensity;
    uint8_t cls;
    int8_t scan_angle;
    uint8_t user_data;
    uint16_t retnum;
    uint16_t numret;
    uint16_t scandir;
    uint16_t fedge;
    long rgpsum;    
    int number_of_point_records;
    int number_of_points_by_return[8];
    int number_of_returns_of_given_pulse[8];
    int classification[32];
    int classification_synthetic;
    int classification_keypoint;
    int classification_withheld;
    LASPointH pmax;
    LASPointH pmin;
} LASPointSummary;


/** Returns the version string for this library.
 *  @return the version string for this library.
*/
LAS_DLL char* LAS_GetVersion();

/****************************************************************************/
/* Error handling                                                           */
/****************************************************************************/

/** Resets the error stack for the libLAS C API.  
*/
LAS_DLL void LASError_Reset(void);

/** Pops the top error off of the error stack for the libLAS C API.
*/
LAS_DLL void LASError_Pop(void);

/** Returns the error number of the last error on the error stack.
 *  @return the error number of the last error on the error stack.
*/
LAS_DLL LASError LASError_GetLastErrorNum(void);

/** Returns the name of the method the last error message happened in.
 *  @return the name of the method the last error message happened in.
*/
LAS_DLL char * LASError_GetLastErrorMsg(void);

/** Returns the name of the method the last error message happened in.
 *  @return the name of the method the last error message happened in.
*/
LAS_DLL char * LASError_GetLastErrorMethod(void);

/** Returns the number of error messages on the error stack.
 *  @return the number of error messages on the error stack.
*/
LAS_DLL int LASError_GetErrorCount(void);

/** Prints the last error message in the error stack to stderr.  If 
 *  there is no error on the error stack, only the message is printed.
 *  The function does not alter the error stack in any way.
 *  @param message Message to include in the stderr output
*/
LAS_DLL void LASError_Print(const char* message);

/****************************************************************************/
/* Reader operations                                                        */
/****************************************************************************/

/** Creates a LASReaderH object that can be used to read LASHeaderH and 
 *  LASPointH objects with.  The LASReaderH must not be created with a 
 *  filename that is opened for read or write by any other API functions.
 *  @return opaque pointer to a LASReaderH instance.
 *  @param filename Filename to open for read 
*/
LAS_DLL LASReaderH LASReader_Create(const char * filename);

/** Reads the next available point on the LASReaderH instance.  If no point 
 *  is available to read, NULL is returned.  If an error happens during 
 *  the reading of the next available point, an error will be added to the 
 *  error stack and can be returned with the LASError_GetLastError* methods.
 *  @param hReader the opaque handle to the LASReaderH 
 *  @return an opaque handle to a LASPointH object, or NULL if no point is 
 *  available to read or an error occured.  Use the 
 *  LASError_GetLastError* methods to confirm the existence of an error 
 *  if NULL is returned.
*/
LAS_DLL LASPointH LASReader_GetNextPoint(const LASReaderH hReader);

/** Reads a LASPointH from the given position in the LAS file represented 
 *  by the LASReaderH instance.  If no point is available at that location, 
 *  NULL is returned.  If an error happens during the reading of the point, 
 *  an error will be added to the error stack and can be returned with the 
 *  LASError_GetLastError* methods.
 *  @param hReader the opaque handle to the LASReaderH
 *  @param position the integer position of the point in the file to read.
 *  @return an opaque handle to a LASPointH object, or NULL if no point is 
 *  available at the given location or an error occured.  Use the 
 *  LASError_GetLastError* methods to confirm the existence of an error 
 *  if NULL is returned.
*/
LAS_DLL LASPointH LASReader_GetPointAt(const LASReaderH hReader, uint32_t position);

/** Closes the file for reading operations represented by the LASReaderH instance.
 *  @param hReader the opqaue handle to the LASReaderH
*/
LAS_DLL void LASReader_Destroy(LASReaderH hReader);

/** Returns a LASHeaderH representing the header for the file
 *  @param hReader the LASReaderH instance
 *  @return a LASHeaderH representing the header for the file.  NULL is returned 
 *  in the event of an error.  Use the LASError_GetLastError* methods to check
 *  in the event of a NULL return.
*/
LAS_DLL LASHeaderH LASReader_GetHeader(const LASReaderH hReader);

/****************************************************************************/
/* Point operations                                                         */
/****************************************************************************/

/** Returns the X value for the point.  This value is not scaled or offset
 *  by any header values and stands on its own.  If you need points to have 
 *  a scale and/or offset applied, this must be done in conjunction with the 
 *  header values after the value is read.
 *  @param hPoint the opaque pointer to the LASPointH instance  
 *  @return the X value for the LASPointH
*/
LAS_DLL double LASPoint_GetX(const LASPointH hPoint);

/** Sets the X value for the point.  This value must be scaled or offset 
 *  by any header values before being set.
 *  @param hPoint the opaque pointer to the LASPointH instance
 *  @param value the double value to set for the X value of the point
 *  @return an error number if an error occured during the setting of the point.
*/
LAS_DLL LASError LASPoint_SetX(LASPointH hPoint, double value);

/** Returns the Y value for the point.  This value is not scaled or offset
 *  by any header values and stands on its own.  If you need points to have 
 *  a scale and/or offset applied, this must be done in conjunction with the 
 *  header values after the value is read.
 *  @param hPoint the opaque pointer to the LASPointH instance  
 *  @return the Y value for the LASPointH
*/
LAS_DLL double LASPoint_GetY(const LASPointH hPoint);

/** Sets the Y value for the point.  This value must be scaled or offset 
 *  by any header values before being set.
 *  @param hPoint the opaque pointer to the LASPointH instance
 *  @param value the double value to set for the Y value of the point
 *  @return an error number if an error occured during the setting of the point.
*/
LAS_DLL LASError LASPoint_SetY(LASPointH hPoint, double value);

/** Returns the Z value for the point.  This value is not scaled or offset
 *  by any header values and stands on its own.  If you need points to have 
 *  a scale and/or offset applied, this must be done in conjunction with the 
 *  header values after the value is read.
 *  @param hPoint the opaque pointer to the LASPointH instance  
 *  @return the Z value for the LASPointH
*/
LAS_DLL double LASPoint_GetZ(const LASPointH hPoint);

/** Sets the Z value for the point.  This value must be scaled or offset 
 *  by any header values before being set.
 *  @param hPoint the opaque pointer to the LASPointH instance
 *  @param value the double value to set for the Z value of the point
 *  @return an error number if an error occured during the setting of the point.
*/
LAS_DLL LASError LASPoint_SetZ(LASPointH hPoint, double value);

/** Returns the intensity value for the point.  This value is the pulse return 
 *  magnitude, it is optional, and it is LiDAR system specific.
 *  @return the intensity value for the point.
*/
LAS_DLL uint16_t LASPoint_GetIntensity(const LASPointH hPoint);

/** Sets the intensity value for the point.
 *  @param hPoint the opaque pointer to the LASPointH instance
 *  @param value the value to set the intensity to
 *  @return an error number if an error occured.
*/
LAS_DLL LASError LASPoint_SetIntensity(LASPointH hPoint, uint16_t value);

/** Returns the return number for the point.  The return number is "the pulse
 *  return number for a given output pulse."  The first return number starts with
 *  the value 1.
 *  @param hPoint LASPointH instance
 *  @return a return number, valid from 1-6, for the point.  Use the LASError 
 *  methods to determine if an error occurred during this operation if 0 
 *  is returned.
*/
LAS_DLL uint16_t LASPoint_GetReturnNumber(const LASPointH hPoint);

/** Sets the return number for the point.  Valid values are from 1-6.
 *  @param hPoint LASPointH instance
 *  @param value the value to set for the return number
 *  @return LASError value determine success or failure.
*/
LAS_DLL LASError LASPoint_SetReturnNumber(LASPointH hPoint, uint16_t value);

/** Returns the total number of returns for a given pulse.
 *  @param hPoint LASPointH instance
 *  @return total number of returns for this pulse.
*/
LAS_DLL uint16_t LASPoint_GetNumberOfReturns(const LASPointH hPoint);

/** Sets the number of returns for the point.  Valid values are from 1-5.
 *  @param hPoint LASPointH instance
 *  @param value the value to set for the number of returns
 *  @return LASError value determine success or failure.
*/
LAS_DLL LASError LASPoint_SetNumberOfReturns(LASPointH hPoint, uint16_t value);

/** Returns the scan direction for a given pulse.
 *  @param hPoint LASPointH instance
 *  @return the scan direction for a given pulse.
*/
LAS_DLL uint16_t LASPoint_GetScanDirection(const LASPointH hPoint);

/** Sets the scan direction for a given pulse.  Valid values are 0 or 1, with 
 *  1 being a positive scan direction and 0 being a negative scan direction.
 *  @param hPoint LASPointH instance
 *  @param value the value to set for scan direction
 *  @return LASError value determine success or failure.
*/
LAS_DLL LASError LASPoint_SetScanDirection(LASPointH hPoint, uint16_t value);

/** Returns whether or not a given pulse is an edge point
 *  @param hPoint LASPointH instance
 *  @return whether or not a given pulse is an edge point.
*/
LAS_DLL uint16_t LASPoint_GetFlightLineEdge(const LASPointH hPoint);

/** Sets the edge marker for a given pulse.  Valid values are 0 or 1, with 
 *  1 being an edge point and 0 being interior.
 *  @param hPoint LASPointH instance
 *  @param value the value to set for flightline edge
 *  @return LASError value determine success or failure.
*/
LAS_DLL LASError LASPoint_SetFlightLineEdge(LASPointH hPoint, uint16_t value);

/** Returns all of the scan flags for the point -- Return number, number of 
 *  returns, flightline edge, scan direction, and scan angle rank.
 *  @param hPoint LASPointH instance
 *  @return all of the scan flags for the point
*/
LAS_DLL uint8_t LASPoint_GetScanFlags(const LASPointH hPoint);

/** Sets all of the scan flags for the point.  No validation is done. 
 *  @param hPoint LASPointH instance
 *  @param value the value to set for the flags
 *  @return LASError value determine success or failure.
*/
LAS_DLL LASError LASPoint_SetScanFlags(LASPointH hPoint, uint8_t value);

/** Returns the classification for the point
 *  @param hPoint LASPointH instance
 *  @return the classification for the point
*/
LAS_DLL uint8_t LASPoint_GetClassification(const LASPointH hPoint);

/** Sets the classification for the point.  No validation is done. 
 *  @param hPoint LASPointH instance
 *  @param value the value to set for the classification
 *  @return LASError value determine success or failure.
*/
LAS_DLL LASError LASPoint_SetClassification(LASPointH hPoint, uint8_t value);

/** Returns the time for the point
 *  @param hPoint LASPointH instance
 *  @return the time for the point
*/
LAS_DLL double LASPoint_GetTime(const LASPointH hPoint);

/** Sets the time for the point.  No validation is done. 
 *  @param hPoint LASPointH instance
 *  @param value the value to set for the time
 *  @return LASError value determine success or failure.
*/
LAS_DLL LASError LASPoint_SetTime(LASPointH hPoint, double value);

/** Returns the scan angle for the point
 *  @param hPoint LASPointH instance
 *  @return the scan angle for the point
*/
LAS_DLL int8_t LASPoint_GetScanAngleRank(const LASPointH hPoint);

/** Sets the scan angle for the point.  No validation is done. 
 *  @param hPoint LASPointH instance
 *  @param value the value to set for the scan angle
 *  @return LASError value determine success or failure.
*/
LAS_DLL LASError LASPoint_SetScanAngleRank(LASPointH hPoint, int8_t value);

/** Returns the arbitrary user data for the point
 *  @param hPoint LASPointH instance
 *  @return the arbitrary user data for the point
*/
LAS_DLL uint8_t LASPoint_GetUserData(const LASPointH hPoint);

/** Sets the arbitrary user data for the point.  No validation is done. 
 *  @param hPoint LASPointH instance
 *  @param value the value to set for the arbitrary user data
 *  @return LASError value determine success or failure.
*/
LAS_DLL LASError LASPoint_SetUserData(LASPointH hPoint, uint8_t value);

/** Returns a bitfield representing the validity of various members
 *  enum DataMemberFlag
 {
     eReturnNumber = 1,
     eNumberOfReturns = 2,
     eScanDirection = 4,
     eFlightLineEdge = 8,
     eClassification = 16,
     eScanAngleRank = 32,
     eTime = 64
 };
 *  @param hPoint LASPointH instance
 *  @return bitfield representing the validity of various members.
*/
LAS_DLL int LASPoint_Validate(LASPointH hPoint);

/** Returns a boolean whether or not the point is valid
 *  @param hPoint LASPointH instance
 *  @return a boolean (1 or 0) whether or not the point is valid.
*/
LAS_DLL int LASPoint_IsValid(LASPointH hPoint);

/** Creates a new empty LASPointH instance 
 *  @return LASPointH instance.  If the value is NULL use the 
 *  LASError_GetLastError* methods to determine the problem
*/
LAS_DLL LASPointH LASPoint_Create(void);

/** Creates a copy of a LASPointH instance
 *  @param hPoint the LASPointH instance to copy 
 *  @return new LASPointH instance.  If the value is NULL use the 
 *  LASError_GetLastError* methods to determine the problem
*/
LAS_DLL LASPointH LASPoint_Copy(const LASPointH);

/** Destroys/deletes a LASPointH instance
*/
LAS_DLL void LASPoint_Destroy(LASPointH hPoint);

/****************************************************************************/
/* Header operations                                                        */
/****************************************************************************/

/** Copies a LASHeaderH instance
 *  @param hHeader the LASHeaderH to copy
 *  @return a LASHeaderH instance or NULL on error
*/
LAS_DLL LASHeaderH LASHeader_Copy(const LASHeaderH hHeader);

/** Creates an empty LASHeaderH with default values
*/
LAS_DLL LASHeaderH LASHeader_Create(void);

/** Destroys/deletes a LASHeader instance
*/
LAS_DLL void LASHeader_Destroy(LASHeaderH hHeader);

/** Returns the file signature the the file.  This should always be 'LASF'
 *  @param hHeader LASHeaderH instance
 *  @return the file signature the the file.  This should always be 'LASF'
*/
LAS_DLL char *LASHeader_GetFileSignature(const LASHeaderH hHeader);

/** Returns the file source id for the file.  It is a number from 1-65535
 *  @param hHeader LASHeaderH instance
 *  @return the file source id for the file.
*/
LAS_DLL uint16_t LASHeader_GetFileSourceId(const LASHeaderH hHeader);

/** Sets the FileSource ID value for the header.  By default, this value is "0" if it 
 *  is not explicitly set.  See the LAS specification for details on what this
 *  value should logically be set to.
 *  @param hHeader LASHeaderH instance
 *  @param value the value to set as the FileSource ID value for the header
 *  @return LASError enum
*/
LAS_DLL LASError LASHeader_SetFileSourceId(LASHeaderH hHeader, uint16_t value);

/** Returns the project id for the header as a GUID string
 *  @return the project id for the header as a GUID string
*/
LAS_DLL char *LASHeader_GetProjectId(const LASHeaderH hHeader);

/** Sets the project id/GUID for the header
 *  @param hHeader LASHeaderH instance
 *  @param value character value GUID to set the header value to
 *  @return LASError enum
*/
LAS_DLL LASError LASHeader_SetProjectId(LASHeaderH hHeader, const char* value);

/** Sets the project id/GUID for the header
 *  @param hHeader LASHeaderH instance
 *  @param hId LASGuidH instance to set the GUID for the header to
 *  @return LASError enum
*/
LAS_DLL LASError LASHeader_SetGUID(LASHeaderH hHeader, LASGuidH hId);

/** Returns the major version number for the header.  This value is expected 
 *  to always be 1.
 *  @param hHeader LASHeaderH instance
 *  @return major version number for the header.
*/
LAS_DLL uint8_t LASHeader_GetVersionMajor(const LASHeaderH hHeader);

/** Sets the major version number for the header.  All values other than 1 
 *  are invalid.
 *  @param hHeader LASHeaderH instance
 *  @param value integer value to set the major version to (only the value 1 is valid)
 *  @return LASError enum
*/
LAS_DLL LASError LASHeader_SetVersionMajor(LASHeaderH hHeader, uint8_t value);

/** Returns the min version number for the header.  This value is expected 
 *  to be 1 or 0 representing LAS 1.1 or LAS 1.0
 *  @param hHeader LASHeaderH instance
 *  @return minor version number for the header.
*/
LAS_DLL uint8_t LASHeader_GetVersionMinor(const LASHeaderH hHeader);

/** Sets the minor version number for the header.  All values other than 1 or 0 
 *  are invalid.
 *  @param hHeader LASHeaderH instance
 *  @param value integer value to set the minor version to (only the values 1 or 0 are valid)
 *  @return LASError enum
*/
LAS_DLL LASError LASHeader_SetVersionMinor(LASHeaderH hHeader, uint8_t value);

/** Returns the System ID for the header.  The caller assumes ownership of the returned string
 *  @return the system id for the header as a character array
*/
LAS_DLL char *LASHeader_GetSystemId(const LASHeaderH hHeader);

/** Sets the System ID for the header.  By default, this value is "libLAS" if it 
 *  is not explicitly set.  See the LAS specification for details on what this
 *  value should logically be set to.
 *  @param hHeader LASHeaderH instance
 *  @param value the value to set as the System ID for the header
 *  @return LASError enum
*/
LAS_DLL LASError LASHeader_SetSystemId(LASHeaderH hHeader, const char* value);

/** Returns the Software ID for the header.  The caller assumes ownership of the returned string
 *  @return the software id for the header as a character array
*/
LAS_DLL char *LASHeader_GetSoftwareId(const LASHeaderH hHeader);

/** Sets the Software ID for the header.  By default, this value is "libLAS 1.0" if it 
 *  is not explicitly set.  See the LAS specification for details on what this
 *  value should logically be set to.
 *  @param hHeader LASHeaderH instance
 *  @param value the value to set as the Software ID for the header
 *  @return LASError enum
*/
LAS_DLL LASError LASHeader_SetSoftwareId(LASHeaderH hHeader, const char* value);

/** Returns the reserved value for the header.  This should aways be 0.
 *  @return the reserved value for the header.
*/
LAS_DLL uint16_t LASHeader_GetReserved(const LASHeaderH hHeader);

/** Sets the Reserved value for the header.  By default, this value is "0" if it 
 *  is not explicitly set.  See the LAS specification for details on what this
 *  value should logically be set to.
 *  @param hHeader LASHeaderH instance
 *  @param value the value to set as the reserved value for the header
 *  @return LASError enum
*/
LAS_DLL LASError LASHeader_SetReserved(LASHeaderH hHeader, uint16_t value);

/** Returns the file creation day of the year.  The values start from 1, being January 1st, 
 *  and end at 365 or 366 being December 31st, depending on leap year.
 *  @return the day of the year as an integer starting from 1 for the file creation.
*/
LAS_DLL uint16_t LASHeader_GetCreationDOY(const LASHeaderH hHeader);

/** Sets the file creation day of the year.  The values start from 1, being January 1st.  No
 *  date validation is done
 *  @param hHeader LASHeaderH instance
 *  @param value the value to set as the creation day
 *  @return LASError enum
*/
LAS_DLL LASError LASHeader_SetCreationDOY(LASHeaderH hHeader, uint16_t value);

/** Returns the file creation year.  This is a four digit number representing the 
 *  year for the file, ie 2003, 2008, etc.
 *  @return the creation year for the file or 0 if none is set
*/
LAS_DLL uint16_t LASHeader_GetCreationYear(const LASHeaderH hHeader);

/** Sets the file creation year.  This should be a four digit number representing
 *  the year for the file, ie 2003, 2008, etc.  No validation on the value is done
 *  @param hHeader LASHeaderH instance
 *  @param value the value to set for the creation year
 *  @return LASError enum
*/
LAS_DLL LASError LASHeader_SetCreationYear(LASHeaderH hHeader, uint16_t value);

/** Returns the size of the header for the file in bytes.
 *  @return the size of the header for the file in bytes.
*/
LAS_DLL uint16_t LASHeader_GetHeaderSize(const LASHeaderH hHeader);

/** Returns the byte offset to the start of actual point data for the file
 *  @param hHeader LASHeaderH instance
 *  @return the type offset to the start of actual point data for the file
*/
LAS_DLL uint32_t LASHeader_GetDataOffset(const LASHeaderH hHeader);

/** Returns the number of variable length records in the header
 *  @param hHeader LASHeaderH instance
 *  @return the number of variable length records in the header
*/
LAS_DLL uint32_t LASHeader_GetRecordsCount(const LASHeaderH hHeader);

/** Returns the record length for the points based on their data format id in bytes
 *  @param hHeader LASHeaderH instance
 *  @return the record length for the points based on their data format id in bytes
*/
LAS_DLL uint16_t LASHeader_GetDataRecordLength(const LASHeaderH hHeader);

/** Returns the data format id.  If this value is 1, the point data have time values
 *  associated with them.  If it is 0, the point data do not have time values.  
 *  @param hHeader LASHeaderH instance
 *  @return the data format id for the file.
*/
LAS_DLL uint8_t LASHeader_GetDataFormatId(const LASHeaderH hHeader);

/** Sets the data format id for the file.  The value should be 1 or 0, with 1 being
 *  points that contain time values and 0 being points that do not.
 *  @param hHeader LASHeaderH instance
 *  @param value the value for the data format id, 1 or 0 are valid values.
 *  @return LASError enum
*/
LAS_DLL LASError LASHeader_SetDataFormatId(const LASHeaderH hHeader, uint8_t value);

/** Returns the number of point records in the file.  This value may not reflect the actual 
 *  number of point records in the file.
 *  @param hHeader LASHeaderH instance
 *  @return the number of point records in the file
*/
LAS_DLL uint32_t LASHeader_GetPointRecordsCount(const LASHeaderH hHeader);

/** Sets the number of point records for the file.
 *  @param hHeader LASHeaderH instance
 *  @param value the long integer to set for the number of point records in the file
 *  @return LASError enum
*/
LAS_DLL LASError LASHeader_SetPointRecordsCount(const LASHeaderH hHeader, uint32_t value);

/** Returns the number of point records by return.
 *  @param hHeader LASHeaderH instance
 *  @param index the return number to fetch the count for
 *  @return the number of point records for a given return
*/
LAS_DLL uint32_t LASHeader_GetPointRecordsByReturnCount(const LASHeaderH hHeader, int index);

/** Sets the number of point records for a given return
 *  @param hHeader LASHeaderH instance
 *  @param index the return number to set the count for
 *  @param value the number of point records for the return 
 *  @return LASError enum
*/
LAS_DLL LASError LASHeader_SetPointRecordsByReturnCount(const LASHeaderH hHeader, int index, uint32_t value);

/** Return the X scale factor
 *  @param hHeader LASHeaderH instance
 *  @return the X scale factor
*/
LAS_DLL double LASHeader_GetScaleX(const LASHeaderH hHeader);

/** Return the Y scale factor
 *  @param hHeader LASHeaderH instance
 *  @return the Y scale factor
*/
LAS_DLL double LASHeader_GetScaleY(const LASHeaderH hHeader);

/** Return the Z scale factor
 *  @param hHeader LASHeaderH instance
 *  @return the Z scale factor
*/
LAS_DLL double LASHeader_GetScaleZ(const LASHeaderH hHeader);

/** Sets the scale factors
 *  @param hHeader LASHeaderH instance
 *  @param x the x scale factor
 *  @param y the y scale factor
 *  @param z the z scale factor
 *  @return LASError enum
*/
LAS_DLL LASError LASHeader_SetScale(LASHeaderH hHeader, double x, double y, double z);

/** Return the X offset
 *  @param hHeader LASHeaderH instance
 *  @return the X offset
*/
LAS_DLL double LASHeader_GetOffsetX(const LASHeaderH hHeader);

/** Return the Y offset
 *  @param hHeader LASHeaderH instance
 *  @return the Y offset
*/
LAS_DLL double LASHeader_GetOffsetY(const LASHeaderH hHeader);

/** Return the Z offset
 *  @param hHeader LASHeaderH instance
 *  @return the Z offset
*/
LAS_DLL double LASHeader_GetOffsetZ(const LASHeaderH hHeader);

/** Sets the offset values
 *  @param hHeader LASHeaderH instance
 *  @param x the x offset
 *  @param y the y offset
 *  @param z the z offset
 *  @return LASError enum
*/
LAS_DLL LASError LASHeader_SetOffset(LASHeaderH hHeader, double x, double y, double z);

/** Return the minimum x value
 *  @param hHeader LASHeaderH instance
 *  @return the minimum x value
*/
LAS_DLL double LASHeader_GetMinX(const LASHeaderH hHeader);

/** Return the minimum y value
 *  @param hHeader LASHeaderH instance
 *  @return the minimum y value
*/
LAS_DLL double LASHeader_GetMinY(const LASHeaderH hHeader);

/** Return the minimum z value
 *  @param hHeader LASHeaderH instance
 *  @return the minimum z value
*/
LAS_DLL double LASHeader_GetMinZ(const LASHeaderH hHeader);

/** Sets the minimum values
 *  @param hHeader LASHeaderH instance
 *  @param x the x minimum
 *  @param y the y minimum
 *  @param z the z minimum
 *  @return LASError enum
*/
LAS_DLL LASError LASHeader_SetMin(LASHeaderH hHeader, double x, double y, double z);

/** Return the maximum x value
 *  @param hHeader LASHeaderH instance
 *  @return the maximum x value
*/
LAS_DLL double LASHeader_GetMaxX(const LASHeaderH hHeader);

/** Return the maximum y value
 *  @param hHeader LASHeaderH instance
 *  @return the maximum y value
*/
LAS_DLL double LASHeader_GetMaxY(const LASHeaderH hHeader);

/** Return the maximum z value
 *  @param hHeader LASHeaderH instance
 *  @return the maximum z value
*/
LAS_DLL double LASHeader_GetMaxZ(const LASHeaderH hHeader);

/** Sets the maximum values
 *  @param hHeader LASHeaderH instance
 *  @param x the x maximum
 *  @param y the y maximum
 *  @param z the z maximum
 *  @return LASError enum
*/
LAS_DLL LASError LASHeader_SetMax(LASHeaderH hHeader, double x, double y, double z);

/** Returns the proj.4 string describing the spatial reference of the 
 *  header if it is available
 *  @param hHeader LASHeaderH instance
 *  @return the proj.4 string or NULL if none is available.  The caller
 *  owns the string.
*/
LAS_DLL char* LASHeader_GetProj4(LASHeaderH hHeader);

/** Sets the proj4 stirng describing the spatial reference of the header.
 *  @param hHeader LASHeaderH instance
 *  @param value the proj4 string to set for the header
 *  @return LASError enum
*/
LAS_DLL LASError LASHeader_SetProj4(LASHeaderH hHeader, const char* value);

/** Returns the VLR record for the given index.  Use LASHeader_GetRecordsCount to 
 *  determine the number of VLR records available on the header.
 *  @param hHeader the LASHeaderH instance
 *  @param i the index starting from 0 of the VLR to fetch
 *  @return LASVLRH instance that models the Variable Length Record
*/
LAS_DLL LASVLRH LASHeader_GetVLR(const LASHeaderH hHeader, uint32_t i);

/** Deletes a VLR record from the header for the given index.
 *  @param hHeader the LASHeaderH instance
 *  @param index the index starting from 0 of the VLR to delete
 *  @return LASErrorEnum
*/
LAS_DLL LASError LASHeader_DeleteVLR(LASHeaderH hHeader, uint32_t index);

/** Adds a VLR record to the header. 
 *  @param hHeader the LASHeaderH instance
 *  @param hVLR the VLR to add to the header
 *  @return LASErrorEnum
*/
LAS_DLL LASError LASHeader_AddVLR(LASHeaderH hHeader, const LASVLRH hVLR);

/****************************************************************************/
/* Writer Operations                                                        */
/****************************************************************************/

/** Creates a new LASWriterH for write operations on LAS files.  The file may 
 *  be opened in either LAS_MODE_APPEND or LAS_MODE_WRITE, but the file cannot 
 *  be open by another other operations (another LASReaderH or LASWriterH).  
 *  @param filename The filename to open to write
 *  @param hHeader an opaque pointer to a LASHeaderH that will be written to 
 *  the file as part of the opening for write operation.
 *  @param mode a mode value to denote whether to open for write or append 
 *  operations.  Valid values are LAS_MODE_APPEND and LAS_MODE_WRITE.
*/
LAS_DLL LASWriterH LASWriter_Create(const char* filename, const LASHeaderH hHeader, int mode);

/** Writes a point to the file.  The location of where the point is writen is 
 *  determined by the mode the file is opened in, and what the last operation was.  
 *  For example, if the file was opened for append, the next point would be written 
 *  at the end of the file.  Likewise, if the file is opened in write mode, even 
 *  if the file already existed, the next WritePoint operation will happen at the 
 *  end of the header and all of the existing points in the file will be overwritten.
 *  @param hWriter opaque pointer to the LASWriterH instance
 *  @param hPoint the opaque LASPointH pointer to write
 *  @return LE_None if no error occurred during the write operation.
*/
LAS_DLL LASError LASWriter_WritePoint(const LASWriterH hWriter, const LASPointH hPoint);

/** Overwrites the header for the file represented by the LASWriterH.  It does 
 *  not matter if the file is opened for append or for write.
 *  @param hWriter opaque pointer to the LASWriterH instance
 *  @param hHeader LASHeaderH instance to write into the file
 *  @return LE_None if no error occurred during the operation.
*/

LAS_DLL LASError LASWriter_WriteHeader(const LASWriterH hWriter, const LASHeaderH hHeader);

/** Destroys the LASWriterH instance, effectively closing the file and performing 
 *  housekeeping operations.
 *  @param hWriter LASWriterH instance to close
*/
LAS_DLL void LASWriter_Destroy(LASWriterH hWriter);

/****************************************************************************/
/* GUID Operations                                                          */
/****************************************************************************/

/** Returns the GUID value for the header as an opaque LASGuidH pointer.
 *  @param hHeader the opaque pointer to the LASHeaderH
 *  @return the GUID value for the header as an opaque LASGuidH pointer.
*/
LAS_DLL LASGuidH LASHeader_GetGUID(const LASHeaderH hHeader);

/** Returns a new random GUID.
 *  @return a new random GUID
*/
LAS_DLL LASGuidH LASGuid_Create();

/** Creates a new GUID opaque pointer using the given string.  
 *  @param string A GUID string in the form "00000000-0000-0000-0000-000000000000"
 *  An example GUID might be something like '8388F1B8-AA1B-4108-BCA3-6BC68E7B062E'
 *  @return the GUID value as an opaque LASGuidH pointer.
*/
LAS_DLL LASGuidH LASGuid_CreateFromString(const char* string);

/** Destroys a GUID opaque pointer and removes it from the heap
 *  @param hId the GUID value to destroy as an opaque LASGuidH pointer.
*/
LAS_DLL void LASGuid_Destroy(LASGuidH hId);

/** Determines if two GUIDs are equal.
 *  @param hId1 the first GUID
 *  @param hId2 the second GUID
 *  @return 0 if false, 1 if true.  Use the LASError_GetLastError* methods to 
 *  determine if an error occured during the operation of this function.
*/
LAS_DLL int LASGuid_Equals(LASGuidH hId1, LASGuidH hId2);

/** Returns a string representation of the GUID opqaue pointer.  The caller 
 *  owns the string.
 *  @param hId the LASGuidH pointer
 *  @return a string representation of the GUID opaque pointer.
*/
LAS_DLL char* LASGuid_AsString(LASGuidH hId);

/****************************************************************************/
/* VLR Operations                                                           */
/****************************************************************************/

/** Creates a new VLR record
 *  @return a new VLR record
*/
LAS_DLL LASVLRH LASVLR_Create(void);

/** Destroys a VLR record and removes it from the heap
*/
LAS_DLL void LASVLR_Destroy(LASVLRH hVLR);

/** Returns the User Id for the VLR 
 *  @param hVLR the LASVLRH instance
 *  @return the User Id for the VLR
*/
LAS_DLL char* LASVLR_GetUserId(const LASVLRH hVLR);

/** Sets the User Id for the VLR
 *  @param hVLR the LASVLRH instance
 *  @param value the value to set for the User Id.  It will be clipped to fit 
 *  within 16 characters
 *  @return LASErrorEnum
*/
LAS_DLL LASError LASVLR_SetUserId(LASVLRH hVLR, const char* value);

/** Gets the description for the VLR
 *  @param hVLR the LASVLRH instance
 *  @return the description for the VLR
*/ 
LAS_DLL char* LASVLR_GetDescription(const LASVLRH hVLR);

/** Sets the description for the VLR
 *  @param hVLR the LASVLRH instance
 *  @param value the value to set for the description.  It will be clipped to fit 
 *  within 32 characters
 *  @return LASErrorEnum
*/
LAS_DLL LASError LASVLR_SetDescription(LASVLRH hVLR, const char* value);

/** Returns the record length of the data stored in the VLR
 *  @param hVLR the LASVLRH instance
 *  @return the record length of the data stored in the VLR
*/
LAS_DLL uint16_t LASVLR_GetRecordLength(const LASVLRH hVLR);

/** Sets the record length of the data stored in the VLR
 *  @param hVLR the LASVLRH instance
 *  @param value the length to set for the VLR data length
 *  @return LASErrorEnum
*/
LAS_DLL LASError LASVLR_SetRecordLength(LASVLRH hVLR, uint16_t value);

/** Gets the record id for the VLR
 *  @param hVLR the LASVLRH instance
 *  @return the record id for the VLR
*/
LAS_DLL uint16_t LASVLR_GetRecordId(const LASVLRH hVLR);

/** Sets the record id for the VLR
 *  @param hVLR the LASVLRH instance
 *  @param value the record id to set
 *  @return LASErrorEnum
*/
LAS_DLL LASError LASVLR_SetRecordId(LASVLRH hVLR, uint16_t value);

/** Gets the reserved value of the VLR.  This should be 0 and should aways be 0.
 *  @param hVLR the LASVLRH instance
 *  @return the reserved value of the VLR.
*/
LAS_DLL uint16_t LASVLR_GetReserved(const LASVLRH hVLR);

/** Sets the reserved value of the VLR.  This should be 0 and you should not 
 *  have to ever monkey with this value according to the spec.
 *  @param hVLR the LASVLRH instance
 *  @param value the value to set for the reserved value
 *  @return LASErrorEnum
*/
LAS_DLL LASError LASVLR_SetReserved(LASVLRH hVLR, uint16_t value);

/** Gets the data stream for the VLR as an array of bytes.  The length of this 
 *  array should be the same as LASVLR_GetRecordLength.  You must allocate it on 
 *  the heap and you are responsible for its destruction.
 *  @param hVLR the LASVLRH instance
 *  @param data a pointer to your array where you want the data copied
 *  @return LASErrorEnum
*/
LAS_DLL LASError LASVLR_GetData(const LASVLRH hVLR, uint8_t* data);

/** Sets the data stream for the VLR as an array of bytes.  The length of this 
 *  array should be the same as LASVLR_GetRecordLength.  The data are copied into 
 *  the VLR structure.
 *  @param hVLR the LASVLRH instance
 *  @param data a pointer to your array.  It must be LASVLR_GetRecordLength in size
 *  @return LASErrorEnum
*/
LAS_DLL LASError LASVLR_SetData(const LASVLRH hVLR, uint8_t* data, uint16_t length);

LAS_C_END
#endif

