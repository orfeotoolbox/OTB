/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbRCC8GraphFileReader_hxx
#define otbRCC8GraphFileReader_hxx

#include "otbRCC8GraphFileReader.h"
#include "otbRCC8VertexIterator.h"
#include "otbMacro.h"
#include <fstream>
#include <iostream>
#include <string>

namespace otb
{
/**
 * Constructor
 */
template <class TOutputGraph>
RCC8GraphFileReader<TOutputGraph>
::RCC8GraphFileReader()
{
  m_FileName = "";
}
/**
 * Destructor
 */
template <class TOutputGraph>
RCC8GraphFileReader<TOutputGraph>
::~RCC8GraphFileReader()
{
}
/**
 * Parse edge information from a given line.
 * \param line The line to parse.
 */
template <class TOutputGraph>
void
RCC8GraphFileReader<TOutputGraph>
::ParseEdge(const std::string& line)
{
  typename std::string::size_type pos1 = line.find_first_of(" ", 0);
  typename std::string::size_type pos2 = line.find_first_of(" ", pos1 + 1);
  typename std::string::size_type pos3 = line.find_first_of(" ", pos2 + 1);
  unsigned int                    source = atoi(line.substr(0, pos1).c_str());
  unsigned int                    target = atoi(line.substr(pos2 + 1, pos3 - pos2 - 1).c_str());
  typename std::string::size_type pos4 = line.find_first_of("\"", 0);
  typename std::string::size_type pos5 = line.find_first_of("\" ", pos4 + 1);
  RCC8ValueType                   value = static_cast<RCC8ValueType>(atoi(line.substr(pos4 + 1, pos5 - pos4 - 1).c_str()));
  otbMsgDevMacro(<< "RCC8GraphFileReader: Edge line parsed: " << source << " -> "
                 << target << " " << value);
  this->GetOutput()->AddEdge(source, target, value);
}
/**
 * Parse vertex information from a given line.
 * \param line The line to parse.
 */
template <class TOutputGraph>
void
RCC8GraphFileReader<TOutputGraph>
::ParseVertex(const std::string& line)
{
  typename VertexType::AttributesMapType attr;
  typename std::string::size_type        pos = line.find_first_of(" ", 0);
  unsigned int                           index = atoi(line.substr(0, pos).c_str());
  otbMsgDevMacro(<< "RCC8GraphFileReader: Vertex index: " << index);
  typename std::string::size_type midPos, nextPos;

  midPos = line.find_first_of("\"", pos + 2);
  nextPos = line.find_first_of("\"", midPos + 1);
  std::string key, value;
  while ((midPos != std::string::npos) && (nextPos != std::string::npos))
    {
    key = line.substr(pos + 2, midPos - pos - 3);
    value = line.substr(midPos + 1, nextPos - midPos - 1);
    attr[key] = value;
    otbMsgDevMacro(<< "RCC8GraphFileReader: Vertex attribute: " << key << " " << value);
    pos = nextPos;
    midPos = line.find_first_of("\"", pos + 2);
    nextPos = line.find_first_of("\"", midPos + 1);
    }
  typename VertexType::Pointer vertex  = VertexType::New();
  vertex->SetAttributesMap(attr);
  this->GetOutput()->SetVertex(index, vertex);
}

/**
 * Generate data method
 */
template <class TOutputGraph>
void
RCC8GraphFileReader<TOutputGraph>
::GenerateData()
{
  std::ifstream fin;
  std::string   line;

  // open file input stream
  fin.open(m_FileName);

  // Test if the file has been opened correctly
  if (!fin)
    {
    RCC8GraphFileReaderException e(__FILE__, __LINE__);
    std::ostringstream msg;
    msg << " Could not create IO object for file ";
    msg << m_FileName << "." << std::endl;
    e.SetDescription(msg.str());
    throw e;
    return;
    }

  // if so, parse it
  while (!fin.eof())
    {
    std::getline(fin, line);
    if (line.find("->") != std::string::npos)
      {
      // edge line
      this->ParseEdge(line);
      }
    else if (line.find("[") != std::string::npos)
      {
      // vertex line
      this->ParseVertex(line);
      }
    }
  fin.close();
}
/**
 * PrintSelf method
 */
template <class TInputGraph>
void
RCC8GraphFileReader<TInputGraph>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // end namespace otb

#endif
