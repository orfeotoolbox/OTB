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
#ifndef __otbDefaultImageMetadataInterface_h
#define __otbDefaultImageMetadataInterface_h

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbImageMetadataInterfaceBase.h"

namespace otb
{
/** \class DefaultImageMetadataInterface
 *
 * \brief Creation of an "otb" DefaultImageMetadataInterface that gets metadata.
 *
 */
class ITK_EXPORT DefaultImageMetadataInterface : public ImageMetadataInterfaceBase
{
public:

  typedef DefaultImageMetadataInterface Self;
  typedef ImageMetadataInterfaceBase    Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DefaultImageMetadataInterface, ImageMetadataInterfaceBase);

  typedef Superclass::ImageType                ImageType;
  typedef Superclass::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;
  typedef Superclass::ImageKeywordlistType     ImageKeywordlistType;

  /** Get the radiometric bias from the ossim metadata */
  VariableLengthVectorType GetPhysicalBias(const MetaDataDictionaryType&) const
  {
    itkExceptionMacro("GetPhysicalBias not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the radiometric gain from the ossim metadata */
  VariableLengthVectorType GetPhysicalGain(const MetaDataDictionaryType&) const
  {
    itkExceptionMacro("GetPhysicalGain not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the solar irradiance from the ossim metadata */
  VariableLengthVectorType GetSolarIrradiance(const MetaDataDictionaryType&) const
  {
    itkExceptionMacro("GetSolarIrradiance not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition day from the ossim metadata */
  int GetDay(const MetaDataDictionaryType&) const
  {
    itkExceptionMacro("GetDay not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition month from the ossim metadata */
  int GetMonth(const MetaDataDictionaryType&) const
  {
    itkExceptionMacro("GetMonth not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition year from the ossim metadata */
  int GetYear(const MetaDataDictionaryType&) const
  {
    itkExceptionMacro("GetYear not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition hour from the ossim metadata */
  int GetHour(const MetaDataDictionaryType&) const
  {
    itkExceptionMacro("GetHour not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging acquisition minute from the ossim metadata */
  int GetMinute(const MetaDataDictionaryType&) const
  {
    itkExceptionMacro("GetMinute not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging production day from the ossim metadata */
  int GetProductionDay(const MetaDataDictionaryType&) const
  {
    itkExceptionMacro("GetProductionDay not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging production month from the ossim metadata */
  int GetProductionMonth(const MetaDataDictionaryType&) const
  {
    itkExceptionMacro("GetProductionMonth not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the imaging production year from the ossim metadata */
  int GetProductionYear(const MetaDataDictionaryType&) const
  {
    itkExceptionMacro("GetProductionYear not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the sat elevation from the ossim metadata */
  double GetSatElevation(const MetaDataDictionaryType&) const
  {
    itkExceptionMacro("GetSatElevation not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the sat azimuth from the ossim metadata */
  double GetSatAzimuth(const MetaDataDictionaryType&) const
  {
    itkExceptionMacro("GetSatElevation not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the first wavelength for the spectral band definition */
  VariableLengthVectorType GetFirstWavelengths(const MetaDataDictionaryType&) const
  {
    itkExceptionMacro("GetFirstWavelengths not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  /** Get the last wavelength for the spectral band definition */
  VariableLengthVectorType GetLastWavelengths(const MetaDataDictionaryType&) const
  {
    itkExceptionMacro("GetLastWavelengths not implemented in DefaultImageMetadataInterface, no captor type found");
  }

  bool CanRead(const MetaDataDictionaryType&) const
  {
    // This clas is the default one, it has to be able to call every metadata
    return true;
  }

protected:
  DefaultImageMetadataInterface(){};
  virtual ~DefaultImageMetadataInterface() {}

private:

  DefaultImageMetadataInterface(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
