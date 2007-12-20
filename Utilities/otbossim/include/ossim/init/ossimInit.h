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
// $Id: ossimInit.h 11959 2007-10-31 19:22:56Z gpotts $
#ifndef ossimInit_HEADER
#define ossimInit_HEADER

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

   void setPluginLoaderEnabledFlag(bool flag);

   /**
    * Can take a file or a directory.  If a directory is givien then it will check all files in
    * the directory and add each file that is detected to be a valid plugin.
    *
    */ 
   void loadPlugins(const ossimFilename& plugin);
   void initializePlugins();
   void initializeDefaultFactories();
   void initializeElevation();

   /**
    * @return The version in the form of:
    * "version major.minor.release (yyyymmdd)"
    * where
    * yyyymmdd is the build date.
    *
    * e.g. "version 1.7.0 (20071003)"
    */
   ossimString version() const;
   ossimFilename appName()const
   {
	   return theAppName;
   }
protected:
   /** protected default constructor. */
   ossimInit();

   /** Hidden from use copy constructor. */
   ossimInit(const ossimInit& obj);
   
   /** Hidden from use assignment operator. */
   void operator=(const ossimInit& rhs) const;
   
   /*!
    * METHOD: parseOptions()
    * Parses the command line as passed in initialize. The options string is
    * modified:
    */
   void parseOptions(int& argc, char** argv);

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
