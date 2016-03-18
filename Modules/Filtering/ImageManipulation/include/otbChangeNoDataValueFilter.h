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
/** \class ChangeNoDataFunctor
 *  \brief Functor used by ChangeNoDataValueFilter
 *
 *  See ChangeNoDataValueFilter for complete documentation. 
 *  \ingroup OTBImageManipulation
 */
template <typename TInputPixel, typename TOutputPixel> 
class ChangeNoDataFunctor
{
public:
  ChangeNoDataFunctor():
    m_Flags(),
    m_Values(),
    m_NewValues(),
    m_NaNIsNoData(false)
  {}
  virtual ~ChangeNoDataFunctor(){}

  inline TOutputPixel operator()(const TInputPixel& in) const
  {
    return otb::ChangeNoData(in,m_Flags,m_Values,m_NewValues,m_NaNIsNoData);
  }

  std::vector<bool>   m_Flags;
  std::vector<double> m_Values;
  std::vector<double> m_NewValues;
  bool                m_NaNIsNoData;
};

} // End namespace Functor

/** \class ChangeNoDataValueFilter
 *  \brief Change no-data flags and values and replace them in image
 *  
 *  This filter reads the no-data flags (a boolean vector indicating
 *  for each band if a no-data value exists) and values (the actual
 *  value to be used as no-data for each band) from
 *  MetaDataDictionary, and allows changing this value.
 * 
 *  The algorithm is the following: for each pixel, for each channel
 *  in the pixel, if there is a no-data value for this channel
 *  (according to no-data flags) and the current channel value equals
 *  to the no-data value set for this channel, then the value is
 *  changed for the new no-data value specified by the
 *  users. Otherwise, value remains untouched.
 * 
 *  If NaNIsNoData is true:
 *  - NaN values will be considered as no data and replaced as well
 *  - Output image will have no-data flags and values for all bands
 *
 *  If NaNIsNoData is false:
 *  - Band for which input no-data flags is false will remain
 *    untouched
 *  - Output image will have no-data flags and values only for bands
 *    for which input no-data flag is true.
 * 
 * \ingroup Streamed
 * \ingroup MultiThreaded
 * \ingroup OTBImageManipulation
 */
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

  /**
   * Set the new no-data values
   * \param newValues The vector of new no-data values (size should be
   * >= to number of bands)
   */
  void SetNewNoDataValues(std::vector<double> & newValues)
  {
    this->GetFunctor().m_NewValues = newValues;
  }

   /**
   * Set the NaN is no data flags
   * \param nanIsNoData If true, NaN values will be considered as
   * no-data as well (default is false)
   */
  void SetNaNIsNoData(bool nanIsNoData)
  {
    this->GetFunctor().m_NaNIsNoData = nanIsNoData;
  }

protected:
  ChangeNoDataValueFilter()
  {}

  virtual ~ChangeNoDataValueFilter()
  {}

  virtual void GenerateOutputInformation()
  {
    Superclass::GenerateOutputInformation();

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
    
    std::vector<bool> flags = noDataValueAvailable;
    
    if(this->GetFunctor().m_NaNIsNoData)
      {
      flags = std::vector<bool>(flags.size(),true);
      }
    
    WriteNoDataFlags(flags,this->GetFunctor().m_NewValues,this->GetOutput()->GetMetaDataDictionary());
  }

private:
  ChangeNoDataValueFilter(const Self&); // purposely not implemented
  void operator =(const Self&); // purposely not implemented

}; 

} // End namespace otb



#endif
