//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//*******************************************************************
//$Id: ossimPolyArea2d.h 22343 2013-07-26 18:09:01Z dlucas $
#ifndef ossimPolyArea2d_HEADER
#define ossimPolyArea2d_HEADER
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimPolygon.h>
#include <ossim/base/ossimReferenced.h>
#include <vector>

class OssimPolyArea2dPrivate;

namespace geos{
  namespace geom{
    class Geometry;
  }
}

class OSSIM_DLL ossimPolyArea2d : public ossimReferenced
{
public:
  friend class OssimPolyArea2dPrivate;
  friend OSSIM_DLL std::ostream& operator <<(std::ostream& out, const ossimPolyArea2d& data);
  
  ossimPolyArea2d();
  ossimPolyArea2d(const vector<ossimGpt>& polygon);

  ossimPolyArea2d(const ossimDpt& p1,
    const ossimDpt& p2,
    const ossimDpt& p3,
    const ossimDpt& p4);
  ossimPolyArea2d(const ossimPolyArea2d& rhs);

  ossimPolyArea2d(const ossimIrect& rect);
  ossimPolyArea2d(const ossimDrect& rect);
  ossimPolyArea2d(const ossimPolygon& polygon);
  ~ossimPolyArea2d();

  void clear()
  {
    clearPolygons();
  }
  const ossimPolyArea2d& operator =(const ossimPolyArea2d& rhs);
  const ossimPolyArea2d& operator =(const ossimPolygon& rhs);
  const ossimPolyArea2d& operator =(const ossimIrect& rect);
  const ossimPolyArea2d& operator =(const ossimDrect& rect);
  const ossimPolyArea2d& operator =(const vector<ossimGpt>& polygon);
  const ossimPolyArea2d& operator &=(const ossimPolyArea2d& rhs);
  ossimPolyArea2d operator &(const ossimPolyArea2d& rhs)const;
  ossimPolyArea2d operator +(const ossimPolyArea2d& rhs)const;
  const ossimPolyArea2d& operator +=(const ossimPolyArea2d& rhs);
  ossimPolyArea2d operator -(const ossimPolyArea2d& rhs)const;
  const ossimPolyArea2d& operator -=(const ossimPolyArea2d& rhs);

  void add(const ossimPolyArea2d& rhs);
  bool getVisiblePolygons(vector<ossimPolygon>& polyList)const;
  bool getPolygonHoles(vector<ossimPolygon>& polyList)const;
  bool isEmpty()const;
  bool isPointWithin(const ossimDpt& point)const;
  bool isPointWithin(double x, double y)const;
  void getBoundingRect(ossimDrect& rect);

  bool saveState(ossimKeywordlist& kwl,
    const char* prefix=0)const;
  bool loadState(const ossimKeywordlist& kwl,
    const char* prefix=0);

  protected:
    void clearPolygons();
    void recurseVisibleGeometries(ossimPolygon::Vector& polyList, const geos::geom::Geometry* geom) const;
    void recurseHoles(ossimPolygon::Vector& polyList, const geos::geom::Geometry* geom) const;

   OssimPolyArea2dPrivate* m_privateData;
 };

#endif