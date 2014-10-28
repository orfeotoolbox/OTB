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


#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiToMonoChannelExtractROI.h"

template <typename  InputPixelType, typename OutputPixelType>
int generic_otbMultiToMonoChannelExtractROI(int itkNotUsed(argc),
                                            char * argv[],
                                            const char * inputFilename,
                                            const char * outputFilename)
{
  typedef otb::MultiToMonoChannelExtractROI<InputPixelType,
      OutputPixelType>  ExtractROIFilterType;

  typename ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();
  int cpt(0);

  while (argv[cpt] != NULL)
    {
    std::string strArgv(argv[cpt]);
    if (strArgv == "-startX")
      {
      extractROIFilter->SetStartX((unsigned long) ::atoi(argv[cpt + 1]));
      std::cout << " ->SetStartX(" << ::atoi(argv[cpt + 1]) << ")" << std::endl;
      cpt += 2;
      }
    else if (strArgv == "-startY")
      {
      extractROIFilter->SetStartY((unsigned long) ::atoi(argv[cpt + 1]));
      std::cout << " ->SetStartY(" << ::atoi(argv[cpt + 1]) << ")" << std::endl;
      cpt += 2;
      }
    else if (strArgv == "-sizeX")
      {
      extractROIFilter->SetSizeX((unsigned long) ::atoi(argv[cpt + 1]));
      std::cout << " ->SetSizeX(" << ::atoi(argv[cpt + 1]) << ")" << std::endl;
      cpt += 2;
      }
    else if (strArgv == "-sizeY")
      {
      extractROIFilter->SetSizeY((unsigned long) ::atoi(argv[cpt + 1]));
      std::cout << " ->SetSizeY(" << ::atoi(argv[cpt + 1]) << ")" << std::endl;
      cpt += 2;
      }
    else if (strArgv == "-channel")
      {
      extractROIFilter->SetChannel((unsigned int) ::atoi(argv[cpt + 1]));
      std::cout << " ->SetChannel(" << ::atoi(argv[cpt + 1]) << ")" << std::endl;
      cpt += 2;
      }
    }

  // Resume de la ligne de commande
  std::cout << " ROI selectionnee : startX " << extractROIFilter->GetStartX() << std::endl;
  std::cout << "                    startY " << extractROIFilter->GetStartY() << std::endl;
  std::cout << "                    sizeX  " << extractROIFilter->GetSizeX() << std::endl;
  std::cout << "                    sizeY  " << extractROIFilter->GetSizeY() << std::endl;
  std::cout << " Canal selectionne : (" << extractROIFilter->GetChannel() << ") : ";

  // OTB-FA-00008-CS
  //        typedef otb::ImageFileReader< typename ExtractROIFilterType::InputImageType, itk::DefaultConvertPixelTraits< InputPixelType >  >       ReaderType;
  typedef otb::ImageFileReader<typename ExtractROIFilterType::InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<typename ExtractROIFilterType::OutputImageType> WriterType;
  typename ReaderType::Pointer reader = ReaderType::New();
  typename WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(inputFilename);
  //THOMAS
  //        reader->Update(); //Necessaire pour connaitre le nombre de canaux dans l'image
  writer->SetFileName(outputFilename);
  extractROIFilter->SetInput(reader->GetOutput());

  writer->SetInput(extractROIFilter->GetOutput());
  writer->Update();
  std::cout << " Nb canaux dans l'image d'entree : " << reader->GetOutput()->GetNumberOfComponentsPerPixel() <<
  std::endl;
  std::cout << " Nb canaux dans l'image de sortie : " <<
  extractROIFilter->GetOutput()->GetNumberOfComponentsPerPixel() << std::endl;

  return EXIT_SUCCESS;
}

int otbMultiToMonoChannelExtractROI(int argc, char * argv[])
{
  std::string  linputPixelType;
  std::string  loutputPixelType;
  const char * inputFilename;
  const char * outputFilename;
  int          cpt(1);
  if (argv[cpt][0] == '-')
    {
    linputPixelType = std::string(argv[cpt]);
    ++cpt;
    inputFilename  = argv[cpt];
    ++cpt;
    }
  else
    {
    linputPixelType = std::string("-uchar");
    inputFilename  = argv[cpt];
    ++cpt;
    }
  if (argv[cpt][0] == '-')
    {
    loutputPixelType = std::string(argv[cpt]);
    ++cpt;
    outputFilename  = argv[cpt];
    ++cpt;
    }
  else
    {
    loutputPixelType = std::string("-uchar");
    outputFilename  = argv[cpt];
    ++cpt;
    }

  argc -= cpt;
  argv += cpt;
  std::cout << " -> " << linputPixelType << " pour " << inputFilename << std::endl;
  std::cout << " -> " << loutputPixelType << " pour " << outputFilename << std::endl;
  std::string pixelType;
  if ((linputPixelType == "-uchar") &&
      (loutputPixelType ==
       "-uchar"))
    return (generic_otbMultiToMonoChannelExtractROI<unsigned char, unsigned char>(argc, argv,
                                                                                  inputFilename,
                                                                                  outputFilename));
  else if ((linputPixelType == "-short") &&
           (loutputPixelType ==
            "-short"))
    return (generic_otbMultiToMonoChannelExtractROI<short, short>(argc, argv, inputFilename,
                                                                  outputFilename));
  else if ((linputPixelType == "-ushort") &&
           (loutputPixelType ==
            "-ushort"))
    return (generic_otbMultiToMonoChannelExtractROI<unsigned short, unsigned short>(argc, argv,
                                                                                    inputFilename,
                                                                                    outputFilename));
  else if ((linputPixelType == "-int") &&
           (loutputPixelType ==
            "-int"))
    return (generic_otbMultiToMonoChannelExtractROI<int, int>(argc, argv, inputFilename,
                                                              outputFilename));
  else if ((linputPixelType == "-uint") &&
           (loutputPixelType ==
            "-uint"))
    return (generic_otbMultiToMonoChannelExtractROI<unsigned int, unsigned int>(argc, argv,
                                                                                inputFilename,
                                                                                outputFilename));
  else if ((linputPixelType == "-float") &&
           (loutputPixelType ==
            "-float"))
    return (generic_otbMultiToMonoChannelExtractROI<float, float>(argc, argv, inputFilename,
                                                                  outputFilename));
  else if ((linputPixelType == "-double") &&
           (loutputPixelType ==
            "-double"))
    return (generic_otbMultiToMonoChannelExtractROI<double, double>(argc, argv, inputFilename,
                                                                    outputFilename));
  // Type -> uchar
  else if ((linputPixelType == "-short") &&
           (loutputPixelType ==
            "-uchar"))
    return (generic_otbMultiToMonoChannelExtractROI<short, unsigned char>(argc, argv, inputFilename,
                                                                          outputFilename));
  else if ((linputPixelType == "-ushort") &&
           (loutputPixelType ==
            "-uchar"))
    return (generic_otbMultiToMonoChannelExtractROI<unsigned short, unsigned char>(argc, argv,
                                                                                   inputFilename,
                                                                                   outputFilename));
  else if ((linputPixelType == "-int") &&
           (loutputPixelType ==
            "-uchar"))
    return (generic_otbMultiToMonoChannelExtractROI<int, unsigned char>(argc, argv, inputFilename,
                                                                        outputFilename));
  else if ((linputPixelType == "-uint") &&
           (loutputPixelType ==
            "-uchar"))
    return (generic_otbMultiToMonoChannelExtractROI<unsigned int, unsigned char>(argc, argv,
                                                                                 inputFilename,
                                                                                 outputFilename));
  else if ((linputPixelType == "-float") &&
           (loutputPixelType ==
            "-uchar"))
    return (generic_otbMultiToMonoChannelExtractROI<float, unsigned char>(argc, argv, inputFilename,
                                                                          outputFilename));
  else if ((linputPixelType == "-double") &&
           (loutputPixelType ==
            "-uchar"))
    return (generic_otbMultiToMonoChannelExtractROI<double, unsigned char>(argc, argv, inputFilename,
                                                                           outputFilename));
  // Type -> ushort
  else if ((linputPixelType == "-uchar") &&
           (loutputPixelType ==
            "-ushort"))
    return (generic_otbMultiToMonoChannelExtractROI<unsigned char, unsigned short>(argc, argv,
                                                                                   inputFilename,
                                                                                   outputFilename));
  else if ((linputPixelType == "-int") &&
           (loutputPixelType ==
            "-ushort"))
    return (generic_otbMultiToMonoChannelExtractROI<int, unsigned short>(argc, argv, inputFilename,
                                                                         outputFilename));
  else if ((linputPixelType == "-uint") &&
           (loutputPixelType ==
            "-ushort"))
    return (generic_otbMultiToMonoChannelExtractROI<unsigned int, unsigned short>(argc, argv,
                                                                                  inputFilename,
                                                                                  outputFilename));
  else if ((linputPixelType == "-float") &&
           (loutputPixelType ==
            "-ushort"))
    return (generic_otbMultiToMonoChannelExtractROI<float, unsigned short>(argc, argv,
                                                                           inputFilename,
                                                                           outputFilename));
  else if ((linputPixelType == "-double") &&
           (loutputPixelType ==
            "-ushort"))
    return (generic_otbMultiToMonoChannelExtractROI<double, unsigned short>(argc, argv,
                                                                            inputFilename,
                                                                            outputFilename));
  // Type -> ushort
  else if ((linputPixelType == "-uchar") &&
           (loutputPixelType ==
            "-short"))
    return (generic_otbMultiToMonoChannelExtractROI<unsigned char, short>(argc, argv, inputFilename,
                                                                          outputFilename));
  else if ((linputPixelType == "-int") &&
           (loutputPixelType ==
            "-short"))
    return (generic_otbMultiToMonoChannelExtractROI<int, short>(argc, argv, inputFilename,
                                                                outputFilename));
  else if ((linputPixelType == "-uint") &&
           (loutputPixelType ==
            "-short"))
    return (generic_otbMultiToMonoChannelExtractROI<unsigned int, short>(argc, argv, inputFilename,
                                                                         outputFilename));
  else if ((linputPixelType == "-float") &&
           (loutputPixelType ==
            "-short"))
    return (generic_otbMultiToMonoChannelExtractROI<float, short>(argc, argv, inputFilename,
                                                                  outputFilename));
  else if ((linputPixelType == "-double") &&
           (loutputPixelType ==
            "-short"))
    return (generic_otbMultiToMonoChannelExtractROI<double, short>(argc, argv, inputFilename,
                                                                   outputFilename));
  // Type -> int
  else if ((linputPixelType == "-uchar") &&
           (loutputPixelType ==
            "-int"))
    return (generic_otbMultiToMonoChannelExtractROI<unsigned char, int>(argc, argv, inputFilename,
                                                                        outputFilename));
  else if ((linputPixelType == "-short") &&
           (loutputPixelType ==
            "-int"))
    return (generic_otbMultiToMonoChannelExtractROI<short, int>(argc, argv, inputFilename,
                                                                outputFilename));
  else if ((linputPixelType == "-ushort") &&
           (loutputPixelType ==
            "-int"))
    return (generic_otbMultiToMonoChannelExtractROI<unsigned short, int>(argc, argv, inputFilename,
                                                                         outputFilename));
  else if ((linputPixelType == "-uint") &&
           (loutputPixelType ==
            "-int"))
    return (generic_otbMultiToMonoChannelExtractROI<unsigned int, int>(argc, argv, inputFilename,
                                                                       outputFilename));
  else if ((linputPixelType == "-float") &&
           (loutputPixelType ==
            "-int"))
    return (generic_otbMultiToMonoChannelExtractROI<float, int>(argc, argv, inputFilename,
                                                                outputFilename));
  else if ((linputPixelType == "-double") &&
           (loutputPixelType ==
            "-int"))
    return (generic_otbMultiToMonoChannelExtractROI<double, int>(argc, argv, inputFilename,
                                                                 outputFilename));
  // Type -> double
  else if ((linputPixelType == "-uchar") &&
           (loutputPixelType ==
            "-double"))
    return (generic_otbMultiToMonoChannelExtractROI<unsigned char, double>(argc, argv,
                                                                           inputFilename,
                                                                           outputFilename));
  else if ((linputPixelType == "-short") &&
           (loutputPixelType ==
            "-double"))
    return (generic_otbMultiToMonoChannelExtractROI<short, double>(argc, argv, inputFilename,
                                                                   outputFilename));
  else if ((linputPixelType == "-ushort") &&
           (loutputPixelType ==
            "-double"))
    return (generic_otbMultiToMonoChannelExtractROI<unsigned short, double>(argc, argv,
                                                                            inputFilename,
                                                                            outputFilename));
  else if ((linputPixelType == "-int") &&
           (loutputPixelType ==
            "-double"))
    return (generic_otbMultiToMonoChannelExtractROI<int, double>(argc, argv, inputFilename,
                                                                 outputFilename));
  else if ((linputPixelType == "-uint") &&
           (loutputPixelType ==
            "-double"))
    return (generic_otbMultiToMonoChannelExtractROI<unsigned int, double>(argc, argv, inputFilename,
                                                                          outputFilename));
  else if ((linputPixelType == "-float") &&
           (loutputPixelType ==
            "-double"))
    return (generic_otbMultiToMonoChannelExtractROI<float, double>(argc, argv, inputFilename,
                                                                   outputFilename));

  else
    {
    std::cout << " Error : wrong format" << std::endl;
    std::cout << "          allowed values : -uchar, -ushort, -uint, -float, -double" << std::endl;
    std::cout << "          default value : -uchar" << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_FAILURE;

}
