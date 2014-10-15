/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbBandMathImageFilterX_txx
#define __otbBandMathImageFilterX_txx
#include "otbBandMathImageFilterX.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "otbMacro.h"


#include <iostream>
#include <string>

namespace otb
{

/** Constructor */
template <class TImage>
BandMathImageFilterX<TImage>
::BandMathImageFilterX()
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
BandMathImageFilterX<TImage>
::~BandMathImageFilterX()
{
}

template <class TImage>
void BandMathImageFilterX<TImage>
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
void BandMathImageFilterX<TImage>
::SetNthInput(unsigned int idx, const ImageType * image)
{
  this->SetInput(idx, const_cast<TImage *>( image ));
  unsigned int nbInput = this->GetNumberOfInputs();
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
void BandMathImageFilterX<TImage>
::SetNthInput(unsigned int idx, const ImageType * image, const std::string& varName)
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
void BandMathImageFilterX<TImage>
::SetNthInputName(unsigned int idx, const std::string& varName)
{
  m_VVarName[idx] = varName;
}

template <typename TImage>
TImage * BandMathImageFilterX<TImage>
::GetNthInput(unsigned int idx)
{
  return const_cast<TImage *>(this->GetInput(idx));
}

template< typename TImage >
void BandMathImageFilterX<TImage>
::SetExpression(const std::string& expression)
{
  if (m_Expression != expression)
    m_Expression = expression;
  this->Modified();
}

template< typename TImage >
std::string BandMathImageFilterX<TImage>
::GetExpression() const
{
  return m_Expression;
}

template< typename TImage >
std::string BandMathImageFilterX<TImage>
::GetNthInputName(unsigned int idx) const
{
  return m_VVarName.at(idx);
}

template< typename TImage >
void BandMathImageFilterX<TImage>
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
  tmpIdxVarNames.at(0) = "idxX";
  tmpIdxVarNames.at(1) = "idxY";
  tmpIdxVarNames.at(2) = "idxPhyX";
  tmpIdxVarNames.at(3) = "idxPhyY";

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
  m_Spacing = this->GetNthInput(0)->GetSpacing();
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
    m_AImage.at(i).resize(m_NbVar);
    m_VParser.at(i)->SetExpr(m_Expression);

    for(j=0; j < nbInputImages; ++j)
      {
      m_VParser.at(i)->DefineVar(m_VVarName.at(j), &m_AImage.at(i).at(j)); 
      }

    for(j=nbInputImages; j < nbInputImages+nbAccessIndex; ++j)
      {
      m_VVarName.at(j) = tmpIdxVarNames.at(j-nbInputImages);
      m_VParser.at(i)->DefineVar(m_VVarName.at(j), &m_AImage.at(i).at(j));
      }

    }

 
}

template< typename TImage >
void BandMathImageFilterX<TImage>
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
void BandMathImageFilterX<TImage>
::ThreadedGenerateData(const ImageRegionType& outputRegionForThread,
           itk::ThreadIdType threadId)
{
  double value;
  unsigned int j;
  unsigned int nbInputImages = this->GetNumberOfInputs();

  typedef itk::ImageRegionConstIterator<TImage> ImageRegionConstIteratorType;

  std::vector< ImageRegionConstIteratorType > Vit;
  Vit.resize(nbInputImages);
  for(j=0; j < nbInputImages; ++j)
    {
    Vit[j] = ImageRegionConstIteratorType (this->GetNthInput(j), outputRegionForThread);
    }

  itk::ImageRegionIterator<TImage> ot (this->GetOutput(), outputRegionForThread);

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  while(!Vit.at(0).IsAtEnd())
    {
    for(j=0; j < nbInputImages; ++j)
      {
      m_AImage.at(threadId).at(j) = static_cast<double>(Vit.at(j).Get());
      }

    // Image Indexes
    for(j=0; j < 2; ++j)
      {
      m_AImage.at(threadId).at(nbInputImages+j)   = static_cast<double>(Vit.at(0).GetIndex()[j]);
      }
    for(j=0; j < 2; ++j)
      {
      m_AImage.at(threadId).at(nbInputImages+2+j) = static_cast<double>(m_Origin[j])
        +static_cast<double>(Vit.at(0).GetIndex()[j]) * static_cast<double>(m_Spacing[j]);
      }

    try
      {
      value = m_VParser.at(threadId)->Eval().GetFloat();
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
      m_ThreadUnderflow[threadId]++;
      }
    // Case value is equal to inf or superior to the maximum value
    // allowed by the pixelType cast
    else if (value > double(itk::NumericTraits<PixelType>::max()))
      {
      ot.Set(itk::NumericTraits<PixelType>::max());
      m_ThreadOverflow[threadId]++;
      }
    else
      {
      ot.Set(static_cast<PixelType>(value));
      }

    for(j=0; j < nbInputImages; ++j)
      {
      ++(Vit.at(j));
      }

    ++ot;

    progress.CompletedPixel();
    }
}

}// end namespace otb

#endif
