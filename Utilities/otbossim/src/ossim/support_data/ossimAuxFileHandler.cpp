//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: SPADAC Inc
//         Adaptd from the GDAL package hfadataset.cpp
// Description: This class provides some simple utilities for aux file.
//
//********************************************************************
// $Id: ossimAuxFileHandler.cpp 2644 2011-05-26 15:20:11Z oscar.kramer $

#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <ossim/support_data/ossimAuxFileHandler.h>

#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
using namespace std;

void ossimAuxStandardFile( int nBytes, void * pData )

{
  int		i;
  char	*entryData = (char *) pData;

  for( i = nBytes/2-1; i >= 0; i-- )
  {
    ossim_uint8	byTemp;

    byTemp = entryData[i];
    entryData[i] = entryData[nBytes-i-1];
    entryData[nBytes-i-1] = byTemp;
  }
}

int ossimAuxGetDataTypeBits( int nDataType )

{
  switch( nDataType )
  {
  case EPT_u1:
    return 1;

  case EPT_u2:
    return 2;

  case EPT_u4:
    return 4;

  case EPT_u8:
  case EPT_s8:
    return 8;

  case EPT_u16:
  case EPT_s16:
    return 16;

  case EPT_u32:
  case EPT_s32:
  case EPT_f32:
    return 32;

  case EPT_f64:
  case EPT_c64:
    return 64;

  case EPT_c128:
    return 128;
  }

  return 0;
}

static char * ossimAuxGetDictionary(ossimAuxInfo* hHFA)

{
  int		nDictMax = 100;
  char	*pszDictionary = (char *)malloc(nDictMax);
  int		nDictSize = 0;

  fseek(hHFA->fp, hHFA->nDictionaryPos, SEEK_SET );

  while( true )
  {
    if( nDictSize >= nDictMax-1 )
    {
      nDictMax = nDictSize * 2 + 100;
      pszDictionary = (char *) realloc(pszDictionary, nDictMax );
    }

    if( fread( pszDictionary + nDictSize, 1, 1, hHFA->fp ) < 1
      || pszDictionary[nDictSize] == '\0'
      || (nDictSize > 2 && pszDictionary[nDictSize-2] == ','
      && pszDictionary[nDictSize-1] == '.') )
      break;

    nDictSize++;
  }

  pszDictionary[nDictSize] = '\0';

  return( pszDictionary );
}

ossimAuxFileHandler::ossimAuxFileHandler()
{
  m_auxInfo = NULL;
}

ossimAuxFileHandler::~ossimAuxFileHandler()
{
  if (m_auxInfo != NULL)
  {
    delete m_auxInfo;
    m_auxInfo = 0;
  }
}

bool ossimAuxFileHandler::isAuxFile(const ossimFilename& file)
{
  bool result = false;

  if ( file.exists() )
  { 
    ossimString ext = file.ext();
    ext.downcase();
    if (ext == "aux")
    {
      result = true;
    }
    else
    {
      result = false;
    } 
  } 

  return result;
}

bool ossimAuxFileHandler::open(const ossimFilename& file)
{
  FILE	*fp = fopen(file.c_str(), "r");
  char	header[16];
  ossim_uint32	nHeaderPos;

  if( fp == NULL )
  {
    return false;
  }

  fread( header, 16, 1, fp);
  
  char* tmpHeader = const_cast<char*>(header);
  if(!strcmp(tmpHeader,"EHFA_HEADER_TAG") == 0)
  {
    return false;
  }

  //Create the ossimAuxInfo                                          
  m_auxInfo = new ossimAuxInfo();
  m_auxInfo->fp = fp;
 
  //Where is the header?						
  fread( &nHeaderPos, sizeof(ossim_int32), 1, fp );

  fseek( fp, nHeaderPos, SEEK_SET );

  fread( &(m_auxInfo->nVersion), sizeof(ossim_int32), 1, fp );
 
  fread( header, 4, 1, fp ); /* skip freeList */

  fread( &(m_auxInfo->nRootPos), sizeof(ossim_int32), 1, fp );
 
  fread( &(m_auxInfo->nEntryHeaderLength), sizeof(ossim_int16), 1, fp );
  fread( &(m_auxInfo->nDictionaryPos), sizeof(ossim_int32), 1, fp );
  
  fseek( fp, 0, SEEK_END );
  m_auxInfo->nEndOfFile = (ossim_int32) ftell(fp);

  m_auxInfo->m_Root = new ossimAuxEntry( m_auxInfo, m_auxInfo->nRootPos, NULL, NULL );

  m_auxInfo->dictionaryData = ossimAuxGetDictionary(m_auxInfo);
  m_auxInfo->m_Dictionary = new ossimAuxDictionary( m_auxInfo->dictionaryData );

  ossimAuxEntry	*node = m_auxInfo->m_Root->getChild();
  while( node != NULL )
  {
    std::string typeStr = node->getType();
    size_t typeLen = typeStr.length();
    std::string str = "Eimg_Layer";
    size_t strLen = str.length();
    if( typeLen == strLen)
    {
      ossimAuxEntry* projEntry = node->getNamedChild("Map_Info");
      if( projEntry == NULL )
      {
         ossimAuxEntry* childEntry;
         for( childEntry = node->getChild(); childEntry != NULL && projEntry == NULL; childEntry = childEntry->getNext() )
         {
            if (ossimString(childEntry->getType()) == "Eprj_MapInfo")
               projEntry = childEntry;
         }
      }

      const char* proj = NULL;
      const char* units = NULL;
      if (projEntry)
      {
        proj = projEntry->getStringField("proName");
        units = projEntry->getStringField("units");
      }
      
      if (proj != NULL)
      {
        m_projName = ossimString(proj);
      }
      if (units != NULL)
      {
         m_unitsType = ossimString(proj);
      }

      ossimAuxEntry* datumEntry = node->getNamedChild( "Projection.Datum" );
      const char* datumStr = NULL;
      if (datumEntry)
      {
        datumStr = datumEntry->getStringField("datumname");
      }
      if (datumStr != NULL)
      {
        m_datumName = ossimString(datumStr);;
      }
      break;
    }
    node = node->getNext();
  }

  fclose(fp);
  return true;
}

/************************************************************************/
/*                              ossimAuxInfo()                              */
/************************************************************************/
ossimAuxInfo::ossimAuxInfo()
{
  fp = NULL;
  m_Root = NULL;
  m_Dictionary = NULL;
  dictionaryData = NULL;
}

ossimAuxInfo::~ossimAuxInfo()
{
  if (m_Root != NULL)
  {
    delete m_Root;
    m_Root = 0;
  }

  if (m_Dictionary != NULL)
  {
    delete m_Dictionary;
    m_Dictionary = 0;
  }

  if (dictionaryData != NULL)
  {
    delete[] dictionaryData;
    dictionaryData = 0;
  }
}

/************************************************************************/
/*                              ossimAuxEntry()                              */
/************************************************************************/

ossimAuxEntry::ossimAuxEntry(ossimAuxInfo * auxInfoIn, ossim_uint32 nPos,
                   ossimAuxEntry * parentIn, ossimAuxEntry * prevIn)

{
  m_auxInfo = auxInfoIn;

  nFilePos = nPos;
  
  m_Parent = parentIn;
  m_Prev = prevIn;

  m_Next = NULL;
  m_Child = NULL;
  entryData = NULL;

  nDataPos = nDataSize = 0;
  nNextPos = nChildPos = 0;

  entryNodeName[0] = entryType[0] = '\0';

  ossim_int32	anEntryNums[6];
 
  if( fseek( m_auxInfo->fp, nFilePos, SEEK_SET ) == -1
    || fread( anEntryNums, sizeof(ossim_uint32), 6, m_auxInfo->fp ) < 1 )
  {
    return;
  }

  nNextPos = anEntryNums[0];
  nChildPos = anEntryNums[3];
  nDataPos = anEntryNums[4];
  nDataSize = anEntryNums[5];

  if( fread( entryNodeName, 1, 64, m_auxInfo->fp ) < 1
    || fread( entryType, 1, 32, m_auxInfo->fp ) < 1 )
  {
    return;
  }
}
ossimAuxEntry::~ossimAuxEntry()
{
  if( entryData != NULL )
  {
    delete entryData;
    entryData = 0;
  }

  if( m_Next != NULL )
  {
    delete m_Next;
    m_Next = 0;
  }

  if( m_Child != NULL )
  {
    delete m_Child;
    m_Child = 0;
  }
}

void ossimAuxEntry::setName( const char *nodeName )
{
  memset( entryNodeName, 0, 64 );
  strncpy( entryNodeName, nodeName, 64 );
}

ossimAuxEntry *ossimAuxEntry::getChild()
{
  if( m_Child == NULL && nChildPos != 0 )
  {
    m_Child = new ossimAuxEntry( m_auxInfo, nChildPos, this, NULL );
  }

  return( m_Child );
}

ossimAuxEntry* ossimAuxEntry::getNext()
{
  if( m_Next == NULL && nNextPos != 0 )
  {
    // Check if we have a loop on the next node in this sibling chain.
    ossimAuxEntry *past;

    for( past = this; 
      past != NULL && past->nFilePos != nNextPos; 
      past = past->m_Prev ) {}

    if( past != NULL )
    {
      nNextPos = 0;
      return NULL;
    }

    m_Next = new ossimAuxEntry(m_auxInfo, nNextPos, m_Parent, this );
  }

  return( m_Next );
}

void ossimAuxEntry::loadData()
{
  if( entryData != NULL || nDataSize == 0 )
  {
    return;
  }

  entryData = (char*) malloc(nDataSize);
  if (entryData == NULL)
  {
    return;
  }

  if(fseek(m_auxInfo->fp, nDataPos, SEEK_SET ) < 0 )
  {
    return;
  }

  if( fread( entryData, 1, nDataSize, m_auxInfo->fp ) < 1 )
  {
    return;
  }

  m_Type = m_auxInfo->m_Dictionary->findType( entryType );
  if(m_Type == NULL)
  {
    return;
  }
}

ossimAuxEntry* ossimAuxEntry::getNamedChild( const char* name )
{
  int		nNameLen;
  ossimAuxEntry	*entry;

  for( nNameLen = 0;
    name[nNameLen] != '.'
    && name[nNameLen] != '\0'
    && name[nNameLen] != ':';
  nNameLen++ ) {}

  
  for( entry = getChild(); entry != NULL; entry = entry->getNext() )
  {
    std::string tmpEntryName = const_cast<char*>(entry->getName());
    tmpEntryName = tmpEntryName.substr(0, nNameLen);
    std::string tmpName = const_cast<char*>(name);
    tmpName = tmpName.substr(0, nNameLen);
    if( strcmp(tmpEntryName.c_str(),tmpName.c_str()) == 0
      && (int) strlen(entry->getName()) == nNameLen )
    {
      if( name[nNameLen] == '.' )
      {
        ossimAuxEntry *result;

        result = entry->getNamedChild( name+nNameLen+1 );
        if( result != NULL )
          return result;
      }
      else
        return entry;
    }
  }

  return NULL;
}

int ossimAuxEntry::getFieldValue( const char * auxFieldPath,
                            char chReqType, void *reqReturn )
{
  ossimAuxEntry	*entry = this;

  if( strchr(auxFieldPath,':') != NULL )
  {
    entry = getNamedChild( auxFieldPath );
    if( entry == NULL )
      return false;

    auxFieldPath = strchr(auxFieldPath,':') + 1;
  }

  loadData();

  if( entryData == NULL )
  {
    return false;
  }

  if( m_Type == NULL )
  {
    return false;
  }

  return (m_Type->extractInstValue( auxFieldPath,
    entryData, nDataPos, nDataSize,
    chReqType, reqReturn ));
}

ossim_int16 ossimAuxEntry::getIntField(const char* auxFieldPath)
{
  ossim_int16	nIntValue;

  if( !getFieldValue( auxFieldPath, 'i', &nIntValue ) )
  {
    return 0;
  }
  else
  {
    return nIntValue;
  }
}

const char* ossimAuxEntry::getStringField( const char* auxFieldPath)
{
  char *result = NULL;

  if( !getFieldValue( auxFieldPath, 's', &result ) )
  {
    return NULL;
  }
  else
  {
    return result;
  }
}

/************************************************************************/
/*                              ossimAuxField()                              */
/************************************************************************/

ossimAuxField::ossimAuxField()
{
  nBytes = 0;

  nItemCount = 0;
  chPointer = '\0';
  chItemType = '\0';

  itemObjectType = NULL;
  m_auxItemObjectType = NULL;

  enumNames = NULL;
  fieldName = NULL;
}

ossimAuxField::~ossimAuxField()
{
  if (itemObjectType != NULL)
  {
    delete[] itemObjectType;
    itemObjectType = 0;
  }

  if (enumNames != NULL)
  {
    delete[] enumNames;
    enumNames = 0;
  }

  if (fieldName != NULL)
  {
    delete[] fieldName;
    fieldName = 0;
  }
}

int ossimAuxField::extractInstValue( const char* /* auxField */, int nIndexValue,
                           char* entryData, ossim_uint32 nDataOffset, int nDataSize,
                           char chReqType, void *reqReturn )

{
  int			nInstItemCount = getInstCount( entryData, nDataSize );
  
  if( nIndexValue < 0 || nIndexValue >= nInstItemCount )
  {
    if( chItemType == 'b' && nIndexValue >= -3 && nIndexValue < 0 )
      /* ok - special index values */;
    else
      return false;
  }

  if( chPointer != '\0' )
  {
    ossim_uint32	nOffset;

    memcpy( &nOffset, entryData+4, 4 );
  
    entryData += 8;

    nDataOffset += 8;
    nDataSize -= 8;
  }

  if( (chItemType == 'c' || chItemType == 'C') && chReqType == 's' )
  {
    *((char **)reqReturn) = entryData;
    return( entryData != NULL );
  }

  return false;
}

int ossimAuxField::getInstCount( char * entryData, int nDataSize )
{
  if( chPointer == '\0' )
    return nItemCount;
  else if( chItemType == 'b' )
  {
    ossim_int32 nRows, nColumns;

    if( nDataSize < 20 )
      return 0;

    memcpy( &nRows, entryData+8, 4 );
    ossimAuxStandardFile( 4, &nRows );
    memcpy( &nColumns, entryData+12, 4 );
    ossimAuxStandardFile( 4, &nColumns );

    return nRows * nColumns;
  }
  else
  {
    ossim_int32 nCount;

    if( nDataSize < 4 )
      return 0;

    memcpy( &nCount, entryData, 4 );
    ossimAuxStandardFile( 4, &nCount );
    return nCount;
  }
}

void ossimAuxField::completeDefn( ossimAuxDictionary * auxDict )

{
  if( itemObjectType != NULL )
  {
    m_auxItemObjectType = auxDict->findType( itemObjectType );
  }

  if( chPointer == 'p' )
  {
    nBytes = -1; /* we can't know the instance size */
  }
  else if( m_auxItemObjectType != NULL )
  {
    m_auxItemObjectType->completeDefn( auxDict );
    if( m_auxItemObjectType->nBytes == -1 )
    {
      nBytes = -1;
    }
    else
    {
      nBytes = m_auxItemObjectType->nBytes * nItemCount;
    }

    if( chPointer == '*' && nBytes != -1 )
    {
      nBytes += 8; /* count, and offset */
    }
  }
  else
  {
    nBytes = auxDict->getItemSize( chItemType ) * nItemCount;
  }
}

const char *ossimAuxField::initialize( const char * fieldInput )
{
  int		i;
  nItemCount = atoi(fieldInput);

  while( *fieldInput != '\0' && *fieldInput != ':' )
  {
    fieldInput++;
  }

  if( *fieldInput == '\0' )
  {
    return NULL;
  }

  fieldInput++;

  if( *fieldInput == 'p' || *fieldInput == '*' )
    chPointer = *(fieldInput++);

  if( *fieldInput == '\0' )
    return NULL;

  chItemType = *(fieldInput++);

  if( chItemType == 'o' )
  {
    for( i = 0; fieldInput[i] != '\0' && fieldInput[i] != ','; i++ ) {}

    itemObjectType = (char *) malloc(i+1);
    strncpy( itemObjectType, fieldInput, i );
    itemObjectType[i] = '\0';

    fieldInput += i+1;
  }

  if( chItemType == 'x' && *fieldInput == '{' )
  {
    int nBraceDepth = 1;
    fieldInput++;

    // Skip past the definition.
    while( nBraceDepth > 0 && *fieldInput != '\0' )
    {
      if( *fieldInput == '{' )
        nBraceDepth++;
      else if( *fieldInput == '}' )
        nBraceDepth--;

      fieldInput++;
    }

    chItemType = 'o';

    // find the comma terminating the type name.
    for( i = 0; fieldInput[i] != '\0' && fieldInput[i] != ','; i++ ) {}

    itemObjectType = (char *) malloc(i+1);
    strncpy( itemObjectType, fieldInput, i );
    itemObjectType[i] = '\0';

    fieldInput += i+1;
  }

  if( chItemType == 'e' )
  {
    int	nEnumCount = atoi(fieldInput);
    int	iEnum;

    fieldInput = strchr(fieldInput,':');
    if( fieldInput == NULL )
      return NULL;

    fieldInput++;

    enumNames = (char **) calloc(sizeof(char *), nEnumCount+1);

    for( iEnum = 0; iEnum < nEnumCount; iEnum++ )
    {
      char	*pszToken;

      for( i = 0; fieldInput[i] != '\0' && fieldInput[i] != ','; i++ ) {}

      if( fieldInput[i] != ',' )
        return NULL;

      pszToken = (char *) malloc(i+1);
      strncpy( pszToken, fieldInput, i );
      pszToken[i] = '\0';

      enumNames[iEnum] = pszToken;

      fieldInput += i+1;
    }
  }

  for( i = 0; fieldInput[i] != '\0' && fieldInput[i] != ','; i++ ) {}

  fieldName = (char *) malloc(i+1);
  strncpy( fieldName, fieldInput, i );
  fieldName[i] = '\0';

  fieldInput += i+1;

  return( fieldInput );
}

int ossimAuxField::getInstBytes( char *entryData, int nDataSize )

{
  int		nCount;
  int		nInstBytes = 0;

  if( nBytes > -1 )
    return nBytes;

  if( chPointer != '\0' )
  {
    memcpy( &nCount, entryData, 4 );
    ossimAuxStandardFile( 4, &nCount );

    entryData += 8;
    nInstBytes += 8;
  }
  else
    nCount = 1;

  if( chItemType == 'b' && nCount != 0 ) // BASEDATA
  {
    ossim_int32 nRows, nColumns;
    ossim_int16 nBaseItemType;

    memcpy( &nRows, entryData, 4 );
    ossimAuxStandardFile( 4, &nRows );
    memcpy( &nColumns, entryData+4, 4 );
    ossimAuxStandardFile( 4, &nColumns );
    memcpy( &nBaseItemType, entryData+8, 2 );
    ossimAuxStandardFile( 2, &nBaseItemType );

    nInstBytes += 12;

    nInstBytes += 
      ((ossimAuxGetDataTypeBits(nBaseItemType) + 7) / 8) * nRows * nColumns;
  }
  else if( m_auxItemObjectType == NULL )
  {
    nInstBytes += nCount * ossimAuxDictionary::getItemSize(chItemType);
  }
  else
  {
    int		i;

    for( i = 0; i < nCount; i++ )
    {
      int	nThisBytes;

      nThisBytes =
        m_auxItemObjectType->getInstBytes( entryData,
        nDataSize - nInstBytes );
      nInstBytes += nThisBytes;
      entryData += nThisBytes;
    }
  }

  return( nInstBytes );
}


/************************************************************************/
/*                              ossimAuxType()                              */
/************************************************************************/
ossimAuxType::ossimAuxType()
{
  nBytes = 0;
  nFields = 0;
  m_auxFields = NULL;
  auxTypeName = NULL;
}

ossimAuxType::~ossimAuxType()
{
  for(int i = 0; i < nFields; i++ )
  {
    delete m_auxFields[i];
    m_auxFields[i] = 0;
  }

  if (auxTypeName != NULL)
  {
    delete[] auxTypeName;
    auxTypeName = 0;
  }
}

const char* ossimAuxType::initialize( const char * typeInput )
{
  int		i;
  if( *typeInput != '{' )
  {
    while( *typeInput != '{' && *typeInput != '\0' )
      typeInput++;

    if( *typeInput == '\0' )
      return NULL;
  }

  typeInput++;

  while( typeInput != NULL && *typeInput != '}' )
  {
    ossimAuxField	*newField = new ossimAuxField();

    typeInput = newField->initialize( typeInput );
    if( typeInput != NULL )
    {
      m_auxFields = (ossimAuxField **)realloc(m_auxFields, sizeof(void*) * (nFields+1) );
      m_auxFields[nFields++] = newField;
    }
    else
    {
      delete newField;
    }
  }

  if( typeInput == NULL )
  {
    return NULL;
  }

  typeInput++; /* skip `}' */

  for( i = 0; typeInput[i] != '\0' && typeInput[i] != ','; i++ ) {}

  auxTypeName = (char *) malloc(i+1);
  strncpy( auxTypeName, typeInput, i );
  auxTypeName[i] = '\0';

  typeInput += i+1;

  return( typeInput );
}

void ossimAuxType::completeDefn( ossimAuxDictionary* auxDict )
{
  int		i;

  if( nBytes != 0 )
    return;

  for( i = 0; i < nFields; i++ )
  {
    m_auxFields[i]->completeDefn( auxDict );
    if( m_auxFields[i]->nBytes < 0 || nBytes == -1 )
    {
      nBytes = -1;
    }
    else
    {
      nBytes += m_auxFields[i]->nBytes;
    }
  }
}

int ossimAuxType::extractInstValue( const char * auxFieldPath,
                          char *entryData, ossim_uint32 nDataOffset, int nDataSize,
                          char chReqType, void *reqReturn )

{
  int		nArrayIndex = 0, nNameLen, iField, nByteOffset;
  const char	*remainder;

  const char *firstArray = strchr(auxFieldPath,'[');
  const char *firstDot = strchr(auxFieldPath,'.');

  if( firstArray != NULL
    && (firstDot == NULL
    || firstDot > firstArray) )
  {
    const char	*theEnd = firstArray;

    nArrayIndex = atoi(theEnd+1);
    nNameLen = theEnd - auxFieldPath;

    remainder = strchr(auxFieldPath,'.');
    if( remainder != NULL )
    {
      remainder++;
    }
  }
  else if( firstDot != NULL )
  {
    const char	*theEnd = firstDot;

    nNameLen = theEnd - auxFieldPath;

    remainder = theEnd + 1;
  }
  else
  {
    nNameLen = (int)strlen(auxFieldPath);
    remainder = NULL;
  }

  //Find this field within this type, if possible.                 
  nByteOffset = 0;
  for( iField = 0; iField < nFields && nByteOffset < nDataSize; iField++ )
  {
    std::string tmpFieldPath = const_cast<char*>(auxFieldPath);
    tmpFieldPath = tmpFieldPath.substr(0, nNameLen);
    std::string tmpFieldName = const_cast<char*>(m_auxFields[iField]->fieldName);
    tmpFieldName = tmpFieldName.substr(0, nNameLen);
    if( strcmp(tmpFieldPath.c_str(),tmpFieldName.c_str()) == 0
      && m_auxFields[iField]->fieldName[nNameLen] == '\0' )
    {
      break;
    }

    nByteOffset +=
      m_auxFields[iField]->getInstBytes( entryData + nByteOffset,
      nDataSize - nByteOffset );
  }

  if( iField == nFields || nByteOffset >= nDataSize )
  {
    return false;
  }

  //Extract this field value, and return.                          
  return( m_auxFields[iField]->
    extractInstValue( remainder, nArrayIndex,
    entryData + nByteOffset,
    nDataOffset + nByteOffset,
    nDataSize - nByteOffset,
    chReqType, reqReturn ) );
}

int ossimAuxType::getInstBytes( char* entryData, int nDataSize )
{
  if( nBytes >= 0 )
  {
    return( nBytes );
  }
  else
  {
    int	nTotal = 0;
    int	iField;

    for( iField = 0; iField < nFields && nTotal < nDataSize; iField++ )
    {
      ossimAuxField	*poField = m_auxFields[iField];

      int nInstBytes = poField->getInstBytes( entryData,
        nDataSize - nTotal );

      entryData += nInstBytes;
      nTotal += nInstBytes;
    }

    return( nTotal );
  }
}

/************************************************************************/
/*                              ossimAuxDictionary()                              */
/************************************************************************/
static const char *defDefn[] = {
  "Eprj_MapInfo", 
  "{0:pcproName,1:*oEprj_Coordinate,upperLeftCenter,1:*oEprj_Coordinate,lowerRightCenter,1:*oEprj_Size,pixelSize,0:pcunits,}Eprj_MapInfo",
  NULL,
  NULL };

ossimAuxDictionary::ossimAuxDictionary( const char* auxDictStr )
{
  int		i;
  nTypes = 0;
  nTypesMax = 0;
  m_auxTypes = NULL;

  //Read all the types.            
  while( auxDictStr != NULL && *auxDictStr != '.' )
  {
    ossimAuxType* newType = new ossimAuxType();
    auxDictStr = newType->initialize( auxDictStr );

    if( auxDictStr != NULL )
    {
      addType( newType );
    }
    else
    {
      delete newType;
    }
  }

  for( i = 0; defDefn[i] != NULL; i += 2 )
  {
    if( findType( defDefn[i] ) == NULL )
    {
      ossimAuxType *newType = new ossimAuxType();

      newType->initialize( defDefn[i+1] );
      addType( newType );
    }
  }

  //Complete the definitions.    
  for( i = 0; i < nTypes; i++ )
  {
    m_auxTypes[i]->completeDefn( this );
  }
}

ossimAuxDictionary::~ossimAuxDictionary()
{
  for(int i = 0; i < nTypes; i++ )
  {
    delete m_auxTypes[i];
    m_auxTypes[i] = 0;
  }
}

void ossimAuxDictionary::addType( ossimAuxType *type )

{
  if( nTypes == nTypesMax )
  {
    nTypesMax = nTypes * 2 + 10;
    m_auxTypes = (ossimAuxType **) realloc( m_auxTypes,
      sizeof(void*) * nTypesMax );
  }

  m_auxTypes[nTypes++] = type;
}

ossimAuxType* ossimAuxDictionary::findType( const char * name )

{
  int		i;

  for( i = 0; i < nTypes; i++ )
  {
    if( strcmp(name, m_auxTypes[i]->auxTypeName) == 0 )
    {
      return( m_auxTypes[i] );
    }
  }

  return NULL;
}

int ossimAuxDictionary::getItemSize( char chType )

{
  switch( chType )
  {
  case '1':
  case '2':
  case '4':
  case 'c':
  case 'C':
    return 1;

  case 'e':
  case 's':
  case 'S':
    return 2;

  case 't':
  case 'l':
  case 'L':
  case 'f':
    return 4;

  case 'd':
  case 'm':
    return 8;

  case 'M':
    return 16;

  case 'b':
    return -1;

  case 'o':
  case 'x':
    return 0;
  }

  return 0;
}
