//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  Base class for overview builders.
//
//----------------------------------------------------------------------------
// $Id: ossimOverviewBuilderBase.h 10203 2007-01-09 15:42:22Z dburken $
#ifndef ossimOverviewBuilderBase_HEADER
#define ossimOverviewBuilderBase_HEADER

#include <ossim/base/ossimSource.h>
#include <ossim/base/ossimProcessInterface.h>
#include <ossim/base/ossimConnectableObjectListener.h>

#include <ossim/base/ossimRtti.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>

class ossimImageHandler;
class ossimFilename;


class OSSIM_DLL ossimOverviewBuilderBase
   :
      public ossimSource,
      public ossimProcessInterface,
      public ossimConnectableObjectListener
{
public:

   /** default constructor */
   ossimOverviewBuilderBase();

   /** virtual destructor */
   virtual ~ossimOverviewBuilderBase();

   /**
    * @brief Sets the input to the builder. (pure virtual)
    * 
    * @param imageSource The input to the builder.
    * 
    * @param bool youOwnItFlag If true this object own the imageSource
    * memory and will delete imageSource at destruction or change of
    * imageSource.
    * 
    * @return True on successful initializion, false on error.
    */
   virtual bool setInputSource(ossimImageHandler* imageSource,
                               bool youOwnItFlag) = 0;

   /**
    * @brief Sets the output file name. (pure virtual)
    * @prama file This will be the output file name like foo.ovr
    */
   virtual void setOutputFile(const ossimFilename& file)=0;

   /**
    * @brief Sets the overview output type. (pure virtual)
    * @param type This should be the string representing the type.  This method
    * will do nothing if type is not handled and return false.
    * @return true if type is handled, false if not.

    * @note Currently handled types are 
    */
   virtual bool setOverviewType(const ossimString& type)=0;

   /**
    * @brief Gets the overview type. (pure virtual)
    * @return The overview output type as a string. */
   virtual ossimString getOverviewType() const=0;

   /**
    * @brief Method to check if builder can handle type.
    * @return true if type is handled by builder, false if not.
    */
   virtual bool hasOverviewType(const ossimString& type) const;

   /**
    * @brief Method to populate class supported types. (pure virtual)
    * Example:
    * ossimTiffOverviewNearest
    * ossimTiffOverviewBox
    *
    * @param typeList List of ossimStrings to add to.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const=0;
 
   /**
    * @brief Builds the overviews. (pure virtual)
    *
    * @return true on success, false on error.
    */
   virtual bool execute()=0;

 
protected:

   /** for rtti stuff */
   TYPE_DATA
};

#endif /* End of "#ifndef ossimOverviewBuilderBase_HEADER" */
