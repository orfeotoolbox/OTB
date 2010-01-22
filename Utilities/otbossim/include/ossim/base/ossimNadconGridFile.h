//**************************************************************************
// Copyright (C) 2003 Storage Area Networks, Inc.
//
// Written by:   Kenneth Melero  <kmelero@sanz.com>
//
//**************************************************************************

#ifndef ossimNadconGridFile_HEADER
#define ossimNadconGridFile_HEADER
#include <ossim/base/ossimNadconGridHeader.h>
#include <fstream>

class ossimNadconGridFile
{
public:
  ossimNadconGridFile():
    theFileOkFlag(false)
      {}
   ~ossimNadconGridFile();
   bool open(const ossimFilename& file);
   void close();

   /*!
    */
   double getShiftAtLatLon(double lat, double lon)const;
   bool pointWithin(double lat, double lon)const;
   const ossimDrect getBoundingRect()const
      {
         return theBoundingRect;
      }
   
   const ossimFilename& getFilename()const
      {
         return theFilename;
      }
   bool getFileOkFlag()const
      {
         return theFileOkFlag;
      }
   ossimDpt getSpacing()const
      {
         return theHeader.getSpacing();
      }
   
protected:
   mutable std::ifstream theInputFile;
  mutable bool theFileOkFlag;
  ossimFilename         theFilename;
   ossimNadconGridHeader theHeader;
   ossimDrect            theBoundingRect;
  ossimDpt               theLatLonOrigin;
};

#endif
