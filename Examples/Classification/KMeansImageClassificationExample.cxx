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

// Software Guide : BeginLatex
//
// The K-Means classification proposed by ITK for images is limited to
// scalar images and is not streamed. In this example, we show
// how the use of the \doxygen{otb}{KMeansImageClassificationFilter}
// allows for a simple implementation of a K-Means classification
// application. We will start by including the appropirate header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbKMeansImageClassificationFilter.h"
// Software Guide : EndCodeSnippet
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int main(int itkNotUsed(argc), char * argv[])
{
  const char *       infname = argv[1];
  const char *       outfname = argv[2];
  const unsigned int nbClasses = atoi(argv[3]);

// Software Guide : BeginLatex
//
// We will assume double precision input images and will also define
// the type for the labeled pixels.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  const unsigned int Dimension = 2;
  typedef double         PixelType;
  typedef unsigned short LabeledPixelType;
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// Our classifier will be genric enough to be able to process images
// with any number of bands. We read the images as
// \doxygen{otb}{VectorImage}s. The labeled image will be a scalar image.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::VectorImage<PixelType, Dimension>  ImageType;
  typedef otb::Image<LabeledPixelType, Dimension> LabeledImageType;
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We can now define the type for the classifier filter, which is
// templated over its input and output image types.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::KMeansImageClassificationFilter<ImageType, LabeledImageType>
  ClassificationFilterType;
  typedef ClassificationFilterType::KMeansParametersType KMeansParametersType;
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// And finally, we define the reader and the writer. Since the images
// to classify can be very big, we will use a streamed writer which
// will trigger the streaming ability of the classifier.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<ImageType>                 ReaderType;
  typedef otb::ImageFileWriter<LabeledImageType> WriterType;
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We instantiate the classifier and the reader objects and we set
// their parameters. Please note the call of the
// \code{GenerateOutputInformation()} method on the reader in order to
// have available the information about the input image (size, number
// of bands, etc.) without needing to actually read the image.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  ClassificationFilterType::Pointer filter = ClassificationFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->GenerateOutputInformation();
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// The classifier needs as input the centroids of
// the classes. We declare the parameter vector, and we read the
// centroids from the arguments of the program.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  const unsigned int sampleSize =
    ClassificationFilterType::MaxSampleDimension;
  const unsigned int   parameterSize = nbClasses * sampleSize;
  KMeansParametersType parameters;

  parameters.SetSize(parameterSize);
  parameters.Fill(0);

  for (unsigned int i = 0; i < nbClasses; ++i)
    {
    for (unsigned int j = 0; j <
         reader->GetOutput()->GetNumberOfComponentsPerPixel(); ++j)
      {
      parameters[i * sampleSize + j] =
        atof(argv[4 + i *
                  reader->GetOutput()->GetNumberOfComponentsPerPixel()
                  + j]);
      }
    }

  std::cout << "Parameters: " << parameters << std::endl;
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We set the parameters for the classifier, we plug the pipeline and
// trigger its execution by updating the output of the writer.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  filter->SetCentroids(parameters);
  filter->SetInput(reader->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();
// Software Guide : EndCodeSnippet
  return EXIT_SUCCESS;
}
