//---
// License:  See top level LICENSE.txt file.
//
// $Id: ossimPolyArea2d.cpp 22344 2013-07-26 18:09:45Z dlucas $
//---

#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimPolyArea2d.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimRefPtr.h>

#include <geos/geom/Coordinate.h>
#include <geos/geom/CoordinateArraySequence.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/Point.h>
#include <geos/geom/PrecisionModel.h>
#include <geos/io/WKTWriter.h>
#include <geos/io/WKTReader.h>

#include <cstdlib>
#include <vector>

class ossimGeometryFactoryWrapper : public ossimReferenced{
public:
  ossimGeometryFactoryWrapper()
  :m_geomFactory(0)
  {
      geos::geom::PrecisionModel *pm = new geos::geom::PrecisionModel(geos::geom::PrecisionModel::FLOATING);
      m_geomFactory                  = new geos::geom::GeometryFactory(pm, -1); 
  }
  virtual ~ossimGeometryFactoryWrapper(){if(m_geomFactory) delete m_geomFactory;m_geomFactory=0;}
  

  geos::geom::GeometryFactory* m_geomFactory;
};

class OssimPolyArea2dPrivate{
public:
  typedef geos::geom::Geometry* GeometryPtr;
  typedef const geos::geom::Geometry* ConstGeometryPtr;

  OssimPolyArea2dPrivate(GeometryPtr geom=0);
  ~OssimPolyArea2dPrivate();

  void deleteGeometry() { if(m_geometry) { delete m_geometry; m_geometry = 0; }}
  void setGeometry(const ossimPolygon& polygon);
  void setGeometry(GeometryPtr geom){deleteGeometry();m_geometry=geom;}
  geos::geom::GeometryFactory* geomFactory(){{return m_globalFactory.valid()?m_globalFactory->m_geomFactory:0;}}
  GeometryPtr m_geometry;
  static ossimRefPtr<ossimGeometryFactoryWrapper> m_globalFactory; 
};
ossimRefPtr<ossimGeometryFactoryWrapper> OssimPolyArea2dPrivate::m_globalFactory;

OssimPolyArea2dPrivate::OssimPolyArea2dPrivate(GeometryPtr geom)
:m_geometry(geom)
{
  static OpenThreads::Mutex globalFactoryMutex;

  {
    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(globalFactoryMutex);
    if(!m_globalFactory.valid())
    {
      m_globalFactory = new ossimGeometryFactoryWrapper();
    }    
  }
}

OssimPolyArea2dPrivate::~OssimPolyArea2dPrivate()
{
  deleteGeometry();
}

void OssimPolyArea2dPrivate::setGeometry(const ossimPolygon& polygon)
{
  deleteGeometry();

  geos::geom::CoordinateArraySequence *cas = new geos::geom::CoordinateArraySequence();

  const std::vector<ossimDpt>& pts = polygon.getVertexList();

  int idx = 0;
  int n = pts.size();

  if(n > 0)
  {
    for (idx = 0; idx < n; idx++)
    {
      cas->add(geos::geom::Coordinate(pts[idx].x, pts[idx].y));
    }    
    cas->add(geos::geom::Coordinate(pts[0].x, pts[0].y));

    geos::geom::LinearRing *lr = geomFactory()->createLinearRing(cas);
    m_geometry                 = geomFactory()->createPolygon(lr, NULL);
  }

}

void ossimPolyArea2d::recurseVisibleGeometries(std::vector<ossimPolygon>& polyList, const geos::geom::Geometry* geom) const {
   int nGeoms = geom->getNumGeometries();

   if(nGeoms < 2 )
   {
      const geos::geom::Polygon* poly = dynamic_cast<const geos::geom::Polygon*> (geom);

      if (poly)
      {
         const geos::geom::LineString* lineString = dynamic_cast<const geos::geom::LineString*> (poly->getExteriorRing());
         if (lineString)
         {
            int currentPolyIdx = polyList.size();
            int nPoints = lineString->getNumPoints();
            int idx = 0;

            polyList.push_back(ossimPolygon());

            for (idx=0; idx<nPoints; idx++)
            {
               const geos::geom::Point* point = lineString->getPointN(idx);
               polyList[currentPolyIdx].addPoint(point->getX(), point->getY());
            }
          }
      }
   }
   else
   {
      int idx = 0;
      
      for (idx=0; idx < nGeoms; idx++)
      {
         recurseVisibleGeometries(polyList, geom->getGeometryN(idx));
      }
   }
}

void ossimPolyArea2d::recurseHoles(std::vector<ossimPolygon>& polyList, const geos::geom::Geometry* geom) const {
   int nGeoms = geom->getNumGeometries();

   if(nGeoms < 2 )
   {
      const geos::geom::Polygon* poly = dynamic_cast<const geos::geom::Polygon*> (geom);

      if (poly)
      {
        ossim_uint32 nInteriorRings = poly->getNumInteriorRing();
        ossim_uint32 idx = 0;

        for(idx = 0; idx < nInteriorRings; ++idx)
        {
          const geos::geom::LineString* lineString = poly->getInteriorRingN(idx);
           if (lineString)
           {
              int currentPolyIdx = polyList.size();
              int nPoints = lineString->getNumPoints();
              int idx = 0;

              polyList.push_back(ossimPolygon());

              for (idx=0; idx<nPoints; idx++)
              {
                 const geos::geom::Point* point = lineString->getPointN(idx);
                 polyList[currentPolyIdx].addPoint(point->getX(), point->getY());
              }
           }
        }
      }
   }
   else
   {
      int idx = 0;
      
      for (idx=0; idx < nGeoms; idx++)
      {
         recurseHoles(polyList, geom->getGeometryN(idx));
      }
   }
}

std::ostream& operator <<(std::ostream& out, const ossimPolyArea2d& rhs)
{
  if(rhs.m_privateData->m_geometry)
  {
    out << rhs.m_privateData->m_geometry->toString();
  }
  return out;
}

ossimPolyArea2d::ossimPolyArea2d()
:m_privateData(new OssimPolyArea2dPrivate)
{
}

ossimPolyArea2d::ossimPolyArea2d(const vector<ossimGpt>& polygon)
:m_privateData(new OssimPolyArea2dPrivate)
{
  (*this) = polygon;
}

ossimPolyArea2d::ossimPolyArea2d(const ossimIrect& rect)
:m_privateData(new OssimPolyArea2dPrivate)
{
  (*this) = rect;
}

ossimPolyArea2d::ossimPolyArea2d(const ossimDrect& rect)
:m_privateData(new OssimPolyArea2dPrivate)
{
  (*this) = rect;
}

ossimPolyArea2d::ossimPolyArea2d(const ossimPolygon& polygon)
:m_privateData(new OssimPolyArea2dPrivate)
{
  (*this) = polygon;
}

ossimPolyArea2d::ossimPolyArea2d(const ossimPolyArea2d& rhs)
:m_privateData(new OssimPolyArea2dPrivate) 
{
  *this = rhs;
}

ossimPolyArea2d::ossimPolyArea2d(const ossimDpt& p1,
                                 const ossimDpt& p2,
                                 const ossimDpt& p3,
                                 const ossimDpt& p4)
:m_privateData(new OssimPolyArea2dPrivate)
{
   ossimPolygon temp(p1,p2,p3,p4);
   *this = temp;
}

ossimPolyArea2d::~ossimPolyArea2d()
{
  if(m_privateData) {
    delete m_privateData;
    m_privateData = 0;
  }
}

const ossimPolyArea2d& ossimPolyArea2d::operator =(const ossimPolyArea2d& rhs)
{ 
   if(this == &rhs)
   {
      return *this;
   }

   if(rhs.m_privateData->m_geometry)
   {
      m_privateData->setGeometry(rhs.m_privateData->m_geometry->clone());
   }
   return *this;
}

const ossimPolyArea2d& ossimPolyArea2d::operator =(const ossimPolygon& polygon)
{
  m_privateData->setGeometry(polygon);

  return *this;
}

const ossimPolyArea2d& ossimPolyArea2d::operator =(const ossimIrect& rect)
{
   return (*this = ossimPolygon(rect));
}

const ossimPolyArea2d& ossimPolyArea2d::operator =(const ossimDrect& rect)
{
   return (*this = ossimPolygon(rect));
}

const ossimPolyArea2d& ossimPolyArea2d::operator =(const vector<ossimGpt>& polygon)
{
  std::vector<ossimDpt> pts;
  int idx = 0;
  int n = polygon.size();
  for(idx = 0; idx < n;++idx)
  {
    pts.push_back(polygon[idx]);
  }
  
  return (*this = ossimPolygon(pts));
}

ossimPolyArea2d ossimPolyArea2d::operator &(const ossimPolyArea2d& rhs)const
{
  if((this!=&rhs) && m_privateData->m_geometry && rhs.m_privateData->m_geometry)
  {
    ossimPolyArea2d result;
    result.m_privateData->setGeometry(m_privateData->m_geometry->intersection(rhs.m_privateData->m_geometry));

    return result;
  }
   return *this;
}

ossimPolyArea2d ossimPolyArea2d::operator +(const ossimPolyArea2d& rhs)const
{
  if((this!=&rhs) && m_privateData->m_geometry && rhs.m_privateData->m_geometry)
  {
    ossimPolyArea2d result;
    result.m_privateData->setGeometry(m_privateData->m_geometry->Union(rhs.m_privateData->m_geometry));
  
    return result;
  }
  return *this;
}
ossimPolyArea2d ossimPolyArea2d::operator -(const ossimPolyArea2d& rhs)const
{
  if((this!=&rhs) && m_privateData->m_geometry && rhs.m_privateData->m_geometry)
  {
    ossimPolyArea2d result;
    result.m_privateData->setGeometry(m_privateData->m_geometry->difference(rhs.m_privateData->m_geometry));
  
    return result;
  }
  return *this;
}

const ossimPolyArea2d& ossimPolyArea2d::operator &=(const ossimPolyArea2d& rhs)
{
  if((this!=&rhs) && m_privateData->m_geometry && rhs.m_privateData->m_geometry)
  {
    m_privateData->setGeometry(m_privateData->m_geometry->intersection(rhs.m_privateData->m_geometry));
  }
   return *this;
}

const ossimPolyArea2d& ossimPolyArea2d::operator +=(const ossimPolyArea2d& rhs)
{
  if((this!=&rhs) && m_privateData->m_geometry && rhs.m_privateData->m_geometry)
  {
    m_privateData->setGeometry(m_privateData->m_geometry->Union(rhs.m_privateData->m_geometry));
  }
  return *this;
}

const ossimPolyArea2d& ossimPolyArea2d::operator -=(const ossimPolyArea2d& rhs)
{
  if((this!=&rhs) && m_privateData->m_geometry && rhs.m_privateData->m_geometry)
  {
    m_privateData->setGeometry(m_privateData->m_geometry->difference(rhs.m_privateData->m_geometry));
  }

  return *this;
}

void ossimPolyArea2d::add(const ossimPolyArea2d& rhs)
{
  m_privateData->setGeometry(m_privateData->m_geometry->Union(rhs.m_privateData->m_geometry));
}

void ossimPolyArea2d::clearPolygons()
{
  m_privateData->deleteGeometry();
  #if 0
   clearEngine();
   #endif
}

bool ossimPolyArea2d::getVisiblePolygons(vector<ossimPolygon>& polyList)const
{
  bool foundPolys = false;
  if(m_privateData->m_geometry)
  {
    ossim_uint32 sizeBefore = polyList.size();
    recurseVisibleGeometries(polyList, m_privateData->m_geometry);
    foundPolys = sizeBefore == polyList.size();
  }

  return foundPolys;
}

bool ossimPolyArea2d::getPolygonHoles(vector<ossimPolygon>& polyList)const
{
  bool foundPolys = false;
  if(m_privateData->m_geometry)
  {
    ossim_uint32 sizeBefore = polyList.size();
    recurseHoles(polyList, m_privateData->m_geometry);
    foundPolys = sizeBefore == polyList.size();
  }

  return foundPolys;
}

bool ossimPolyArea2d::isEmpty()const
{
  bool result = true;
  if (m_privateData&&m_privateData->m_geometry)
  {
    result = m_privateData->m_geometry->isEmpty();
  }

   return result;
}

bool ossimPolyArea2d::isPointWithin(const ossimDpt& point)const
{
  return isPointWithin(point.x, point.y);
}

bool ossimPolyArea2d::isPointWithin(double x, double y)const
{
  bool result = false;

  if(!isEmpty())
  {
      geos::geom::Coordinate c(x,y);
      geos::geom::Geometry* geom = m_privateData->geomFactory()->createPoint(c);
  
      result = m_privateData->m_geometry->intersects(geom);

      delete geom;
  }

  return result;
}

void ossimPolyArea2d::getBoundingRect(ossimDrect& rect)
{
  rect.makeNan();

  if(!isEmpty())
  {
    const geos::geom::Envelope* envelope = m_privateData->m_geometry->getEnvelopeInternal();

    rect = ossimDrect(envelope->getMinX(), envelope->getMinY(), envelope->getMaxX(), envelope->getMaxY());
  }
}

bool ossimPolyArea2d::saveState(ossimKeywordlist& kwl,
                               const char* prefix)const
{
   kwl.add(prefix,
           ossimKeywordNames::TYPE_KW,
           "ossimPolyArea2d",
           true);

  if(!isEmpty())
  {
    geos::io::WKTWriter writer;

    kwl.add(prefix,
            "wkt",
             writer.write(m_privateData->m_geometry).c_str(),
            true);
  }
  else
  {

  }

  return true;
}

bool ossimPolyArea2d::loadState(const ossimKeywordlist& kwl,
                                const char* prefix)
{
  if(m_privateData)
  {
    ossimString wkt = kwl.find(prefix, "wkt");

    if(!wkt.empty())
    {
      geos::io::WKTReader reader(m_privateData->geomFactory());
      try{
        m_privateData->setGeometry(reader.read(wkt.c_str()));
      }
      catch(...)
      {

      }
    }
  }
  return true;
}