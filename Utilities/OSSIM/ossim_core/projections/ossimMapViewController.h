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
//
//*************************************************************************
// $Id: ossimMapViewController.h,v 1.7 2003/05/13 11:31:10 dburken Exp $
#ifndef ossimMapViewController_HEADER
#define ossimMapViewController_HEADER
#include "base/common/ossimViewController.h"
#include "base/data_types/ossimFilename.h"

class ossimMapViewController : public ossimViewController
{
public:
   ossimMapViewController();
   ossimMapViewController(ossimObject* owner,
                          ossim_uint32 inputListSize,
                          ossim_uint32 outputListSize,
                          bool         inputListFixedFlag=true,
                          bool         outputListFixedFlag=false);

   virtual ~ossimMapViewController();
   virtual bool setView(ossimObject* object);
   virtual void changeOwner(ossimObject* owner);
   virtual void setFilename(const ossimFilename& file);
   virtual const ossimFilename& getFilename()const
      {
         return theGeometryFile;
      }
   
   virtual ossimString getShortName()const
      {
         return ossimString("Map View");
      }
   
   virtual ossimString getLongName()const
      {
         return ossimString("Map View controller");
      }
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
protected:

   /*!
    * If the file is not "" (empty) then it will
    * use this filename to load and save the geometry
    * projection.
    */
   ossimFilename theGeometryFile;
   
TYPE_DATA
};
#endif
