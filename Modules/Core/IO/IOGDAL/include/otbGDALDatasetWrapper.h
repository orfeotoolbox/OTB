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

#ifndef otbGDALDatasetWrapper_h
#define otbGDALDatasetWrapper_h

#include "itkLightObject.h"
#include "itkObjectFactory.h"

#include "otbConfigure.h"
#include "otbGeometryMetadata.h"


class GDALDataset;


namespace otb
{

// only two states : the Pointer is Null or GetDataSet() returns a
// valid dataset
class GDALDatasetWrapper : public itk::LightObject
{
  friend class GDALDriverManagerWrapper;

public:
  typedef GDALDatasetWrapper      Self;
  typedef itk::LightObject        Superclass;
  typedef itk::SmartPointer<Self> Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GDALImageIO, itk::LightObject);

  /** Easy access to the internal GDALDataset object.
   *  Don't close it, it will be automatic */
  const GDALDataset* GetDataSet() const;
  GDALDataset*       GetDataSet();

  /** Test if the dataset corresponds to a Jpeg2000 file format
   *  Return true if the dataset exists and has a JPEG2000 driver
   *  Return false in all other cases */
  bool IsJPEG2000() const;

  /**
   */
  int GetOverviewsCount() const;

  /**
   */
  unsigned int GetWidth() const;

  /**
   */
  unsigned int GetHeight() const;

  /**
   */
  size_t GetPixelBytes() const;

  Projection::GCPParam GetGCPParam() const;
  void SetGCPParam(Projection::GCPParam gcpParam);

protected:
  GDALDatasetWrapper();

  ~GDALDatasetWrapper() override;


private:
  GDALDataset* m_Dataset;
}; // end of GDALDatasetWrapper


} // end namespace otb


#endif // otbGDALDatasetWrapper_h
