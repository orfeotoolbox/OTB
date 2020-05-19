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

#include <ostream>

#include "otbVectorDataIOBase.h"

namespace otb
{
VectorDataIOBase::VectorDataIOBase() : m_ByteOrder(OrderNotApplicable)
{
  this->Reset(false);
}

void VectorDataIOBase::Reset(const bool)
{
  m_Initialized = false;
  m_FileName    = "";
}


VectorDataIOBase::~VectorDataIOBase()
{
}

std::string VectorDataIOBase::GetByteOrderAsString(ByteOrder t) const
{
  std::string s;
  switch (t)
  {
  case BigEndian:
    return s = "BigEndian";
  case LittleEndian:
    return s = "LittleEndian";
  case OrderNotApplicable:
  default:
    return s = "OrderNotApplicable";
  }
}

void VectorDataIOBase::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "FileName: " << m_FileName << std::endl;
  os << indent << "ByteOrder: " << this->GetByteOrderAsString(m_ByteOrder) << std::endl;
}

} // namespace otb
