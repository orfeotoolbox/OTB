/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbCosmoImageMetadataInterface_h
#define otbCosmoImageMetadataInterface_h

#include "otbSarImageMetadataInterface.h"


namespace otb
{
/** \class CosmoImageMetadataInterface
 *
 * \brief Creation of an "otb" CosmoImageMetadataInterface that gets metadata.
 *
 *
 * \ingroup OTBMetadata
 */

class OTBMetadata_EXPORT CosmoImageMetadataInterface : public SarImageMetadataInterface
{
public:
  typedef CosmoImageMetadataInterface   Self;
  typedef SarImageMetadataInterface     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);


  /** Run-time type information (and related methods). */
  itkTypeMacro(CosmoImageMetadataInterface, SarImageMetadataInterface);

  typedef Superclass::ImageType                ImageType;
  typedef Superclass::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;
  typedef Superclass::ImageKeywordlistType     ImageKeywordlistType;
  typedef Superclass::RealType                 RealType;
  typedef Superclass::LookupDataPointerType    LookupDataPointerType;

  /** check sensor ID */
  bool CanRead() const override;

  UIntVectorType GetDefaultDisplay() const override;

  /*SarImageMetadataInterface pure virutals rituals */
  double GetPRF() const override;

  double GetRSF() const override;

  double GetRadarFrequency() const override;

  double GetCenterIncidenceAngle() const override;

protected:
  /* class ctor */
  CosmoImageMetadataInterface() = default;

  /* class dtor */
  ~CosmoImageMetadataInterface() = default;

private:
  CosmoImageMetadataInterface(const Self&) = delete;
  void operator=(const Self&) = delete;

  /* Helper function to parse date and time into a std::vector<std::string>
   * using boost::split() expect date time in yyyy-mm-ddThh:mm:ss.ms
   * the date-time string is to be found in keywordlist with key 'key'
   * fills argument dateFields of type std::vector<std::string> which is mutable!
   * TODO: move this method into base class
   */
  void ParseDateTime(std::string key, std::vector<int>& dateFields) const;

  mutable std::vector<int> m_ProductionDateFields;
  mutable std::vector<int> m_AcquisitionDateFields;
};


} // end namespace otb

#endif
