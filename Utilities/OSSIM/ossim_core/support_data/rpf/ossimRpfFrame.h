//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
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
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: This class give the capability to access tiles from an
//              rpf file.
//
//********************************************************************
// $Id: ossimRpfFrame.h,v 1.3 2003/06/15 19:01:34 dburken Exp $
#ifndef ossimRpfFrame_HEADER
#define ossimRpfFrame_HEADER
#include <vector>
using namespace std;

#include "base/data_types/ossimFilename.h"
#include "base/context/ossimErrorContext.h"
#include "ossimRpfColorGrayscaleTable.h"
#include "ossimRpfConstants.h"

class ossimRpfHeader;
class ossimRpfAttributes;
class ossimRpfCoverageSection;
class ossimRpfImageDescriptionSubheader;
class ossimRpfImageDisplayParameterSubheader;
class ossimRpfMaskSubheader;
class ossimRpfCompressionSection;
class ossimRpfColorGrayscaleSubheader;
class ossimRpfColorConverterSubsection;

class ossimRpfFrame
{
public:
   friend ostream& operator <<(ostream& out,
                               const ossimRpfFrame& data);
   ossimRpfFrame();
   virtual ~ossimRpfFrame();
   
   void print(ostream& out)const;

   ossimErrorCode parseFile(const ossimFilename& filename);
   const ossimRpfHeader* getRpfHeader()const{return theHeader;}

   bool hasSubframeMaskTable()const;

   const vector< vector<ossim_uint32> >& getSubFrameMask(ossim_uint32 spectralGroup)
      {
         return theSubframeMaskTable[spectralGroup];
      }
   
   bool fillSubFrameBuffer(ossim_uint8* buffer,
                           ossim_uint32 spectralGroup,
                           ossim_uint32 row,
                           ossim_uint32 col)const;
   
   const ossimRpfCompressionSection* getCompressionSection()const
      {
         return theCompressionSection;
      }
   const vector<ossimRpfColorGrayscaleTable>& getColorGrayscaleTable()const
      {
         return theColorGrayscaleTable;
      }
   const ossimRpfColorConverterSubsection* getColorConverterSubsection()const
      {
         return theColorConverterSubsection;
      }
private:
   void clearFields();
   void deleteAll();
   ossimErrorCode populateCoverageSection(istream& in);
   ossimErrorCode populateCompressionSection(istream& in);
   ossimErrorCode populateImageSection(istream& in);
   ossimErrorCode populateAttributeSection(istream& in);
   ossimErrorCode populateColorGrayscaleSection(istream& in);
   ossimErrorCode populateMasks(istream& in);

   /*!
    * The header will be instantiated during the opening of the
    * frame file.
    */
   ossimRpfHeader *theHeader;

   /*!
    * The filename is set if its a successful open.
    */
   ossimFilename   theFilename;

   /*!
    * This will hold the coverage.
    */
   ossimRpfCoverageSection* theCoverage;

   /*!
    * This will hold the attributes of this frame.
    */
   ossimRpfAttributes *theAttributes;

   /*!
    * This is the subheader for the image.  It will have
    * some general information about the image.
    */
   ossimRpfImageDescriptionSubheader* theImageDescriptionSubheader;

   /*!
    * This is the mask subheader.
    */
   ossimRpfMaskSubheader* theMaskSubheader;
   
   /*!
    * This is the subheader for the display parameters
    * for this frame.
    */
   ossimRpfImageDisplayParameterSubheader* theImageDisplayParameterSubheader;

   /*!
    * Will hold a pointer to the compression information section.
    */
   ossimRpfCompressionSection*             theCompressionSection;
   
   /*!
    * If present, it will hold the color grayscale subheader.
    */
   ossimRpfColorGrayscaleSubheader*        theColorGrayscaleSubheader;

   /*!
    * 
    */
   ossimRpfColorConverterSubsection*       theColorConverterSubsection;

   /*!
    * 
    */
   vector<ossimRpfColorGrayscaleTable>     theColorGrayscaleTable;


   /*!
    * We have a 3-D array.  For the most part the numberof spectral groups
    * should be one.  The indexes are as follows:
    *
    * theSubframeMasKTable[spectralGroup][row][col]
    *
    *  It will hold RPF_NULL(0xffffffff) if the subframe does not exist
    *  and it will hold an offset value from the start of the
    * [spatial data section] to the first byte of the subframe table.
    */
   vector< vector< vector< ossim_uint32> > > theSubframeMaskTable;

   /*!
    * We have a 3-D array.  For the most part the numberof spectral groups
    * should be one.  The indexes are as follows:
    *
    * theSubframeTransparencyMaskTable[spectralGroup][row][col]
    *
    *  It will hold RPF_NULL(0xffffffff) if the subframe does not exist
    *  and it will hold an offset value from the start of the
    * [spatial data section] to the first byte of the subframe table.
    */
   vector< vector< vector< ossim_uint32> > > theSubframeTransparencyMaskTable;
};

#endif
