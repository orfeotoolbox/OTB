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
#ifndef __otbChangeNoDataValueFilter_h
#define __otbChangeNoDataValueFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "itkMetaDataObject.h"
#include "otbMetaDataKey.h"
#include "otbNoDataHelper.h"

namespace otb
{
namespace Functor
{
template <typename TInputPixel, typename TOutputPixel> 
class ChangeNoDataFunctor
{
public:
  ChangeNoDataFunctor():
    m_Flags(),
    m_Values(),
    m_NewValues()
  {}
  virtual ~ChangeNoDataFunctor(){}

  inline TOutputPixel operator()(const TInputPixel& in) const
  {
    return otb::ChangeNoData(in,m_Flags,m_Values,m_NewValues);
  }

  std::vector<bool>   m_Flags;
  std::vector<double> m_Values;
  std::vector<double> m_NewValues;
};

} // End namespace Functor



template <typename TInputImage, typename TOutputImage>
class ChangeNoDataValueFilter
  : public itk::UnaryFunctorImageFilter<TInputImage,
                                        TOutputImage,
                                        Functor::ChangeNoDataFunctor<typename TInputImage::PixelType,
                                                                     typename TOutputImage::PixelType> >
{
public:
  typedef Functor::ChangeNoDataFunctor<typename TInputImage::PixelType,typename TOutputImage::PixelType> FunctorType;

  typedef ChangeNoDataValueFilter                           Self;
  typedef itk::UnaryFunctorImageFilter<TInputImage,
                                        TOutputImage,
                                       FunctorType>         Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ChangeNoDataValueFilter, itk::ImageToImageFilter);

  void SetNewNoDataValues(std::vector<double> & newValues)
  {
    this->GetFunctor().m_NewValues = newValues;
  }

protected:
  ChangeNoDataValueFilter()
  {}

  virtual ~ChangeNoDataValueFilter()
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

  virtual void GenerateOutputInformation()
  {
    Superclass::GenerateOutputInformation();

    std::vector<bool> newAvail(this->GetFunctor().m_NewValues.size(),true);
    
    itk::EncapsulateMetaData<std::vector<bool> >(this->GetOutput()->GetMetaDataDictionary(),MetaDataKey::NoDataValueAvailable,newAvail);
    
    itk::EncapsulateMetaData<std::vector<double> >(this->GetOutput()->GetMetaDataDictionary(),MetaDataKey::NoDataValue,this->GetFunctor().m_NewValues);
    
  }

private:
  ChangeNoDataValueFilter(const Self&); // purposely not implemented
  void operator =(const Self&); // purposely not implemented

}; 

} // End namespace otb



#endif
