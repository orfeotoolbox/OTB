#include <ossim/support_data/ossimNitfImageHeaderV2_X.h>

#include <cmath> /* for fmod */
#include <iomanip>
#include <sstream>

#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimDms.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/support_data/ossimNitfImageHeader.h>
#include <ossim/support_data/ossimNitfCommon.h>

static const ossimTrace traceDebug(
   ossimString("ossimNitfImageHeaderV2_X:debug"));

const ossimString ossimNitfImageHeaderV2_X::IM_KW       = "IM";
const ossimString ossimNitfImageHeaderV2_X::IID1_KW     = "IID1";
const ossimString ossimNitfImageHeaderV2_X::IDATIM_KW   = "IDATIM";
const ossimString ossimNitfImageHeaderV2_X::TGTID_KW    = "TGTID";
const ossimString ossimNitfImageHeaderV2_X::IID2_KW     = "IID2";
const ossimString ossimNitfImageHeaderV2_X::ITITLE_KW   = "ITITLE";
const ossimString ossimNitfImageHeaderV2_X::ISCLAS_KW   = "ISCLAS";
const ossimString ossimNitfImageHeaderV2_X::ENCRYP_KW   = "ENCRYP";
const ossimString ossimNitfImageHeaderV2_X::ISORCE_KW   = "ISORCE";
const ossimString ossimNitfImageHeaderV2_X::NROWS_KW    = "NROWS";
const ossimString ossimNitfImageHeaderV2_X::NCOLS_KW    = "NCOLS";
const ossimString ossimNitfImageHeaderV2_X::PVTYPE_KW   = "PVTYPE";
const ossimString ossimNitfImageHeaderV2_X::IREP_KW     = "IREP";
const ossimString ossimNitfImageHeaderV2_X::ICAT_KW     = "ICAT";
const ossimString ossimNitfImageHeaderV2_X::ABPP_KW     = "ABPP";
const ossimString ossimNitfImageHeaderV2_X::PJUST_KW    = "PJUST";
const ossimString ossimNitfImageHeaderV2_X::ICORDS_KW   = "ICORDS";
const ossimString ossimNitfImageHeaderV2_X::IGEOLO_KW   = "IGEOLO";
const ossimString ossimNitfImageHeaderV2_X::NICOM_KW    = "NICOM";
const ossimString ossimNitfImageHeaderV2_X::ICOM_KW     =  "ICOM";
const ossimString ossimNitfImageHeaderV2_X::IC_KW       = "IC";
const ossimString ossimNitfImageHeaderV2_X::COMRAT_KW   = "COMRAT";
const ossimString ossimNitfImageHeaderV2_X::NBANDS_KW   = "NBANDS";
const ossimString ossimNitfImageHeaderV2_X::ISYNC_KW    = "ISYNC";
const ossimString ossimNitfImageHeaderV2_X::IMODE_KW    = "IMODE";
const ossimString ossimNitfImageHeaderV2_X::NBPR_KW     = "NBPR";
const ossimString ossimNitfImageHeaderV2_X::NBPC_KW     = "NBPC";
const ossimString ossimNitfImageHeaderV2_X::NPPBH_KW    = "NPPBH";
const ossimString ossimNitfImageHeaderV2_X::NPPBV_KW    = "NPPBV";
const ossimString ossimNitfImageHeaderV2_X::NBPP_KW     = "NBPP";
const ossimString ossimNitfImageHeaderV2_X::IDLVL_KW    = "IDLVL";
const ossimString ossimNitfImageHeaderV2_X::IALVL_KW    = "IALVL";
const ossimString ossimNitfImageHeaderV2_X::ILOC_KW     = "ILOC";
const ossimString ossimNitfImageHeaderV2_X::IMAG_KW     = "IMAG";
const ossimString ossimNitfImageHeaderV2_X::UDIDL_KW    = "UDIDL";
const ossimString ossimNitfImageHeaderV2_X::UDOFL_KW    = "UDOFL";
const ossimString ossimNitfImageHeaderV2_X::IXSHDL_KW   = "IXSHDL";
const ossimString ossimNitfImageHeaderV2_X::IXSOFL_KW   = "IXSOFL";
const ossimString ossimNitfImageHeaderV2_X::IMDATOFF_KW = "IMDATOFF";
const ossimString ossimNitfImageHeaderV2_X::BMRLNTH_KW  = "BMRLNTH";



RTTI_DEF1(ossimNitfImageHeaderV2_X,
          "ossimNitfImageHeaderV2_X",
          ossimNitfImageHeader);

ossimNitfImageHeaderV2_X::ossimNitfImageHeaderV2_X()
:theImageComments(0)
{
}

ossimIrect ossimNitfImageHeaderV2_X::getImageRect()const
{
   ossimIpt ul(0, 0);
   ossimIpt lr(getNumberOfCols()-1, getNumberOfRows()-1);
   return ossimIrect(ul, lr);
   
#if 0 
   //---
   // Changed to not include offset 20100619 (drb).
   // Use void getImageLocation(ossimIpt& loc)const to get offset.
   //---
   ossimDpt ul(ossimString((char*)(&theImageLocation[5])).toDouble(),
               ossimString((char*)theImageLocation,
                           (char*)(&theImageLocation[5])).toDouble());
   
    double rows = ossimString(theSignificantRows).toDouble();
    double cols = ossimString(theSignificantCols).toDouble();

    ossimDpt lr(ul.x + cols-1,
                ul.y + rows-1);
     return ossimDrect(ul, lr);
#endif
}

ossimIrect ossimNitfImageHeaderV2_X::getBlockImageRect()const
{
   ossimIpt ul(0, 0);
   ossimIpt lr( (getNumberOfPixelsPerBlockHoriz()*getNumberOfBlocksPerRow())-1,
                (getNumberOfPixelsPerBlockVert() *getNumberOfBlocksPerCol())-1);
               
   return ossimIrect(ul, lr);
   
#if 0
   //---
   // Changed to not include offset 20100619 (drb).
   // Use void getImageLocation(ossimIpt& loc)const to get offset.
   //---   
   ossimDpt ul(ossimString((char*)(&theImageLocation[5])).toDouble(),
               ossimString((char*)theImageLocation,
                           (char*)(&theImageLocation[5])).toDouble());
   
   double rows = getNumberOfPixelsPerBlockVert()*getNumberOfBlocksPerCol();
   double cols = getNumberOfPixelsPerBlockHoriz()*getNumberOfBlocksPerRow();;
   
   ossimDpt lr(ul.x + cols-1,
               ul.y + rows-1);
   return ossimDrect(ul, lr);
#endif
}

void ossimNitfImageHeaderV2_X::setImageId(const ossimString& value)
{
   ossimNitfCommon::setField(theImageId, value, 10);
}

void ossimNitfImageHeaderV2_X::setAquisitionDateTime(const ossimString& value)
{
   ossimNitfCommon::setField(theDateTime, value, 14);
}

void ossimNitfImageHeaderV2_X::setTargetId(const ossimString& value)
{
   ossimNitfCommon::setField(theTargetId, value, 17);
}

void ossimNitfImageHeaderV2_X::setTitle(const ossimString& value)
{
   ossimNitfCommon::setField(theTitle, value, 80);
}

void ossimNitfImageHeaderV2_X::setSecurityClassification(const ossimString& value)
{
   ossimNitfCommon::setField(theSecurityClassification, value, 1);
}

void ossimNitfImageHeaderV2_X::setEncryption(const ossimString& value)
{
   ossimNitfCommon::setField(theEncryption, value, 1);
}

void ossimNitfImageHeaderV2_X::setImageSource(const ossimString& value)
{
   ossimNitfCommon::setField(theImageSource, value, 42);
}

void ossimNitfImageHeaderV2_X::setJustification(const ossimString& value)
{
   ossimNitfCommon::setField(theJustification, value, 1);
}

void ossimNitfImageHeaderV2_X::setCoordinateSystem(const ossimString& value)
{
   ossimNitfCommon::setField(theCoordinateSystem, value, 1);
}

void ossimNitfImageHeaderV2_X::setGeographicLocation(const ossimString& value)
{
   ossimNitfCommon::setField(theGeographicLocation, value, 60);
}

void ossimNitfImageHeaderV2_X::setNumberOfComments(const ossimString& value)
{
   ossimNitfCommon::setField(theNumberOfComments, value, 1);
}

void ossimNitfImageHeaderV2_X::setCompression(const ossimString& value)
{
   ossimNitfCommon::setField(theCompression, value, 2);
}

void ossimNitfImageHeaderV2_X::setCompressionRateCode(const ossimString& value)
{
   ossimNitfCommon::setField(theCompressionRateCode, value, 4);
}

void ossimNitfImageHeaderV2_X::setDisplayLevel(const ossimString& value)
{
   ossimNitfCommon::setField(theDisplayLevel, value, 3, std::ios::right, '0');
}

void ossimNitfImageHeaderV2_X::setAttachmentLevel(const ossimString& value)
{
   ossimNitfCommon::setField(theAttachmentLevel, value, 3);
}

void ossimNitfImageHeaderV2_X::setPixelType(const ossimString& pixelType)
{
   ossimNitfCommon::setField(thePixelValueType, pixelType, 3);
}

void ossimNitfImageHeaderV2_X::setBitsPerPixel(ossim_uint32 bitsPerPixel)
{
   std::stringstream strm;
   strm << bitsPerPixel;
   ossimNitfCommon::setField(theNumberOfBitsPerPixelPerBand,
                             strm.str(),
                             2,
                             std::ios::right,
                             '0');
}

void ossimNitfImageHeaderV2_X::setActualBitsPerPixel(ossim_uint32 bitsPerPixel)
{
   std::stringstream strm;
   strm << bitsPerPixel;
   ossimNitfCommon::setField(theActualBitsPerPixelPerBand,
                             strm.str(),
                             2,
                             std::ios::right,
                             '0');
}

void ossimNitfImageHeaderV2_X::setImageMode(char mode)
{
   theImageMode[0] = mode;
}

void ossimNitfImageHeaderV2_X::setCategory(const ossimString& category)
{
   ossimNitfCommon::setField(theCategory, category, 8);
}

void ossimNitfImageHeaderV2_X::setRepresentation(const ossimString& rep)
{
   ossimNitfCommon::setField(theRepresentation, rep, 8);
}

void ossimNitfImageHeaderV2_X::setBlocksPerRow(ossim_uint32 blocks)
{
   std::stringstream strm;
   strm << blocks;
   ossimNitfCommon::setField(theNumberOfBlocksPerRow,
                             strm.str(),
                             4,
                             std::ios::right,
                             '0');
}

void ossimNitfImageHeaderV2_X::setBlocksPerCol(ossim_uint32 blocks)
{
   std::stringstream strm;
   strm << blocks;
   ossimNitfCommon::setField(theNumberOfBlocksPerCol,
                             strm.str(),
                             4,
                             std::ios::right,
                             '0');
}

void ossimNitfImageHeaderV2_X::setNumberOfPixelsPerBlockRow(ossim_uint32 pixels)
{
   std::stringstream strm;
   strm << pixels;
   ossimNitfCommon::setField(theNumberOfPixelsPerBlockHoriz,
                             strm.str(),
                             4,
                             std::ios::right,
                             '0');
}

void ossimNitfImageHeaderV2_X::setNumberOfPixelsPerBlockCol(ossim_uint32 pixels)
{
   std::stringstream strm;
   strm << pixels;
   ossimNitfCommon::setField(theNumberOfPixelsPerBlockVert,
                             strm.str(),
                             4,
                             std::ios::right,
                             '0');
}

ossim_uint64 ossimNitfImageHeaderV2_X::getDataLocation() const
{
   return theDataLocation;
}

ossimString ossimNitfImageHeaderV2_X::getImageId()const
{
   return theImageId;
}

ossimString ossimNitfImageHeaderV2_X::getAquisitionDateTime()const
{
   return theDateTime;
}

ossimString ossimNitfImageHeaderV2_X::getTargetId()const
{
   return ossimString(theTargetId).trim();
}

ossimString ossimNitfImageHeaderV2_X::getTitle()const
{
   return ossimString(theTitle).trim();
}

ossimString ossimNitfImageHeaderV2_X::getSecurityClassification()const
{
   return ossimString(theSecurityClassification).trim();
}

ossimString ossimNitfImageHeaderV2_X::getEncryption()const
{
   return ossimString(theEncryption).trim();
}

ossimString ossimNitfImageHeaderV2_X::getImageSource()const
{
   return ossimString(theImageSource).trim();
}

ossimString ossimNitfImageHeaderV2_X::getJustification()const
{
   return ossimString(theJustification).trim();
}

ossimString ossimNitfImageHeaderV2_X::getCoordinateSystem()const
{
   return ossimString(theCoordinateSystem).trim();
}

ossimString ossimNitfImageHeaderV2_X::getGeographicLocation()const
{
   return ossimString(theGeographicLocation).trim();
}

ossimString ossimNitfImageHeaderV2_X::getNumberOfComments()const
{
   return ossimString(theNumberOfComments).trim();
}

ossimString ossimNitfImageHeaderV2_X::getCompressionCode()const
{
   return ossimString(theCompression).trim();
}

ossimString ossimNitfImageHeaderV2_X::getCompressionRateCode()const
{
   return theCompressionRateCode;
}

ossim_uint32 ossimNitfImageHeaderV2_X::getDisplayLevel()const
{
   return ossimString(theDisplayLevel).toUInt32();
}

ossim_uint32 ossimNitfImageHeaderV2_X::getAttachmentLevel()const
{
   return ossimString(theAttachmentLevel).toUInt32();
}

ossimString ossimNitfImageHeaderV2_X::getPixelType()const
{
   return thePixelValueType;
}

ossim_uint32 ossimNitfImageHeaderV2_X::getBitsPerPixel()const
{
   return ossimString(theNumberOfBitsPerPixelPerBand).toUInt32();
}

ossim_uint32 ossimNitfImageHeaderV2_X::getActualBitsPerPixel()const
{
   return ossimString(theActualBitsPerPixelPerBand).toUInt32();
}

char ossimNitfImageHeaderV2_X::getImageMode()const
{
   return theImageMode[0];
}

ossimString ossimNitfImageHeaderV2_X::getCategory()const
{
   return theCategory;
}

ossimString ossimNitfImageHeaderV2_X::getRepresentation()const
{
   return theRepresentation;
}

ossim_uint32 ossimNitfImageHeaderV2_X::getBlocksPerRow()const
{
   return ossimString(theNumberOfBlocksPerRow).toUInt32();
}

ossim_uint32 ossimNitfImageHeaderV2_X::getBlocksPerCol()const
{
   return ossimString(theNumberOfBlocksPerCol).toUInt32();
}

ossim_uint32 ossimNitfImageHeaderV2_X::getNumberOfPixelsPerBlockRow()const
{
   return ossimString(theNumberOfPixelsPerBlockVert).toUInt32();
}

ossim_uint32 ossimNitfImageHeaderV2_X::getNumberOfPixelsPerBlockCol()const
{
   return ossimString(theNumberOfPixelsPerBlockHoriz).toUInt32();
}

bool ossimNitfImageHeaderV2_X::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   // Note: Currently not looking up all fieds only ones that make sense.
   
   const char* lookup;

   lookup = kwl.find( prefix, IID1_KW);
   if ( lookup )
   {
      setImageId( ossimString(lookup) );
   }
   lookup = kwl.find( prefix, IDATIM_KW);
   if ( lookup )
   {
      setAquisitionDateTime( ossimString(lookup) );
   }
   lookup = kwl.find( prefix, TGTID_KW);
   if ( lookup )
   {
      setTargetId( ossimString(lookup) );
   }
   lookup = kwl.find( prefix, IID2_KW);
   if ( lookup )
   {
      setTitle( ossimString(lookup) );
   }
   lookup = kwl.find( prefix, ITITLE_KW); // duplicate of above
   if ( lookup )
   {
      setTitle( ossimString(lookup) );
   }
   lookup = kwl.find( prefix, ISCLAS_KW);
   if ( lookup )
   {
      setSecurityClassification( ossimString(lookup) );
   }
   lookup = kwl.find( prefix, ENCRYP_KW);
   if ( lookup )
   {
      setEncryption( ossimString(lookup) );
   }  
   lookup = kwl.find( prefix, ISORCE_KW);
   if ( lookup )
   {
      setImageSource( ossimString(lookup) );
   }
   lookup = kwl.find( prefix, ICAT_KW);
   if ( lookup )
   {
      setCategory( ossimString(lookup) );
   }
   
   return true;
}

void ossimNitfImageHeaderV2_X::setProperty(ossimRefPtr<ossimProperty> property)
{
   ossimString name = property->getName();

   // Make case insensitive:
   name.upcase();
   
   std::ostringstream out;
   
   if(!property) return;
   
   if(name.contains(IID1_KW))
   {
      setImageId(property->valueToString());
   }
   else if(name.contains(IDATIM_KW))
   {
      setAquisitionDateTime(property->valueToString());
   }
   else if(name.contains(TGTID_KW))
   {
      setTargetId(property->valueToString());
   }
   else if(name.contains(IID2_KW)||
           name.contains(ITITLE_KW))
   {
      setTitle(property->valueToString());
   }
   else if(name.contains(ISCLAS_KW))
   {
      setSecurityClassification(property->valueToString());
   }
   else if(name.contains(ENCRYP_KW))
   {
      setEncryption(property->valueToString());
   }
   else if(name.contains(ISORCE_KW))
   {
      setImageSource(property->valueToString());
   }
   else if(name.contains(PVTYPE_KW))
   {
      setPixelType(property->valueToString());
   }
   else if(name.contains(IREP_KW))
   {
      setRepresentation(property->valueToString());
   }
   else if(name.contains(ICAT_KW))
   {
      setCategory(property->valueToString());
   }
   else if(name.contains(ABPP_KW))
   {
      setActualBitsPerPixel(property->valueToString().toUInt32());
   }
   else if(name.contains(PJUST_KW))
   {
      setJustification(property->valueToString());
   }
   else if(name.contains(ICORDS_KW))
   {
      setCoordinateSystem(property->valueToString());
   }
   else if(name.contains(IGEOLO_KW))
   {
      setGeographicLocation(property->valueToString());
   }
   else if(name.contains(NICOM_KW))
   {
      setNumberOfComments(property->valueToString());
   }
   else if(name.contains(IC_KW))
   {
      setCompression(property->valueToString());
   }
   else if(name.contains(COMRAT_KW))
   {
      setCompressionRateCode(property->valueToString());
   }
   else if(name.contains(ISYNC_KW))
   {
   }
   else if(name.contains(IMODE_KW))
   {
      setImageMode(*(property->valueToString().trim().begin()));
   }
   else if(name.contains(NBPR_KW))
   {
      setBlocksPerRow(property->valueToString().toUInt32());
   }
   else if(name.contains(NBPC_KW))
   {
      setBlocksPerCol(property->valueToString().toUInt32());
   }
   else if(name.contains(NPPBH_KW))
   {
      setNumberOfPixelsPerBlockRow(property->valueToString().toUInt32());
   }
   else if(name.contains(NPPBV_KW))
   {
      setNumberOfPixelsPerBlockCol(property->valueToString().toUInt32());
   }
   else if(name.contains(NBPP_KW))
   {
      setBitsPerPixel(property->valueToString().toUInt32());
   }
   else if(name.contains(IDLVL_KW))
   {
      setDisplayLevel(property->valueToString());
   }
   else if(name.contains(IALVL_KW))
   {
      setAttachmentLevel(property->valueToString());
   }
   else if(name.contains(ILOC_KW))
   {
   }
   else if(name.contains(IMAG_KW))
   {
   }
   else
   {
      ossimNitfImageHeader::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimNitfImageHeaderV2_X::getProperty(const ossimString& name)const
{
   ossimProperty* property = 0;

   if(name == IID1_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theImageId).trim());
   }
   else if(name == IDATIM_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theDateTime).trim());
   }
   else if(name == TGTID_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theTargetId).trim());
   }
   else if((name == IID2_KW)||
           (name == ITITLE_KW))
   {
      property = new ossimStringProperty(name,
                                         ossimString(theTitle).trim());
   }
   else if(name == ISCLAS_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theSecurityClassification).trim());
   }
   else if(name == ENCRYP_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theEncryption).trim());
   }
   else if(name == ISORCE_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theImageSource).trim());
   }
   else if(name == PVTYPE_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(thePixelValueType).trim());
   }
   else if(name == IREP_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theRepresentation).trim());
   }
   else if(name == ICAT_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theCategory).trim());
   }
   else if(name == ABPP_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theActualBitsPerPixelPerBand).trim());
   }
   else if(name == PJUST_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theJustification).trim());
   }
   else if(name == ICORDS_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theCoordinateSystem).trim());
   }
   else if(name == IGEOLO_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theGeographicLocation).trim());
   }
   else if(name == NICOM_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theNumberOfComments).trim());
   }
   else if(name == IC_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theCompression).trim());
   }
   else if(name == COMRAT_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theCompressionRateCode).trim());
   }
   else if(name == ISYNC_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theImageSyncCode).trim());
   }
   else if(name == IMODE_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theImageMode).trim());
   }
   else if(name == NBPR_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theNumberOfBlocksPerRow).trim());
   }
   else if(name == NBPC_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theNumberOfBlocksPerCol).trim());
   }
   else if(name == NPPBH_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theNumberOfPixelsPerBlockHoriz).trim());
   }
   else if(name == NPPBV_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theNumberOfPixelsPerBlockVert).trim());
   }
   else if(name == NBPP_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theNumberOfBitsPerPixelPerBand).trim());
   }
   else if(name == IDLVL_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theDisplayLevel).trim());
   }
   else if(name == IALVL_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theAttachmentLevel).trim());
   }
   else if(name == ILOC_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theImageLocation).trim());
   }
   else if(name == IMAG_KW)
   {
      property = new ossimStringProperty(name,
                                         ossimString(theImageMagnification).trim());
   }
   else if(name == "source")
   {
      property = new ossimStringProperty(name,
                                         getImageSource());
   }
   else if(name == "image_date")
   {
      property = new ossimStringProperty(name,
                                         getAcquisitionDateMonthDayYear());
   }
   else if(name == "image_title")
   {
      property = new ossimStringProperty(name,
                                         getTitle());
   }
   else
   {
      return ossimNitfImageHeader::getProperty(name);
                                         
   }
   return property;
}

void ossimNitfImageHeaderV2_X::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimNitfImageHeader::getPropertyNames(propertyNames);
   propertyNames.push_back(IID1_KW);
   propertyNames.push_back(IDATIM_KW);
   propertyNames.push_back(TGTID_KW);
   propertyNames.push_back(IID2_KW);
   propertyNames.push_back(ISCLAS_KW);
   propertyNames.push_back(ENCRYP_KW);
   propertyNames.push_back(ISORCE_KW);
   propertyNames.push_back(PVTYPE_KW);
   propertyNames.push_back(IREP_KW);
   propertyNames.push_back(ICAT_KW);
   propertyNames.push_back(ABPP_KW);
   propertyNames.push_back(PJUST_KW);
   propertyNames.push_back(ICORDS_KW);
   propertyNames.push_back(IGEOLO_KW);
   propertyNames.push_back(NICOM_KW);
   propertyNames.push_back(IC_KW);
   propertyNames.push_back(COMRAT_KW);
   propertyNames.push_back(ISYNC_KW);
   propertyNames.push_back(IMODE_KW);
   propertyNames.push_back(NBPR_KW);
   propertyNames.push_back(NBPC_KW);
   propertyNames.push_back(NPPBH_KW);
   propertyNames.push_back(NPPBV_KW);
   propertyNames.push_back(NBPP_KW);
   propertyNames.push_back(IDLVL_KW);
   propertyNames.push_back(IALVL_KW);
   propertyNames.push_back(ILOC_KW);
   propertyNames.push_back(IMAG_KW);

   // Temp addition (drb)
   propertyNames.push_back(ossimString("source"));
   propertyNames.push_back(ossimString("image_date"));
   propertyNames.push_back(ossimString("image_title"));
}

ossimString ossimNitfImageHeaderV2_X::getImageMagnification()const
{
   return ossimString(theImageMagnification).trim();
}

void ossimNitfImageHeaderV2_X::getImageLocation(ossimIpt& loc)const
{
   loc.x = ossimString((char*)(&theImageLocation[5])).toInt32();
   loc.y = ossimString((char*)theImageLocation,
                       (char*)(&theImageLocation[5])).toInt32();
}

void ossimNitfImageHeaderV2_X::setGeographicLocationDms(const ossimDpt& ul,
                                                        const ossimDpt& ur,
                                                        const ossimDpt& lr,
                                                        const ossimDpt& ll)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
      << ossimDms(ul.y, true).toString("ddmmss.ssssC").c_str()
      << ossimDms(ul.x, false).toString("dddmmss.ssssC").c_str()
      << ossimDms(ur.y, true).toString("ddmmss.ssssC").c_str()
      << ossimDms(ur.x, false).toString("dddmmss.ssssC").c_str()
      << ossimDms(lr.y, true).toString("ddmmss.ssssC").c_str()
      << ossimDms(lr.x, false).toString("dddmmss.ssssC").c_str()
      << ossimDms(ll.y, true).toString("ddmmss.ssssC").c_str()
      << ossimDms(ll.x, false).toString("dddmmss.ssssC").c_str()
      << std::endl;
      
      checkForGeographicTiePointTruncation(ul);
      checkForGeographicTiePointTruncation(ur);
      checkForGeographicTiePointTruncation(lr);
      checkForGeographicTiePointTruncation(ll);
   }
   
   theCoordinateSystem[0] = 'G';
   
   memcpy(theGeographicLocation,
          ossimNitfCommon::encodeGeographicDms(ul,ur,lr,ll).c_str(), 60);
}

void ossimNitfImageHeaderV2_X::setGeographicLocationDecimalDegrees(
   const ossimDpt& ul,
   const ossimDpt& ur,
   const ossimDpt& lr,
   const ossimDpt& ll)
{
   theCoordinateSystem[0] = 'D';
   memcpy(theGeographicLocation,
          ossimNitfCommon::encodeGeographicDecimalDegrees(
             ul, ur, lr, ll).c_str(), 60);
}

void ossimNitfImageHeaderV2_X::setUtmNorth(ossim_uint32 zone,
                                           const ossimDpt& ul,
                                           const ossimDpt& ur,
                                           const ossimDpt& lr,
                                           const ossimDpt& ll)
{
   theCoordinateSystem[0] = 'N';
   
   memcpy(theGeographicLocation,
          ossimNitfCommon::encodeUtm(zone, ul, ur, lr, ll).c_str(), 60);
}

void ossimNitfImageHeaderV2_X::setUtmSouth(ossim_uint32 zone,
                                           const ossimDpt& ul,
                                           const ossimDpt& ur,
                                           const ossimDpt& lr,
                                           const ossimDpt& ll)
{
   theCoordinateSystem[0] = 'S';
   
   memcpy(theGeographicLocation,
          ossimNitfCommon::encodeUtm(zone, ul, ur, lr, ll).c_str(), 60);
}


bool ossimNitfImageHeaderV2_X::saveState(ossimKeywordlist& kwl, const ossimString& prefix)const
{
   bool result = ossimNitfImageHeader::saveState(kwl, prefix);
   
   kwl.add(prefix, IM_KW.c_str(),       theType);
   kwl.add(prefix, IID1_KW.c_str(),     theImageId);
   kwl.add(prefix, IDATIM_KW.c_str(),   theDateTime);
   kwl.add(prefix, TGTID_KW.c_str(),    theTargetId);
   kwl.add(prefix, IID2_KW.c_str(),     theTitle);
   kwl.add(prefix, ISCLAS_KW.c_str(),   theSecurityClassification);
   kwl.add(prefix, ENCRYP_KW.c_str(),   theEncryption);
   kwl.add(prefix, ISORCE_KW.c_str(),   theImageSource);
   kwl.add(prefix, NROWS_KW.c_str(),    theSignificantRows);
   kwl.add(prefix, NCOLS_KW.c_str(),    theSignificantCols);
   kwl.add(prefix, PVTYPE_KW.c_str(),   thePixelValueType);
   kwl.add(prefix, IREP_KW.c_str(),     theRepresentation);
   kwl.add(prefix, ABPP_KW.c_str(),     theActualBitsPerPixelPerBand);
   kwl.add(prefix, ICAT_KW.c_str(),     theCategory);
   kwl.add(prefix, PJUST_KW.c_str(),    theJustification);
   kwl.add(prefix, ICORDS_KW.c_str(),   theCoordinateSystem);
   kwl.add(prefix, NICOM_KW.c_str(),    theNumberOfComments);
   kwl.add(prefix, ICOM_KW.c_str(),     ossimString(theImageComments));
   kwl.add(prefix, IGEOLO_KW.c_str(),   theGeographicLocation);
   kwl.add(prefix, IC_KW.c_str(),       theCompression);
   kwl.add(prefix, COMRAT_KW.c_str(),   theCompressionRateCode);
   kwl.add(prefix, NBANDS_KW.c_str(),   theNumberOfBands);
   kwl.add(prefix, ISYNC_KW.c_str(),    theImageSyncCode);
   kwl.add(prefix, IMODE_KW.c_str(),    theImageMode);
   kwl.add(prefix, NBPR_KW.c_str(),     theNumberOfBlocksPerRow);
   kwl.add(prefix, NBPC_KW.c_str(),     theNumberOfBlocksPerCol);
   kwl.add(prefix, NPPBH_KW.c_str(),    theNumberOfPixelsPerBlockHoriz);
   kwl.add(prefix, NPPBV_KW.c_str(),    theNumberOfPixelsPerBlockVert);
   kwl.add(prefix, NBPP_KW.c_str(),     theNumberOfBitsPerPixelPerBand);
   kwl.add(prefix, IDLVL_KW.c_str(),    theDisplayLevel);
   kwl.add(prefix, IALVL_KW.c_str(),    theAttachmentLevel);
   kwl.add(prefix, ILOC_KW.c_str(),     theImageLocation);
   kwl.add(prefix, IMAG_KW.c_str(),     theImageMagnification);
   kwl.add(prefix, UDIDL_KW.c_str(),    theUserDefinedImageDataLength);
   kwl.add(prefix, UDOFL_KW.c_str(),    theUserDefinedOverflow);
   kwl.add(prefix, IXSHDL_KW.c_str(),   theExtendedSubheaderDataLen);
   kwl.add(prefix, IXSOFL_KW.c_str(),   theExtendedSubheaderOverflow);
   kwl.add(prefix, IMDATOFF_KW.c_str(), theBlockedImageDataOffset);
   kwl.add(prefix, BMRLNTH_KW.c_str(),  theBlockMaskRecordLength);
   
   return result;
}






