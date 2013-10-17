//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Hicks
//
// Description: Storage class for observations.
//----------------------------------------------------------------------------
#ifndef ossimObservationSet_HEADER
#define ossimObservationSet_HEADER 1

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimPointObservation.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/matrix/newmat.h>
#include <ossim/matrix/newmatio.h>
#include <iostream>
#include <vector>

class OSSIM_DLL ossimObservationSet : public ossimObject
{
public:
   ossimObservationSet();

   ~ossimObservationSet();
  
   inline ossim_uint32 numImages()const { return (ossim_uint32)theImageHandlers.size(); }
   inline ossim_uint32    numObs()const { return (ossim_uint32)theObs.size(); }
   inline ossim_uint32 numAdjPar()const { return (ossim_uint32)theNumAdjPar; }
   inline ossim_uint32   numMeas()const { return (ossim_uint32)theNumMeas; }

   // observation accessor
   inline ossimRefPtr<ossimPointObservation> observ(const int& i)const{ return theObs[i]; }

   // image accessor
   ossimImageGeometry* getImageGeom(const int index);
   void setImageGeom(const int index, ossimImageGeometry* geom);

   // image index accessor
   inline int imIndex(const int& i)const { return theImageIndex[i]; }

   // adjustable parameter count accessor
   inline int adjParCount(const int& i)const { return theNumAdjParams[i]; }

   // Image file access
   inline ossimFilename imageFile(const int index)const { return theImageFiles[index]; }

   /**
    * standard evaluation
    *   [1] measResiduals:  [x,y] residuals (numMeas X 2)
    *   [2] objPartials:    object point partial derivatives (numMeas*3 X 2)
    *   [3] parPartials:    image parameter partial derivatives (numParams X 2)
    */
   bool evaluate(NEWMAT::Matrix& measResiduals,
                 NEWMAT::Matrix& objPartials,
                 NEWMAT::Matrix& parPartials);

   /**
    * operations
    */
   bool addObservation(ossimRefPtr<ossimPointObservation> obs);

   /**
    * text output : header + tab separated tie points
    */
   std::ostream& print(std::ostream& os) const;

protected:
   int theNumAdjPar;
   int theNumMeas;
   int theNumPartials;

   // member observations
   std::vector< ossimRefPtr<ossimPointObservation> > theObs;

   // measurement -> image index
   std::vector<int> theImageIndex;

   // image files
   std::vector<ossimFilename> theImageFiles;

   // image adjustable parameter count
   std::vector<int> theNumAdjParams;

   std::vector< ossimRefPtr<ossimImageHandler> > theImageHandlers;

   // groups (TODO in future integration of correlated parameters)
   //   Note: Currently, each image is assumed to be independent, which can result
   //     in redundant parameters.  For example, images from a single flight line
   //     clearly share a common focal length and, most likely, a common position
   //     bias.  Implementation of this enhancement will require a mechanism for
   //     defining inter-image parametric relationships.
   //std::vector<????????*> theGroups;
};

#endif // #ifndef ossimObservationSet_HEADER
