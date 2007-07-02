//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
//----------------------------------------------------------------------------
// $Id: ossimFilterTable.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimFilterTable_HEADER
#define ossimFilterTable_HEADER

#include <cmath>
#include <ossim/base/ossimConstants.h>

class ossimFilter;

class ossimFilterTable
{
public:
   /** default constructor */
   ossimFilterTable();

   /** destructor */
   ~ossimFilterTable();

   /** Builds a table with filter being used in both x and y direction. */
   void buildTable(ossim_uint32  filterSteps,
                   const ossimFilter& filter);

   /** Builds a table. */
   void buildTable(ossim_uint32  filterSteps,
                   const ossimFilter& xFilter,
                   const ossimFilter& yFilter);

   /** @return theWidth*theHeight */
   ossim_uint32 getWidthByHeight() const;

   /** @return theXSupport */
   ossim_uint32 getXSupport()      const;

   /** @return theYSupport */
   ossim_uint32 getYSupport()      const;

   /** @return theWidth */
   ossim_uint32 getWidth()         const;

   /** @return theHeight */
   ossim_uint32 getHeight()        const;

   /**
    * Inlined below.
    *
    * @return const double* to the closest weight of x and y.
    */
   const double* getClosestWeights(const double& x, const double& y)const;

protected:

   /**
    * Creates the weight array "theWeights".  Will delete if previously
    * allocated.
    */
   void allocateWeights();

   double*      theWeights;
   ossim_uint32 theWidth;
   ossim_uint32 theHeight;
   ossim_uint32 theWidthHeight;
   ossim_uint32 theXSupport;
   ossim_uint32 theYSupport;
   ossim_uint32 theFilterSteps;
};

/**
 * Method inlined for speed as it is used in the inner pixel loop of
 * ossimFilterResampler::resampleBilinearTile method.
 */
inline const double* ossimFilterTable::getClosestWeights(const double& x,
                                                         const double& y)const
{
   // Extract the decimal part of x and y.
   double intPartDummy;
   double decimalPrecisionX = fabs(modf(x, &intPartDummy));
   double decimalPrecisionY = fabs(modf(y, &intPartDummy));
   
   ossim_int32 kernelLine =
      (ossim_int32)(theFilterSteps*decimalPrecisionY);
   ossim_int32 kernelSamp =
      (ossim_int32)(theFilterSteps*decimalPrecisionX);
  
  return &theWeights[(kernelLine*theFilterSteps +
                      kernelSamp)*theWidthHeight];
}

#endif /* End of "#ifndef ossimFilterTable_HEADER" */
