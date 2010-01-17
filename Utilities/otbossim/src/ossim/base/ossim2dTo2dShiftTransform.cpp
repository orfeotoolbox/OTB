#include <ossim/base/ossim2dTo2dShiftTransform.h>
#include <ossim/base/ossimKeywordlist.h>
RTTI_DEF1(ossim2dTo2dShiftTransform, "ossim2dTo2dShiftTransform", ossim2dTo2dTransform);
bool ossim2dTo2dShiftTransform::saveState(ossimKeywordlist& kwl,
                                          const char* prefix)const
{
   bool result = ossim2dTo2dTransform::saveState(kwl, prefix);
   kwl.add(prefix, "shift", m_shift.toString(), true);
   
   return result;
}

bool ossim2dTo2dShiftTransform::loadState(const ossimKeywordlist& kwl,
                                          const char* prefix)
{
   bool result = ossim2dTo2dTransform::loadState(kwl, prefix);
   
   ossimString shift = kwl.find(prefix, "shift");
   if(!shift.empty())
   {
      m_shift.toPoint(shift);
   }
   
   return result;
}
