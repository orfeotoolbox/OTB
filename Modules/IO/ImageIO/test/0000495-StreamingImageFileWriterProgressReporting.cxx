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
#include "otbImage.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkCastImageFilter.h"

/* See #495 in the bug tracker for details */
int main(int argc, char* argv[])
{
  if (argc != 4)
    {
    std::cout << argv[0] << " <input image> <output image 1> <output image 2>" << std::endl;
    return EXIT_FAILURE;
    }

  typedef otb::Image<float, 2>                        ImageType;
  typedef otb::ImageFileReader<ImageType>             ReaderType;
  typedef otb::ImageFileWriter<ImageType>    WriterType;
  typedef itk::CastImageFilter<ImageType, ImageType>  CastFilterType;

  bool hasObserver = false;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  CastFilterType::Pointer cast = CastFilterType::New();
  cast->SetInput(reader->GetOutput());

  hasObserver = cast->HasObserver( itk::ProgressEvent() );

  std::cout << "Before Scope : hasObserver = " << (hasObserver ? "true" : "false") << std::endl;
  itkAssertOrThrowMacro( hasObserver == false, "No observer should be registered at this point" );

  {
    WriterType::Pointer scopedWriter = WriterType::New();
    scopedWriter->SetInput(cast->GetOutput());
    scopedWriter->SetFileName(argv[2]);

    // Ensure that streaming is enabled to have progress reporting
    scopedWriter->SetNumberOfLinesStrippedStreaming(10);

    hasObserver = cast->HasObserver( itk::ProgressEvent() );
    std::cout << "Before Update : hasObserver = " << (hasObserver ? "true" : "false") << std::endl;
    itkAssertOrThrowMacro( hasObserver == false, "No observer should be registered at this point" );

    // Write the output : this connect the writer to its source filter
    // for progress reporting
    scopedWriter->Update();

    hasObserver = cast->HasObserver( itk::ProgressEvent() );
    std::cout << "After Update : hasObserver = " << (hasObserver ? "true" : "false") << std::endl;
    itkAssertOrThrowMacro( hasObserver == false, "The previously registered observer should have been unregistered at this point" );

    // Get out of scope : this deletes the scopedWriter object
  }

  hasObserver = cast->HasObserver( itk::ProgressEvent() );
  std::cout << "After Scope : hasObserver = " << (hasObserver ? "true" : "false") << std::endl;
  itkAssertOrThrowMacro( hasObserver == false, "The previously registered observer should have been unregistered at this point" );

  // Make a new writer, just to exercise streaming in the reader
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(cast->GetOutput());
  writer->SetFileName(argv[3]);
  writer->SetNumberOfLinesStrippedStreaming(10);

  // Execute the pipeline
  // The reader object
  writer->Update();

  return EXIT_SUCCESS;
}
