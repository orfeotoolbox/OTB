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
#ifndef __otbMultiDisparityMapTo3DFilter_txx
#define __otbMultiDisparityMapTo3DFilter_txx

#include "otbMultiDisparityMapTo3DFilter.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionIterator.h"

namespace otb
{

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::MultiDisparityMapTo3DFilter()
{
  // Set the number of inputs (1 moving image by default -> 3 inputs)
  this->SetNumberOfInputs(3);
  this->SetNumberOfRequiredInputs(1);
  this->m_MovingKeywordLists.resize(1);

  // Set the outputs
  this->SetNumberOfOutputs(2);
  this->SetNthOutput(0,TOutputImage::New());
  this->SetNthOutput(1,TResidueImage::New());
}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::~MultiDisparityMapTo3DFilter()
{}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
void
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::SetNumberOfMovingImages(unsigned int nb)
{
  if (nb > 0)
    {
    this->SetNumberOfInputs(3 * nb);
    this->m_MovingKeywordLists.resize(nb);
    }
}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
unsigned int
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::GetNumberOfMovingImages()
{
  return this->m_MovingKeywordLists.size();
}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
void
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::SetHorizontalDisparityMapInput(unsigned int index, const TDisparityImage * hmap)
{
  if ((3 * (index + 1)) > this->GetNumberOfInputs())
    {
    itkExceptionMacro(<<"Index is greater than the number of moving images");
    } 
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(3 * index, const_cast<TDisparityImage *>( hmap ));
}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
void
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::SetVerticalDisparityMapInput(unsigned int index, const TDisparityImage * vmap)
{
  if ((3 * (index + 1)) > this->GetNumberOfInputs())
    {
    itkExceptionMacro(<<"Index is greater than the number of moving images");
    }
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(3 * index + 1, const_cast<TDisparityImage *>( vmap ));
}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
void
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::SetDisparityMaskInput(unsigned int index, const TMaskImage * mask)
{
  if ((3 * (index + 1)) > this->GetNumberOfInputs())
    {
    itkExceptionMacro(<<"Index is greater than the number of moving images");
    }
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(3 * index + 2, const_cast<TMaskImage *>( mask ));
}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
const TDisparityImage *
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::GetHorizontalDisparityMapInput(unsigned int index) const
{
  if ((3 * (index + 1)) > this->GetNumberOfInputs())
    {
    return 0;
    }
  return static_cast<const TDisparityImage *>(this->itk::ProcessObject::GetInput(3 * index));
}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
const TDisparityImage *
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::GetVerticalDisparityMapInput(unsigned int index) const
{
  if ((3 * (index + 1)) > this->GetNumberOfInputs())
    {
    return 0;
    }
  return static_cast<const TDisparityImage *>(this->itk::ProcessObject::GetInput(3 * index + 1));
}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
const TMaskImage *
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::GetDisparityMaskInput(unsigned int index) const
{
  if ((3 * (index + 1)) > this->GetNumberOfInputs())
    {
    return 0;
    }
  return static_cast<const TMaskImage *>(this->itk::ProcessObject::GetInput(3 * index + 2));
}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
const TResidueImage *
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::GetResidueOutput() const
{
  return static_cast<const TResidueImage *>(this->itk::ProcessObject::GetOutput(1));
}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
TResidueImage *
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::GetResidueOutput()
{
  return static_cast<TResidueImage *>(this->itk::ProcessObject::GetOutput(1));
}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
void
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::GenerateOutputInformation()
{
  const TDisparityImage * horizDisp = this->GetHorizontalDisparityMapInput(0);
  TOutputImage * outputPtr = this->GetOutput();
  TResidueImage * residuePtr = this->GetResidueOutput();
  
  if (horizDisp)
    {
    outputPtr->SetLargestPossibleRegion(horizDisp->GetLargestPossibleRegion());
    outputPtr->SetNumberOfComponentsPerPixel(3);

    residuePtr->SetLargestPossibleRegion(horizDisp->GetLargestPossibleRegion());
    residuePtr->SetNumberOfComponentsPerPixel(1);
    
    // copy also origin and spacing
    outputPtr->SetOrigin(horizDisp->GetOrigin());
    outputPtr->SetSpacing(horizDisp->GetSpacing());
  
    residuePtr->SetOrigin(horizDisp->GetOrigin());
    residuePtr->SetSpacing(horizDisp->GetSpacing());
    
    if (this->m_ReferenceKeywordList.GetSize() > 0)
      {
      itk::EncapsulateMetaData<ImageKeywordListType>
        (outputPtr->GetMetaDataDictionary(),
        MetaDataKey::OSSIMKeywordlistKey,
        this->m_ReferenceKeywordList);
      itk::EncapsulateMetaData<ImageKeywordListType>
        (residuePtr->GetMetaDataDictionary(),
        MetaDataKey::OSSIMKeywordlistKey,
        this->m_ReferenceKeywordList);
      }
    else
      {
      itkExceptionMacro(<<"Reference keywordlist is missing");
      }
    }
  else
    {
    itkExceptionMacro(<< "First horizontal disparity map is missing");
    }
}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
void
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::GenerateInputRequestedRegion()
{
  RegionType requested = this->GetOutput()->GetRequestedRegion();
  RegionType largest = this->GetHorizontalDisparityMapInput(0)->GetLargestPossibleRegion();
  
  for (unsigned int i=0 ; i<this->GetNumberOfInputs() ; ++i)
    {
    unsigned int index  = i % 3;
    unsigned int pos    = i / 3;
    switch (index)
      {
      case 0:
        {
        // check horizontal disparity maps
        TDisparityImage * horizDisp = const_cast<TDisparityImage*>(this->GetHorizontalDisparityMapInput(pos));
        if (horizDisp)
          {
          if (horizDisp->GetLargestPossibleRegion() != largest)
            {
            itkExceptionMacro(<<"Horizontal disparity map at position "<<pos<<" has a different largest region");
            }
          horizDisp->SetRequestedRegion(requested);
          }
        else
          {
          itkExceptionMacro(<<"Horizontal disparity map at position "<<pos<<" is missing");
          }
        }
      break;
      case 1:
        {
        // check vertical disparity maps
        TDisparityImage * vertiDisp = const_cast<TDisparityImage*>(this->GetVerticalDisparityMapInput(pos));
        if (vertiDisp)
          {
          if (vertiDisp->GetLargestPossibleRegion() != largest)
            {
            itkExceptionMacro(<<"Vertical disparity map at position "<<pos<<" has a different largest region");
            }
          vertiDisp->SetRequestedRegion(requested);
          }
        }
      break;
      case 2:
        {
        // check disparity masks
        TMaskImage * maskDisp = const_cast<TMaskImage*>(this->GetDisparityMaskInput(pos));
        if (maskDisp)
          {
          if (maskDisp->GetLargestPossibleRegion() != largest)
            {
            itkExceptionMacro(<<"Disparity mask at position "<<pos<<" has a different largest region");
            }
          maskDisp->SetRequestedRegion(requested);
          }
        }
      break;
      default:
        itkExceptionMacro(<< "Unexpected value for (i%3) ");
      } 
    }
  
  // Check moving keywordlist
  for (unsigned int k=0 ; k<this->m_MovingKeywordLists.size() ; ++k)
  {
    if (this->m_MovingKeywordLists[k].GetSize() == 0)
      {
      itkExceptionMacro(<< "Keywordlist of moving image at position "<<k<<" is empty");
      }
  }

}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
void
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::BeforeThreadedGenerateData()
{
  // Instanciate all transforms
  this->m_ReferenceToGroundTransform = RSTransformType::New();
  this->m_ReferenceToGroundTransform->SetInputKeywordList(this->m_ReferenceKeywordList);
  this->m_ReferenceToGroundTransform->InstanciateTransform();
  
  this->m_MovingToGroundTransform.clear();
  for (unsigned int k=0 ; k<this->m_MovingKeywordLists.size() ; ++k)
    {
    RSTransformType::Pointer transfo = RSTransformType::New();
    transfo->SetInputKeywordList(this->m_MovingKeywordLists[k]);
    transfo->InstanciateTransform();
    this->m_MovingToGroundTransform.push_back(transfo);
    }
}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
void
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::ThreadedGenerateData(const RegionType & outputRegionForThread, int threadId)
{
  // TODO
   
}

}

#endif
