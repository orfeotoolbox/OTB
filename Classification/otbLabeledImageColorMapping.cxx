/*=========================================================================

 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/
#include "otbLabeledImageColorMapping.h"

#include <fstream>
#include "otbCommandLineArgumentParser.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbChangeLabelImageFilter.h"
#include "otbStandardWriterWatcher.h"

namespace otb
{

int LabeledImageColorMapping::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("LabeledImageColorMapping");
  descriptor->SetDescription("Replace labels of a classification map with user defined 8-bits RGB colors. Unknown label are mapped to black by default.");
  descriptor->AddInputImage();
  descriptor->AddOutputImage();
  descriptor->AddOption("ColorTable",
                        "An ascii file containing the color table with one color per line (for instance line 1 255 0 0 means that all pixel with label 1 will be replaced by RGB  color 255 0 0). Lines begining with a # are ignored.",
                        "ct", 1, true, ApplicationDescriptor::FileName);
  descriptor->AddOption("AvailableMemory",
                        "Set the maximum of available memory for the pipeline execution in mega bytes (optional, 256 by default)",
                        "ram", 1, false, otb::ApplicationDescriptor::Integer);

  return EXIT_SUCCESS;
}

int LabeledImageColorMapping::Execute(otb::ApplicationOptionsResult* parseResult)
{
  // Pixel type
  typedef unsigned char PixelType;
  typedef unsigned short LabelType;

  typedef otb::VectorImage<PixelType, 2> VectorImageType;
  typedef VectorImageType::PixelType VectorPixelType;
  typedef otb::Image<LabelType, 2> LabelImageType;
  typedef otb::ImageFileReader<LabelImageType> ReaderType;
  typedef otb::StreamingImageFileWriter<VectorImageType> WriterType;
  typedef otb::ChangeLabelImageFilter<LabelImageType, VectorImageType> ChangeLabelFilterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(parseResult->GetInputImage().c_str());

  ChangeLabelFilterType::Pointer mapper = ChangeLabelFilterType::New();
  mapper->SetInput(reader->GetOutput());
  mapper->SetNumberOfComponentsPerPixel(3);

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(mapper->GetOutput());
  writer->SetFileName(parseResult->GetOutputImage());

  std::ifstream ifs;

  const char * colormap = parseResult->GetParameterString("ColorTable").c_str();

  ifs.open(colormap);

  if (!ifs)
    {
    std::cerr << "Can not read file " << colormap << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "Parsing color map file " << colormap << "." << std::endl;

  while (!ifs.eof())
    {
    std::string line;
    std::getline(ifs, line);

    // Avoid commented lines or too short ones
    if (!line.empty() && line[0] != '#')
      {
      // retrieve the label
      std::string::size_type pos = line.find_first_of(" ", 0);
      LabelType clabel = atoi(line.substr(0, pos).c_str());
      ++pos;
      // Retrieve the color
      VectorPixelType color(3);
      color.Fill(0);
      for (unsigned int i = 0; i < 3; ++i)
        {
        std::string::size_type nextpos = line.find_first_of(" ", pos);
        int value = atoi(line.substr(pos, nextpos).c_str());
        if (value < 0 || value > 255) std::cerr
            << "WARNING: color value outside 8-bits range (<0 or >255). Value will be clamped." << std::endl;
        color[i] = static_cast<PixelType> (value);
        pos = nextpos + 1;
        nextpos = line.find_first_of(" ", pos);
        }
      std::cout << "Adding color mapping " << clabel << " -> [" << (int) color[0] << " " << (int) color[1] << " "
          << (int) color[2] << " ]" << std::endl;
      mapper->SetChange(clabel, color);
      }
    }
  ifs.close();

  unsigned int ram = 256;
  if (parseResult->IsOptionPresent("AvailableMemory"))
    {
    ram = parseResult->GetParameterUInt("AvailableMemory");
    }
  writer->SetAutomaticTiledStreaming(ram);

  otb::StandardWriterWatcher watcher(writer, mapper, "Color mapping");

  writer->Update();

  return EXIT_SUCCESS;
}

}
