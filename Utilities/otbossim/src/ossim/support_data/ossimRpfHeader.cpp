//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: This class extends the stl's string class.
//
//********************************************************************
// $Id: ossimRpfHeader.cpp 19682 2011-05-31 14:21:20Z dburken $

#include <cstring>
#include <iostream>
#include <fstream>

#include <ossim/support_data/ossimRpfHeader.h>
#include <ossim/base/ossimCommon.h> /* ossim::byteOrder() */
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/support_data/ossimNitfCommon.h>
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

#include <ossim/base/ossimTrace.h>

 // Static trace for debugging
static ossimTrace traceDebug("ossimRpfHeader:debug");

// Keywords:
static const ossimString HEADER_SECTION_LENGTH_KW    = "HeaderSectionLength";
static const ossimString NEW_REP_UP_INDICATOR_KW     = "NewRepUpIndicator";
static const ossimString GOV_SPEC_NUMBER_KW          = "GovSpecNumber";
static const ossimString GOV_SPEC_DATE_KW            = "GovSpecDate";
static const ossimString SECURITY_CLASSIFICATION_KW  = "SecurityClassification";
static const ossimString COUNTRY_CODE_KW             = "CountryCode";
static const ossimString SECURITY_RELEASE_MARKING_KW = "SecurityReleaseMarking";


ossimRpfHeader::ossimRpfHeader()
   :
   m_littleBigEndianIndicator(0x00),
   m_headerSectionLength(48),
   m_fileName(),
   m_newRepUpIndicator(0),
   m_govSpecNumber(),
   m_govSpecDate(),
   m_securityClassification(),
   m_countryCode(),
   m_securityReleaseMarking(),
   m_locSectionLoc(0),
   m_locationSection(new ossimRpfLocationSection)
{
   memset(m_fileName, ' ' , 12);
   memset(m_govSpecNumber, ' ', 15);
   memset(m_govSpecDate, ' ', 8);
   memset(m_securityClassification, ' ', 1);
   memset(m_countryCode, ' ', 2);
   memset(m_securityReleaseMarking, ' ', 2);
   
   m_fileName[12] = '\0';
   m_govSpecNumber[15] = '\0';
   m_govSpecDate[8] = '\0';
   m_securityClassification[1] = '\0';
   m_countryCode[2] = '\0';
   m_securityReleaseMarking[2] = '\0';
}

ossimRpfHeader::ossimRpfHeader(const ossimRpfHeader& obj)
   :
   m_littleBigEndianIndicator(obj.m_littleBigEndianIndicator),
   m_headerSectionLength(obj.m_headerSectionLength),
   m_fileName(),
   m_newRepUpIndicator(obj.m_newRepUpIndicator),
   m_govSpecNumber(),
   m_govSpecDate(),
   m_securityClassification(),
   m_countryCode(),
   m_securityReleaseMarking(),
   m_locSectionLoc(obj.m_locSectionLoc),
   m_locationSection( new ossimRpfLocationSection( *(obj.m_locationSection) ) )
{
   memcpy(m_fileName, obj.m_fileName, 13);
   memcpy(m_govSpecNumber, obj.m_govSpecNumber, 16);
   memcpy(m_govSpecDate, obj.m_govSpecDate, 9);
   memcpy(m_securityClassification, obj.m_securityClassification, 2);
   memcpy(m_countryCode, obj.m_countryCode, 3);
   memcpy(m_securityReleaseMarking, obj.m_securityReleaseMarking, 3);
}

const ossimRpfHeader& ossimRpfHeader::operator=(const ossimRpfHeader& rhs)
{
   if ( this != &rhs )
   {
      m_littleBigEndianIndicator = rhs.m_littleBigEndianIndicator;
      m_headerSectionLength = rhs.m_headerSectionLength;
      memcpy(m_fileName, rhs.m_fileName, 13);
      m_newRepUpIndicator = rhs.m_newRepUpIndicator;
      memcpy(m_govSpecNumber, rhs.m_govSpecNumber, 16);
      memcpy(m_govSpecDate, rhs.m_govSpecDate, 9);   
      memcpy(m_securityClassification, rhs.m_securityClassification, 2);
      memcpy(m_countryCode, rhs.m_countryCode, 3);
      memcpy(m_securityReleaseMarking, rhs.m_securityReleaseMarking, 3);
      m_locSectionLoc = rhs.m_locSectionLoc;

      m_locationSection = new ossimRpfLocationSection( *(rhs.m_locationSection) );
   }
   return *this;
}

ossimRpfHeader::~ossimRpfHeader()
{
   if(m_locationSection)
   {
      delete m_locationSection;
      m_locationSection = 0;
   }
}

std::string ossimRpfHeader::getRegisterTagName()const
{
   return std::string("RPFHDR");
}

void ossimRpfHeader::parseStream(std::istream& in)
{
   if(in)
   {
      in.read((char*)&m_littleBigEndianIndicator, 1);

      in.read((char*)&m_headerSectionLength, 2);
      in.read((char*)m_fileName, 12);
      in.read((char*)&m_newRepUpIndicator, 1);
      in.read((char*)m_govSpecNumber, 15);
      in.read((char*)m_govSpecDate, 8);
      in.read((char*)m_securityClassification, 1);
      in.read((char*)m_countryCode, 2);
      in.read((char*)m_securityReleaseMarking, 2);
      in.read((char*)&m_locSectionLoc, 4);
      
      m_fileName[12] = '\0';
      m_govSpecNumber[15] = '\0';
      m_govSpecDate[8] = '\0';
      m_securityClassification[1] = '\0';
      m_countryCode[2] = '\0';
      m_securityReleaseMarking[2] = '\0';
      
      //---
      // From spec:  MIL-PRF-89038CARDG m_littleBigEndianIndicator shall
      // be 0x00 for all data denoting big endian storage.  We will test
      // anyway just in case...
      //---
      ossimByteOrder dataByteOrder = getByteOrder();

      if( ossim::byteOrder() != dataByteOrder )
      {
         ossimEndian anEndian;
         anEndian.swap(m_headerSectionLength);
         anEndian.swap(m_locSectionLoc);
      }

      std::streamoff saveGet = in.tellg();
      in.seekg(m_locSectionLoc, ios::beg);
      m_locationSection->parseStream(in, dataByteOrder);
      in.seekg(saveGet, ios::beg);
   }
}

void ossimRpfHeader::writeStream(std::ostream& out)
{
   // Always write in big endian.
   if (m_littleBigEndianIndicator != 0x00)
   {
      ossimNotify(ossimNotifyLevel_NOTICE)
         << "ossimRpfHeader::writeStream writing in big endian even though"
         << " the m_littleBigEndianIndicator is set to little endian."
         << std::endl;
      m_littleBigEndianIndicator = 0x00;
   }
   
   ossimByteOrder dataByteOrder = getByteOrder();

   if( ossim::byteOrder() != dataByteOrder )
   {
      ossimEndian anEndian;
      anEndian.swap(m_headerSectionLength);
      anEndian.swap(m_locSectionLoc);
   }

   out.write((char*)&m_littleBigEndianIndicator, 1);
   
   out.write((char*)&m_headerSectionLength, 2);
   out.write((char*)m_fileName, 12);
   out.write((char*)&m_newRepUpIndicator, 1);
   out.write((char*)m_govSpecNumber, 15);
   out.write((char*)m_govSpecDate, 8);
   out.write((char*)m_securityClassification, 1);
   out.write((char*)m_countryCode, 2);
   out.write((char*)m_securityReleaseMarking, 2);
   out.write((char*)&m_locSectionLoc, 4);

   if( ossim::byteOrder() != dataByteOrder )
   {
      // Must swap things back or we will seek to a bad location.
      ossimEndian anEndian;
      anEndian.swap(m_headerSectionLength);
      anEndian.swap(m_locSectionLoc);
   }

   if (m_locSectionLoc) // May or may not be set.
   {
      std::streampos pos = out.tellp();
      out.seekp(m_locSectionLoc, ios::beg);
      m_locationSection->writeStream(out);
      out.seekp(pos);
   }
}

std::ostream& ossimRpfHeader::print(std::ostream& out, const std::string& prefix) const
{
   out << prefix << "byte_order:               "
       << (m_littleBigEndianIndicator==0x00?"big_endian\n":"little_endian\n")
       << prefix << HEADER_SECTION_LENGTH_KW << ":      "
       << m_headerSectionLength << "\n"
       << prefix << ossimKeywordNames::FILENAME_KW << ":                 "
       << m_fileName << "\n"
       << prefix << NEW_REP_UP_INDICATOR_KW << ":        "
       << int(m_newRepUpIndicator) << "\n"       
       << prefix << GOV_SPEC_NUMBER_KW << ":            "
       << m_govSpecNumber << "\n"
       << prefix << GOV_SPEC_DATE_KW << ":              "
       << m_govSpecDate << "\n"
       << prefix << SECURITY_CLASSIFICATION_KW << ":   "
       << m_securityClassification << "\n"
       << prefix << COUNTRY_CODE_KW << ":              "
       << m_countryCode   << "\n"           
       << prefix << SECURITY_RELEASE_MARKING_KW << ":   "
       << m_securityReleaseMarking << "\n";

   if ( traceDebug() )
   {
      out << prefix << "LocSectionLoc:            "
          << m_locSectionLoc << "\n";
      if (m_locationSection)
      {
         m_locationSection->print(out, prefix);
      }
   }

   return out;
}

ossim_uint32 ossimRpfHeader::getSizeInBytes()const
{
   return 48;
}

ossimByteOrder ossimRpfHeader::getByteOrder()const
{
   return ( (m_littleBigEndianIndicator==0x00) ? OSSIM_BIG_ENDIAN : OSSIM_LITTLE_ENDIAN);
}

const ossimRpfLocationSection* ossimRpfHeader::getLocationSection() const
{
   return m_locationSection;
}

ossimRpfLocationSection* ossimRpfHeader::getLocationSection()
{
   return m_locationSection;
}

bool ossimRpfHeader::hasComponent(ossimRpfComponentId componentId)const
{
   if(m_locationSection)
   {
      return m_locationSection->hasComponent(componentId);
   }
   
   return false;
}

ossimString ossimRpfHeader::getSecurityClassification()const
{
   return m_securityClassification;
}

ossimRpfCoverageSection* ossimRpfHeader::getNewCoverageSection(std::istream& in)const
{
   ossimRpfCoverageSection* result = 0;

   if(in&&m_locationSection)
   {
      ossimRpfComponentLocationRecord component;

      if(m_locationSection->getComponent(OSSIM_RPF_COVERAGE_SECTION_SUBHEADER,
                                          component))
      {
         result = new ossimRpfCoverageSection;

         in.seekg(component.m_componentLocation, ios::beg);
         
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

ossimRpfMaskSubsection*  ossimRpfHeader::getNewMaskSubsection(std::istream& in)const
{
   ossimRpfMaskSubsection* result = 0;

   if(in&&m_locationSection)
   {
      ossimRpfComponentLocationRecord component;

      if(m_locationSection->getComponent(OSSIM_RPF_MASK_SUBSECTION,
                                          component))
      {
         result = new ossimRpfMaskSubsection;

         in.seekg(component.m_componentLocation, ios::beg);
         
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


ossimRpfAttributeSectionSubheader* ossimRpfHeader::getNewAttributeSectionSubheader(std::istream& in)const
{
   ossimRpfAttributeSectionSubheader* result = 0;

   if(in&&m_locationSection)
   {
      ossimRpfComponentLocationRecord component;

      if(m_locationSection->getComponent(OSSIM_RPF_ATTRIBUTE_SECTION_SUBHEADER,
                                          component))
      {
         result = new ossimRpfAttributeSectionSubheader;

         in.seekg(component.m_componentLocation, ios::beg);
         
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

ossimRpfColorGrayscaleSubheader* ossimRpfHeader::getNewColorGrayscaleSubheader(std::istream& in)const
{
   ossimRpfColorGrayscaleSubheader* result = 0;
   
   if(in&&m_locationSection)
   {
      ossimRpfComponentLocationRecord component;

      if(m_locationSection->getComponent(OSSIM_RPF_COLOR_GRAYSCALE_SECTION_SUBHEADER,
                                          component))
      {
         result = new ossimRpfColorGrayscaleSubheader;

         in.seekg(component.m_componentLocation, ios::beg);
         
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

ossimRpfCompressionSection* ossimRpfHeader::getNewCompressionSection(std::istream& in)const
{   
   ossimRpfCompressionSection* result = 0;

   if(in&&m_locationSection)
   {
      ossimRpfComponentLocationRecord component;

      if(m_locationSection->getComponent(OSSIM_RPF_COMPRESSION_SECTION_SUBHEADER,
                                          component))
      {
         result = new ossimRpfCompressionSection;
         
         in.seekg(component.m_componentLocation, ios::beg);
         
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

ossimRpfCompressionSectionSubheader* ossimRpfHeader::getNewCompressionSectionSubheader(std::istream& in)const
{
   ossimRpfCompressionSectionSubheader* result = 0;

   if(in&&m_locationSection)
   {
      ossimRpfComponentLocationRecord component;

      if(m_locationSection->getComponent(OSSIM_RPF_COMPRESSION_SECTION_SUBHEADER,
                                          component))
      {
         result = new ossimRpfCompressionSectionSubheader;

         in.seekg(component.m_componentLocation, ios::beg);
         
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

   

ossimRpfImageDisplayParameterSubheader* ossimRpfHeader::getNewImageDisplayParameterSubheader(std::istream& in)const
{
   ossimRpfImageDisplayParameterSubheader* result = 0;

   if(in&&m_locationSection)
   {
      ossimRpfComponentLocationRecord component;

      if(m_locationSection->getComponent(OSSIM_RPF_IMAGE_DISPLAY_PARAMETERS_SUBHEADER,
                                          component))
      {
         result = new ossimRpfImageDisplayParameterSubheader;

         in.seekg(component.m_componentLocation, ios::beg);
         
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

ossimRpfImageDescriptionSubheader* ossimRpfHeader::getNewImageDescriptionSubheader(std::istream& in)const
{
   ossimRpfImageDescriptionSubheader* result = 0;

   if(in&&m_locationSection)
   {
      ossimRpfComponentLocationRecord component;

      if(m_locationSection->getComponent(OSSIM_RPF_IMAGE_DESCRIPTION_SUBHEADER,
                                          component))
      {
         result = new ossimRpfImageDescriptionSubheader;

         in.seekg(component.m_componentLocation, ios::beg);
         
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


ossimRpfBoundaryRectTable* ossimRpfHeader::getNewBoundaryRectTable(std::istream &in)const
{
   ossimRpfBoundaryRectTable* result = 0;

   if(in&&m_locationSection)
   {
      ossimRpfComponentLocationRecord component;

      ossimRpfBoundaryRectSectionSubheader* tempSubheader = getNewBoundaryRectSectSubheader(in);

      if(tempSubheader)
      {
         if(m_locationSection->getComponent(OSSIM_RPF_BOUNDARY_RECT_TABLE,
                                             component))
         {
            result = new ossimRpfBoundaryRectTable;
            
            result->setNumberOfEntries(tempSubheader->getNumberOfEntries());
            in.seekg(component.m_componentLocation, ios::beg);
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

ossimRpfBoundaryRectSectionSubheader* ossimRpfHeader::getNewBoundaryRectSectSubheader(std::istream &in)const
{
   ossimRpfBoundaryRectSectionSubheader* result = 0;

   if(in&&m_locationSection)
   {
      ossimRpfComponentLocationRecord component;
      
      if(m_locationSection->getComponent(OSSIM_RPF_BOUNDARY_RECT_SECTION_SUBHEADER,
                                          component))
      {
         result = new ossimRpfBoundaryRectSectionSubheader;

         in.seekg(component.m_componentLocation, ios::beg);
         
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

ossimRpfFrameFileIndexSectionSubheader* ossimRpfHeader::getNewFrameFileIndexSectionSubheader(std::istream &in)const
{
   ossimRpfFrameFileIndexSectionSubheader* result = 0;

   if(in&&m_locationSection)
   {
      ossimRpfComponentLocationRecord component;

      if(m_locationSection->getComponent(OSSIM_RPF_FRAME_FILE_INDEX_SECTION_SUBHEADER,
                                          component))
      {
         result = new ossimRpfFrameFileIndexSectionSubheader;

         in.seekg(component.m_componentLocation, ios::beg);
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

ossimRpfFrameFileIndexSubsection* ossimRpfHeader::getNewFileIndexSubsection(std::istream& in)const
{
   ossimRpfFrameFileIndexSubsection* result = 0;

   if(in&&m_locationSection)
   {
      ossimRpfComponentLocationRecord component;
      ossimRpfFrameFileIndexSectionSubheader* tempSubheader = getNewFrameFileIndexSectionSubheader(in);

      if(m_locationSection->getComponent(OSSIM_RPF_FRAME_FILE_INDEX_SUBSECTION,
                                          component))
      {
         result = new ossimRpfFrameFileIndexSubsection;

         result->setNumberOfFileIndexRecords(tempSubheader->getNumberOfIndexRecords());
         result->setNumberOfPathnames(tempSubheader->getNumberOfPathnameRecords());
         in.seekg(component.m_componentLocation, ios::beg);
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
   return m_govSpecDate;
}

ossim_uint32 ossimRpfHeader::getLocationSectionLocation() const
{
   return m_locSectionLoc;
}

void ossimRpfHeader::setFilename(const ossimString& file)
{
   ossimNitfCommon::setField(m_fileName, file, 12);
}

void ossimRpfHeader::setNewRepUpIndicator(const ossimString& s)
{
   if (s.size())
   {
      // Range check maybe??? (drb)
      m_newRepUpIndicator = static_cast<ossim_uint8>(*s.begin());
   }
}

void ossimRpfHeader::setGovSpecNumber(const ossimString& s)
{
   ossimNitfCommon::setField(m_govSpecNumber, s, 15); 
}

void ossimRpfHeader::setGovSpecDate(const ossimString& s)
{
   ossimNitfCommon::setField(m_govSpecDate, s, 8); 
}

void ossimRpfHeader::setSecurityClassification(const ossimString& s)
{
   ossimNitfCommon::setField(m_securityClassification, s, 1);
}

void ossimRpfHeader::setCountryCode(const ossimString& s)
{
   ossimNitfCommon::setField(m_countryCode, s, 2); 
}

void ossimRpfHeader::setSecurityReleaseMarking(const ossimString& s)
{
   ossimNitfCommon::setField(m_securityReleaseMarking, s, 2);
}

void ossimRpfHeader::setLocationSectionPos(std::streamoff off)
{
   m_locSectionLoc = static_cast<ossim_uint32>(off);
}

bool ossimRpfHeader::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   const char* lookup = 0;
   ossimString s;

   lookup = kwl.find(prefix, NEW_REP_UP_INDICATOR_KW);
   if (lookup)
   {
      s = lookup;
      setNewRepUpIndicator(s);
   }

   lookup = kwl.find(prefix, GOV_SPEC_NUMBER_KW);
   if (lookup)
   {
      s = lookup;
      setGovSpecNumber(s);
   }

   lookup = kwl.find(prefix, GOV_SPEC_DATE_KW);
   if (lookup)
   {
      s = lookup;
      setGovSpecDate(s);
   }

   lookup = kwl.find(prefix, SECURITY_CLASSIFICATION_KW);
   if (lookup)
   {
      s = lookup;
      setSecurityClassification(s);
   }

   lookup = kwl.find(prefix, COUNTRY_CODE_KW);
   if (lookup)
   {
      s = lookup;
      setCountryCode(s);
   }

   lookup = kwl.find(prefix, SECURITY_RELEASE_MARKING_KW);
   if (lookup)
   {
      s = lookup;
      setSecurityReleaseMarking(s);
   }

   return true;
}
