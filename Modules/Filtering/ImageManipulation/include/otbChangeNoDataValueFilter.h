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
    std::vector<double> noDataValues;

    bool ret = ReadNoDataFlags(this->GetInput()->GetMetaDataDictionary(),noDataValueAvailable,noDataValues);

    if(!ret)
      {
      noDataValueAvailable.resize(this->GetInput()->GetNumberOfComponentsPerPixel(),false);
      noDataValues.resize(this->GetInput()->GetNumberOfComponentsPerPixel(),0);
      }

    this->GetFunctor().m_Flags = noDataValueAvailable;
    this->GetFunctor().m_Values = noDataValues;
  }

  virtual void GenerateOutputInformation()
  {
    Superclass::GenerateOutputInformation();

    WriteNoDataFlags(this->GetFunctor().m_Flags,this->GetFunctor().m_NewValues,this->GetOutput()->GetMetaDataDictionary());
  }

private:
  ChangeNoDataValueFilter(const Self&); // purposely not implemented
  void operator =(const Self&); // purposely not implemented

}; 

} // End namespace otb



#endif
