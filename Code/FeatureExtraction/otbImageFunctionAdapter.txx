/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbImageFunctionAdapter_txx
#define __otbImageFunctionAdapter_txx

#include "otbImageFunctionAdapter.h"

namespace otb
{
/** ----- Template Partial Specialization ----- **/
// Dummy
template <class TInputImage, class TInternalImageFunctionType, class TCoordRep>
typename ImageFunctionAdapter<TInputImage, TInternalImageFunctionType, TCoordRep >::OutputType
ImageFunctionAdapter <TInputImage, TInternalImageFunctionType, TCoordRep >
::EvaluateAtIndex(const IndexType& index) const
{
  OutputType result;
  result.SetSize(1);
  result[0] = 0.0;
  return result;
}
template <class TInputImage, class TInternalImageFunctionType, class TCoordRep >
void
ImageFunctionAdapter <TInputImage, TInternalImageFunctionType, TCoordRep >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


// Fourier-Mellin Descriptors
template <class TInputImage, class TCoordRep>
typename ImageFunctionAdapter<TInputImage,
                              FourierMellinDescriptorsImageFunction< TInputImage, TCoordRep >, 
                              TCoordRep >::OutputType
ImageFunctionAdapter<TInputImage, 
                     FourierMellinDescriptorsImageFunction< TInputImage, TCoordRep >, 
                     TCoordRep >
::EvaluateAtIndex(const IndexType& index) const
{
  unsigned int p, q, rsltIdx = 0;
  OutputType result;

  p = this->GetImageFunction()->GetPmax();
  q = this->GetImageFunction()->GetQmax();

  result.SetSize((p+1)*(q+1));

  typename InternalImageFunctionType::OutputType tmpResult;
  this->GetImageFunction()->SetInputImage(this->GetInputImage());
  tmpResult = this->GetImageFunction()->EvaluateAtIndex(index);

  for (unsigned int i=0; i<=p; i++)
    {
    for (unsigned int j=0; j<=q; j++)
      {
      result[rsltIdx] = tmpResult.at(i).at(j);
      rsltIdx ++;
      }
    }
  
  return result;
}

template <class TInputImage, class TCoordRep>
void
ImageFunctionAdapter<TInputImage, 
                     FourierMellinDescriptorsImageFunction< TInputImage, TCoordRep >, 
                     TCoordRep >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


// Real Moments
template <class TInputImage, class TCoordRep>
typename ImageFunctionAdapter<TInputImage,
                              RealMomentsImageFunction< TInputImage, TCoordRep >, 
                              TCoordRep >::OutputType
ImageFunctionAdapter<TInputImage,
                     RealMomentsImageFunction< TInputImage, TCoordRep >, 
                     TCoordRep >
::EvaluateAtIndex(const IndexType& index) const
{
  unsigned int p, q, rsltIdx = 0;
  OutputType result;
  
  p = this->GetImageFunction()->GetPmax();
  q = this->GetImageFunction()->GetQmax();
  
  result.SetSize((p+1)*(q+1));
  
  typename InternalImageFunctionType::OutputType tmpResult;
  this->GetImageFunction()->SetInputImage(this->GetInputImage());
  tmpResult = this->GetImageFunction()->EvaluateAtIndex(index);
  
  for (unsigned int i=0; i<=p; i++)
    {
    for (unsigned int j=0; j<=q; j++)
      {
      result[rsltIdx] = tmpResult.at(i).at(j);
      rsltIdx ++;
      }
    }
  
  return result;
}

template <class TInputImage, class TCoordRep>
void
ImageFunctionAdapter<TInputImage, 
                     RealMomentsImageFunction< TInputImage, TCoordRep >, 
                     TCoordRep >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


//Complex Moments
template <class TInputImage, class TCoordRep>
typename ImageFunctionAdapter<TInputImage,
                              ComplexMomentsImageFunction< TInputImage, TCoordRep >, 
                              TCoordRep >::OutputType
ImageFunctionAdapter<TInputImage,
                     ComplexMomentsImageFunction< TInputImage, TCoordRep >, 
                     TCoordRep >
::EvaluateAtIndex(const IndexType& index) const
{
  unsigned int p, q, rsltIdx = 0;
  OutputType result;
  
  p = this->GetImageFunction()->GetPmax();
  q = this->GetImageFunction()->GetQmax();
  
  result.SetSize((p+1)*(q+1)*2);
  
  typename InternalImageFunctionType::OutputType tmpResult;
  this->GetImageFunction()->SetInputImage(this->GetInputImage());
  tmpResult = this->GetImageFunction()->EvaluateAtIndex(index);
  
  for (unsigned int i=0; i<=p; i++)
    {
    for (unsigned int j=0; j<=q; j++)
      {
      result[rsltIdx] = tmpResult.at(i).at(j).real();
      rsltIdx ++;
      result[rsltIdx] = tmpResult.at(i).at(j).imag();
      rsltIdx ++;
      }
    }
  
  return result;
}

template <class TInputImage, class TCoordRep>
void
ImageFunctionAdapter<TInputImage, 
                     ComplexMomentsImageFunction< TInputImage, TCoordRep >, 
                     TCoordRep >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


//Flusser Moments
template <class TInputImage, class TCoordRep>
typename ImageFunctionAdapter<TInputImage,
                              FlusserMomentsImageFunction< TInputImage, TCoordRep >, 
                              TCoordRep >::OutputType
ImageFunctionAdapter<TInputImage,
                     FlusserMomentsImageFunction< TInputImage, TCoordRep >,
                     TCoordRep >
::EvaluateAtIndex(const IndexType& index) const
{
  unsigned int rsltIdx = 0;
  OutputType result;
  
  result.SetSize(11);
  
  typename InternalImageFunctionType::OutputType tmpResult;
  this->GetImageFunction()->SetInputImage(this->GetInputImage());
  tmpResult = this->GetImageFunction()->EvaluateAtIndex(index);
  
  for (unsigned int i=0; i<11; i++)
    {
    result[rsltIdx] = tmpResult[i];
    rsltIdx ++;
    }
  
  return result;
}

template <class TInputImage, class TCoordRep>
void
ImageFunctionAdapter<TInputImage, 
                     FlusserMomentsImageFunction<TInputImage, TCoordRep >, 
                     TCoordRep >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


//Hu Moments
template <class TInputImage, class TCoordRep>
typename ImageFunctionAdapter<TInputImage,
                              HuMomentsImageFunction< TInputImage, TCoordRep >, 
                              TCoordRep >::OutputType
ImageFunctionAdapter<TInputImage,
                     HuMomentsImageFunction< TInputImage, TCoordRep >, 
                     TCoordRep >
::EvaluateAtIndex(const IndexType& index) const
{
  unsigned int rsltIdx = 0;
  OutputType result;
  
  result.SetSize(7);
  
  typename InternalImageFunctionType::OutputType tmpResult;
  this->GetImageFunction()->SetInputImage(this->GetInputImage());
  tmpResult = this->GetImageFunction()->EvaluateAtIndex(index);
  
  for (unsigned int i=0; i<7; i++)
    {
    result[rsltIdx] = tmpResult[i];
    rsltIdx ++;
    }
  
  return result;
}

template <class TInputImage, class TCoordRep>
void
ImageFunctionAdapter<TInputImage, 
                     HuMomentsImageFunction< TInputImage, TCoordRep >, 
                     TCoordRep >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


//Radiometric Moments
template <class TInputImage, class TCoordRep>
typename ImageFunctionAdapter<TInputImage,
                              RadiometricMomentsImageFunction< TInputImage, TCoordRep >, 
                              TCoordRep >::OutputType
ImageFunctionAdapter<TInputImage,
                     RadiometricMomentsImageFunction< TInputImage, TCoordRep >, 
                     TCoordRep >
::EvaluateAtIndex(const IndexType& index) const
{
  unsigned int rsltIdx = 0;
  OutputType result;
  
  result.SetSize(4);
  
  typename InternalImageFunctionType::OutputType tmpResult;
  this->GetImageFunction()->SetInputImage(this->GetInputImage());
  tmpResult = this->GetImageFunction()->EvaluateAtIndex(index);
  
  for (unsigned int i=0; i<4; i++)
    {
    result[rsltIdx] = tmpResult[i];
    rsltIdx ++;
    }
  
  return result;
}

template <class TInputImage, class TCoordRep>
void
ImageFunctionAdapter<TInputImage,
                     RadiometricMomentsImageFunction< TInputImage, TCoordRep >, 
                     TCoordRep >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


//Local Histogramm
template <class TInputImage, class TCoordRep>
typename ImageFunctionAdapter<TInputImage,
                              LocalHistogramImageFunction< TInputImage, TCoordRep >, 
                              TCoordRep >::OutputType
ImageFunctionAdapter<TInputImage,
                     LocalHistogramImageFunction< TInputImage, TCoordRep >, 
                     TCoordRep >
::EvaluateAtIndex(const IndexType& index) const
{
  unsigned int nbBins, rsltIdx = 0;
  OutputType result;
  
  nbBins = this->GetImageFunction()->GetNumberOfHistogramBins();

  result.SetSize(nbBins);
  
  typename InternalImageFunctionType::OutputType tmpResult;
  this->GetImageFunction()->SetInputImage(this->GetInputImage());
  tmpResult = this->GetImageFunction()->EvaluateAtIndex(index);
  
  for (unsigned int i=0; i<nbBins; i++)
    {
    result[rsltIdx] = tmpResult->GetFrequency(i);
    rsltIdx ++;
    }
  
  return result;
}

template <class TInputImage, class TCoordRep>
void
ImageFunctionAdapter<TInputImage, 
                     LocalHistogramImageFunction< TInputImage, TCoordRep >, 
                     TCoordRep >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
