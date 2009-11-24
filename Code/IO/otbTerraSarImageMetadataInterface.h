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

#ifndef __otbTerraSarImageMetadataInterface_h
#define __otbTerraSarImageMetadataInterface_h

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkObject.h"

#include "itkMetaDataDictionary.h"
#include "otbMetaDataKey.h"
#include "otbImageKeywordlist.h"
#include "itkImageBase.h"
#include <string>

namespace otb
{
/** \class TerraSarImageMetadataInterface
 *
 * \brief Creation of an "otb" TerraSarImageMetadataInterface that gets metadata.
 *
 */
  class ITK_EXPORT TerraSarImageMetadataInterface : public itk::Object
{
public:

  typedef TerraSarImageMetadataInterface Self;
  typedef itk::Object                    Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;


  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(TerraSarImageMetadataInterface, itk::Object);

  typedef itk::ImageBase< 2 >                   ImageType;
  typedef itk::MetaDataDictionary               MetaDataDictionaryType;
  typedef MetaDataKey::VectorType               VectorType;
  typedef MetaDataKey::VariableLengthVectorType VariableLengthVectorType;
  typedef ImageKeywordlist                      ImageKeywordlistType;

  /** Set the image used to get the metadata */
  itkSetObjectMacro(Image,ImageType);

 /** Get the radiometric bias from the ossim metadata */
  VariableLengthVectorType GetPhysicalBias( const MetaDataDictionaryType & ) const
  {
    VariableLengthVectorType toto;
    return toto;
  };
  
  /** Get the radiometric gain from the ossim metadata */
  VariableLengthVectorType GetPhysicalGain( const MetaDataDictionaryType & ) const
  {
    VariableLengthVectorType toto;
    return toto;
  };
  
  /** Get the solar irradiance from the ossim metadata */
  VariableLengthVectorType GetSolarIrradiance( const MetaDataDictionaryType & ) const
  {
    VariableLengthVectorType toto;
    return toto;
  };

  /** Get the sensor ID from the ossim metadata */
  std::string GetSensorID(const MetaDataDictionaryType & dict ) const;

  /** Get the imaging acquisition day from the ossim metadata */
  int GetDay( const MetaDataDictionaryType & ) const;

  /** Get the imaging acquisition month from the ossim metadata */
  int GetMonth( const MetaDataDictionaryType & ) const;

  /** Get the imaging acquisition year from the ossim metadata */
  int GetYear( const MetaDataDictionaryType & ) const;

  /** Get the imaging acquisition hour from the ossim metadata */
  int GetHour( const MetaDataDictionaryType & ) const;

  /** Get the imaging acquisition minute from the ossim metadata */
  int GetMinute( const MetaDataDictionaryType & ) const;

  /** Get the imaging production day from the ossim metadata */
  int GetProductionDay( const MetaDataDictionaryType & ) const;

  /** Get the imaging production month from the ossim metadata */
  int GetProductionMonth( const MetaDataDictionaryType & ) const;

  /** Get the imaging production year from the ossim metadata */
  int GetProductionYear( const MetaDataDictionaryType & ) const;

  bool CanRead( const MetaDataDictionaryType & ) const;

protected:
  TerraSarImageMetadataInterface();
  virtual ~TerraSarImageMetadataInterface() {};


private:
  TerraSarImageMetadataInterface(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  OTB_GCP m_GCP;
  ImageType::Pointer m_Image;

};

} // end namespace otb

#endif
