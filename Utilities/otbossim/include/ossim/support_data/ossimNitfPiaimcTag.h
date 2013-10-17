//----------------------------------------------------------------------------
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: PIAIMC tag class declaration.
// 
// "Profile for Imagery Access Image Support Extensions"
//
// See document STDI-0002 Table 6-1 for more info.
// 
// http://164.214.2.51/ntb/baseline/docs/stdi0002/final.pdf
//
//----------------------------------------------------------------------------
// $Id: ossimNitfPiaimcTag.h 22013 2012-12-19 17:37:20Z dburken $
#ifndef ossimNitfPiaimcTag_HEADER
#define ossimNitfPiaimcTag_HEADER

#include <ossim/support_data/ossimNitfRegisteredTag.h>
#include <ossim/base/ossimConstants.h>

class ossimString;

class OSSIM_DLL ossimNitfPiaimcTag : public ossimNitfRegisteredTag
{
public:
   enum 
   {
      CLOUDCVR_SIZE   =   3,
      SRP_SIZE        =   1,
      SENSMODE_SIZE   =  12,
      SENSNAME_SIZE   =  18,
      SOURCE_SIZE     = 255,
      COMGEN_SIZE     =   2,
      SUBQUAL_SIZE    =   1,
      PIAMSNNUM_SIZE  =   7,
      CAMSPECS_SIZE   =  32,
      PROJID_SIZE     =   2,
      GENERATION_SIZE =   1,
      ESD_SIZE        =   1,
      OTHERCOND_SIZE  =   2,
      MEANGSD_SIZE    =   7,
      IDATUM_SIZE     =   3,
      IELLIP_SIZE     =   3,
      PREPROC_SIZE    =   2,
      IPROJ_SIZE      =   2,
      SATTRACK_SIZE   =   8
      //             -------
      //                362
   };
   
   /** default constructor */
   ossimNitfPiaimcTag();
  
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
                               const std::string& prefix) const;
   /**
    * @return CLOUDCVR field as an ossimString.
    */
   ossimString getCloudcvrString() const;

   /**
    * @return SRP field as an ossimString.
    */
   ossimString getSrpString() const;
   
   /**
    * @return SENSMODE field as an ossimString.
    */
   ossimString getSensmodeString() const;
   
   /**
    * @return SENSNAME field as an ossimString.
    */
   ossimString getSensnameString() const;
   
    /**
    * @return SOURCE field as an ossimString.
    */
   ossimString getSourceString() const;
  
   /**
    * @return COMGEN field as an ossimString.
    */
   ossimString getComgenString() const;
   
   /**
    * @return SUBQUAL field as an ossimString.
    */
   ossimString getSubqualString() const;

   /**
    * @return PIAMSNNUM field as an ossimString.
    */
   ossimString getPiamsnnumString() const;

   /**
    * @return field as an ossimString.
    */
   ossimString getString() const;

   /**
    * @return CAMSPECS field as an ossimString.
    */
   ossimString getCamspecsString() const;

   /**
    * @return PROJID field as an ossimString.
    */
   ossimString getProjidString() const;

   /**
    * @return GENERATION field as an ossimString.
    */
   ossimString getGenerationString() const;

   /**
    * @return ESD field as an ossimString.
    */
   ossimString getEsdString() const;
   
   /**
    * @return OTHERCONDfield as an ossimString.
    */
   ossimString getOthercondString() const;

   /**
    * @return MEANGSD field as an ossimString.
    */
   ossimString getMeadGsdString() const;

   /**
    * @return MEANGSD field as a double converted to meters.
    *
    * @note Conversion used: gsd_in_meters = gsd / 12.0 * MTRS_PER_FT
    */
   ossim_float64 getMeanGsdInMeters() const;

   /**
    * @return IDATUM field as an ossimString.
    */
   ossimString getIdatumString() const;
   
   /**
    * @return IELLIP field as an ossimString.
    */
   ossimString getIellipString() const;
   
   /**
    * @return PREPROC field as an ossimString.
    */
   ossimString getPreprocString() const;
   
   /**
    * @return IPROJ field as an ossimString.
    */
   ossimString getIprojString() const;
   
   /**
    * @return SATTRACK field as an ossimString.
    */
   ossimString getSattrackString() const;
   
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

protected:

   /**
    * FIELD: CLOUDCVR
    *
    * 3 byte field
    *
    * Indicates the precentage of the image that is obscured by cloud. A value
    * os 999 indicates an unknown condition.
    *
    * 000 to 100, or 999
    */
   char theCloudcvr[CLOUDCVR_SIZE+1];
   
   /**
    * FIELD: SRP
    *
    * 1 byte field
    *
    * Indicates whether or not standard radiometric product data is available.
    *
    * Y,N
    */
   char theSrp[SRP_SIZE+1];
   
   /**
    * FIELD: SENSMODE
    *
    * 12 byte field
    *
    * Indentifies the sensor mode used in capturing the image.
    *
    * WHISKBROOM, PUSHROOM, FRAMING, SPOT, SWATH, TBD
    */
   char thesSensmode[SENSMODE_SIZE+1];
   
   /**
    * FIELD: SENSNAME
    *
    * 18 byte field
    *
    * Indentifies the name of sensor used in capturing the image.
    *
    * USIGS DM, SENSORTYPE Name
    */
   char theSensname[SENSNAME_SIZE+1];
   
   /**
    * FIELD: SOURCE
    *
    * 18 byte field
    *
    * Indicated where the image came from.
    *
    * alphanumeric
    */
   char theSource[SOURCE_SIZE+1];
   
   /**
    * FIELD: COMGEN
    *
    * 2 byte field
    *
    * Compression Generation.
    *
    * 00 to 99
    */
   char theComgen[COMGEN_SIZE+1];
   
   /**
    * FIELD: SUBQUAL
    *
    * 1 byte field
    *
    * Subjective Quality
    *
    * P-Poor, G-Good, E-Excellent, F-Fair
    */
   char theSubqual[SUBQUAL_SIZE+1];
   
   /**
    * FIELD: PIAMSNNUM
    *
    * 7 byte field
    *
    * Indicates the mission number assigned to the reconnaissance mission.
    *
    * EARS 1.1 page 4-28
    */
   char thePiamsnnum[PIAMSNNUM_SIZE+1];
   
   /**
    * FIELD: CAMSPECS
    *
    * 32 byte field
    *
    * Specifies the brand name of the camera used and the focal length of the
    * lens.
    *
    * alphanumeric
    */
   char theCamspecs[CAMSPECS_SIZE+1];
   
   /**
    * FIELD: PROJID
    *
    * 2 byte field
    *
    * Indentifies collection platform project identifier code.
    *
    * EARS Appendix 9
    */
   char theProjid[PROJID_SIZE+1];
   
   /**
    * FIELD: GENERATION
    *
    * 1 byte field
    *
    * Specifies the number of image generations of the product.  The number(0)
    * is reserved for the original product.
    *
    * 0 to 9
    */
   char theGeneration[GENERATION_SIZE+1];

   /**
    * FIELD: ESD
    *
    * 1 byte field
    *
    * Indicates whether or not Exploitation Support Data is available and
    * contained withing the product data.
    *
    * Y,N
    */
   char theEsd[ESD_SIZE+1];

   /**
    * FIELD: OTHERCOND
    *
    * 2 byte field
    *
    * Indicates other conditions that affect the imagery over the target.
    *
    * EARS 1.1 page 4 to 28
    */
   char theOthercond[OTHERCOND_SIZE+1];

   /**
    * FIELD: MEANGSD
    *
    * 7 byte field
    *
    * The geometric mean of the across and along scan center-to-center distance
    * between contiguous ground samples.
    *
    * 00000.0 to 99999.9 Expressed in inches, accuracy=10%
    */
   char theMeanGsd[MEANGSD_SIZE+1];

   /**
    * FIELD: IDATUM
    *
    * 3 byte field
    *
    * Identifies the mathematical representation of the earth used to
    * geo-correct/or to rectify the image.
    * (Identifies the Datum assiciated with IGEOLO.)
    *
    * Horizontal_Reference_Datum_Code(refer to DDDS element)
    */
   char theIdatum[IDATUM_SIZE+1];

   /**
    * FIELD: IELLIP
    *
    * 3 byte field
    *
    * Identifies the mathematical representation of the earth used to
    * geo-correct/or to rectify the image.
    * (Identifies the Ellipsoid assiciated with IGEOLO.)
    */
   char theIellip[IELLIP_SIZE+1];
  
   /**
    * FIELD: PREPROC
    *
    * 2 byte field
    *
    * Identifies the level of radiometric and geometric processing applied to
    * the product by the commercial vendor.
    *
    * USIGS DM, IMAGEDATASET Processing Level Code.
    */
   char thePreproc[PREPROC_SIZE+1];
  
   /**
    * FIELD: IPROJ
    *
    * 2 byte field
    *
    * Identifies the 2D-map projection used by commercial vendors to
    * geo-correct/or to rectify the image.
    *
    * DIGEST, Part 3 table 6-1
    */
   char theIproj[IPROJ_SIZE+1];

   /**
    * FIELD: SATTRACK
    *
    * 8 byte field
    *
    * Identifies location of an image acquired by LANDSAT or SPOT (only) along
    * the satellite path.
    *
    * Minimum values:
    *  PATH(J)=0001
    *  ROW(K) =0001
    * Maximum values:
    *  PATH(J)=9999
    *  ROW(K) =9999
    *
    *  Recorded as PATH/ROW 00010001
    */
   char theSattrack[SATTRACK_SIZE+1];
   
TYPE_DATA   
};

#endif /* End of "#ifndef ossimNitfPiaimcTag_HEADER" */
