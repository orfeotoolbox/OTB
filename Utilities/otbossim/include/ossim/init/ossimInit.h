//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// DESCRIPTION:
//   Contains declaration of class ossimInit. This object handles all aspects
//   of initialization for OSSIM applications. These tasks include:
//
//      1. Parsing the command line.
//      2. Instantiating all class factories as declared in ossimFactories.h
//      3. Initializing the "trace" code execution tracing functionality.
//      4. Scanning the preferences file for relevant values.
//
// SOFTWARE HISTORY:
//>
//   24Apr2001  Oscar Kramer
//              Initial coding.
//<
//*****************************************************************************
// $Id: ossimInit.h 19440 2011-04-25 16:41:28Z dburken $
#ifndef ossimInit_HEADER
#define ossimInit_HEADER 1

#include <ossim/base/ossimFilename.h>

class ossimPreferences;
class ossimArgumentParser;

class OSSIMDLLEXPORT ossimInit
{
public:

   ~ossimInit();
   /*!
    * Returns the static instance of an ossimInit object. This is of no use
    * until non-static methods are implemented.
    */
   static ossimInit* instance();

   void addOptions(ossimArgumentParser& parser);

   /*!
    * METHOD: initialize()
    * This method shall be called from the application's main module with
    * the command-line arguments. Every OSSIM application should have the
    * following line early in the main module:
    *
    *   ossimInit::initialize(argc, argv);
    *
    * OR
    *
    *   ossimInit::instance()->initialize(argc, argv);
    *
    * The two forms are functionally identical. Pick the latter form if you
    * like to type. The argv command line options are parsed and may be
    * stripped. the value of argc will be adjusted to account for stripped
    * options.
    */
   void initialize(int& argc, char** argv);

   void initialize(ossimArgumentParser& parser);
   
   void initialize();

   void finalize();

   /*!
    * METHOD: usage()
    * Prints to stdout the list of command line options that this object parses
    */
   void usage();

   /** @return theElevEnabledFlag */
   bool getElevEnabledFlag() const;

   /**
    * @brief Sets theElevEnabledFlag.
    * @param flag If true ossimElevManager will be initialized. Set to false
    * to NOT initialize the ossimElevManager from preferences.
    * Default in class is true.
    */
   void setElevEnabledFlag(bool flag);

   void setPluginLoaderEnabledFlag(bool flag);

   /**
    * Can take a file or a directory.  If a directory is givien then it will check all files in
    * the directory and add each file that is detected to be a valid plugin.
    *
    * @param plugin Is the filename of the plugin to load
    * @param options Is a keywordlist of plugin specific options
    
    */ 
   void loadPlugins(const ossimFilename& plugin, const char* options=0);
   
   void initializePlugins();
   void initializeDefaultFactories();
   void initializeElevation();

   /**
    * @brief Initializes log file from preferences keyword lookup
    * of "ossim.log.file" if log file has not already been set and
    * keyword is set in preferences.
    *
    * So this should be called after parse options as the --ossim-logfile
    * should override any preferences setting.
    */
   void initializeLogFile();

   /**
    * @return The version in the form of:
    * "version major.minor.release (yyyymmdd)"
    * where
    * yyyymmdd is the build date.
    *
    * e.g. "version 1.7.0 (20071003)"
    */
   ossimString version() const;

   ossimFilename appName()const;

protected:
   /** protected default constructor. */
   ossimInit();

   /** Hidden from use copy constructor. */
   ossimInit(const ossimInit& obj);
   
   /** Hidden from use assignment operator. */
   void operator=(const ossimInit& rhs) const;
   
   void parseOptions(ossimArgumentParser& parser);

   void parseNotifyOption(ossimArgumentParser& parser);
   /*!
    * METHOD: removeOptions()
    * Utility for stripping from argv all characters associated with a
    * particular option:
    */
   void removeOption(int&   argc, 
                     char** argv,
                     int    argToRemove);
   
   static ossimInit*  theInstance;
   bool               theInitializedFlag;  
   ossimFilename      theAppName;
   ossimPreferences*  thePreferences;
   bool               theElevEnabledFlag;
   bool               thePluginLoaderEnabledFlag;
};

#endif
