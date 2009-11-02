//-----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Class declaration of ossimImageProjectionModel.
//
//-----------------------------------------------------------------------------
// $Id$

#ifndef ossimImageProjectionModel_HEADER
#define ossimImageProjectionModel_HEADER 1

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimRtti.h>
#include <ossim/imaging/ossimImageModel.h>

class ossimProjection;

/**
 * @brief Class derived from ossimImageModel, this adds an image projection
 * for lineSampleToWorld and worldToLineSample.
 *
 * Note that image points fed to projection methods should be full
 * resolution with any sub image offset applied.
 */
class OSSIM_DLL ossimImageProjectionModel : public ossimImageModel
{
public:
   
   /** @brief default constructor */
   ossimImageProjectionModel();


   /**
    * @brief Method to initialize class from an image handler.
    *
    * @param ih Image handler.
    */
   virtual void initialize(const ossimImageHandler& ih);

   /**
    * @brief Method to get projection.
    *
    * @return Constant pointer to projection or 0 if not initialized.
    */
   const ossimProjection* getProjection() const;

protected:
   /** @brief virtual destructor */
   virtual ~ossimImageProjectionModel();

   ossimProjection* theProjection;

TYPE_DATA
};

#endif /* #ifndef ossimImageProjectionModel_HEADER */
