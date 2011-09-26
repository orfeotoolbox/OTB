//**************************************************************************************************
//                          OSSIM -- Open Source Software Image Map
//
// LICENSE: See top level LICENSE.txt file.
//
// AUTHOR: Oscar Kramer, GeoEye Inc.
//
// DESCRIPTION: Contains Lagrange Interpolator class
// 
//**************************************************************************************************
//  $Id$
#include <iomanip>
#include <string>
using namespace std;

#include "ossim/base/ossimLagrangeInterpolator.h"

// Define Trace flags for use within this file:
#include "ossim/base/ossimTrace.h"
static ossimTrace traceDebug ("ossimLagrangeInterpolator:debug");

static const char HEADER_LABEL[] = "LAGRANGE_INTERP";


//**************************************************************************************************
//  CONSTRUCTOR: ossimLagrangeInterpolator()
//**************************************************************************************************
ossimLagrangeInterpolator::ossimLagrangeInterpolator(istream& stream)
{
   static const char MODULE[] = "ossimLagrangeInterpolator(istream&) CONSTRUCTOR";
   if (traceDebug())  CLOG << "entering..." << endl;

   stream >> *this;

   if (traceDebug())  CLOG << "returning..." << endl;
}


//**************************************************************************************************
//  CONSTRUCTOR: ossimLagrangeInterpolator()
//**************************************************************************************************
ossimLagrangeInterpolator::ossimLagrangeInterpolator(const std::vector<double>& t_array, 
   const std::vector<NEWMAT::ColumnVector>&  data_array)
:  theTeeArray (t_array),
   theDataArray(data_array),
   theNumElements(0)
{
   static const char MODULE[]="ossimLagrangeInterpolator(int, double, Vector) CONSTRUCTOR";
   if (traceDebug())  CLOG << "entering..." << endl;

   // Assign data members:
   ossim_uint32 numPoints = (ossim_uint32) theTeeArray.size();
   if (theDataArray.size() > 0)
      theNumElements = theDataArray[0].Nrows();

   ossim_uint32 i;
   double n;
   for (i=0; i<numPoints; i++)
   {
      // Compute the normalizer value at this data point:
      n = 1.0;
      for (ossim_uint32 j=0; j<numPoints; j++)
      {
         if (i != j) 
            n *= (theTeeArray[i] - theTeeArray[j]);
      }
      theNormalizer.push_back(n);
   }

   if (traceDebug())  CLOG << "returning..." << endl;
}


//**************************************************************************************************
//  DESTRUCTOR: ossimLagrangeInterpolator
//**************************************************************************************************
ossimLagrangeInterpolator::~ossimLagrangeInterpolator()
{
}

//**************************************************************************************************
// Adds one data sample to the collection.
//**************************************************************************************************
void ossimLagrangeInterpolator::addData(const double& t, const NEWMAT::ColumnVector& data)
{
   theTeeArray.push_back(t);
   theDataArray.push_back(data);
}

//**************************************************************************************************
// PUBLIC METHOD: ossimLagrangeInterpolator::interpolate(double t)
//**************************************************************************************************
bool ossimLagrangeInterpolator::interpolate(const double& t, NEWMAT::ColumnVector& result) const
{
   static const char MODULE[] = "ossimLagrangeInterpolator::interpolate()";
   if (traceDebug())  CLOG << "entering..." << endl;

   // Prepare to sum:
   double weight;

   // Perform interpolation:
   ossim_uint32 numPoints = (ossim_uint32) theTeeArray.size();
   for (ossim_uint32 i=0; i<numPoints; i++)
   {
      weight = 1.0/theNormalizer[i];
      for (ossim_uint32 j=0; j<numPoints; j++)
      {
         if (i != j) 
            weight *= (t - theTeeArray[j]);
      }

      result += theDataArray[i] * weight;
   }

   if (traceDebug())  CLOG << "leaving." << endl;
   return true;
}


//*****************************************************************************
//  Outputs contents to output stream. Compatible with input operator >> for
//  restoring state of interpolator from file:
//*****************************************************************************
ostream& operator << (ostream& stream, const ossimLagrangeInterpolator& interpolator)
{
   stream << "\n" << HEADER_LABEL
          << "  " << interpolator.theTeeArray.size()
          << "  " << interpolator.theNumElements << endl;

   for (ossim_uint32 i=0; i<interpolator.theTeeArray.size(); ++i)
   {
      stream << setprecision(20);
      stream << interpolator.theTeeArray[i]   << " "
             << interpolator.theNormalizer[i] << endl
             << "( "<<interpolator.theDataArray[i](0)
             << interpolator.theDataArray[i](1)
             << interpolator.theDataArray[2](1) << " )" << endl;
   }
   
   return stream;
}


//*****************************************************************************
//  Inputs object from input stream. Compatible with output operator << for
//  restoring state of interpolator from file. Assumes 3D data vector.
//*****************************************************************************
istream& operator >> (istream& stream, ossimLagrangeInterpolator& interpolator)
{
   // Start with ensuring we're at the right spot.  Let it go a max of ten
   // reads.  If we go beyond that there's a problem...
   string s;
   int count = 0;
   stream >> s;
   while (s != "LAGRANGE_INTERP") 
   {
      stream >> s;
      ++count;
      if (count == 10) break; // Avoid infinite loop...
   }

   if (count == 10)
   {
      cerr << "ossimLagrangeInterpolator operator>> ERROR:"
           << "\nStream seems to not contain LAGRANGE_INTERP header!"
           << "\nReturning..." << endl;
      return stream;
   }

   ossim_uint32 numPoints;
   stream >> numPoints >> interpolator.theNumElements;

   interpolator.theTeeArray.clear();
   interpolator.theDataArray.clear();
   interpolator.theNormalizer.clear();

   double tee, normalizer;
   NEWMAT::ColumnVector v(3);
   for (ossim_uint32 i=0; i<numPoints; i++)
   {
      stream >> tee >> v(0) >> v(1) >> v(2) >> normalizer;
      if (!stream.fail())
      {
         interpolator.theTeeArray.push_back(tee);
         interpolator.theDataArray.push_back(v);
         interpolator.theNormalizer.push_back(normalizer);
      }
   }

   return stream;
}
