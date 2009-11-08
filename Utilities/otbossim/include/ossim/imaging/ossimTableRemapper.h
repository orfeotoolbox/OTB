//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  David Burken
//
// Description:
//
// Table remapper class declaration.
// 
// Takes input tile, remaps it through a table, then output tile in the desired
// scalar type.
//
// Two modes one that works on native remap tables, that being of the same
// scalar type (like ossim_uint8) of the input connection, and another that
// uses a normalized remap table (more scalar independent).
//
//*************************************************************************
// $Id: ossimTableRemapper.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimTableRemapper_HEADER
#define ossimTableRemapper_HEADER

#include <ossim/imaging/ossimImageSourceFilter.h>

class OSSIMDLLEXPORT ossimTableRemapper : public ossimImageSourceFilter
{
public:
   enum RemapTableType
   {
      UKNOWN     = 0,
      NATIVE     = 1,
      NORMALIZED = 2
   };

   /** default constructor */
   ossimTableRemapper();

   virtual ossimScalarType getOutputScalarType() const;
    
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tile_rect,
                                               ossim_uint32 resLevel=0);

   virtual void initialize();



   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
   
   virtual ostream& print(ostream& os) const;

   friend ostream& operator << (ostream& os,  const ossimTableRemapper& tr);

protected:
   /** virtual destructor */
   virtual ~ossimTableRemapper();

   /**
    * Method:  setTable
    * 
    * @param table      - ossim_uint8* to the table
    * @param bin_count  - number of bins in the table per band
    * @param band_count - number of bands in the table
    * @param table_type - Either ossimTableRemapper::NATIVE or
    *                     ossimTableRemapper::NORMALIZED  
    * @param own_table  - If true "this" object will destroy table on
    *                     destruction. (default == true)
    *
    * Note on table:
    * - It's alway passed in as an "ossim_uint8*" so that on destruction there
    *   are no complaints.  You can do the math, if your table is really of
    *   double data type it's:
    *   "new ossim_uint8[bin_count * band_count * sizeof(double)]"...
    * - For each band in the table:
    *   - Table index[0] should always be the null value.
    *   - Table index[1] should always be the min value.
    * 
    * Note on band_count:
    * - If the "band_count == 1", and the input source has more than one band,
    *   this implies that the same remap table will be used for all bands.
    *
    * Notes on RemapTableType:
    * - If you want speed, use NATIVE, in which case the remap table scalar
    *   type should match input source.
    * - If you want to be able to handle any scalar type with one table, use
    *   NORMALIZED, in which case, your table values should be
    *   between 0.0 and 1.0.
    *
    * Note on own_table flag:
    * - If (own_table == true) this object will delete table on destruction.
    * - If you want to use one table to remap many image chains you might
    *   build the table and manage the memory yourself; in which case, you
    *   would set the own_table false; else it will be de
    */
   virtual void setTable(const std::vector<ossim_uint8>&    table,
                         ossim_uint32    table_bin_count,
                         ossim_uint32    table_band_count,
                         RemapTableType  table_type,
                         ossimScalarType output_scalar_type);

   
   ossimRefPtr<ossimImageData> theTile;
   ossimRefPtr<ossimImageData> theTmpTile;
   std::vector<ossim_uint8>    theTable;
   ossim_float64*  theNormBuf;
   ossim_uint32    theTableBinCount;
   ossim_uint32    theTableBandCount;
   RemapTableType  theTableType;
   ossimScalarType theInputScalarType;
   ossimScalarType theOutputScalarType;
   
   
   void allocate(const ossimIrect& rect);
   void destroy();

   void remapFromNativeTable(ossimRefPtr<ossimImageData>& inputTile);

   template <class T> void remapFromNativeTable(
      T dummy,
      ossimRefPtr<ossimImageData>& inputTile);

   void remapFromNormalizedTable(ossimRefPtr<ossimImageData>& inputTile);

   template <class T> void dumpTable(T dummy, ostream& os) const;

   // Do not allow copy constructor, operator=.
   ossimTableRemapper(const ossimTableRemapper& tr);
   ossimTableRemapper& operator=(const ossimTableRemapper& tr);
   
   TYPE_DATA
};

#endif  /* #ifndef ossimTableRemapper_HEADER */
