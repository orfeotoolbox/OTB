/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:          $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Mines-Telecom. All rights reserved.
  See IMTCopyright.txt for details.

         This software is distributed WITHOUT ANY WARRANTY; without even
         the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
         PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbImageSeriesFileReader_txx
#define __otbImageSeriesFileReader_txx
#include "otbImageSeriesFileReader.h"

namespace otb {

template <class TImage, class TInternalImage>
ImageSeriesFileReader<TImage, TInternalImage>
::ImageSeriesFileReader ()
{
  m_ExtractorList = ExtractSelectionListType::New();
}

template <class TImage, class TInternalImage>
void
ImageSeriesFileReader<TImage, TInternalImage>
::AllocateListOfComponents()
{
  for (unsigned int i = 0; i < this->GetNumberOfOutputs(); ++i)
    {
    this->m_ImageFileReaderList->PushBack(ReaderType::New());
    this->m_OutputList->PushBack(OutputImageType::New());
    m_ExtractorList->PushBack(ExtractSelectionListType::New());
    }
}

template <class TImage, class TInternalImage>
void
ImageSeriesFileReader<TImage, TInternalImage>
::GenerateData(DataObjectPointerArraySizeType itkNotUsed(idx))
{
  std::ostringstream msg;
  msg << "Something wrong... Check the template definition of this class in the program...\n";
  msg << "\"ENVI META FILE\" FileName: " << this->m_FileName << "\n";
  ImageSeriesFileReaderException e(__FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION);
  throw e;
}

/* **********************************************************
 * Methods that are specific to instanciation with Image type
 * **********************************************************
 */

template <class TPixel, class TInternalPixel>
ImageSeriesFileReader<Image<TPixel, 2>, Image<TInternalPixel, 2> >
::ImageSeriesFileReader ()
{
  m_ExtractorList = ExtractSelectionListType::New();
}

/**
 * Allocation of the component... Here, based on ExtractROI
 */
template <class TPixel, class TInternalPixel>
void
ImageSeriesFileReader<Image<TPixel, 2>, Image<TInternalPixel, 2> >
::AllocateListOfComponents()
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
void
ImageSeriesFileReader<Image<TPixel, 2>, Image<TInternalPixel, 2> >
::TestBandSelection(std::vector<unsigned int>& bands)
{
  if (bands.size() != 1)
    {
    std::ostringstream msg;
    msg << "Unable to handle multicomponent file from Image<> class\n";
    msg << "\"ENVI META FILE\" FileName: " << this->m_FileName << "\n";
    ImageSeriesFileReaderException e(__FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION);
    throw e;
    }

  if (bands[0] != 1)
    {
    std::ostringstream msg;
    msg << "Unable to handle given band reading from multicomponent file with Image<> class\n";
    msg << "\"ENVI META FILE\" FileName: " << this->m_FileName << "\n";
    ImageSeriesFileReaderException e(__FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION);
    throw e;
    }
  return;
}

/**
 * GenerateData for Image type
 */
template <class TPixel, class TInternalPixel>
void
ImageSeriesFileReader<Image<TPixel, 2>, Image<TInternalPixel, 2> >
::GenerateData(DataObjectPointerArraySizeType idx)
{
  otbMsgDebugMacro(<< "Reading " << idx << "th image: " << this->m_ListOfFileNames[idx]);

  ReaderType * reader
    = static_cast<ReaderType*>(this->m_ImageFileReaderList->GetNthElement(idx));

  reader->SetFileName(this->m_ListOfFileNames[idx]);

  ExtractSelectionType * selection
    = static_cast<ExtractSelectionType*>(m_ExtractorList->GetNthElement(idx));

  selection->SetExtractionRegion(this->m_ListOfRegionSelection[idx]);

  selection->SetInput(reader->GetOutput());
  selection->GraftOutput(this->m_OutputList->GetNthElement(idx));
  selection->Update();

  this->m_OutputList->GetNthElement(idx)->Graft(selection->GetOutput());
}

/* *********************************************************************
 * Methods that are specific to instanciation with Image type for TImage
 * and VectorImage as TInternalImage
 * *********************************************************************
 */

/**
 * Constructor
 */
template <class TPixel, class TInternalPixel>
ImageSeriesFileReader<Image<TPixel, 2>, VectorImage<TInternalPixel, 2> >
::ImageSeriesFileReader ()
{
  //this->m_OutputList = OutputImageListType::New();
  //this->m_ImageFileReaderList = ReaderListType::New();
  m_ExtractorList = ExtractSelectionListType::New();
}

/**
 * Allocation of the component... Here, based on MultiToMonoChannelExtractROI
 */
template <class TPixel, class TInternalPixel>
void
ImageSeriesFileReader<Image<TPixel, 2>, VectorImage<TInternalPixel, 2> >
::AllocateListOfComponents()
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
void
ImageSeriesFileReader<Image<TPixel, 2>, VectorImage<TInternalPixel, 2> >
::TestBandSelection(std::vector<unsigned int>& bands)
{
  if (bands.size() != 1)
    {
    std::ostringstream msg;
    msg << "Unable to handle multicomponent file from Image<> class as output\n";
    msg << "\"ENVI META FILE\" FileName: " << this->m_FileName << "\n";
    ImageSeriesFileReaderException e(__FILE__, __LINE__, msg.str().c_str(), ITK_LOCATION);
    throw e;
    }
  return;
}

/**
 * GenerateData for Image type as output and VectorImage type for reading
 */
template <class TPixel, class TInternalPixel>
void
ImageSeriesFileReader<Image<TPixel, 2>, VectorImage<TInternalPixel, 2> >
::GenerateData(DataObjectPointerArraySizeType idx)
{
  otbMsgDebugMacro(<< "Reading " << idx << "th image: " << this->m_ListOfFileNames[idx]);

  ReaderType * reader
    = static_cast<ReaderType*>(this->m_ImageFileReaderList->GetNthElement(idx));

  reader->SetFileName(this->m_ListOfFileNames[idx]);

  ExtractSelectionType * selection
    = static_cast<ExtractSelectionType*>(this->m_ExtractorList->GetNthElement(idx));

  selection->SetExtractionRegion(this->m_ListOfRegionSelection[idx]);
  selection->SetChannel(this->m_ListOfBandSelection[idx][0]);

  selection->SetInput(reader->GetOutput());
  selection->GraftOutput(this->m_OutputList->GetNthElement(idx));
  selection->Update();

  this->m_OutputList->GetNthElement(idx)->Graft(selection->GetOutput());
}

/* ******************************************************************
 * Methods that are specific to instanciation with VectorImage types
 * ******************************************************************
 */

/**
 * Constructor
 */
template <class TPixel, class TInternalPixel>
ImageSeriesFileReader<VectorImage<TPixel, 2>, VectorImage<TInternalPixel, 2> >
::ImageSeriesFileReader ()
{
  //this->m_OutputList = OutputImageListType::New();
  //this->m_ImageFileReaderList = ReaderListType::New();
  m_ExtractorList = ExtractSelectionListType::New();
}

/**
 * Allocation of the component... Here, based on MultiChannelExtractROI
 */
template <class TPixel, class TInternalPixel>
void
ImageSeriesFileReader<VectorImage<TPixel, 2>, VectorImage<TInternalPixel, 2> >
::AllocateListOfComponents()
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
void
ImageSeriesFileReader<VectorImage<TPixel, 2>, VectorImage<TInternalPixel, 2> >
::GenerateData(DataObjectPointerArraySizeType idx)
{
  otbMsgDebugMacro(<< "Reading " << idx << "th image: " << this->m_ListOfFileNames[idx]);

  ReaderType * reader
    = static_cast<ReaderType*>(this->m_ImageFileReaderList->GetNthElement(idx));

  reader->SetFileName(this->m_ListOfFileNames[idx]);

  ExtractSelectionType * selection
    = static_cast<ExtractSelectionType*>(this->m_ExtractorList->GetNthElement(idx));

  selection->SetExtractionRegion(this->m_ListOfRegionSelection[idx]);

  for (std::vector<unsigned int>::iterator band = this->m_ListOfBandSelection[idx].begin();
       band != this->m_ListOfBandSelection[idx].end();
       ++band)
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
