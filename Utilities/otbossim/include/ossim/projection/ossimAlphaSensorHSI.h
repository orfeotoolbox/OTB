//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Dave Hicks
//
// Description:  Alpha HSI Sensor Model
//
//*******************************************************************
//  $Id$
#ifndef ossimAlphaSensorHSI_HEADER
#define ossimAlphaSensorHSI_HEADER 1
#include <ossim/projection/ossimAlphaSensor.h>

class ossimAlphaSensorSupportData;

class OSSIM_DLL ossimAlphaSensorHSI : public ossimAlphaSensor
{
public:
   ossimAlphaSensorHSI();

   ossimAlphaSensorHSI(const ossimAlphaSensorHSI& src);

   virtual ossimObject* dup()const;
   
   virtual void imagingRay(const ossimDpt& image_point,
                           ossimEcefRay&   image_ray) const;
   
   virtual void worldToLineSample(const ossimGpt& world_point,
                                  ossimDpt&       image_point) const;

   virtual void updateModel();
   
   virtual void initAdjustableParameters();
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=0)const;

   ossim_float64 getScanAngle(const ossim_float64& line)const;
   
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

#endif // #ifndef ossimAlphaSensorHSI_HEADER
