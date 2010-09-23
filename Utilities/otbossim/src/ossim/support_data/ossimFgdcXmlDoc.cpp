//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Description: Utility class to encapsulate parsing projection info in the xml file.
// 
//----------------------------------------------------------------------------
// $Id: ossimFgdcXmlDoc.cpp 1204 2010-08-02 18:48:46Z david.burken $

#include <ossim/support_data/ossimFgdcXmlDoc.h>

#include <ossim/base/ossimXmlNode.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimEpsgProjectionFactory.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>

static double METER_PER_FOOT = 0.304801;

ossimFgdcXmlDoc::ossimFgdcXmlDoc(const ossimFilename& xmlFileName)
   : m_xmlFilename(xmlFileName),
     m_xmlDocument(0),
     m_projection(0)
{
}

ossimFgdcXmlDoc::~ossimFgdcXmlDoc()
{
}

ossimProjection* ossimFgdcXmlDoc::getProjection()
{
   if ( m_projection.valid() )
   {
      return m_projection.get();
   }
   
   ossimDrect rect = getBoundingBox();
   ossimString projName (getProjCsn());
   
   if (!projName.empty())
   {
      m_projection = ossimEpsgProjectionFactory::instance()->createProjection(projName);
      
      if ( m_projection.valid() )
      {
         ossimDpt gsd(fabs(getXRes()), fabs(getYRes()));
         ossimMapProjection* mapProj = dynamic_cast<ossimMapProjection*>(m_projection.get());
         if (mapProj)
         {
            if (mapProj->isGeographic())
            {
               ossimGpt tie(rect.ul().lat, rect.ul().lon);
               mapProj->setUlTiePoints(tie);
               mapProj->setDecimalDegreesPerPixel(gsd);
            }
            else
            {
               ossimDpt tie(rect.ul().x, rect.ul().y);
               if (getUnits().upcase() == "FEET")
               {
                  gsd = gsd * METER_PER_FOOT;
                  tie = tie * METER_PER_FOOT;
               }
               mapProj->setUlTiePoints(tie);
               mapProj->setMetersPerPixel(gsd);
            }
         }
      }
   }
   return m_projection.get();
}

bool ossimFgdcXmlDoc::isOpen()
{
   if ( m_xmlDocument.valid() )
   {
      return true;
   }
   else
   {
      if (!m_xmlFilename.empty())
      {
         m_xmlDocument = new ossimXmlDocument(m_xmlFilename);
         
         if ( m_xmlDocument.valid() )
         {
            return true;
         }
      }
   }
   return false;
}

ossimString ossimFgdcXmlDoc::getGeoCsn()
{
   if (isOpen())
   {
      ossimString xpath = "/metadata/spref/horizsys/cordsysn/geogcsn";
      vector<ossimRefPtr<ossimXmlNode> > xml_nodes;
      m_xmlDocument->findNodes(xpath, xml_nodes);
      if (xml_nodes.size() > 0)
      {
         ossimString geoProjStr = xml_nodes[0]->getText();
         return geoProjStr;
      }
   }
   return "";
}

ossimString ossimFgdcXmlDoc::getProjCsn()
{
   if (isOpen())
   {
      ossimString xpath = "/metadata/spref/horizsys/cordsysn/projcsn";
      vector<ossimRefPtr<ossimXmlNode> > xml_nodes;
      m_xmlDocument->findNodes(xpath, xml_nodes);
      if (xml_nodes.size() > 0)
      {
         ossimString projStr = xml_nodes[0]->getText();
         return projStr;
      }
   }
   return "";
}

ossimString ossimFgdcXmlDoc::getUnits()
{
   if (isOpen())
   {
      ossimString xpath = "/metadata/spref/vertdef/altsys/altunits";
      vector<ossimRefPtr<ossimXmlNode> > xml_nodes;
      m_xmlDocument->findNodes(xpath, xml_nodes);
      if (xml_nodes.size() > 0)
      {
         ossimString units = xml_nodes[0]->getText();
         return units;
      }
   }
   return "";
}

double ossimFgdcXmlDoc::getXRes()
{
   if (isOpen())
   {
      ossimString xpath = "/metadata/spdoinfo/rastinfo/rastxsz";
      vector<ossimRefPtr<ossimXmlNode> > xml_nodes;
      m_xmlDocument->findNodes(xpath, xml_nodes);
      if (xml_nodes.size() > 0)
      {
         double rastXRes = xml_nodes[0]->getText().toDouble();
         return rastXRes;
      }
   }
   return 0;
}

double ossimFgdcXmlDoc::getYRes()
{
   if (isOpen())
   {
      ossimString xpath = "/metadata/spdoinfo/rastinfo/rastysz";
      vector<ossimRefPtr<ossimXmlNode> > xml_nodes;
      m_xmlDocument->findNodes(xpath, xml_nodes);
      if (xml_nodes.size() > 0)
      {
         double rastYRes = xml_nodes[0]->getText().toDouble();
         return rastYRes;
      }
   }
   return 0;
}

ossim_uint32 ossimFgdcXmlDoc::getNumberOfSamples()
{
   if (isOpen())
   {
      ossimString xpath = "/metadata/spatRepInfo/GridSpatRep/axDimProps/Dimen/dimSize";
      vector<ossimRefPtr<ossimXmlNode> > xml_nodes;
      m_xmlDocument->findNodes(xpath, xml_nodes);
      if (xml_nodes.size() > 0)
      {
         ossim_uint32 numOfSamples = xml_nodes[0]->getText().toInt();
         return numOfSamples;
      }
   }
   return 0;
}

ossim_uint32 ossimFgdcXmlDoc::getNumberOfLines()
{
   if (isOpen())
   {
      ossimString xpath = "/metadata/spatRepInfo/GridSpatRep/axDimProps/Dimen/dimSize";
      vector<ossimRefPtr<ossimXmlNode> > xml_nodes;
      m_xmlDocument->findNodes(xpath, xml_nodes);
      if (xml_nodes.size() > 1)
      {
         ossim_uint32 numOfLines = xml_nodes[1]->getText().toInt();
         return numOfLines;
      }
   }
   return 0;
}

ossimDrect ossimFgdcXmlDoc::getBoundingBox()
{
   if (isOpen())
   {
      double ll_lat = 0.0;
      double ll_lon = 0.0;
      double lr_lat = 0.0;
      double lr_lon = 0.0;
      double ul_lat = 0.0;
      double ul_lon = 0.0;
      double ur_lat = 0.0;
      double ur_lon = 0.0;
      
      ossimString xpath = "/metadata/idinfo/spdom/lboundng/leftbc";
      vector<ossimRefPtr<ossimXmlNode> > xml_nodes;
      m_xmlDocument->findNodes(xpath, xml_nodes);
      if (xml_nodes.size() > 0)
      {
         ul_lon = xml_nodes[0]->getText().toDouble();
         ll_lon = ul_lon;
      }
      
      xpath = "/metadata/idinfo/spdom/lboundng/rightbc";
      xml_nodes.clear();
      m_xmlDocument->findNodes(xpath, xml_nodes);
      if (xml_nodes.size() > 0)
      {
         ur_lon = xml_nodes[0]->getText().toDouble();
         lr_lon = ur_lon;
      }
      
      xpath = "/metadata/idinfo/spdom/lboundng/bottombc";
      xml_nodes.clear();
      m_xmlDocument->findNodes(xpath, xml_nodes);
      if (xml_nodes.size() > 0)
      {
         ll_lat = xml_nodes[0]->getText().toDouble();
         lr_lat = ll_lat;
      }
      
      xpath = "/metadata/idinfo/spdom/lboundng/topbc";
      xml_nodes.clear();
      m_xmlDocument->findNodes(xpath, xml_nodes);
      if (xml_nodes.size() > 0)
      {
         ul_lat = xml_nodes[0]->getText().toDouble();
         ur_lat = ul_lat;
      }
      
      ossimDrect rect(ossimDpt(ul_lon, ul_lat),
                      ossimDpt(ur_lon, ur_lat),
                      ossimDpt(lr_lon, lr_lat),
                      ossimDpt(ll_lon, ll_lat), OSSIM_RIGHT_HANDED); 
      
      return rect;
   }
   
   return ossimDrect();
}
