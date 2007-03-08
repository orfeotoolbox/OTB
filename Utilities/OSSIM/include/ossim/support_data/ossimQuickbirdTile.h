//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
//*******************************************************************
//  $Id: ossimQuickbirdTile.h 9967 2006-11-29 02:01:23Z gpotts $
#ifndef ossimQuickbirdTil_HEADER
#define ossimQuickbirdTil_HEADER
#include <map>
#include <fstream>
#include <ossim/base/ossimErrorStatusInterface.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimFilename.h>

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
