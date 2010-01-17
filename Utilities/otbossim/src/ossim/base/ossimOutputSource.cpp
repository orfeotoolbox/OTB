#include <ossim/base/ossimOutputSource.h>

RTTI_DEF1(ossimOutputSource, "ossimOutputSource", ossimSource);

bool ossimOutputSource::saveState(ossimKeywordlist& kwl,
                                  const char* prefix)const
{
   return ossimSource::saveState(kwl, prefix);
}

bool ossimOutputSource::loadState(const ossimKeywordlist& kwl,
                                  const char* prefix)
{
   return ossimSource::loadState(kwl, prefix);
}
