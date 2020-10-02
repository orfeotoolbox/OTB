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
#ifndef otbSOMModel_hxx
#define otbSOMModel_hxx

#include "otbSOMModel.h"
#include "otbSOM.h"

#include "itkMacro.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImage.h"

#include <fstream>

namespace otb
{

namespace internal
{
template <typename T>
std::ostream& BinaryWrite(std::ostream& stream, const T& value)
{
  return stream.write(reinterpret_cast<const char*>(&value), sizeof(T));
}

std::ostream& BinaryWriteString(std::ofstream& stream, const std::string& value)
{
  return stream.write(value.c_str(), value.length());
}

template <typename T>
std::istream& BinaryRead(std::istream& stream, T& value)
{
  return stream.read(reinterpret_cast<char*>(&value), sizeof(T));
}
} // end of namespace internal

template <class TInputValue, unsigned int MapDimension>
SOMModel<TInputValue, MapDimension>::SOMModel()
{
  this->m_Dimension = MapType::ImageDimension;
}

template <class TInputValue, unsigned int MapDimension>
SOMModel<TInputValue, MapDimension>::~SOMModel()
{
}

template <class TInputValue, unsigned int MapDimension>
void SOMModel<TInputValue, MapDimension>::Train()
{
  typedef otb::SOM<InputListSampleType, MapType> EstimatorType;
  typename EstimatorType::Pointer estimator = EstimatorType::New();
  estimator->SetListSample(this->GetInputListSample());
  estimator->SetMapSize(m_MapSize);
  estimator->SetNeighborhoodSizeInit(m_NeighborhoodSizeInit);
  estimator->SetNumberOfIterations(m_NumberOfIterations);
  estimator->SetBetaInit(m_BetaInit);
  estimator->SetBetaEnd(m_BetaEnd);
  estimator->SetMaxWeight(m_MaxWeight);
  estimator->Update();
  m_SOMMap = estimator->GetOutput();
}

template <class TInputValue, unsigned int MapDimension>
bool SOMModel<TInputValue, MapDimension>::CanReadFile(const std::string& filename)
{
  try
  {
    this->Load(filename);
  }
  catch (...)
  {
    return false;
  }
  return true;
}

template <class TInputValue, unsigned int MapDimension>
bool SOMModel<TInputValue, MapDimension>::CanWriteFile(const std::string& /*filename*/)
{
  return true;
}

template <class TInputValue, unsigned int MapDimension>
void SOMModel<TInputValue, MapDimension>::Save(const std::string& filename, const std::string& /*name*/)
{
  itk::ImageRegionConstIterator<MapType> inputIterator(m_SOMMap, m_SOMMap->GetLargestPossibleRegion());
  inputIterator.GoToBegin();
  std::ofstream ofs(filename, std::ios::binary);
  internal::BinaryWriteString(ofs, "som");
  internal::BinaryWrite(ofs, static_cast<unsigned int>(MapDimension));
  SizeType size = m_SOMMap->GetLargestPossibleRegion().GetSize();
  for (size_t i = 0; i < MapDimension; i++)
  {
    internal::BinaryWrite(ofs, size[i]);
  }

  internal::BinaryWrite(ofs, inputIterator.Get().GetNumberOfElements());
  while (!inputIterator.IsAtEnd())
  {
    InputSampleType vect = inputIterator.Get();
    for (size_t i = 0; i < vect.GetNumberOfElements(); i++)
    {
      internal::BinaryWrite(ofs, vect[i]);
    }
    ++inputIterator;
  }
  ofs.close();

  // output the map vectors in a txt file
  if (this->m_WriteMap == true)
  {
    std::ofstream otxt(filename + ".txt");
    inputIterator.GoToBegin();
    while (!inputIterator.IsAtEnd())
    {
      InputSampleType vect = inputIterator.Get();
      for (size_t i = 0; i < vect.GetNumberOfElements(); i++)
      {
        otxt << vect[i] << " ";
      }
      otxt << std::endl;
      ++inputIterator;
    }
    otxt.close();
  }
}

template <class TInputValue, unsigned int MapDimension>
void SOMModel<TInputValue, MapDimension>::Load(const std::string& filename, const std::string& /*name*/)
{
  std::ifstream ifs(filename, std::ios::binary);

  /**  Read the model key (should be som) */
  char s[] = "   ";
  for (int i = 0; i < 3; i++)
  {
    internal::BinaryRead(ifs, s[i]);
  }
  std::string modelType(s);
  /** Read the dimension of the map (should be equal to MapDimension) */

  unsigned int dimension;
  internal::BinaryRead(ifs, dimension);
  if (modelType != "som" || dimension != MapDimension)
  {
    itkExceptionMacro(<< "Error opening " << filename.c_str());
  }

  SizeType                 size;
  itk::Index<MapDimension> index;
  for (unsigned int i = 0; i < MapDimension; i++)
  {
    internal::BinaryRead(ifs, size[i]);
    index[i] = 0;
  }
  unsigned int numberOfElements;
  internal::BinaryRead(ifs, numberOfElements);
  m_SOMMap = MapType::New();
  typename MapType::RegionType region;
  region.SetSize(size);
  m_SOMMap->SetNumberOfComponentsPerPixel(numberOfElements);
  region.SetIndex(index);
  m_SOMMap->SetRegions(region);
  m_SOMMap->Allocate();

  itk::ImageRegionIterator<MapType> outputIterator(m_SOMMap, region);
  outputIterator.GoToBegin();
  std::string value;
  while (!outputIterator.IsAtEnd())
  {
    InputSampleType vect(numberOfElements);
    for (unsigned int i = 0; i < numberOfElements; i++)
    {
      // InputValue type is not the same during training anddimredvector.
      float v;
      internal::BinaryRead(ifs, v);
      vect[i] = static_cast<double>(v);
    }
    outputIterator.Set(vect);
    ++outputIterator;
  }
  ifs.close();
  this->m_Dimension = MapType::ImageDimension;
}

template <class TInputValue, unsigned int MapDimension>
typename SOMModel<TInputValue, MapDimension>::TargetSampleType
SOMModel<TInputValue, MapDimension>::DoPredict(const InputSampleType& value, ConfidenceValueType* /*quality*/, ProbaSampleType* /*proba*/) const
{
  TargetSampleType target;
  target.SetSize(this->m_Dimension);

  auto winner = m_SOMMap->GetWinner(value);
  for (unsigned int i = 0; i < this->m_Dimension; i++)
  {
    target[i] = winner.GetElement(i);
  }
  return target;
}

} // namespace otb
#endif
