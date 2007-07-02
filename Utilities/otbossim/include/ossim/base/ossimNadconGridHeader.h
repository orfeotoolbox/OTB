//**************************************************************************
// Copyright (C) 2003 Storage Area Networks, Inc.
//
// Written by:   Kenneth Melero  <kmelero@sanz.com>
//
//**************************************************************************

#ifndef ossimNadconGridHeader_HEADER
#define ossimNadconGridHeader_HEADER
#include <iostream>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimDrect.h>

class ossimNadconGridHeader
{
public:
   friend std::ostream& operator <<(std::ostream& out, const ossimNadconGridHeader& rhs);
   ossimNadconGridHeader()
      :theCols(0),
       theRows(0),
       theZ(0),
       theMinX(0),
       theDx(0),
       theMinY(0),
       theDy(0)
      {
      }
   bool readHeader(const ossimFilename& file);
   bool readHeader(std::istream& in);

   int getStartOffset()const
      {
         return (theCols+2)*4;
      }

   int getBytesPerRow()const
      {
         return (theCols+1)*4;
      }

   int getNumberOfRows()const
      {
         return theRows;
      }
   int getNumberOfCols()const
      {
         return theCols;
      }
   double getDeltaX()const
      {
         return (double)theDx;
      }
   double getDeltaY()const
      {
         return (double)theDy;
      }
   ossimDrect getBoundingRect()const
      {
         return ossimDrect(theMinX,
                           theMinY + (theRows)*theDy,
                           theMinX + (theCols)*theDx,
                           theMinY,
                           OSSIM_RIGHT_HANDED);
      }
   double getMinX()const
      {
         return theMinX;
      }
   double getMinY()const
      {
         return theMinY;
      }
   ossimDpt getSpacing()const
      {
         return ossimDpt(theDx, theDy);
      }
protected:
   int*   theCharBuf[64];
   int   theCols;
   int   theRows;
   int   theZ;
   double theMinX;
   double theDx;
   double theMinY;
   double theDy;
};

#endif
