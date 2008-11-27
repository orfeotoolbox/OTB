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
#ifndef __otbScalarImageTextureFunctor_h
#define __otbScalarImageTextureFunctor_h


#include "itkScalarImageTextureCalculator.h"
#include "itkImageRegionIterator.h"
namespace otb
{
namespace Functor
{
/** \class ScalarImageTextureFunctor
 *  \brief Unary neighborhood functor to compute the value of a pixel which is a sum 
 *   of the surrounding pixels value ponderated by a coefficient.
 *  
 *  \ingroup Functor
*/
template <class TNeighIter, class TInputImage, class TOutput>
class ScalarImageTextureFunctor
{
 public:
  ScalarImageTextureFunctor() 
    {
      m_FeatureIndex=0;
    };
  ~ScalarImageTextureFunctor() { };
  
  void SetFeatureIndex(int i){ m_FeatureIndex = i;};
  int GetFeatureIndex(){ return m_FeatureIndex; };
  
  typedef TInputImage InputImageType;
  typedef typename itk::Statistics::ScalarImageTextureCalculator< InputImageType> TextureCalcType;
  
  inline TOutput operator()(const TNeighIter & it)
    {	 
      unsigned int neighborhoodSize = it.Size();
   
      typename TextureCalcType::Pointer textureFilter = TextureCalcType::New();
      typename InputImageType::Pointer image = InputImageType::New();
      
      typename InputImageType::IndexType index;
      index.Fill(0);
      typename InputImageType::RegionType region;
      
      region = it.GetBoundingBoxAsImageRegion();
      region.SetIndex(index);
      
      image->SetRegions( region );
      image->Allocate();
   
      typedef itk::ImageRegionIterator< InputImageType > IteratorType;
      IteratorType inputIt( image, image->GetLargestPossibleRegion() );
      inputIt.GoToBegin();
      for (unsigned int i = 0; i < neighborhoodSize; ++i)
	{
	  inputIt.Set(it.GetPixel(i));
	  ++inputIt;
	}

      textureFilter->FastCalculationsOn();
      textureFilter->SetInput(image);
      textureFilter->Compute();
    
      return (static_cast<TOutput>(textureFilter->GetFeatureMeans()->at(m_FeatureIndex)));
      
    }
 private:
  int m_FeatureIndex;
  
};
 
}
}

#endif
