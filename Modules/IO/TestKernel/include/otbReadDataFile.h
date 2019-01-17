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

#include "otbStringUtils.h"
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
bool ReadDataFile(
  const std::string & infname,
  itk::SmartPointer<itk::Statistics::ListSample<TInput> > samples,
  itk::SmartPointer<itk::Statistics::ListSample<TTarget> > labels)
{
  typedef typename itk::Statistics::ListSample<TInput>::MeasurementType IValueType;
  typedef typename itk::Statistics::ListSample<TTarget>::MeasurementType TValueType;

  std::ifstream ifs;
  ifs.open(infname);

  if(!ifs)
    {
    std::cout<<"Could not read file "<<infname<<std::endl;
    return false;
    }

  labels->SetMeasurementVectorSize(1);
  unsigned int nbfeatures = 0;

  while (!ifs.eof())
    {
    std::string line;
    std::getline(ifs, line);
    boost::algorithm::trim(line);

    if(nbfeatures == 0)
      {
      nbfeatures = std::count(line.begin(),line.end(),' ');
      samples->SetMeasurementVectorSize(nbfeatures);
      }

    if(line.size()>1)
      {
      TInput sample;
      itk::NumericTraits<TInput>::SetLength(sample, nbfeatures);

      std::string::size_type pos = line.find_first_of(" ", 0);

      // Parse label
      TTarget label;
      itk::NumericTraits<TTarget>::SetLength(label,1);
      label[0] = boost::lexical_cast<TValueType>(line.substr(0, pos));

      bool endOfLine = false;
      unsigned int id = 0;

      while(!endOfLine)
        {
        std::string::size_type nextpos = line.find_first_of(" ", pos+1);
        if(nextpos == std::string::npos)
          {
          endOfLine = true;
          nextpos = line.size();
          }

        std::string::size_type semicolonpos = line.find_first_of(":", pos+1, nextpos-pos-1);
        if (semicolonpos == std::string::npos)
          {
          id++;
          sample[id - 1] = boost::lexical_cast<IValueType>(line.substr(pos+1,nextpos-pos-1));
          }
        else
          {
          id = boost::lexical_cast<unsigned int>(line.substr(pos+1,semicolonpos-pos-1));
          sample[id - 1] = boost::lexical_cast<IValueType>(
            line.substr(semicolonpos+1,nextpos-semicolonpos-1));
          }
        pos = nextpos;

        }
      samples->PushBack(sample);
      labels->PushBack(label);
      }
    }

  ifs.close();
  return true;
}

} // end of namespace otb
