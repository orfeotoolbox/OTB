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

#include <otb/SceneCoord.h>
#include <ossim/base/ossimDpt3d.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimString.h>


namespace ossimplugins
{
static const char SCENE_COORD[] = "sceneCoord";
static const char NUMBER_OF_SCENE_CORNER_COORD[] = "numberOfSceneCornerCoord";
static const char SCENE_CENTER_COORD[] = "sceneCenterCoord";
static const char SCENE_CORNER_COORD[] = "sceneCornerCoord";

SceneCoord::SceneCoord()
   :
   _numberOfSceneCoord(0),
   _centerSceneCoord(),
   _tabCornersSceneCoord()
{
}
   
SceneCoord::~SceneCoord()
{
}
   
SceneCoord::SceneCoord(const SceneCoord& rhs)
   :
   _numberOfSceneCoord(rhs._numberOfSceneCoord),
   _centerSceneCoord(rhs._centerSceneCoord),
   _tabCornersSceneCoord(rhs._tabCornersSceneCoord)
{
}

SceneCoord& SceneCoord::operator=(const SceneCoord& rhs)
{
   if ( this != &rhs )
   {
      _numberOfSceneCoord = rhs._numberOfSceneCoord;
      _centerSceneCoord = rhs._centerSceneCoord;
      _tabCornersSceneCoord = rhs._tabCornersSceneCoord;
   }
   
   return *this;
}

bool SceneCoord::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   std::string pfx("");
   std::string pfx2("");
   if (prefix)
   {
      pfx = prefix;
   }
   pfx += SCENE_COORD;
   pfx2 = pfx;
   pfx2 += ".";
   
   kwl.add(pfx2.c_str(), NUMBER_OF_SCENE_CORNER_COORD, _numberOfSceneCoord);

   std::string s = pfx + "." + SCENE_CENTER_COORD;
   _centerSceneCoord.saveState(kwl, s.c_str());

   std::string s2 =pfx + "." + SCENE_CORNER_COORD;
   for (unsigned int i = 0; i < _tabCornersSceneCoord.size(); ++i)
   {
      std::string s3 = s2 + "[" + ossimString::toString(i).c_str() + "]";
      _tabCornersSceneCoord[i].saveState(kwl, s3.c_str());
   }
   return true;
}

bool SceneCoord::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
  static const char MODULE[] = "SceneCoord::loadState";

   bool result = true;

   ossimString s;
   const char* lookup = 0;
   
   std::string pfx("");
   if (prefix)
   {
      pfx = prefix;
   }
   pfx += SCENE_COORD;
   pfx += ".";
 

   lookup = kwl.find(pfx.c_str(), NUMBER_OF_SCENE_CORNER_COORD);

   if (lookup)
   {
     s = lookup;
     _numberOfSceneCoord = s.toUInt32();
   }
   else
   {
     ossimNotify(ossimNotifyLevel_WARN)
        << MODULE << " Keyword not found: " << NUMBER_OF_SCENE_CORNER_COORD << " in "<<pfx.c_str()<<" path.\n";
     result = false;
   }
   
   std::string s1 = pfx + SCENE_CENTER_COORD;
   
   result = _centerSceneCoord.loadState(kwl, s1.c_str());

   _tabCornersSceneCoord.clear();
   std::string s2 = pfx + SCENE_CORNER_COORD;
   for (unsigned int i = 0; i < _numberOfSceneCoord; ++i)
   {
      std::string s3 = s2 + "[" + ossimString::toString(i).c_str() + "]";
      InfoSceneCoord isc;
      result = isc.loadState(kwl, s3.c_str());
      _tabCornersSceneCoord.push_back(isc);
   }
   if( _numberOfSceneCoord != _tabCornersSceneCoord.size() )
   {
    ossimNotify(ossimNotifyLevel_WARN)
	<< MODULE << " Keyword " << NUMBER_OF_SCENE_CORNER_COORD << " is different with the number of _tabCornersScenceCoord nodes \n";
   }

  return result;
}


std::ostream& SceneCoord::print(std::ostream& out) const
{
   out << setprecision(15) << setiosflags(ios::fixed)
       << "\n SceneCoord class data members:\n";

   const char* prefix = 0;
   ossimKeywordlist kwl;
   ossimString pfx;
   pfx += SCENE_COORD;
   ossimString s = pfx + "." + NUMBER_OF_SCENE_CORNER_COORD;
   kwl.add(prefix, s.c_str(), _numberOfSceneCoord);
   
   ossimString s1 = pfx + "." + SCENE_CENTER_COORD;
   _centerSceneCoord.saveState(kwl, s1.c_str());

   ossimString s2 =pfx + "." + SCENE_CORNER_COORD;
   for (unsigned int i = 0; i < _tabCornersSceneCoord.size(); ++i)
   {
     ossimString s3 = s2 + "[" + ossimString::toString(i) + "]";
     _tabCornersSceneCoord[i].saveState(kwl, s3.c_str());
   }
   out << kwl;

   return out;
}

}
