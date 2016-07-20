//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#include "otb/SceneCoord.h"
#include "ossimKeyWordListUtilities.h"
#include <ossim/base/ossimString.h>
#include <iomanip>


namespace ossimplugins
{
static const char SCENE_COORD[] = "sceneCoord";
static const char NUMBER_OF_SCENE_CORNER_COORD[] = "numberOfSceneCornerCoord";
static const char SCENE_CENTER_COORD[] = "sceneCenterCoord";
static const char SCENE_CORNER_COORD[] = "sceneCornerCoord";

SceneCoord::SceneCoord()
   : m_numberOfSceneCoord(0),
   m_centerSceneCoord(),
   m_tabCornersSceneCoord()
   {
   }

bool SceneCoord::saveState(ossimKeywordlist& kwl, std::string const& prefix) const
{
   const std::string pfx = prefix + SCENE_COORD + '.';

   add(kwl, pfx, NUMBER_OF_SCENE_CORNER_COORD, m_numberOfSceneCoord);

   const std::string s1 = pfx + SCENE_CENTER_COORD;
   m_centerSceneCoord.saveState(kwl, s1);

   const std::string s2 = pfx + SCENE_CORNER_COORD;
   for (unsigned int i = 0; i < m_tabCornersSceneCoord.size(); ++i)
   {
      const std::string s3 = s2 + '[' + ossimString::toString(i).c_str() + ']';
      m_tabCornersSceneCoord[i].saveState(kwl, s3);
   }
   return true;
}

bool SceneCoord::loadState(const ossimKeywordlist& kwl, std::string const& prefix)
{
   static const char MODULE[] = "SceneCoord::loadState";

   const std::string pfx = prefix + SCENE_COORD +'.';
   const std::string s1 = pfx + SCENE_CENTER_COORD;
   const std::string s2 = pfx + SCENE_CORNER_COORD;

#if 1
   get(kwl, pfx, NUMBER_OF_SCENE_CORNER_COORD, m_numberOfSceneCoord);
#else
   bool result = true;
   ossimString s;
   const char* lookup = 0;
   lookup = kwl.find(pfx.c_str(), NUMBER_OF_SCENE_CORNER_COORD);

   if (lookup)
   {
      s = lookup;
      m_numberOfSceneCoord = s.toUInt32();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << NUMBER_OF_SCENE_CORNER_COORD << " in "<<pfx.c_str()<<" path.\n";
      result = false;
   }
#endif

   m_centerSceneCoord.loadState(kwl, s1);

   m_tabCornersSceneCoord.clear();
   for (unsigned int i = 0; i < m_numberOfSceneCoord; ++i)
   {
      std::string s3 = s2 + '[' + ossimString::toString(i).c_str() + ']';
      InfoSceneCoord isc;
      isc.loadState(kwl, s3);
      m_tabCornersSceneCoord.push_back(isc);
   }
   return true;
}

std::ostream& SceneCoord::print(std::ostream& out) const
{
   ossimKeywordlist kwl;
   if (this->saveState(kwl))
   {
      out << setprecision(15) << setiosflags(ios::fixed)
         << "\n SceneCoord class data members:\n";
      out << kwl;
   }
#if 0
   else
   {
      out.set_state(std::ios_base::badbit);
   }
#endif
   return out;
}

}
