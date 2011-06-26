//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: SPADAC Inc
//         Adaptd from the GDAL package hfa dataset
// Description: This class provides some simple utilities for aux file.
//
//********************************************************************
// $Id: ossimAuxFileHandler.h 2644 2011-05-26 15:20:11Z oscar.kramer $

#ifndef ossimAuxFileHandler_HEADER
#define ossimAuxFileHandler_HEADER

#include <cstring>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>

#define EPT_u1	0
#define EPT_u2	1
#define EPT_u4	2
#define EPT_u8	3
#define EPT_s8	4
#define EPT_u16	5
#define EPT_s16	6
#define EPT_u32	7
#define EPT_s32	8
#define EPT_f32	9
#define EPT_f64	10
#define EPT_c64	11
#define EPT_c128 12

class ossimFilename;
class ossimAuxEntry;
class ossimAuxType;
class ossimAuxDictionary;

class ossimAuxInfo{

public:
  ossimAuxInfo();

  virtual ~ossimAuxInfo();   

  FILE *fp;

  ossim_uint32  nEndOfFile;
  ossim_uint32	nRootPos;
  ossim_uint32	nDictionaryPos;

  ossim_int16	nEntryHeaderLength;
  ossim_int32	nVersion;

  ossimAuxEntry* m_Root;

  ossimAuxDictionary*  m_Dictionary;
  char* dictionaryData;
};


class ossimAuxEntry
{
  ossim_uint32	nFilePos;

  ossimAuxInfo*   m_auxInfo;
  ossimAuxEntry*  m_Parent;
  ossimAuxEntry*  m_Prev;
  ossimAuxEntry*  m_Next;
  ossimAuxEntry*  m_Child;

  ossim_uint32	  nNextPos;
  ossim_uint32	  nChildPos;
  
  char	entryNodeName[64];
  char	entryType[32];

  ossimAuxType* m_Type;
  ossim_uint32	nDataPos;
  ossim_uint32	nDataSize;
  char*  entryData;

  void	     loadData();

  int 	     getFieldValue(const char* auxFieldPath,
                           char chReqType, void *reqReturn);

public:
  ossimAuxEntry( ossimAuxInfo* auxInfoIn, ossim_uint32 nPos,
    ossimAuxEntry* parentIn, ossimAuxEntry* prevIn);

  virtual     ~ossimAuxEntry();                

  ossim_uint32	getFilePos() { return nFilePos; }

  const char*   getName() { return entryNodeName; }
  void          setName( const char *nodeName );

  const char*   getType() { return entryType; }

  char*  getData() { loadData(); return entryData; }
  ossim_uint32	getDataPos() { return nDataPos; }
  ossim_uint32	getDataSize() { return nDataSize; }

  ossimAuxEntry*     getChild();
  ossimAuxEntry*     getNext();
  ossimAuxEntry*     getNamedChild( const char * name);

  ossim_int16	  getIntField( const char * auxFieldPath);
  const char*   getStringField( const char * auxFieldPath);

};

class ossimAuxField
{
public:
  int		nBytes;

  int		nItemCount;
  char	chPointer; 	/* '\0', '*' or 'p' */
  char	chItemType;	/* 1|2|4|e|... */

  char	*itemObjectType; /* if chItemType == 'o' */
  ossimAuxType* m_auxItemObjectType;

  char** enumNames; /* normally NULL if not an enum */

  char	*fieldName;

  ossimAuxField();
  ~ossimAuxField();

  int 	extractInstValue( const char * auxField, int nIndexValue,
    char *entryData, ossim_uint32 nDataOffset, int nDataSize,
    char chReqType, void *reqReturn );

  int		getInstCount( char * entryData, int nDataSize );

  int   getInstBytes( char *entryData, int nDataSize );

  const char* initialize( const char * fieldInput );

  void completeDefn( ossimAuxDictionary * auxDict );
};


class ossimAuxType
{
public:
  int		nBytes;
  int		nFields;

  ossimAuxField** m_auxFields;

  char* auxTypeName;

  ossimAuxType();
  ~ossimAuxType();

  const char* initialize( const char * typeInput);

  void	completeDefn( ossimAuxDictionary * auxDict);

  int		getInstBytes(char *entryData, int nDataSize );

  int   extractInstValue( const char * auxField,
    char *entryData, ossim_uint32 nDataOffset, int nDataSize,
    char chReqType, void *reqReturn );
};

class ossimAuxDictionary
{
public:
  int		nTypes;
  int   nTypesMax;
  ossimAuxType**  m_auxTypes;

  ossimAuxDictionary(const char *auxDictStr);
  ~ossimAuxDictionary();

  ossimAuxType* findType( const char * name);
  void          addType( ossimAuxType * type);

  static int	  getItemSize( char chType);
};


class OSSIM_DLL ossimAuxFileHandler
{
public:


  ossimAuxFileHandler();
  virtual ~ossimAuxFileHandler();
  /**
  * @read .aux file
  */
  bool isAuxFile(const ossimFilename& file);

  /**
  * @open the file for reading
  */
  bool open(const ossimFilename& file);

  /**
  *  Reads a single record from a aux file.
  *  Returns true if succesful. Returns false if EOF or error.
  */
  const ossimString& getProjectionName() const { return m_projName; }

  const ossimString& getDatumName() const { return m_datumName; }

  const ossimString& getUnitType() const { return m_unitsType; }

private:

  ossimAuxInfo* m_auxInfo;
  ossimString m_projName;
  ossimString m_datumName;
  ossimString m_unitsType;
};

#endif  // #ifndef ossimAuxFileHandlerl_HEADER
