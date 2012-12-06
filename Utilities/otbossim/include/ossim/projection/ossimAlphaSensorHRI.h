//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Dave Hicks
//
// Description:  Alpha HRI Sensor Model
//
//*******************************************************************
//  $Id$
#ifndef ossimAlphaSensorHRI_HEADER
#define ossimAlphaSensorHRI_HEADER 1
#include <ossim/projection/ossimAlphaSensor.h>

class ossimAlphaSensorSupportData;

class OSSIM_DLL ossimAlphaSensorHRI : public ossimAlphaSensor
{
public:
   ossimAlphaSensorHRI();

   ossimAlphaSensorHRI(const ossimAlphaSensorHRI& src);

   virtual ossimObject* dup()const;
   
   virtual void imagingRay(const ossimDpt& image_point,
                           ossimEcefRay&   image_ray) const;

   virtual void worldToLineSample(const ossimGpt& world_point,
                                  ossimDpt&       image_point) const;

   virtual void updateModel();
   
   virtual void initAdjustableParameters();
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=0)const;
   
   /**
    * @brief Initializes model from support data and calls update Model on 
    * success.
    * @param supData
    * @return true on success, false on error.
    */
   virtual bool initialize( const ossimAlphaSensorSupportData& supData );
   
protected:
   TYPE_DATA
};

#endif // #ifndef ossimAlphaSensorHRI_HEADER
