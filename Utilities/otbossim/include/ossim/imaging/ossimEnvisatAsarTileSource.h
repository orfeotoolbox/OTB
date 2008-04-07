#ifndef ossimEnvisatAsarTileSource_H
#define ossimEnvisatAsarTileSource_H


#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/imaging/EnvisatAsar/EnvisatAsarData.h>

class ossimImageData;

/**
 * @brief This class is able to read an EnvisatAsar file structure
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 10-12-07
 */
class OSSIMDLLEXPORT ossimEnvisatAsarTileSource : public ossimImageHandler
{
public:
	ossimEnvisatAsarTileSource();
	
   virtual ~ossimEnvisatAsarTileSource();

   virtual ossimString getLongName()  const;
   virtual ossimString getShortName() const;
   
   /*!
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
   /*!
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Returns true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   /*!
    *  Returns true if the image_file can be opened and is a valid tiff file.
    */
  virtual bool open();
  virtual void close();
  virtual bool isOpen()const;
   /*!
     *  Returns the number of lines in the image.
     *  Satisfies pure virtual from ImageHandler class.
     */
   virtual ossim_uint32 getNumberOfLines(ossim_uint32 reduced_res_level = 0) const
   {
	   return 0;
   };

   /*!
    *  Returns the number of samples in the image.
    *  Satisfies pure virtual from ImageHandler class.
    */
   virtual ossim_uint32 getNumberOfSamples(ossim_uint32 reduced_res_level = 0) const
   {
	   return 0;
   };
   /*!
    * Returns the width of the output tile.
    */
   virtual ossim_uint32 getImageTileWidth() const
   {
	   return 0;
   };
   
   /*!
    * Returns the height of the output tile.
    */
   virtual ossim_uint32 getImageTileHeight() const
   {
	   return 0;
   };
   /*!
     *  Returns the number of bands in the image.
     *  Satisfies pure virtual from ImageHandler class.
     */
   virtual ossim_uint32 getNumberOfInputBands() const
   {
	   return 0;
   };
   
   /**
	* @brief Populates the keyword list with image geometry information
	*
	* This method is used to relay projection/model information to users.
	* @param kwl 	Keyword list that will be initialized with geometry info. Returns true if geometry info is present, false if not.
	* @param prefix 	The prefix is added to the resulting keyword list keywords. So is you if you pass in a prefix of "image01.", the keyword ul_lat will be like: "image01.ul_lat: -40.00000"
	*/
	bool getImageGeometry(ossimKeywordlist& kwl,const char* prefix = 0);

protected:
	/**
	 * @brief List of metadata contained in the Data file
	 */
	EnvisatAsarData * _EnvisatAsarData;

private:

	TYPE_DATA
};

#endif