//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
// Description: This class give the capability to access tiles from an
//              rpf file.
//
//********************************************************************
// $Id: ossimRpfFrame.cpp 20324 2011-12-06 22:25:23Z dburken $

#include <ossim/support_data/ossimRpfFrame.h>
#include <ossim/support_data/ossimRpfHeader.h>
#include <ossim/support_data/ossimRpfAttributes.h>
#include <ossim/support_data/ossimRpfAttributeOffsetRecord.h>
#include <ossim/support_data/ossimRpfAttributeSectionSubheader.h>
#include <ossim/support_data/ossimRpfCoverageSection.h>
#include <ossim/support_data/ossimRpfImageDescriptionSubheader.h>
#include <ossim/support_data/ossimRpfImageDisplayParameterSubheader.h>
#include <ossim/support_data/ossimRpfMaskSubheader.h>
#include <ossim/support_data/ossimRpfCompressionSection.h>
#include <ossim/support_data/ossimRpfColorGrayscaleSubheader.h>
#include <ossim/support_data/ossimRpfColorGrayscaleOffsetRecord.h>
#include <ossim/support_data/ossimRpfColorConverterSubsection.h>
#include <ossim/support_data/ossimRpfColorConverterTable.h>
#include <ossim/support_data/ossimRpfColorGrayscaleTable.h>
#include <ossim/support_data/ossimRpfLocationSection.h>
#include <ossim/support_data/ossimRpfColorConverterSubsection.h>
#include <ossim/support_data/ossimRpfConstants.h>
#include <ossim/support_data/ossimRpfReplaceUpdateRecord.h>
#include <ossim/support_data/ossimRpfReplaceUpdateSectionSubheader.h>
#include <ossim/support_data/ossimNitfFile.h>
#include <ossim/support_data/ossimNitfFileHeader.h>
#include <ossim/support_data/ossimNitfTagInformation.h>
#include <ossim/base/ossimEndian.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimTrace.h>
#include <istream>
#include <ostream>

static const ossimTrace traceDebug("ossimRpfFrame:debug");

std::ostream& operator <<(std::ostream& out, const ossimRpfFrame& data)
{
   data.print(out);
   
   return out;
}

ossimRpfFrame::ossimRpfFrame()
   :theHeader(0),
    theFilename(""),
    theCoverage(0),
    theAttributes(0),
    theImageDescriptionSubheader(0),
    theMaskSubheader(0),
    theImageDisplayParameterSubheader(0),
    theCompressionSection(0),
    theColorGrayscaleSubheader(0),
    theColorConverterSubsection(0),
    theNitfFile(0),
    theSubframeMaskTable(0),
    theSubframeTransparencyMaskTable(0),
    theReplaceUpdateTable(0)
{
}

ossimRpfFrame::~ossimRpfFrame()
{
   deleteAll();
}

std::ostream& ossimRpfFrame::print(std::ostream& out, const std::string& prefix) const
{
   if (traceDebug())
   {
      out << "begin_rpf_frame_print:\n";
   }
   
   if(theHeader)
   {
      theHeader->print(out, prefix);
   }

   out << prefix << "filename: " << theFilename << "\n";

   if ( theReplaceUpdateTable.valid() )
   {
      theReplaceUpdateTable->print(out, prefix);
   }

   if (traceDebug())
   {
      if(theCoverage)
      {
         theCoverage->print(out, prefix);
      }
   }

   if(theAttributes)
   {
      theAttributes->print(out, prefix);
   }

   if (traceDebug())
   {
      if(theImageDescriptionSubheader)
      {
         theImageDescriptionSubheader->print(out, prefix);
      }
      
      if(theMaskSubheader)
      {
         out << *theMaskSubheader << endl;
      }
      if(theImageDisplayParameterSubheader)
      {
         out << *theImageDisplayParameterSubheader << endl;
      }
      if(theCompressionSection)
      {
         out << *theCompressionSection << endl;
      }
      if(theColorGrayscaleSubheader)
      {
         out << *theColorGrayscaleSubheader << endl;
      }
      if(theColorConverterSubsection)
      {
         out << *theColorConverterSubsection << endl;
      }
      
      copy(theColorGrayscaleTable.begin(),
           theColorGrayscaleTable.end(),
           ostream_iterator<ossimRpfColorGrayscaleTable>(out, "\n"));
      
      if(theImageDescriptionSubheader&&
         !(theImageDescriptionSubheader->isSubframeMaskTableOffsetNull()))
      {
         out << "Subframe Mask Table:" << endl;
         unsigned long spectralIndex;
         unsigned long rowIndex;
         unsigned long colIndex;
         // first loop through the Mask table and allocate while we do it
         for(spectralIndex = 0;
             spectralIndex < theImageDescriptionSubheader->getNumberOfSpectralGroups();
             ++spectralIndex)
         {
            for(rowIndex = 0;
                rowIndex < theImageDescriptionSubheader->getNumberOfSubframesVertical();
                ++rowIndex)
            {
               for(colIndex = 0;
                   colIndex < theImageDescriptionSubheader->getNumberOfSubframesHorizontal();
                   ++colIndex)
               {
                  if( theSubframeMaskTable[spectralIndex][rowIndex][colIndex] == OSSIM_RPF_ULONG_NULL)
                  {
                     out << "NULL ";
                  }
                  else
                  {
                     out << theSubframeMaskTable[spectralIndex][rowIndex][colIndex] << " ";
                  }
               }
               out << endl;
            }
         }
      }


      if(theImageDescriptionSubheader&&
         !(theImageDescriptionSubheader->isTransparencyMaskTableOffsetNull()))
      {
         out << "Transparency Mask Table:" << endl;
         unsigned long spectralIndex;
         unsigned long rowIndex;
         unsigned long colIndex;
         // first loop through the Mask table and allocate while we do it
         for(spectralIndex = 0;
             spectralIndex < theImageDescriptionSubheader->getNumberOfSpectralGroups();
             ++spectralIndex)
         {
            for(rowIndex = 0;
                rowIndex < theImageDescriptionSubheader->getNumberOfSubframesVertical();
                ++rowIndex)
            {
               for(colIndex = 0;
                   colIndex < theImageDescriptionSubheader->getNumberOfSubframesHorizontal();
                   ++colIndex)
               {
                  if( theSubframeTransparencyMaskTable[spectralIndex][rowIndex][colIndex] == OSSIM_RPF_ULONG_NULL)
                  {
                     out << "NULL ";
                  }
                  else
                  {
                     out << theSubframeTransparencyMaskTable[spectralIndex][rowIndex][colIndex] << " ";
                  }
               }
            }
         }
      }


      
      out << "end_rpf_frame_print:\n";
      
   } // matches:  if (traceDebug())

   out << std::endl;
   
   return out;
}

ossimErrorCode ossimRpfFrame::parseFile(const ossimFilename& filename,
                                        bool minimalParse)
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimRpfFrame::parseFile: entered......" << std::endl;
   }
   ossimErrorCode result = ossimErrorCodes::OSSIM_OK;

   //Make sure any fileds have beend cleared
   clearFields();

   // Make sure all data is deleted.  The initialize call the
   // populate methods.  These methods will re-allocate the information
   deleteAll();


   theNitfFile = new ossimNitfFile;
   theNitfFile->parseFile(filename);
   
   const ossimRefPtr<ossimNitfFileHeader> nitfFileHeader =
      theNitfFile.valid() ? theNitfFile->getHeader() : 0;

   if(!nitfFileHeader)
   {
      theNitfFile = 0;

      return ossimErrorCodes::OSSIM_ERROR;
   }
   ossimNitfTagInformation info;
   nitfFileHeader->getTag(info, "RPFHDR");

   theFilename = filename;
   if(info.getTagName() == "RPFHDR")
   {
      ifstream in(filename.c_str(), ios::in|ios::binary);
      // set the get pointer for the stream to the start
      // of the Rpf header data
      in.seekg(info.getTagDataOffset(), ios::beg);

      if(theHeader) delete theHeader;
      theHeader = new ossimRpfHeader;
      
      // if(theHeader->parseStream(in) != ossimErrorCodes::OSSIM_OK)
      theHeader->parseStream(in);

      if ( in.fail() )
      {
         deleteAll();
         
         return ossimErrorCodes::OSSIM_ERROR;
      }
      else
         // if(!in.fail()&&theHeader)
      {
         result = populateAttributeSection(in);

         // This is needed for ossim-rpf --list-frames so NOT put in full parse section.
         if(!in.fail()&&(result == ossimErrorCodes::OSSIM_OK))
         {
            result = populateReplaceUpdateTable(in);
         }

         if ( minimalParse == false )
         {
            if(!in.fail()&&(result == ossimErrorCodes::OSSIM_OK))
            {
               populateCompressionSection(in);
            }
            if(!in.fail()&&(result == ossimErrorCodes::OSSIM_OK))
            {
               result = populateCoverageSection(in);
            }
            if(!in.fail()&&(result == ossimErrorCodes::OSSIM_OK))
            {
               result = populateImageSection(in);
            }
            if(!in.fail()&&(result == ossimErrorCodes::OSSIM_OK))
            {
               result = populateColorGrayscaleSection(in);
            }
            if(!in.fail()&&(result == ossimErrorCodes::OSSIM_OK))
            {
               result = populateMasks(in);
            }
         }
      }
   }
   else
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }

   return ossimErrorCodes::OSSIM_OK;
}

bool ossimRpfFrame::hasSubframeMaskTable()const
{
   if(theImageDescriptionSubheader)
   {
      return (!theImageDescriptionSubheader->isSubframeMaskTableOffsetNull());
   }

   return false;
          
}

bool ossimRpfFrame::fillSubFrameBuffer(ossim_uint8* buffer,
                                       ossim_uint32 /* spectralGroup */,
                                       ossim_uint32 row,
                                       ossim_uint32 col)const
{
   if(theImageDescriptionSubheader &&
      theImageDisplayParameterSubheader&&
      theHeader&&
      (col < theImageDescriptionSubheader->getNumberOfSubframesHorizontal()) &&
      (row < theImageDescriptionSubheader->getNumberOfSubframesVertical()))
   {
      ossimRpfComponentLocationRecord info;
      
      if(!theHeader->getLocationSection()->getComponent(OSSIM_RPF_SPATIAL_DATA_SUBSECTION, info))
      {
         return false;
      }
      
      ossim_uint32 offset = info.m_componentLocation;
      ossim_uint32 bytesPerSubframe = 0;

      // note that the code length is bit encoded so we must devide by 8
      // in order to get the byte offset instead of the bit offset.  We
      // then add that to the start of the spatial data section which is
      // the current value of offset.
      bytesPerSubframe = (theImageDisplayParameterSubheader->getNumberOfImageRows()*
                          theImageDisplayParameterSubheader->getNumberOfImageCodesPerRow()*
                          theImageDisplayParameterSubheader->getImageCodeLength())/8;
      
         
      if(hasSubframeMaskTable())
      {
         // check to see if the offset is NULL.  If it is
         // then just return false to let the caller know that the
         // buffer did not exist.
         if(theSubframeMaskTable[0][row][col] != OSSIM_RPF_ULONG_NULL)
         {
            offset += theSubframeMaskTable[0][row][col];
         }
         else
         {
            return false;
         }
      }
      else
      {
         ossim_uint32 bytes = bytesPerSubframe*(row*theImageDescriptionSubheader->getNumberOfSubframesHorizontal()+
                                                col);
         offset += bytes;
      }
      
      // now since we have the adjustment and we got to this point then
      // we can read in the data into the destination buffer.
      ifstream in(theFilename.c_str(), ios::in|ios::binary);
      if(!in.fail())
      {
         in.seekg(offset, ios::beg);
         in.read((char*)buffer, bytesPerSubframe);
         if(in.fail())
         {
            return false;
         }
      }
      else
      {
         return false;
      }
   }
   else
   {
      // this is if it was not within the rows and cols of
      // the subframe or if the image description header didn't exist
      return false;
   }

   return true;
}

void ossimRpfFrame::clearFields()
{   
   theFilename = "";
}

void ossimRpfFrame::deleteAll()
{
   if(theHeader)
   {
      delete theHeader;
      theHeader = 0;
   }
   if(theAttributes)
   {
      delete theAttributes;
      theAttributes = 0;
   }
   if(theCoverage)
   {
      delete theCoverage;
      theCoverage = 0;
   }
   if(theImageDescriptionSubheader)
   {
      delete theImageDescriptionSubheader;
      theImageDescriptionSubheader = 0;
   }
   if(theMaskSubheader)
   {
      delete theMaskSubheader;
      theMaskSubheader = 0;
   }
   if(theImageDisplayParameterSubheader)
   {
      delete theImageDisplayParameterSubheader;
      theImageDisplayParameterSubheader = 0;
   }
   if(theCompressionSection)
   {
      delete theCompressionSection;
      theCompressionSection = 0;
   }
   if(theColorGrayscaleSubheader)
   {
      delete theColorGrayscaleSubheader;
      theColorGrayscaleSubheader = 0;
   }
   if(theColorConverterSubsection)
   {
      delete theColorConverterSubsection;
      theColorConverterSubsection = 0;
   }
   if (theNitfFile.valid())
   {
      theNitfFile = 0;
   }
}

ossimErrorCode ossimRpfFrame::populateCoverageSection(istream& in)
{
   if(theCoverage) delete theCoverage;
   theCoverage = 0;
   if(theHeader)
   {
      theCoverage = theHeader->getNewCoverageSection(in);
   }
   return ossimErrorCodes::OSSIM_OK;
}

ossimErrorCode ossimRpfFrame::populateCompressionSection(istream& in)
{
   if(theCompressionSection) delete theCompressionSection;
   theCompressionSection = 0;
   if(theHeader)
   {
      theCompressionSection = theHeader->getNewCompressionSection(in);
   }
   return ossimErrorCodes::OSSIM_OK;
}

ossimErrorCode ossimRpfFrame::populateColorGrayscaleSection(istream& in)
{
   ossimErrorCode resultError = ossimErrorCodes::OSSIM_OK;
   if(theColorGrayscaleSubheader) delete theColorGrayscaleSubheader;
   theColorGrayscaleSubheader = 0;
   if(theHeader)
   {
      theColorGrayscaleSubheader  = theHeader->getNewColorGrayscaleSubheader(in);

      if(theColorGrayscaleSubheader)
      {
         const ossimRpfLocationSection* location = theHeader->getLocationSection();
         if(location)
         {
            ossimRpfComponentLocationRecord component;
            
            if(location->getComponent(OSSIM_RPF_COLOR_CONVERTER_SUBSECTION,
                                     component))
            {
               in.seekg(component.m_componentLocation,ios::beg);
               if(theColorConverterSubsection) delete theColorConverterSubsection;
               theColorConverterSubsection = new ossimRpfColorConverterSubsection;
               theColorConverterSubsection->setNumberOfColorConverterOffsetRecords(
                  theColorGrayscaleSubheader->getNumberOfColorConverterOffsetRecords());
               resultError = theColorConverterSubsection->parseStream(in, theHeader->getByteOrder());
            }
            if(location->getComponent(OSSIM_RPF_COLORMAP_SUBSECTION, component))
            {
               in.seekg(component.m_componentLocation, ios::beg);
               
               theColorGrayscaleTable.clear();
               theColorGrayscaleTable.resize(theColorGrayscaleSubheader->getNumberOfColorGreyscaleOffsetRecords());

               // we will ignore the offset table offset which is a 4 byte value
               // and ignore the offset record length which is a 2 byte value.
               // This is suppose to put us at the beginning of the
               // [color/grayscale offset record].  The total number of
               // these offset records is found in the [color/grayscale section
               // subheader.
               in.ignore(6);
               for(ossim_uint32 index = 0;
                   index < theColorGrayscaleSubheader->getNumberOfColorGreyscaleOffsetRecords();
                   ++index)
               {
                  
                  ossimRpfColorGrayscaleOffsetRecord grayscaleOffsetRecord;

                  if(grayscaleOffsetRecord.parseStream(in, theHeader->getByteOrder()) == ossimErrorCodes::OSSIM_OK)
                  {
                     ossim_uint32 rememberLocation = in.tellg();
                     in.seekg(grayscaleOffsetRecord.getColorGrayscaleTableOffset()+component.m_componentLocation, ios::beg);
                     theColorGrayscaleTable[index].setTableData(grayscaleOffsetRecord.getColorGrayscaleTableId(),
                                                                grayscaleOffsetRecord.getNumberOfColorGrayscaleRecords());
                     theColorGrayscaleTable[index].parseStream(in, theHeader->getByteOrder());
                     in.seekg(rememberLocation, ios::beg);
                  }
                  else
                  {
                     return ossimErrorCodes::OSSIM_ERROR;
                  }
               }
            }
         }
      }
   }
   
   return resultError;
}

ossimErrorCode ossimRpfFrame::populateImageSection(istream& in)
{
   ossimErrorCode result = ossimErrorCodes::OSSIM_OK;
   if(theImageDescriptionSubheader) delete theImageDescriptionSubheader;
   theImageDescriptionSubheader = 0;
   if(theHeader&&in)
   {
      // get the subheader information.
      theImageDescriptionSubheader = theHeader->getNewImageDescriptionSubheader(in);

      
      // need to do something with the table before going to the display.
      if(theImageDescriptionSubheader &&(!theImageDescriptionSubheader->isSubframeMaskTableOffsetNull()))
      {
         if(theMaskSubheader) delete theMaskSubheader;
         // need to get the mask subheader
         theMaskSubheader = new ossimRpfMaskSubheader;

         // seek to the start of the subheader
         in.seekg(theImageDescriptionSubheader->getEndOffset() +
                  theImageDescriptionSubheader->getSubframeMaskTableOffset(), ios::beg);

         // get the information
         result = theMaskSubheader->parseStream(in, theHeader->getByteOrder());
      }

      if(theImageDisplayParameterSubheader) delete theImageDisplayParameterSubheader;
      // Get the display parameter header
      theImageDisplayParameterSubheader = theHeader->getNewImageDisplayParameterSubheader(in);
   }
   return result;
}

ossimErrorCode ossimRpfFrame::populateAttributeSection(istream& in)
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimRpfFrame::populateAttributeSection:  entered ..." << std::endl;
   }
   ossimRpfAttributeSectionSubheader* temp = 0;
   
   const ossimRpfLocationSection* location = theHeader->getLocationSection();
   if(location)
   {
      ossimRpfComponentLocationRecord component;
      
      if(location->getComponent(OSSIM_RPF_ATTRIBUTE_SECTION_SUBHEADER,
                               component))
      {
         temp = new ossimRpfAttributeSectionSubheader;
         in.seekg(component.m_componentLocation, std::ios::beg);
         if(temp->parseStream(in, theHeader->getByteOrder()) != ossimErrorCodes::OSSIM_OK)
         {
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG) << "ossimRpfFrame::populateAttributeSection:  leaving 1 ..." << std::endl;
            }
            return ossimErrorCodes::OSSIM_ERROR;
         }
      }
   }
   if(!in.fail()&&temp)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG: Has attribute section" << std::endl;
      }
      if(theAttributes) delete theAttributes;
      theAttributes = new ossimRpfAttributes;
      ossimRpfAttributeOffsetRecord record;
      ossim_uint64 locationAttributeSubsection = in.tellg();
      ossim_uint64 absoluteAddresToTable = locationAttributeSubsection + temp->getAttributeOffsetTableOffset();
      ossimEndian anEndian;

      
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG: Number of attributes = "
                                             <<  temp->getNumberOfAttributeOffsetRecords()
                                             << std::endl;
      }
      in.seekg(absoluteAddresToTable, ios::beg);
      for(ossim_uint32 index = 0;
          index < temp->getNumberOfAttributeOffsetRecords();
          index++)
      {
         if((!in.fail())&&
            record.parseStream(in, theHeader->getByteOrder()) ==
            ossimErrorCodes::OSSIM_OK)
         {
            // make sure we save the current position.  The attribute
            // parser in the switch/case will change the location and
            // we must come back to the start of the next record.
            ossim_uint64 currentLocation = in.tellg();

            // go to the location of the start of the attribute.               
            in.seekg(locationAttributeSubsection + record.getAttributeRecordOffset(), ios::beg);

            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG: Parsing attribute number  " << index
                                                   << " with id = " << record.getAttributeId()
                                                   << std::endl;
            }
            // allocate some temporary space for the string type
            // attributes.
            char tempString[50];
            if(!theAttributes->getAttributeFlag(record.getAttributeId()))
            {
               theAttributes->setAttributeFlag(record.getAttributeId(), true);
               switch(record.getAttributeId())
               {
               case 1:
               {
                  in.read((char*)tempString, 8);
                  tempString[8] = '\0';
                  theAttributes->theCurrencyDate = tempString;
                  break;
               }
               case 2:
               {
                  in.read((char*)tempString, 8);
                  tempString[8] = '\0';
                  theAttributes->theProductionDate = tempString;
                  break;
               }
               case 3:
               {
                  in.read((char*)tempString, 8);
                  tempString[8] = '\0';
                  theAttributes->theSignificantDate = tempString;
                  break;
               }
               case 4:
               {
                  in.read((char*)tempString, 10);
                  theAttributes->theChartSeriesCode = ossimString(tempString,
                                                                  &tempString[10]);
                  in.read((char*)tempString, 8);
                  theAttributes->theMapDesignationCode = ossimString(tempString,
                                                                     &tempString[8]);
                  in.read((char*)tempString, 4);
                  theAttributes->theOldHorDatum = ossimString(tempString,
                                                              &tempString[4]);
                  in.read((char*)tempString, 7);
                  theAttributes->theEdition = ossimString(tempString,
                                                          &tempString[7]);
                  break;
               }
               case 5:
               {
                  in.read((char*)tempString, 2);
                  theAttributes->theProjectionCode = ossimString(tempString,
                                                                 &tempString[2]);
                  theAttributes->theProjectionCode = theAttributes->theProjectionCode.upcase();
                  if((theAttributes->theProjectionCode == "AC") ||
                     (theAttributes->theProjectionCode == "LE") ||
                     (theAttributes->theProjectionCode == "PG"))
                  {
                     in.read((char*)&(theAttributes->theProjectionA), 4);
                     in.read((char*)&(theAttributes->theProjectionB), 4);
                     in.read((char*)&(theAttributes->theProjectionC), 4);
                     in.read((char*)&(theAttributes->theProjectionD), 4);
                  }
                  else if((theAttributes->theProjectionCode == "RB") ||
                          (theAttributes->theProjectionCode == "OC") ||
                          (theAttributes->theProjectionCode == "TC"))
                  {
                     in.read((char*)&(theAttributes->theProjectionA), 4);
                     in.read((char*)&(theAttributes->theProjectionB), 4);
                     in.read((char*)&(theAttributes->theProjectionC), 4);
                  }
                  else if((theAttributes->theProjectionCode == "AL") ||
                          (theAttributes->theProjectionCode == "MC") ||
                          (theAttributes->theProjectionCode == "OD") ||
                          (theAttributes->theProjectionCode == "PH"))
                  {
                     in.read((char*)&(theAttributes->theProjectionA), 4);
                     in.read((char*)&(theAttributes->theProjectionB), 4);
                  }
                  else if((theAttributes->theProjectionCode == "UT"))
                  {
                     in.read((char*)&(theAttributes->theProjectionA), 4);
                  }
                  if(anEndian.getSystemEndianType() != theHeader->getByteOrder())
                  {
                     anEndian.swap(theAttributes->theProjectionA);
                     anEndian.swap(theAttributes->theProjectionB);
                     anEndian.swap(theAttributes->theProjectionC);
                     anEndian.swap(theAttributes->theProjectionD);
                  }
                  break;
               }
               case 6:
               {
                  in.read((char*)tempString, 4);
                  theAttributes->theVertDatumCode = ossimString(tempString,
                                                                &tempString[4]);
                  break;
               }
               case 7:
               {
                  in.read((char*)tempString, 4);
                  theAttributes->theHorDatumCode = ossimString(tempString,
                                                               &tempString[4]);
                  break;
               }
               case 8:
               {
                  in.read((char*)&(theAttributes->theVertAbsAccuracy), 4);
                  in.read((char*)&(theAttributes->theVertAbsUnits), 2);
                  if(anEndian.getSystemEndianType() != theHeader->getByteOrder())
                  {
                     anEndian.swap(theAttributes->theVertAbsAccuracy);
                     anEndian.swap(theAttributes->theVertAbsUnits);
                  }
                  break;
               }
               case 9:
               {
                  in.read((char*)&(theAttributes->theHorAbsAccuracy), 4);
                  in.read((char*)&(theAttributes->theHorAbsUnits), 2);
                  if(anEndian.getSystemEndianType() != theHeader->getByteOrder())
                  {
                     anEndian.swap(theAttributes->theHorAbsAccuracy);
                     anEndian.swap(theAttributes->theHorAbsUnits);
                  }
                  break;
               }
               case 10:
               {
                  in.read((char*)&(theAttributes->theVertRelAccuracy), 4);
                  in.read((char*)&(theAttributes->theVertRelUnits), 2);
                  if(anEndian.getSystemEndianType() != theHeader->getByteOrder())
                  {
                     anEndian.swap(theAttributes->theVertRelAccuracy);
                     anEndian.swap(theAttributes->theVertRelUnits);
                  }
                  break;
               }
               case 11:
               {
                  in.read((char*)&(theAttributes->theHorRelAccuracy), 4);
                  in.read((char*)&(theAttributes->theHorRelUnits), 2);
                  if(anEndian.getSystemEndianType() != theHeader->getByteOrder())
                  {
                     anEndian.swap(theAttributes->theHorRelAccuracy);
                     anEndian.swap(theAttributes->theHorRelUnits);
                  }
                  break;
               }            
               case 12:
               {
                  in.read((char*)tempString, 3);
                  theAttributes->ellipsoidCode = ossimString(tempString,
                                                             &tempString[3]);
                  break;
               }            
               case 13:
               {
                  in.read((char*)tempString, 4);
                  theAttributes->theSoundingDatumCode = ossimString(tempString,
                                                                    &tempString[4]);
                  break;
               }            
               case 14:
               {
                  in.read((char*)&(theAttributes->theNavSystemCode), 2);
                  
                  if(anEndian.getSystemEndianType() != theHeader->getByteOrder())
                  {
                     anEndian.swap(theAttributes->theNavSystemCode);
                  }
                  break;
               }            
               case 15:
               {
                  in.read((char*)tempString, 2);
                  theAttributes->theGridCode = ossimString(tempString,
                                                           &tempString[2]);
                  break;
               }            
               case 16:
               {
                  in.read((char*)&(theAttributes->theEeasterlyMagChange), 4);
                  in.read((char*)&(theAttributes->theEasterlyMagChangeUnits), 2);
                  if(anEndian.getSystemEndianType() != theHeader->getByteOrder())
                  {
                     anEndian.swap(theAttributes->theEeasterlyMagChange);
                     anEndian.swap(theAttributes->theEasterlyMagChangeUnits);
                  }
                  break;
               }
               case 17:
               {
                  in.read((char*)&(theAttributes->theWesterlyMagChange), 4);
                  in.read((char*)&(theAttributes->theWesterlyMagChangeUnits), 2);
                  if(anEndian.getSystemEndianType() != theHeader->getByteOrder())
                  {
                     anEndian.swap(theAttributes->theWesterlyMagChange);
                     anEndian.swap(theAttributes->theWesterlyMagChangeUnits);
                  }
                  break;
               }
               case 18:
               {
                  in.read((char*)&(theAttributes->theMagAngle), 4);
                  in.read((char*)&(theAttributes->theMagAngleUnits), 2);
                  if(anEndian.getSystemEndianType() != theHeader->getByteOrder())
                  {
                     anEndian.swap(theAttributes->theMagAngle);
                     anEndian.swap(theAttributes->theMagAngleUnits);
                  }
                  break;
               }
               case 19:
               {
                  in.read((char*)&(theAttributes->theGridConver), 4);
                  in.read((char*)&(theAttributes->theGridConverUnits), 2);
                  if(anEndian.getSystemEndianType() != theHeader->getByteOrder())
                  {
                     anEndian.swap(theAttributes->theGridConver);
                     anEndian.swap(theAttributes->theGridConverUnits);
                  }
                  break;
               }
               case 20:
               {
                  in.read((char*)&(theAttributes->theHighElevation), 8);
                  in.read((char*)&(theAttributes->theHighElevationUnits), 2);
                  in.read((char*)&(theAttributes->theHighLat), 8);
                  in.read((char*)&(theAttributes->theHighLon), 8);
                  if(anEndian.getSystemEndianType() != theHeader->getByteOrder())
                  {
                     anEndian.swap(theAttributes->theHighElevation);
                     anEndian.swap(theAttributes->theHighElevationUnits);
                     anEndian.swap(theAttributes->theHighLat);
                     anEndian.swap(theAttributes->theHighLon);
                  }
                  break;
               }
               case 21:
               {
                  in.read((char*)tempString, 12);
                  theAttributes->theLegendFileName = ossimString(tempString,
                                                                 &tempString[12]);
                  break;
               }
               case 22:
               {
                  in.read((char*)tempString, 12);
                  tempString[12] = '\0';
                  theAttributes->theDataSource = tempString;
                  in.read((char*)&(theAttributes->theGsd), 4);
                  if(anEndian.getSystemEndianType() != theHeader->getByteOrder())
                  {
                     anEndian.swap(theAttributes->theGsd);
                  }
                  break;
               }
               case 23:
               {
                  in.read((char*)&(theAttributes->theDataLevel), 2);
                  if(anEndian.getSystemEndianType() != theHeader->getByteOrder())
                  {
                     anEndian.swap(theAttributes->theDataLevel);
                  }
                  break;
               }
               }
            }
            in.seekg(currentLocation, ios::beg);
         }
         else
         {
            delete temp;
            temp = 0;
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG) << "ossimRpfFrame::populateAttributeSection:  leaving 2 ..." << std::endl;
            }
            return ossimErrorCodes::OSSIM_ERROR;
         }
      }
   }
   if(temp) delete temp;
   temp = 0;

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimRpfFrame::populateAttributeSection:  leaving OK ..." << std::endl;
   }
   return ossimErrorCodes::OSSIM_OK;
}

ossimErrorCode ossimRpfFrame::populateMasks(istream& in)
{
   theSubframeMaskTable.clear();
   theSubframeTransparencyMaskTable.clear();

   if(!in)
   {
      return ossimErrorCodes::OSSIM_ERROR;
   }
   
   if(theImageDescriptionSubheader)
   {
      const ossimRpfLocationSection* location = theHeader->getLocationSection();
      if(location)
      {
         ossimRpfComponentLocationRecord component;
         
         if(location->getComponent(OSSIM_RPF_MASK_SUBSECTION,
                                   component))
         {
            
            // now allocate the space we need and then reader the table in
            
            theSubframeMaskTable.resize(theImageDescriptionSubheader->getNumberOfSpectralGroups());
            theSubframeTransparencyMaskTable.resize(theImageDescriptionSubheader->getNumberOfSpectralGroups());
            
            ossim_uint32 spectralIndex;
            ossim_uint32 rowIndex;
            ossim_uint32 colIndex;
            
            if(!theImageDescriptionSubheader->isSubframeMaskTableOffsetNull())
            {
               // move get to the first byte of the Mask table.  This is the offset from the
               // start of the maskSubsection. 
               in.seekg(component.m_componentLocation +
                        theImageDescriptionSubheader->getSubframeMaskTableOffset(), ios::beg);
               // first loop through the Mask table and allocate while we do it
               for(spectralIndex = 0;
                   spectralIndex < theImageDescriptionSubheader->getNumberOfSpectralGroups();
                   ++spectralIndex)
               {
                  // reserve space for the number of rows
                  theSubframeMaskTable[spectralIndex].resize(theImageDescriptionSubheader->getNumberOfSubframesVertical());
                  for(rowIndex = 0;
                      rowIndex < theImageDescriptionSubheader->getNumberOfSubframesVertical();
                      ++rowIndex)
                  {
                     // allocate the columns
                     theSubframeMaskTable[spectralIndex][rowIndex].resize(theImageDescriptionSubheader->getNumberOfSubframesHorizontal());
                     for(colIndex = 0;
                         colIndex < theImageDescriptionSubheader->getNumberOfSubframesHorizontal();
                         ++colIndex)
                     {
                        ossim_uint32 value;
                        
                        in.read((char*)&value, 4);
                        ossimEndian anEndian;
                        
                        if(anEndian.getSystemEndianType() != theHeader->getByteOrder())
                        {
                           anEndian.swap(value);
                        }
                        theSubframeMaskTable[spectralIndex][rowIndex][colIndex] = value;
                     }
                  }
               }
            }
            if(!theImageDescriptionSubheader->isTransparencyMaskTableOffsetNull())
            {
               // move get to the first byte of the Mask table.  This is the offset from the
               // start of the maskSubsection. 
               in.seekg(component.m_componentLocation +
                        theImageDescriptionSubheader->getTransparencyMaskTableOffset(),ios::beg);
               for(spectralIndex = 0;
                   spectralIndex < theImageDescriptionSubheader->getNumberOfSpectralGroups();
                   ++spectralIndex)
               {
                  // reserve space for the number of rows
                  theSubframeTransparencyMaskTable[spectralIndex].resize(theImageDescriptionSubheader->getNumberOfSubframesVertical());
                  for(rowIndex = 0;
                      rowIndex < theImageDescriptionSubheader->getNumberOfSubframesVertical();
                      ++rowIndex)
                  {
                     // allocate the columns
                     theSubframeTransparencyMaskTable[spectralIndex][rowIndex].resize(theImageDescriptionSubheader->getNumberOfSubframesHorizontal());
                     for(colIndex = 0;
                         colIndex < theImageDescriptionSubheader->getNumberOfSubframesHorizontal();
                         ++colIndex)
                     {
                        ossim_uint32 value;
                        
                        in.read((char*)&value, 4);
                        ossimEndian anEndian;
                        
                        if(anEndian.getSystemEndianType() != theHeader->getByteOrder())
                        {
                           anEndian.swap(value);
                        }
                        theSubframeTransparencyMaskTable[spectralIndex][rowIndex][colIndex] = value;
                     }
                  }
               }
            }
         }
      }
   }
   
   return ossimErrorCodes::OSSIM_OK;
}

ossimErrorCode ossimRpfFrame::populateReplaceUpdateTable(std::istream& in)
{
   ossimErrorCode result = ossimErrorCodes::OSSIM_OK;
   
   const ossimRpfLocationSection* location = theHeader->getLocationSection();

   if( location )
   {
      if ( location->hasComponent(OSSIM_RPF_REPLACE_UPDATE_SECTION_SUBHEADER) )
      {
         ossimRpfComponentLocationRecord component;
         if( location->getComponent(OSSIM_RPF_REPLACE_UPDATE_SECTION_SUBHEADER, component) )
         {
            ossimRefPtr<ossimRpfReplaceUpdateSectionSubheader> hdr =
               new ossimRpfReplaceUpdateSectionSubheader();
            
            in.seekg(component.m_componentLocation, ios::beg);
            
            if( hdr->parseStream( in, theHeader->getByteOrder() ) == ossimErrorCodes::OSSIM_OK )
            {
               ossim_uint16 count = hdr->getNumberOfRecords();
               if ( count )
               {
                  if ( theReplaceUpdateTable.valid() )
                  {
                     theReplaceUpdateTable->clear();
                  }
                  else
                  {
                     theReplaceUpdateTable = new ossimRpfReplaceUpdateTable();
                  }
                  ossimRpfReplaceUpdateRecord record;
                  for ( ossim_uint16 i = 0; i < count; ++i )
                  {
                     if ( record.parseStream(in) == ossimErrorCodes::OSSIM_OK )
                     {
                        theReplaceUpdateTable->addRecord( record );
                     }
                     else
                     {
                        break;
                     }
                  }
               }
               
            }
         }  
      }
   }

   if ( in.fail() )
   {
      theReplaceUpdateTable = 0;
      result = ossimErrorCodes::OSSIM_ERROR;
   }

   return result;

} // End: ossimRpfFrame::populateReplaceUpdateTable(std::istream& in)

ossimRefPtr<ossimRpfReplaceUpdateTable> ossimRpfFrame::getRpfReplaceUpdateTable() const
{
   return theReplaceUpdateTable;
}
