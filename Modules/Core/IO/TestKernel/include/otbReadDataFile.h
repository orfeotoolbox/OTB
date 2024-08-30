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

#include "otbStringUtils.h"
#include "otbStringUtilities.h"
#include "otb_boost_string_header.h"

#include "itkListSample.h"
#include <string>
#include <algorithm>

namespace otb
{
/** Utility function to read the data file letter.scale, a CSV type file
 *  (whitespace separators) with the letter index in first column, followed by
 *  16 descriptors. Each descriptor is a pair 'index:value'
 */
template <typename TInput, typename TTarget>
bool ReadDataFile(const std::string& infname, itk::SmartPointer<itk::Statistics::ListSample<TInput>> samples,
                  itk::SmartPointer<itk::Statistics::ListSample<TTarget>> labels)
{
  typedef typename itk::Statistics::ListSample<TInput>::MeasurementType  IValueType;
  typedef typename itk::Statistics::ListSample<TTarget>::MeasurementType TValueType;

  std::ifstream ifs;
  ifs.open(infname);

  if (!ifs)
  {
    std::cout << "Could not read file " << infname << std::endl;
    return false;
  }

  labels->SetMeasurementVectorSize(1);
  unsigned int nbfeatures = 0;

  for (std::string line; std::getline(ifs, line) ;)
  {
    boost::algorithm::trim(line);

    if (nbfeatures == 0)
    {
      nbfeatures = std::count(line.begin(), line.end(), ' ');
      samples->SetMeasurementVectorSize(nbfeatures);
    }

    if (line.empty())
      continue;
   
    TInput sample;
    itk::NumericTraits<TInput>::SetLength(sample, nbfeatures);

    auto const parts = otb::split_on(line, ' ');
    auto       it_parts = parts.begin();
    auto const end_parts = parts.end();

    if (it_parts == end_parts) {
      throw std::runtime_error("Line is not correctly formatted.");
    }

    // Parse label
    TTarget label;
    itk::NumericTraits<TTarget>::SetLength(label, 1);
    label[0] = to<TValueType>(*it_parts, "extracting label");

    unsigned int id        = 0;
    
    IValueType value;
    for (++it_parts ; it_parts != end_parts ;  ++it_parts)
    {
      auto semicolon_pos = std::find(it_parts->begin(), it_parts->end(), ':');
      if (semicolon_pos == it_parts->end())
      {
          id++;
          value = to<IValueType>(*it_parts, "extracting field");
      }
      else
      {
	id = to<unsigned int>(string_view(it_parts->begin(), semicolon_pos), "extracting field id");
	value = to<IValueType>(string_view(semicolon_pos + 1, it_parts->end()), "extracting field");
      }
      if (id > nbfeatures)
      {
      	throw std::runtime_error("id is higher than the number of features.");
      }
      sample[id - 1] = value;
    }
    samples->PushBack(sample);
    labels->PushBack(label);
  }

  ifs.close();
  return true;
}

} // end of namespace otb
