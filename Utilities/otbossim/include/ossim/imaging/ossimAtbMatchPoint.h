//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// AUTHOR: Oscar Kramer (okramer@imagelinks.com)
//
// DESCRIPTION: Contains declaration of class ossimAtbMatchPoint. This class
//    is used by the automated tonal balancing code for bookkeeping and
//    statistics associated with match-points. Match points are points shared in
//    common between two or more images being adjusted. 
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimAtbMatchPoint.h 15766 2009-10-20 12:37:09Z gpotts $

#ifndef ossimAtbMatchPoint_HEADER
#define ossimAtbMatchPoint_HEADER

#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimReferenced.h>
#include <vector>
using namespace std;

class ossimAtbPointSource;
class ossimGridRemapEngine;
class ossimGridRemapSource;

/*!****************************************************************************
 *
 * CLASS:  ossimAtbMatchPoint
 *
 *****************************************************************************/
class ossimAtbMatchPoint : public ossimReferenced
{
public:
   ossimAtbMatchPoint()
      : theGridRemapEngine(0) {}
   
   ossimAtbMatchPoint(const ossimDpt& view_point,
                      ossimGridRemapEngine* engine)
      : theViewPoint(view_point), theGridRemapEngine(engine) {}

   
   /*!
    * Returns the point in view coordinates associated with this object.
    */
   const ossimDpt& viewPoint() const { return theViewPoint; }

   /*!
    * Adds a new contribution to the sample set.
    */
   void addImage(ossimGridRemapSource* image_source);

   /*!
    * The target parameter value computed given all contributions. Returns true
    * if a valid quantity is returned.
    */
   bool assignRemapValues();

   /*!
    * Hook to set the size of the kernel used by all point sources in computing
    * their mean pixel value. The kernels will be resized to NxN.
    */
   void setKernelSize(int side_size);

   /*!
    * Sets the ATB remap engine reference owned by the controller. This engine
    * is the only object that understands how to compute remap parameters from
    * pixel data. This permits easily modifying the ATB algorithm without
    * the support classes such as this one.
    */
   void setGridRemapEngine(ossimGridRemapEngine* engine);

   /*!
    * Dumps the contents of the object to the stream in human readable format.
    */
//   void print(ostream& os) const;

//   friend ostream& operator << (ostream& os);

protected:
   virtual ~ossimAtbMatchPoint();
   
   ossimDpt                     theViewPoint;
   vector<ossimAtbPointSource*> thePointSourceList; 
   ossimGridRemapEngine*        theGridRemapEngine;
};

#endif
