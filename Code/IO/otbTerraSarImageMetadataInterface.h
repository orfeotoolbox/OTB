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
#include <otb/Noise.h>
#include <otb/IncidenceAngles.h>
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
  typedef ImageType::IndexType                  IndexType;
  typedef std::vector<IndexType>                IndexVectorType;
  typedef itk::MetaDataDictionary               MetaDataDictionaryType;
  typedef MetaDataKey::VectorType               VectorType;
  typedef MetaDataKey::VariableLengthVectorType VariableLengthVectorType;
  typedef ImageKeywordlist                      ImageKeywordlistType;
  typedef std::vector<double>                   DoubleVectorType;
  typedef std::vector<DoubleVectorType>         DoubleVectorVectorType;
  typedef std::vector<unsigned int>             UIntVectorType;

  /** Set the image used to get the metadata */
  itkSetObjectMacro(Image,ImageType);

  /** Get the sensor ID from the ossim metadata */
  std::string GetSensorID(const MetaDataDictionaryType & dict ) const;

  /** Get the imaging start acquisition day from the ossim metadata */
  int GetDay( const MetaDataDictionaryType & ) const;

  /** Get the imaging start acquisition month from the ossim metadata */
  int GetMonth( const MetaDataDictionaryType & ) const;

  /** Get the imaging start acquisition year from the ossim metadata */
  int GetYear( const MetaDataDictionaryType & ) const;

  /** Get the imaging start acquisition hour from the ossim metadata */
  int GetHour( const MetaDataDictionaryType & ) const;

  /** Get the imaging start acquisition minute from the ossim metadata */
  int GetMinute( const MetaDataDictionaryType & ) const;

  /** Get the imaging production day from the ossim metadata : generationTime variable */
  int GetProductionDay( const MetaDataDictionaryType & ) const;

  /** Get the imaging production month from the ossim metadata : generationTime variable */
  int GetProductionMonth( const MetaDataDictionaryType & ) const;

  /** Get the imaging production year from the ossim metadata : generationTime variable */
  int GetProductionYear( const MetaDataDictionaryType & ) const;

  /** Get the calibration.calFactor : generationTime variable */
   double GetCalibrationFactor( const MetaDataDictionaryType & ) const;

   /** Get the noise structure */
   ossimplugins::Noise * GetNoise( const MetaDataDictionaryType & ) const;
   
   /** Get the number of noise records */   
   unsigned int GetNumberOfNoiseRecords( const MetaDataDictionaryType & ) const;
   
   /** Get the polynomial degree list */
   UIntVectorType GetNoisePolynomialDegrees( const MetaDataDictionaryType & ) const;

  /** Get the polynomial coefficient list */
   DoubleVectorVectorType GetNoisePolynomialCoefficientsList( const MetaDataDictionaryType & ) const;

   /** Get timeUTC noise acquisition list in Julian day */
   DoubleVectorType GetNoiseTimeUTCList( const MetaDataDictionaryType & ) const;

   /** Get noise minimum validity range list */
   DoubleVectorType GetNoiseValidityRangeMinList( const MetaDataDictionaryType & ) const;

   /** Get noise maximum validity range list */
   DoubleVectorType GetNoiseValidityRangeMaxList( const MetaDataDictionaryType & ) const; 
  
   /** Get noise reference point list */
   DoubleVectorType GetNoiseReferencePointList( const MetaDataDictionaryType & ) const; 

   /** Get the radar frequency */
   double GetRadarFrequency( const MetaDataDictionaryType & ) const;
 
   /** Get the PRF */
   double GetPRF( const MetaDataDictionaryType & ) const;
   
   /** Get the incidence angles structure */
   ossimplugins::IncidenceAngles* GetIncidenceAngles( const MetaDataDictionaryType & ) const;
   
   /** Get the number of corner incidence angles */
   unsigned int GetNumberOfCornerIncidenceAngles( const MetaDataDictionaryType & ) const;
   
   /** Get the Mean Incidence angles */
   double GetMeanIncidenceAngles( const MetaDataDictionaryType & ) const;
   
   /** Get the center incidence angle */
   double GetCenterIncidenceAngle( const MetaDataDictionaryType & ) const;
    
   /** Get the center index */
   IndexType GetCenterIncidenceAngleIndex( const MetaDataDictionaryType & ) const;
   
   /** Get the corners incidence angles */
   DoubleVectorType GetCornersIncidenceAngles( const MetaDataDictionaryType & ) const;
   
   /** Get the corners index */
   IndexVectorType GetCornersIncidenceAnglesIndex( const MetaDataDictionaryType & ) const;
   
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
