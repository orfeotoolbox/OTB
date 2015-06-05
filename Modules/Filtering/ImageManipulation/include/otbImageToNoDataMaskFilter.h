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
#ifndef __otbImageToNoDataMaskFilter_h
#define __otbImageToNoDataMaskFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "otbNoDataHelper.h"

namespace otb
{
namespace Functor
{
template <typename TInputPixel, typename TOutputPixel> 
class NoDataFunctor
{
public:
  NoDataFunctor():
    m_Flags(),
    m_Values(),
    m_OutsideValue(0),
    m_InsideValue(1)
  {}
  virtual ~NoDataFunctor(){}

  inline TOutputPixel operator()(const TInputPixel& in) const
  {
    return otb::IsNoData(in,m_Flags,m_Values)?m_OutsideValue:m_InsideValue;
  }

  std::vector<bool>   m_Flags;
  std::vector<double> m_Values;
  TOutputPixel        m_OutsideValue;
  TOutputPixel        m_InsideValue;
};

} // End namespace Functor



template <typename TInputImage, typename TOutputImage>
class ImageToNoDataMaskFilter
  : public itk::UnaryFunctorImageFilter<TInputImage,
                                        TOutputImage,
                                        Functor::NoDataFunctor<typename TInputImage::PixelType,
                                                               typename TOutputImage::PixelType> >
{
public:
  typedef Functor::NoDataFunctor<typename TInputImage::PixelType,typename TOutputImage::PixelType> FunctorType;

  typedef ImageToNoDataMaskFilter                           Self;
  typedef itk::UnaryFunctorImageFilter<TInputImage,
                                        TOutputImage,
                                       FunctorType>         Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(SpectralAngleDistanceImageFilter, itk::ImageToImageFilter);

  void SetInsideValue(const typename TOutputImage::PixelType & value)
  {
    this->GetFunctor().m_InsideValue = value;
  }

  void SetOutsideValue(const typename TOutputImage::PixelType & value)
  {
    this->GetFunctor().m_OutsideValue = value;
  }

protected:
  ImageToNoDataMaskFilter()
  {}

  virtual ~ImageToNoDataMaskFilter()
  {}

  virtual void BeforeThreadedGenerateData()
  {
    std::vector<bool> noDataValueAvailable;
    bool ret = itk::ExposeMetaData<std::vector<bool> >(this->GetInput()->GetMetaDataDictionary(),MetaDataKey::NoDataValueAvailable,noDataValueAvailable);

    if(!ret)
      {
      noDataValueAvailable.resize(this->GetInput()->GetNumberOfComponentsPerPixel(),false);
      }

    this->GetFunctor().m_Flags = noDataValueAvailable;

    std::vector<double> noDataValues;
    ret = itk::ExposeMetaData<std::vector<double> >(this->GetInput()->GetMetaDataDictionary(),MetaDataKey::NoDataValue,noDataValues);

    if(!ret)
      {
      noDataValues.resize(this->GetInput()->GetNumberOfComponentsPerPixel(),0);
      }

    this->GetFunctor().m_Values = noDataValues;
  }

private:
  ImageToNoDataMaskFilter(const Self&); // purposely not implemented
  void operator =(const Self&); // purposely not implemented
}; 

} // End namespace otb



#endif
