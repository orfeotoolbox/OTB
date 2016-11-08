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
#ifndef otbWorldView2ImageMetadataInterface_h
#define otbWorldView2ImageMetadataInterface_h

#include "otbOpticalImageMetadataInterface.h"

namespace otb
{
/** \class WorldView2ImageMetadataInterface
 *
 * \brief Creation of an "otb" WorldView2ImageMetadataInterface that gets metadata.
 *
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT WorldView2ImageMetadataInterface : public OpticalImageMetadataInterface
{
public:

  typedef WorldView2ImageMetadataInterface Self;
  typedef ImageMetadataInterfaceBase       Superclass;
  typedef itk::SmartPointer<Self>          Pointer;
  typedef itk::SmartPointer<const Self>    ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(WorldView2ImageMetadataInterface, ImageMetadataInterfaceBase);

  typedef Superclass::ImageType                ImageType;
  typedef Superclass::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;
  typedef Superclass::ImageKeywordlistType     ImageKeywordlistType;

  /** Get the radiometric bias from the ossim metadata */
  VariableLengthVectorType GetPhysicalBias() const ITK_OVERRIDE;

  /** Get the radiometric gain from the ossim metadata */
  VariableLengthVectorType GetPhysicalGain() const ITK_OVERRIDE;

  /** Get the solar irradiance from the ossim metadata */
  VariableLengthVectorType GetSolarIrradiance() const ITK_OVERRIDE;

  /** Get the imaging acquisition day from the ossim metadata : TLCTime metadata value */
  int GetDay() const ITK_OVERRIDE;

  /** Get the imaging acquisition month from the ossim metadata : TLCTime metadata value */
  int GetMonth() const ITK_OVERRIDE;

  /** Get the imaging acquisition year from the ossim metadata : TLCTime metadata value */
  int GetYear() const ITK_OVERRIDE;

  /** Get the imaging acquisition hour from the ossim metadata : TLCTime metadata value */
  int GetHour() const ITK_OVERRIDE;

  /** Get the imaging acquisition year from the ossim metadata : TLCTime metadata value */
  int GetMinute() const ITK_OVERRIDE;

  /** Get the imaging production day from the ossim metadata : generationTime metadata value */
  int GetProductionDay() const ITK_OVERRIDE;

  /** Get the imaging production month from the ossim metadata : generationTime metadata value */
  int GetProductionMonth() const ITK_OVERRIDE;

  /** Get the imaging production year from the ossim metadata : generationTime metadata value */
  int GetProductionYear() const ITK_OVERRIDE;

  /** Get the sat elevation from the ossim metadata */
  double GetSatElevation() const ITK_OVERRIDE;

  /** Get the sat azimuth from the ossim metadata */
  double GetSatAzimuth() const ITK_OVERRIDE;

  /** Get the first wavelength for the spectral band definition */
  VariableLengthVectorType GetFirstWavelengths() const ITK_OVERRIDE;

  /** Get the last wavelength for the spectral band definition */
  VariableLengthVectorType GetLastWavelengths() const ITK_OVERRIDE;

  bool CanRead() const ITK_OVERRIDE;

  /** Get the 3 spectral band numbers corresponding to the default display for visualization,
   *  in the order R, G, B */
  std::vector<unsigned int> GetDefaultDisplay() const ITK_OVERRIDE;

  /** Vector that contains the filter function value in 6S format (step of 0.0025 micro m).
   * There values a computed by 6S. */
  WavelengthSpectralBandVectorType GetSpectralSensitivity()  const ITK_OVERRIDE;

  /** Get the enhanced band names from band names collected by ossim */
  std::vector<std::string> GetEnhancedBandNames() const ITK_OVERRIDE;

protected:
  WorldView2ImageMetadataInterface();
  ~WorldView2ImageMetadataInterface() ITK_OVERRIDE {}

private:

  WorldView2ImageMetadataInterface(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
