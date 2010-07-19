//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// DESCRIPTION:
//  This class maintains the relationship between a local space rectangular
//  (LSR) coordinate system and the earth-centered, earth-fixed (ECEF) system.
//
// SOFTWARE HISTORY:
//
//   09Aug2001  Oscar Kramer (okramer@imagelinks.com)
//              Initial coding.
//
//*****************************************************************************
//  $Id: ossimLsrSpace.cpp 17593 2010-06-17 19:07:26Z dburken $

#include <ossim/base/ossimLsrSpace.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimColumnVector3d.h>
#include <ossim/base/ossimEcefVector.h>


//*****************************************************************************
//  CONSTRUCTOR: ossimLsrSpace(origin, x_dir, y_dir, int)
//
//  Constructs the space given origin, and X and Y ECEF directions. The int
//  argument is a place holder only and not used.
//  
//*****************************************************************************
ossimLsrSpace::ossimLsrSpace(const ossimEcefPoint&  origin,
                             const ossimEcefVector& x_dir_ecf_vec,
                             const ossimEcefVector& y_dir_ecf_vec,
                             int   /* z_not_provided_space_holder */)
   : theOrigin (origin)
{
   //***
   // Compute the remaining axis given first two:
   //***
   ossimColumnVector3d xdir (x_dir_ecf_vec.data().unit());
   ossimColumnVector3d ydir (y_dir_ecf_vec.data().unit());
   ossimColumnVector3d zdir (xdir.cross(ydir));

   //***
   // Fill the rotation matrix:
   //***
   theLsrToEcefRotMatrix = ossimMatrix3x3::create(xdir[0], ydir[0], zdir[0],
                                                  xdir[1], ydir[1], zdir[1],
                                                  xdir[2], ydir[2], zdir[2]);
}

//*****************************************************************************
//  CONSTRUCTOR: ossimLsrSpace
//  
//  Constructs the space given origin, and X and Z ECEF directions. The int
//  argument is a place holder only and not used.
//  
//*****************************************************************************
ossimLsrSpace::ossimLsrSpace(const ossimEcefPoint&  origin,
                             const ossimEcefVector& x_dir_ecf_vec,
                             int   /* y_not_provided_space_holder */,
                             const ossimEcefVector& z_dir_ecf_vec)
   : theOrigin (origin)
{
   //***
   // Compute the remaining axis given first two:
   //***
   ossimColumnVector3d xdir (x_dir_ecf_vec.data().unit());
   ossimColumnVector3d zdir (z_dir_ecf_vec.data().unit());
   ossimColumnVector3d ydir (zdir.cross(xdir));

   //***
   // Fill the rotation matrix:
   //***
   theLsrToEcefRotMatrix = ossimMatrix3x3::create(xdir[0], ydir[0], zdir[0],
                                                  xdir[1], ydir[1], zdir[1],
                                                  xdir[2], ydir[2], zdir[2]);
}

//*****************************************************************************
//  CONSTRUCTOR: ossimLsrSpace
//  
//  Constructs the space given origin, and Y and Z ECEF directions. The int
//  argument is a place holder only and not used.
//  
//*****************************************************************************
ossimLsrSpace::ossimLsrSpace(const ossimEcefPoint&  origin,
                             int   /* x_not_provided_space_holder */,
                             const ossimEcefVector& y_dir_ecf_vec,
                             const ossimEcefVector& z_dir_ecf_vec)
   : theOrigin (origin)
{
   //***
   // Compute the remaining axis given first two:
   //***
   ossimColumnVector3d ydir (y_dir_ecf_vec.data().unit());
   ossimColumnVector3d zdir (z_dir_ecf_vec.data().unit());
   ossimColumnVector3d xdir (ydir.cross(zdir));

   //***
   // Fill the rotation matrix:
   //***
   theLsrToEcefRotMatrix = ossimMatrix3x3::create(xdir[0], ydir[0], zdir[0],
                                                  xdir[1], ydir[1], zdir[1],
                                                  xdir[2], ydir[2], zdir[2]);
}

//*****************************************************************************
//  CONSTRUCTORS: ossimLsrSpace(ossimGpt, y_azimuth)
//
//  This constructor sets up a local coordinate system centered at the
//  specified groundpoint, with the Z-axis normal to the ellipsoid and the
//  Y-axis rotated clockwise from north by the y_azimuth. This angle defaults
//  to 0, producing an East-North-Up system.
//  
//*****************************************************************************
ossimLsrSpace::ossimLsrSpace(const ossimGpt& origin,
                             const double&   y_azimuth)
{
   //***
   // Convert ground point origin to ECEF coordinates:
   //***
   theOrigin = ossimEcefPoint(origin);
   
   //***
   // Establish the component vectors for ENU system::
   //***
   double sin_lat = ossim::sind(origin.lat);
   double cos_lat = ossim::cosd(origin.lat);
   double sin_lon = ossim::sind(origin.lon);
   double cos_lon = ossim::cosd(origin.lon);
   
   ossimColumnVector3d E (-sin_lon,
                          cos_lon,
                          0.0);
   ossimColumnVector3d N (-sin_lat*cos_lon,
                          -sin_lat*sin_lon,
                          cos_lat);
   ossimColumnVector3d U (E.cross(N));

   //
   // Fill rotation matrix with these components, rotated about the Z axis
   // by the azimuth indicated:
   //
   if (std::abs(y_azimuth) > FLT_EPSILON)
   {
      double cos_azim = ossim::cosd(y_azimuth);
      double sin_azim = ossim::sind(y_azimuth);
      ossimColumnVector3d X (cos_azim*E - sin_azim*N);
      ossimColumnVector3d Y (sin_azim*E + cos_azim*N);
      ossimColumnVector3d Z (X.cross(Y));
      
      theLsrToEcefRotMatrix
         = ossimMatrix3x3::create(X[0], Y[0], Z[0],
                                  X[1], Y[1], Z[1],
                                  X[2], Y[2], Z[2]);
   }
   else
   {
      //***
      // No azimuth rotation, so simplify:
      //***
      theLsrToEcefRotMatrix = ossimMatrix3x3::create(E[0], N[0], U[0],
                                                     E[1], N[1], U[1],
                                                     E[2], N[2], U[2]);
   }
}

//*****************************************************************************
//  OPERATOR: ==
//*****************************************************************************
bool ossimLsrSpace::operator == (const ossimLsrSpace& that) const
{
   if (theOrigin != that.origin())
      return false;
   
   if (theLsrToEcefRotMatrix != that.theLsrToEcefRotMatrix)
      return false;

   return true;
}
   
//*****************************************************************************
//  OPERATOR: =
//*****************************************************************************
const ossimLsrSpace& ossimLsrSpace::operator = (const ossimLsrSpace& space)
{
   theOrigin = space.theOrigin;
   theLsrToEcefRotMatrix = space.theLsrToEcefRotMatrix;
   return *this;
}

//*****************************************************************************
//  STATIC METHOD: lsrSpaceErrorMessage()
//  
//  Convenience method accessible to all owners of an ossimLsrSpace for
//  displaying an error message when LSR spaces do not match between
//  objects. All operations between LSR objects must be in a common space.
//  
//*****************************************************************************
ostream& ossimLsrSpace::lsrSpaceErrorMessage(ostream& os)
{
   os<<"ossimLsrSpace ERROR: An operation was attempted between two LSR \n"
     <<"  objects with differing LSR spaces. This is an illegal condition.\n"
     <<"  Please check the data and/or report the error to OSSIM development."
     << std::endl;

   return os;
}

//*****************************************************************************
//  METHOD: print()
//  
//  Dumps contents to stream for debug purposes. Defaults to cout.
//  
//*****************************************************************************
void ossimLsrSpace::print(ostream& stream) const
{
   stream << "(ossimLsrSpace)"
          << "\n   theOrigin = " << theOrigin
          << "\n   theLsrToEcefRotMatrix = \n" << theLsrToEcefRotMatrix << std::endl;
}
   
