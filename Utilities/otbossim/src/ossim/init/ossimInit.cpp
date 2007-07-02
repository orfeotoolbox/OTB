//*****************************************************************************
// FILE: ossimInit.cc
//
// Copyright (C) 2001 ImageLinks, Inc.
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
// $Id: ossimInit.cpp 9922 2006-11-21 23:11:33Z dburken $

#include <iostream>
using namespace std;
#include <ossim/init/ossimInit.h>
#include <ossim/base/ossimPreferences.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimArgumentParser.h>
#include <ossim/base/ossimApplicationUsage.h>
#include <ossim/base/ossimTraceManager.h>
#include <algorithm>
#include <ossim/base/ossimEnvironmentUtility.h>

//***
// Define Trace flags for use within this file:
//***
#include <ossim/base/ossimTrace.h>

// include the image file formats
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <ossim/imaging/ossimImageSourceFactoryRegistry.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/elevation/ossimElevManager.h>
#include <ossim/base/ossimGeoidManager.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimOverviewBuilderFactoryRegistry.h>
#include <ossim/imaging/ossimOverviewBuilderFactory.h>

#include <ossim/imaging/ossimImageWriterFactoryRegistry.h>
#include <ossim/imaging/ossimImageMetaDataWriterRegistry.h>
#include <ossim/projection/ossimProjectionViewControllerFactory.h>
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

ossimInit* ossimInit::theInstance = NULL;

ossimInit::~ossimInit()
{
   theInstance = NULL;
}

ossimInit::ossimInit()
    :
       theAppName(),
       thePreferences(ossimPreferences::instance()),
       theElevEnabledFlag(true),
       thePluginLoaderEnabledFlag(true)
{
}

ossimInit* ossimInit::instance()
{
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
   
}

/*!****************************************************************************
 * METHOD: ossimInit::initialize()
 *  
 *  Method called from the OSSIM application main.
 *  
 *****************************************************************************/
void ossimInit::initialize(int& argc, char** argv)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG ossimInit::initialize(argc, argv): entering..." << std::endl;

   theInstance->theAppName  = argv[0];

   theInstance->thePreferences = ossimPreferences::instance();

   // Parse the command line:
   theInstance->parseOptions(argc, argv);

   theInstance->initializeDefaultFactories();
   theInstance->initializeElevation();
   if(thePluginLoaderEnabledFlag)
   {
      theInstance->initializePlugins();
   }
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG ossimInit::initialize(argc, argv): leaving..." << std::endl;
   return;
}

void ossimInit::initialize(ossimArgumentParser& parser)
{
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG ossimInit::initialize(parser): entering..." << endl;
   }

   theInstance->theAppName  = parser.getApplicationUsage()->getApplicationName();
   theInstance->thePreferences = ossimPreferences::instance();
      
    //Parse the command line:
   theInstance->parseOptions(parser);

   theInstance->initializeDefaultFactories();
   theInstance->initializeElevation();
   if(thePluginLoaderEnabledFlag)
   {
      theInstance->initializePlugins();
   }

   if(!theElevEnabledFlag)
   {
      ossimElevManager::instance()->disableSource();
      ossimElevManager::instance()->disableAutoLoad();
   }
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG ossimInit::initialize(parser): leaving..." << endl;
   }
   return;
}

void ossimInit::initialize()
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG ossimInit::initialize(): entering..." << std::endl;

   theInstance->theAppName  = "";
   theInstance->thePreferences = ossimPreferences::instance();
   theInstance->initializeDefaultFactories();
   theInstance->initializeElevation();

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
   
   return;
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
      << std::endl;
   return;
}

void ossimInit::setPluginLoaderEnabledFlag(bool flag)
{
   thePluginLoaderEnabledFlag = flag;  
}


void ossimInit::parseOptions(ossimArgumentParser& parser)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG ossimInit::parseOptions: entering..." << endl;
   
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
}


/*!****************************************************************************
 * METHOD: ossimInit::parseOptions()
 *  
 *  Parses the command line options.
 *  
 *****************************************************************************/
void ossimInit::parseOptions(int& argc, char** argv)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG ossimInit::parseOptions(argc, argv): entering..." << std::endl;

   int currArgc = 1;
   while (currArgc < argc)
   {
      if(argv[currArgc][0] == '-')
      {
         switch (argv[currArgc][1])
         {
         case 'P':
         {
            /*!
             * OPTION_SYNTAX: -P<pref_filename>
             * This option permits replacing the default preferences file
             * with a different, user-specified keyword preference file:
             */
            ossimFilename pref_file = (&argv[currArgc][2]);
            thePreferences->loadPreferences(pref_file);
            removeOption (argc, argv, currArgc--);
            break;
         }

         case 'K':
         {
            /*!
             * OPTION SYNTAX: -K<keyword>[=<value>]
             * This option used to add a keyword to the already loaded
             * preferences KWL:
             */
            ossimString option = (&argv[currArgc][2]);
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
            removeOption (argc, argv, currArgc--);
            break;
         }
   
         case 'T':
         {
            /*!
             * OPTION SYNTAX: -T<trace_tag>
             * Turns on the trace flag for all trace objects matching
             * the trace_tag:
             */
            ossimString tag = (&argv[currArgc][2]);
            if (tag.empty())
            {
               // Someone did -T "some_tag" instead of -T"some_tag"
               removeOption (argc, argv, currArgc);
               tag = argv[currArgc];
            }
            ossimTraceManager::instance()->setTracePattern(tag);
            removeOption (argc, argv, currArgc--);
            break;
         }

         case 'S':
         {
            /*!
             * OPTION SYNTAX: -S<sesion_filename>
             * This option is specified for autoloading session KWL files:
             */
            ossimFilename session_file = (&argv[currArgc][2]);
            thePreferences->loadPreferences(session_file);
            removeOption (argc, argv, currArgc--);
            break;
         }

         default:
            // option unrecognized, skip
            break;
          }
      }

      currArgc++;
   }
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG ossimInit::parseOptions(argc, argv): leaving..." << std::endl;
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
   ossimImageMetaDataWriterRegistry::instance();
   ossimImageHandlerRegistry::instance();

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

   ossim_uint32 numberOfDirs = keys.size();
   ossim_uint32 offset = ossimString("plugin.dir").size();
   int idx = 0;
   
   std::vector<int> numberList(numberOfDirs);


   
   ossimFilename installedPluginDir = ossimEnvironmentUtility::instance()->getInstalledOssimPluginDir();
   if(installedPluginDir.exists())
   {
      ossimDirectory dir;
      if(dir.open(installedPluginDir))
      {
         ossimFilename file;

         if(dir.getFirst(file,
                         ossimDirectory::OSSIM_DIR_FILES))
         {
            do
            {
               ossimSharedPluginRegistry::instance()->registerPlugin(file);
            }
            while(dir.getNext(file));
         }
      }
   }
   
   // register user plugins first
   ossimFilename userPluginDir = ossimEnvironmentUtility::instance()->getUserOssimPluginDir();
   if(userPluginDir.exists())
   {
      ossimDirectory dir;
      if(dir.open(userPluginDir))
      {
         ossimFilename file;

         if(dir.getFirst(file,
                         ossimDirectory::OSSIM_DIR_FILES))
         {
            do
            {
               ossimSharedPluginRegistry::instance()->registerPlugin(file);
            }
            while(dir.getNext(file));
         }
      }
   }
   
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
            ossimDirectory d;
            
            if(d.open(ossimFilename(directory)))
            {
               ossimFilename file;
               
               if(d.getFirst(file, ossimDirectory::OSSIM_DIR_FILES))
               {
                  do
                  {
                     ossimSharedPluginRegistry::instance()->registerPlugin(file);
                  }while(d.getNext(file));
               }
            }
         }
      }
   }
   keys = kwl.getSubstringKeyList( regExpressionFile );
   
   ossim_uint32 numberOfFiles = keys.size();
   offset = ossimString("plugin.file").size();
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
            ossimSharedPluginRegistry::instance()->registerPlugin(file);
         }
      }
   }
}

void ossimInit::initializeElevation()
{
   if (traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG ossimInit::initializeElevation(): Entered..." << std::endl;
   
   const ossimKeywordlist& KWL = thePreferences->preferencesKWL();

   ossimGeoidManager::instance()->loadState(KWL);
   ossimElevManager::instance()->loadState(KWL);
   
   if (traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG ossimInit::initializeElevation(): leaving..." << std::endl;
}
