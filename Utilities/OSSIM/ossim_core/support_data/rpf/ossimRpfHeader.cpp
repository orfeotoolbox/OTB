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
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfHeader.cpp,v 1.8 2004/06/17 17:40:29 gpotts Exp $
#include "ossimRpfHeader.h"
#include <string.h>
#include "base/misc/ossimEndian.h"
#include "base/data_types/ossimString.h"
#include "base/common/ossimErrorCodes.h"
#include "ossimRpfLocationSection.h"
#include "ossimRpfCoverageSection.h"
#include "ossimRpfBoundaryRectSectionSubheader.h"
#include "ossimRpfBoundaryRectTable.h"
#include "ossimRpfFrameFileIndexSectionSubheader.h"
#include "ossimRpfFrameFileIndexSubsection.h"
#include "ossimRpfImageDescriptionSubheader.h"
#include "ossimRpfMaskSubsection.h"
#include "ossimRpfAttributeSectionSubheader.h"
#include "ossimRpfImageDisplayParameterSubheader.h"
#include "ossimRpfCompressionSectionSubheader.h"
#include "ossimRpfCompressionSection.h"
#include "ossimRpfColorGrayscaleSubheader.h"

#ifndef NULL
#include <stddef.h>
#endif

ostream& operator <<(ostream &out, const ossimRpfHeader &data)
{
   data.print(out);

   return out;
}

ossimRpfHeader::~ossimRpfHeader()
{
   if(theLocationSection)
   {
      delete theLocationSection;
      theLocationSection = NULL;
   }
}
ossimRpfHeader::ossimRpfHeader()
   :theLittleBigEndianIndicator(true),
    theHeaderSectionLength(0),
    theNewRepUpIndicator(0),
    theLocSectionLoc(0),
    theLocationSection(NULL)
{
   theLocationSection = new ossimRpfLocationSection;
   
   memset(theFileName, ' ' , 12);
   memset(theGovSpecNumber, ' ', 15);
   memset(theGovSpecDate, ' ', 8);
   memset(theSecurityClassification, ' ', 1);
   memset(theCountryCode, ' ', 2);
   memset(theSecurityReleaseMarking, ' ', 2);

   theFileName[12] = '\0';
   theGovSpecNumber[15] = '\0';
   theGovSpecDate[8] = '\0';
   theSecurityClassification[1] = '\0';
   theCountryCode[2] = '\0';
   theSecurityReleaseMarking[2] = '\0';
}

ossimErrorCode ossimRpfHeader::parseStream(istream& in)
{   
   if(in)
   {      
      ossimEndian anEndian;

      in.read((char*)&theLittleBigEndianIndicator, 1);

      in.read((char*)&theHeaderSectionLength, 2);
      in.read((char*)theFileName, 12);
      in.read((char*)&theNewRepUpIndicator, 1);
      in.read((char*)theGovSpecNumber, 15);
      in.read((char*)theGovSpecDate, 8);
      in.read((char*)theSecurityClassification, 1);
      in.read((char*)theCountryCode, 2);
      in.read((char*)theSecurityReleaseMarking, 2);
      in.read((char*)&theLocSectionLoc, 4);

      theFileName[12] = '\0';
      theGovSpecNumber[15] = '\0';
      theGovSpecDate[8] = '\0';
      theSecurityClassification[1] = '\0';
      theCountryCode[2] = '\0';
      theSecurityReleaseMarking[2] = '\0';

      ossimByteOrder dataByteOrder = ((theLittleBigEndianIndicator==0xff)?OSSIM_LITTLE_ENDIAN:OSSIM_BIG_ENDIAN);

      if(anEndian.getSystemEndianType() != dataByteOrder)
      {

         anEndian.swap(theHeaderSectionLength);
         anEndian.swap(theLocSectionLoc);
      }

      long saveGet = in.tellg();
      in.seekg(theLocSectionLoc, ios::beg);
      theLocationSection->parseStream(in,
                                      dataByteOrder);
      in.seekg(saveGet, ios::beg);
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }

   return ossimErrorCodes::OSSIM_OK;
}

void ossimRpfHeader::print(ostream &out)const
{
   out << "theLittleBigEndianIndicator:               " << theLittleBigEndianIndicator << endl
       << "theHeaderSectionLength:                   " << theHeaderSectionLength     << endl
       << "theFileName:                              " << theFileName                << endl
       << "theNewRepUpIndicator:                     " << theNewRepUpIndicator       << endl
       << "theGovSpecNumber:                         " << theGovSpecNumber           << endl
       << "theSecurityClassification:                " << theSecurityClassification  << endl
       << "theCountryCode:                           " << theCountryCode             << endl
       << "theSecurityReleaseMarking:                " << theSecurityReleaseMarking  << endl
       << "theLocSectionLoc:                         " << theLocSectionLoc           << endl
       << (*theLocationSection) << endl;
}

bool ossimRpfHeader::hasComponent(ossimRpfComponentId componentId)const
{
   if(theLocationSection)
   {
      return theLocationSection->hasComponent(componentId);
   }
   
   return false;
}

ossimString ossimRpfHeader::getSecurityClassification()const
{
   return theSecurityClassification;
}

ossimRpfCoverageSection* ossimRpfHeader::getNewCoverageSection(istream& in)const
{
   ossimRpfCoverageSection* result = NULL;

   if(in&&theLocationSection)
   {
      ossimRpfComponentLocationRecord component;

      if(theLocationSection->getComponent(OSSIM_RPF_COVERAGE_SECTION_SUBHEADER,
                                          component))
      {
         result = new ossimRpfCoverageSection;

         in.seekg(component.theComponentLocation, ios::beg);
         
         if(in)
         {
            result->parseStream(in, getByteOrder());
         }
         else
         {
            delete result;
            result = NULL;
         }
      }
   }
   return result;
}

ossimRpfMaskSubsection*  ossimRpfHeader::getNewMaskSubsection(istream& in)const
{
   ossimRpfMaskSubsection* result = NULL;

   if(in&&theLocationSection)
   {
      ossimRpfComponentLocationRecord component;

      if(theLocationSection->getComponent(OSSIM_RPF_MASK_SUBSECTION,
                                          component))
      {
         result = new ossimRpfMaskSubsection;

         in.seekg(component.theComponentLocation, ios::beg);
         
         if(in)
         {
            if(result->parseStream(in, getByteOrder()) !=
               ossimErrorCodes::OSSIM_OK)
            {
               delete result;
               result = NULL;
            }
         }
         else
         {
            delete result;
            result = NULL;
         }
      }
   }
   
   return result;   
}


ossimRpfAttributeSectionSubheader* ossimRpfHeader::getNewAttributeSectionSubheader(istream& in)const
{
   ossimRpfAttributeSectionSubheader* result = NULL;

   if(in&&theLocationSection)
   {
      ossimRpfComponentLocationRecord component;

      if(theLocationSection->getComponent(OSSIM_RPF_ATTRIBUTE_SECTION_SUBHEADER,
                                          component))
      {
         result = new ossimRpfAttributeSectionSubheader;

         in.seekg(component.theComponentLocation, ios::beg);
         
         if(in)
         {
            if(result->parseStream(in, getByteOrder()) !=
               ossimErrorCodes::OSSIM_OK)
            {
               delete result;
               result = NULL;
            }
         }
         else
         {
            delete result;
            result = NULL;
         }
      }
   }
   
   return result;
}

ossimRpfColorGrayscaleSubheader* ossimRpfHeader::getNewColorGrayscaleSubheader(istream& in)const
{
   ossimRpfColorGrayscaleSubheader* result = NULL;
   
   if(in&&theLocationSection)
   {
      ossimRpfComponentLocationRecord component;

      if(theLocationSection->getComponent(OSSIM_RPF_COLOR_GRAYSCALE_SECTION_SUBHEADER,
                                          component))
      {
         result = new ossimRpfColorGrayscaleSubheader;

         in.seekg(component.theComponentLocation, ios::beg);
         
         if(in)
         {
            if(result->parseStream(in, getByteOrder()) !=
               ossimErrorCodes::OSSIM_OK)
            {
               delete result;
               result = NULL;
            }
         }
         else
         {
            delete result;
            result = NULL;
         }
      }      
   }

   return result;
}

ossimRpfCompressionSection* ossimRpfHeader::getNewCompressionSection(istream& in)const
{   
   ossimRpfCompressionSection* result = NULL;

   if(in&&theLocationSection)
   {
      ossimRpfComponentLocationRecord component;

      if(theLocationSection->getComponent(OSSIM_RPF_COMPRESSION_SECTION_SUBHEADER,
                                          component))
      {
         result = new ossimRpfCompressionSection;
         
         in.seekg(component.theComponentLocation, ios::beg);
         
         if(in)
         {
            if(result->parseStream(in, getByteOrder()) !=
               ossimErrorCodes::OSSIM_OK)
            {
               delete result;
               result = NULL;
            }
         }
         else
         {
            delete result;
            result = NULL;
         }
      }
   }
   
   return result;   
}

ossimRpfCompressionSectionSubheader* ossimRpfHeader::getNewCompressionSectionSubheader(istream& in)const
{
   ossimRpfCompressionSectionSubheader* result = NULL;

   if(in&&theLocationSection)
   {
      ossimRpfComponentLocationRecord component;

      if(theLocationSection->getComponent(OSSIM_RPF_COMPRESSION_SECTION_SUBHEADER,
                                          component))
      {
         result = new ossimRpfCompressionSectionSubheader;

         in.seekg(component.theComponentLocation, ios::beg);
         
         if(in)
         {
            if(result->parseStream(in, getByteOrder()) !=
               ossimErrorCodes::OSSIM_OK)
            {
               delete result;
               result = NULL;
            }
         }
         else
         {
            delete result;
            result = NULL;
         }
      }
   }
   
   return result;   
}

   

ossimRpfImageDisplayParameterSubheader* ossimRpfHeader::getNewImageDisplayParameterSubheader(istream& in)const
{
   ossimRpfImageDisplayParameterSubheader* result = NULL;

   if(in&&theLocationSection)
   {
      ossimRpfComponentLocationRecord component;

      if(theLocationSection->getComponent(OSSIM_RPF_IMAGE_DISPLAY_PARAMETERS_SUBHEADER,
                                          component))
      {
         result = new ossimRpfImageDisplayParameterSubheader;

         in.seekg(component.theComponentLocation, ios::beg);
         
         if(in)
         {
            if(result->parseStream(in, getByteOrder()) !=
               ossimErrorCodes::OSSIM_OK)
            {
               delete result;
               result = NULL;
            }
         }
         else
         {
            delete result;
            result = NULL;
         }
      }
   }
   
   return result;
}

ossimRpfImageDescriptionSubheader* ossimRpfHeader::getNewImageDescriptionSubheader(istream& in)const
{
   ossimRpfImageDescriptionSubheader* result = NULL;

   if(in&&theLocationSection)
   {
      ossimRpfComponentLocationRecord component;

      if(theLocationSection->getComponent(OSSIM_RPF_IMAGE_DESCRIPTION_SUBHEADER,
                                          component))
      {
         result = new ossimRpfImageDescriptionSubheader;

         in.seekg(component.theComponentLocation, ios::beg);
         
         if(in)
         {
            if(result->parseStream(in, getByteOrder()) !=
               ossimErrorCodes::OSSIM_OK)
            {
               delete result;
               result = NULL;
            }
         }
         else
         {
            delete result;
            result = NULL;
         }
      }
   }
   
   return result;
}


ossimRpfBoundaryRectTable* ossimRpfHeader::getNewBoundaryRectTable(istream &in)const
{
   ossimRpfBoundaryRectTable* result = NULL;

   if(in&&theLocationSection)
   {
      ossimRpfComponentLocationRecord component;

      ossimRpfBoundaryRectSectionSubheader* tempSubheader = getNewBoundaryRectSectSubheader(in);

      if(tempSubheader)
      {
         if(theLocationSection->getComponent(OSSIM_RPF_BOUNDARY_RECT_TABLE,
                                             component))
         {
            result = new ossimRpfBoundaryRectTable;
            
            result->setNumberOfEntries(tempSubheader->getNumberOfEntries());
            in.seekg(component.theComponentLocation, ios::beg);
            if(in)
            {
               if(result->parseStream(in, getByteOrder()) !=
                  ossimErrorCodes::OSSIM_OK)
               {
                  delete result;
                  result = NULL;
               }
            }
            else
            {
               delete result;
               result = NULL;
            }
         }

         delete tempSubheader;
         tempSubheader = NULL;
      }
   }   

   return result;
}

ossimRpfBoundaryRectSectionSubheader* ossimRpfHeader::getNewBoundaryRectSectSubheader(istream &in)const
{
   ossimRpfBoundaryRectSectionSubheader* result = NULL;

   if(in&&theLocationSection)
   {
      ossimRpfComponentLocationRecord component;
      
      if(theLocationSection->getComponent(OSSIM_RPF_BOUNDARY_RECT_SECTION_SUBHEADER,
                                          component))
      {
         result = new ossimRpfBoundaryRectSectionSubheader;

         in.seekg(component.theComponentLocation, ios::beg);
         
         if(in)
         {
            if(result->parseStream(in, getByteOrder()) !=
               ossimErrorCodes::OSSIM_OK)
            {
               delete result;
               result = NULL;
            }
         }
         else
         {
            delete result;
            result = NULL;
         }
      }
   }   

   return result;
   
}

ossimRpfFrameFileIndexSectionSubheader* ossimRpfHeader::getNewFrameFileIndexSectionSubheader(istream &in)const
{
   ossimRpfFrameFileIndexSectionSubheader* result = NULL;

   if(in&&theLocationSection)
   {
      ossimRpfComponentLocationRecord component;

      if(theLocationSection->getComponent(OSSIM_RPF_FRAME_FILE_INDEX_SECTION_SUBHEADER,
                                          component))
      {
         result = new ossimRpfFrameFileIndexSectionSubheader;

         in.seekg(component.theComponentLocation, ios::beg);
         if(in)
         {
            if(result->parseStream(in, getByteOrder()) !=
               ossimErrorCodes::OSSIM_OK)
            {
               delete result;
               result = NULL;
            }
         }
         else
         {
            delete result;
            result = NULL;
         }
      }
   }   

   return result;   
}

ossimRpfFrameFileIndexSubsection* ossimRpfHeader::getNewFileIndexSubsection(istream& in)const
{
   ossimRpfFrameFileIndexSubsection* result = NULL;

   if(in&&theLocationSection)
   {
      ossimRpfComponentLocationRecord component;
      ossimRpfFrameFileIndexSectionSubheader* tempSubheader = getNewFrameFileIndexSectionSubheader(in);

      if(theLocationSection->getComponent(OSSIM_RPF_FRAME_FILE_INDEX_SUBSECTION,
                                          component))
      {
         result = new ossimRpfFrameFileIndexSubsection;

         result->setNumberOfFileIndexRecords(tempSubheader->getNumberOfIndexRecords());
         result->setNumberOfPathnames(tempSubheader->getNumberOfPathnameRecords());
         in.seekg(component.theComponentLocation, ios::beg);
         if(in)
         {
            if(result->parseStream(in, getByteOrder()) !=
               ossimErrorCodes::OSSIM_OK)
            {
               delete result;
               result = NULL;
            }
         }
         else
         {
            delete result;
            result = NULL;
         }
      }
      if(tempSubheader)
      {
         delete tempSubheader;
         tempSubheader = NULL;
      }
   }   

   return result;      
}

ossimRpfCompressionSection* ossimRpfHeader::getNewCompressionSection(const ossimFilename& file)const
{
   ifstream in(file.c_str(), ios::in|ios::binary);

   return getNewCompressionSection(in);
}

ossimRpfCoverageSection* ossimRpfHeader::getNewCoverageSection(const ossimFilename& file)const
{
   ifstream in(file.c_str(), ios::in|ios::binary);
   
   return getNewCoverageSection(in);
}

ossimRpfBoundaryRectTable* ossimRpfHeader::getNewBoundaryRectTable(const ossimFilename &file)const
{
   ifstream in(file.c_str(), ios::in|ios::binary);
   
   return getNewBoundaryRectTable(in);
}

ossimRpfBoundaryRectSectionSubheader* ossimRpfHeader::getNewBoundaryRectSectSubheader(const ossimFilename &file)const
{
   ifstream in(file.c_str(), ios::in|ios::binary);

   return getNewBoundaryRectSectSubheader(in);
}

ossimRpfFrameFileIndexSectionSubheader* ossimRpfHeader::getNewFrameFileIndexSectionSubheader(const ossimFilename &file)const
{
   ifstream in(file.c_str(), ios::in|ios::binary);

   return getNewFrameFileIndexSectionSubheader(in);
}

ossimRpfFrameFileIndexSubsection* ossimRpfHeader::getNewFileIndexSubsection(const ossimFilename& file)const
{
   ifstream in(file.c_str(), ios::in|ios::binary);

   return getNewFileIndexSubsection(in);
}

ossimString ossimRpfHeader::getDate()const
{
   return theGovSpecDate;
}
