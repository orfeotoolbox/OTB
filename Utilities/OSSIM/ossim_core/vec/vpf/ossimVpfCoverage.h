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
// $Id: ossimVpfCoverage.h,v 1.5 2004/05/21 21:38:51 dburken Exp $
#ifndef ossimVpfCoverage_HEADER
#define ossimVpfCoverage_HEADER
#include <map>
#include <vector>
#include "base/data_types/ossimFilename.h"
#include "base/context/ossimErrorContext.h"
#include "ossimVpfTable.h"
#include "ossimVpfFeatureClass.h"
#include "ossimVpfExtent.h"

class ossimVpfLibrary;
class ossimVpfFeatureClassSchema;
class OSSIMDLLEXPORT ossimVpfCoverage
{
public:
   ossimVpfCoverage ();
   virtual ~ossimVpfCoverage();
   
   bool openCoverage(ossimVpfLibrary* library,
                     const ossimString& name,
                     const ossimFilename& fileName);
   const ossimFilename& getPath()const;
   const ossimString& getName()const;
   
   ossim_uint32 getNumberOfFeatureClasses()const;
   void print(ostream& out)const;

   bool getExtent(ossimVpfExtent& extent)const;
   ossimVpfLibrary* getLibrary();
   bool openFeatureClassSchema(ossimVpfFeatureClassSchema& schema);
   
private:
   ossimVpfLibrary* theLibrary;
   ossimString      theCoverageName;

   /*!
    * will hold the full path to this coverage.
    */
   ossimFilename theCoverageNameFullPath;
   
};

#endif
