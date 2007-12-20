//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: A brief description of the contents of the file.
//
//*******************************************************************
// $Id: ossimImageData.h 12034 2007-11-13 19:48:39Z gpotts $
#ifndef ossimImageData_HEADER
#define ossimImageData_HEADER

#include <vector>

#include <ossim/base/ossimRectilinearDataObject.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimRefPtr.h>

class ossimMultiBandHistogram;

class OSSIMDLLEXPORT ossimImageData : public ossimRectilinearDataObject
{
public:

   /** @brief copy constructor */
   ossimImageData(const ossimImageData &rhs);

   /**
    * @brief Constructor
    * @param source The owner.  This can be null.
    * @param scalar The Scalar type like OSSIM_UINT8, OSSIM_UINT16...
    * @param bands  The number of bands.
    * 
    * @note This does not initialize the undelying data buffer.
    *       Call the initialize method to allocate storage.
    */
   ossimImageData(ossimSource* source,
                  ossimScalarType scalar,
                  ossim_uint32 bands = 1);

   /**
    * @brief Constructor
    * @param source The owner.  This can be null.
    * @param scalar The Scalar type like OSSIM_UINT8, OSSIM_UINT16...
    * @param bands  The number of bands.
    * @param width  The width or number of samples in the buffer.
    * @param height The height or numer of lines in the buffer.
    * 
    * @note This does not initialize the undelying data buffer.
    *       Call the initialize method to allocate storage.
    */
   ossimImageData(ossimSource* source,
                  ossimScalarType scalar,
                  ossim_uint32 bands,
                  ossim_uint32 width,
                  ossim_uint32 height);

   /** @brief virtual destructor */
   virtual ~ossimImageData();

   /**
    * @return The width of the data object.
    */
   virtual ossim_uint32 getWidth()const;

   /**
    * @return The height of the data object.
    */
   virtual ossim_uint32 getHeight()const;

   /**
    * @return The image rectangle relative to origin, height and width.
    */
   virtual ossimIrect getImageRectangle() const;
   virtual void getWidthHeight(ossim_uint32& w, ossim_uint32& h);
   virtual void setWidth(ossim_uint32 width);
   virtual void setHeight(ossim_uint32 height);
   virtual void setWidthHeight(ossim_uint32 w, ossim_uint32 h);
   virtual void setImageRectangleAndBands(const ossimIrect& rect,
                                          ossim_uint32 numberOfBands);
   virtual void setImageRectangle(const ossimIrect& rect);
   virtual void setOrigin(const ossimIpt& origin);   
                                    
   virtual void assign(const ossimDataObject* data);
   virtual void assign(const ossimImageData* data);
   virtual void assignBand(const ossimImageData* data,
                           ossim_uint32 source_band,
                           ossim_uint32 output_band);
   
   virtual ossimObject* dup() const;

  /**
   * Uses prime numbers as coefficients for this summation.  
   * Take the the fours bytes of each origin and multiply 
   * by the prime numbers and sum together.  This has
   * been proven by others to give nicely distributed 
   * id number.
   */
   virtual ossim_uint32 getHashId()const;
   
   virtual void setNullPix(ossim_float64 null_pix);
   virtual void setNullPix(ossim_float64 null_pix, ossim_uint32 band);
   virtual void setNullPix(const ossim_float64* nullPixArray,
                           ossim_uint32 numberOfValues);
   virtual void setMinPix(ossim_float64 min_pix);
   virtual void setMinPix(ossim_float64 min_pix, ossim_uint32 band);
   virtual void setMinPix(const ossim_float64* minPixArray,
                          ossim_uint32 numberOfValues);
   virtual void setMaxPix(ossim_float64 max_pix);
   virtual void setMaxPix(ossim_float64 max_pix, ossim_uint32 band);
   virtual void setMaxPix(const ossim_float64* maxPixArray,
                          ossim_uint32 numberOfValues);

   virtual const ossimIpt& getOrigin()const{return theOrigin;}

   /**
    * If the minBands and maxBands are empty or not equal to the imageData's
    * current number of bands it will erase the contents, resize, and then
    * replace the values in the passed in array with this objects min max
    * values.  If the arrays are the same size as this objects number of
    * bands it will just update the values and use the current values in the
    * arrays as initial min max values.
    *
    * If the status of this object(getDataObjectStatus()) is OSSIM_NULL or
    * OSSIM_EMPTY no action is performed.
    */
   virtual void computeMinMaxPix(std::vector<ossim_float64>& minBands,
                                 std::vector<ossim_float64>& maxBands);
   
   virtual const ossim_float64* getNullPix()const;
   virtual const ossim_float64* getMinPix()const;
   virtual const ossim_float64* getMaxPix()const;
   virtual ossim_float64        getNullPix(ossim_uint32 band) const;
   virtual ossim_float64        getMinPix(ossim_uint32 band)  const;
   virtual ossim_float64        getMaxPix(ossim_uint32 band)  const;

   /**
    * returns normalized minimum pixel value of band zero.
    */
   virtual ossim_float64   getMinNormalizedPix() const;
   virtual ossimString     getScalarTypeAsString()const;
   virtual ossim_uint32    getNumberOfBands() const;
   virtual void            setNumberOfBands(ossim_uint32 bands,
                                            bool reallocate=false);

   /**
    * Will return the pixel at location position.  Note it will first get
    * the passed in position relative to the origin or upper left
    * corner of this tile and then return the result.
    */
   virtual ossim_float64 getPix(const ossimIpt& position,
                                ossim_uint32 band=0) const;

   /**
    * Will return the pixel at offset and band number.
    */
   virtual ossim_float64 getPix(ossim_uint32 offset,
                                ossim_uint32 band = 0) const;
   
   /**
    * will fill the entire band with
    * the value.
    */
   void fill(ossim_uint32 band, ossim_float64 value);

   /**
    * will fill all bands with the passed in
    * value.
    */
   void fill(ossim_float64 value);
   bool isNull(ossim_uint32 offset)const;
   bool isNull(ossim_uint32 offset, ossim_uint32 band)const;
   void setNull(ossim_uint32 offset);
   void setNull(ossim_uint32 offset, ossim_uint32 band);

   bool isNull(const ossimIpt& pt)const;
   void setNull(const ossimIpt& pt);
   bool isNull(const ossimIpt& pt, ossim_uint32 band)const;
   void setNull(const ossimIpt& pt, ossim_uint32 band);
   
   virtual bool   isValidBand(ossim_uint32 band) const;
   virtual ossimDataObjectStatus validate() const;

   /**
    * Will take this tile and normalize it to a newly
    * allocated floating point tile.
    */
   virtual ossimRefPtr<ossimImageData> newNormalizedFloat()const;

   /**
    * will go to the band and offset and compute the
    * normalized float and return it back to the
    * caller through the result argument.
    */
   virtual void getNormalizedFloat(ossim_uint32 offset,
                                   ossim_uint32 bandNumber,
                                   ossim_float32& result)const;

   /**
    * This will assign to this object a normalized
    * value by unnormalizing to its native type.
    */
   virtual void setNormalizedFloat(ossim_uint32 offset,
                                   ossim_uint32 bandNumber,
                                   ossim_float32 input);

   /**
    * Will use the memory that you pass in to normalize
    * this data object.
    */
   virtual void convertToNormalizedFloat(ossimImageData* result)const;

   /**
    * Will take this tile and normalize it to a newly
    * allocated double point tile.
    */
   virtual ossimImageData* newNormalizedDouble()const;

   /**
    * Will use the memory that you pass in to normalize
    * this data object.
    */
   virtual void convertToNormalizedDouble(ossimImageData* result)const;

   /**
    * Will take the normalized input and convert it
    * to this tile's data type.  Example:  if this
    * tile is of type UCHAR and its input is of type
    * NORALIZED_FLOAT it will unnormalize the data by
    * doing:
    *
    * minPix + normalizedInput*(maxPix-minPix)
    *
    * on a per band basis.
    */
   virtual void unnormalizeInput(ossimImageData* normalizedInput);

   /**
    * This will compute the average value for the band.
    */
   virtual ossim_float64 computeAverageBandValue(
      ossim_uint32 bandNumber = 0) const;

   /**
    * This will call the compute average band value and then
    * use that in the calculation of:
    
    * It will then do a SUM[(Mean - Actual)^2]/width*height.
    *
    * This is the average variance from the passed in
    * mean.  
    */
   virtual ossim_float64 computeMeanSquaredError(
      ossim_float64 meanValue,
      ossim_uint32 bandNumber = 0) const;
  
   virtual void populateHistogram(ossimRefPtr<ossimMultiBandHistogram> histo);

   /**
    * @return const void* to theDataBuffer
    */
   virtual const void* getBuf() const;
   
   /**
    * @return const ossim_uint8* to theDataBuffer if scalar is of type
    * OSSIM_UCHAR; else, NULL.
    */
   virtual const ossim_uint8*    getUcharBuf()  const;

   /**
    * @return const ossim_uint16* to theDataBuffer if scalar is of type
    * OSSIM_USHORT16 or USHORT11; else, NULL.
    */
   virtual const ossim_uint16*   getUshortBuf() const;

   /**
    * @return const ossim_sint16* to theDataBuffer if scalar is of type
    * OSSIM_SSHORT16; else, NULL.
    */
   virtual const ossim_sint16*   getSshortBuf() const;

   /**
    * @return const ossim_float32* to theDataBuffer if scalar is of type
    * OSSIM_FLOAT or OSSIM_NORMALIZED_FLOAT; else, NULL.
    */
   virtual const ossim_float32*  getFloatBuf()  const;

   /**
    * @return const ossim_float64* to theDataBuffer if scalar is of type
    * OSSIM_DOUBLE or OSSIM_NORMALIZED_DOUBLE; else, NULL.
    */
   virtual const ossim_float64*  getDoubleBuf() const;

   /**
    * @return void* to theDataBuffer
    */
   virtual void* getBuf();
   
   /**
    * @return ossim_uint8* to theDataBuffer if scalar is of type
    * OSSIM_UCHAR; else, NULL.
    */
   virtual ossim_uint8*    getUcharBuf();
   
   /**
    * @return ossim_uint16* to theDataBuffer if scalar is of type
    * OSSIM_USHORT16 or OSSIM_USHORT11; else, NULL.
    */
   virtual ossim_uint16*   getUshortBuf();
   
   /**
    * @return ossim_sint16* to theDataBuffer if scalar is of type
    * OSSIM_SSHORT16; else, NULL.
    */
   virtual ossim_sint16*   getSshortBuf();

   /**
    * @return ossim_float32* to theDataBuffer if scalar is of type
    * OSSIM_FLOAT or OSSIM_NORMALIZED_FLOAT; else, NULL.
    */
   virtual ossim_float32*  getFloatBuf();
   
   /**
    * @return float64* to theDataBuffer if scalar is of type OSSIM_DOUBLE or
    * OSSIM_NORMALIZED_DOUBLE; else, NULL.
    */
   virtual ossim_float64*  getDoubleBuf();

   /**
    * @param band (zero based)
    * @return void* to theDataBuffer positioned at band.
    */
   virtual const void*           getBuf(ossim_uint32 band) const;
   
   /**
    * @param band (zero based)
    * @return ossim_uint8* to theDataBuffer positioned at band if scalar is
    * of type OSSIM_UCHAR; else, NULL.
    */
   virtual const ossim_uint8*    getUcharBuf(ossim_uint32 band) const;

   /**
    * @param band (zero based)
    * @return ossim_uint16* to theDataBuffer positioned at band if scalar is
    * of type OSSIM_USHORT16 or OSSIM_USHORT11; else, NULL.
    */
   virtual const ossim_uint16*   getUshortBuf(ossim_uint32 band) const;
   
   /**
    * @param band (zero based)
    * @return ossim_sint16* to theDataBuffer positioned at band if scalar is
    * of type OSSIM_SSHORT16; else, NULL.
    */
   virtual const ossim_sint16*   getSshortBuf(ossim_uint32 band) const;
   
   /**
    * @param band (zero based)
    * @return ossim_float32* to theDataBuffer positioned at band if scalar is
    * of type OSSIM_FLOAT or OSSIM_NORMALIZED_FLOAT; else, NULL.
    */
   virtual const ossim_float32*  getFloatBuf(ossim_uint32 band) const;
   
   /**
    * @param band (zero based)
    * @return ossim_float64* to theDataBuffer positioned at band if scalar is
    * of type OSSIM_DOUBLE or OSSIM_NORMALIZED_DOUBLE; else, NULL.
    */
   virtual const ossim_float64*  getDoubleBuf(ossim_uint32 band) const;

   /**
    * @param band (zero based)
    * @return void* to theDataBuffer positioned at band.
    */
   virtual void*           getBuf(ossim_uint32 band);
   
   /**
    * @param band (zero based)
    * @return ossim_uint8* to theDataBuffer positioned at band if scalar is
    * of type OSSIM_UCHAR; else, NULL.
    */
   virtual ossim_uint8*    getUcharBuf(ossim_uint32 band);
   
   /**
    * @param band (zero based)
    * @return ossim_uint16* to theDataBuffer positioned at band if scalar is
    * of type OSSIM_USHORT16 or OSSIM_USHORT11; else, NULL.
    */
   virtual ossim_uint16*   getUshortBuf(ossim_uint32 band);
   
   /**
    * @param band (zero based)
    * @return ossim_sint16* to theDataBuffer positioned at band if scalar is
    * of type OSSIM_SSHORT16; else, NULL.
    */
   virtual ossim_sint16*   getSshortBuf(ossim_uint32 band);
   
   /**
    * @param band (zero based)
    * @return ossim_float32* to theDataBuffer positioned at band if scalar is
    * of type OSSIM_FLOAT or OSSIM_NORMALIZED_FLOAT; else, NULL.
    */
   virtual ossim_float32*  getFloatBuf(ossim_uint32 band);
   
   /**
    * @param band (zero based)
    * @return ossim_float64* to theDataBuffer positioned at band if scalar is
    * of type OSSIM_DOUBLE or OSSIM_NORMALIZED_DOUBLE; else, NULL.
    */
   virtual ossim_float64*  getDoubleBuf(ossim_uint32 band);
   
   /**
    *  Returns the total number of pixels in a tile for all bands.
    *  Note:  This is not in bytes but in pixels.
    */
   virtual ossim_uint32 getSize() const;

   /**
    *  Returns the number of pixels in a single band in a tile.
    *  Note:  This is not in bytes but in pixels.
    */
   virtual ossim_uint32 getSizePerBand()const;

   /**
    *  Returns the number of bytes in single band of the tile.
    */
   virtual ossim_uint32 getSizePerBandInBytes() const;

   virtual ossim_uint32 getDataSizeInBytes()const;

   /**
    * Returns the total number of bytes for all bands.
    */
   virtual ossim_uint32  getSizeInBytes() const;

   /**
    *  Initializes data to null pixel values.
    */
   virtual void makeBlank();

   /**
    *  Initialize the data buffer.
    */
   virtual void initialize();

   /**
    * Copies entire tile to buf passed in.  Data put in buf is normalized.
    * The "buf" passed to method is assumed to be at least as big as:
    * "getSize() * sizeof(double)"
    */
   virtual void copyTileToNormalizedBuffer(ossim_float64* buf) const;
   virtual void copyTileToNormalizedBuffer(ossim_float32* buf)const;

   /**
    * Will copy this tiles specified band number to the normalized buffer.
    * if the band is out of range then nothing is done and returns.
    *
    * @param band Zero based band to copy.
    *
    * @param buf Buffer to copy to.
    */
   virtual void copyTileBandToNormalizedBuffer(ossim_uint32 band,
                                               ossim_float64* buf) const;
   
   /**
    * Will copy this tiles specified band number to the normalized buffer.
    * if the band is out of range then nothing is done and returns.
    *
    * @param band Zero based band to copy.
    *
    * @param buf Buffer to copy to.
    */
   virtual void copyTileBandToNormalizedBuffer(ossim_uint32 band,
                                               ossim_float32* buf)const;

   /**
    * Copies buf passed in to tile.  Data is unnormalized to the tile's
    * scalar type.
    * The "buf" passed to method is assumed to be at least as big as the tiles:
    * "getSize() * sizeof(double)"
    */
   virtual void copyNormalizedBufferToTile(ossim_float64* buf);
   virtual void copyNormalizedBufferToTile(ossim_float32* buf);

   /**
    * Will copy the normalized buffer to this tiles
    * specified band.  If band is out of range then nothing
    *
    * is done and returns.
    */
   virtual void copyNormalizedBufferToTile(ossim_uint32 band,
                                           ossim_float64* buf);
   virtual void copyNormalizedBufferToTile(ossim_uint32 band,
                                           ossim_float32* buf);

   virtual bool isWithin(ossim_int32 x, ossim_int32 y);
   virtual void setValue(ossim_int32 x, ossim_int32 y, ossim_float64 color);

   virtual void loadBand(const void* src,
                         const ossimIrect& src_rect,
                         ossim_uint32 band);

   virtual void loadBand(const void* src,
                         const ossimIrect& src_rect,
                         const ossimIrect& clip_rect,
                         ossim_uint32 band);
   
   virtual void loadTile(const void* src,
                         const ossimIrect& src_rect,
                         ossimInterleaveType il_type);

   virtual void loadTile(const void* src,
                         const ossimIrect& src_rect,
                         const ossimIrect& clip_rect,                         
                         ossimInterleaveType il_type);

   virtual void nullTileAlpha(const ossim_uint8* src,
                              const ossimIrect& src_rect,
                              bool mutliplyAlphaFlag=false);
   virtual void nullTileAlpha(const ossim_uint8* src,
                              const ossimIrect& src_rect,
                              const ossimIrect& clip_rect,
                              bool multiplyAlphaFlag=false);
   /**
    * Will load a tile of different types.  If they
    * are the same then it will call loadTile(void*...)
    * method.  Note if they are of different types we
    * will have to go through a much slower assign
    * process by normalizing the right side and then
    * unnormalizing to the destination type. For this
    * method to work they both must have the same
    * number of bands.
    */
   virtual void loadTile(const ossimImageData* src);
   
   virtual void loadTileFrom1Band(const ossimImageData* data);

   virtual void loadTileFrom1Band(const void* src,
                                  const ossimIrect& src_rect);
   /**
    * Specialized to load a tile from a short (16 bit scalar type) buffer.
    * Performs byte swapping if swap_bytes is set to true.
    */
   virtual void loadShortBand(const void* src,
                              const ossimIrect& src_rect,
                              ossim_uint32 band,
                              bool swap_bytes=false);
   
   virtual void unloadTile(void* dest,
                           const ossimIrect& dest_rect,
                           ossimInterleaveType il_type) const;
   
   virtual void unloadTile(void* dest,
                           const ossimIrect& dest_rect,
                           const ossimIrect& clip_rect,
                           ossimInterleaveType il_type) const;

   /**
    * @param dest The destination buffer.  It is assumed this is a single band
    * and the size of dest_rect.
    * @param dest_rect The rectangle of the destination buffer.
    * @param band The band to copy (unload) from the tile.
    */
   virtual void unloadBand(void* dest,
                           const ossimIrect& dest_rect,
                           ossim_uint32 band) const;

   /**
    * @param dest The destination buffer.  It is assumed this is a single band
    * and the size of dest_rect.
    * @param dest_rect The rectangle of the destination buffer.
    * param clip_rect Only data within the clip_rect will be copied.
    * @param band The band to copy (unload) from the tile.
    */
   virtual void unloadBand(void* dest,
                           const ossimIrect& dest_rect,
                           const ossimIrect& clip_rect,
                           ossim_uint32 band) const;
   
   
   virtual bool isPointWithin(const ossimIpt& point)const;
   virtual bool isPointWithin(ossim_int32 x,
                              ossim_int32 y)const;

   virtual std::ostream& print(std::ostream& out) const;
   
protected:
   ossimImageData();

   /**
    * Templated valide method.
    *
    * @return The status of the ossimImageDataMethod.
    *
    * @note See ossimConstants.h for ossimDataObjectStatus enumerations.
    */
   template <class T> ossimDataObjectStatus validate(T dummyTemplate) const;

   /**
    * Templated makeBlank method.
    */
   template <class T> void makeBlank(T dummyTemplate);

   /**
    * Templated computeMeanSquaredError method.
    */
   template <class T>
      ossim_float64 computeMeanSquaredError(T dummyTemplate,
                                            ossim_float64 meanValue,
                                            ossim_uint32 bandNumber) const;

   /**
    * Templated computeAverageBandValue method.
    */
   template <class T>
      ossim_float64 computeAverageBandValue(T dummyTemplate,
                                            ossim_uint32 bandNumber) const;
   
   /**
    *  
    *  Takes normPix, unnormalizes and initializes pixel.
    *  "band" is used as index to get the min/max/null values.
    */
   void getPixFromNorm(ossim_int32& pixel,
                       ossim_float32  normPix,
                       ossim_uint32 band) const;

   
   /**
    *  Takes normPix, unnormalizes and initializes pixel.
    *  "band" is used as index to get the min/max/null values.
    */
   void getPixFromNorm(ossim_int32& pixel,
                       ossim_float64 normPix,
                       ossim_uint32 band) const;

   /**
    *  Take pixel, normalizes and initializes normPix.
    *  "band" is used as index to get the min/max/null values.
    */
   void getNormFromPix  (ossim_int32  pixel,
                         ossim_float32& normPix,
                         ossim_uint32 band) const;

   /**
    *  Take pixel, normalizes and initializes normPix.
    *  "band" is used as index to get the min/max/null values.
    */
   void getNormFromPix (ossim_int32  pixel,
                        ossim_float64& normPix,
                        ossim_uint32 band) const;

   void loadTileFromBip(const void* src, const ossimIrect& src_rect);
   void loadTileFromBil(const void* src, const ossimIrect& src_rect);
   void loadTileFromBsq(const void* src, const ossimIrect& src_rect);

   void loadTileFromBip(const void* src,
                        const ossimIrect& src_rect,
                        const ossimIrect& clip_rect);
   void loadTileFromBil(const void* src,
                        const ossimIrect& src_rect,
                        const ossimIrect& clip_rect);
   void loadTileFromBsq(const void* src,
                        const ossimIrect& src_rect,
                        const ossimIrect& clip_rect);

   void unloadTileToBip(void* dest,
                        const ossimIrect& dest_rect,
                        const ossimIrect& clip_rect) const;
   void unloadTileToBil(void* dest,
                        const ossimIrect& dest_rect,
                        const ossimIrect& clip_rect) const;
   void unloadTileToBsq(void* dest,
                        const ossimIrect& dest_rect,
                        const ossimIrect& clip_rect) const;

   /**
    * Copies entire tile to buf passed in.  Data put in buf is normalized.
    * The "buf" passed to method is assumed to be at least as big as:
    * "getSize() * sizeof(double)"
    */
   template <class T>
   void copyTileToNormalizedBuffer(T dummyTemplate,
                                   ossim_float64* buf) const;
   template <class T>
   void copyTileToNormalizedBuffer(T dummyTemplate,
                                   ossim_float32* buf)const;

   /**
    * Will copy this tiles specified band number to the normalized buffer.
    * if the band is out of range then nothing is done and returns.
    */
   template <class T>
   void copyTileToNormalizedBuffer(T dummyTemplate,
                                   ossim_uint32 band, ossim_float64* buf) const;
   template <class T>
   void copyTileToNormalizedBuffer(T dummyTemplate,
                                   ossim_uint32 band, ossim_float32* buf)const;

   /**
    * Copies buf passed in to tile.  Data is unnormalized to the tile's
    * scalar type.
    * The "buf" passed to method is assumed to be at least as big as the tiles:
    * "getSize() * sizeof(double)"
    */
   template <class T>
   void copyNormalizedBufferToTile(T dummyTemplate,
                                   ossim_float64* buf);
   template <class T>
   void copyNormalizedBufferToTile(T dummyTemplate,
                                   ossim_float32* buf);
   template <class T>
   void copyNormalizedBufferToTile(T dummyTemplate,
                                   ossim_uint32 band,
                                   ossim_float64* buf);
   template <class T>
   void copyNormalizedBufferToTile(T dummyTemplate,
                                   ossim_uint32 band,
                                   ossim_float32* buf);

   
   template <class T> void computeMinMaxPix(T dummyValue,
                                            std::vector<ossim_float64>& minBands,
                                            std::vector<ossim_float64>& maxBands);
   
   template <class T> void loadBandTemplate(T, // dummy template variable
                                            const void* src,
                                            const ossimIrect& src_rect,
                                            ossim_uint32 band);

   template <class T> void loadBandTemplate(T, // dummy template variable
                                            const void* src,
                                            const ossimIrect& src_rect,
                                            const ossimIrect& clip_rect,
                                            ossim_uint32 band);

   template <class T> void nullTileAlphaTemplate(T,
                                                 const ossim_uint8* src,
                                                 const ossimIrect& src_rect,
                                                 const ossimIrect& clip_rect,
                                                 bool multiplyAlphaFlag=false);
   
   template <class T> void loadTileFromBipTemplate(T, // dummy template variable
                                                   const void* src,
                                                   const ossimIrect& src_rect);

   template <class T> void loadTileFromBipTemplate(T, // dummy template variable
                                                   const void* src,
                                                   const ossimIrect& src_rect,
                                                   const ossimIrect& clip_rect);
   
   template <class T> void loadTileFromBilTemplate(T, // dummy template variable
                                                   const void* src,
                                                   const ossimIrect& src_rect);
   
   template <class T> void loadTileFromBilTemplate(T, // dummy template variable
                                                   const void* src,
                                                   const ossimIrect& src_rect,
                                                   const ossimIrect& clip_rect);
   
   template <class T> void loadTileFromBsqTemplate(T, // dummy template variable
                                                   const void* src,
                                                   const ossimIrect& src_rect);
   
   template <class T> void loadTileFromBsqTemplate(T, // dummy template variable
                                                   const void* src,
                                                   const ossimIrect& src_rect,
                                                   const ossimIrect& clip_rect);
   
   template <class T> void unloadTileToBipTemplate(T, // dummy tmeplate variable
                                                   void* dest,
                                                   const ossimIrect& dest_rect,
                                                   const ossimIrect& clip_rect) const;

   template <class T> void unloadTileToBilTemplate(T, // dummy tmeplate variable
                                                   void* dest,
                                                   const ossimIrect& dest_rect,
                                                   const ossimIrect& clip_rect) const;

   template <class T> void unloadTileToBsqTemplate(T, // dummy tmeplate variable
                                                   void* dest,
                                                   const ossimIrect& dest_rect,
                                                   const ossimIrect& clip_rect) const;

   template <class T> void unloadBandTemplate(T, // dummy tmeplate variable
                                              void* dest,
                                              const ossimIrect& dest_rect,
                                              const ossimIrect& clip_rect,
                                              ossim_uint32 band) const;

   /**
    * @brief initializeDefaults() Resizes and sets min/max/null arrays to
    * number of bands and some default value for scalar type.
    */
   virtual void initializeDefaults();

   /**
    * @brief initializeMinDefault() Resizes theMinPixelValue array to number
    * of bands and initializes to the default min for scalar type.
    */
   virtual void initializeMinDefault();

   /**
    * @brief initializeMaxDefault() Resizes theMaxPixelValue array to number
    * of bands and initializes to the default max for scalar type.
    */
   virtual void initializeMaxDefault();

   /**
    * @brief initializeNullDefault() Resizes theNullPixelValue array to number
    * of bands and initializes to the default null for scalar type.
    */
   virtual void initializeNullDefault();

   virtual bool isEqualTo(const ossimDataObject& rhs,
                          bool deepTest=false)const;
   
   bool hasSameDimensionsAs(ossimImageData* data)const
   {
      if(!data) return false;
      return ((theNumberOfSpatialComponents ==
               data->theNumberOfSpatialComponents)&
              (theSpatialExtents[0] == data->theSpatialExtents[0])&&
              (theSpatialExtents[1] == data->theSpatialExtents[1])&&
              (theScalarType == data->theScalarType));
   }
   
   /**
    * Have a null pixel value per band.
    */
   std::vector<ossim_float64> theNullPixelValue;

   /**
    * Have a min pixel value per band.
    */
   std::vector<ossim_float64> theMinPixelValue;

   /**
    * Have a max pixel value per band.
    */
   std::vector<ossim_float64> theMaxPixelValue;
   
   ossimIpt       theOrigin;
   
private:
   /**
    * @note disallow 
    */
   const ossimImageData& operator=(const ossimImageData &rhs);
   
TYPE_DATA
};

inline void ossimImageData::getPixFromNorm(ossim_int32& pixel,
                                           ossim_float32 normPix,
                                           ossim_uint32 band) const
{
   // If norm pix is 0.0 or negative make if null.
   ossim_float32 p = theNullPixelValue[band]; 
   
   if (normPix > 0.0)
   {
      // un-normalize...
      p = normPix * (theMaxPixelValue[band]-theMinPixelValue[band]+1.0);
      
      // handle negative min values.
      p = p + theMinPixelValue[band] - 1.0;  
      
      // Clamp to  max.
      if (p > theMaxPixelValue[band]) p = theMaxPixelValue[band];
   
      //***
      // If p is less than min, make it min as it wasn't a null before.
      // Add or subtract 0.5 to p otherwise as it will be cast to an
      // int for proper rounding.
      //***
      p = p > theMinPixelValue[band] ?
          ( p<0.0 ? p-0.5 : p+0.5 ) :
          theMinPixelValue[band];
   }

   // Cast back to an int.
   pixel = static_cast<ossim_int32>(p);
}

inline void ossimImageData::getPixFromNorm(ossim_int32& pixel,
                                           ossim_float64 normPix,
                                           ossim_uint32 band) const
{
   // If norm pix is 0.0 or negative make if null.
   ossim_float64 p = theNullPixelValue[band]; 
   
   if (normPix > 0.0)
   {
      // un-normalize...
      p = normPix * (theMaxPixelValue[band]-theMinPixelValue[band]+1.0);
      
      // handle negative min values.
      p = p + theMinPixelValue[band] - 1.0;  
      
      // Clamp to  max.
      if (p > theMaxPixelValue[band]) p = theMaxPixelValue[band];
   
      //***
      // If p is less than min, make it min as it wasn't a null before.
      // Add or subtract 0.5 to p otherwise as it will be cast to an
      // int for proper rounding.
      //***
      p = p > theMinPixelValue[band] ?
          ( p<0.0 ? p-0.5 : p+0.5 ) :
          theMinPixelValue[band];
   }

   // Cast back to an int.
   pixel = static_cast<ossim_int32>(p);
}

inline void ossimImageData::getNormFromPix(ossim_int32 pixel,
                                           ossim_float32&  normPix,
                                           ossim_uint32 band) const
{
   if      (pixel == theNullPixelValue[band]) normPix = 0.0;
   else if (pixel >= theMaxPixelValue[band])  normPix = 1.0;

   //***
   // Normalize...
   // Note that this will shift any negatives to positive prior to dividing.
   //***
   else normPix = ( (pixel-theMinPixelValue[band]+1) /
                    (theMaxPixelValue[band] - theMinPixelValue[band] + 1.0) );
}

inline void ossimImageData::getNormFromPix(ossim_int32  pixel,
                                           ossim_float64&   normPix,
                                           ossim_uint32 band) const
{
   if      (pixel == theNullPixelValue[band]) normPix = 0.0;
   else if (pixel >= theMaxPixelValue[band])  normPix = 1.0;

   //***
   // Normalize...
   // Note that this will shift any negatives to positive prior to dividing.
   //***
   else normPix = ( (pixel-theMinPixelValue[band]+1) /
                    (theMaxPixelValue[band] - theMinPixelValue[band] + 1.0) );
}

inline ossimIrect ossimImageData::getImageRectangle() const
{
   ossimIpt lr(theOrigin.x + getWidth()  - 1,
               theOrigin.y + getHeight() - 1);
   
   return ossimIrect(theOrigin, lr);
}

inline const ossim_float64* ossimImageData::getNullPix()const
{
   if (theNullPixelValue.size() > 0)
   {
      return &theNullPixelValue.front();
   }
   return 0;
}

inline const ossim_float64* ossimImageData::getMinPix()const
{
   if (theMinPixelValue.size() > 0)
   {
      return &theMinPixelValue.front();
   }
   return 0;
}

inline const ossim_float64* ossimImageData::getMaxPix()const
{
   if (theMaxPixelValue.size() > 0)
   {
      return &theMaxPixelValue.front();
   }
   return 0;
}

inline ossim_float64 ossimImageData::getNullPix(ossim_uint32 band) const
{
   return theNullPixelValue[band];
}

inline ossim_float64 ossimImageData::getMinPix(ossim_uint32 band) const
{
   return theMinPixelValue[band];
}

inline ossim_float64 ossimImageData::getMaxPix(ossim_uint32 band) const
{
   return theMaxPixelValue[band];
}

inline ossim_uint32 ossimImageData::getHashId()const
{
   const unsigned char *bufx = (unsigned char*)(&theOrigin.x);
   const unsigned char *bufy = (unsigned char*)(&theOrigin.y);
   return (ossim_uint32)(bufx[0]*101 + bufx[1]*103 +
                         bufx[2]*107 + bufx[3]*109 +
                         bufy[0]*139 + bufy[1]*149 +
                         bufy[2]*151 + bufy[3]*157);
}

#endif
