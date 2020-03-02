/*
 * Copyright (C) 2017-2019 CS Systemes d'Information (CS SI)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbMultiImageFileWriter_hxx
#define otbMultiImageFileWriter_hxx

#include "otbMultiImageFileWriter.h"
#include "otbImageIOFactory.h"
#include "otbMacro.h"

namespace otb
{

template <class TImage>
MultiImageFileWriter::Sink<TImage>::Sink(typename TImage::ConstPointer inputImage, const std::string& fileName)
  : SinkBase(dynamic_cast<const ImageBaseType*>(inputImage.GetPointer())), m_Writer(otb::ImageFileWriter<TImage>::New()), m_ImageIO(NULL)
{
  m_Writer->SetFileName(fileName);
  m_Writer->SetInput(inputImage);
}

template <class TImage>
MultiImageFileWriter::Sink<TImage>::Sink(typename otb::ImageFileWriter<TImage>::Pointer writer)
  : SinkBase(dynamic_cast<const ImageBaseType*>(writer->GetInput())), m_Writer(writer), m_ImageIO(NULL)
{
}

template <class TImage>
bool MultiImageFileWriter::Sink<TImage>::CanStreamWrite() const
{
  if (m_ImageIO.IsNull())
    return false;
  return m_ImageIO->CanStreamWrite();
}

template <class TImage>
void MultiImageFileWriter::Sink<TImage>::WriteImageInformation()
{
  m_Writer->UpdateOutputInformation();
  m_ImageIO = m_Writer->GetImageIO();
}

template <class TImage>
void MultiImageFileWriter::Sink<TImage>::Write(const RegionType& streamRegion)
{
  // Write the image stream
  itk::ImageIORegion ioRegion(TImage::ImageDimension);
  for (unsigned int i = 0; i < TImage::ImageDimension; ++i)
  {
    ioRegion.SetSize(i, streamRegion.GetSize(i));
    ioRegion.SetIndex(i, streamRegion.GetIndex(i));
  }
  m_ImageIO->SetIORegion(ioRegion);
  m_Writer->UpdateOutputData(nullptr);
}

template <class TImage>
itk::ImageRegion<2>
MultiImageFileWriter::Sink<TImage>::GetRegionToWrite() const
{
  auto fnameHelper = m_Writer->GetFilenameHelper();
  if (fnameHelper->BoxIsSet())
  {
    std::vector<unsigned int> boxVector;
    Utils::ConvertStringToVector(fnameHelper->GetBox(), boxVector, "ExtendedFileName:box", ":");

    if (boxVector.size() != 4)
    {
      itk::ImageFileWriterException e(__FILE__, __LINE__);
      std::ostringstream            msg;
      msg << "Invalid box option " << fnameHelper->GetBox() << ". The box should contains four elements: startx:starty:sizex:sizey";
      e.SetDescription(msg.str());
      e.SetLocation(ITK_LOCATION);
      throw e;
    }
    
    // indices are signed, sizes are unsigned
    typename itk::ImageRegion<2>::IndexType start {{static_cast<int>(boxVector[0]), static_cast<int>(boxVector[1])}};
    typename itk::ImageRegion<2>::SizeType size {{boxVector[2], boxVector[3]}};
    itk::ImageRegion<2> regionToWrite {start , size};
    regionToWrite.Crop(m_InputImage->GetLargestPossibleRegion());
    return regionToWrite;
  }
  else
  {
    return m_InputImage->GetLargestPossibleRegion();
  }
}

template <class TWriter>
void MultiImageFileWriter::AddInputWriter(typename TWriter::Pointer writer)
{
  Sink<typename TWriter::InputImageType>* sink = new Sink<typename TWriter::InputImageType>(writer);
  m_SinkList.push_back(SinkBase::Pointer(sink));
  unsigned int size = m_SinkList.size();
  this->SetNthInput(size - 1, const_cast<itk::DataObject*>(dynamic_cast<const itk::DataObject*>(writer->GetInput())));
      /** Parse streaming modes */
  
  auto filenameHelper = writer->GetFilenameHelper();
  if (filenameHelper->StreamingTypeIsSet())
  {
    otbLogMacro(
        Warning,
        << "Streaming configuration through extended filename is used. Any previous streaming configuration (ram value, streaming mode ...) will be ignored.");

    std::string type = filenameHelper->GetStreamingType();

    std::string sizemode = "auto";

    if (filenameHelper->StreamingSizeModeIsSet())
    {
      sizemode = filenameHelper->GetStreamingSizeMode();
    }
  
    unsigned int sizevalue = 0;
    // Save the DefaultRAM value for later
    unsigned int oldDefaultRAM = m_StreamingManager->GetDefaultRAM();
    if (sizemode == "auto")
    {
      sizevalue = oldDefaultRAM;
    }

    if (filenameHelper->StreamingSizeValueIsSet())
    {
      sizevalue = static_cast<unsigned int>(filenameHelper->GetStreamingSizeValue());
    }

    if (type == "auto")
    {
      if (sizemode != "auto")
      {
        otbLogMacro(Warning, << "In auto streaming type, the sizemode option will be ignored.");
      }
      if (sizevalue == 0)
      {
        otbLogMacro(Warning, << "sizemode is auto but sizevalue is 0. Value will be fetched from the OTB_MAX_RAM_HINT environment variable if set, or else use "
                                "the default value");
      }
      this->SetAutomaticAdaptativeStreaming(sizevalue);
    }
    else if (type == "tiled")
    {
      if (sizemode == "auto")
      {
        if (sizevalue == 0)
        {
          otbLogMacro(Warning, << "sizemode is auto but sizevalue is 0. Value will be fetched from the OTB_MAX_RAM_HINT environment variable if set, or else "
                                  "use the default value");
        }
        this->SetAutomaticTiledStreaming(sizevalue);
      }
      else if (sizemode == "nbsplits")
      {
        if (sizevalue == 0)
        {
          otbLogMacro(Warning, << "Streaming sizemode is set to nbsplits but sizevalue is 0. This will result in undefined behaviour. Please consider setting "
                                  "the sizevalue by using &streaming:sizevalue=x.");
        }
        this->SetNumberOfDivisionsTiledStreaming(sizevalue);
      }
      else if (sizemode == "height")
      {
        if (sizevalue == 0)
        {
          otbLogMacro(Warning, << "Streaming sizemode is set to height but sizevalue is 0. This will result in undefined behaviour. Please consider setting "
                                  "the sizevalue by using &streaming:sizevalue=x.");
        }

        this->SetTileDimensionTiledStreaming(sizevalue);
      }
    }
    else if (type == "stripped")
    {
      if (sizemode == "auto")
      {
        if (sizevalue == 0)
        {
          otbLogMacro(
              Warning, << "sizemode is auto but sizevalue is 0. Value will be fetched from configuration file if any, or from cmake configuration otherwise.");
        }

        this->SetAutomaticStrippedStreaming(sizevalue);
      }
      else if (sizemode == "nbsplits")
      {
        if (sizevalue == 0)
        {
          otbLogMacro(Warning, << "Streaming sizemode is set to nbsplits but sizevalue is 0. This will result in undefined behaviour. Please consider setting "
                                  "the sizevalue by using &streaming:sizevalue=x.");
        }
        this->SetNumberOfDivisionsStrippedStreaming(sizevalue);
      }
      else if (sizemode == "height")
      {
        if (sizevalue == 0)
        {
          otbLogMacro(Warning, << "Streaming sizemode is set to height but sizevalue is 0. This will result in undefined behaviour. Please consider setting "
                                  "the sizevalue by using &streaming:sizevalue=x.");
        }
        this->SetNumberOfLinesStrippedStreaming(sizevalue);
      }
    }
    else if (type == "none")
    {
      if (sizemode != "" || sizevalue != 0)
      {
        otbLogMacro(Warning, << "Streaming is explicitly disabled, sizemode and sizevalue will be ignored.");
      }
      this->SetNumberOfDivisionsTiledStreaming(0);
    }
    m_StreamingManager->SetDefaultRAM(oldDefaultRAM);
  }
  else
  {
    if (filenameHelper->StreamingSizeValueIsSet() || filenameHelper->StreamingSizeModeIsSet())
    {
      otbLogMacro(Warning, << "No streaming type is set, streaming sizemode and sizevalue will be ignored.");
    }
  }
}

template <class TImage>
void MultiImageFileWriter::AddInputImage(const TImage* inputPtr, const std::string& fileName)
{
  Sink<TImage>* sink = new Sink<TImage>(inputPtr, fileName);
  m_SinkList.push_back(SinkBase::Pointer(sink));
  unsigned int size = m_SinkList.size();
  this->SetNthInput(size - 1, const_cast<itk::DataObject*>(dynamic_cast<const itk::DataObject*>(inputPtr)));
}

} // end of namespace otb

#endif // otbMultiImageFileWriter_hxx
