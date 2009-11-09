/******************************************************************************
 * Copied from "gdal" project. See licence below.
 *
 * Project:  ISO 8211 Access
 * Purpose:  Implements the DDFSubfieldDefn class.
 * Author:   Frank Warmerdam, warmerda@home.com
 *
 ******************************************************************************
 * Copyright (c) 1999, Frank Warmerdam
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ******************************************************************************
 * $Id: ossimDdfsubfielddefn.cpp 15833 2009-10-29 01:41:53Z eshirschorn $
 */

#include <cstring>
#include <ossim/imaging/ossimIso8211.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimCommon.h>

/************************************************************************/
/*                          DDFSubfieldDefn()                           */
/************************************************************************/

ossimDDFSubfieldDefn::ossimDDFSubfieldDefn()

{
    pszName = NULL;
    
    bIsVariable = true;
    nFormatWidth = 0;
    chFormatDelimeter = OSSIM_DDF_UNIT_TERMINATOR;
    eBinaryFormat = NotBinary;
    eType = DDFString;
    
    pszFormatString = strdup("");

    nMaxBufChars = 0;
    pachBuffer = NULL;
}

/************************************************************************/
/*                          ~DDFSubfieldDefn()                          */
/************************************************************************/

ossimDDFSubfieldDefn::~ossimDDFSubfieldDefn()

{
    free( pszName );
    free( pszFormatString );
    free( pachBuffer );
}

/************************************************************************/
/*                              SetName()                               */
/************************************************************************/

void ossimDDFSubfieldDefn::SetName( const char * pszNewName )

{
    int         i;
    
    free( pszName );

    pszName = strdup( pszNewName );

    for( i = (int)strlen(pszName)-1; i > 0 && pszName[i] == ' '; i-- )
        pszName[i] = '\0';
}

/************************************************************************/
/*                             SetFormat()                              */
/*                                                                      */
/*      While interpreting the format string we don't support:          */
/*                                                                      */
/*       o Passing an explicit terminator for variable length field.    */
/*       o 'X' for unused data ... this should really be filtered       */
/*         out by DDFFieldDefn::ApplyFormats(), but isn't.              */
/*       o 'B' bitstrings that aren't a multiple of eight.              */
/************************************************************************/

int ossimDDFSubfieldDefn::SetFormat( const char * pszFormat )

{
    free( pszFormatString );
    pszFormatString = strdup( pszFormat );

/* -------------------------------------------------------------------- */
/*      These values will likely be used.                               */
/* -------------------------------------------------------------------- */
    if( pszFormatString[1] == '(' )
    {
        nFormatWidth = atoi(pszFormatString+2);
        bIsVariable = nFormatWidth == 0;
    }
    else
        bIsVariable = true;
    
/* -------------------------------------------------------------------- */
/*      Interpret the format string.                                    */
/* -------------------------------------------------------------------- */
    switch( pszFormatString[0] )
    {
      case 'A':
      case 'C':         // It isn't clear to me how this is different than 'A'
        eType = DDFString;
        break;

      case 'R':
        eType = DDFFloat;
        break;
        
      case 'I':
      case 'S':
        eType = DDFInt;
        break;

      case 'B':
      case 'b':
        // Is the width expressed in bits? (is it a bitstring)
        bIsVariable = false;
        if( pszFormatString[1] == '(' )
        {
           // CPLAssert( atoi(pszFormatString+2) % 8 == 0 );
            
            nFormatWidth = atoi(pszFormatString+2) / 8;
            eBinaryFormat = SInt; // good default, works for SDTS.

            if( nFormatWidth < 5 )
                eType = DDFInt;
            else
                eType = DDFBinaryString;
        }
        
        // or do we have a binary type indicator? (is it binary)
        else
        {
            eBinaryFormat = (DDFBinaryFormat) (pszFormatString[1] - '0');
            nFormatWidth = atoi(pszFormatString+2);

            if( eBinaryFormat == SInt || eBinaryFormat == UInt )
                eType = DDFInt;
            else
                eType = DDFFloat;
        }
        break;

      case 'X':
        // 'X' is extra space, and shouldn't be directly assigned to a
        // subfield ... I haven't encountered it in use yet though.
        ossimNotify(ossimNotifyLevel_WARN)
           << "Format type of `%c' not supported.\n"
           << pszFormatString[0] << std::endl;
        
        // CPLAssert( false );
        return false;
        
      default:
        ossimNotify(ossimNotifyLevel_WARN)
           << "Format type of `%c' not recognised.\n"
           << pszFormatString[0] << std::endl;
        
        // CPLAssert( false );
        return false;
    }
    
    return true;
}

/************************************************************************/
/*                                Dump()                                */
/************************************************************************/

/**
 * Write out subfield definition info to debugging file.
 *
 * A variety of information about this field definition is written to the
 * give debugging file handle.
 *
 * @param fp The standard io file handle to write to.  ie. stderr
 */

void ossimDDFSubfieldDefn::Dump( FILE * fp )

{
    fprintf( fp, "    DDFSubfieldDefn:\n" );
    fprintf( fp, "        Label = `%s'\n", pszName );
    fprintf( fp, "        FormatString = `%s'\n", pszFormatString );
}

/************************************************************************/
/*                           GetDataLength()                            */
/*                                                                      */
/*      This method will scan for the end of a variable field.          */
/************************************************************************/

/**
 * Scan for the end of variable length data.  Given a pointer to the data
 * for this subfield (from within a DDFRecord) this method will return the
 * number of bytes which are data for this subfield.  The number of bytes
 * consumed as part of this field can also be fetched.  This number may
 * be one longer than the length if there is a terminator character
 * used.<p>
 *
 * This method is mainly for internal use, or for applications which
 * want the raw binary data to interpret themselves.  Otherwise use one
 * of ExtractStringData(), ExtractIntData() or ExtractFloatData().
 *
 * @param pachSourceData The pointer to the raw data for this field.  This
 * may have come from DDFRecord::GetData(), taking into account skip factors
 * over previous subfields data.
 * @param nMaxBytes The maximum number of bytes that are accessable after
 * pachSourceData.
 * @param pnConsumedBytes Pointer to an integer into which the number of
 * bytes consumed by this field should be written.  May be NULL to ignore.
 *
 * @return The number of bytes at pachSourceData which are actual data for
 * this record (not including unit, or field terminator).  
 */

int ossimDDFSubfieldDefn::GetDataLength( const char * pachSourceData,
                                    int nMaxBytes, int * pnConsumedBytes )

{
    if( !bIsVariable )
    {
        if( nFormatWidth > nMaxBytes )
        {
            ossimNotify(ossimNotifyLevel_WARN)
               << "Only %d bytes available for subfield %s with\n"
               << "format string %s ... returning shortened data."
               << nMaxBytes << pszName << pszFormatString << std::endl;

            if( pnConsumedBytes != NULL )
                *pnConsumedBytes = nMaxBytes;

            return nMaxBytes;
        }
        else
        {
            if( pnConsumedBytes != NULL )
                *pnConsumedBytes = nFormatWidth;

            return nFormatWidth;
        }
    }
    else
    {
        int     nLength = 0;
        int     bCheckFieldTerminator = true;

        /* We only check for the field terminator because of some buggy 
         * datasets with missing format terminators.  However, we have found
         * the field terminator is a legal character within the fields of
         * some extended datasets (such as JP34NC94.000).  So we don't check
         * for the field terminator if the field appears to be multi-byte
         * which we established by the first character being out of the 
         * ASCII printable range (32-127). 
         */

        if( pachSourceData[0] < 32 || pachSourceData[0] >= 127 )
            bCheckFieldTerminator = false;
        
        while( nLength < nMaxBytes
               && pachSourceData[nLength] != chFormatDelimeter )
        {
            if( bCheckFieldTerminator 
                && pachSourceData[nLength] == OSSIM_DDF_FIELD_TERMINATOR )
                break;

            nLength++;
        }

        if( pnConsumedBytes != NULL )
        {
            if( nMaxBytes == 0 )
                *pnConsumedBytes = nLength;
            else
                *pnConsumedBytes = nLength+1;
        }
        
        return nLength;
    }
}

/************************************************************************/
/*                         ExtractStringData()                          */
/************************************************************************/

/**
 * Extract a zero terminated string containing the data for this subfield.
 * Given a pointer to the data
 * for this subfield (from within a DDFRecord) this method will return the
 * data for this subfield.  The number of bytes
 * consumed as part of this field can also be fetched.  This number may
 * be one longer than the string length if there is a terminator character
 * used.<p>
 *
 * This function will return the raw binary data of a subfield for
 * types other than DDFString, including data past zero chars.  This is
 * the standard way of extracting DDFBinaryString subfields for instance.<p>
 *
 * @param pachSourceData The pointer to the raw data for this field.  This
 * may have come from DDFRecord::GetData(), taking into account skip factors
 * over previous subfields data.
 * @param nMaxBytes The maximum number of bytes that are accessable after
 * pachSourceData.
 * @param pnConsumedBytes Pointer to an integer into which the number of
 * bytes consumed by this field should be written.  May be NULL to ignore.
 * This is used as a skip factor to increment pachSourceData to point to the
 * next subfields data.
 *
 * @return A pointer to a buffer containing the data for this field.  The
 * returned pointer is to an internal buffer which is invalidated on the
 * next ExtractStringData() call on this DDFSubfieldDefn().  It should not
 * be freed by the application.
 *
 * @see ExtractIntData(), ExtractFloatData()
 */

const char *
ossimDDFSubfieldDefn::ExtractStringData( const char * pachSourceData,
                                    int nMaxBytes, int * pnConsumedBytes )

{
    int         nLength = GetDataLength( pachSourceData, nMaxBytes,
                                         pnConsumedBytes );

/* -------------------------------------------------------------------- */
/*      Do we need to grow the buffer.                                  */
/* -------------------------------------------------------------------- */
    if( nMaxBufChars < nLength+1 )
    {
        free( pachBuffer );
        
        nMaxBufChars = nLength+1;
        pachBuffer = (char *) malloc(nMaxBufChars);
    }

/* -------------------------------------------------------------------- */
/*      Copy the data to the buffer.  We use memcpy() so that it        */
/*      will work for binary data.                                      */
/* -------------------------------------------------------------------- */
    memcpy( pachBuffer, pachSourceData, nLength );
    pachBuffer[nLength] = '\0';

    return pachBuffer;
}

/************************************************************************/
/*                          ExtractFloatData()                          */
/************************************************************************/

/**
 * Extract a subfield value as a float.  Given a pointer to the data
 * for this subfield (from within a DDFRecord) this method will return the
 * floating point data for this subfield.  The number of bytes
 * consumed as part of this field can also be fetched.  This method may be
 * called for any type of subfield, and will return zero if the subfield is
 * not numeric.
 *
 * @param pachSourceData The pointer to the raw data for this field.  This
 * may have come from DDFRecord::GetData(), taking into account skip factors
 * over previous subfields data.
 * @param nMaxBytes The maximum number of bytes that are accessable after
 * pachSourceData.
 * @param pnConsumedBytes Pointer to an integer into which the number of
 * bytes consumed by this field should be written.  May be NULL to ignore.
 * This is used as a skip factor to increment pachSourceData to point to the
 * next subfields data.
 *
 * @return The subfield's numeric value (or zero if it isn't numeric).
 *
 * @see ExtractIntData(), ExtractStringData()
 */

double
ossimDDFSubfieldDefn::ExtractFloatData( const char * pachSourceData,
                                   int nMaxBytes, int * pnConsumedBytes )

{
    switch( pszFormatString[0] )
    {
      case 'A':
      case 'I':
      case 'R':
      case 'S':
      case 'C':
        return atof(ExtractStringData(pachSourceData, nMaxBytes,
                                      pnConsumedBytes));

      case 'B':
      case 'b':
      {
          unsigned char   abyData[8];

          // CPLAssert( nFormatWidth <= nMaxBytes );
          if( pnConsumedBytes != NULL )
              *pnConsumedBytes = nFormatWidth;

          // Byte swap the data if it isn't in machine native format.
          // In any event we copy it into our buffer to ensure it is
          // word aligned.
#ifdef CPL_LSB
          if( pszFormatString[0] == 'B' )
#else            
              if( pszFormatString[0] == 'b' )
#endif            
              {
                  for( int i = 0; i < nFormatWidth; i++ )
                      abyData[nFormatWidth-i-1] = pachSourceData[i];
              }
              else
              {
                  memcpy( abyData, pachSourceData, nFormatWidth );
              }

          // Interpret the bytes of data.
          switch( eBinaryFormat )
          {
             case UInt:
                if( nFormatWidth == 1 )
                {
                   return( abyData[0] );
                }
                else if( nFormatWidth == 2 )
                {
                   ossim_uint16* ptr = (ossim_uint16*) abyData;
                   return *ptr;
                }
                else if( nFormatWidth == 4 )
                {
                   ossim_uint32* ptr = (ossim_uint32*) abyData;
                   return *ptr;
                }
                else
                {
                   // CPLAssert( false );
                   return 0.0;
                }
                
             case SInt:
                if( nFormatWidth == 1 )
                {
                   signed char* ptr = (signed char*) abyData;
                   return *ptr;
                }
                else if( nFormatWidth == 2 )
                {
                   ossim_int16* ptr = (ossim_int16*) abyData;
                   return *ptr;
                }
                else if( nFormatWidth == 4 )
                {
                   ossim_int32* ptr = (ossim_int32*) abyData;
                   return *ptr;
                }
                else
                {
                   // CPLAssert( false );
                   return 0.0;
                }
            
             case FloatReal:
                if( nFormatWidth == 4 )
                {
                   float* ptr = (float*) abyData;
                   return *ptr;
                }
                else if( nFormatWidth == 8 )
                {
                   double* ptr = (double*) abyData;
                   return *ptr;
                }
                else
                {
                   // CPLAssert( false );
                   return 0.0;
                }
                
             case NotBinary:            
             case FPReal:
             case FloatComplex:
                // CPLAssert( false );
                return 0.0;
          }
          break;
          // end of 'b'/'B' case.
      }

      default:
        // CPLAssert( false );
        return 0.0;
    }

    // CPLAssert( false );
    return 0.0;
}

/************************************************************************/
/*                           ExtractIntData()                           */
/************************************************************************/

/**
 * Extract a subfield value as an integer.  Given a pointer to the data
 * for this subfield (from within a DDFRecord) this method will return the
 * int data for this subfield.  The number of bytes
 * consumed as part of this field can also be fetched.  This method may be
 * called for any type of subfield, and will return zero if the subfield is
 * not numeric.
 *
 * @param pachSourceData The pointer to the raw data for this field.  This
 * may have come from DDFRecord::GetData(), taking into account skip factors
 * over previous subfields data.
 * @param nMaxBytes The maximum number of bytes that are accessable after
 * pachSourceData.
 * @param pnConsumedBytes Pointer to an integer into which the number of
 * bytes consumed by this field should be written.  May be NULL to ignore.
 * This is used as a skip factor to increment pachSourceData to point to the
 * next subfields data.
 *
 * @return The subfield's numeric value (or zero if it isn't numeric).
 *
 * @see ExtractFloatData(), ExtractStringData()
 */

int
ossimDDFSubfieldDefn::ExtractIntData( const char * pachSourceData,
                                 int nMaxBytes, int * pnConsumedBytes )

{
    switch( pszFormatString[0] )
    {
      case 'A':
      case 'I':
      case 'R':
      case 'S':
      case 'C':
        return atoi(ExtractStringData(pachSourceData, nMaxBytes,
                                      pnConsumedBytes));

      case 'B':
      case 'b':
      {
          unsigned char   abyData[8];

          if( nFormatWidth > nMaxBytes )
          {
              ossimNotify(ossimNotifyLevel_WARN)
                 << "Attempt to extract int subfield %s with format %s\n"
                 << "failed as only %d bytes available.  Using zero."
                 << pszName << pszFormatString << nMaxBytes << std::endl;
              return 0;
          }

          if( pnConsumedBytes != NULL )
              *pnConsumedBytes = nFormatWidth;

          // Byte swap the data if it isn't in machine native format.
          // In any event we copy it into our buffer to ensure it is
          // word aligned.
#ifdef CPL_LSB
          if( pszFormatString[0] == 'B' )
#else            
              if( pszFormatString[0] == 'b' )
#endif            
              {
                  for( int i = 0; i < nFormatWidth; i++ )
                      abyData[nFormatWidth-i-1] = pachSourceData[i];
              }
              else
              {
                  memcpy( abyData, pachSourceData, nFormatWidth );
              }

          // Interpret the bytes of data.
          switch( eBinaryFormat )
          {
            case UInt:
              if( nFormatWidth == 4 )
              {
                 ossim_uint32* ptr = (ossim_uint32*) abyData;
                 return *ptr;
              }
              else if( nFormatWidth == 1 )
              {
                 return( abyData[0] );
              }
              else if( nFormatWidth == 2 )
              {
                 ossim_uint16* ptr = (ossim_uint16*)abyData;
                 return *ptr;
              }
              else
              {
                 // CPLAssert( false );
                 return 0;
              }
              
             case SInt:
                if( nFormatWidth == 4 )
                {
                   ossim_int32* ptr = (ossim_int32 *) abyData;
                   return *ptr;
                }
                else if( nFormatWidth == 1 )
                {
                   signed char* ptr = (signed char *) abyData;
                   return *ptr;
                }
                else if( nFormatWidth == 2 )
                {
                   ossim_int16* ptr = (ossim_int16 *) abyData;
                   return *ptr;
                }
                else
                {
                   // CPLAssert( false );
                   return 0;
                }
                
             case FloatReal:
                if( nFormatWidth == 4 )
                {
                   float* ptr = (float *) abyData;
                   return (int) *ptr;
                }
                else if( nFormatWidth == 8 )
                {
                   double* ptr = (double *) abyData;
                   return (int) *ptr;
                }
                else
                {
                   // CPLAssert( false );
                   return 0;
                }

             case NotBinary:            
             case FPReal:
             case FloatComplex:
                // CPLAssert( false );
                return 0;
          }
          break;
          // end of 'b'/'B' case.
      }
      
       default:
          // CPLAssert( false );
          return 0;
    }
    
    // CPLAssert( false );
    return 0;
}

/************************************************************************/
/*                              DumpData()                              */
/*                                                                      */
/*      Dump the instance data for this subfield from a data            */
/*      record.  This fits into the output dump stream of a DDFField.   */
/************************************************************************/

/**
 * Dump subfield value to debugging file.
 *
 * @param pachData Pointer to data for this subfield.
 * @param nMaxBytes Maximum number of bytes available in pachData.
 * @param fp File to write report to.
 */

void ossimDDFSubfieldDefn::DumpData( const char * pachData, int nMaxBytes,
                                FILE * fp )

{
    if( eType == DDFFloat )
        fprintf( fp, "      Subfield `%s' = %f\n",
                 pszName,
                 ExtractFloatData( pachData, nMaxBytes, NULL ) );
    else if( eType == DDFInt )
        fprintf( fp, "      Subfield `%s' = %d\n",
                 pszName,
                 ExtractIntData( pachData, nMaxBytes, NULL ) );
    else if( eType == DDFBinaryString )
    {
        int     nBytes, i;
        ossim_uint8   *pabyBString = (ossim_uint8 *) ExtractStringData( pachData, nMaxBytes, &nBytes );

        fprintf( fp, "      Subfield `%s' = 0x", pszName );
        for( i = 0; i < std::min(nBytes,24); i++ )
            fprintf( fp, "%02X", pabyBString[i] );

        if( nBytes > 24 )
            fprintf( fp, "%s", "..." );

        fprintf( fp, "\n" );
    }
    else
        fprintf( fp, "      Subfield `%s' = `%s'\n",
                 pszName,
                 ExtractStringData( pachData, nMaxBytes, NULL ) );
}

/************************************************************************/
/*                          GetDefaultValue()                           */
/************************************************************************/

/**
 * Get default data. 
 *
 * Returns the default subfield data contents for this subfield definition.
 * For variable length numbers this will normally be "0<unit-terminator>". 
 * For variable length strings it will be "<unit-terminator>".  For fixed
 * length numbers it is zero filled.  For fixed length strings it is space
 * filled.  For binary numbers it is binary zero filled. 
 *
 * @param pachData the buffer into which the returned default will be placed.
 * May be NULL if just querying default size.
 * @param nBytesAvailable the size of pachData in bytes. 
 * @param pnBytesUsed will receive the size of the subfield default data in
 * bytes.
 *
 * @return true on success or false on failure or if the passed buffer is too
 * small to hold the default.
 */

int ossimDDFSubfieldDefn::GetDefaultValue( char *pachData, int nBytesAvailable, 
                                      int *pnBytesUsed )

{
    int nDefaultSize;

    if( !bIsVariable )
        nDefaultSize = nFormatWidth;
    else
        nDefaultSize = 1;

    if( pnBytesUsed != NULL )
        *pnBytesUsed = nDefaultSize;

    if( pachData == NULL )
        return true;

    if( nBytesAvailable < nDefaultSize )
        return false;

    if( bIsVariable )
    {
        pachData[0] = OSSIM_DDF_UNIT_TERMINATOR;
    }
    else
    {
        if( GetBinaryFormat() == NotBinary )
        {
            if( GetType() == DDFInt || GetType() == DDFFloat )
                memset( pachData, '0', nDefaultSize );
            else
                memset( pachData, ' ', nDefaultSize );
        }
        else
            memset( pachData, 0, nDefaultSize );
    }

    return true;
}

/************************************************************************/
/*                         FormatStringValue()                          */
/************************************************************************/

/**
 * Format string subfield value.
 *
 * Returns a buffer with the passed in string value reformatted in a way
 * suitable for storage in a DDFField for this subfield.  
 */

int ossimDDFSubfieldDefn::FormatStringValue( char *pachData, int nBytesAvailable, 
                                        int *pnBytesUsed, 
                                        const char *pszValue,
                                        int nValueLength )

{
    int nSize;

    if( nValueLength == -1 )
        nValueLength = (int)strlen(pszValue);

    if( bIsVariable )
    {
        nSize = nValueLength + 1;
    }
    else
    {                                                                  
        nSize = nFormatWidth;
    }

    if( pnBytesUsed != NULL )
        *pnBytesUsed = nSize;

    if( pachData == NULL )
        return true;

    if( nBytesAvailable < nSize )
        return false;

    if( bIsVariable )
    {
        strncpy( pachData, pszValue, nSize-1 );
        pachData[nSize-1] = OSSIM_DDF_UNIT_TERMINATOR;
    }
    else
    {
        if( GetBinaryFormat() == NotBinary )
        {
            memset( pachData, ' ', nSize );
            memcpy( pachData, pszValue, std::min(nValueLength,nSize) );
        }
        else
        {
            memset( pachData, 0, nSize );
            memcpy( pachData, pszValue, std::min(nValueLength,nSize) );
        }
    }

    return true;
}

/************************************************************************/
/*                           FormatIntValue()                           */
/************************************************************************/

/**
 * Format int subfield value.
 *
 * Returns a buffer with the passed in int value reformatted in a way
 * suitable for storage in a DDFField for this subfield.  
 */

int ossimDDFSubfieldDefn::FormatIntValue( char *pachData, int nBytesAvailable, 
                                     int *pnBytesUsed, int nNewValue )

{
    int nSize;
    char szWork[30];

    sprintf( szWork, "%d", nNewValue );

    if( bIsVariable )
    {
        nSize = (int)strlen(szWork) + 1;
    }
    else
    {                                                                  
        nSize = nFormatWidth;

        if( GetBinaryFormat() == NotBinary && (int) strlen(szWork) > nSize )
            return false;
    }

    if( pnBytesUsed != NULL )
        *pnBytesUsed = nSize;

    if( pachData == NULL )
        return true;

    if( nBytesAvailable < nSize )
        return false;

    if( bIsVariable )
    {
        strncpy( pachData, szWork, nSize-1 );
        pachData[nSize-1] = OSSIM_DDF_UNIT_TERMINATOR;
    }
    else
    {
        ossim_uint32 nMask = 0xff;
        int i;

        switch( GetBinaryFormat() )
        {
          case NotBinary:
            memset( pachData, '0', nSize );
            strncpy( pachData + nSize - strlen(szWork), szWork,
                     strlen(szWork) );
            break;

          case UInt:
          case SInt:
            for( i = 0; i < nFormatWidth; i++ )
            {
                int iOut;

                // big endian required?
                if( pszFormatString[0] == 'B' )
                    iOut = nFormatWidth - i - 1;
                else
                    iOut = i;

                pachData[iOut] = (nNewValue & nMask) >> (i*8);
                nMask *= 256;
            }
            break;

          case FloatReal:
            // CPLAssert( false );
            break;

          default:
            // CPLAssert( false );
            break;
        }
    }

    return true;
}

/************************************************************************/
/*                          FormatFloatValue()                          */
/************************************************************************/

/**
 * Format float subfield value.
 *
 * Returns a buffer with the passed in float value reformatted in a way
 * suitable for storage in a DDFField for this subfield.  
 */

int ossimDDFSubfieldDefn::FormatFloatValue( char *pachData, int nBytesAvailable, 
                                       int *pnBytesUsed, double dfNewValue )

{
    int nSize;
    char szWork[120];

    sprintf( szWork, "%.16g", dfNewValue );

    if( bIsVariable )
    {
        nSize = (int)strlen(szWork) + 1;
    }
    else
    {
        nSize = nFormatWidth;

        if( GetBinaryFormat() == NotBinary && (int) strlen(szWork) > nSize )
            return false;
    }

    if( pnBytesUsed != NULL )
        *pnBytesUsed = nSize;

    if( pachData == NULL )
        return true;

    if( nBytesAvailable < nSize )
        return false;

    if( bIsVariable )
    {
        strncpy( pachData, szWork, nSize-1 );
        pachData[nSize-1] = OSSIM_DDF_UNIT_TERMINATOR;
    }
    else
    {
        if( GetBinaryFormat() == NotBinary )
        {
            memset( pachData, '0', nSize );
            strncpy( pachData + nSize - strlen(szWork), szWork,
                     strlen(szWork) );
        }
        else
        {
            // CPLAssert( false );
            /* implement me */
        }
    }

    return true;
}
