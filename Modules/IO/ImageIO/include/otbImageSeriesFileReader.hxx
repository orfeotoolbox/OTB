/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2007-2012 Institut Mines Telecom / Telecom Bretagne
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


#ifndef otbImageSeriesFileReader_hxx
#define otbImageSeriesFileReader_hxx
#include "otbImageSeriesFileReader.h"

namespace otb
{

template <class TImage, class TInternalImage>
ImageSeriesFileReader<TImage, TInternalImage>::ImageSeriesFileReader()
{
  m_ExtractorList = ExtractSelectionListType::New();
}

template <class TImage, class TInternalImage>
void ImageSeriesFileReader<TImage, TInternalImage>::AllocateListOfComponents()
{
  for (unsigned int i = 0; i < this->GetNumberOfOutputs(); ++i)
  {
    this->m_ImageFileReaderList->PushBack(ReaderType::New());
    this->m_OutputList->PushBack(OutputImageType::New());
    m_ExtractorList->PushBack(ExtractSelectionListType::New());
  }
}

template <class TImage, class TInternalImage>
void ImageSeriesFileReader<TImage, TInternalImage>::GenerateData(DataObjectPointerArraySizeType itkNotUsed(idx))
{
  std::ostringstream msg;
  msg << "Something wrong... Check the template definition of this class in the program...\n";
  msg << "\"ENVI META FILE\" FileName: " << this->m_FileName << "\n";
  ImageSeriesFileReaderException e(__FILE__, __LINE__, msg.str(), ITK_LOCATION);
  throw e;
}

/* **********************************************************
 * Methods that are specific to instantiation with Image type
 * **********************************************************
 */

template <class TPixel, class TInternalPixel>
ImageSeriesFileReader<Image<TPixel, 2>, Image<TInternalPixel, 2>>::ImageSeriesFileReader()
{
  m_ExtractorList = ExtractSelectionListType::New();
}

/**
 * Allocation of the component... Here, based on ExtractROI
 */
template <class TPixel, class TInternalPixel>
void ImageSeriesFileReader<Image<TPixel, 2>, Image<TInternalPixel, 2>>::AllocateListOfComponents()
{
  for (unsigned int i = 0; i < this->GetNumberOfOutputs(); ++i)
  {
    this->m_ImageFileReaderList->PushBack(ReaderType::New());
    this->m_OutputList->PushBack(OutputImageType::New());
    m_ExtractorList->PushBack(ExtractSelectionType::New());
  }
}

/**
 * TestBandSelection tests if the templated Image type is compatible
 * with the bande selection provided in the Meta File
 */
template <class TPixel, class TInternalPixel>
void ImageSeriesFileReader<Image<TPixel, 2>, Image<TInternalPixel, 2>>::TestBandSelection(std::vector<unsigned int>& bands)
{
  if (bands.size() != 1)
  {
    std::ostringstream msg;
    msg << "Unable to handle multicomponent file from Image<> class\n";
    msg << "\"ENVI META FILE\" FileName: " << this->m_FileName << "\n";
    ImageSeriesFileReaderException e(__FILE__, __LINE__, msg.str(), ITK_LOCATION);
    throw e;
  }

  if (bands[0] != 1)
  {
    std::ostringstream msg;
    msg << "Unable to handle given band reading from multicomponent file with Image<> class\n";
    msg << "\"ENVI META FILE\" FileName: " << this->m_FileName << "\n";
    ImageSeriesFileReaderException e(__FILE__, __LINE__, msg.str(), ITK_LOCATION);
    throw e;
  }
  return;
}

/**
 * GenerateData for Image type
 */
template <class TPixel, class TInternalPixel>
void ImageSeriesFileReader<Image<TPixel, 2>, Image<TInternalPixel, 2>>::GenerateData(DataObjectPointerArraySizeType idx)
{
  otbMsgDebugMacro(<< "Reading " << idx << "th image: " << this->m_ListOfFileNames[idx]);

  ReaderType* reader = static_cast<ReaderType*>(this->m_ImageFileReaderList->GetNthElement(idx));

  reader->SetFileName(this->m_ListOfFileNames[idx]);

  ExtractSelectionType* selection = static_cast<ExtractSelectionType*>(m_ExtractorList->GetNthElement(idx));

  selection->SetExtractionRegion(this->m_ListOfRegionSelection[idx]);

  selection->SetInput(reader->GetOutput());
  selection->GraftOutput(this->m_OutputList->GetNthElement(idx));
  selection->Update();

  this->m_OutputList->GetNthElement(idx)->Graft(selection->GetOutput());
}

/* *********************************************************************
 * Methods that are specific to instantiation with Image type for TImage
 * and VectorImage as TInternalImage
 * *********************************************************************
 */

/**
 * Constructor
 */
template <class TPixel, class TInternalPixel>
ImageSeriesFileReader<Image<TPixel, 2>, VectorImage<TInternalPixel, 2>>::ImageSeriesFileReader()
{
  // this->m_OutputList = OutputImageListType::New();
  // this->m_ImageFileReaderList = ReaderListType::New();
  m_ExtractorList = ExtractSelectionListType::New();
}

/**
 * Allocation of the component... Here, based on MultiToMonoChannelExtractROI
 */
template <class TPixel, class TInternalPixel>
void ImageSeriesFileReader<Image<TPixel, 2>, VectorImage<TInternalPixel, 2>>::AllocateListOfComponents()
{
  for (unsigned int i = 0; i < this->GetNumberOfOutputs(); ++i)
  {
    this->m_ImageFileReaderList->PushBack(ReaderType::New());
    this->m_OutputList->PushBack(OutputImageType::New());
    m_ExtractorList->PushBack(ExtractSelectionType::New());
  }
}

/**
 * TestBandSelection tests if the templated Image type is compatible
 * with the bande selection provided in the Meta File
 */
template <class TPixel, class TInternalPixel>
void ImageSeriesFileReader<Image<TPixel, 2>, VectorImage<TInternalPixel, 2>>::TestBandSelection(std::vector<unsigned int>& bands)
{
  if (bands.size() != 1)
  {
    std::ostringstream msg;
    msg << "Unable to handle multicomponent file from Image<> class as output\n";
    msg << "\"ENVI META FILE\" FileName: " << this->m_FileName << "\n";
    ImageSeriesFileReaderException e(__FILE__, __LINE__, msg.str(), ITK_LOCATION);
    throw e;
  }
  return;
}

/**
 * GenerateData for Image type as output and VectorImage type for reading
 */
template <class TPixel, class TInternalPixel>
void ImageSeriesFileReader<Image<TPixel, 2>, VectorImage<TInternalPixel, 2>>::GenerateData(DataObjectPointerArraySizeType idx)
{
  otbMsgDebugMacro(<< "Reading " << idx << "th image: " << this->m_ListOfFileNames[idx]);

  ReaderType* reader = static_cast<ReaderType*>(this->m_ImageFileReaderList->GetNthElement(idx));

  reader->SetFileName(this->m_ListOfFileNames[idx]);

  ExtractSelectionType* selection = static_cast<ExtractSelectionType*>(this->m_ExtractorList->GetNthElement(idx));

  selection->SetExtractionRegion(this->m_ListOfRegionSelection[idx]);
  selection->SetChannel(this->m_ListOfBandSelection[idx][0]);

  selection->SetInput(reader->GetOutput());
  selection->GraftOutput(this->m_OutputList->GetNthElement(idx));
  selection->Update();

  this->m_OutputList->GetNthElement(idx)->Graft(selection->GetOutput());
}

/* ******************************************************************
 * Methods that are specific to instantiation with VectorImage types
 * ******************************************************************
 */

/**
 * Constructor
 */
template <class TPixel, class TInternalPixel>
ImageSeriesFileReader<VectorImage<TPixel, 2>, VectorImage<TInternalPixel, 2>>::ImageSeriesFileReader()
{
  // this->m_OutputList = OutputImageListType::New();
  // this->m_ImageFileReaderList = ReaderListType::New();
  m_ExtractorList = ExtractSelectionListType::New();
}

/**
 * Allocation of the component... Here, based on MultiChannelExtractROI
 */
template <class TPixel, class TInternalPixel>
void ImageSeriesFileReader<VectorImage<TPixel, 2>, VectorImage<TInternalPixel, 2>>::AllocateListOfComponents()
{
  for (unsigned int i = 0; i < this->GetNumberOfOutputs(); ++i)
  {
    this->m_ImageFileReaderList->PushBack(ReaderType::New());
    this->m_OutputList->PushBack(OutputImageType::New());
    m_ExtractorList->PushBack(ExtractSelectionType::New());
  }
}

/**
 * GenerateData specialised for VectorImages
 */
template <class TPixel, class TInternalPixel>
void ImageSeriesFileReader<VectorImage<TPixel, 2>, VectorImage<TInternalPixel, 2>>::GenerateData(DataObjectPointerArraySizeType idx)
{
  otbMsgDebugMacro(<< "Reading " << idx << "th image: " << this->m_ListOfFileNames[idx]);

  ReaderType* reader = static_cast<ReaderType*>(this->m_ImageFileReaderList->GetNthElement(idx));

  reader->SetFileName(this->m_ListOfFileNames[idx]);

  ExtractSelectionType* selection = static_cast<ExtractSelectionType*>(this->m_ExtractorList->GetNthElement(idx));

  selection->SetExtractionRegion(this->m_ListOfRegionSelection[idx]);

  for (std::vector<unsigned int>::iterator band = this->m_ListOfBandSelection[idx].begin(); band != this->m_ListOfBandSelection[idx].end(); ++band)
  {
    selection->SetChannel(*band);
  }

  selection->SetInput(reader->GetOutput());
  selection->GraftOutput(this->m_OutputList->GetNthElement(idx));
  selection->Update();

  this->m_OutputList->GetNthElement(idx)->Graft(selection->GetOutput());
}

} // end of namespace otb

#endif
