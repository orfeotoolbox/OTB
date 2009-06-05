//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
// 
// Class declaration for ossimFileParser.
// 
// This class provides tools for parsinf a file.
//
//********************************************************************
// $Id: ossimFileParser.h 14412 2009-04-27 16:58:46Z dburken $

#ifndef ossimFileParser_HEADER
#define ossimFileParser_HEADER

#include <iosfwd>

#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimString.h>

class ossimFilename;


class OSSIMDLLEXPORT ossimFileParser : public ossimObject
{
public:

   /** @brief default constructor */
   ossimFileParser();

   /** virtual destructor */
   virtual ~ossimFileParser();


  bool getEndOfLine( char * fileBuf, ossimString lineBeginning, char * format, ossimString & name );

  bool getEndOfLine( char * fileBuf, ossimString lineBeginning, char * format, ossim_float64 & value);
 



TYPE_DATA
};

#endif /* #ifndef ossimFileParser_HEADER */
