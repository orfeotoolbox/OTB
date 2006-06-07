/*=========================================================================


=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif

//  Software Guide : BeginLatex
//
//  As we have seen, the reading of images is managed by the class \doxygen{otb::ImageFileReader}
//  while writing is performed by the class
//  \doxygen{otb::ImageFileWriter}. ITK's pipeline implements
//  streaming. That means that a filter for which the
//  \code{ThreadedGenerateData} method is implemented, will only produce the
//  data for the region requested by the following filter in the
//  pipeline. Therefore, in order to use the streaming functionnality
//  one needs to use a filter at the end of the pipeline which
//  requests for adjacent regions of the image to be processed. In
//  ITK, the \doxygen{itk::StreamingImageFilter} class is used for
//  this purpose. However, ITK does not implement streaming from/to
//  files. This means that even if the pipeline has a small memory
//  footprint, the images have to be stored in memory at least after
//  the read operation and before the write operation.
//
//  OTB implements read/write streaming. For the image file reading,
//  this is transparent for the programmer, and if a streaming loop is
//  used at the end of the pipeline, the read operation will be
//  streamed. For the file writing, the
//  \doxygen{otb::StreamingImageFileWriter} has to be used.
//
//  The first step for performing streamed reading and writing is to include the
//  following headers.
//
//  \index{otb::ImageFileReader|textbf}
//  \index{otb::ImageFileReader!header}
//
//  \index{otb::StreamingImageFileWriter|textbf}
//  \index{otb::StreamingImageFileWriter!header}
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
// Software Guide : EndCodeSnippet


#include "otbImage.h"


int main( int argc, char ** argv )
{
  // Verify the number of parameters in the command line
  if( argc < 3 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile  outputImageFile " << std::endl;
    return EXIT_FAILURE;
    }


  //  Software Guide : BeginLatex
  //
  //  Then, as usual, a decision must be made about the type of pixel used to
  //  represent the image processed by the pipeline.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef unsigned short      PixelType;
  const   unsigned int        Dimension = 2;
  typedef otb::Image< PixelType, Dimension >    ImageType;
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  We can now instantiate the types of the reader and writer. These two
  //  classes are parameterized over the image type.
  //
  //  \index{otb::StreamingImageFileWriter!Instantiation}
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader< ImageType >  ReaderType;
  typedef otb::StreamingImageFileWriter< ImageType >  WriterType;
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  Then, we create one object of each type using the New() method and
  //  assigning the result to a \doxygen{SmartPointer}.
  //
  //  \index{otb::ImageFileReader!New()}
  //  \index{otb::ImageFileWriter!New()}
  //  \index{otb::StreamingImageFileReader!SmartPointer}
  //  \index{otb::ImageFileWriter!SmartPointer}
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  // Software Guide : EndCodeSnippet


  // Here we recover the file names from the command line arguments
  //
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];


  //  Software Guide : BeginLatex
  //
  //  The name of the file to be read or written is passed with the
  //  SetFileName() method. 
  //
  //  \index{otb::ImageFileReader!SetFileName()}
  //  \index{otb::StreamingImageFileWriter!SetFileName()}
  //  \index{SetFileName()!otb::ImageFileReader}
  //  \index{SetFileName()!otb::StreamingImageFileWriter}
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  reader->SetFileName( inputFilename  );
  writer->SetFileName( outputFilename );
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  We can now connect these readers and writers to filters to create a
  //  pipeline. For example, we can create a short pipeline by passing
  //  the output of the reader directly to the input of the writer.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  writer->SetInput( reader->GetOutput() );
  // Software Guide : EndCodeSnippet



  try 
    { 
    writer->Update(); 
    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cerr << "ExceptionObject caught !" << std::endl; 
    std::cerr << err << std::endl; 
    return EXIT_FAILURE;
    } 

  return EXIT_SUCCESS;
}



