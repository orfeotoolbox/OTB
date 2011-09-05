//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// DESCRIPTION:
//   Contains declaration of class ossimPreferences. This class provides
//   a static keywordlist for global preferences. Objects needing access to
//   application-wide global parameters shall do so through this class.
//
// SOFTWARE HISTORY:
//>
//   23Apr2001  Oscar Kramer
//              Initial coding.
//<
//*****************************************************************************

#ifndef ossimPreferences_HEADER
#define ossimPreferences_HEADER

#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>

/*!****************************************************************************
 *
 * CLASS:  ossimPreferences
 *
 *****************************************************************************/
class OSSIMDLLEXPORT ossimPreferences
{
public:
   // This is only here so we can swig wrap this.  The destructor should never be called directly.
   ~ossimPreferences();
   /*!
    * METHOD: instance()
    * The static singleton instance of this object is accessed via this method:
    */
   static ossimPreferences* instance();

   /*!
    * METHOD: loadPreferences()
    * These methods clear the current preferences and load either the default
    * preferences file or the specified file. Returns TRUE if loaded properly:
    */
   bool loadPreferences();
   bool loadPreferences(const ossimFilename& pathname);

   /*!
    * METHOD: savePrefences()
    * This method permits saving the preferences file to the default location
    * or to a specified location:
    */
   bool savePreferences() const;
   bool savePreferences(const ossimFilename& pathname);

   /*!
    * METHOD: findPreference()
    * Performs a lookup for the specified keyword in the preferences KWL:
    */
   const char* findPreference(const char* key) const {return theKWL.find(key);}


   /*!
    * METHOD: addPreference()
    * Inserts keyword/value pair into the in-memory preferences KWL. It does
    * save to disk. App must do a savePreferences() for changes to be saved.
    */
   void addPreference(const char* key,
                      const char* value);

   /*!
    * METHOD: preferencesKWL()
    * An alternative to utilizing findPreference(), for objects derived from
    * ossimObject, is to access the preferences KWL with this method
    * (typically during construction) and provide it to the object's
    * loadState() method.
    */
   const ossimKeywordlist& preferencesKWL() const { return theKWL; }
   
   /*!
    * METHOD: preferencesKWL()
    * An alternative to utilizing findPreference(), for objects derived from
    * ossimObject, is to access the preferences KWL with this method
    * (typically during construction) and provide it to the object's
    * loadState() method.
    */
   ossimKeywordlist& preferencesKWL()  { return theKWL; }
   
   void addPreferences(const ossimKeywordlist& kwl,
                       const char* prefix=0,
                       bool stripPrefix=true);

   /** @return The preference filename. */ 
   ossimFilename getPreferencesFilename() const;
   
protected:
   /*!
    * Override the compiler default constructors:
    */
   ossimPreferences();
   ossimPreferences(const ossimPreferences&) {}

   void operator = (const ossimPreferences&) const {}

   static ossimPreferences* theInstance;
   ossimKeywordlist         theKWL;
   ossimFilename            thePrefFilename;
   mutable bool             theInstanceIsModified;
};

#endif
