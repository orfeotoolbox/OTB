//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: ICHIPB tag class declaration.
//
// See document STDI-0002 Table 5-2 for more info.
// 
// http://164.214.2.51/ntb/baseline/docs/stdi0002/final.pdf
//
//----------------------------------------------------------------------------
// $Id: ossimNitfIchipbTag.cpp 10177 2007-01-03 19:22:09Z gpotts $

#include <cstring> /* for memcpy */
#include <iomanip>

#include <ossim/support_data/ossimNitfIchipbTag.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimStringProperty.h>

static const ossimString XFRM_FLAG_KW = "XFRM_FLAG";
static const ossimString SCALE_FACTOR_KW = "SCALE_FACTOR";
static const ossimString ANAMRPH_CORR_KW = "ANAMRPH_CORR";
static const ossimString SCANBLK_NUM_KW = "SCANBLK_NUM";
static const ossimString OP_ROW_11_KW = "OP_ROW_11";
static const ossimString OP_COL_11_KW = "OP_COL_11";
static const ossimString OP_ROW_12_KW = "OP_ROW_12";
static const ossimString OP_COL_12_KW = "OP_COL_12";
static const ossimString OP_ROW_21_KW = "OP_ROW_21";
static const ossimString OP_COL_21_KW = "OP_COL_21";
static const ossimString OP_ROW_22_KW = "OP_ROW_22";
static const ossimString OP_COL_22_KW = "OP_COL_22";
static const ossimString FI_ROW_11_KW = "FI_ROW_11";
static const ossimString FI_COL_11_KW = "FI_COL_11";
static const ossimString FI_ROW_12_KW = "FI_ROW_12";
static const ossimString FI_COL_12_KW = "FI_COL_12";
static const ossimString FI_ROW_21_KW = "FI_ROW_21";
static const ossimString FI_COL_21_KW = "FI_COL_21";
static const ossimString FI_ROW_22_KW = "FI_ROW_22";
static const ossimString FI_COL_22_KW = "FI_COL_22";
static const ossimString FI_ROW_KW = "FI_ROW";
static const ossimString FI_COL_KW = "FI_COL";

RTTI_DEF1(ossimNitfIchipbTag, "ossimNitfIchipbTag", ossimNitfRegisteredTag);

ossimNitfIchipbTag::ossimNitfIchipbTag()
{
   clearFields();
}

ossimString ossimNitfIchipbTag::getRegisterTagName() const
{
   return ossimString("ICHIPB");
}

void ossimNitfIchipbTag::parseStream(std::istream& in)
{
   clearFields();

   in.read(theXfrmFlag,     XFRM_FLAG_SIZE);
   in.read(theScaleFactor,  SCALE_FACTOR_SIZE);
   in.read(theAnamrphCorr,  ANAMRPH_CORR_SIZE);
   in.read(theScanBlock,    SCANBLK_NUM_SIZE);
   in.read(theOpRow11,      OP_ROW_11_SIZE);
   in.read(theOpCol11,      OP_COL_11_SIZE);
   in.read(theOpRow12,      OP_ROW_12_SIZE);
   in.read(theOpCol12,      OP_COL_12_SIZE);
   in.read(theOpRow21,      OP_ROW_21_SIZE);
   in.read(theOpCol21,      OP_COL_21_SIZE);
   in.read(theOpRow22,      OP_ROW_22_SIZE);
   in.read(theOpCol22,      OP_COL_22_SIZE);
   in.read(theFiRow11,      FI_ROW_11_SIZE);
   in.read(theFiCol11,      FI_COL_11_SIZE);
   in.read(theFiRow12,      FI_ROW_12_SIZE);
   in.read(theFiCol12,      FI_COL_12_SIZE);
   in.read(theFiRow21,      FI_ROW_21_SIZE);
   in.read(theFiCol21,      FI_COL_21_SIZE);
   in.read(theFiRow22,      FI_ROW_22_SIZE);
   in.read(theFiCol22,      FI_COL_22_SIZE);
   in.read(theFullImageRow, FI_ROW_SIZE);
   in.read(theFullImageCol, FI_COL_SIZE);
}

void ossimNitfIchipbTag::writeStream(std::ostream& out)
{
   out.write(theXfrmFlag,     XFRM_FLAG_SIZE);
   out.write(theScaleFactor,  SCALE_FACTOR_SIZE);
   out.write(theAnamrphCorr,  ANAMRPH_CORR_SIZE);
   out.write(theScanBlock,    SCANBLK_NUM_SIZE);
   out.write(theOpRow11,      OP_ROW_11_SIZE);
   out.write(theOpCol11,      OP_COL_11_SIZE);
   out.write(theOpRow12,      OP_ROW_12_SIZE);
   out.write(theOpCol12,      OP_COL_12_SIZE);
   out.write(theOpRow21,      OP_ROW_21_SIZE);
   out.write(theOpCol21,      OP_COL_21_SIZE);
   out.write(theOpRow22,      OP_ROW_22_SIZE);
   out.write(theOpCol22,      OP_COL_22_SIZE);
   out.write(theFiRow11,      FI_ROW_11_SIZE);
   out.write(theFiCol11,      FI_COL_11_SIZE);
   out.write(theFiRow12,      FI_ROW_12_SIZE);
   out.write(theFiCol12,      FI_COL_12_SIZE);
   out.write(theFiRow21,      FI_ROW_21_SIZE);
   out.write(theFiCol21,      FI_COL_21_SIZE);
   out.write(theFiRow22,      FI_ROW_22_SIZE);
   out.write(theFiCol22,      FI_COL_22_SIZE);
   out.write(theFullImageRow, FI_ROW_SIZE);
   out.write(theFullImageCol, FI_COL_SIZE);
}

ossim_uint32 ossimNitfIchipbTag::getSizeInBytes()const
{
   return 224;
}

void ossimNitfIchipbTag::clearFields()
{
   memset(theXfrmFlag,     '0', XFRM_FLAG_SIZE);
   memset(theScaleFactor,  '0', SCALE_FACTOR_SIZE);
   memset(theAnamrphCorr,  '0', ANAMRPH_CORR_SIZE);
   memset(theScanBlock,    '0', SCANBLK_NUM_SIZE);
   memset(theOpRow11,      '0', OP_ROW_11_SIZE);
   memset(theOpCol11,      '0', OP_COL_11_SIZE);
   memset(theOpRow12,      '0', OP_ROW_12_SIZE);
   memset(theOpCol12,      '0', OP_COL_12_SIZE);
   memset(theOpRow21,      '0', OP_ROW_21_SIZE);
   memset(theOpCol21,      '0', OP_COL_21_SIZE);
   memset(theOpRow22,      '0', OP_ROW_22_SIZE);
   memset(theOpCol22,      '0', OP_COL_22_SIZE);
   memset(theFiRow11,      '0', FI_ROW_11_SIZE);
   memset(theFiCol11,      '0', FI_COL_11_SIZE);
   memset(theFiRow12,      '0', FI_ROW_12_SIZE);
   memset(theFiCol12,      '0', FI_COL_12_SIZE);
   memset(theFiRow21,      '0', FI_ROW_21_SIZE);
   memset(theFiCol21,      '0', FI_COL_21_SIZE);
   memset(theFiRow22,      '0', FI_ROW_22_SIZE);
   memset(theFiCol22,      '0', FI_COL_22_SIZE);
   memset(theFullImageRow, '0', FI_ROW_SIZE);
   memset(theFullImageCol, '0', FI_COL_SIZE);

   theXfrmFlag[XFRM_FLAG_SIZE]       = '\0';
   theScaleFactor[SCALE_FACTOR_SIZE] = '\0';
   theAnamrphCorr[ANAMRPH_CORR_SIZE] = '\0';
   theScanBlock[SCANBLK_NUM_SIZE]    = '\0';
   theOpRow11[OP_ROW_11_SIZE]        = '\0';
   theOpCol11[OP_COL_11_SIZE]        = '\0';
   theOpRow12[OP_ROW_12_SIZE]        = '\0';
   theOpCol12[OP_COL_12_SIZE]        = '\0';
   theOpRow21[OP_ROW_21_SIZE]        = '\0';
   theOpCol21[OP_COL_21_SIZE]        = '\0';
   theOpRow22[OP_ROW_22_SIZE]        = '\0';
   theOpCol22[OP_COL_22_SIZE]        = '\0';
   theFiRow11[FI_ROW_11_SIZE]        = '\0';
   theFiCol11[FI_COL_11_SIZE]        = '\0';
   theFiRow12[FI_ROW_12_SIZE]        = '\0';
   theFiCol12[FI_COL_12_SIZE]        = '\0';
   theFiRow21[FI_ROW_21_SIZE]        = '\0';
   theFiCol21[FI_COL_21_SIZE]        = '\0';
   theFiRow22[FI_ROW_22_SIZE]        = '\0';
   theFiCol22[FI_COL_22_SIZE]        = '\0';
   theFullImageRow[FI_ROW_SIZE]      = '\0';
   theFullImageCol[FI_COL_SIZE]      = '\0';
}

std::ostream& ossimNitfIchipbTag::print(std::ostream& out) const
{
   out << setiosflags(std::ios::left) << "ossimNitfIchipbTag::print"
       << std::setw(24) << "\nXFRM_FLAG:"     <<theXfrmFlag
       << std::setw(24) << "\nSCALE_FACTOR:"  <<theScaleFactor
       << std::setw(24) << "\nANAMRPH_CORR:"  <<theAnamrphCorr
       << std::setw(24) << "\nSCANBLK_NUM:"   <<theScanBlock
       << std::setw(24) << "\nOP_ROW_11:"     <<theOpRow11
       << std::setw(24) << "\nOP_COL_11:"     <<theOpCol11
       << std::setw(24) << "\nOP_ROW_12:"     <<theOpRow12
       << std::setw(24) << "\nOP_COL_12:"     <<theOpCol12
       << std::setw(24) << "\nOP_ROW_21:"     <<theOpRow21
       << std::setw(24) << "\nOP_COL_21:"     <<theOpCol21
       << std::setw(24) << "\nOP_ROW_22:"     <<theOpRow22
       << std::setw(24) << "\nOP_COL_22:"     <<theOpCol22
       << std::setw(24) << "\nFI_ROW_11:"     <<theFiRow11
       << std::setw(24) << "\nFI_COL_11:"     <<theFiCol11
       << std::setw(24) << "\nFI_ROW_12:"     <<theFiRow12
       << std::setw(24) << "\nFI_COL_12:"     <<theFiCol12
       << std::setw(24) << "\nFI_ROW_21:"     <<theFiRow21
       << std::setw(24) << "\nFI_COL_21:"     <<theFiCol21
       << std::setw(24) << "\nFI_ROW_22:"     <<theFiRow22
       << std::setw(24) << "\nFI_COL_22:"     <<theFiCol22
       << std::setw(24) << "\nFI_ROW:"        <<theFullImageRow
       << std::setw(24) << "\nFI_COL:"        <<theFullImageCol
       << std::endl;

   return out;
}

bool ossimNitfIchipbTag::getXfrmFlag() const
{
   return ossimString::toBool(theXfrmFlag);
}

ossim_float64 ossimNitfIchipbTag::getScaleFactor() const
{
   return ossimString::toFloat64(theScaleFactor);
}

bool ossimNitfIchipbTag::getAnamrphCorrFlag() const
{
   return ossimString::toBool(theAnamrphCorr);
}

ossim_uint32 ossimNitfIchipbTag::getScanBlock() const
{
   return ossimString::toUInt32(theScanBlock);
}

ossim_float64 ossimNitfIchipbTag::getOpRow11() const
{
   return ossimString::toFloat64(theOpRow11);
}

ossim_float64 ossimNitfIchipbTag::getOpCol11() const
{
   return ossimString::toFloat64(theOpCol11);
}

ossim_float64 ossimNitfIchipbTag::getOpRow12() const
{
   return ossimString::toFloat64(theOpRow12);
}

ossim_float64 ossimNitfIchipbTag::getOpCol12() const
{
   return ossimString::toFloat64(theOpCol12);
}

ossim_float64 ossimNitfIchipbTag::getOpRow21() const
{
   return ossimString::toFloat64(theOpRow21);
}

ossim_float64 ossimNitfIchipbTag::getOpCol21()
   const
{ return ossimString::toFloat64(theOpCol21);
}

ossim_float64 ossimNitfIchipbTag::getOpRow22() const
{
   return ossimString::toFloat64(theOpRow22);
}

ossim_float64 ossimNitfIchipbTag::getOpCol22() const
{
   return ossimString::toFloat64(theOpCol22);
}

ossim_float64 ossimNitfIchipbTag::getFiRow11() const
{
   return ossimString::toFloat64(theFiRow11);
}

ossim_float64 ossimNitfIchipbTag::getFiCol11() const
{
   return ossimString::toFloat64(theFiCol11);
}

ossim_float64 ossimNitfIchipbTag::getFiRow12() const
{
   return ossimString::toFloat64(theFiRow12);
}

ossim_float64 ossimNitfIchipbTag::getFiCol12() const
{
   return ossimString::toFloat64(theFiCol12);
}

ossim_float64 ossimNitfIchipbTag::getFiRow21() const
{
   return ossimString::toFloat64(theFiRow21);
}

ossim_float64 ossimNitfIchipbTag::getFiCol21() const
{
   return ossimString::toFloat64(theFiCol21);
}

ossim_float64 ossimNitfIchipbTag::getFiRow22() const
{
   return ossimString::toFloat64(theFiRow22);
}

ossim_float64 ossimNitfIchipbTag::getFiCol22() const
{
   return ossimString::toFloat64(theFiCol22);
}

ossim_uint32 ossimNitfIchipbTag::getFullImageRows() const
{
   return ossimString::toUInt32(theFullImageRow);
}

ossim_uint32 ossimNitfIchipbTag::getFullImageCols() const
{
   return ossimString::toUInt32(theFullImageCol);
}
 
void ossimNitfIchipbTag::getImageRect(ossimDrect& rect) const
{
   ossimDpt pt;

   pt.x = getOpCol11() - 0.5;
   pt.y = getOpRow11() - 0.5;
   rect.set_ul(pt);

   pt.x = getOpCol22() - 0.5;
   pt.y = getOpRow22() - 0.5;
   rect.set_lr(pt);
}

void ossimNitfIchipbTag::getFullImageRect(ossimDrect& rect) const
{
   ossimDpt pt;

   pt.x = getFiCol11() - 0.5;
   pt.y = getFiRow11() - 0.5;
   rect.set_ul(pt);

   pt.x = getFiCol22() - 0.5;
   pt.y = getFiRow22() - 0.5;
   rect.set_lr(pt);
}

void ossimNitfIchipbTag::getSubImageOffset(ossimDpt& pt) const
{
   pt.x = getFiCol11() - 0.5;
   pt.y = getFiRow11() - 0.5;
}

void ossimNitfIchipbTag::setProperty(ossimRefPtr<ossimProperty> property)
{
   ossimNitfRegisteredTag::setProperty(property);   
}

ossimRefPtr<ossimProperty> ossimNitfIchipbTag::getProperty(const ossimString& name)const
{
   ossimProperty* result = 0;

   if(name == XFRM_FLAG_KW)
   {
      result = new ossimStringProperty(name, theXfrmFlag);
   }
   else if(name == SCALE_FACTOR_KW)
   {
      result = new ossimStringProperty(name, theScaleFactor);
   }
   else if(name == ANAMRPH_CORR_KW)
   {
      result = new ossimStringProperty(name, theAnamrphCorr);
   }
   else if(name == SCANBLK_NUM_KW)
   {
      result = new ossimStringProperty(name, theScanBlock);
   }
   else if(name == OP_ROW_11_KW)
   {
      result = new ossimStringProperty(name, theOpRow11);
   }
   else if(name == OP_COL_11_KW)
   {
      result = new ossimStringProperty(name, theOpCol11);
   }
   else if(name == OP_ROW_12_KW)
   {
      result = new ossimStringProperty(name,theOpRow12 );
   }
   else if(name == OP_COL_12_KW)
   {
      result = new ossimStringProperty(name, theOpCol12);
   }
   else if(name == OP_ROW_21_KW)
   {
      result = new ossimStringProperty(name,theOpRow21);
   }
   else if(name == OP_COL_21_KW)
   {
      result = new ossimStringProperty(name, theOpCol21);
   }
   else if(name == OP_ROW_22_KW)
   {
      result = new ossimStringProperty(name, theOpRow22);
   }
   else if(name == OP_COL_22_KW)
   {
      result = new ossimStringProperty(name, theOpCol22);
   }
   else if(name == FI_ROW_11_KW)
   {
      result = new ossimStringProperty(name, theFiRow11);
   }
   else if(name == FI_COL_11_KW)
   {
      result = new ossimStringProperty(name, theFiCol11);
   }
   else if(name == FI_ROW_12_KW)
   {
      result = new ossimStringProperty(name, theFiRow12);
   }
   else if(name == FI_COL_12_KW)
   {
      result = new ossimStringProperty(name, theFiCol12);
   }
   else if(name == FI_ROW_21_KW)
   {
      result = new ossimStringProperty(name, theFiRow21);
   }
   else if(name == FI_COL_21_KW)
   {
      result = new ossimStringProperty(name, theFiCol21);
   }
   else if(name == FI_ROW_22_KW)
   {
      result = new ossimStringProperty(name, theFiRow22);
   }
   else if(name == FI_COL_22_KW)
   {
      result = new ossimStringProperty(name, theFiCol22);
   }
   else if(name == FI_ROW_KW)
   {
      result = new ossimStringProperty(name, theFullImageRow);
   }
   else if(name == FI_COL_KW)
   {
      result = new ossimStringProperty(name, theFullImageCol);
   }
   else
   {
      return ossimNitfRegisteredTag::getProperty(name);
   }

   return result;
}

void ossimNitfIchipbTag::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimNitfRegisteredTag::getPropertyNames(propertyNames);

   propertyNames.push_back(XFRM_FLAG_KW);
   propertyNames.push_back(SCALE_FACTOR_KW);
   propertyNames.push_back(ANAMRPH_CORR_KW);
   propertyNames.push_back(SCANBLK_NUM_KW);
   propertyNames.push_back(OP_ROW_11_KW);
   propertyNames.push_back(OP_COL_11_KW);
   propertyNames.push_back(OP_ROW_12_KW);
   propertyNames.push_back(OP_COL_12_KW);
   propertyNames.push_back(OP_ROW_21_KW);
   propertyNames.push_back(OP_COL_21_KW);
   propertyNames.push_back(OP_ROW_22_KW);
   propertyNames.push_back(OP_COL_22_KW);
   propertyNames.push_back(FI_ROW_11_KW);
   propertyNames.push_back(FI_COL_11_KW);
   propertyNames.push_back(FI_ROW_12_KW);
   propertyNames.push_back(FI_COL_12_KW);
   propertyNames.push_back(FI_ROW_21_KW);
   propertyNames.push_back(FI_COL_21_KW);
   propertyNames.push_back(FI_ROW_22_KW);
   propertyNames.push_back(FI_COL_22_KW);
   propertyNames.push_back(FI_ROW_KW);
   propertyNames.push_back(FI_COL_KW);
}
