//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Ken Melero (kmelero@imagelinks.com)
//         Orginally written by Jamie Moyers (jmoyers@geeks.com)
//         Adapted from the package KDEM.
// Description: This class defines a DEM grid.
//
//********************************************************************
// $Id: ossimDemGrid.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <ossim/support_data/ossimDemGrid.h>
#include <ossim/support_data/ossimDemPoint.h>
#include <ossim/support_data/ossimDemUtil.h>
#include <ossim/base/ossimNotifyContext.h>

ossimDemGrid::ossimDemGrid(ossim_float32 missingDataValue)
   : _missDataVal(missingDataValue),
     _width(0),
     _height(0),
     _grid(0),
     _firstTime(true),
     _curProfile(0)
{        
}

ossimDemGrid::~ossimDemGrid()
{
   if(_grid) delete [] _grid; _grid =0;
}

long
ossimDemGrid::read(std::istream& dem, bool incrementalRead)
{
   if (_firstTime)
   {
      dem >> _header;
   }

   long retval;
   if (_header.getGroundRefSysCode() == 0)  // Geographic
      retval = fillGeographic(dem,incrementalRead);
   else
      retval = fillUTM(dem,incrementalRead);   // This may not work if it's 
                                               // really in State Plane.

   if (_firstTime)
      _firstTime = false;

   return retval;
}

ossimDemHeader const&
ossimDemGrid::getHeader() const
{
   return _header;
}

long
ossimDemGrid::getWidth() const
{
   return _width;
}

long
ossimDemGrid::getHeight() const
{
   return _height;
}

ossim_float32 ossimDemGrid::getElevation(long x, long y) const
{
   if (_grid == 0)
      return _missDataVal;

   if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height))
      return _missDataVal;

   return _grid[(y * _width) + x];
}

ossim_float32 ossimDemGrid::getMissingDataValue() const
{
   return _missDataVal;
}

long ossimDemGrid::fillGeographic(std::istream& dem,bool incrementalRead)
{
   if (_firstTime) {
      _curProfile = 0;
      _width = _header.getProfileColumns();
   }

   while (_curProfile < _width)
   {
      _profiles.push_back(ossimDemProfile());
      dem >> _profiles.back();
      _curProfile++;
      if (incrementalRead)
         return _width - _curProfile + 1;
   }

   // Assume all profiles have as many elevations as the first.
   _height = _profiles[0].getNumberOfElevations();
   if(_grid) delete [] _grid; _grid =0;
   _grid = new ossim_float32[_width * _height];

   ossimDemPoint sw_corner = _profiles[0].getProfileLocation();
   _northwest_x = sw_corner.getX();
   _northwest_y = sw_corner.getY() 
                  + ((_profiles[0].getNumberOfElevations() - 1) * _header.getSpatialResY());


   unsigned int i,j;
   for (i = 0; (int)i < _width; i++)
   {
      ossimDemElevationVector const& elev = _profiles[i].getElevations();
      for (j = 0; j < elev.size(); j++)
      {
         setElevation(i, _height - j - 1, elev[j]);
      }
   }

   _profiles.erase(_profiles.begin(), _profiles.end());

   return 0;
}

long
ossimDemGrid::fillUTM(std::istream& dem, bool incrementalRead)
{
   // 7.5 UTM DEMs are small enough we can get away with doing this stupid...

   unsigned int i;
   unsigned int x,y;

   if (_firstTime)
   {
      _curProfile = 0;
      _width = _header.getProfileColumns();
   }


   while (_curProfile < _width)
   {
      _profiles.push_back(ossimDemProfile());
      dem >> _profiles.back();
      _curProfile++;
      if (incrementalRead)
         return _width - _curProfile + 1;
   }

   double dy = _header.getSpatialResY();

   // Determine min and max Y values.
   // Some DEMs can have profiles which do not have any
   // elevations, and erroneous (x,y) values. I suspect
   // these are probably illegal DEMs, but we'll try to
   // do the right thing anyway.
   ossimDemPoint curpoint;
   i = 0;
   while ((_profiles[i].getNumberOfElevations() == 0) &&
          (i < _profiles.size()))
      i++;
   if (i < _profiles.size())
      curpoint = _profiles[i].getProfileLocation();
   else
   {
      // XXX This isn't the best way to handle this...
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimDemGrid::fillUTM: Yikes! All profiles have zero elevations!\n";
      return(1);
   }
 
   double miny, maxy;
   miny = curpoint.getY();
   maxy = miny;
   double profymin, profymax;  // Min and max y values for current profile.
   for (i = 0; i < _profiles.size(); i++)
   {
      if (_profiles[i].getNumberOfElevations() > 0)
      {
         curpoint = _profiles[i].getProfileLocation();
         profymin = curpoint.getY();
         profymax = profymin + ((_profiles[i].getNumberOfElevations() - 1) * dy);

         if (profymin < miny)
            miny = profymin;
         if (profymax > maxy)
            maxy = profymax;
      }
   }

   // We now have minimum and maximum y values over all profiles in the DEM.
   // Allocate a rectangular array large enough to hold them.

   _height = static_cast<long>(((maxy - miny) / dy) + 1);
   if(_grid) delete [] _grid; _grid =0;
   _grid = new ossim_float32[_width * _height];

   // Fill grid with the "missing data" value.
   for (i = 0; (int)i < _width * _height; i++)
      _grid[i] = _missDataVal;

   ossimDemPoint sw_corner = _profiles[0].getProfileLocation();
   _northwest_x = sw_corner.getX();
   _northwest_y = maxy;


   // Now, insert the elevations in the profiles in the appropriate place in the grid.

   long startpos;
   for (x = 0; (int)x < _width; x++)
   {
      ossimDemElevationVector const& elev = _profiles[x].getElevations();
      curpoint = _profiles[x].getProfileLocation();
      startpos = static_cast<long>((curpoint.getY() - miny) / dy);
      for (y = 0; y < elev.size(); y++)
      {
         setElevation(x, _height - startpos - y - 1, elev[y]);
      }
   }

   _profiles.erase(_profiles.begin(), _profiles.end());

   return 0;
}

void
ossimDemGrid::getGroundCoords(long x, long y, double& ground_x,
                              double& ground_y)
{
   ground_x = _northwest_x + (x * _header.getSpatialResX());
   ground_y = _northwest_y - (y * _header.getSpatialResY());
}
