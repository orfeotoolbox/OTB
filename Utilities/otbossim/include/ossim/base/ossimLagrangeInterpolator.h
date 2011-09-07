//**************************************************************************************************
//                          OSSIM -- Open Source Software Image Map
//
// LICENSE: See top level LICENSE.txt file.
//
// AUTHOR: Oscar Kramer, GeoEye Inc.
//
// DESCRIPTION: Contains Lagrange Interpolator class
// 
//  $Id$
//**************************************************************************************************
#ifndef LagrangeInterp_HEADER
#define LagrangeInterp_HEADER

#include <iostream>
#include <vector>
#include <ossim/matrix/newmat.h>
#include <ossim/base/ossimReferenced.h>

//******************************************************************************
// CLASS:  ossimLagrangeInterpolator
//******************************************************************************
class ossimLagrangeInterpolator : public ossimReferenced
{
   friend std::ostream& operator<<(std::ostream&, const ossimLagrangeInterpolator&);
   friend std::istream& operator>>(std::istream&, ossimLagrangeInterpolator&);
   
public:
   ossimLagrangeInterpolator() : theNumElements(0) {}
   ossimLagrangeInterpolator(std::istream&);
   ossimLagrangeInterpolator(const std::vector<double>& t_array, 
                             const std::vector<NEWMAT::ColumnVector>&  data_array);
   ~ossimLagrangeInterpolator();
   
   void addData(const double& t, const NEWMAT::ColumnVector& data);

   bool interpolate(const double& t, NEWMAT::ColumnVector& result) const;
   
private:
   std::vector<double> theTeeArray;
   std::vector<NEWMAT::ColumnVector> theDataArray;
   std::vector<double> theNormalizer;
   ossim_uint32 theNumElements;
};

#endif
