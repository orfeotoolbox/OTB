//*****************************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Description: Class implementation of ossimImageGeometryFactory.
// See .h file for class documentation.
//
//*****************************************************************************
// $Id$
#include <ossim/imaging/ossimImageGeometryFactory.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossim2dTo2dShiftTransform.h>
#include <ossim/imaging/ossimNitfTileSource.h>
#include <ossim/imaging/ossimTiffTileSource.h>
#include <ossim/support_data/ossimNitfRegisteredTag.h>
#include <ossim/support_data/ossimNitfIchipbTag.h>
#include <ossim/support_data/ossimNitfStdidcTag.h>
#include <ossim/projection/ossimSensorModel.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/base/ossim2dTo2dTransformRegistry.h>

ossimImageGeometryFactory* ossimImageGeometryFactory::m_instance = 0;

ossimImageGeometryFactory::ossimImageGeometryFactory()
{
   m_instance = this;
}

ossimImageGeometryFactory* ossimImageGeometryFactory::instance()
{
   if(!m_instance)
   {
      m_instance = new ossimImageGeometryFactory();
   }
   
   return m_instance;
}

ossimImageGeometry* ossimImageGeometryFactory::createGeometry(
   const ossimString& typeName)const
{
   if(typeName == "ossimImageGeometry")
   {
      return new ossimImageGeometry();
   }
   
   return 0;
}

ossimImageGeometry* ossimImageGeometryFactory::createGeometry(
   const ossimKeywordlist& kwl, const char* prefix)const
{
   ossimRefPtr<ossimImageGeometry> result = 0;
   ossimString type = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
   if(!type.empty())
   {
      result = createGeometry(type);
      if(result.valid()&&!result->loadState(kwl, prefix))
      {
         result = 0;
      }
   }
   
   return result.release();
}

ossimImageGeometry* ossimImageGeometryFactory::createGeometry(
   const ossimFilename& /* filename */, ossim_uint32 /* entryIdx */)const
{
   // currently don't support this option just yet by this factory
   return 0;
}

bool ossimImageGeometryFactory::extendGeometry(ossimImageHandler* handler)const
{
   bool result = true;
   if (handler)
   {
      bool add2D = true;
      ossimRefPtr<ossimImageGeometry> geom = handler->getImageGeometry();
      if(geom.valid())
      {
         if(!geom->getProjection())
         {
            geom->setProjection(createProjection(handler));
            result = geom->hasProjection();
         }
         if(geom->getProjection())
         {
            if( !(dynamic_cast<ossimSensorModel*>(geom->getProjection())))
            {
               add2D = false;
            }
         }
         if(!geom->getTransform()&&add2D)
         {
            geom->setTransform(createTransform(handler));
            result |= geom->hasTransform();
        }
      }
   }
   return result;
}

void ossimImageGeometryFactory::getTypeNameList(
   std::vector<ossimString>& typeList)const
{
   typeList.push_back("ossimImageGeometry");
}

ossim2dTo2dTransform* ossimImageGeometryFactory::createTransform(
   ossimImageHandler* handler)const
{
   // Currently nothing to do...
   
   ossimRefPtr<ossim2dTo2dTransform> result = 0;
   
   ossimNitfTileSource* nitf = dynamic_cast<ossimNitfTileSource*> (handler);
   
   if(nitf)
   {
      result = createTransformFromNitf(nitf);
   }
   
   return result.release();
}

ossimProjection* ossimImageGeometryFactory::createProjection(
   ossimImageHandler* handler) const
{
   ossimRefPtr<ossimProjection> result =
      ossimProjectionFactoryRegistry::instance()->createProjection(handler);

   return result.release();
}

ossim2dTo2dTransform* ossimImageGeometryFactory::createTransformFromNitf(ossimNitfTileSource* nitf)const
{
   ossim2dTo2dTransform* result = 0;
   
   ossimRefPtr<ossimNitfImageHeader> hdr = nitf->getCurrentImageHeader();
   if (!hdr)
   {
      return result;
   }
   
   //---
   // Test for the ichipb tag and set the sub image if needed.
   // 
   // NOTE # 1:
   // 
   // There are nitf writers that set the ichipb offsets and only have
   // IGEOLO field present.  For these it has been determined
   // (but still in question) that we should not apply the sub image offset.
   //
   // See trac # 1578
   // http://trac.osgeo.org/ossim/ticket/1578
   //
   // NOTE # 2:
   //
   // Let the ICHIPB have precedence over the STDIDC tag as we could have a
   // chip of a segment.
   //---
   ossimRefPtr<ossimNitfRegisteredTag> tag =
   hdr->getTagData(ossimString("ICHIPB"));
   if (tag.valid())
   {
      ossimNitfIchipbTag* ichipb = PTR_CAST(ossimNitfIchipbTag, tag.get());
      if (ichipb)
      {
//         const ossimRefPtr<ossimNitfRegisteredTag> blocka =
//         hdr->getTagData(ossimString("BLOCKA"));
//         const ossimRefPtr<ossimNitfRegisteredTag> rpc00a =
//         hdr->getTagData(ossimString("RPC00A"));              
//         const ossimRefPtr<ossimNitfRegisteredTag> rpc00b =
//         hdr->getTagData(ossimString("RPC00B"));
         
         //---
         // If any of these tags are present we will use the sub image from
         // the ichipb tag.
         //---
//         if ( blocka.get() || rpc00a.get() || rpc00b.get() )
         
         // ************************* THERE ARE PROBLEMS NOT SETTING THIS AT SITE.  GO AHEAD AND ALWAYS INIT THE SHIFT
         {
            result = ichipb->newTransform();
         }
      }
   }
   
   if ( !result)
   {
      //---
      // Look for the STDIDC tag for a sub image (segment) offset.
      //
      // See: STDI-002 Table 7.3 for documentation.
      //---
      tag = hdr->getTagData(ossimString("STDIDC"));
      if (tag.valid() && (hdr->getIMode() == "B") )
      {
         ossimDpt shift;
         ossimNitfStdidcTag* stdidc = PTR_CAST(ossimNitfStdidcTag, tag.get());
         if (stdidc)
         {
            ossim_int32 startCol = stdidc->getStartColumn().toInt32();
            ossim_int32 startRow = stdidc->getStartRow().toInt32();
            if ( (startCol > 0) && (startRow > 0) )
            {
               
               // field are one based; hence, the - 1.
               shift.x = (startCol-1) * hdr->getNumberOfPixelsPerBlockHoriz();
               shift.y = (startRow-1) * hdr->getNumberOfPixelsPerBlockVert();
            }
            if(shift.x > 0 ||
               shift.y > 0)
            {
               result = new ossim2dTo2dShiftTransform(shift);
            }
         }
      }
   }
   
   return result;
}

