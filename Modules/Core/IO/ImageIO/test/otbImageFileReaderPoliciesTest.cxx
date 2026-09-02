/*
 * Copyright (C) 2005-2026 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbImage.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbTestLogInspector.h"

#include <regex>
#include <string>
#include <sstream>

#include <iostream>
#include <format>

#define EXPECT(cond, fmt, ...)                  \
  if (! (cond))                                 \
  {                                             \
    std::cerr << std::format(fmt, __VA_ARGS__); \
    return EXIT_FAILURE;                        \
  }

int otbImageFileReaderPoliciesTest(int itkNotUsed(argc), char* argv[])
{

  // Verify the number of parameters in the command line
  char* const inputFilename  = argv[1];
  char* const outputFilename = argv[2];
  auto  const buffer_height  = std::stoul(argv[3]);

  using InputPixelType  = unsigned char;
  using OutputPixelType = unsigned char;
  constexpr unsigned int    Dimension = 2;

  using InputImageType  = otb::Image<InputPixelType, Dimension>;
  using OutputImageType = otb::Image<OutputPixelType, Dimension>;

  using ReaderType      = otb::ImageFileReader<InputImageType>;
  using WriterType      = otb::ImageFileWriter<OutputImageType>;

  ReaderType::Pointer reader = ReaderType::New(buffer_height);
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  writer->SetInput(reader->GetOutput());
  auto const logs = otb::WatchLogs::Watch(
      [&]() { writer->Update(); }
  );
  auto const input_size        = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
  auto const size_y            = input_size[1];
  auto const nb_expected_loads = (size_y+buffer_height-1) / buffer_height;
  std::cout << "Input dims: " << input_size << "\n";

  std::regex const re_line{R"(.*otbImageFileReader.hxx, line \d+, ALLOCATE temp buffer: (\d+)MB.*, size=\d+x(\d+) @\(\d+, (\d+)\))"};
  std::istringstream iss(logs);

  unsigned long expected_y_idx = 0;
  unsigned long expected_y_idx_next = -1;
  unsigned long stream_iter    = 0;

  std::cout << "Matching lines:\n";
  for (std::string line
       ; std::getline(iss, line)
       ;)
  {
    if (std::smatch sub; std::regex_search(line, sub, re_line))
    {
      expected_y_idx_next = std::min(expected_y_idx + buffer_height, size_y);

      // std::cout << "-> " << line << "\n";
      std::cout << "  --> " << sub[1] << "\t  Δ" << sub[2] << "  @" << sub[3] << "\n";
      auto const expected_stream_height = expected_y_idx_next - expected_y_idx;
      std::cout << "   VS \t  Δ" << expected_stream_height << "  @" << expected_y_idx << "\n";

      EXPECT(expected_y_idx == std::stoul(sub[3]),
             "{}th stream index expected to be {}, but {} has been found in logs\n>>> {}",
             stream_iter+1, expected_y_idx, sub[3].str(), sub[0].str()
      );

      EXPECT(expected_stream_height == std::stoul(sub[2]),
             "{}th stream height expected to be {}, but {} has been found in logs\n>>> {}",
             stream_iter+1, expected_stream_height, sub[2].str(), sub[0].str()
      );

      expected_y_idx = expected_y_idx_next;
      ++stream_iter;
    }
  }
  EXPECT(nb_expected_loads == stream_iter,
         "{} streams were expected, but {} were actually found. Special case: image size ({}) is a multiple of stream height ({})",
         nb_expected_loads, stream_iter, size_y, buffer_height
  );

  return EXIT_SUCCESS;
}

