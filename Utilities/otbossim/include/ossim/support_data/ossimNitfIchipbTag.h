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
// $Id: ossimNitfIchipbTag.h 22013 2012-12-19 17:37:20Z dburken $
#ifndef ossimNitfIchipbTag_HEADER
#define ossimNitfIchipbTag_HEADER 1

#include <ossim/base/ossimConstants.h>
#include <ossim/support_data/ossimNitfRegisteredTag.h>

class ossimDpt;
class ossimDrect;
class ossim2dTo2dTransform;
class OSSIM_DLL ossimNitfIchipbTag : public ossimNitfRegisteredTag
{
public:
   enum 
   {
      XFRM_FLAG_SIZE    =  2,
      
      SCALE_FACTOR_SIZE = 10,
      
      ANAMRPH_CORR_SIZE =  2,
      
      SCANBLK_NUM_SIZE  =  2,

      OP_ROW_11_SIZE    = 12,
      OP_COL_11_SIZE    = 12,

      OP_ROW_12_SIZE    = 12,
      OP_COL_12_SIZE    = 12,

      OP_ROW_21_SIZE    = 12,
      OP_COL_21_SIZE    = 12,

      OP_ROW_22_SIZE    = 12,
      OP_COL_22_SIZE    = 12,
      
      FI_ROW_11_SIZE    = 12,
      FI_COL_11_SIZE    = 12,

      FI_ROW_12_SIZE    = 12,
      FI_COL_12_SIZE    = 12,

      FI_ROW_21_SIZE    = 12,
      FI_COL_21_SIZE    = 12,

      FI_ROW_22_SIZE    = 12,
      FI_COL_22_SIZE    = 12,

      FI_ROW_SIZE       =  8,
      FI_COL_SIZE       =  8
      //                ------
      //                 224
   };
   
   /** default constructor */
   ossimNitfIchipbTag();

   /**
    * Parse method.
    *
    * @param in Stream to parse.
    */
   virtual void parseStream(std::istream& in);
   
   /**
    * Write method.
    *
    * @param out Stream to write to.
    */
   virtual void writeStream(std::ostream& out);
   
   /**
    * Clears all string fields within the record to some default nothingness.
    */
   virtual void clearFields();
   
   /**
    * @brief Print method that outputs a key/value type format
    * adding prefix to keys.
    * @param out Stream to output to.
    * @param prefix Prefix added to key like "image0.";
    */
   virtual std::ostream& print(std::ostream& out,
                               const std::string& prefix=std::string()) const;

   /**
    * @return The Non-linear transformation flag.
    */
   bool getXfrmFlag() const;

   /**
    * @return Scale factor relative to R0 (original full res image resolution).
    */
   ossim_float64 getScaleFactor() const;

   /**
    * @return Anamophic correction indicator.
    */
   bool getAnamrphCorrFlag() const;

   /**
    * @return Scan block number or scan block index.
    */
   ossim_uint32 getScanBlock() const;

   /**
    * @return Output product row number component of grid point index (1,1) for
    * intelligent data,
    */
   ossim_float64 getOpRow11() const;

   /**
    * Output product column number component of grid point index (1,1) for
    * intelligent data.
    */
   ossim_float64 getOpCol11() const;

   /**
    * @return Output product row number component of grid point index (1,2) for
    * intelligent data,
    */
   ossim_float64 getOpRow12() const;

   /**
    * Output product column number component of grid point index (1,2) for
    * intelligent data.
    */
   ossim_float64 getOpCol12() const;

   /**
    * @return Output product row number component of grid point index (2,1) for
    * intelligent data,
    */
   ossim_float64 getOpRow21() const;

   /**
    * Output product column number component of grid point index (2,1) for
    * intelligent data.
    */
   ossim_float64 getOpCol21() const;

   /**
    * @return Output product row number component of grid point index (2,2) for
    * intelligent data,
    */
   ossim_float64 getOpRow22() const;
   
   /**
    * @return Output product row number component of grid point index (2,2) for
    * intelligent data,
    */
   ossim_float64 getOpCol22() const;

   /**
    * @return Grid point (1,1) row number in full image coordinate system.
    */
   ossim_float64 getFiRow11() const;
   
   /**
    * @return Grid point (1,1) column number in full image coordinate system.
    */
   ossim_float64 getFiCol11() const;
   
   /**
    * @return Grid point (1,2) row number in full image coordinate system.
    */
   ossim_float64 getFiRow12() const;
   
   /**
    * @return Grid point (1,2) row number in full image coordinate system.
    */
   ossim_float64 getFiCol12() const;
   
   /**
    * @return Grid point (2,1) row number in full image coordinate system.
    */
   ossim_float64 getFiRow21() const;

   /**
    * @return Grid point (2,1) row number in full image coordinate system.
    */
   ossim_float64 getFiCol21() const;

   /**
    * @return Grid point (2,2) row number in full image coordinate system.
    */
   ossim_float64 getFiRow22() const;

   /**
    * @return Grid point (2,2) row number in full image coordinate system.
    */
   ossim_float64 getFiCol22() const;

   /**
    * @return Full image number of rows.
    */
   ossim_uint32 getFullImageRows() const;

   /**
    * @return Full image number of cols.
    */
   ossim_uint32 getFullImageCols() const;

   /**
    * Initializes rect with the image rectangle without any sub image offset
    * applied.   This is the rectangle of the chip starting at (0,0).
    *
    * @param rect The rect to initialize.
    *
    * @note Rectangle is shifted by -0.5 since per spec (0,0) is the
    * considered the upper left corner of the upper left pixel and we use a
    * point method where (0,0) refers to the center of the pixel.
    */
   void getImageRect(ossimDrect& rect) const;

    /**
    * Initializes rect with the image rectangle with sub image offset
    * applied.   This is the rectangle of the chip relative to the full
    * image or starting at the sub image offset.
    *
    * @param rect The rect to initialize.
    *
    * @note Rectangle is shifted by -0.5 since per spec (0,0) is the
    * considered the upper left corner of the upper left pixel and we use a
    * point method where (0,0) refers to the center of the pixel.
    */
   void getFullImageRect(ossimDrect& rect) const;

   ossim2dTo2dTransform* newTransform()const;
   
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   
protected:


   /**
    * FIELD: XFRM_FLAG
    *
    * required 2 byte field
    *
    * 00 or 01
    * 
    * Non-linear transformation flag.
    *
    * NOTE:  If 01 remaining data zero filled.
    */
   char theXfrmFlag[XFRM_FLAG_SIZE+1];

   
   /**
    * FIELD: SCALE_FACTOR
    *
    * required 10 byte field
    *
    * xxxx.xxxxx
    *
    * Scale factor relative to R0 (original full res image resolution).
    */
   char theScaleFactor[SCALE_FACTOR_SIZE+1];
      
  
   /**
    * FIELD: ANAMRPH_CORR
    *
    * required 2 byte field
    *
    * 00 or 01
    * 
    * Anamophic correction indicator.
    */
   char theAnamrphCorr[ANAMRPH_CORR_SIZE+1];
   
   /**
    * FIELD: SCANBLK_NUM
    *
    * required 2 byte field
    *
    * 00-99, 00 if not applicable
    * 
    * Scan block number or scan block index.
    */
   char theScanBlock[SCANBLK_NUM_SIZE+1];
   
   /**
    * FIELD: OP_ROW_11
    *
    * required 12 byte field
    *
    * xxxxxxxx.yyy typically 00000000.500
    * 
    * Output product row number component of grid point index (1,1) for
    * intelligent data.
    */
   char theOpRow11[OP_ROW_11_SIZE+1];

   /**
    * FIELD: OP_COL_11
    *
    * required 12 byte field
    *
    * xxxxxxxx.yyy typically 00000000.500
    * 
    * Output product column number component of grid point index (1,1) for
    * intelligent data.
    * 
    */
   char theOpCol11[OP_COL_11_SIZE+1];

   /**
    * FIELD: OP_ROW_12
    *
    * required 12 byte field
    *
    * xxxxxxxx.yyy
    * 
    * Output product row number component of grid point index (1,2) for
    * intelligent data.
    */
   char theOpRow12[OP_ROW_12_SIZE+1];

   /**
    * FIELD: OP_COL_12
    *
    * required 12 byte field
    *
    * xxxxxxxx.yyy
    * 
    * Output product column number component of grid point index (1,2) for
    * intelligent data.
    */
   char theOpCol12[OP_COL_12_SIZE+1];
   
   /**
    * FIELD: OP_ROW_21
    *
    * required 12 byte field
    *
    * xxxxxxxx.yyy
    * 
    * Output product row number component of grid point index (2,1) for
    * intelligent data.
    */
   char theOpRow21[OP_ROW_21_SIZE+1];

   /**
    * FIELD: OP_COL_21
    *
    * required 12 byte field
    *
    * xxxxxxxx.yyy
    * 
    * Output product column number component of grid point index (2,1) for
    * intelligent data.
    */
   char theOpCol21[OP_COL_21_SIZE+1];
   
   /**
    * FIELD: OP_ROW_22
    *
    * required 12 byte field
    *
    * xxxxxxxx.yyy
    * 
    * Output product row number component of grid point index (2,2) for
    * intelligent data.
    */
   char theOpRow22[OP_ROW_22_SIZE+1];

   /**
    * FIELD: OP_COL_22
    *
    * required 12 byte field
    *
    * xxxxxxxx.yyy
    * 
    * Output product column number component of grid point index (2,2) for
    * intelligent data.
    */
   char theOpCol22[OP_COL_22_SIZE+1];

   /**
    * FIELD: FI_ROW_11
    *
    * required 12 byte field
    *
    * xxxxxxxx.yyy typically 00000000.500
    * 
    * Grid point (1,1) row number in full image coordinate system.
    */
   char theFiRow11[FI_ROW_11_SIZE+1];

   /**
    * FIELD: FI_COL_11
    *
    * required 12 byte field
    *
    * xxxxxxxx.yyy typically 00000000.500
    * 
    * Grid point (1,1) column number in full image coordinate system.
    */
   char theFiCol11[FI_COL_11_SIZE+1];

   /**
    * FIELD: FI_ROW_12
    *
    * required 12 byte field
    *
    * xxxxxxxx.yyy
    * 
    * Grid point (1,2) row number in full image coordinate system.
    */
   char theFiRow12[FI_ROW_12_SIZE+1];

   /**
    * FIELD: FI_COL_12
    *
    * required 12 byte field
    *
    * xxxxxxxx.yyy
    * 
    * Grid point (1,2) column number in full image coordinate system.
    */
   char theFiCol12[FI_COL_12_SIZE+1];
   
   /**
    * FIELD: FI_ROW_21
    *
    * required 12 byte field
    *
    * xxxxxxxx.yyy
    * 
    * Grid point (2,1) row number in full image coordinate system.
    */
   char theFiRow21[FI_ROW_21_SIZE+1];

   /**
    * FIELD: FI_COL_21
    *
    * required 12 byte field
    *
    * xxxxxxxx.yyy
    * 
    * Grid point (2,1) column number in full image coordinate system.
    */
   char theFiCol21[FI_COL_21_SIZE+1];
   
   /**
    * FIELD: FI_ROW_22
    *
    * required 12 byte field
    *
    * xxxxxxxx.yyy
    * 
    * Grid point (2,2) row number in full image coordinate system.
    */
   char theFiRow22[FI_ROW_22_SIZE+1];

   /**
    * FIELD: FI_COL_22
    *
    * required 12 byte field
    *
    * xxxxxxxx.yyy
    * 
    * Grid point (2,2) column number in full image coordinate system.
    */
   char theFiCol22[FI_COL_22_SIZE+1];

   /**
    * FIELD:  FI_ROW
    *
    * required 8 byte field
    *
    * 00000000 and 00000002 to 99999999
    * 
    * Full image number of rows.
    */
   char theFullImageRow[FI_ROW_SIZE+1];
   
   /**
    * FIELD:  FI_COL
    *
    * required 8 byte field
    *
    * 00000000 and 00000002 to 99999999
    * 
    * Full image number of cols.
    */
   char theFullImageCol[FI_COL_SIZE+1];

   
   
   























   
                            
   /**
    * FIELD: 
    *
    * required  byte field
    *
    * 01 to 99
    * 
    * 
    */
   /**
    * FIELD: 
    *
    * required  byte field
    *
    * 01 to 99
    * 
    * 
    */
   /**
    * FIELD: 
    *
    * required  byte field
    *
    * 01 to 99
    * 
    * 
    */
   
   
TYPE_DATA   
};

#endif /* End of "#ifndef ossimNitfIchipbTag_HEADER" */
