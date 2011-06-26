//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts
//
// Description:
//
//*******************************************************************
//  $Id: ossimQuickbirdTile.h 19682 2011-05-31 14:21:20Z dburken $
#ifndef ossimQuickbirdTil_HEADER
#define ossimQuickbirdTil_HEADER
#include <map>
#include <fstream>
#include <ossim/base/ossimErrorStatusInterface.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimCommon.h>

class ossimQuickbirdTileInfo
{
public:
   friend std::ostream& operator << (std::ostream& out, const ossimQuickbirdTileInfo& /* rhs */)
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

         theUlLon     = ossim::nan();
         theUlLat     = ossim::nan();
         theUrLon     = ossim::nan();
         theUrLat     = ossim::nan();
         theLrLon     = ossim::nan();
         theLrLat     = ossim::nan();
         theLlLon     = ossim::nan();
         theLlLat     = ossim::nan();
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

   const std::map<std::string, ossimQuickbirdTileInfo>& getMap() const { return theTileMap; }
   
   typedef std::map<std::string, ossimQuickbirdTileInfo> TileMap;

protected:

   TileMap     theTileMap;
   ossim_int32 theNumberOfTiles;
   ossimString theBandId;
   
   void parseTileGroup(std::istream& in,
                       const ossimString& tileName);
   void parseNameValue(ossimString& name,
                       ossimString& value,
                       const ossimString& line)const;

   
};

#endif
