//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Ken Melero
//         Orginally written by Jamie Moyers (jmoyers@geeks.com)
//         Adapted from the package KDEM.
// Description: This class provides some simple utilities for DEMs.
//
//********************************************************************
// $Id: ossimDemUtil.h 15307 2009-09-01 15:41:07Z dburken $

#ifndef ossimDemUtil_HEADER
#define ossimDemUtil_HEADER

#include <cstring>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>

class ossimFilename;

class ossimDemUtil
{
public:

   /**
    * @brief Does basic sanity checks to see if file is a dem.
    *
    * 1) Check extension for .dem
    *
    * 2) Look for file.omd (ossim meta data) file containing keyword
    *    "dem_type" with value of "usgs_dem".
    *
    * 3) Check 512 bytes and make sure there is no binary data.
    *
    * @note
    * There is a keyword list template stored in the templates directory:
    * "ossim/etc/templates/usgs_dem_template.kwl"
    *
    * @param file The file to check.
    *
    * @return true on success, false on error.
    */
   static bool isUsgsDem(const ossimFilename& file);

   /**
    *  Reads a single record from a DEM.
    *  Returns true if succesful. Returns false if EOF or error.
    */
   static bool getRecord(std::istream& s, std::string& strbuf, long reclength = 1024);

   /**
    *  Same as getRecord above. buf should be at least reclength+1 in size.
    */
   static bool getRecord(std::istream& s, char* buf, long reclength = 1024);

   static long getLong(char* const strbuf, // string to extract long from
                       long const startpos,  // starting position of field
                       long const width)     // width of field
      {
         char temp[1024];
         strncpy(temp,strbuf+startpos,width);
         temp[width] = '\0';
         return atol(temp);
      }

   static bool getDouble(std::string const& strbuf, // string to extract double from
                         long const startpos,  // starting position of field
                         long const width,     // width of field
                         double& val);         // value extracted from field.

private:

   ossimDemUtil();
};

#endif  // #ifndef ossimDemUtil_HEADER
