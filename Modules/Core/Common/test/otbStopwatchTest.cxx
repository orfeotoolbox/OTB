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


#include <cassert>
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <thread>

#include "itkMacro.h"

#include "otbStopwatch.h"

using namespace std::chrono_literals;

int otbStopwatchTest(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  otb::Stopwatch sw;

  assert(!sw.IsRunning());
  assert(sw.GetElapsedMilliseconds() == 0);

  sw.Start();
  assert(sw.IsRunning());
  sw.Stop();

  sw.Reset();
  assert(!sw.IsRunning());
  assert(sw.GetElapsedMilliseconds() == 0);

  sw = otb::Stopwatch::StartNew();
  assert(sw.IsRunning());
  sw.Stop();

  // We have no portable sleep() and otbThreads is not linked here
  sw.Start();
  std::this_thread::sleep_for(500ms);
  sw.Stop();
  assert(sw.GetElapsedMilliseconds() > 450 && sw.GetElapsedMilliseconds() < 550);

  sw.Start();
  std::this_thread::sleep_for(500ms);
  sw.Stop();
  assert(sw.GetElapsedMilliseconds() > 900 && sw.GetElapsedMilliseconds() < 1100);

  sw.Restart();
  std::this_thread::sleep_for(500ms);
  sw.Stop();
  assert(sw.GetElapsedMilliseconds() > 450 && sw.GetElapsedMilliseconds() < 550);

  return EXIT_SUCCESS;
}
