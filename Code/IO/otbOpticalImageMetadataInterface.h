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
#ifndef __otbOpticalImageMetadataInterface_h
#define __otbOpticalImageMetadataInterface_h

#include <string>

#include "otbImageMetadataInterfaceBase.h"
#include "otbFilterFunctionValues.h"
#include "otbObjectList.h"

namespace otb
{
/** \class OpticalImageMetadataInterface
 *
 * \brief Class for Optical captor metadata reading.
 *
 */
class ITK_EXPORT OpticalImageMetadataInterface : public ImageMetadataInterfaceBase
{
public:

  typedef OpticalImageMetadataInterface Self;
  typedef ImageMetadataInterfaceBase    Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(OpticalImageMetadataInterface, ImageMetadataInterfaceBase);

  typedef Superclass::ImageType                      ImageType;
  typedef Superclass::MetaDataDictionaryType         MetaDataDictionaryType;
  typedef Superclass::VectorType                     VectorType;
  typedef Superclass::VariableLengthVectorType       VariableLengthVectorType;
  typedef Superclass::ImageKeywordlistType           ImageKeywordlistType;

  typedef FilterFunctionValues                              FilterFunctionValuesType;
  typedef otb::ObjectList <FilterFunctionValuesType>        InternalWavelengthSpectralBandVectorType;
  typedef InternalWavelengthSpectralBandVectorType::Pointer WavelengthSpectralBandVectorType;

  /** Get the sun elevation from the ossim metadata */
  double GetSunElevation() const;

  /** Get the sun azimuth from the ossim metadata */
  double GetSunAzimuth() const;

  /** Get the sat elevation from the ossim metadata */
  virtual double GetSatElevation() const = 0;

  /** Get the sat azimuth from the ossim metadata */
  virtual double GetSatAzimuth() const = 0;

  /** Get the radiometric bias from the ossim metadata */
  virtual VariableLengthVectorType GetPhysicalBias() const = 0;

  /** Get the radiometric gain from the ossim metadata */
  virtual VariableLengthVectorType GetPhysicalGain() const = 0;

  /** Get the solar irradiance from the ossim metadata */
  virtual VariableLengthVectorType GetSolarIrradiance() const = 0;

  /** Get the first wavelength for the spectral band definition */
  virtual VariableLengthVectorType GetFirstWavelengths() const = 0;

  /** Get the last wavelength for the spectral band definition */
  virtual VariableLengthVectorType GetLastWavelengths() const = 0;

  /** Get the enhanced band names */
  std::vector<std::string> GetEnhancedBandNames() const = 0;

  /** This method is to handle the permutation of the spectral band by some image provider
   * in most cases, this method won't change the value, but for SPOT data, the bands are set up as
   *  2 1 0 3 in the tiff file, this method which is overloaded for SPOT enables to retrieve the
   *  proper band. */
  virtual unsigned int BandIndexToWavelengthPosition(unsigned int i) const;

  /** Vector that contains the filter function value in 6S format (step of 0.0025 micro m).
   * There values a computed by 6S. */
  virtual WavelengthSpectralBandVectorType GetSpectralSensitivity ()  const = 0;
protected:
  OpticalImageMetadataInterface();
  virtual ~OpticalImageMetadataInterface() {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const;


private:
  OpticalImageMetadataInterface(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
