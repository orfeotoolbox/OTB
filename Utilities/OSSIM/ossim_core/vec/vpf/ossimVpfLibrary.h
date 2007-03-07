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
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: This class give the capability to access tiles from an
//              vpf file.
//
//********************************************************************
// $Id: ossimVpfLibrary.h,v 1.3 2004/05/21 21:38:51 dburken Exp $
#ifndef ossimVpfLibrary_HEADER
#define ossimVpfLibrary_HEADER
#include <vector>
#include <map>
#include "base/data_types/ossimString.h"
#include "base/data_types/ossimFilename.h"
#include "base/context/ossimErrorContext.h" // for ossimErrorCode

class ossimVpfCoverage;
class ossimVpfDatabase;
class ossimVpfExtent;

class OSSIMDLLEXPORT ossimVpfLibrary
{
public:
   ossimVpfLibrary();
   virtual ~ossimVpfLibrary();
   bool openLibrary(ossimVpfDatabase* database,
                    const ossimString& name,
                    const ossimFilename& libraryPath);
   void              setDatabase(ossimVpfDatabase* database);
   ossimVpfDatabase* getDatabase()const;
   bool getCoverage(long coverageNumber, ossimVpfCoverage& coverage);
   bool getCoverage(const ossimString& name, ossimVpfCoverage& coverage);

   bool                getExtent(ossimVpfExtent& result)const;
   bool                hasGazette()const;

   ossimFilename       getPath()const;
   ossimString         getName()const;
   bool getCoverageNames(std::vector<ossimString>& coverageNames)const;
   int getNumberOfCoverages()const;

   bool hasCoverage(const ossimString& coverageName)const;
   ossimString getTileName(ossim_int32 id)const;
   void getTileNames(std::vector<ossimString>& tileNames)const;

private:
   ossimVpfDatabase*        theDatabase;
   ossimString              theLibraryName;
   ossimFilename            theLibraryNameFullPath;
   ossim_uint32             theNumberOfCoverages;
   std::vector<ossimString> theCoverageNames;
   mutable std::map<ossim_int32, ossimString> theTileNameMap;
   void setCoverageNames();
   void setTileNames()const;
};

#endif
