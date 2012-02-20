#ifndef ossimNitfImageHeaderV2_X_HEADER
#define ossimNitfImageHeaderV2_X_HEADER
#include <ossim/base/ossimConstants.h>
#include <ossim/support_data/ossimNitfImageHeader.h>

class OSSIM_DLL ossimNitfImageHeaderV2_X : public ossimNitfImageHeader
{
public:
   ossimNitfImageHeaderV2_X();

   /** @brief Returns the zero based image rectangle. */
   virtual ossimIrect  getImageRect()const;

   /**
    * @brief Returns the zero based image rectangle expanded out to block boundaries.
    * @note  This may or may not be the same as the rect from getImageRect.
    */
   virtual ossimIrect  getBlockImageRect()const;
   
   void setImageId(const ossimString& value);
   void setAquisitionDateTime(const ossimString& value);
   void setTargetId(const ossimString& value);
   void setTitle(const ossimString& value);
   void setSecurityClassification(const ossimString& value);
   void setEncryption(const ossimString& value);
   void setImageSource(const ossimString& value);
   void setJustification(const ossimString& value);
   void setCoordinateSystem(const ossimString& value);
   void setGeographicLocation(const ossimString& value);
   void setNumberOfComments(const ossimString& value);
   void setCompression(const ossimString& value);
   void setCompressionRateCode(const ossimString& value);
   void setDisplayLevel(const ossimString& value);
   void setAttachmentLevel(const ossimString& value);
   void setPixelType(const ossimString& pixelType = ossimString("INT"));
   void setBitsPerPixel(ossim_uint32 bitsPerPixel);
   void setActualBitsPerPixel(ossim_uint32 bitsPerPixel);
   void setImageMode(char mode);
   void setCategory(const ossimString& category);
   void setRepresentation(const ossimString& rep);
   void setBlocksPerRow(ossim_uint32 blocks);
   void setBlocksPerCol(ossim_uint32 blocks);
   void setNumberOfPixelsPerBlockRow(ossim_uint32 pixels);
   void setNumberOfPixelsPerBlockCol(ossim_uint32 pixels);

   ossim_uint64 getDataLocation()const;
   ossimString getImageId()const;
   ossimString getAquisitionDateTime()const;
   ossimString getTargetId()const;
   ossimString getTitle()const;
   ossimString getSecurityClassification()const;
   ossimString getEncryption()const;
   ossimString getImageSource()const;
   ossimString getJustification()const;
   ossimString getCoordinateSystem()const;
   ossimString getGeographicLocation()const;

   
   ossimString getNumberOfComments()const;
   ossimString getCompressionCode()const;
   ossimString getCompressionRateCode()const;
   ossim_uint32 getDisplayLevel()const;
   ossim_uint32 getAttachmentLevel()const;
   ossimString getPixelType()const;
   ossim_uint32 getBitsPerPixel()const;
   ossim_uint32 getActualBitsPerPixel()const;
   char getImageMode()const;
   ossimString getCategory()const;
   ossimString getRepresentation()const;
   ossim_uint32 getBlocksPerRow()const;
   ossim_uint32 getBlocksPerCol()const;
   ossim_uint32 getNumberOfPixelsPerBlockRow()const;
   ossim_uint32 getNumberOfPixelsPerBlockCol()const;

   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   /**
    * @brief Gets the IMAG field as a string.  Satisfies pure virtual.
    * @return IMAG field
    */
   virtual ossimString  getImageMagnification()const;

   /**
    * @brief Gets the ILOC field as an ossimIpt. Satisfies pure virtual.
    * @param loc Initialized with the field.
    * @note: Do not confuse with "data location".
    */
   virtual void getImageLocation(ossimIpt& loc)const;
   
   virtual void setGeographicLocationDms(const ossimDpt& ul,
                                         const ossimDpt& ur,
                                         const ossimDpt& lr,
                                         const ossimDpt& ll);
   
   virtual void setGeographicLocationDecimalDegrees(const ossimDpt& ul,
                                                    const ossimDpt& ur,
                                                    const ossimDpt& lr,
                                                    const ossimDpt& ll);
   virtual void setUtmNorth(ossim_uint32 zone,
                            const ossimDpt& ul,
                            const ossimDpt& ur,
                            const ossimDpt& lr,
                            const ossimDpt& ll);
   
   virtual void setUtmSouth(ossim_uint32 zone,
                            const ossimDpt& ul,
                            const ossimDpt& ur,
                            const ossimDpt& lr,
                            const ossimDpt& ll);
  
   virtual bool saveState(ossimKeywordlist& kwl, const ossimString& prefix="")const;

   /*!
    * @brief Method to set fields from a keyword list.
    *
    * This is not a true loadState as it does not lookup/initialize all class
    * members.  This was added to allow defaults, e.g ISCLAS, to be set via a
    * site configuration file.
    * Code does not return false if a field(key) is not found..
    *
    * @return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   static const ossimString IM_KW;
   static const ossimString IID1_KW;
   static const ossimString IDATIM_KW;
   static const ossimString TGTID_KW;
   static const ossimString IID2_KW;
   static const ossimString ITITLE_KW; // also IID2 in 2.1 spec and ITITLE in 2.0 spec
   static const ossimString ISCLAS_KW;
   static const ossimString ENCRYP_KW;
   static const ossimString ISORCE_KW;
   static const ossimString NROWS_KW;
   static const ossimString NCOLS_KW;
   static const ossimString PVTYPE_KW;
   static const ossimString IREP_KW;
   static const ossimString ICAT_KW;
   static const ossimString ABPP_KW;
   static const ossimString PJUST_KW;
   static const ossimString ICORDS_KW;
   static const ossimString IGEOLO_KW;
   static const ossimString NICOM_KW;
   static const ossimString ICOM_KW;
   static const ossimString IC_KW;
   static const ossimString COMRAT_KW;
   static const ossimString NBANDS_KW;
   static const ossimString ISYNC_KW;
   static const ossimString IMODE_KW;
   static const ossimString NBPR_KW;
   static const ossimString NBPC_KW;
   static const ossimString NPPBH_KW;
   static const ossimString NPPBV_KW;
   static const ossimString NBPP_KW;
   static const ossimString IDLVL_KW;
   static const ossimString IALVL_KW;
   static const ossimString ILOC_KW;
   static const ossimString IMAG_KW;
   static const ossimString UDIDL_KW;
   static const ossimString UDOFL_KW;
   static const ossimString IXSHDL_KW;
   static const ossimString IXSOFL_KW;
   static const ossimString IMDATOFF_KW;
   static const ossimString BMRLNTH_KW;
   
protected:
   /*!
    * FIELD IM:
    * Is a required 2 byte value and is IM.
    */
   char theType[3];

   /**
    * FIELD: IID1
    * Is a required 10 Alphanumeric value.
    */
   char theImageId[11];

   /*!
    * FIELD IDATIM:
    * Contains the time of acquisition of the image.
    * this is a 14 byte field and has the format:
    *
    * DDHHMMSSZMONYY
    *
    * DD   is the day of the month
    * HH   is the Hour
    * MM   is the minutes
    * SS   is the seconds
    * Z    is fixed (Zulu time)
    * MON  is the first 3 chars of the month
    * YY   is the year
    */
   char theDateTime[15];

   /**
    * FIELD: TGTID
    * 
    * is a 17 byte field.  Has the form
    *
    * BBBBBBBBBBFFFFFCC
    *
    * where:
    *
    * B     is the encyclopedia id
    * F     is the functional category code
    * C     is the country code.
    */
   char theTargetId[18];

   /**
    * FIELD: IID2
    * 
    * Is 80 bytes and contains the title
    */
   char theTitle[81];

   /**
    * FIELD: ISCLAS
    * 
    * Is a required 1 byte field and has value:
    *
    * T    = Top secret
    * S    = Secrect
    * C    = confidential
    * R    = Restricted
    * U    = Unclassified
    */
   char theSecurityClassification[2];

   /*!
    * FIELD ENCRYP:
    * Is arequired 1 byte field.
    *
    * 0  = no encryption
    * 1  = encryption
    */
   char theEncryption[2];

   /**
    * FIELD: ISORCE
    * 
    * Is a 42 byte optional field.
    */
   char theImageSource[43];

   /**
    * FIELD: NROWS
    * 
    * Required 8 byte number 00000002-99999999
    */
   char theSignificantRows[9];

   /**
    * FIELD: NCOLS
    * 
    * Required 8 byte number 00000002-99999999
    */
   char theSignificantCols[9];

   /**
    * FIELD: PVTYPE
    * 
    * Required 3 byte value.
    * Values:
    *
    * INT   integer 16 bits
    * B     for bi-level represented as single bits 1 or 0
    * SI    signend 16 bit integer
    * R     32 bit float
    * C     for complex (32 bit float real part, 32 bit float imag part)
    *       Adjacent 4 byte blocks
    * U     for user defined.
    */
   char thePixelValueType[4];

   /**
    * FIELD: IREP
    * 
    * This is required 8 byte field.
    *
    * Values:
    *
    * MONO    indicates monnochrome
    * RGB     indicates true color red green blue
    * RGB/LUT indicates RGB lookup table.
    * 1D      indicates monoband data.
    * 2D      indicates for 2 dimensional data
    * NM      indicates multidimensional data
    * MULTI   indicates multiband imgery
    * YCbCr601 indicates compressed in CCIR 601
    *          color space using JPEG (field IC=C3)
    */
   char theRepresentation[9];

   /**
    * FIELD: ABPP
    * 
    * Is optional and indicates the number of
    * bits per band 01-96
    */
   char theActualBitsPerPixelPerBand[3];

   /**
    * FIELD: ICAT
    *
    * This is a required 8 byte field.  Contains info
    * about the specific category of the image.
    *
    * VIS    visible imagery
    * SL     side looking infrared
    * TI     thermal infrared
    * FL     forward looking infrared
    * RD     radar
    * EO     elctro-optical
    * OP     optical
    * HR     high resolution radar
    * HS     hyperspectral
    * CP     color frame photography
    * BP     black/white frame photography
    * SAR    Synthetic Aperture Radar
    * SARIQ  SAR radio hologram
    * IR     infrared.
    * MS     multispectral
    * FP     fingerprints
    * MRI    Magnetic Resonance imagery
    * XRAY   x-rays
    * CAT    cat scans
    * MAP    maps
    * PAT    patch
    * LEG    legends
    * DTEM   elevation models
    * MATR   matrix data
    * LOCG   location grids
    * VD     video
    * 
    *
    * default value is VIS
    */
   char theCategory[9];

   /**
    * FIELD: PJUST
    * 
    * Is 1 byte optional field.  Can have
    * values L (left) or R (right).  Any other value
    * is to interpreted as right justified.
    *
    * This indicates how the significant bits are justified
    * in the field
    */
   char theJustification[2];

   /**
    * FIELD: ICORDS
    * 
    * Is a 1 byte required field and can have
    * values 
    *
    * U indicates UTM MGRS format
    * G indicates Geodetic (Geographic)
    * N indicates UTM Northern hemisphere
    * S indicates UTM SOuthern hemisphere
    * D Decimal degrees
    *
    * spaces means no coordinate system
    *
    * Geodetic reference system is WGS84.
    */
   char theCoordinateSystem[2];

   /*!
    * FIELD NICOM:
    * this is a required 1 byte field and indicates the number of comments.
    */
   char theNumberOfComments[2];

   /*!
   * FIELD ICOMnn:
   * Dynamic buffer for the comments.  Each comment is 80 bytes
   */
   char *theImageComments;

   /**
    * FIELD: IGEOLO
    * 
    * This is a conditional field.  If theCoordinateSystem
    * is blank then it does not exist.  If it's any other
    * value then it's given in image coordinate order
    *
    * (0, 0) (0, MaxCol) (MaxRow, MaxCol) (MaxRow, 0)
    *
    * if its geodetic or geocentric it will have a lat lon coordinate
    * with each coordinate as:
    * Lat = ddmmssX
    *       where d is degrees and m is minutes
    *       and s is seconds and X is either N (North) or S (South).
    * Lon = dddmmssX
    *       where d is degrees and m is minutes
    *       and s is seconds and X is either N (North) or S (South).
    *
    *       
    * if its UTM then zzeeeeeennnnnnn where
    *
    * z is the zone number
    * e is the easting
    * n is the northing
    *
    * if its decimal degrees then +-dd.ddd +-dd.ddd four times where
    * + is northern hemispher and - is souther hemisphere for lat
    * and longitude + is easting and - is westing.
    *
    */
   char theGeographicLocation[61];

   /**
    * FIELD: IC
    * 
    * This is a required 2 byte field and indicates the compression
    * type of the image.
    *
    * NC  indicates not compressed
    * C1  indicates bi-level
    * C2  indicates ARIDPCM
    * C3  inidates JPEG
    * C4  inidcates Vector Quantization
    * C5  indicates lossless jpeg
    * I1  indicates a downsampled jpeg
    * M0  indicates compressed images
    * M3  indicates compressed images
    * M4  indicates compressed images
    * M5  indicates compression
    * NM  indicates uncompressed
    *
    * The M's and NM are blocked images that contain
    * block mask and/or transparent pixel mask.  There
    * will be an Image Data Mask Subheader ath the beginning
    * of the image data area.
    *
    */
   char theCompression[3];

   /**
    * FIELD COMRAT
    * 
    * This is a conditional 4 byte  field.  if theCompression is
    * not equal to NC then this field exists.
    *
    * Compression Rate Code.
    *
    * If the Image Compression (IC) field contains, C1, C4, M1, or M4,
    * this field shall be present and contain a code indicating the
    * compression rate for the image.
    * If the value in IC is C1 or M1, the valid codes are 1D, 2DS, and 2DH,
    * where: 1D represents One-dimensional Coding
    *        2DS represents Two-dimensional Coding Standard Vertical
    *        Resolution (K=2)
    *        2DH represents Two-dimensional Coding High Vertical Resolution
    *        (K=4)
    *        
    * A BCS zero (0x30) will be used for the Y value when custom Q-Tables are
    * used.
    * Explanation of these codes can be found in ITU-T T-4, AMD2 08/95.
    *
    * If the value in IC is C3, C5, I1, or M5, the value of this field shall
    * be 00.0.
    * The value 00.0 represents embedded tables and is required by JPEG.
    * Explanation of embedded tables can be found in MIL-STD-188-198A and
    * NIMA N0105-97.
    * If the value in IC is C4 or M4, this field shall contain a value given
    * in the
    * form nn.n representing the number of bits-per-pixel for the compressed
    * image.
    * Explanation of the compression rate for vector quantization can be
    * found in MIL-STD-188-199.
    * This field is omitted if the value in IC is NC or NM. If IC = I1, value
    * = 00.0.
    * 4 BCS-A See description for constraints C 
    */
   char theCompressionRateCode[5];

   /**
    * FIELD: NBANDS
    * 
    * Is required 1 byte field  and will have value 1-9
    *
    * If theRepresentation is MONO this should be 1
    * if theRepresentation is RGB  this should be 3
    * if theRepresentation is RGB/LUT this should be 1
    * if theRepresentation YCbCr601 this should be 3
    * if theRepresentation is MULTI this should be 2-9
    *
    * if this field is 0 then it's multi spectral data
    * larger than 9
    */
   char theNumberOfBands[2];

   /**
    * FIELD: ISYNC
    * 
    * Required 1 byte field.  Has value 0 or 4
    */
   char theImageSyncCode[2];

   /**
    * FIELD: IMODE
    *
    * Required 1 byte field.
    *
    *
    * Image Mode.  This field shall contain an indicator of whether the image
    * bands are stored in the
    * file sequentially or band interleaved by block or band interleaved by
    * pixel format or
    * band interleaved by row.
    *
    * Valid values are B, P, R, and S.  The significance of the IMODE value
    * must be interpreted
    * with the knowledge of whether the image is:
    *     JPEG compressed (IC=C3, C5, M3, or M5),
    *     VQ compressed (IC=C4, or M4),
    *     or uncompressed (IC=NC or NM).
    * When IC=C1 or M1, the use of IMODE defaults to B.  When IC=I1, IMODE is
    * B.
    *
    * For the uncompressed case:  The value R represents band interleaved by
    * row,
    * where the ordering mechanism stores the pixel values of each band in
    * row sequential order.
    * Within each block, all pixel values of the first row of the first band
    * are followed by
    * pixel values of the first row of the second band continuing until all
    * values of the
    * first row are stored.  The remaining rows are stored in a similar
    * fashion until the last
    * row of values has been stored.  Each block shall be zero filled to the
    * next octet boundary
    * when necessary.  The value S represents band sequential, where all
    * blocks for the first band
    * are followed by all blocks for the second band, and so on:
    * [(block1, band1), (block2, band1), ... (blockM, band1)],
    * [(block1, band2), (block2, band 2), ... (blockM, band2)] ...
    * [(block1, bandN), (block2, bandN), ... (blockM, bandN)].
    *
    * The values B and P indicate variations on block sequential where all
    * data from all bands
    * for the first block is followed by all data from all bands for the
    * second block, and so on.
    * The variations are based on the way the bands are organized within each
    * block.
    * B represents band interleaved by block.
    *
    * This means that within each block, the bands follow one 1 BCS-A B, P,
    * R, S R
    *
    *
    * another:  [(block1, band1), (block1, band2), ...
    *           (block1, bandN)],
    *           [(block2, band1),
    *           (block2, band2), ...
    *           (block2, bandN)], ...
    *           [(blockM, band1),
    *           (blockM, band2), ...
    *           (blockM, bandN)].
    *
    * P represents band interleaved by pixel within each block:  such as, for
    * each block,
    * one after the other, the full pixel vector (all band values) appears
    * for every pixel in
    * the block, one pixel after another, the block column index varying
    * faster than the block
    * row index.  If the NBANDS field is 1, the cases B and S coincide.
    * In this case, this field shall contain B.  If the Number of Blocks is
    * 1(NBPR = NBPC = 1),
    * this field shall contain B for non-interleaved by pixel, and P for
    * interleaved by pixel.
    * The value S is only valid for images with multiple blocks and multiple
    * bands.
    * For the JPEG-compressed case:  The presence of B, P, or S implies
    * specific ordering of
    * data within the JPEG image data representation.  The interpretation of
    * the values of IMODE
    * for this case is specified in MIL-STD188-198A.
    *
    * For the Vector Quantization compressed case:  VQ compressed images are
    * normally either RGB with a
    * color look-up table or monochromatic.  In either case, the image is
    * single band, and the IMODE
    * field defaults to B.  However, it is possible to have a multiband VQ
    * compressed image in band sequential,
    * band interleaved by block, or band interleaved by pixel format.
    * 
    */
   char theImageMode[2];

   /**
    * FIELD: NBPR
    * 
    * Required 4 byte value.  ranges from
    * 0001-9999
    */
   char theNumberOfBlocksPerRow[5];
   
   /**
    * FIELD: NBPC
    * 
    * Required 4 byte value. Ranges from
    * 0001-9999
    */
   char theNumberOfBlocksPerCol[5];

   /**
    * FIELD: NPPBH
    * 
    * Reuqired 4 byte field
    */
   char theNumberOfPixelsPerBlockHoriz[5];

   /**
    * FIELD: NPPBV
    * 
    * Reuqired 4 byte field
    */
   char theNumberOfPixelsPerBlockVert[5];

   /**
    * FIELD: NBPP
    * 
    * Required 2 byte field.
    *
    * Number of Bits Per Pixel Per Band.  If IC contains NC, NM, C4, or M4,
    * this field shall contain the number of storage bits used for the value
    * from each component of a pixel vector.  The value in this field always
    * shall be greater than or equal to Actual Bits Per Pixel (ABPP).
    * For example, if 11-bit pixels are stored in 16 bits,
    * this field shall contain 16 and Actual Bits Per Pixel shall contain 11.
    * If IC = C3, M3, C5, M5, or I1 this field shall contain the value 8 or
    * the value 12.
    * If IC = C1, this field shall contain the value 1. 2 BCS-N 01-96 R 
    */
   char theNumberOfBitsPerPixelPerBand[3];

   /**
    * FIELD: IDLVL
    *
    * Display Level.
    * 
    * Required 3 byte field.  Ranges from 001-999
    *
    * This field shall contain a valid value that indicates the graphic display
    * level of the image relative to other displayed file components in a
    * composite
    * display.  The display level of each displayable file component (image
    * or graphic)
    * within a file shall be unique; that is, each number from 001 to 999 is
    * the display
    * level of, at most, one item.  The meaning of display level is fully
    * discussed in
    * paragraph 5.3.3.  The image or graphic component in the file having the
    * minimum
    * display level shall have attachment level 0.
    * 
    */
   char theDisplayLevel[4];

   /**
    * FIELD: IALVL
    *
    * Attachment Level
    * 
    * Required 3 byte field.  Ranges from 000-998
    *
    * This field shall contain a valid value that indicates the attachment
    * level of the image.
    * Valid values for this field are 0, and the display level value of any
    * other image or
    * graphic in the file.  The meaning of attachment level is fully discussed
    * in
    * paragraph 5.3.4. The image, graphic, or text component in the file
    * having the
    * minimum display level shall have attachment level 0.
    * 
    */
   char theAttachmentLevel[4];

   /**
    * FIELD: ILOC
    * 
    * Required 10 byte field.
    *
    * Image Location.  The image location is the location of the first pixel
    * of the first line of
    * the image.  This field shall contain the image location represented as
    * rrrrrccccc, where rrrrr and ccccc are the row and column offset from the
    * ILOC or SLOC value of the item to which the image is attached.  A row or
    * column
    * value of 00000 indicates no offset.  Positive row and column values
    * indicate offsets down
    * and to the right and range from 00001 to 99999, while negative row and
    * column
    * values indicate offsets up and to the left and must be within the range
    * -0001 to -9999.
    * The location in the common coordinate system of all displayable graphic
    * components can be computed from the offsets given in the ILOC and SLOC
    * fields. 10 BCS-N -9999
    * 
    */
   char theImageLocation[11];

   /**
    * FIELD: IMAG
    * 
    * Required 4 byte field
    *
    * Image Magnification.
    *
    * This field shall contain the magnification (or reduction) factor of the
    * image
    * relative to the original source image.  Decimal values are used to
    * indicate magnification,
    * and decimal fraction values indicate reduction.
    * For example, 2.30 indicates the original image has been magnified by a
    * factor of
    * Ò2.30,Ó while Ò0.5Ó indicates the original image has been reduced by a
    * factor of 2.
    * The default value is 1.0, indicating no magnification or reduction.
    * In addition, the following values shall be used for reductions that are
    * reciprocals of
    * nonnegative powers of 2: /2 (for 1/2), /4 (for 1/4), /8 (for 1/8), /16
    * (for 1/16), /32 (for 1/32),
    * /64 (for 1/64), /128 (for 1/128). 4 BCS-A /2, /4, /8, /16, /32, /64,
    * /128 or
    * decimal value (Default is 1.0 followed by BCS space (0x20)) R
    * 
    */
   char theImageMagnification[5];

   /**
    * FIELD: UDIDL
    * 
    * Required 5 byte field. 00000 or 00003-99999
    *
    * User Defined Image Data Length.  A value of 0 shall represent that no
    * tagged record extensions
    * are included in the UDID field.  If a tagged record extension exists,
    * the field shall contain
    * the sum of the length of all the tagged record extensions (see
    * paragraph 5.7.l.l) appearing in
    * the UDID field plus 3 bytes (length of UDOFL field).  If a tagged
    * record extension is too long
    * to fit in the UDID field, it may be put in a data extension segment
    * (see paragraph 5.8.1.3.1.)
    *
    */
   char theUserDefinedImageDataLength[6];

   /**
    * FIELD: UDOFL
    * 
    * conditional 3 byte field. if theUserDefinedImageDataLength
    * is not 0
    *
    * User Defined Overflow.  If present, this field shall contain 000 if the
    * tagged record extensions
    * in UDID do not overflow into a DES, or shall contain the sequence number
    * of the DES into which
    * they do overflow.  This field shall be omitted if the field UDIDL
    * contains BCS zeros (0x30).
    *
    * 
    */
   char theUserDefinedOverflow[4];

   /**
    * FIELD: IXSHDL
    * 
    * Is a required 5 byte field. 00000 or 00003-99999
    *
    * Extended Subheader Data Length.  This field shall contain the length in
    * bytes in
    * IXSHD plus 3 (length of IXSOFL).  The length is 3 plus sum of the
    * lengths of all the
    * tagged record extensions (see paragraph 5.7.1.2) appearing in the IXSHD
    * field.  A value of
    * BCS zeros (0x30) shall represent that no tagged record extensions are
    * included in the IXSHD field.
    * If a tagged record extension exists, the field shall contain the sum of
    * the length of all the tagged
    * record extensions (see paragraph 5.7.1.2) appearing in the IXSHD field
    * plus 3 bytes (length of
    * IXSOFL field).  If a tagged record extension is too long to fit in the
    * IXSHD field, it shall may
    * be put in a data extension segment (see paragraph 5.7.1.4). 
    */
   char theExtendedSubheaderDataLen[6];

   /**
    * FIELD: IXSOFL
    * 
    * Is a conditional 3 byte field.  This depends on
    * theExtendedSubheaderDataLen not being 0.
    *
    * Extended Subheader Data.  If present, this field shall contain tagged
    * record
    * extensions (see paragraph 5.8.1.2) approved and under configuration
    * management by the ISMC.
    * The length of this field shall be the length specified by the field
    * IXSHDL minus 3.  Tagged
    * record extensions in this field for an image shall contain information
    * pertaining specifically
    * to the image.  Tagged record extensions shall appear one after the other
    * in this field with no
    * intervening bytes.  The first byte of this field shall be the first byte
    * of the  first tagged record
    * extension appearing in the field.  The last byte of this field shall be
    * the last byte of the last
    * tagged record extension to appear in the field.  This field shall be
    * omitted if the field IXSHDL
    * contains 0.
    */
   char theExtendedSubheaderOverflow[4];

   /**************************************************************************
    *                    THIS IS THE MASK TABLE IF PRESENT
    */
   
   /**
    * FIELD NAME: IMDATOFF
    * 
    * Blocked Image Data Offset.
    * 
    * 4 byte Binary integer: 0x00000000 to 0xFFFFFFFF
    * 
    * This field is included if the IC value equals NM, M1, M3, M4, or M5.
    * It identifies the offset from the beginning of the Image Data Mask to
    * the first byte of the blocked image data.
    * This offset, when used in combination with the offsets provided in the
    * BMR fields,
    * can provide random access to any recorded image block in any image band.
    * 
    */
   ossim_uint32 theBlockedImageDataOffset;

   /**
    * FIELD NAME: BMRLNTH
    *
    * Block Mask Record Length.
    *
    * 2 byte Unsigned binary integer
    * 0x0000=No Block mask record; 0x0004=Block mask
    * records (4 bytes each) are present  
    * 
    * This field is included if the IC value equals NM, M1, M3, M4, or M5.
    * It identifies the length of each Block Mask Record in bytes.
    * When present, the length of each Block Mask Record is 4 bytes.
    * The total length of all the block Mask Records is equal to
    *
    * For S mode BMRLNTH x NBPR x NBPC x NBANDS  (one 4 byte record for each
    * block of each band in the image).
    * and for all other modes BMRLNTH x NBPR x NBPC
    * 
    * If all of the image blocks are recorded,
    * this value may be set to 0, and the conditional BMR fields are not
    * recorded/transmitted.
    * Otherwise, the value may be set to 4, and the conditional BMR fields
    * are
    * recorded/transmitted and can be used as an off-set index for each
    * image block in each band of the image.
    * If this field is present, but coded as 0, then only a pad pixel mask is
    * included.
    */
   ossim_uint16 theBlockMaskRecordLength;

   /**
    * FIELD: TMRLNTH
    *
    * Pad Pixel Mask Record Length.
    *
    * 2 byte Unsigned binary integer a value of
    * 0x0000=No Pad pixel mask records and 
    * 0x0004=Pad pixel mask records (4 bytes each) are present
    * 
    * This field is included if the IC value equals NM, M1, M3, M4, or M5 
    * It identifies the length of each Pad Pixel Mask Record in bytes.
    * When present, the length of each Pad Pixel Mask Record is 4 bytes.
    * The total length of the Pad Pixel Mask Records is equal to
    * 
    * For image mode S then TMRLNTH x NBPR x NBPC x NBANDS (one 4 byte record
    * for each block for each band in the image).
    * else for all other modes TMRLNTH x NBPR x NBPC
    * 
    * If none of the image blocks contain pad pixels, this value is set to 0,
    * and the conditional TMR fields are not recorded/transmitted.
    * For IC value of M3, the value shall be set to 0.
    * If this field is present, but coded as 0, then a Block Mask is included.
    */
   ossim_uint16 thePadPixelMaskRecordLength;
   
   /**
    *  FIELD: TPXCDLNTH
    *
    *  Transparent Output Pixel Code Length.
    *
    *  2 byte Unsigned binary integer;
    *  0x0000=No pad pixels; 0x0001 to 0x0010=pad pixel code length in bits
    *  
    *  This field is included if the IC value equals NM, M1, M3, M4, M5.
    *  It identifies the length in bits of the Transparent Output Pixel Code.
    *  If coded as 0, then no transparent pixels are present,
    *  and the TPXCD field is not recorded.  For IC value of M3, the
    *  value shall be set to 0..
    *  
    */
   ossim_uint16 theTransparentOutputPixelCodeLength;

   /**
    * FIELD: TPXCD
    *
    * Pad Output Pixel Code.
    *
    * This field is included if the IC value equals NM, M1, M3, M4, M5, M0 and
    * TPXCDLNTH is not 0.  It contains the output pixel code that represents a
    * pad pixel in the image.  This value is unique within the image, and
    * allows the user to identify pad pixels.  The pad output pixel code
    * length is determined by TPXCDLNTH, but the value is stored in a maximum
    * of 2 bytes.  If the number of bits used by TPXCD is less than the number
    * of bits available for storage,
    * the value shall be justified in accordance with the PJUST field in the
    * image subheader.  
    * 3A Binary integer; 0 to 2^n -1 where n=TPXCDLNTH
    */
   ossim_uint16 thePadOutputPixelCode;

   std::vector<ossim_uint32> theBlockMaskRecords;
   std::vector<ossim_uint32> thePadPixelMaskRecords;
   ossimRefPtr<ossimNitfCompressionHeader> theCompressionHeader;
   ossim_uint64 theDataLocation;

TYPE_DATA
   
};

#endif
