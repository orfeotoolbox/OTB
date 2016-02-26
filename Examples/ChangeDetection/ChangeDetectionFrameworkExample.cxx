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
// This example illustrates the Change Detector framework implemented
// in OTB. This framework uses the generic programming approach. All
// change detection filters are
// \doxygen{otb}{BinaryFunctorNeighborhoodImageFilter}s, that is, they
// are filters taking two images as input and providing one image as
// output. The change detection computation itself is performed on a
// the neighborhood of each pixel of the input images.
//
// The first step required to build a change detection filter is to
// include the header of the parent class.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbBinaryFunctorNeighborhoodImageFilter.h"
// Software Guide : EndCodeSnippet

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbImage.h"
#include "otbCommandProgressUpdate.h"

// Software Guide : BeginLatex
//
// The change
// detection operation itself is one of the templates of the change
// detection filters and takes the form of a function, that is,
// something accepting the syntax \code{foo()}. This can be
// implemented using classical C/C++ functions, but it is preferable
// to implement it using C++ functors. These are classical C++ classes
// which overload the \code{()} operator. This allows using them with
// the same syntax as C/C++ functions.
//
// Since change detectors operate on neighborhoods, the functor
// call will take 2 arguments which are
// \doxygen{itk}{ConstNeighborhoodIterator}s.
//
// The change detector functor is templated over the types of the
// input iterators and the output result type. The core of the change
// detection is implemented in the \code{operator()} section.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
template<class TInput1, class TInput2, class TOutput>
class MyChangeDetector
{
public:
  // The constructor and destructor.
  MyChangeDetector() {}
  ~MyChangeDetector() {}
  // Change detection operation
  inline TOutput operator ()(const TInput1& itA,
                             const TInput2& itB)
  {

    TOutput result = 0.0;

    for (unsigned long pos = 0; pos < itA.Size(); ++pos)
      {

      result += static_cast<TOutput>(itA.GetPixel(pos) - itB.GetPixel(pos));

      }
    return static_cast<TOutput>(result / itA.Size());
  }
};
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// The interest of using functors is that complex operations can be
// performed using internal \code{protected} class methods and that
// class variables can be used to store information so different pixel
// locations can access to results of previous computations.
//
// The next step is the definition of the change detector filter. As
// stated above, this filter will inherit from
// \doxygen{otb}{BinaryFunctorNeighborhoodImageFilter} which is
// templated over the 2 input image types, the output image type and
// the functor used to perform the change detection operation.
//
// Inside the class only a few \code{typedef}s and the constructors
// and destructors have to be declared.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
template <class TInputImage1, class TInputImage2, class TOutputImage>
class ITK_EXPORT MyChangeDetectorImageFilter :
  public otb::BinaryFunctorNeighborhoodImageFilter<
      TInputImage1, TInputImage2, TOutputImage,
      MyChangeDetector<
          typename itk::ConstNeighborhoodIterator<TInputImage1>,
          typename itk::ConstNeighborhoodIterator<TInputImage2>,
          typename TOutputImage::PixelType> >
{
public:
  /** Standard class typedefs. */
  typedef MyChangeDetectorImageFilter Self;

  typedef typename otb::BinaryFunctorNeighborhoodImageFilter<
      TInputImage1, TInputImage2, TOutputImage,
      MyChangeDetector<
          typename itk::ConstNeighborhoodIterator<TInputImage1>,
          typename itk::ConstNeighborhoodIterator<TInputImage2>,
          typename TOutputImage::PixelType>
      >  Superclass;

  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

protected:
  MyChangeDetectorImageFilter() {}
  virtual ~MyChangeDetectorImageFilter() {}

private:
  MyChangeDetectorImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// Pay attention to the fact that no \code{.txx} file is needed, since
// filtering operation is implemented in the
// \doxygen{otb}{BinaryFunctorNeighborhoodImageFilter} class. So all
// the algorithmics part is inside the functor.
//
// We can now write a program using the change detector.
//
// SoftwareGuide : EndLatex

int main(int argc, char* argv[])
{

  if (argc < 5)
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] <<
    " inputImageFile1 inputImageFile2  radius outputImageFile " << std::endl;
    return -1;
    }

  // Define the dimension of the images
  const unsigned int Dimension = 2;

  // Software Guide : BeginLatex
  //
  // As usual, we start by defining the image types. The internal
  // computations will be performed with floating point precision,
  // while the output image will be stored using one byte per pixel.
  //
  // SoftwareGuide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef float                                    InternalPixelType;
  typedef unsigned char                            OutputPixelType;
  typedef otb::Image<InternalPixelType, Dimension> InputImageType1;
  typedef otb::Image<InternalPixelType, Dimension> InputImageType2;
  typedef otb::Image<InternalPixelType, Dimension> ChangeImageType;
  typedef otb::Image<OutputPixelType, Dimension>   OutputImageType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We declare the readers, the writer, but also the
  // \doxygen{itk}{RescaleIntensityImageFilter} which will be used to
  // rescale the result before writing it to a file.
  //
  // SoftwareGuide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<InputImageType1> ReaderType1;
  typedef otb::ImageFileReader<InputImageType2> ReaderType2;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  typedef itk::RescaleIntensityImageFilter<ChangeImageType,
      OutputImageType> RescalerType;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // The next step is declaring the filter for the change detection.
  //
  // SoftwareGuide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef MyChangeDetectorImageFilter<InputImageType1, InputImageType2,
      ChangeImageType>      FilterType;
  // Software Guide : EndCodeSnippet

  ReaderType1::Pointer  reader1 = ReaderType1::New();
  ReaderType2::Pointer  reader2 = ReaderType2::New();
  WriterType::Pointer   writer = WriterType::New();
  FilterType::Pointer   filter = FilterType::New();
  RescalerType::Pointer rescaler = RescalerType::New();

  const char * inputFilename1  = argv[1];
  const char * inputFilename2  = argv[2];
  const char * outputFilename = argv[4];

  // Software Guide : BeginLatex
  //
  // We connect the pipeline.
  //
  // SoftwareGuide : EndLatex

  // Software Guide : BeginCodeSnippet
  reader1->SetFileName(inputFilename1);
  reader2->SetFileName(inputFilename2);
  writer->SetFileName(outputFilename);
  rescaler->SetOutputMinimum(itk::NumericTraits<OutputPixelType>::min());
  rescaler->SetOutputMaximum(itk::NumericTraits<OutputPixelType>::max());

  filter->SetInput1(reader1->GetOutput());
  filter->SetInput2(reader2->GetOutput());
  filter->SetRadius(atoi(argv[3]));

  rescaler->SetInput(filter->GetOutput());
  writer->SetInput(rescaler->GetOutput());
  // Software Guide : EndCodeSnippet

  typedef otb::CommandProgressUpdate<FilterType> CommandType;

  CommandType::Pointer observer = CommandType::New();
  filter->AddObserver(itk::ProgressEvent(), observer);

  try
    {
    writer->Update();
    }
  catch (itk::ExceptionObject& err)
    {
    std::cout << "ExceptionObject caught !" << std::endl;
    std::cout << err << std::endl;
    return -1;
    }

  // Software Guide : BeginLatex
  //
  // And that is all.
  //
  // SoftwareGuide : EndLatex

  return EXIT_SUCCESS;

}
