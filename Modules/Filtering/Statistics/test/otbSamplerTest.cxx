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


#include "otbPeriodicSampler.h"
#include "otbPatternSampler.h"
#include "otbRandomSampler.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"

template <typename SamplerType>
std::string RunSampler(SamplerType* sampler, unsigned long total)
{
  std::string        output;
  std::ostringstream oss;
  sampler->Reset();
  for (unsigned long i = 0UL; i < total; ++i)
  {
    if (sampler->TakeSample())
    {
      oss << "X";
    }
    else
    {
      oss << "_";
    }
  }
  output = oss.str();
  std::cout << "Sampling result : " << output << std::endl;
  return output;
}

int otbPeriodicSamplerTest(int, char* [])
{
  otb::PeriodicSampler::Pointer       sampler = otb::PeriodicSampler::New();
  otb::PeriodicSampler::ParameterType param;
  param.Offset    = 2;
  param.MaxJitter = 0;

  sampler->SetNumberOfElements(5, 23);
  sampler->SetParameters(param);
  std::string test1 = RunSampler<otb::PeriodicSampler>(sampler, 23);

  if (sampler->GetChosenElements() != sampler->GetNeededElements())
  {
    std::cout << "1 - Wrong nb of needed elements :" << std::endl;
    std::cout << "  expected : " << sampler->GetNeededElements() << std::endl;
    std::cout << "  got : " << sampler->GetChosenElements() << std::endl;
    return EXIT_FAILURE;
  }

  std::string baseline1("__X___X____X___X____X__");
  if (test1 != baseline1)
  {
    std::cout << "1 - Wrong sampling sequence :" << std::endl;
    std::cout << "  expected : " << baseline1 << std::endl;
    std::cout << "  got : " << test1 << std::endl;
    return EXIT_FAILURE;
  }

  param.Offset        = 0;
  param.MaxJitter     = 10;
  param.MaxBufferSize = 1000000UL;
  sampler->SetRate(0.2, 50);
  sampler->SetParameters(param);
  std::string test2 = RunSampler<otb::PeriodicSampler>(sampler, 50);

  if (sampler->GetChosenElements() != sampler->GetNeededElements())
  {
    std::cout << "2 - Wrong nb of needed elements :" << std::endl;
    std::cout << "  expected : " << sampler->GetNeededElements() << std::endl;
    std::cout << "  got : " << sampler->GetChosenElements() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int otbPatternSamplerTest(int, char* [])
{
  otb::PatternSampler::Pointer       sampler = otb::PatternSampler::New();
  otb::PatternSampler::ParameterType param;
  param.Seed           = 121212;
  param.MaxPatternSize = 100;

  sampler->SetNumberOfElements(5, 23);
  sampler->SetParameters(param);
  std::string test1 = RunSampler<otb::PatternSampler>(sampler, 23);

  if (sampler->GetChosenElements() != sampler->GetNeededElements())
  {
    std::cout << "1 - Wrong nb of needed elements :" << std::endl;
    std::cout << "  expected : " << sampler->GetNeededElements() << std::endl;
    std::cout << "  got : " << sampler->GetChosenElements() << std::endl;
    return EXIT_FAILURE;
  }

  param.Seed = 0;
  param.Pattern1.clear();
  param.Pattern1.push_back(false);
  param.Pattern1.push_back(true);
  param.Pattern1.push_back(false);
  param.Pattern1.push_back(false);
  param.Pattern1.push_back(true);
  param.Pattern1.push_back(true);
  param.Pattern1.push_back(false);
  param.Pattern2.clear();
  // here, the pattern is imposed so the sampling rate will be ignored
  sampler->SetRate(0.34, 23);
  sampler->SetParameters(param);
  std::string test2 = RunSampler<otb::PatternSampler>(sampler, 23);
  std::string baseline2("_X__XX__X__XX__X__X____");
  if (test2 != baseline2)
  {
    std::cout << "2 - Wrong sampling sequence :" << std::endl;
    std::cout << "  expected : " << baseline2 << std::endl;
    std::cout << "  got : " << test2 << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int otbRandomSamplerTest(int, char* [])
{
  itk::Statistics::MersenneTwisterRandomVariateGenerator::GetInstance()->SetSeed(121212);
  otb::RandomSampler::Pointer       sampler = otb::RandomSampler::New();
  otb::RandomSampler::ParameterType param;
  param.MaxBufferSize = 100000000;

  sampler->SetNumberOfElements(5, 23);
  sampler->SetParameters(param);
  std::string test1 = RunSampler<otb::RandomSampler>(sampler, 23);

  if (sampler->GetChosenElements() != sampler->GetNeededElements())
  {
    std::cout << "1 - Wrong nb of needed elements :" << std::endl;
    std::cout << "  expected : " << sampler->GetNeededElements() << std::endl;
    std::cout << "  got : " << sampler->GetChosenElements() << std::endl;
    return EXIT_FAILURE;
  }

  param.MaxBufferSize = 5;
  sampler->SetRate(0.1, 23);
  sampler->SetParameters(param);
  std::string test2 = RunSampler<otb::RandomSampler>(sampler, 23);

  if (sampler->GetChosenElements() != sampler->GetNeededElements())
  {
    std::cout << "2 - Wrong nb of needed elements :" << std::endl;
    std::cout << "  expected : " << sampler->GetNeededElements() << std::endl;
    std::cout << "  got : " << sampler->GetChosenElements() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
