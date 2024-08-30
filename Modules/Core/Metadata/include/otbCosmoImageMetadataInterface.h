/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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
#include "otbSARMetadata.h"

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
  typedef Superclass::RealType                 RealType;
  typedef Superclass::LookupDataPointerType    LookupDataPointerType;

  double GetCenterIncidenceAngle(const MetadataSupplierInterface&) const override;

  void Parse(ImageMetadata &) override;

  void ParseGdal(ImageMetadata &) override;
  
  void ParseGeom(ImageMetadata &) override;

  std::vector<std::map<std::string, std::string> > saveMetadataBands(std::string file) ;

  std::vector<Orbit> getOrbits(const std::string & referenceTime) const;
  
  std::vector<BurstRecord> CreateBurstRecord(const std::string & firstLineTimeStr,
					     const std::string & lastLineTimeStr,
					     const unsigned long endLine, 
					     const unsigned long endSample) const;

protected:
  /* class ctor */
  CosmoImageMetadataInterface() = default;

  /* class dtor */
  ~CosmoImageMetadataInterface() = default;

private:
  CosmoImageMetadataInterface(const Self&) = delete;
  void operator=(const Self&) = delete;
};


} // end namespace otb

#endif
