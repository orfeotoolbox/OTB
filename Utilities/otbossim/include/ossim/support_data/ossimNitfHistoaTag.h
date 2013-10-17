//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description: HISTOA tag class declaration.
// 
//----------------------------------------------------------------------------
// $Id: ossimNitfHistoaTag.h 22013 2012-12-19 17:37:20Z dburken $

#ifndef ossimNitfHistoaTag_HEADER
#define ossimNitfHistoaTag_HEADER 1

#include <ossim/support_data/ossimNitfRegisteredTag.h>
#include <vector>

/**
 * All comments in this document were taken from the online resource found at:
 *
 * http://www.gwg.nga.mil/ntb/baseline/docs/stdi0002/
 *
 * Reference: HISTOA section (currently in Appendix L)
 * 
 * The purpose of the Softcopy History Tagged Record Extension, HISTOA, is to
 * provide a history of the softcopy processing functions that have been applied to NSIF
 * imagery. It is meant to describe previous processing actions and the current state of the
 * imagery that was distributed within the intelligence and imagery user community. To be
 * effective, HISTOA needs to be applied to the NSIF product as early as practical and must
 * be updated each time the image is processed and saved by a softcopy processing system.
 * This will allow the user to know with confidence the complete history of the imagery.
 * HISTOA may be created as the NSIF image is created, or when the imagery is first
 * modified.
 */
class OSSIM_DLL ossimNitfHistoaProcessingEvent
{
public:
   enum
   {
      PDATE_SIZE     = 14,
      PSITE_SIZE     = 10,
      PAS_SIZE       = 10,
      NIPCOM_SIZE    = 1,
      IPCOM_SIZE     = 80,
      IBPP_SIZE      = 2,
      IPVTYPE_SIZE   = 3,
      INBWC_SIZE     = 10,
      DISP_FLAG_SIZE = 1,
      ROT_FLAG_SIZE  = 1,
      ROT_ANGLE_SIZE = 8,
      ASYM_FLAG_SIZE = 1,
      ZOOMROW_SIZE   = 7,
      ZOOMCOL_SIZE   = 7,
      PROJ_FLAG_SIZE = 1,
      SHARP_FLAG_SIZE= 1,
      SHARPFAM_SIZE  = 2,
      SHARPMEM_SIZE  = 2,
      MAG_FLAG_SIZE  = 1,
      MAG_LEVEL_SIZE = 7,
      DRA_FLAG_SIZE  = 1,
      DRA_MULT_SIZE  = 7,
      DRA_SUB_SIZE   = 5,
      TTC_FLAG_SIZE  = 1,
      TTCFAM_SIZE    = 2,
      TTCMEM_SIZE    = 2,
      DEVLUT_FLAG_SIZE = 1,
      OBPP_SIZE      = 2,
      OPVTYPE_SIZE   = 3,
      OUTBWC_SIZE    = 10
   };
   ossimNitfHistoaProcessingEvent();
   ossim_uint32 getSizeInBytes()const;
   void parseStream(std::istream& in);
   void writeStream(std::ostream& out);

   std::ostream& print(std::ostream& out,
                       const std::string& prefix=std::string()) const;
   void clearFields();
   
private:
   
   /**
    * Field: PDATE
    *
    * This field shall contain the date and time (UTC) on which the processing event occurred. The
    * valid form of the field is CCYYMMDDhhmmss, where CC is the first two digits of the year (00
    * to 99), YY is the last two digits of the year (00 to 99), MM is the month (01 to 12), DD is
    * the day of the month (01 to 31), hh is the hour (00 to 23), mm is the minute (00 to 59), and
    * ss is the second (00 to 59). UTC (Zulu) is assumed to be the time zone designator to express
    * the time of day. This field can be used in conjunction with the FDT field in the NSIF file
    * header to determine if the History Tag has been updated each time the image was processed and
    * saved. If the PDATE field and the FDT field are identical, the History Tag has been properly
    * updated. If the fields are not identical, then the History Tag has not been properly updated
    * and the data may not be accurate or timely.
    */
   char m_pdate[PDATE_SIZE+1];

   /**
    * Field: PSITE
    *
    * This field shall contain the name of the site or segment that performed the processing event.
    * This 10 character alphanumeric field is free form text. Examples of PSITE entries are FOS,
    * JWAC, or CENTCOM.
    */
   char m_psite[PSITE_SIZE+1];
   
   /**
    * Field: PAS
    *
    * This field shall contain the processing application software used to perform the processing
    * steps cited in the event (e.g. IDEX, VITEC, or DIEPS). The version number of the application
    * would also be helpful to include in this field.
    */
   char m_pas[PAS_SIZE+1];
   
   /**
    * Field: NIPCOM
    *
    * This field shall contain the valid number of image processing comments for this processing
    * event. The valid field codes are 0 to 9.
    */
   char m_nIpcom[NIPCOM_SIZE+1];
   
   /**
    * Field IPCOM
    *
    * This field shall contain the first line of comment text. The fields IPCOM1 to IPCOMn, if
    * present shall contain free form alphanumeric text. They are intended for use as a single
    * comment block and shall be used that way. This field shall be omitted if the value in NIPCOM
    * field is zero. The comment field shall be used to clarify or indicate special processing not
    * accounted for in the Processing Event Fields. Reasons for populating this field would be to
    * vindicate alternate processing for multi-spectral imagery, to indicate the order of S/C
    * processing steps contained within a single processing event, or to inform downstream users of
    * potential problems with the image.
    */
   std::vector<std::string> m_comment;
   
   /**
    * Field: IBPP
    *
    * This field shall contain the number of significant bits for each pixel before the processing
    * functions denoted in the processing event have been performed and before compression. This
    * type of pixel depth description is consistent with the ABPP field within the NSIF image
    * subheader. For example, if an 11-bpp word is stored in 16 bits, this field would contain 11 and
    * the NBPP field in the NSIF image subheader would contain 16. The valid IBPP field codes are
    * 01 to 64, indicating 1 to 64 bpp.
    */
   char m_ibpp[IBPP_SIZE+1];
   
   /**
    * Field IPVTYPE
    *
    * This field shall contain an indicator of the type of computer representation used for the value
    * of each pixel before the processing functions denoted in the processing events have been
    * performed and before compression. Valid entries are INT for integer, SI for 2’s complement
    * signed integer, R for real, and C for complex. The databits of INT and SI values shall appear in
    * the file in order of significance, beginning with the most significant bit (MSB) and ending with
    * the least significant bit (LSB). INT and SI data types shall be limited to 16 bits. R values
    * shall be represented according to IEEE 32-bit floating-point representation (IEEE754). C values
    * shall be represented with the Real and Imaginary parts each represented in IEEE 32-bit floating
    * point representation (IEEE754) and appearing adjacent four-byte blocks, first Real, then
    * Imaginary. B (bi-level) pixel values shall be represented as single bits with value 1 or 0.
    */
   char m_ipvType[IPVTYPE_SIZE+1];
   
   /**
    * Field INBWC
    *
    * This field shall indicate the type of bandwidth compression or expansion that has been applied
    * to the image prior to any enhancements denoted in the processing event. The valid field codes to
    * describe each type of compression are 5 byte character strings. The first two characters
    * indicate the type of compression such as DCT or DPCM. The next two characters indicate either
    * the bit rate or the quality level. The last character indicates if the process is compression
    * or an expansion. Compression is denoted by a C, an E denotes expansion, and 0 indicates that
    * neither process occurred. The types of compression are indicated by the following codes:
    * Value Definition
    * DP43 DPCM (Differential Pulse Coded Modulation) – 4.3 bpp
    * DC13 DCT (Discrete Cosine Transform – 2.3 bpp
    * DC23 DCT (Discrete Cosine Transform) – 2.3 bpp
    * NJNL NSIF JPEG – Lossless
    * NJQ0 NSIF JPEG – Quality Level 0
    * NJQ1 NSIF JPEG – Quality Level 1
    * NJQ2 NSIF JPEG – Quality Level 2
    * C11D NSIF Bi-level – 1D
    * C12S NSIF Bi-level – 2DS
    * C12H NSIF Bi-level – 2DH
    * M11D NSIF Bi-level – 1D
    * M12S NSIF Bi-level with masked blocks – 2DS
    * M12H NSIF Bi-level with masked blocks – 2DH
    * C207 NITF ARIDPCM – 0.75 bpp
    * C214 NITF ARIDPCM – 1.40 bpp
    * C223 NITF ARIDPCM – 2.30 bpp
    * C245 NITF ARIDPCM – 4.50 bpp
    * C3Q0 NSIF Lossy JPEG – Q0 Custom Tables
    * C3Q1 NSIF Lossy JPEG – Q1 Default Tables
    * C3Q2 NSIF Lossy JPEG – Q2 Default Tables
    * C3Q3 NSIF Lossy JPEG – Q2 Default Tables
    * C3Q4 NSIF Lossy JPEG – Q4 Default Tables
    * C3Q5 NSIF Lossy JPEG – Q5 Default Tables
    * M3Q0 NSIF Lossy JPEG with masked blocks – Q0 Custom
    * M3Q1 NSIF Lossy JPEG with masked blocks – Q1 Default
    * M3Q2 NSIF Lossy JPEG with masked blocks – Q2 Default
    * M3Q3 NSIF Lossy JPEG with masked blocks – Q3 Default
    * M3Q4 NSIF Lossy JPEG with masked blocks – Q4 Default
    * M3Q5 NSIF Lossy JPEG with masked blocks – Q5 Default
    * C4LO NSIF Vector Quantization – Lossy
    * M4LO NSIF Vector Quantization with masked blocks
    * C5NL NSIF Lossless JPEG
    * M5NL NSIF Lossless JPEG with masked blocks
    * NC00 NSIF uncompressed
    * NM00 NSIF with masked blocks uncompressed
    * I1Q1 NSIF Downsample JPEG – Q1
    * I1Q2 NSIF Downsample JPEG – Q2
    * I1Q3 NSIF Downsample JPEG – Q3
    * I1Q4 NSIF Downsample JPEG – Q4
    * I1Q5 NSIF Downsample JPEG – Q5
    * WVLO Wavelet Lossy
    * WVNL Wavelet Lossless
    * JP20 JPEG 2000
    * NONE No Compression
    * UNKC Unknown Compression
    * OTLO Unknown lossy compression – requires mandatory IPCOM entry to explain
    * technique or source
    * OTNL Unknown lossless compression – requires mandatory IPCOM entry to explain
    * technique or source
    * The entire BWC field is 10 bytes long to allow for the concatenation of up to 2 compression
    * algorithms. Two consecutive 5 byte character strings shall indicate the application of two
    * compression algorithms in succession. If only one operation is performed, then the remaining 5
    * characters are zero. Examples of valid codes for the BWC field are shown below.
    * The DP43E00000 code indicates that a 4.3 DPCM compressed input image was expanded prior
    * to NSIF formation.
    * The DC13E00000 code indicates that 1.3 DCT compressed input image was expanded prior to
    * NSIF formation.
    * The NONE000000 code indicates that the input image to the NSIF formation process was
    * uncompressed.
    */
   char m_inbwc[INBWC_SIZE+1];
   
   /**
    * Field DISPLAY_FLAG
    *
    * This field shall indicate if the image is “Display-Ready.” The DISP_FLAG field applies only to
    * System B, System D, and certain other systems. Display-Ready data has had a system-specific
    * transformation applied to it that is described in appendix A. The valid field codes are 0 to 9
    * and a blank (BCS 0x20). A value of 0 means that image is not Display-Ready and must be
    * converted to a displayable format, using the pre-defined mappings for Linlog or PEDF formats.
    * A value of 1 means that the image is Display-Ready and needs only basic tonal processing and
    * device compensation for corrects display. Since this field applies to Systems B and D imagery
    * currently, the field shall be filled with a blank (BCS 0x20) for all other system types. Values
    * 2 to 9 are reserved for future use and shall not be used at this time. A more detailed
    * explanation of the Display-Ready transformations is provided in appendix A.
    */
   char m_dispFlag[DISP_FLAG_SIZE + 1];
   
   /**
    * Field ROT_FLAG
    *
    * This field shall indicate if the image has been rotated. The valid field codes are 0 and 1. A
    * value or 0 means that the image has not been rotated. A value of 1 means that the image has been
    * rotated. If this field is equal to 1, then the ROT_ANGLE field must be filled with the angle of
    * rotation.
    */
   char m_rotFlag[ROT_FLAG_SIZE + 1];
   
   /**
    * Field: ROT_ANGLE
    *
    * This field shall contain the angel in degrees that the image has been rotated, where a positive
    * angle denotes clockwise rotation. The valid field codes are 000.0000 to 359.9999. This field is
    * conditional on the ROT_FLAG field being equal to 1. If the rotation has included an
    * interpolation, then the interpolation method shall be described in the comment sections.
    */
   char m_rotAngle[ROT_ANGLE_SIZE + 1];
   
   /**
    * Field: ASYM_FLAG
    *
    * This field shall indicate if asymmetric correction has been applied to the image. This
    * processing step is only allowed for certain types of EO processing. The valid field codes are
    * 0 and 1, and a blank (BCS 0x20). A value of 0 means that asymmetric correction has not been
    * applied to the image. A value of 1 means that asymmetric correction has been applied to the
    * image. Since this field applies only to certain types of EO imagery, this field shall be filled
    * with a blank (BCS 0x20) for all other system types. If this field is equal to 1, the ZOOMROW
    * and ZOOMCOL fields must be filled with the magnification levels in the row (line) and column
    * (element) directions, respectively.
    */
   char m_asymFlag[ASYM_FLAG_SIZE + 1];
   
   /**
    * Field: ZOOMROW
    *
    * This field shall contain the level of magnification that was applied to the image in the line or
    * row direction, if asymmetric correction was applied. The valid field codes are 00.0000 to
    * 99.9999. The level of magnification is relative to the input image at this processing step. This
    * field is conditional on the ASYM_FLAG field
    */
   char m_zoomRow[ZOOMROW_SIZE + 1];
   
   /**
    * Field: ZOOMCOL
    *
    * This field shall contain the level of magnification that was applied to the image in the element
    * or column direction, if asymmetric correction was applied. The valid field codes are 00.0000 to
    * 99.9999. The level of magnification is relative to the input image at this processing step. This
    * field is conditional on the ASYM_FLAG field.
    */
   char m_zoomCol[ZOOMCOL_SIZE + 1];

   /**
    * Field: PROJ_FLAG
    *
    * This field shall indicate if the image has been projected from the collection geometry into
    * geometry more suitable for display. The valid field codes are 0 and 1. A value of 0 means that
    * no geometric transformation has been applied to the image, meaning it is probably stilled in the
    * collection geometry. A value of 1 means that the image has been projected into another
    * geometry. If this field is equal to 1, then a description of the projection or rectification
    * shall be given in the comment section.
    */
   char m_projFlag[PROJ_FLAG_SIZE + 1];
   
   /**
    * Field: SHARP_FLAG
    *
    * This field shall indicate if the image has been passed through a sharpening operation. The valid
    * field codes are 0 and 1. A value of 0 means that no sharpening has been applied to the image. A
    * value of 1 means that sharpening has been applied to the image. If this field is equal to 1,
    * then the SHARPFAM and SHARPMEN fields must be filled with the appropriate numbers. Refer to
    * paragraph 15.5 for a more complete description of the sharpening kernel database.
    */ 
   char m_sharpFlag[SHARP_FLAG_SIZE+1];
   
   /**
    * Field: SHARPFAM
    *
    * This field shall contain the number of the sharpening family, if a sharpening operation was
    * applied to the image. The valid field codes are –1, 00 to 99. This field is conditional on the
    * SHARP_FLAG field. Although the IDEX sharpening family numbers are one-based, many
    * commercial softcopy systems use a zero-based system for their databases. For example, IDEX
    * family 5 would be family 4 for many other softcopy systems. If the sharpening kernel is not a
    * part of the existing group of families and members, a value of – shall be placed in this field
    * and the nature of the sharpening kernel specified in the comment section. Refer to paragraph
    * 15-5 for a more complete description of the sharpening kernel database.
    */
   char m_sharpFam[SHARPFAM_SIZE+1];
   
   /**
    * Field: SHARPMEM
    *
    * This field shall contain the number of the sharpening member, if a sharpening operation was
    * applied to the image. The valid field codes are –1, 00 to 99. This field is conditional on the
    * SHARP_FLAG field. If the sharpening kernel is not a part of the existing group of families and
    * members, a value of –1 shall be placed in this field and the nature of the sharpening kernel
    * shall be specified in the comment section. Refer to 15.5 for a more complete description of the
    * sharpening database.
    */
   char m_sharpMem[SHARPMEM_SIZE+1];
   
   /**
    * Field MAG_FLAG
    *
    * This field shall indicate if the image has been symmetrically (same amount in each direction)
    * magnified during this processing step. The valid field codes are 0 and 1. A value of 0 means
    * that the image was not magnified. A value of 1 means that the image has been magnified. If this
    * field is equal to 1, then the MAG_LEVEL field shall be filled with the level of magnification.
    */
   char m_magFlag[MAG_FLAG_SIZE+1];
   
   /**
    * Field MAG_LEVEL
    *
    * This field shall contain the level of symmetrical magnification that has been applied to the
    * image relative to the input image at this processing step. For example, a value of 02.0000 would
    * indicate a 2X magnification relative to the input image. The valid field codes are 00.0000 to
    * 99.9999. This field is conditional on the MAG_FLAG field. A value greater than 1 shall
    * indicate that the image was magnified to a size larger than its previous size and a value less
    * than 1 shall indicate the image size was decreased. The method of magnification shall be
    * described in the comment section.
    */
   char m_magLevel[MAG_LEVEL_SIZE +1];
   
   /**
    * Field: DRA_FLAG
    *
    * This field shall indicate if a dynamic Range Adjustment (DRA) has been applied to the image.
    * DRA is an affine transformation of the image pixel values of the form Y = DRA_MULT*(X –
    * DRA_SUB), where X is the input pixel value, DRA_SUB is the DRA subtractor, DRA_MULT
    * is the DRA multiplier, and Y is the output pixel value. The DRA is said to be spatially
    * invariant when the DRA subtractor and DRA multiplier do not depend on pixel position. If the DRA
    * subtractor and DRA multiplier do depend on pixel position, the DRA is said to be spatially
    * variant. The valid field codes are 0, 1, and 2. A value of 0 means that a DRA has not been
    * applied to the image. A value of 1 means that a spatially invariant DRA has been applied to the
    * image. In this case, the DRA_SUB and DRA_MULT fields shall be filled with the appropriate
    * codes. A value of 2 means that a spatially variant DRA has been applied to the image. In cases
    * where DRA_FLAG equals 0 or 2, the DRA_SUB and DRA_MULT fields shall not be filled.
    */
   char m_draFlag[DRA_FLAG_SIZE+1];
   
   /**
    * Field: DRA_MULT
    *
    * This field shall contain the multiplier value of the DRA. The valid field codes are 000.000 to
    * 999.999. This field is conditional on the DRA_FLAG field being equal to 1.
    */
   char m_draMult[DRA_MULT_SIZE+1];
   
   /**
    * Field: DRA_SUB
    *
    * This field shall contain the subtractor value of the DRA. The valid field codes are 000.000 to
    * 999.999. This field is conditional on the DRA_FLAG field being equal to 1.
    */
   char m_draSub[DRA_SUB_SIZE+1];
   
   /**
    * Field: TTC_FLAG
    *
    * This field shall indicate if a TTC (Tonal Transfer Curve) has been applied to the image. The
    * valid field codes are 0 and 1. A value of 0 means that a TTC has not been applied to the image.
    * A value of 1 means that a TTC has been applied to the image. If a TTC has been applied, then
    * the TTCFAM and TTCNUM fields shall be filled with the appropriate codes. Refer to
    * paragraph 15-5 for more complete description of the TTC database.
    */
   char m_ttcFlag[TTC_FLAG_SIZE+1];
   
   /**
    * Field: TTCFAM
    *
    * This field shall contain the number of the TTC family, if a TTC was applied to the image. The
    * valid field codes are –1, 00 to 99. This field is conditional on the TTC_FLAG field. Although
    * the IDEXZ TTC family numbers are one-based, many commercial softcopy systems use a zerobased
    * system for their databases. For example, IDEX family 5 would be family 4 for many
    * other softcopy systems. If the TTC is not a part of the existing group of families and members,
    * a value of –1 shall be placed in this field and the nature of the TTC shall be specified in the
    * comment section. Refer to paragraph 15-5 for a more complete description of the TTC database.
    */
   char m_ttcFam[TTCFAM_SIZE+1];
   
   /**
    * Field: TTCMEM
    *
    * This field shall contain the number of the TTC member, if a TTC was applied to the image. The
    * valid field codes are 00 to 99. This field is conditional on the TTC_FLAG field. If the TTC is
    * not a part of the existing group of families and members, a value of –1 shall be placed in this
    * field and the nature of the TTC shall be specified in the comment section. Refer to paragraph
    * 15-5 for a more complete description of the TTC database.
    */
   char m_ttcMem[TTCMEM_SIZE+1];
   
   /**
    * Field: DEVLUT_FLAG
    *
    * This field shall indicate if device compensation LUT has been applied to the image. The valid
    * field codes are 0 and 1. A value of 0 means that a device LUT has not been applied to the
    * image. A value of 1 means that t device LUT has been applied to the image. The nature of the
    * LUT may be specified in the comment section and should include the device for which the LUT
    * is applied. If the device is not known, an appropriate method for describing the LUT shall be
    * given.
    */
   char m_devLutFlag[DEVLUT_FLAG_SIZE+1];
   
   /**
    * Field OBPP
    *
    * This field shall contain the number of significant bits for each pixel after the processing
    * functions denoted in the processing event have been performed, but prior to any output
    * compression. For example, if an 8 bpp System B image is mapped into Display-Ready space
    * using the proper 8 to 11 bpp transformation (see appendix A), the IBPP field shall contain the
    * actual number of data pixels, not the word length. For example, if an 11-bpp word were stored
    * in 16 bits, this field would contain 11. The valid OBPP field codes are 01 to 64, indicating 1
    * to 64 bpp. In many cases, this field will match the IBPP field.
    */
   char m_obpp[OBPP_SIZE+1];
   
   /**
    * Field: OPVTYPE
    *
    * This field shall contain an indicator of the type of computer representation used for the value
    * of each pixel after the processing functions denoted in the processing event have been
    * performed, but prior to any output compression. Valid entries are INT for integer, B for
    * bi-level, SI for 2’s complement signed integer, R for real, and C for complex. The data bits of
    * INT and SI values shall appear in the file in order of significance, beginning with the MSB and
    * ending with the LSB. INT and SI data types shall be limited to 16 bits. R values shall be
    * represented according to IEEE 32-bit floating-point representation (IEEE754). C values shall be
    * represented with the Real and Imaginary parts each 32-bit floating point representation
    * (IEEE754) and appearing adjacent four-byte blocks, first Real, then Imaginary. B (bi-level)
    * pixel values shall be represented as single bits with value 1 or 0.
    */
   char m_opvType[OPVTYPE_SIZE+1];
   
   /**
    * Field: OUTBWC
    *
    * This field shall indicate the type of bandwidth compression or expansion that has been applied
    * to the image after any enhancements denoted in the processing event. The valid field codes to
    * describe each type of compression are 5 byte character strings. The first two characters
    * indicate the type of compression such as DCT or DPCM. The next two characters indicate either
    * the bit rate or the quality level. The last character indicates if the process is compression
    * or an expansion. Compression is denoted by a C, an E denotes expansion, and 0 indicates that
    * neither process occurred. The types of compression are indicated by the same codes used in the
    * INBWC field and can be found in the field description for INBWC.
    * The entire BWC field is 10 bytes long to allow for the concatenation of up to 2 compression
    * algorithms. Two consecutive 5 byte character strings shall indicate the application of two
    * compression algorithms in succession. If only one operation is performed, then the remaining 5
    * characters are zero. Examples of valid codes for the BWC field are shown below.
    * The NJQ1C00000 code indicates that the processed image was saved as a NSIF JPEG lossless
    * compressed image.
    * The NJNLC00000 indicates that the processed image was saved as a NSIF JPEG lossless
    * compressed image.
    * The C3Q3C00000 code indicates that the processed image was saved as a NSIF JPEG
    * compressed image at quality level 3.
    */
   char m_outBwc[OUTBWC_SIZE+1];
   
}; // Matches: class OSSIM_DLL ossimNitfHistoaProcessingEvent

class OSSIM_DLL ossimNitfHistoaTag : public ossimNitfRegisteredTag
{
public:
   enum
   {
      SYSTYPE_SIZE    = 20,
      PC_SIZE         = 12,
      PE_SIZE         = 4,
      REMAP_FLAG_SIZE = 1,
      LUTID_SIZE      = 2,
      NEVENTS_SIZE    = 2
   };
   
   ossimNitfHistoaTag();
   
   /**
    * 
    * This will allow the user defined data to parse the stream.
    * 
    */
   virtual void parseStream(std::istream& in);
   virtual void writeStream(std::ostream& out);
   
   virtual ossim_uint32 getSizeInBytes()const;
   
//   virtual void setProperty(ossimRefPtr<ossimProperty> property);
//   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
//   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   
   /**
    * @brief Print method that outputs a key/value type format
    * adding prefix to keys.
    * @param out Stream to output to.
    * @param prefix Prefix added to key like "image0.";
    */
   virtual std::ostream& print(std::ostream& out,
                               const std::string& prefix=std::string()) const;
   virtual void clearFields();
protected:
   /**
    * Field: SYSTYPE
    *
    * This field shall contain the name of the sensor from which the original image was collected. For
    * national imagery, the valid field codes are SystemA, SystemB, SystemC, and SystemD. These
    * codes shall not be used to indicate any other airborne or commercial systems and are reserved
    * solely for the National systems. The codes in the SYSTYPE field shall be left justified and the
    * remainder of the field filled with blanks to 20 characters. The NTB has requested that this tag be
    * able to handle other types of airborne and commercial imagery currently supported by NITF.
    *  Additional valid field codes are listed below:
    * ASARS-2 ASARS System
    * GHR Global Hawk Radar
    * SYERS-EO SYERS Electro-Optical System
    * SYERS-MSI SYERS Multispectral System
    * SYERS-IR SYERS Infrared System
    * DSR Dark Star Radar
    * TSAR TESAR
    * TBD Other
    */
   char m_systype[SYSTYPE_SIZE+1];
   
   /**
    * Field: PC
    *
    * This field shall contain an alphanumeric string that indicates if bandwidth
    * compression/expansion was applied to the image prior to NITF image creation. This field
    * should be used in conjunction with the PE field to determine the state of the image prior to
    * NITF formation. The valid field codes for the PC field is 4 byte character strings. The first two
    * characters indicate the type of compression such as DCT or DPCM. The next two characters
    * indicate either the bit rate or the quality level. The types of compression are indicated by the
    * following codes:
    * Value Definition
    * DP43 DPCM (Differential Pulse Coded Modulation) – 4.3 bpp
    * DC13 DCT (Discrete Cosine Transform –1.3 bpp
    * DC23 DCT (Discrete Cosine Transform) – 2.3 bpp
    * NJNL NSIF JPEG – Lossless
    * NJQ0 NSIF JPEG – Quality Level 0
    * NJQ1 NSIF JPEG – Quality Level 1
    * NJQ2 NSIF JPEG – Quality Level 2
    * C11D NSIF Bi-level – 1D
    * C12S NSIF Bi-level – 2DS
    * C12H NSIF Bi-level – 2DH
    * M11D NSIF Bi-level – 1D
    * M12S NSIF Bi-level with masked blocks – 2DS
    * M12H NSIF Bi-level with masked blocks – 2DH
    * C207 NITF ARIDPCM – 0.75 bpp
    * C214 NITF ARIDPCM – 1.40 bpp
    * C223 NITF ARIDPCM – 2.30 bpp
    * C245 NITF ARIDPCM – 4.50 bpp
    * C3Q0 NSIF Lossy JPEG – Q0 Custom Tables
    * C3Q1 NSIF Lossy JPEG – Q1 Default Tables
    * C3Q2 NSIF Lossy JPEG – Q2 Default Tables
    * C3Q3 NSIF Lossy JPEG – Q2 Default Tables
    * C3Q4 NSIF Lossy JPEG – Q4 Default Tables
    * C3Q5 NSIF Lossy JPEG – Q5 Default Tables
    * M3Q0 NSIF Lossy JPEG with masked blocks – Q0 Custom
    * M3Q1 NSIF Lossy JPEG with masked blocks – Q1 Default
    * M3Q2 NSIF Lossy JPEG with masked blocks – Q2 Default
    * M3Q3 NSIF Lossy JPEG with masked blocks – Q3 Default
    * M3Q4 NSIF Lossy JPEG with masked blocks – Q4 Default
    * M3Q5 NSIF Lossy JPEG with masked blocks – Q5 Default
    * C4LO NSIF Vector Quantization – Lossy
    * M4LO NSIF Vector Quantization with masked blocks
    * C5NL NSIF Lossless JPEG
    * M5NL NSIF Lossless JPEG with masked blocks
    * NC00 NSIF uncompressed
    * NM00 NSIF with masked blocks uncompressed
    * I1Q1 NSIF Downsample JPEG – Q1
    * I1Q2 NSIF Downsample JPEG – Q2
    * I1Q3 NSIF Downsample JPEG – Q3
    * I1Q4 NSIF Downsample JPEG – Q4
    * I1Q5 NSIF Downsample JPEG – Q5
    * WVLO Wavelet Lossy
    * WVNL Wavelet Lossless
    * JP20 JPEG 2000
    * NONE No Compression
    * UNKC Unknown Compression
    * The entire PC field is 12 bytes long to allow for the concatenation of up to 3 compression
    * algorithms. Consecutive 4 byte character strings shall indicate the application of two or three
    * compression algorithms in succession. If only one compression algorithm is applied then the
    * last eight characters are zero. If the NSIF creator does not know where the image came from or
    * what processing has been applied to it, then the code for unknown compression (UNKC) shall
    * be used.
    * Examples of valid codes for the PC field are shown below.
    * The DP43DC130000 code indicates that a concatenation of the 4.3 DPCM and the 1.3 DCT
    * compression and expansion was applied to the image prior to its NSIF formation.
    * The NONE00000000 code indicates that no compression was applied to the image prior to its
    * NSIF formation.
    */
   char m_pc[PC_SIZE+1];
   
   /**
    * Field: PE
    *
    * This field shall contain an alphanumeric string that indicates if any enhancements were applied
    * to the image prior to NSIF image creation. This field should be used in conjunction with the PC
    * field to determine the state of the image prior to NSIF formation. The valid field codes for the
    * PC field are given below
    * EH08 Enhanced 8 bpp from IDEX
    * EH11 Enhanced 11 bpp from IDEX
    * UE08 8 bpp data with DRA but no enhancements from IDEX
    * EU11 Unenhanced 22 bpp from IDEX
    * DGHC Digitized Hardcopy
    * UNKP Unknown Processing
    * NONE No prior processing
    * The first four codes explicitly define the types of ODS (Output Data Server) products that are
    * available for NSIF formation. Additional codes may be added for airborne systems. If the NSIF
    * creator does not know where the image came from or what processing has been applied to it,
    * then the code for unknown processing (UNKP) shall be used.
    */
   char m_pe[PE_SIZE+1];
   
   /**
    * Field: REMAP_FLAG
    *
    * This field shall indicate whether or not a system specific remap has been applied to the image.
    * The valid field codes are 0 – 9, and a blank (BCS 0x20), but 2 – 9 are reserved for future use. A
    * value of 0 means that no systems specific remap has been applied. A value of 1 means that the
    * System C specific 16 – 12 bit remap has been applied to the System C image. If the image is not
    * a System C image, this field does not apply at this time and should be filled with a blank.
    * Values from 2 – 9 are reserved for future use and shall not be used at this time.
    */
   char m_remapFlag[REMAP_FLAG_SIZE+1];
   
   /**
    * Field: LUTID
    *
    * This field shall contain the DMID (Data Mapping ID) for Systems B and D imagery. The
    * DMID is contained in IMDAT records 97 and 98 in the ESD (Exploitation Support Data). This
    * information is also referenced in IF200EAA. The valid field codes are 07, 08, and 12 – 64. A
    * value of 07 and 08 indicates that the image is PEDF (Piecewise Extended Density Format). A
    * value between 12 and 64 indicates that the image is a Linlog formatted image. Numbers
    * between 01 and 06, 09, 10, and 11 are reserved and should not be used at this time. There are no
    * valid DMID values greater than 64. NSIF users to help determine what type of processing
    * should be applied to the image can use this field. For all other systems, this field should be filled
    * with 00.
    */
   char m_lutid[LUTID_SIZE+1];
   
   /**
    * Field: NEVENTS
    *
    * This field shall contain the number of processing events associated with the image. The tag is
    * designed to record up to 99 separate processing events. The valid field codes are 01 to 99. The
    * processing events are listed in chronological order, starting with the first event and ending with
    * the most recent processing event. At a minimum, the first processing event shall be the
    * processing immediately following the generation of the NSIF formatted image; however, if
    * practical, the originator of the NSIF image can create the HISTOA TRE earlier - with the
    * creation of the NSIF formatted image. In that instance, the first processing event would be the
    * creation of the NSIF formatted image. Each successive processing event is to record what
    * transformations have been applied to the image, once the image has been processed and saved.
    */
   char m_nEvents[NEVENTS_SIZE+1];
   
   std::vector<ossimNitfHistoaProcessingEvent> m_eventList;
   
}; // Matches: class OSSIM_DLL ossimNitfHistoaTag

#endif /* #ifndef ossimNitfHistoaTag_HEADER */
