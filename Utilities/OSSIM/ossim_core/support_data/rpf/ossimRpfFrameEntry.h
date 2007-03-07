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
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfFrameEntry.h,v 1.1 2003/04/02 20:45:00 gpotts Exp $
#ifndef ossimRpfFrameEntry_HEADER
#define ossimRpfFrameEntry_HEADER
#include <iostream>
using namespace std;

#include "base/data_types/ossimFilename.h"
class ossimRpfFrameEntry
{
public:
   friend ostream& operator<<(ostream& out,
                              const ossimRpfFrameEntry& data);
   ossimRpfFrameEntry(const ossimString& rootDirectory=ossimString(""),
                      const ossimString& pathToFrameFileFromRoot=ossimString(""));
   ossimRpfFrameEntry(const ossimRpfFrameEntry& rhs);
   void print(ostream& out)const;
   bool exists()const{return theExists;}
   void setEntry(const ossimString& rootDirectory,
                 const ossimString& pathToFrameFileFromRoot);
   const ossimFilename& getFullPath()const{return theFullValidPath;}
   const ossimString&   getRootDirectory()const{return theRootDirectory;}
   const ossimString    getPathToFrameFileFromRoot()const
      {
         return thePathToFrameFileFromRoot;
      }
   
private:
   bool theExists;

   ossimString theRootDirectory;

   ossimString thePathToFrameFileFromRoot;

   ossimFilename theFullValidPath;
};

#endif
