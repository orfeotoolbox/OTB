//*******************************************************************
// Copyright (C) 2002 ImageLinks Inc. 
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
// 
// Description:
// Class designed to be used by ossimDirectory class.  Should not be used
// directly.
//
//*************************************************************************
// $Id: ossimDirectoryData.h,v 1.2 2004/05/12 17:34:04 dburken Exp $
#ifndef ossimDirectoryData_HEADER
#define ossimDirectoryData_HEADER

#ifndef _MSC_VER

#include <dirent.h>
#include "ossimFilename.h"

class OSSIMDLLEXPORT ossimDirectoryData
{
 public:
   ossimDirectoryData(const ossimFilename& dirname);
   ~ossimDirectoryData();
   
   bool isOk() const { return theDir != NULL; }
   
   void setFlags(int flags);
   int  getFlags() const;
   
   void rewind();
   bool read(ossimFilename &filename);
   
 private:
   DIR*          theDir;
   ossimFilename theDirectoryName;
   int           theFlags;
};

#endif /* #ifndef _MSC_VER */

#endif /* #ifndef ossimDirectoryData_HEADER */
