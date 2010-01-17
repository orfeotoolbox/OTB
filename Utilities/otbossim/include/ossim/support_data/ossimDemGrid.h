//*******************************************************************
//
// License:  See top level LICENSE.txt.
//
// Author: Ken Melero
//         Orginally written by Jamie Moyers (jmoyers@geeks.com)
//         Adapted from the package KDEM.
// Description: This class defines a DEM grid.
//
//********************************************************************
// $Id: ossimDemGrid.h 9305 2006-07-19 17:47:04Z dburken $

#ifndef ossimDemGrid_HEADER
#define ossimDemGrid_HEADER

#include <iostream>
#include <vector>

#include <ossim/base/ossimConstants.h>
#include <ossim/support_data/ossimDemHeader.h>
#include <ossim/support_data/ossimDemProfile.h>

/*!
 *  class ossimDemGrid
 *  This class uses the DEM library to read a DEM and construct a rectangular
 *  grid of signed 16 bit point values.
 */
class ossimDemGrid
{
public:

   /*!
    *  Missing or void data is replaced with missingDataValue.
    */
   ossimDemGrid(ossim_float32 missingDataValue = -32768.0);

   ~ossimDemGrid();

   /*!
    *  Convertes a DEM to a rectangular grid.
    *
    *  If incrementalRead is false, the entire DEM will be converted
    *  with just one call to read. 0 (zero) is returned if successful, -1
    *  if error.
    *
    *  If incrementalRead is true, multiple calls to read are
    *  required, and the return value is as follows:
    *     >0  --  Number of calls remaining until conversion is complete.
    *    ==0  --  Conversion was successful. No more calls to read required.
    *     -1  --  Error during conversion.
    */
   long read(std::istream& dem, bool incrementalRead = false);

   // Accessors
   ossimDemHeader const& getHeader() const;

   /*!
    *  Returns the number of columns in the grid.
    */
   long getWidth() const;

   /*!
    *  Returns the number of rows in the grid.
    */
   long getHeight() const;

   /*!
    *  Returns the elevation value at (x,y). (0,0) is in the upper left
    *  corner, x is positive to the right, y is positive down.
    */
   ossim_float32 getElevation(long x, long y) const;

   /*!
    *  Returns the value used to indicate missing data.
    */
   ossim_float32 getMissingDataValue() const;

   /*!
    *  (x,y) are the coordinates of a elevation value in the dem.
    *  (ground_x, ground_y) are the coordinates of that value in the
    *  units of the horizontal projection system used by the dem.
    */
   void getGroundCoords(long x, long y, double& ground_x, double& ground_y);

private:

   ossim_float32   _missDataVal;
   ossimDemHeader  _header;
   long            _width;
   long            _height;
   ossim_float32*  _grid;
   bool            _firstTime; // Whether or not we've started reading the DEM.
   long            _curProfile;
   std::vector<ossimDemProfile> _profiles;  // Used by fillUTM()
   double _northwest_x, _northwest_y;

   void setElevation(long x, long y, ossim_float32 val);
   long fillGeographic(std::istream& dem, bool incrementalRead);
   long fillUTM(std::istream& dem, bool incrementalRead);
};

inline void ossimDemGrid::setElevation(long x, long y, ossim_float32 val)
{
   if (_grid == 0)
      return;

   _grid[(_width * y) + x] = val;
}

#endif  // #ifndef ossimDemGrid_HEADER
