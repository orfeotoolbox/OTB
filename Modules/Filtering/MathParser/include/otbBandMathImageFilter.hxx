/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbBandMathImageFilter_hxx
#define otbBandMathImageFilter_hxx
#include "otbBandMathImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "otbMacro.h"


#include <iostream>
#include <string>

namespace otb
{

/** Constructor */
template <class TImage>
BandMathImageFilter<TImage>
::BandMathImageFilter()
{
  //This number will be incremented each time an image
  //is added over the one minimumrequired
  this->SetNumberOfRequiredInputs( 1 );
  this->InPlaceOff();

  m_UnderflowCount = 0;
  m_OverflowCount = 0;
  m_ThreadUnderflow.SetSize(1);
  m_ThreadOverflow.SetSize(1);
}

/** Destructor */
template <class TImage>
BandMathImageFilter<TImage>
::~BandMathImageFilter()
{
}

template <class TImage>
void BandMathImageFilter<TImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Expression: "      << m_Expression                  << std::endl;
  os << indent << "Computed values follow:"                            << std::endl;
  os << indent << "UnderflowCount: "  << m_UnderflowCount              << std::endl;
  os << indent << "OverflowCount: "   << m_OverflowCount               << std::endl;
  os << indent << "itk::NumericTraits<PixelType>::NonpositiveMin()  :  "
               << itk::NumericTraits<PixelType>::NonpositiveMin()      << std::endl;
  os << indent << "itk::NumericTraits<PixelType>::max()  :             "
         << itk::NumericTraits<PixelType>::max()                 << std::endl;
}

template <class TImage>
void BandMathImageFilter<TImage>
::SetNthInput(DataObjectPointerArraySizeType idx, const ImageType * image)
{
  this->SetInput(idx, const_cast<TImage *>( image ));
  DataObjectPointerArraySizeType nbInput = this->GetNumberOfInputs();
  m_VVarName.resize(nbInput+4);
  std::ostringstream varName;
  varName << "b" << nbInput;
  m_VVarName[idx] = varName.str();

  m_VVarName[idx+1] = "idxX";
  m_VVarName[idx+2] = "idxY";
  m_VVarName[idx+3] = "idxPhyX";
  m_VVarName[idx+4] = "idxPhyY";
}

template <class TImage>
void BandMathImageFilter<TImage>
::SetNthInput(DataObjectPointerArraySizeType idx, const ImageType * image, const std::string& varName)
{
  this->SetInput(idx, const_cast<TImage *>( image ));
  m_VVarName.resize(this->GetNumberOfInputs()+4);
  m_VVarName[idx] = varName;

  m_VVarName[idx+1] = "idxX";
  m_VVarName[idx+2] = "idxY";
  m_VVarName[idx+3] = "idxPhyX";
  m_VVarName[idx+4] = "idxPhyY";
}

template <class TImage>
void BandMathImageFilter<TImage>
::SetNthInputName(DataObjectPointerArraySizeType idx, const std::string& varName)
{
  m_VVarName[idx] = varName;
}

template <typename TImage>
TImage * BandMathImageFilter<TImage>
::GetNthInput(DataObjectPointerArraySizeType idx)
{
  return const_cast<TImage *>(this->GetInput(idx));
}

template< typename TImage >
void BandMathImageFilter<TImage>
::SetExpression(const std::string& expression)
{
  if (m_Expression != expression)
    m_Expression = expression;
  this->Modified();
}

template< typename TImage >
std::string BandMathImageFilter<TImage>
::GetExpression() const
{
  return m_Expression;
}

template< typename TImage >
std::string BandMathImageFilter<TImage>
::GetNthInputName(DataObjectPointerArraySizeType idx) const
{
  return m_VVarName.at(idx);
}

template< typename TImage >
void BandMathImageFilter<TImage>
::BeforeThreadedGenerateData()
{
  typename std::vector<ParserType::Pointer>::iterator        itParser;
  typename std::vector< std::vector<PixelType> >::iterator   itVImage;
  unsigned int nbThreads = this->GetNumberOfThreads();
  unsigned int nbInputImages = this->GetNumberOfInputs();
  unsigned int nbAccessIndex = 4; //to give access to image and physical index
  unsigned int i, j;
  unsigned int inputSize[2];
  std::vector< std::string > tmpIdxVarNames;

  tmpIdxVarNames.resize(nbAccessIndex);

  tmpIdxVarNames.resize(nbAccessIndex);
  tmpIdxVarNames[0] = "idxX";
  tmpIdxVarNames[1] = "idxY";
  tmpIdxVarNames[2] = "idxPhyX";
  tmpIdxVarNames[3] = "idxPhyY";

  // Check if input image dimensions matches
  inputSize[0] = this->GetNthInput(0)->GetLargestPossibleRegion().GetSize(0);
  inputSize[1] = this->GetNthInput(0)->GetLargestPossibleRegion().GetSize(1);

  for(unsigned int p = 1; p < nbInputImages; p++)
    {
    if((inputSize[0] != this->GetNthInput(p)->GetLargestPossibleRegion().GetSize(0))
       || (inputSize[1] != this->GetNthInput(p)->GetLargestPossibleRegion().GetSize(1)))
      {
      itkExceptionMacro(<< "Input images must have the same dimensions." << std::endl
                        << "band #1 is [" << inputSize[0] << ";" << inputSize[1] << "]" << std::endl
                        << "band #" << p+1 << " is ["
                        << this->GetNthInput(p)->GetLargestPossibleRegion().GetSize(0) << ";"
                        << this->GetNthInput(p)->GetLargestPossibleRegion().GetSize(1) << "]");
      }
    }

  // Store images specs
  m_Spacing = this->GetNthInput(0)->GetSignedSpacing();
  m_Origin = this->GetNthInput(0)->GetOrigin();

  // Allocate and initialize the thread temporaries
  m_ThreadUnderflow.SetSize(nbThreads);
  m_ThreadUnderflow.Fill(0);
  m_ThreadOverflow.SetSize(nbThreads);
  m_ThreadOverflow.Fill(0);
  m_VParser.resize(nbThreads);
  m_AImage.resize(nbThreads);
  m_NbVar = nbInputImages+nbAccessIndex;
  m_VVarName.resize(m_NbVar);

  for(itParser = m_VParser.begin(); itParser < m_VParser.end(); itParser++)
    {
    *itParser = ParserType::New();
    }

  for(i = 0; i < nbThreads; ++i)
    {
    m_AImage[i].resize(m_NbVar);
    m_VParser[i]->SetExpr(m_Expression);

    for(j=0; j < nbInputImages; ++j)
      {
      m_VParser[i]->DefineVar(m_VVarName[j], &(m_AImage[i][j]));
      }

    for(j=nbInputImages; j < nbInputImages+nbAccessIndex; ++j)
      {
      m_VVarName[j] = tmpIdxVarNames[j-nbInputImages];
      m_VParser[i]->DefineVar(m_VVarName[j], &(m_AImage[i][j]));
      }
    }
}

template< typename TImage >
void BandMathImageFilter<TImage>
::AfterThreadedGenerateData()
{
  unsigned int nbThreads = this->GetNumberOfThreads();
  unsigned int i;

  m_UnderflowCount = 0;
  m_OverflowCount = 0;

  // Accumulate counts for each thread
  for(i = 0; i < nbThreads; ++i)
    {
    m_UnderflowCount += m_ThreadUnderflow[i];
    m_OverflowCount += m_ThreadOverflow[i];
    }

  if((m_UnderflowCount != 0) || (m_OverflowCount!=0))
    otbWarningMacro(<< std::endl
        << "The Following Parsed Expression  :  "
        << this->GetExpression()                                 << std::endl
        << "Generated " << m_UnderflowCount << " Underflow(s) "
        << "And " << m_OverflowCount        << " Overflow(s) "   << std::endl
        << "The Parsed Expression, The Inputs And The Output "
        << "Type May Be Incompatible !");
}

template< typename TImage >
void BandMathImageFilter<TImage>
::ThreadedGenerateData(const ImageRegionType& outputRegionForThread,
           itk::ThreadIdType threadId)
{
  double value;
  unsigned int j;
  unsigned int nbInputImages = this->GetNumberOfInputs();

  typedef itk::ImageRegionConstIterator<TImage> ImageRegionConstIteratorType;

  assert(nbInputImages);
  std::vector< ImageRegionConstIteratorType > Vit(nbInputImages);

  for(j=0; j < nbInputImages; ++j)
    {
    Vit[j] = ImageRegionConstIteratorType (this->GetNthInput(j), outputRegionForThread);
    }

  itk::ImageRegionIterator<TImage> ot (this->GetOutput(), outputRegionForThread);

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  std::vector<double>      & threadImage     = m_AImage[threadId];
  ParserType::Pointer const& threadParser    = m_VParser[threadId];
  long                     & threadUnderflow = m_ThreadUnderflow[threadId];
  long                     & threadOverflow  = m_ThreadOverflow[threadId];
  ImageRegionConstIteratorType & firstImageRegion = Vit.front(); // alias for better perfs
  while(!firstImageRegion.IsAtEnd())
     {
     for(j=0; j < nbInputImages; ++j)
       {
       threadImage[j] = static_cast<double>(Vit[j].Get());
       }

    // Image Indexes
    for(j=0; j < 2; ++j)
      {
      threadImage[nbInputImages+j]   = static_cast<double>(firstImageRegion.GetIndex()[j]);
      }
    for(j=0; j < 2; ++j)
      {
      threadImage[nbInputImages+2+j] = static_cast<double>(m_Origin[j])
        + static_cast<double>(firstImageRegion.GetIndex()[j]) * static_cast<double>(m_Spacing[j]);
      }

    try
      {

      value = threadParser->Eval();
      }
    catch(itk::ExceptionObject& err)
      {
      itkExceptionMacro(<< err);
      }

    // Case value is equal to -inf or inferior to the minimum value
    // allowed by the pixelType cast
    if (value < double(itk::NumericTraits<PixelType>::NonpositiveMin()))
      {
      ot.Set(itk::NumericTraits<PixelType>::NonpositiveMin());
      threadUnderflow++;
      }
    // Case value is equal to inf or superior to the maximum value
    // allowed by the pixelType cast
    else if (value > double(itk::NumericTraits<PixelType>::max()))
      {
      ot.Set(itk::NumericTraits<PixelType>::max());
      threadOverflow++;
      }
    else
      {
      ot.Set(static_cast<PixelType>(value));
      }

    for(j=0; j < nbInputImages; ++j)
      {
      ++Vit[j];
      }

    ++ot;

    progress.CompletedPixel();

    }
}

}// end namespace otb

#endif
