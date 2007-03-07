//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks)
// Description: 
//
//*************************************************************************
// $Id: ossimRpfToc.h,v 1.2 2003/05/13 11:31:10 dburken Exp $
#ifndef osimRpfToc_HEADER
#define osimRpfToc_HEADER
#include <vector>
using namespace std;

#include "base/common/ossimConstants.h" // ossiByteOrder and RPF constants
#include "base/context/ossimErrorContext.h" // for ossimErrorCode

#include "base/data_types/ossimFilename.h"

class ossimRpfHeader;
class ossimRpfBoundaryRectTable;
class ossimRpfTocEntry;

class ossimRpfToc
{
public:
   friend ostream& operator <<(ostream& out,
                               const ossimRpfToc& data);
   ossimRpfToc();
   virtual ~ossimRpfToc();

   ossimErrorCode parseFile(const ossimFilename &fileName);
   void print(ostream& out)const;
   
   unsigned long getNumberOfEntries()const{return theTocEntryList.size();}
   const ossimRpfTocEntry* getTocEntry(unsigned long index)const;
   /*!
    * Returns -1 if not found.
    */
   ossim_int32 getTocEntryIndex(const ossimRpfTocEntry* entry);
   
   const ossimRpfHeader* getRpfHeader()const{return theRpfHeader;}
   
private:
   void deleteAll();
   void clearAll();
   void deleteTocEntryList();
   void buildTocEntryList(ossimRpfHeader* rpfHeader);
   void allocateTocEntryList(unsigned long numberOfEntries);

   /*!
    * This will hold a list of table of content entries.  There is one entry
    * per directory.  Each entry will have its geographic coverage.
    * Each directory is then divided into frames.  There could be 30 or
    * more frame images that make up an entire image.
    */
   vector<ossimRpfTocEntry*> theTocEntryList;
   
   /*!
    * We will remember the file that we opened
    */
   ossimFilename theFilename;

   ossimRpfHeader*                          theRpfHeader;
};

#endif
