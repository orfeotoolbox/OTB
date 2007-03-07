//*****************************************************************************
// FILE: ossimInit.h
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
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
//   24Apr2001  Oscar Kramer (okramer@imagelinks.com)
//              Initial coding.
//<
//*****************************************************************************

#ifndef ossimInit_HEADER
#define ossimInit_HEADER

#include <base/data_types/ossimString.h>

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
   
protected:
   ossimInit();
   ossimInit(const ossimInit&) {}   

   /*!
    * METHOD: parseOptions()
    * Parses the command line as passed in initialize. The options string is
    * modified:
    */
   void parseOptions(int& argc, char** argv);

   void parseOptions(ossimArgumentParser& parser);
   
   /*!
    * METHOD: removeOptions()
    * Utility for stripping from argv all characters associated with a
    * particular option:
    */
   void removeOption(int&   argc, 
                     char** argv,
                     int    argToRemove);

   void initializeDefaultFactories();

   void initializePlugins();

   void initializeElevation();
   
   static ossimInit*  theInstance;
   ossimString        theAppName;
   ossimPreferences*  thePreferences;
   bool               theElevEnabledFlag;
   bool               thePluginLoaderEnabledFlag;
};

#endif
