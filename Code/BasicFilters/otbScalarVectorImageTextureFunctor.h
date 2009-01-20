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
#ifndef __otbScalarVectorImageTextureFunctor_h
#define __otbScalarVectorImageTextureFunctor_h

#include <vector>

#include "itkScalarImageTextureCalculator.h"
#include "itkImageRegionIterator.h"

namespace otb
{
namespace Functor
{
/** \class ScalarVectorImageTextureFunctor
 *  \brief Unary neighborhood functor to compute the value of a pixel which is a sum
 *   of the surrounding pixels value ponderated by a coefficient.
 *
 *  \ingroup Functor
*/
template <class TNeighIter, class TInputImage, class TOutput>
class ScalarVectorImageTextureFunctor
{
 public:
  ScalarVectorImageTextureFunctor()
    {
      m_FeatureIndex.clear();
    };
  ~ScalarVectorImageTextureFunctor() { };

  typedef std::vector<int> IndexSelectFeaturesType;
  typedef TInputImage InputImageType;
  typedef typename itk::Statistics::ScalarImageTextureCalculator< InputImageType> TextureCalcType;

  void SetFeatureIndex(const IndexSelectFeaturesType & listIndex){ m_FeatureIndex = listIndex;};
  const IndexSelectFeaturesType & GetFeatureIndex(){ return m_FeatureIndex; };

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

      typedef itk::ImageRegionIterator< InputImageType > IteratorrrType;
      IteratorrrType inputIt( image, image->GetLargestPossibleRegion() );
      inputIt.GoToBegin();
      for (unsigned int i = 0; i < neighborhoodSize; ++i)
  {
    inputIt.Set(it.GetPixel(i));
    ++inputIt;
  }

      textureFilter->FastCalculationsOn();
      textureFilter->SetInput(image);
      textureFilter->Compute();

      TOutput resultvalue;
      resultvalue.SetSize(m_FeatureIndex.size());
      resultvalue.AllocateElements(m_FeatureIndex.size());

      for(unsigned int cpt=0; cpt < m_FeatureIndex.size(); cpt++)
      {
        resultvalue[cpt] = textureFilter->GetFeatureMeans()->at(m_FeatureIndex[cpt]);
      }

      return (static_cast<TOutput>(resultvalue));

    }
 private:
  IndexSelectFeaturesType m_FeatureIndex;

};

}
}

#endif
