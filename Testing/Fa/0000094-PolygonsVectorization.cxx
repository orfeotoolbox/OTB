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

#include "itkMacro.h"
#include "otbCommandLineArgumentParser.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "itkImageRegion.h"

#include "otbPolygon.h"
#include "otbObjectList.h"
#include "otbPersistentVectorizationImageFilter.h"
#include "itkRelabelComponentImageFilter.h"
#include "itkConnectedComponentImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

/*
#include "itkBinaryErodeImageFilter.h"
#include "itkBinaryDilateImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
//#include "itkBinaryMorphologicalClosingImageFilter.h"
//#include "itkBinaryMorphologicalOpeningImageFilter.h"
#include "itkMedianImageFilter.h"
#include "otbRemoveObjectPreprocessingImageFilter.h"
*/

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImageFileWriter.h"

#include "otbStandardWriterWatcher.h"
#include "otbStandardFilterWatcher.h"

int main(int argc, char * argv[])
{

  typedef unsigned long LabelPixelType;
  typedef unsigned char PixelTypeOutput;

  typedef otb::Image<LabelPixelType, 2>  LabeledImageType;
  typedef otb::Image<PixelTypeOutput, 2> OutputImageType;
  typedef itk::ImageRegionIterator<LabeledImageType> IteratorType;
  typedef otb::Polygon<double>             PolygonType;
  typedef otb::ObjectList<PolygonType>     PolygonListType;
  typedef PolygonListType::Pointer         PolygonListPointerType;
  typedef itk::ImageRegion<2>              ImageRegionType;

  typedef otb::PersistentVectorizationImageFilter<LabeledImageType, PolygonType> PersistentVectorizationFilterType;

  typedef itk::ConnectedComponentImageFilter<LabeledImageType, LabeledImageType> ConnectedFilterType;
  typedef itk::RescaleIntensityImageFilter<LabeledImageType, OutputImageType>    RescalerType;
/*
  typedef itk::BinaryBallStructuringElement< LabelPixelType, 2 >        StructuringElementType;
  typedef itk::BinaryErodeImageFilter<LabeledImageType, LabeledImageType, StructuringElementType>   ErodeFilterType;
  typedef itk::BinaryDilateImageFilter<LabeledImageType, LabeledImageType, StructuringElementType>  DilateFilterType;
  typedef itk::MedianImageFilter<LabeledImageType, LabeledImageType> MedianFilterType;
  typedef otb::RemoveObjectPreprocessingImageFilter<LabeledImageType>           PreprocessingFilterType;
*/
  typedef otb::ImageFileReader<LabeledImageType> ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>  WriterType;

  //-----------------------------------------------------------------
  //Command Line Argument Parser
  try
    {
    typedef otb::CommandLineArgumentParser ParserType;
    ParserType::Pointer parser = ParserType::New();

    parser->SetProgramDescription("This program remove small objects");
    parser->AddInputImage();
    parser->AddOutputImage();
    parser->AddOption("--surface", "objects surface limit. Default is 100", "-s", 1, false);
    parser->AddOption("--outputText", "output text file name", "-ot", 1, false);

    typedef otb::CommandLineArgumentParseResult ParserResultType;
    ParserResultType::Pointer parseResult = ParserResultType::New();

    try
      {
      parser->ParseCommandLine(argc, argv, parseResult);
      }
    catch (itk::ExceptionObject& err)
      {
      std::string descriptionException = err.GetDescription();
      if (descriptionException.find("ParseCommandLine(): Help Parser")
          != std::string::npos)
        {
        return EXIT_SUCCESS;
        }
      if (descriptionException.find("ParseCommandLine(): Version Parser")
          != std::string::npos)
        {
        return EXIT_SUCCESS;
        }
      return EXIT_FAILURE;
      }

    //object under this value will be removed
    double surfaceLimit;
    if (parseResult->IsOptionPresent("--surface")) surfaceLimit = parseResult->GetParameterDouble("--surface");
    else surfaceLimit = 100;

    //-----------------------------------------------------------------
    //read image
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName(parseResult->GetInputImage().c_str());
    reader->UpdateOutputInformation();

    ///////////////////////////////////////////////////////////////////
    // Object Processing
    ///////////////////////////////////////////////////////////////////

    //-----------------------------------------------------------------
    //Label the objects in a binary image
    ConnectedFilterType::Pointer connectedFilter = ConnectedFilterType::New();
    connectedFilter->SetInput(reader->GetOutput());

    //-----------------------------------------------------------------
    //Perform vectorization in a persistent way
    PersistentVectorizationFilterType::Pointer persistentVectorization = PersistentVectorizationFilterType::New();
    persistentVectorization->Reset();
    persistentVectorization->SetInput(connectedFilter->GetOutput());
    try
      {
      persistentVectorization->Update();
      }
    catch (itk::ExceptionObject& err)
      {
      std::cout << "\nExceptionObject caught !" << std::endl;
      std::cout << err << std::endl;
      return EXIT_FAILURE;
      }

    PolygonListPointerType OutputPolyList = persistentVectorization->GetPathList();

    //Display results
    std::cout << "nb objects found = " << OutputPolyList->Size() << std::endl;

    //-------------------
    //DEBUG
    //-------------------
    unsigned int  polygon = 0;
    std::ofstream file;
    if (parseResult->IsOptionPresent("--outputText"))
      {
      file.open(parseResult->GetParameterString("--outputText").c_str());
      }

    double minSize = -1;
    //Initializing the minSize
    if (OutputPolyList->Size() > 0)
      {
      minSize = OutputPolyList->GetNthElement(0)->GetArea();
      }

    while (polygon < OutputPolyList->Size())
      {
      ImageRegionType polygonRegion = OutputPolyList->GetNthElement(polygon)->GetBoundingRegion().GetImageRegion();
      if (OutputPolyList->GetNthElement(polygon)->GetArea() < minSize)
        {
        minSize = OutputPolyList->GetNthElement(polygon)->GetArea();
        }
      if (parseResult->IsOptionPresent("--outputText"))
        {
        file << "polygon " << polygon << "\tnPoints="
             << OutputPolyList->GetNthElement(polygon)->GetVertexList()->Size() << "\tsurface="
             << OutputPolyList->GetNthElement(polygon)->GetArea() << "\tlength="
             << OutputPolyList->GetNthElement(polygon)->GetLength() << "\tregion size="
             << polygonRegion.GetSize() << "\tregion nb pixel="
             << polygonRegion.GetNumberOfPixels() << std::endl;
//       file << OutputPolyList->GetNthElement(polygon)<< std::endl << std::endl;
        }
      else
        {
        std::cout << "polygon " << polygon << "\tnPoints="
                  << OutputPolyList->GetNthElement(polygon)->GetVertexList()->Size() << "\tsurface="
                  << OutputPolyList->GetNthElement(polygon)->GetArea() << "\tlength="
                  << OutputPolyList->GetNthElement(polygon)->GetLength() << "\tregion size="
                  << polygonRegion.GetSize() << "\tregion nb pixel="
                  << polygonRegion.GetNumberOfPixels() << std::endl;
//       std::cout << OutputPolyList->GetNthElement(polygon)<< std::endl << std::endl;
        }
      polygon++;
      }
    if (parseResult->IsOptionPresent("--outputText"))
      {
      file.close();
      }
    //-------------------
    // END DEBUG
    //-------------------

    //-----------------------------------------------------------------
    //erase object
    unsigned int i = 0;
    std::cout << "erase ..." << std::endl;
    while (i < OutputPolyList->Size())
      {
      if ((OutputPolyList->GetNthElement(i)->GetArea() > surfaceLimit)   //delete big polygon
          || (OutputPolyList->GetNthElement(i)->GetArea() == 0))         //delete invalid polygon
        OutputPolyList->Erase(i);
      else ++i;
      }

    //Display results after erasure
    std::cout << "nb objects found  = " << OutputPolyList->Size() << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    ///////////////////////////////////////////////////////////////////
    // Output Image Writing
    ///////////////////////////////////////////////////////////////////

    //-----------------------------------------------------------------
    //allocate the memory for the output file
    LabeledImageType::Pointer outputImage = LabeledImageType::New();
    outputImage->SetRegions(reader->GetOutput()->GetRequestedRegion());
    outputImage->CopyInformation(reader->GetOutput());
    outputImage->Allocate();
    // copy input
    IteratorType iit(reader->GetOutput(), reader->GetOutput()->GetRequestedRegion());
    IteratorType oit(outputImage, outputImage->GetRequestedRegion());

    for (iit.GoToBegin(), oit.GoToBegin(); !iit.IsAtEnd(); ++iit, ++oit)
      oit.Set(iit.Get());

    // erase small polygon
    for (unsigned int i = 0; i < OutputPolyList->Size(); ++i)
      {
      std::cout << "polygon " << i << std::endl;
      ImageRegionType polygonRegion = OutputPolyList->GetNthElement(i)->GetBoundingRegion().GetImageRegion();

      IteratorType outputIt(outputImage, polygonRegion);

      outputIt.GoToBegin();
      while (!outputIt.IsAtEnd())
        {
        outputIt.Set(0);
        ++outputIt;
        }

      }

    //-----------------------------------------------------------------
    //rescale image
    RescalerType::Pointer rescaler = RescalerType::New();
    rescaler->SetOutputMinimum(0);
    rescaler->SetOutputMaximum(255);
    rescaler->SetInput(outputImage);
    rescaler->Update();

    //-----------------------------------------------------------------
    //write image
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(parseResult->GetOutputImage().c_str());
    writer->SetInput(rescaler->GetOutput());

    try
      {
      otb::StandardWriterWatcher watcher(writer, "Remove small object");
      writer->Update();
      }
    catch (itk::ExceptionObject& err)
      {
      std::cout << "ExceptionObject caught !" << std::endl;
      std::cout << err << std::endl;
      return EXIT_FAILURE;
      }

    if (minSize == 0)
      {
      return EXIT_FAILURE;
      }

    } //end block try global
  catch (itk::ExceptionObject& err)
    {
    std::cout << "Following otbException catch :" << std::endl;
    std::cout << err << std::endl;
    return EXIT_FAILURE;
    }
  catch (std::bad_alloc& err)
    {
    std::cout << "Exception bad_alloc : " << (char*) err.what() << std::endl;
    return EXIT_FAILURE;
    }
  catch (...)
    {
    std::cout << "Unknown Exception found !" << std::endl;
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}
