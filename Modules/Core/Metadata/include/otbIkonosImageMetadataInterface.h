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
#ifndef __otbIkonosImageMetadataInterface_h
#define __otbIkonosImageMetadataInterface_h

#include "otbOpticalImageMetadataInterface.h"

namespace otb
{
/** \class IkonosImageMetadataInterface
 *
 * \brief Creation of an "otb" IkonosImageMetadataInterface that gets metadata.
 *
 */
class ITK_EXPORT IkonosImageMetadataInterface : public OpticalImageMetadataInterface
{
public:

  typedef IkonosImageMetadataInterface  Self;
  typedef ImageMetadataInterfaceBase    Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(IkonosImageMetadataInterface, ImageMetadataInterfaceBase);

  typedef Superclass::ImageType                ImageType;
  typedef Superclass::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;
  typedef Superclass::ImageKeywordlistType     ImageKeywordlistType;

  /** Get the radiometric bias from the ossim metadata */
  VariableLengthVectorType GetPhysicalBias() const;

  /** Get the radiometric gain from the ossim metadata */
  VariableLengthVectorType GetPhysicalGain() const;

  /** Get the solar irradiance from the ossim metadata */
  VariableLengthVectorType GetSolarIrradiance() const;

  /** Get the imaging acquisition day from the ossim metadata : "Acquisition Date/Time" metadata variable */
  int GetDay() const;

  /** Get the imaging acquisition month from the ossim metadata : "Acquisition Date/Time" metadata variable */
  int GetMonth() const;

  /** Get the imaging acquisition year from the ossim metadata : "Acquisition Date/Time" metadata variable */
  int GetYear() const;

  /** Get the imaging acquisition hour from the ossim metadata : "Acquisition Date/Time" metadata variable */
  int GetHour() const;

  /** Get the imaging acquisition year from the ossim metadata : "Acquisition Date/Time" metadata variable */
  int GetMinute() const;

  /** Get the imaging production day from the ossim metadata : "Creation Date" metadata variable */
  int GetProductionDay() const;

  /** Get the imaging production month from the ossim metadata : "Creation Date" metadata variable */
  int GetProductionMonth() const;

  /** Get the imaging production year from the ossim metadata : "Creation Date" metadata variable */
  int GetProductionYear() const;

  /** Get the sat elevation from the ossim metadata */
  double GetSatElevation() const;

  /** Get the sat azimuth from the ossim metadata */
  double GetSatAzimuth() const;

  /** Get the first wavelength for the spectral band definition */
  VariableLengthVectorType GetFirstWavelengths() const;

  /** Get the last wavelength for the spectral band definition */
  VariableLengthVectorType GetLastWavelengths() const;

  bool CanRead() const;

  /** Get the enhanced band names of Ikonos data*/
  std::vector<std::string> GetEnhancedBandNames() const;

  /** Get the 3 spectral band numbers corresponding to the default display for visualization,
   *  in the order R, G, B */
  std::vector<unsigned int> GetDefaultDisplay() const;

  /** Vector that contains the filter function value in 6S format (step of 0.0025 micro m).
     * There values a computed by 6S. */
  WavelengthSpectralBandVectorType GetSpectralSensitivity()  const;

protected:
  IkonosImageMetadataInterface();
  virtual ~IkonosImageMetadataInterface() {}

private:

  IkonosImageMetadataInterface(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
