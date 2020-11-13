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

#include "otbGDALRPCTransformer.h"
#include <assert.h>
#include "cpl_string.h"

namespace otb
{
GDALRPCTransformer::GDALRPCTransformer(double LineOffset, double SampleOffset, double LatOffset, double LonOffset, double HeightOffset,
				       double LineScale, double SampleScale, double LatScale, double LonScale, double HeightScale,
				       const double (&LineNum)[20], const double (&LineDen)[20], const double (&SampleNum)[20], const double (&SampleDen)[20])
{
  // Offsets
  this->m_GDALRPCInfo.dfLINE_OFF = LineOffset;
  this->m_GDALRPCInfo.dfSAMP_OFF = SampleOffset;
  this->m_GDALRPCInfo.dfLAT_OFF = LatOffset;
  this->m_GDALRPCInfo.dfLONG_OFF = LonOffset;
  this->m_GDALRPCInfo.dfHEIGHT_OFF = HeightOffset;
  // Scales
  this->m_GDALRPCInfo.dfLINE_SCALE = LineScale;
  this->m_GDALRPCInfo.dfSAMP_SCALE = SampleScale;
  this->m_GDALRPCInfo.dfLAT_SCALE = LatScale;
  this->m_GDALRPCInfo.dfLONG_SCALE = LonScale;
  this->m_GDALRPCInfo.dfHEIGHT_SCALE = HeightScale;
  // Coefficients
  std::copy_n(LineNum, 20, this->m_GDALRPCInfo.adfLINE_NUM_COEFF);
  std::copy_n(LineDen, 20, this->m_GDALRPCInfo.adfLINE_DEN_COEFF);
  std::copy_n(SampleNum, 20, this->m_GDALRPCInfo.adfSAMP_NUM_COEFF);
  std::copy_n(SampleDen, 20, this->m_GDALRPCInfo.adfSAMP_DEN_COEFF);
}

void GDALRPCTransformer::SetOption(const std::string& Name, const std::string& Value)
{
  this->m_Options = CSLSetNameValue(this->m_Options, Name.c_str(), Value.c_str());
  this->m_Modified = true;
}

void GDALRPCTransformer::SetPixErrThreshold(double PixErrThreshold)
{
  this->m_PixErrThreshold = PixErrThreshold;
  this->m_Modified = true;
}

void GDALRPCTransformer::Update()
{
  this->m_TransformArg = GDALCreateRPCTransformer(&this->m_GDALRPCInfo, false, this->m_PixErrThreshold, this->m_Options);
  this->m_Modified = false;
}

bool GDALRPCTransformer::ForwardTransform(double* x, double* y, double* z, int nPointCount)
{
  assert(x);
  assert(y);
  assert(z);
  if (this->m_Modified)
    this->Update();
  std::vector<int> success(nPointCount);
  GDALRPCTransform(this->m_TransformArg, false, nPointCount, x, y, z, success.data());
  bool finalSuccess = std::all_of(success.begin(), success.end(), [](int i){return i;});
  return finalSuccess;
}

GDALRPCTransformer::PointType GDALRPCTransformer::ForwardTransform(GDALRPCTransformer::PointType p)
{
  if (m_Modified)
    this->Update();
  int success;
  GDALRPCTransform(this->m_TransformArg, false, 1, &p[0], &p[1], &p[2], &success);
  if (!success)
    throw std::runtime_error("GDALRPCTransform was not able to process the ForwardTransform.");
  return p;
}


bool GDALRPCTransformer::InverseTransform(double* x, double* y, double* z, int nPointCount)
{
  assert(x);
  assert(y);
  assert(z);
  if (this->m_Modified)
    this->Update();
  std::vector<int> success(nPointCount);
  GDALRPCTransform(this->m_TransformArg, true, nPointCount, x, y, z, success.data());
  bool finalSuccess = std::all_of(success.begin(), success.end(), [](int i){return i;});
  return finalSuccess;
}

GDALRPCTransformer::PointType GDALRPCTransformer::InverseTransform(GDALRPCTransformer::PointType p)
{
  if (m_Modified)
    this->Update();
  int success;
  GDALRPCTransform(this->m_TransformArg, true, 1, &p[0], &p[1], &p[2], &success);
  if (!success)
    throw std::runtime_error("GDALRPCTransform was not able to process the InverseTransform.");
  return p;
}
}
