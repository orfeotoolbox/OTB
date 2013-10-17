//*****************************************************************************
// FILE: ossimInit.cpp
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// DESCRIPTION:
//   Contains implementation of class ossimInit. This object handles all
//   aspects of initialization for OSSIM applications. These tasks include:
//
//      1. Parsing the command line.
//      2. Instantiating all class factories.
//      3. Initializing the "trace" code execution tracing functionality.
//      4. Scanning the preferences file for relevant values.
//
// SOFTWARE HISTORY:
//   24Apr2001  Oscar Kramer
//              Initial coding.
//*****************************************************************************
// $Id: ossimInit.cpp 22278 2013-06-08 01:19:14Z gpotts $


#include <ossim/init/ossimInit.h>
#include <ossim/ossimVersion.h>
#include <ossim/base/ossimPreferences.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimArgumentParser.h>
#include <ossim/base/ossimApplicationUsage.h>
#include <ossim/base/ossimTraceManager.h>
#include <algorithm>
#include <ossim/base/ossimEnvironmentUtility.h>
#include <ossim/base/ossimGeoidEgm96.h>
//***
// Define Trace flags for use within this file:
//***
#include <ossim/base/ossimTrace.h>

// include the image file formats
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <ossim/imaging/ossimImageSourceFactoryRegistry.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/base/ossim2dTo2dTransformRegistry.h>
#include <ossim/imaging/ossimImageGeometryRegistry.h>
#include <ossim/elevation/ossimElevManager.h>
#include <ossim/base/ossimGeoidManager.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimOverviewBuilderFactoryRegistry.h>
#include <ossim/imaging/ossimOverviewBuilderFactory.h>

#include <ossim/imaging/ossimImageWriterFactoryRegistry.h>
#include <ossim/imaging/ossimImageMetaDataWriterRegistry.h>
#include <ossim/projection/ossimProjectionViewControllerFactory.h>
#include <ossim/base/ossimDatumFactoryRegistry.h>
#include <ossim/base/ossimBaseObjectFactory.h>
#include <ossim/base/ossimCustomEditorWindowRegistry.h>
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/plugin/ossimSharedPluginRegistry.h>
#include <ossim/plugin/ossimDynamicLibrary.h>
#include <ossim/font/ossimFontFactoryRegistry.h>
#include <ossim/base/ossimNotifyContext.h>


static ossimTrace traceExec = ossimTrace("ossimInit:exec");
static ossimTrace traceDebug = ossimTrace("ossimInit:debug");

ossimInit* ossimInit::theInstance = 0;

ossimInit::~ossimInit()
{
   theInstance = 0;
}

ossimInit::ossimInit()
    :
       theInitializedFlag(false),
       theAppName(),
       thePreferences(ossimPreferences::instance()),
       theElevEnabledFlag(true),
       thePluginLoaderEnabledFlag(true)
{
}

ossimInit* ossimInit::instance()
{
   static OpenThreads::Mutex m;
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m);
   if (!theInstance)
   {
      theInstance = new ossimInit();
   }
   return theInstance;
}

void ossimInit::addOptions(ossimArgumentParser& parser)
{
   parser.getApplicationUsage()->addCommandLineOption("-P", "specify a preference file to load");
   parser.getApplicationUsage()->addCommandLineOption("-K", "specify individual keywords to add to the preferences keyword list: name=value");
   parser.getApplicationUsage()->addCommandLineOption("-T", "specify the classes to trace, ex: ossimInit|ossimImage.* \nwill trace ossimInit and all ossimImage classes");
   parser.getApplicationUsage()->addCommandLineOption("--disable-elev", "Will disable the elevation");
   parser.getApplicationUsage()->addCommandLineOption("--disable-plugin", "Will disable the plugin loader");
   
   parser.getApplicationUsage()->addCommandLineOption("--ossim-logfile", "takes a logfile as an argument.  All output messages are redirected to the specified log file.  By default there is no log file and all messages are enabled.");
   parser.getApplicationUsage()->addCommandLineOption("--disable-notify", "Takes an argument. Arguments are ALL, WARN, NOTICE, INFO, FATAL, DEBUG.  If you want multiple disables then just do multiple --disable-notify on the command line.  All argument are case insensitive.  Default is all are enabled.");
   parser.getApplicationUsage()->addCommandLineOption("-V or --version", "Display version information.");
}

/*!****************************************************************************
 * METHOD: ossimInit::initialize()
 *  
 *  Method called from the OSSIM application main.
 *  
 *****************************************************************************/
void ossimInit::initialize(int& argc, char** argv)
{
    static OpenThreads::Mutex m;
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m);
  if( !theInitializedFlag )
   {
      ossimArgumentParser argumentParser(&argc, argv);
      theInstance->initialize(argumentParser);
   }
}

void ossimInit::initialize(ossimArgumentParser& parser)
{
   static OpenThreads::Mutex m;
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m);
   if(theInitializedFlag)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimInit::initialize(parser):"
            << " Already initialized, returning......"
            << std::endl;
      }
      return;
   }

   theInstance->theAppName  = parser.getApplicationUsage()->getApplicationName();
   theInstance->parseNotifyOption(parser);
   theInstance->thePreferences = ossimPreferences::instance();
      
   //Parse the command line:
   theInstance->parseOptions(parser);

   theInstance->initializeDefaultFactories();
   
   if ( theElevEnabledFlag )
   {
      theInstance->initializeElevation();
   }

   theInstance->initializeLogFile();
   
   if(thePluginLoaderEnabledFlag)
   {
      theInstance->initializePlugins();
   }
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossim preferences file: "
         << theInstance->thePreferences->getPreferencesFilename()
         << "\nVersion: " << version()
         << "\nossimInit::initialize(parser): leaving..." << std::endl;
   }
   
   theInitializedFlag = true;
}

void ossimInit::initialize()
{
    static OpenThreads::Mutex m;
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m);
   if(theInitializedFlag)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimInit::initialize(): Already initialized, returning......" << std::endl;
      }
      return;
   }

   theInstance->theAppName  = "";
   theInstance->thePreferences = ossimPreferences::instance();
   theInstance->initializeDefaultFactories();
   
   if ( theElevEnabledFlag )
   {
      theInstance->initializeElevation();
   }

   theInstance->initializeLogFile();

   //---
   // To do:
   // We need a mechanism to register factories to the "front" or the
   // "back" of factory list so that plugins can override things.  For
   // now we will initialize the plugins last...
   //---
   if(thePluginLoaderEnabledFlag)
   {
      theInstance->initializePlugins();
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossim preferences file: "
         << theInstance->thePreferences->getPreferencesFilename()
         << "\nVersion: " << version()
         << "\nossimInit::initialize() leaving..."
         << std::endl;
   } 
   
   theInitializedFlag = true;
}

void ossimInit::finalize()
{
   
}
/*!****************************************************************************
 *  Prints to stdout the list of command line options that this object parses.
 *****************************************************************************/
void ossimInit::usage()
{
   ossimNotify(ossimNotifyLevel_INFO)
      << "INFORMATION ossimInit::usage():\n"
      << "Additional command-line options available are as follows: "
      << "\n"
      << "\n  -P<pref_filename> -- Allows the user to override the loading "
      << "\n    of the default preferences with their own pref file."
      << "\n"
      << "\n  -K<keyword>[=<value>] -- Allows the user to specify additional"
      << "\n    keyword/value pairs that are added onto the preferences "
      << "\n    previously loaded. Keywords specified here override those in"
      << "\n    the preferences file."
      << "\n"
      << "\n  -T<trace_tag> -- Lets user turn on specific trace flags."
      << "\n"
      << "\n  -S<session_filename> -- Allows user to specify a session file"
      << "\n    to load."
      << "\n  -V or --version -- Outputs version information."
      << std::endl;
   return;
}

bool ossimInit::getElevEnabledFlag() const
{
   return theElevEnabledFlag;
}

void ossimInit::setElevEnabledFlag(bool flag)
{
   theElevEnabledFlag = flag; 
}

void ossimInit::setPluginLoaderEnabledFlag(bool flag)
{
   thePluginLoaderEnabledFlag = flag;  
}

void ossimInit::loadPlugins(const ossimFilename& plugin, const char* options)
{
   if(!thePluginLoaderEnabledFlag) return;

   if(plugin.exists())
   {
      if(plugin.isDir())
      {
         ossimDirectory dir;
         if(dir.open(plugin))
         {
            ossimFilename file;
            
            if(dir.getFirst(file,
                            ossimDirectory::OSSIM_DIR_FILES))
            {
               do
               { 
                  ossimSharedPluginRegistry::instance()->registerPlugin(file, options);
               }
               while(dir.getNext(file));
            }
         }
      }
      else
      {
         ossimSharedPluginRegistry::instance()->registerPlugin(plugin, options);
      }
   }
}

void ossimInit::parseOptions(ossimArgumentParser& parser)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG ossimInit::parseOptions: entering..." << std::endl;
   
   std::string tempString;
   ossimArgumentParser::ossimParameter stringParameter(tempString);
   while(parser.read("-P", stringParameter));

   if(tempString != "")
   {
      thePreferences->loadPreferences(ossimFilename(tempString));
   }
   while(parser.read("-K", stringParameter))
   {
      ossimString option = tempString;
      if (option.contains("=") )
      {
         ossimString delimiter = "=";
         ossimString key (option.before(delimiter));
         ossimString value = option.after(delimiter);
         thePreferences->addPreference(key.c_str(), value.c_str());
      }
      else
      {
         ossimString key (option);
         thePreferences->addPreference(key, "");
      }
   }

   while(parser.read("-T", stringParameter))
   {
      ossimTraceManager::instance()->setTracePattern(ossimString(tempString));
   }

   while(parser.read("--ossim-logfile", stringParameter))
   {
      ossimSetLogFilename(ossimFilename(tempString));
   }
   while(parser.read("--disable-notify", stringParameter))
   {
      ossimString tempDownCase = tempString;
      tempDownCase = tempDownCase.downcase();

      if(tempDownCase == "warn")
      {
         ossimDisableNotify(ossimNotifyFlags_WARN);
      }
      else if(tempDownCase == "fatal")
      {
         ossimDisableNotify(ossimNotifyFlags_FATAL);
      }
      else if(tempDownCase == "debug")
      {
         ossimDisableNotify(ossimNotifyFlags_DEBUG);
      }
      else if(tempDownCase == "info")
      {
         ossimDisableNotify(ossimNotifyFlags_INFO);
      }
      else if(tempDownCase == "notice")
      {
         ossimDisableNotify(ossimNotifyFlags_NOTICE);
      }
      else if(tempDownCase == "all")
      {
         ossimDisableNotify(ossimNotifyFlags_ALL);
      }
   }
   if(parser.read("--disable-elev"))
   {
      theElevEnabledFlag = false;
   }
   if(parser.read("--disable-plugin"))
   {
      thePluginLoaderEnabledFlag = false;
   }
   if (parser.read("--version") || parser.read("-V")) 
   {
      ossimNotify(ossimNotifyLevel_NOTICE)
         << "\n" << parser.getApplicationName().c_str() << " " << version() << std::endl;
   }
}

void ossimInit::parseNotifyOption(ossimArgumentParser& parser)
{
   std::string tempString;
   ossimArgumentParser::ossimParameter stringParameter(tempString);
   while(parser.read("--disable-notify", stringParameter))
   {
      ossimString tempDownCase = tempString;
      tempDownCase = tempDownCase.downcase();

      if(tempDownCase == "warn")
      {
         ossimDisableNotify(ossimNotifyFlags_WARN);
      }
      else if(tempDownCase == "fatal")
      {
         ossimDisableNotify(ossimNotifyFlags_FATAL);
      }
      else if(tempDownCase == "debug")
      {
         ossimDisableNotify(ossimNotifyFlags_DEBUG);
      }
      else if(tempDownCase == "info")
      {
         ossimDisableNotify(ossimNotifyFlags_INFO);
      }
      else if(tempDownCase == "notice")
      {
         ossimDisableNotify(ossimNotifyFlags_NOTICE);
      }
      else if(tempDownCase == "all")
      {
         ossimDisableNotify(ossimNotifyFlags_ALL);
      }
   }
}

/*!****************************************************************************
 * METHOD: ossimInit::removeOption()
 *  
 * Removes all characters associated with the indicated option from the
 * command line string.
 *  
 *****************************************************************************/
void ossimInit::removeOption(int& argc,
                             char** argv,
                             int argToRemove)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG ossimInit::removeOption(argc, argv, argToRemove): entering..."
      << std::endl;
   
   // Shift the args up by one position, overwriting the arg being removed:
   for (int i=argToRemove+1; i<argc;  i++)
   {
      argv[i - 1] = argv[i];
   }
   
   argc--;
   argv[argc] = 0;
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG ossimInit::removeOption(argc, argv, argToRemove): leaving..."
      << std::endl;
   return;
}

void ossimInit::initializeDefaultFactories()
{
   ossimObjectFactoryRegistry::instance()->registerFactory(ossimImageSourceFactoryRegistry::instance());

   //---
   // Because of how the imagehandlers work off a magic number make sure
   // we place the writer first if we don't then the imagehandler will
   // be false and will then try to open the filename and go through a
   // magic number and if the file already
   // existed it will open and create a handler instead of a writer.
   //---
   ossimImageWriterFactoryRegistry::instance();
   ossimDatumFactoryRegistry::instance();
   ossimImageMetaDataWriterRegistry::instance();
   ossimImageHandlerRegistry::instance();
   ossim2dTo2dTransformRegistry::instance();
   ossimImageGeometryRegistry::instance();
   // Initialize the overview factories.
   ossimOverviewBuilderFactoryRegistry::instance()->
      registerFactory(ossimOverviewBuilderFactory::instance(), true);
   
   ossimObjectFactoryRegistry::instance()->addFactory(ossimBaseObjectFactory::instance());
   
   // initialize. projection factory.
   ossimProjectionFactoryRegistry::instance();

   // add the view controllers
   ossimObjectFactoryRegistry::instance()->registerFactory(ossimProjectionViewControllerFactory::instance());

   ossimFontFactoryRegistry::instance();
}

void ossimInit::initializePlugins()
{      
   ossimString regExpressionDir =  ossimString("^(") + "plugin.dir[0-9]+)";
   ossimString regExpressionFile =  ossimString("^(") + "plugin.file[0-9]+)";

   const ossimKeywordlist& kwl = thePreferences->preferencesKWL();

   vector<ossimString> keys = kwl.getSubstringKeyList( regExpressionDir );

   ossim_uint32 numberOfDirs = (ossim_uint32)keys.size();
   ossim_uint32 offset = (ossim_uint32)ossimString("plugin.dir").size();
   int idx = 0;
   
   std::vector<int> numberList(numberOfDirs);
   
   // register user plugins first
   ossimFilename userPluginDir = ossimEnvironmentUtility::instance()->getUserOssimPluginDir();
   loadPlugins(userPluginDir);

   if(numberList.size()>0)
   {
      for(idx = 0; idx < (int)numberList.size();++idx)
      {
         ossimString numberStr(keys[idx].begin() + offset,
                               keys[idx].end());
         numberList[idx] = numberStr.toInt();
      }
      
      std::sort(numberList.begin(), numberList.end());
      for(idx=0;idx < (int)numberList.size();++idx)
      {
         ossimString newPrefix = "plugin.dir";
         newPrefix += ossimString::toString(numberList[idx]);
         const char* directory = kwl.find(newPrefix.c_str());
         
         if(directory)
         {
            loadPlugins(ossimFilename(directory));
         }
      }
   }
   keys = kwl.getSubstringKeyList( regExpressionFile );
   
   ossim_uint32 numberOfFiles = (ossim_uint32)keys.size();
   offset = (ossim_uint32)ossimString("plugin.file").size();
   numberList.resize(numberOfFiles);
   if(numberList.size()>0)
   {
      for(idx = 0; idx < (int)numberList.size();++idx)
      {
         ossimString numberStr(keys[idx].begin() + offset,
                               keys[idx].end());
         numberList[idx] = numberStr.toInt();
      }
      
      std::sort(numberList.begin(), numberList.end());   
      for(idx=0;idx < (int)numberList.size();++idx)
      {
         ossimString newPrefix="plugin.file";
         newPrefix += ossimString::toString(numberList[idx]);
         const char* file = kwl.find(newPrefix.c_str());
         
         if(file&&ossimFilename(file).exists())
         {
            loadPlugins(file);
//             ossimSharedPluginRegistry::instance()->registerPlugin(file);
         }
      }
   }
   
   // now check new plugin loading that supports passing options to the plugins
   // 
   regExpressionFile =  ossimString("^(") + "plugin[0-9]+\\.file)";
   keys = kwl.getSubstringKeyList( regExpressionFile );
   
   numberOfFiles = (ossim_uint32)keys.size();
   offset = (ossim_uint32)ossimString("plugin").size();
   numberList.resize(numberOfFiles);
   
   if(numberList.size()>0)
   {
      for(idx = 0; idx < (int)numberList.size();++idx)
      {
         std::vector<ossimString> splitArray;
         keys[idx].split(splitArray, ".");
         if(splitArray.size())
         {
            keys[idx] = ossimString(splitArray[0].begin(), splitArray[0].begin()+offset);
         }
         ossimString numberStr(splitArray[0].begin() + offset,
                               splitArray[0].end());
         numberList[idx] = numberStr.toInt();
      }
      
      std::sort(numberList.begin(), numberList.end());   
      for(idx=0;idx < (int)numberList.size();++idx)
      {
         ossimString newPrefix = ossimString("plugin")+ossimString::toString(numberList[idx]) + ".";
         const char* file    = kwl.find((newPrefix+"file").c_str());
         const char* options = kwl.find((newPrefix+"options").c_str());
         if(file&&ossimFilename(file).exists())
         {
            loadPlugins(file, options);
         }
      }
   }

   ossimString auto_load_plugins(ossimPreferences::instance()->findPreference("ossim_init.auto_load_plugins"));
   
   if(auto_load_plugins.empty()) auto_load_plugins = "true";
   // now load any plugins not found in the keywordlist
   //
   // check for plugins in the current directory
   // and load them
   if(auto_load_plugins.toBool())
   {
      ossimFilename ossimpluginsDir = ossimFilename(theAppName).dirCat("ossimplugins");
      ossimDirectory currentDir(theAppName.path());

      if(ossimpluginsDir.exists())
      {
         currentDir = ossimpluginsDir;
      }
      std::vector<ossimFilename> result;
      currentDir.findAllFilesThatMatch(result, "ossim.*plugin.*", ossimDirectory::OSSIM_DIR_FILES);
      
      if(result.size())
      {
         ossim_uint32 idx = 0;
         for(idx = 0; idx < result.size(); ++idx)
         {
            ossimSharedPluginRegistry::instance()->registerPlugin(result[idx]);
         }
      }
   }
}

void ossimInit::initializeElevation()
{
   if (traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG ossimInit::initializeElevation(): Entered..." << std::endl;
   
   const ossimKeywordlist& KWL = thePreferences->preferencesKWL();

   ossimFilename appPath = theAppName.path();
   // look for bundled elevation and geoids
   {
	   ossimFilename geoid = appPath.dirCat("geoids");
	   geoid = geoid.dirCat("geoid1996");
	   geoid = geoid.dirCat("egm96.grd");
	   if(geoid.exists())
	   {
         ossimGeoid* geoidPtr = new ossimGeoidEgm96(geoid);

         if (geoidPtr->getErrorStatus() == ossimErrorCodes::OSSIM_OK)
         {
		    ossimGeoidManager::instance()->addGeoid(geoidPtr);
		 }
	   }
   }
   ossimGeoidManager::instance()->loadState(KWL);
   
   ossimFilename elevation = appPath.dirCat("elevation");
   if(elevation.exists())
   {
      ossimElevManager::instance()->loadElevationPath(elevation);
   }
   // lets do backward compatability here
   //
   ossimString regExpression =  ossimString("^(") + "elevation_source[0-9]+.)";
   vector<ossimString> keys =
   KWL.getSubstringKeyList( regExpression );
   if(!keys.empty())
   {
      ossimNotify(ossimNotifyLevel_WARN) << "Please specify elevation_source keywords with the new prefix\n"
                                         << "of elevation_manager.elevation_source....\n";
      thePreferences->preferencesKWL().addPrefixToKeysThatMatch("elevation_manager.", regExpression);
   }
   ossimElevManager::instance()->loadState(KWL, "elevation_manager.");
   
   if (traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG ossimInit::initializeElevation(): leaving..." << std::endl;
}

void ossimInit::initializeLogFile()
{
   //---
   // Do not set if already as --ossim-logfile take precidence over preferences
   // file.
   //---
   ossimFilename logFile;
   ossimGetLogFilename(logFile);

   if ( (logFile.size() == 0) && thePreferences )
   {
      const char* lookup =
         thePreferences->preferencesKWL().find("ossim.log.file");
      if (lookup)
      {
         logFile = lookup;
         ossimSetLogFilename(logFile);
      }
   }
}

ossimString ossimInit::version() const
{
   ossimString versionString;
#ifdef OSSIM_VERSION
   versionString += OSSIM_VERSION;
#else
   versionString += "Version ?.?.?";
#endif
   
   versionString += " ";

#ifdef OSSIM_BUILD_DATE
   versionString += OSSIM_BUILD_DATE;
#else
   versionString += "(yyyymmdd)";
#endif

   return versionString;
}

ossimFilename ossimInit::appName()const
{
   return theAppName;
}

ossimInit::ossimInit(const ossimInit& /* obj */ )
{}       

void ossimInit::operator=(const ossimInit& /* rhs */) const
{}
