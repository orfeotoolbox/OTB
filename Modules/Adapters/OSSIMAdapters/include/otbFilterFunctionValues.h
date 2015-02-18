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
#ifndef __otbFilterFunctionValues_h
#define __otbFilterFunctionValues_h

#include <vector>
#include <iostream>

#include "itkDataObject.h"
#include "itkObjectFactory.h"
#include "itkMacro.h"

namespace otb
{
/** \class FilterFunctionValues
 *  \brief This class contains the values of the filter function for the processed spectral band.
 *
 * The step between 2 values is 0.0025 micro meter. The class contains the min and the max value.
 * Those value can be directly read from the image metadatas.
 *
 * \ingroup IO
 */
class ITK_EXPORT FilterFunctionValues : public itk::DataObject
{
public:
  /** Standard typedefs */
  typedef FilterFunctionValues          Self;
  typedef itk::DataObject               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkTypeMacro(FilterFunctionValues, DataObject);

  /** Creation through object factory macro */
  itkNewMacro(Self);

  typedef float                                   WavelengthSpectralBandType;
  typedef std::vector<WavelengthSpectralBandType> ValuesVectorType;

  /** Set vector that contains the filter function value. */
  void SetFilterFunctionValues(const ValuesVectorType& vect)
  {
    m_FilterFunctionValues = vect;
    this->Modified();
  }
  /** Get vector that contains the filter function value. */
  const ValuesVectorType& GetFilterFunctionValues() const
  {
    return m_FilterFunctionValues;
  }
  /** Get vector that contains the filter function value 6S. */
  void SetFilterFunctionValues6S(const ValuesVectorType& vect)
  {
    m_FilterFunctionValues6S = vect;
    this->Modified();
  }
  /** Get vector that contains the filter function value 6S. */
  const ValuesVectorType& GetFilterFunctionValues6S() const
  {
    return m_FilterFunctionValues6S;
  }

  /** Set minimum spectral value. */
  itkSetMacro(MinSpectralValue, WavelengthSpectralBandType);
  /** Get minimum spectral value. */
  itkGetMacro(MinSpectralValue, WavelengthSpectralBandType);
  /** Set maximum spectral value. This value is automatically computed.*/
  itkSetMacro(MaxSpectralValue, WavelengthSpectralBandType);
  /** Get maximum spectral value. This value is automatically computed.*/
  itkGetMacro(MaxSpectralValue, WavelengthSpectralBandType);
  /** Set user step between each wavelength spectral band values. */
  itkSetMacro(UserStep, WavelengthSpectralBandType);
  /** Get user step between each wavelength spectral band values. */
  itkGetMacro(UserStep, WavelengthSpectralBandType);

  /** Compute the center wavelength value for the filter */
  WavelengthSpectralBandType GetCenterSpectralValue() const;

protected:
  /** Constructor */
  FilterFunctionValues();
  /** Destructor */
  ~FilterFunctionValues() {}

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  FilterFunctionValues(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Vector that contains the filter function value. */
  ValuesVectorType m_FilterFunctionValues;
  /** Vector that contains the filter function value in 6S format (step of 0.0025 micro m).
    * There values a computed by 6S. If the UserStep is 0.0025 micro m, then m_FilterFunctionValues is identical as m_FilterFunctionValues6S
    */
  ValuesVectorType m_FilterFunctionValues6S;
  /** Minimum spectral value (in  micro m). */
  WavelengthSpectralBandType m_MinSpectralValue;
  /** Maximum spectral value (in  micro m). */
  WavelengthSpectralBandType m_MaxSpectralValue;
  /** User step between each wavelength spectral band values. (in  micro micro m) */
  WavelengthSpectralBandType m_UserStep;
};

} //end namespace

#endif
