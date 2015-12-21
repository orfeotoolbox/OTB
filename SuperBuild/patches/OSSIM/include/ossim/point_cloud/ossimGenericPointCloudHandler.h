//**************************************************************************************************
//
// OSSIM (http://trac.osgeo.org/ossim/)
//
// License:  LGPL -- See LICENSE.txt file in the top level directory for more details.
//
//**************************************************************************************************
// $Id: ossimGenericPointCloudHandler.h 23446 2015-07-21 13:23:00Z okramer $

#ifndef ossimGenericPointCloudHandler_HEADER
#define ossimGenericPointCloudHandler_HEADER 1

#include <ossim/point_cloud/ossimPointCloudHandler.h>
#include <ossim/point_cloud/ossimPointBlock.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimGrect.h>
#include <vector>

using namespace std;

class OSSIMDLLEXPORT ossimGenericPointCloudHandler : public ossimPointCloudHandler
{
public:
   ossimGenericPointCloudHandler(vector<ossimEcefPoint>& ecef_points)
   {
      // Fill the point storage in any order.
      // Loop to add your points (assume your points are passed in a vector ecef_points[])
      for (ossim_uint32 i=0; i<ecef_points.size(); ++i)
      {
         ossimPointRecord* point = new ossimPointRecord(ossimGpt(ecef_points[i]));
         m_pointBlock.addPoint(point);
      }
      ossimGrect bounds;
      m_pointBlock.getBounds(bounds);
      m_minRecord = new ossimPointRecord(bounds.ll());
      m_maxRecord = new ossimPointRecord(bounds.ur());
   }

   ossimGenericPointCloudHandler(vector<ossimGpt>& ground_points)
   {
      // Fill the point storage in any order.
      // Loop to add your points (assume your points are passed in a vector ecef_points[])
      for (ossim_uint32 i=0; i<ground_points.size(); ++i)
      {
         ossimPointRecord* point = new ossimPointRecord(ground_points[i]);
         m_pointBlock.addPoint(point);
      }
      ossimGrect bounds;
      m_pointBlock.getBounds(bounds);
      m_minRecord = new ossimPointRecord(bounds.ll());
      m_maxRecord = new ossimPointRecord(bounds.ur());
   }

   virtual ~ossimGenericPointCloudHandler() { m_pointBlock.clear(); }

   virtual ossim_uint32 getNumPoints() const { return m_pointBlock.size(); }

   virtual void getFileBlock(ossim_uint32 offset,
                             ossimPointBlock& block,
                             ossim_uint32 maxNumPoints=0xFFFFFFFF) const
   {
      block.clear();
      if (offset >= m_pointBlock.size())
         return;

      for (ossim_uint32 i=offset; i<m_pointBlock.size(); ++i)
         block.addPoint(new ossimPointRecord(*(m_pointBlock[i])));

      m_currentPID = block.size();
   }

   virtual ossim_uint32 getFieldCode() const { return 0; }

   virtual bool open(const ossimFilename& pointsFile) { return false; }
   virtual void close() {  }

protected:
   ossimGenericPointCloudHandler();
   ossimPointBlock m_pointBlock;
};

#endif /* #ifndef ossimPdalReader_HEADER */
