#ifndef ossimEastingNorthingCutter_HEADER
#define ossimEastingNorthingCutter_HEADER
#include <ossim/imaging/ossimRectangleCutFilter.h>
#include <ossim/base/ossimViewInterface.h>

class ossimProjection;
class ossimEastingNorthingCutter : public ossimRectangleCutFilter,
                                   public ossimViewInterface
{
public:
   ossimEastingNorthingCutter(ossimObject* owner,
                              ossimImageSource* inputSource=NULL);
   ossimEastingNorthingCutter(ossimImageSource* inputSource=NULL);
   
   void setEastingNorthingRectangle(const ossimDpt& ul,
                                    const ossimDpt& lr);
   ossimDpt getUlEastingNorthing()const;
   ossimDpt getLrEastingNorthing()const;
   
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
   virtual bool setView(ossimObject* baseObject);

   virtual ossimObject* getView();
   virtual const ossimObject* getView()const;

   virtual void initialize();
   
protected:
   virtual ~ossimEastingNorthingCutter();
   ossimDpt theUlEastingNorthing;
   ossimDpt theLrEastingNorthing;
   
   ossimRefPtr<ossimProjection> theViewProjection;


   void transformVertices();
TYPE_DATA
};

#endif
