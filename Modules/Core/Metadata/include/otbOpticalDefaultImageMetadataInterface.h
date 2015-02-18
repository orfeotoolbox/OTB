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
#ifndef __otbOpticalDefaultImageMetadataInterface_h
#define __otbOpticalDefaultImageMetadataInterface_h


#include "otbMacro.h"

#include "otbOpticalImageMetadataInterface.h"

namespace otb
{
/** \class OpticalDefaultImageMetadataInterface
 *
 * \brief Creation of an "otb" DefaultImageMetadataInterface that gets metadata.
 *
 */
class ITK_EXPORT OpticalDefaultImageMetadataInterface : public OpticalImageMetadataInterface
{
public:

  typedef OpticalDefaultImageMetadataInterface Self;
  typedef OpticalImageMetadataInterface        Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(OpticalDefaultImageMetadataInterface, OpticalImageMetadataInterface);

  typedef Superclass::ImageType                ImageType;
  typedef Superclass::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;
  typedef Superclass::ImageKeywordlistType     ImageKeywordlistType;

  /** Get the radiometric bias from the ossim metadata */
  VariableLengthVectorType GetPhysicalBias() const
  {
    itkExceptionMacro("GetPhysicalBias not implemented in OpticalDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the radiometric gain from the ossim metadata */
  VariableLengthVectorType GetPhysicalGain() const
  {
    itkExceptionMacro("GetPhysicalGain not implemented in OpticalDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the solar irradiance from the ossim metadata */
  VariableLengthVectorType GetSolarIrradiance() const
  {
    itkExceptionMacro("GetSolarIrradiance not implemented in OpticalDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition day from the ossim metadata */
  int GetDay() const
  {
    itkExceptionMacro("GetDay not implemented in OpticalDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition month from the ossim metadata */
  int GetMonth() const
  {
    itkExceptionMacro("GetMonth not implemented in OpticalDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition year from the ossim metadata */
  int GetYear() const
  {
    itkExceptionMacro("GetYear not implemented in OpticalDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition hour from the ossim metadata */
  int GetHour() const
  {
    itkExceptionMacro("GetHour not implemented in OpticalDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition minute from the ossim metadata */
  int GetMinute() const
  {
    itkExceptionMacro("GetMinute not implemented in OpticalDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging production day from the ossim metadata */
  int GetProductionDay() const
  {
    itkExceptionMacro("GetProductionDay not implemented in OpticalDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging production month from the ossim metadata */
  int GetProductionMonth() const
  {
    itkExceptionMacro("GetProductionMonth not implemented in OpticalDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging production year from the ossim metadata */
  int GetProductionYear() const
  {
    itkExceptionMacro("GetProductionYear not implemented in OpticalDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the sat elevation from the ossim metadata */
  double GetSatElevation() const
  {
    itkExceptionMacro("GetSatElevation not implemented in OpticalDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the sat azimuth from the ossim metadata */
  double GetSatAzimuth() const
  {
    itkExceptionMacro("GetSatElevation not implemented in OpticalDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the first wavelength for the spectral band definition */
  VariableLengthVectorType GetFirstWavelengths() const
  {
    itkExceptionMacro("GetFirstWavelengths not implemented in OpticalDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the last wavelength for the spectral band definition */
  VariableLengthVectorType GetLastWavelengths() const
  {
    itkExceptionMacro("GetLastWavelengths not implemented in OpticalDefaultImageMetadataInterface, no captor type found");
  }

  /** Get the enhanced band names (here nothing because the sensor is not identify) */
  std::vector<std::string> GetEnhancedBandNames() const
  {
    std::vector<std::string> nothing;
    return nothing;
  }

  bool CanRead() const
  {
    // This clas is the default one, it has to be able to call every metadata
    return false;
  }

  /** Get the 3 spectral band numbers corresponding to the default display for visualization,
   *  in the order R, G, B */
  virtual std::vector<unsigned int> GetDefaultDisplay() const
      {
    std::vector<unsigned int> rgb(3);
    rgb[0] = 0;
    rgb[1] = 1;
    rgb[2] = 2;
    return rgb;
      }

  /** Vector that contains the filter function value in 6S format (step of 0.0025 micro m).
   * There values a computed by 6S. */
  WavelengthSpectralBandVectorType GetSpectralSensitivity()  const
  {
    itkExceptionMacro("GetSpectralSensitivity not implemented in OpticalDefaultImageMetadataInterface, no captor type found");
  }

protected:
  OpticalDefaultImageMetadataInterface(){};
  virtual ~OpticalDefaultImageMetadataInterface() {}

private:

  OpticalDefaultImageMetadataInterface(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
