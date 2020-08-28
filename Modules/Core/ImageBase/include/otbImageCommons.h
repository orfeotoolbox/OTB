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

#ifndef otbImageCommons_h
#define otbImageCommons_h

#include "otbImageMetadata.h"
#include "otbImageMetadataInterfaceBase.h"
#include "OTBImageBaseExport.h"

namespace otb
{

class OTBImageBase_EXPORT ImageCommons
{
public:

  void SetImageMetadata(ImageMetadata imd);
  
  void SetBandImageMetadata(ImageMetadata::ImageMetadataBandsType imd);

  const ImageMetadata & GetImageMetadata() const;

  // boilerplate code...

  /** Get the projection coordinate system of the image. */
  std::string GetProjectionRef(void) const;

  void SetProjectionRef(const std::string& wkt);

  /** Get the GCP projection coordinates of the image. */
  std::string GetGCPProjection(void) const;

  unsigned int GetGCPCount(void) const;

  // Removed non-const getter on GCPs
  //~ GCP& GetGCPs(unsigned int GCPnum);
  
  const GCP& GetGCPs(unsigned int GCPnum) const;

  std::string GetGCPId(unsigned int GCPnum) const;
  std::string GetGCPInfo(unsigned int GCPnum) const;
  double GetGCPRow(unsigned int GCPnum) const;
  double GetGCPCol(unsigned int GCPnum) const;
  double GetGCPX(unsigned int GCPnum) const;
  double GetGCPY(unsigned int GCPnum) const;
  double GetGCPZ(unsigned int GCPnum) const;

  /** Returns true if a sensor geometric model is present */
  bool HasSensorGeometry() const;

private:
  ImageMetadata m_Imd;
};

} // end namespace otb

#endif
