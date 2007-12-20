//*******************************************************************
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
// Description: This class extends the stl's string class.
//
//********************************************************************
// $Id: ossimRpfHeader.cpp 11094 2007-05-30 14:48:30Z dburken $
#include <ossim/support_data/ossimRpfHeader.h>
#include <string.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/support_data/ossimRpfLocationSection.h>
#include <ossim/support_data/ossimRpfCoverageSection.h>
#include <ossim/support_data/ossimRpfBoundaryRectSectionSubheader.h>
#include <ossim/support_data/ossimRpfBoundaryRectTable.h>
#include <ossim/support_data/ossimRpfFrameFileIndexSectionSubheader.h>
#include <ossim/support_data/ossimRpfFrameFileIndexSubsection.h>
#include <ossim/support_data/ossimRpfImageDescriptionSubheader.h>
#include <ossim/support_data/ossimRpfMaskSubsection.h>
#include <ossim/support_data/ossimRpfAttributeSectionSubheader.h>
#include <ossim/support_data/ossimRpfImageDisplayParameterSubheader.h>
#include <ossim/support_data/ossimRpfCompressionSectionSubheader.h>
#include <ossim/support_data/ossimRpfCompressionSection.h>
#include <ossim/support_data/ossimRpfColorGrayscaleSubheader.h>

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
      theLocationSection = 0;
   }
}
ossimRpfHeader::ossimRpfHeader()
   :theLittleBigEndianIndicator(0x00),
    theHeaderSectionLength(0),
    theNewRepUpIndicator(0),
    theLocSectionLoc(0),
    theLocationSection(0)
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

      //---
      // From spec:  MIL-PRF-89038CARDG theLittleBigEndianIndicator shall
      // be 0x00 for all data denoting big endian storage.  We will test
      // anyway just in case...
      //---
      ossimByteOrder dataByteOrder = ((theLittleBigEndianIndicator==0x00) ?
                                      OSSIM_BIG_ENDIAN:OSSIM_LITTLE_ENDIAN);

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
   out << "ossimRpfHeader::print:"
       << "\ntheLittleBigEndianIndicator: "
       << int(theLittleBigEndianIndicator)
       << "\ntheHeaderSectionLength:      " << theHeaderSectionLength
       << "\ntheFileName:                 " << theFileName
       << "\ntheNewRepUpIndicator:        " << theNewRepUpIndicator       
       << "\ntheGovSpecNumber:            " << theGovSpecNumber           
       << "\ntheSecurityClassification:   " << theSecurityClassification  
       << "\ntheCountryCode:              " << theCountryCode             
       << "\ntheSecurityReleaseMarking:   " << theSecurityReleaseMarking  
       << "\ntheLocSectionLoc:            " << theLocSectionLoc << "\n"
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
   ossimRpfCoverageSection* result = 0;

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
            result = 0;
         }
      }
   }
   return result;
}

ossimRpfMaskSubsection*  ossimRpfHeader::getNewMaskSubsection(istream& in)const
{
   ossimRpfMaskSubsection* result = 0;

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
               result = 0;
            }
         }
         else
         {
            delete result;
            result = 0;
         }
      }
   }
   
   return result;   
}


ossimRpfAttributeSectionSubheader* ossimRpfHeader::getNewAttributeSectionSubheader(istream& in)const
{
   ossimRpfAttributeSectionSubheader* result = 0;

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
               result = 0;
            }
         }
         else
         {
            delete result;
            result = 0;
         }
      }
   }
   
   return result;
}

ossimRpfColorGrayscaleSubheader* ossimRpfHeader::getNewColorGrayscaleSubheader(istream& in)const
{
   ossimRpfColorGrayscaleSubheader* result = 0;
   
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
               result = 0;
            }
         }
         else
         {
            delete result;
            result = 0;
         }
      }      
   }

   return result;
}

ossimRpfCompressionSection* ossimRpfHeader::getNewCompressionSection(istream& in)const
{   
   ossimRpfCompressionSection* result = 0;

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
               result = 0;
            }
         }
         else
         {
            delete result;
            result = 0;
         }
      }
   }
   
   return result;   
}

ossimRpfCompressionSectionSubheader* ossimRpfHeader::getNewCompressionSectionSubheader(istream& in)const
{
   ossimRpfCompressionSectionSubheader* result = 0;

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
               result = 0;
            }
         }
         else
         {
            delete result;
            result = 0;
         }
      }
   }
   
   return result;   
}

   

ossimRpfImageDisplayParameterSubheader* ossimRpfHeader::getNewImageDisplayParameterSubheader(istream& in)const
{
   ossimRpfImageDisplayParameterSubheader* result = 0;

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
               result = 0;
            }
         }
         else
         {
            delete result;
            result = 0;
         }
      }
   }
   
   return result;
}

ossimRpfImageDescriptionSubheader* ossimRpfHeader::getNewImageDescriptionSubheader(istream& in)const
{
   ossimRpfImageDescriptionSubheader* result = 0;

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
               result = 0;
            }
         }
         else
         {
            delete result;
            result = 0;
         }
      }
   }
   
   return result;
}


ossimRpfBoundaryRectTable* ossimRpfHeader::getNewBoundaryRectTable(istream &in)const
{
   ossimRpfBoundaryRectTable* result = 0;

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
                  result = 0;
               }
            }
            else
            {
               delete result;
               result = 0;
            }
         }

         delete tempSubheader;
         tempSubheader = 0;
      }
   }   

   return result;
}

ossimRpfBoundaryRectSectionSubheader* ossimRpfHeader::getNewBoundaryRectSectSubheader(istream &in)const
{
   ossimRpfBoundaryRectSectionSubheader* result = 0;

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
               result = 0;
            }
         }
         else
         {
            delete result;
            result = 0;
         }
      }
   }   

   return result;
   
}

ossimRpfFrameFileIndexSectionSubheader* ossimRpfHeader::getNewFrameFileIndexSectionSubheader(istream &in)const
{
   ossimRpfFrameFileIndexSectionSubheader* result = 0;

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
               result = 0;
            }
         }
         else
         {
            delete result;
            result = 0;
         }
      }
   }   

   return result;   
}

ossimRpfFrameFileIndexSubsection* ossimRpfHeader::getNewFileIndexSubsection(istream& in)const
{
   ossimRpfFrameFileIndexSubsection* result = 0;

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
               result = 0;
            }
         }
         else
         {
            delete result;
            result = 0;
         }
      }
      if(tempSubheader)
      {
         delete tempSubheader;
         tempSubheader = 0;
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
