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


//  Software Guide : BeginCommandLineArgs
//    INPUTS: {QB_Suburb.png}
//    INPUTS: {QB_SuburbRotated10.png}
//    OUTPUTS: {ImageRegistration5Output.png}
//    OUTPUTS: {ImageRegistration5DifferenceBefore.png}
//    OUTPUTS: {ImageRegistration5DifferenceAfter.png}
//    0.1
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {QB_Suburb.png}
//    INPUTS: {QB_SuburbR10X13Y17.png}
//    OUTPUTS: {ImageRegistration5Output2.png}
//    OUTPUTS: {ImageRegistration5DifferenceBefore2.png}
//    OUTPUTS: {ImageRegistration5DifferenceAfter2.png}
//    1.0
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example illustrates the use of the \doxygen{itk}{CenteredRigid2DTransform}
// for performing rigid registration in $2D$. The example code is for the
// most part identical to that presented in Section
// \ref{sec:IntroductionImageRegistration}.  The main difference is the use
// of the CenteredRigid2DTransform here instead of the
// \doxygen{itk}{TranslationTransform}.
//
// \index{itk::CenteredRigid2DTransform}
//
// Software Guide : EndLatex

#include "itkImageRegistrationMethod.h"
#include "itkMeanSquaresImageToImageMetric.h"
#include "itkRegularStepGradientDescentOptimizer.h"
#include "otbImage.h"
#include "itkMeanImageFilter.h"

//  Software Guide : BeginLatex
//
//  In addition to the headers included in previous examples, the
//  following header must also be included.
//
//  \index{itk::CenteredRigid2DTransform!header}
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "itkCenteredRigid2DTransform.h"
// Software Guide : EndCodeSnippet

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "itkResampleImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

//  The following section of code implements a Command observer
//  that will monitor the evolution of the registration process.
//
#include "itkCommand.h"
class CommandIterationUpdate : public itk::Command
{
public:
  typedef  CommandIterationUpdate Self;
  typedef  itk::Command           Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  itkNewMacro(Self);
protected:
  CommandIterationUpdate() {}
public:
  typedef itk::RegularStepGradientDescentOptimizer OptimizerType;
  typedef   const OptimizerType *                  OptimizerPointer;

  void Execute(itk::Object *caller, const itk::EventObject& event)
  {
    Execute((const itk::Object *) caller, event);
  }

  void Execute(const itk::Object * object, const itk::EventObject& event)
  {
    OptimizerPointer optimizer =
      dynamic_cast<OptimizerPointer>(object);
    if (!itk::IterationEvent().CheckEvent(&event))
      {
      return;
      }
    std::cout << optimizer->GetCurrentIteration() << "   ";
    std::cout << optimizer->GetValue() << "   ";
    std::cout << optimizer->GetCurrentPosition() << std::endl;
  }
};

int main(int argc, char *argv[])
{
  if (argc < 4)
    {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " fixedImageFile  movingImageFile ";
    std::cerr << " outputImagefile  [differenceAfterRegistration] ";
    std::cerr << " [differenceBeforeRegistration] ";
    std::cerr << " [initialStepLength] " << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int Dimension = 2;
  typedef  unsigned char PixelType;

  typedef otb::Image<PixelType, Dimension> FixedImageType;
  typedef otb::Image<PixelType, Dimension> MovingImageType;

  //  Software Guide : BeginLatex
  //
  //  The transform type is instantiated using the code below. The only
  //  template parameter for this class is the representation type of the
  //  space coordinates.
  //
  //  \index{itk::CenteredRigid2DTransform!Instantiation}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::CenteredRigid2DTransform<double> TransformType;
  // Software Guide : EndCodeSnippet

  typedef itk::RegularStepGradientDescentOptimizer OptimizerType;

  typedef itk::MeanSquaresImageToImageMetric<FixedImageType,
      MovingImageType>    MetricType;

  typedef itk::LinearInterpolateImageFunction<MovingImageType,
      double>    InterpolatorType;

  typedef itk::ImageRegistrationMethod<FixedImageType,
      MovingImageType>  RegistrationType;

  MetricType::Pointer       metric        = MetricType::New();
  OptimizerType::Pointer    optimizer     = OptimizerType::New();
  InterpolatorType::Pointer interpolator  = InterpolatorType::New();
  RegistrationType::Pointer registration  = RegistrationType::New();

  registration->SetMetric(metric);
  registration->SetOptimizer(optimizer);
  registration->SetInterpolator(interpolator);

  //  Software Guide : BeginLatex
  //
  //  The transform object is constructed below and passed to the registration
  //  method.
  //
  //  \index{itk::CenteredRigid2DTransform!New()}
  //  \index{itk::CenteredRigid2DTransform!Pointer}
  //  \index{itk::RegistrationMethod!SetTransform()}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  TransformType::Pointer transform = TransformType::New();
  registration->SetTransform(transform);
  // Software Guide : EndCodeSnippet

  typedef otb::ImageFileReader<FixedImageType>  FixedImageReaderType;
  typedef otb::ImageFileReader<MovingImageType> MovingImageReaderType;

  FixedImageReaderType::Pointer  fixedImageReader  = FixedImageReaderType::New();
  MovingImageReaderType::Pointer movingImageReader = MovingImageReaderType::New();

  fixedImageReader->SetFileName(argv[1]);
  movingImageReader->SetFileName(argv[2]);

  //  Software Guide : BeginLatex
  //
  //  Since we are working with high resolution images and expected
  //  shifts are larger than the resolution, we will need to smooth
  //  the images in order to avoid the optimizer to get stucked on
  //  local minima. In order to do this, we will use a simple mean filter.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::MeanImageFilter<
      FixedImageType, FixedImageType>  FixedFilterType;

  typedef itk::MeanImageFilter<
      MovingImageType, MovingImageType>  MovingFilterType;

  FixedFilterType::Pointer  fixedFilter  = FixedFilterType::New();
  MovingFilterType::Pointer movingFilter = MovingFilterType::New();

  FixedImageType::SizeType indexFRadius;

  indexFRadius[0] = 4; // radius along x
  indexFRadius[1] = 4; // radius along y

  fixedFilter->SetRadius(indexFRadius);

  MovingImageType::SizeType indexMRadius;

  indexMRadius[0] = 4; // radius along x
  indexMRadius[1] = 4; // radius along y

  movingFilter->SetRadius(indexMRadius);

  fixedFilter->SetInput(fixedImageReader->GetOutput());
  movingFilter->SetInput(movingImageReader->GetOutput());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Now we can plug the output of the smoothing filters at the input
  //  of the registration method.
  //
  //  Software Guide : EndLatex
  // Software Guide : BeginCodeSnippet
  registration->SetFixedImage(fixedFilter->GetOutput());
  registration->SetMovingImage(movingFilter->GetOutput());
  // Software Guide : EndCodeSnippet

  fixedImageReader->Update();

  registration->SetFixedImageRegion(
    fixedImageReader->GetOutput()->GetBufferedRegion());

  //  Software Guide : BeginLatex
  //
  //  In this example, the input images are taken from readers. The code
  //  below updates the readers in order to ensure that the image parameters
  //  (size, origin and spacing) are valid when used to initialize the
  //  transform.  We intend to use the center of the fixed image as the
  //  rotation center and then use the vector between the fixed image center
  //  and the moving image center as the initial translation to be applied
  //  after the rotation.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  fixedImageReader->Update();
  movingImageReader->Update();
  // Software Guide : EndCodeSnippet

  typedef FixedImageType::SpacingType SpacingType;
  typedef FixedImageType::PointType   OriginType;
  typedef FixedImageType::RegionType  RegionType;
  typedef FixedImageType::SizeType    SizeType;

  //  Software Guide : BeginLatex
  //
  //  The center of rotation is computed using the origin, size and spacing of
  //  the fixed image.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  FixedImageType::Pointer fixedImage = fixedImageReader->GetOutput();

  const SpacingType fixedSpacing = fixedImage->GetSpacing();
  const OriginType  fixedOrigin  = fixedImage->GetOrigin();
  const RegionType  fixedRegion  = fixedImage->GetLargestPossibleRegion();
  const SizeType    fixedSize    = fixedRegion.GetSize();

  TransformType::InputPointType centerFixed;

  centerFixed[0] = fixedOrigin[0] + fixedSpacing[0] * fixedSize[0] / 2.0;
  centerFixed[1] = fixedOrigin[1] + fixedSpacing[1] * fixedSize[1] / 2.0;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The center of the moving image is computed in a similar way.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  MovingImageType::Pointer movingImage = movingImageReader->GetOutput();

  const SpacingType movingSpacing = movingImage->GetSpacing();
  const OriginType  movingOrigin  = movingImage->GetOrigin();
  const RegionType  movingRegion  = movingImage->GetLargestPossibleRegion();
  const SizeType    movingSize    = movingRegion.GetSize();

  TransformType::InputPointType centerMoving;

  centerMoving[0] = movingOrigin[0] + movingSpacing[0] * movingSize[0] / 2.0;
  centerMoving[1] = movingOrigin[1] + movingSpacing[1] * movingSize[1] / 2.0;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //   The most straightforward method of initializing the transform parameters
  //   is to configure the transform and then get its parameters with the
  //   method \code{GetParameters()}. Here we initialize the transform by
  //   passing the center of the fixed image as the rotation center with the
  //   \code{SetCenter()} method. Then the translation is set as the vector
  //   relating the center of the moving image to the center of the fixed
  //   image.  This last vector is passed with the method
  //   \code{SetTranslation()}.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  transform->SetCenter(centerFixed);
  transform->SetTranslation(centerMoving - centerFixed);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Let's finally initialize the rotation with a zero angle.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  transform->SetAngle(0.0);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Now we pass the current transform's parameters as the initial
  //  parameters to be used when the registration process starts.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  registration->SetInitialTransformParameters(transform->GetParameters());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Keeping in mind that the scale of units in rotation and translation is
  //  quite different, we take advantage of the scaling functionality provided
  //  by the optimizers. We know that the first element of the parameters array
  //  corresponds to the angle that is measured in radians, while the other
  //  parameters correspond to translations that are measured in the
  //  units of the spacin (pixels in our case).
  //  For this reason we use small factors in the scales associated with
  //  translations and the coordinates of the rotation center .
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef OptimizerType::ScalesType OptimizerScalesType;
  OptimizerScalesType optimizerScales(transform->GetNumberOfParameters());
  const double        translationScale = 1.0 / 1000.0;

  optimizerScales[0] = 1.0;
  optimizerScales[1] = translationScale;
  optimizerScales[2] = translationScale;
  optimizerScales[3] = translationScale;
  optimizerScales[4] = translationScale;

  optimizer->SetScales(optimizerScales);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Next we set the normal parameters of the optimization method. In this
  //  case we are using an \doxygen{itk}{RegularStepGradientDescentOptimizer}.
  //  Below, we define the optimization parameters like the relaxation factor,
  //  initial step length, minimal step length and number of iterations. These
  //  last two act as stopping criteria for the optimization.
  //
  //  \index{Regular\-Step\-Gradient\-Descent\-Optimizer!SetRelaxationFactor()}
  //  \index{Regular\-Step\-Gradient\-Descent\-Optimizer!SetMaximumStepLength()}
  //  \index{Regular\-Step\-Gradient\-Descent\-Optimizer!SetMinimumStepLength()}
  //  \index{Regular\-Step\-Gradient\-Descent\-Optimizer!SetNumberOfIterations()}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  double initialStepLength = 0.1;
  // Software Guide : EndCodeSnippet

  if (argc > 6)
    {
    initialStepLength = atof(argv[6]);
    }

  // Software Guide : BeginCodeSnippet
  optimizer->SetRelaxationFactor(0.6);
  optimizer->SetMaximumStepLength(initialStepLength);
  optimizer->SetMinimumStepLength(0.001);
  optimizer->SetNumberOfIterations(200);
  // Software Guide : EndCodeSnippet

  // Create the Command observer and register it with the optimizer.
  //
  CommandIterationUpdate::Pointer observer = CommandIterationUpdate::New();
  optimizer->AddObserver(itk::IterationEvent(), observer);

  try
    {
    registration->Update();
    }
  catch (itk::ExceptionObject& err)
    {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
    }

  OptimizerType::ParametersType finalParameters =
    registration->GetLastTransformParameters();

  const double finalAngle           = finalParameters[0];
  const double finalRotationCenterX = finalParameters[1];
  const double finalRotationCenterY = finalParameters[2];
  const double finalTranslationX    = finalParameters[3];
  const double finalTranslationY    = finalParameters[4];

  const unsigned int numberOfIterations = optimizer->GetCurrentIteration();

  const double bestValue = optimizer->GetValue();

  // Print out results
  //
  const double finalAngleInDegrees = finalAngle * 45.0 / atan(1.0);

  std::cout << "Result = " << std::endl;
  std::cout << " Angle (radians)   = " << finalAngle  << std::endl;
  std::cout << " Angle (degrees)   = " << finalAngleInDegrees  << std::endl;
  std::cout << " Center X      = " << finalRotationCenterX  << std::endl;
  std::cout << " Center Y      = " << finalRotationCenterY  << std::endl;
  std::cout << " Translation X = " << finalTranslationX  << std::endl;
  std::cout << " Translation Y = " << finalTranslationY  << std::endl;
  std::cout << " Iterations    = " << numberOfIterations << std::endl;
  std::cout << " Metric value  = " << bestValue          << std::endl;

  //  Software Guide : BeginLatex
  //
  //  Let's execute this example over two of the images provided in
  //  \code{Examples/Data}:
  //
  //  \begin{itemize}
  //  \item \code{QB\_Suburb.png}
  //  \item \code{QB\_SuburbRotated10.png}
  //  \end{itemize}
  //
  //  The second image is the result of intentionally rotating the first image
  //  by $10$ degrees around the geometrical center of the image. Both images
  //  have unit-spacing and are shown in Figure
  //  \ref{fig:FixedMovingImageRegistration5}. The registration takes $21$
  //  iterations and produces the results:
  //
  //  \begin{center}
  //  \begin{verbatim}
  //  [0.176168, 134.515, 103.011, -0.00182313, 0.0717891]
  //  \end{verbatim}
  //  \end{center}
  //
  //  These results are interpreted as
  //
  //  \begin{itemize}
  //  \item Angle         =                  $0.176168$     radians
  //  \item Center        = $( 134.515    , 103.011     )$ pixels
  //  \item Translation   = $(   -0.00182313,   0.0717891 )$ pixels
  //  \end{itemize}
  //
  //  As expected, these values match the misalignment intentionally introduced
  //  into the moving image quite well, since $10$ degrees is about $0.174532$
  //  radians.
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{QB_Suburb.eps}
  // \includegraphics[width=0.44\textwidth]{QB_SuburbRotated10.eps}
  // \itkcaption[Rigid2D Registration input images]{Fixed and moving images
  // are provided as input to the registration method using the CenteredRigid2D
  // transform.}
  // \label{fig:FixedMovingImageRegistration5}
  // \end{figure}
  //
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.32\textwidth]{ImageRegistration5Output.eps}
  // \includegraphics[width=0.32\textwidth]{ImageRegistration5DifferenceBefore.eps}
  // \includegraphics[width=0.32\textwidth]{ImageRegistration5DifferenceAfter.eps}
  // \itkcaption[Rigid2D Registration output images]{Resampled moving image
  // (left). Differences between the fixed and moving images, before (center)
  // and after (right) registration using the CenteredRigid2D transform.}
  // \label{fig:ImageRegistration5Outputs}
  // \end{figure}
  //
  // Figure \ref{fig:ImageRegistration5Outputs} shows from left to right the
  // resampled moving image after registration, the difference between fixed
  // and moving images before registration, and the difference between fixed
  // and resampled moving image after registration. It can be seen from the
  // last difference image that the rotational component has been solved but
  // that a small centering misalignment persists.
  //
  //  Software Guide : EndLatex

  typedef itk::ResampleImageFilter<
      MovingImageType,
      FixedImageType>    ResampleFilterType;

  TransformType::Pointer finalTransform = TransformType::New();

  finalTransform->SetParameters(finalParameters);

  ResampleFilterType::Pointer resample = ResampleFilterType::New();

  resample->SetTransform(finalTransform);
  resample->SetInput(movingImageReader->GetOutput());

  resample->SetSize(fixedImage->GetLargestPossibleRegion().GetSize());
  resample->SetOutputOrigin(fixedImage->GetOrigin());
  resample->SetOutputSpacing(fixedImage->GetSpacing());
  resample->SetDefaultPixelValue(100);

  typedef otb::ImageFileWriter<FixedImageType> WriterFixedType;

  WriterFixedType::Pointer writer =  WriterFixedType::New();

  writer->SetFileName(argv[3]);

  writer->SetInput(resample->GetOutput());

  try
    {
    writer->Update();
    }
  catch (itk::ExceptionObject& excp)
    {
    std::cerr << "ExceptionObject while writing the resampled image !" <<
    std::endl;
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  typedef otb::Image<float, Dimension> DifferenceImageType;

  typedef itk::SubtractImageFilter<
      FixedImageType,
      FixedImageType,
      DifferenceImageType> DifferenceFilterType;

  DifferenceFilterType::Pointer difference = DifferenceFilterType::New();

  typedef  unsigned char OutputPixelType;

  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;

  typedef itk::RescaleIntensityImageFilter<
      DifferenceImageType,
      OutputImageType>   RescalerType;

  RescalerType::Pointer intensityRescaler = RescalerType::New();

  intensityRescaler->SetOutputMinimum(0);
  intensityRescaler->SetOutputMaximum(255);

  difference->SetInput1(fixedImageReader->GetOutput());
  difference->SetInput2(resample->GetOutput());

  resample->SetDefaultPixelValue(1);

  intensityRescaler->SetInput(difference->GetOutput());

  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  WriterType::Pointer writer2 =  WriterType::New();

  writer2->SetInput(intensityRescaler->GetOutput());

  try
    {
    // Compute the difference image between the
    // fixed and moving image after registration.
    if (argc > 4)
      {
      writer2->SetFileName(argv[5]);
      writer2->Update();
      }

    // Compute the difference image between the
    // fixed and resampled moving image after registration.
    TransformType::Pointer identityTransform = TransformType::New();
    identityTransform->SetIdentity();
    resample->SetTransform(identityTransform);
    if (argc > 5)
      {
      writer2->SetFileName(argv[4]);
      writer2->Update();
      }
    }
  catch (itk::ExceptionObject& excp)
    {
    std::cerr << "Error while writing difference images" << std::endl;
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  //  Software Guide : BeginLatex
  //
  //  Let's now consider the case in which rotations and translations are
  //  present in the initial registration, as in the following pair
  //  of images:
  //
  //  \begin{itemize}
  //  \item \code{QB\_Suburb.png}
  //  \item \code{QB\_SuburbR10X13Y17.png}
  //  \end{itemize}
  //
  //  The second image is the result of intentionally rotating the first image
  //  by $10$ degrees and then translating it $13$ pixels in $X$ and
  //  $17$ pixels in $Y$.
  //  Both images have unit-spacing and are shown in Figure
  //  \ref{fig:FixedMovingImageRegistration5b}. In order to accelerate
  //  convergence it is convenient to use a larger step length as shown here.
  //
  //  \code{optimizer->SetMaximumStepLength( 1.0 ); }
  //
  //  The registration now takes $34$ iterations and produces the following
  //  results:
  //
  //  \begin{center}
  //  \begin{verbatim}
  //  [0.176125, 135.553, 102.159, -11.9102, -15.8045]
  //  \end{verbatim}
  //  \end{center}
  //
  //  These parameters are interpreted as
  //
  //  \begin{itemize}
  //  \item Angle         =                     $0.176125$   radians
  //  \item Center        = $( 135.553     , 102.159      )$ millimeters
  //  \item Translation   = $(  -11.9102     ,  -15.8045     )$ millimeters
  //  \end{itemize}
  //
  //  These values approximately match the initial misalignment intentionally
  //  introduced into the moving image, since $10$ degrees is about $0.174532$
  //  radians. The horizontal translation is well resolved while the vertical
  //  translation ends up being off by about one millimeter.
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{QB_Suburb.eps}
  // \includegraphics[width=0.44\textwidth]{QB_SuburbR10X13Y17.eps}
  // \itkcaption[Rigid2D Registration input images]{Fixed and moving images
  // provided as input to the registration method using the CenteredRigid2D
  // transform.}
  // \label{fig:FixedMovingImageRegistration5b}
  // \end{figure}
  //
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.32\textwidth]{ImageRegistration5Output2.eps}
  // \includegraphics[width=0.32\textwidth]{ImageRegistration5DifferenceBefore2.eps}
  // \includegraphics[width=0.32\textwidth]{ImageRegistration5DifferenceAfter2.eps}
  // \itkcaption[Rigid2D Registration output images]{Resampled moving image
  // (left). Differences between the fixed and moving images, before (center)
  // and after (right) registration with the CenteredRigid2D transform.}
  // \label{fig:ImageRegistration5Outputs2}
  // \end{figure}
  //
  // Figure \ref{fig:ImageRegistration5Outputs2} shows the output of the
  // registration. The rightmost image of this figure shows the difference
  // between the fixed image and the resampled moving image after registration.
  //
  //  Software Guide : EndLatex

  return EXIT_SUCCESS;
}
