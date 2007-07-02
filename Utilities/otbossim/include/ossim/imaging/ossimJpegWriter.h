//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
//
// Contains class definition for JpegWriter.
//
//*******************************************************************
//  $Id: ossimJpegWriter.h 9817 2006-10-30 12:35:40Z dburken $
#ifndef ossimJpegWriter_HEADER
#define ossimJpegWriter_HEADER

#include <ossim/imaging/ossimImageFileWriter.h>
#include <ossim/base/ossimKeywordlist.h>

//*******************************************************************
// CLASS:  ossimJpegWriterFactory
//*******************************************************************

class OSSIMDLLEXPORT ossimJpegWriter : public ossimImageFileWriter
{
public:

   /** default constructor */
   ossimJpegWriter();

   /** virtual destructor */
   virtual ~ossimJpegWriter();

   /**
    * void getImageTypeList(std::vector<ossimString>& imageTypeList)const
    *
    * Appends this writer image types to list "imageTypeList".
    *
    * This writer only has one type "jpeg".
    *
    * @param imageTypeList stl::vector<ossimString> list to append to.
    */
   virtual void getImageTypeList(std::vector<ossimString>& imageTypeList)const;
   
   /**
    * Sets the jpeg compression quality.
    *
    * @param quality Compression quality.  (valid range is 1 to 100)
    */
   virtual void  setQuality(ossim_int32 quality);
   
   virtual bool isOpen()const;   
   
   virtual bool open();

   virtual void close();
   
   /**
    * saves the state of the object.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   /**
    * Will set the property whose name matches the argument
    * "property->getName()".
    *
    * @param property Object containing property to set.
    */
   virtual void setProperty(ossimRefPtr<ossimProperty> property);

   /**
    * @param name Name of property to return.
    * 
    * @returns A pointer to a property object which matches "name".
    */
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;

   /**
    * Pushes this's names onto the list of property names.
    *
    * @param propertyNames array to add this's property names to.
    */
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   bool hasImageType(const ossimString& imageType) const;

   
private:
   virtual bool writeFile();
   
   ossim_int32 theQuality;
   bool        theOverviewFlag;
   FILE*       theOutputFilePtr;
   
TYPE_DATA
};

#endif /* #ifndef ossimJpegWriter_HEADER */
