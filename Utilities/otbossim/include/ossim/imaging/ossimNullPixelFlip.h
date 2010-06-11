//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// 
//*******************************************************************
//  $Id$
#ifndef ossimNullPixelFlip_HEADER
#define ossimNullPixelFlip_HEADER
#include <ossim/imaging/ossimImageSourceFilter.h>

class OSSIM_DLL ossimNullPixelFlip : public ossimImageSourceFilter
{
public:
   enum ClipMode
   {
      ClipMode_NONE = 0,
      ClipMode_BOUNDING_RECT  = 1
   };
   enum ReplacementType
   {
      ReplacementType_MINPIX = 0,
      ReplacementType_MAXPIX = 1
   };
   
   ossimNullPixelFlip();
   virtual void initialize();
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tile_rect,
                                               ossim_uint32 resLevel=0);
   
protected:
   template <class T> void flipPixels(T dummy,
                                      ossimImageData* inpuTile,
                                      ossim_uint32 resLevel);
   
   template <class T> ossimImageData* clipTile(T dummy,
                                    ossimImageData* inpuTile,
                                    ossim_uint32 resLevel);
   
   ClipMode m_clipMode;
   ReplacementType m_replacementType;
TYPE_DATA   
};
#endif
