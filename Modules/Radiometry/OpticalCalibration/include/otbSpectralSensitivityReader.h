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

#ifndef otbSpectralSensitivityReader_h
#define otbSpectralSensitivityReader_h

#include "itkProcessObject.h"
#include "itkImageBase.h"
#include "otbObjectList.h"
#include "otbFilterFunctionValues.h"
#include "OTBOpticalCalibrationExport.h"
#include <string>

namespace otb
{

/** \class SpectralSensitivityReader
 *  \brief Read a file that contains spectral sensitivity
 *
 *  This class read a file that contains spectral sensitivity values and fill
 *  a structure that contains man and max wavelengths, the coefficients list and the step.
 *
 *
 * \ingroup OTBOpticalCalibration
 */

class OTBOpticalCalibration_EXPORT SpectralSensitivityReader : public itk::ProcessObject
{
public:
  /** Standard class typedefs */
  typedef SpectralSensitivityReader     Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Creation through the object factory */
  itkNewMacro(Self);
  /** Standard type macros */
  itkTypeMacro(SpectralSensitivityReader, itk::ProcessObject);

  typedef FilterFunctionValues::Pointer             FFVFPointerType;
  typedef FilterFunctionValues::ValuesVectorType    ValuesVectorType;
  typedef ObjectList<FilterFunctionValues>          WavelengthSpectralBandVectorType;
  typedef WavelengthSpectralBandVectorType::Pointer WavelengthSpectralBandVectorPointerType;
  typedef itk::ImageBase<2>                         ImageType;
  typedef ImageType::Pointer                        ImagePointerType;

  /** Get/Set filename */
  itkGetConstMacro(FileName, std::string);
  itkSetStringMacro(FileName);

  /** Get/Set path */
  itkGetConstMacro(DataPath, std::string);
  itkSetStringMacro(DataPath);

  /** Override of the GetOutput() method */
  virtual WavelengthSpectralBandVectorType*       GetOutput(void);
  virtual const WavelengthSpectralBandVectorType* GetOutput(void) const;

  /** Image */
  itkSetObjectMacro(Image, ImageType);
  itkGetConstObjectMacro(Image, ImageType);

protected:
  SpectralSensitivityReader();
  ~SpectralSensitivityReader() override;

  /** Main computation method */
  void GenerateData() override;

  /** Find the filename using image metadata */
  void FindFileName();

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Struct use to remove multiple spaces in file */
  struct BothAre
  {
    char c;
    BothAre(char r) : c(r)
    {
    }
    bool operator()(char l, char r) const
    {
      return r == c && l == c;
    }
  };


private:
  SpectralSensitivityReader(const Self&) = delete;
  void operator=(const Self&) = delete;

  std::string      m_FileName;
  std::string      m_DataPath;
  ImagePointerType m_Image;
};
} // end namespace otb

#endif
