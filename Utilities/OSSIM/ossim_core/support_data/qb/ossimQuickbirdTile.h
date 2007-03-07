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
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
//*******************************************************************
//  $Id: ossimQuickbirdTile.h,v 1.3 2004/05/06 17:56:21 gpotts Exp $
#ifndef ossimQuickbirdTil_HEADER
#define ossimQuickbirdTil_HEADER
#include <map>
#include <fstream>
#include "base/common/ossimErrorStatusInterface.h"
#include "base/data_types/ossimString.h"
#include "base/data_types/ossimFilename.h"

class ossimQuickbirdTileInfo
{
public:
   friend std::ostream& operator << (std::ostream& out, const ossimQuickbirdTileInfo& rhs)
      {
		return out;
      }
   bool operator ==(const ossimString& rhs)const
      {
         return theFilename == rhs;
      }
   bool operator <(const ossimString& rhs)const
      {
         return theFilename < rhs;
      }
   bool operator >(const ossimString& rhs)const
      {
         return theFilename > rhs;
      }
   bool operator ==(const ossimQuickbirdTileInfo& rhs)const
      {
         return theFilename == rhs.theFilename;
      }
   bool operator <(const ossimQuickbirdTileInfo& rhs)const
      {
         return theFilename < rhs.theFilename;
      }
   bool operator >(const ossimQuickbirdTileInfo& rhs)const
      {
         return theFilename > rhs.theFilename;
      }

   ossimQuickbirdTileInfo()
      {
         initialize();
      }
   
   void initialize()
      {
         theTileGroup = "";
         theFilename  = "";
         theUlXOffset = OSSIM_INT_NAN;
         theUlYOffset = OSSIM_INT_NAN;
         theUrXOffset = OSSIM_INT_NAN;
         theUrYOffset = OSSIM_INT_NAN;
         theLrXOffset = OSSIM_INT_NAN;
         theLrYOffset = OSSIM_INT_NAN;
         theLlXOffset = OSSIM_INT_NAN;
         theLlYOffset = OSSIM_INT_NAN;

         theUlLon     = OSSIM_DBL_NAN;
         theUlLat     = OSSIM_DBL_NAN;
         theUrLon     = OSSIM_DBL_NAN;
         theUrLat     = OSSIM_DBL_NAN;
         theLrLon     = OSSIM_DBL_NAN;
         theLrLat     = OSSIM_DBL_NAN;
         theLlLon     = OSSIM_DBL_NAN;
         theLlLat     = OSSIM_DBL_NAN;
      }
   ossimString    theTileGroup;
   ossimFilename  theFilename;
   ossim_int32    theUlXOffset;
   ossim_int32    theUlYOffset;
   ossim_int32    theUrXOffset;
   ossim_int32    theUrYOffset;
   ossim_int32    theLrXOffset;
   ossim_int32    theLrYOffset;
   ossim_int32    theLlXOffset;
   ossim_int32    theLlYOffset;
   ossim_float64  theUlLon;
   ossim_float64  theUlLat;
   ossim_float64  theUrLon;
   ossim_float64  theUrLat;
   ossim_float64  theLrLon;
   ossim_float64  theLrLat;
   ossim_float64  theLlLon;
   ossim_float64  theLlLat;
};

class ossimQuickbirdTile : public ossimErrorStatusInterface
{
public:
   ossimQuickbirdTile();
   bool open(const ossimFilename tileFile);

   bool getInfo(ossimQuickbirdTileInfo& result,
                const ossimFilename& filename)const;
   
protected:
   std::map<ossimString, ossimQuickbirdTileInfo> theTileMap;
   ossim_int32 theNumberOfTiles;
   ossimString theBandId;
   
   void parseTileGroup(std::istream& in,
                       const ossimString& tileName);
   void parseNameValue(ossimString& name,
                       ossimString& value,
                       const ossimString& line)const;

   
};

#endif
