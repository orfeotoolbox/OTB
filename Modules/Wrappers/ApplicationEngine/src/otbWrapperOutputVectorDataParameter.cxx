/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#include "otbWrapperOutputVectorDataParameter.h"

#include <string>

namespace otb
{
namespace Wrapper
{

bool OutputVectorDataParameter::HasValue() const
{
  std::string filename(this->GetFileName());
  return !filename.empty();
}

/** Return any value */
void OutputVectorDataParameter::SetValue(VectorDataType* vd)
{
  m_VectorData = vd;
  SetActive(true);
}

/** Return any value */
VectorDataType* OutputVectorDataParameter::GetValue(void)
{
  return m_VectorData;
}

void OutputVectorDataParameter::SetFileName(const char* filename)
{
  this->SetFileName(std::string(filename));
}

void OutputVectorDataParameter::SetFileName(const std::string& filename)
{
  m_FileName = filename;
  SetActive(true);
}

void OutputVectorDataParameter::Write()
{
  m_Writer->SetFileName(m_FileName);
  m_Writer->SetInput(m_VectorData);
  m_Writer->Update();
}

itk::ProcessObject* OutputVectorDataParameter::GetWriter()
{
  return m_Writer;
}

void OutputVectorDataParameter::InitializeWriters()
{
  m_Writer = otb::VectorDataFileWriter<VectorDataType>::New();
}

OutputVectorDataParameter::OutputVectorDataParameter()
{
  this->SetName("Output Vector Data");
  this->SetKey("outvd");
}
}
}
