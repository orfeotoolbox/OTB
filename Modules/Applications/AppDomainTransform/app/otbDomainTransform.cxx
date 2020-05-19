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


#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "otbWaveletImageFilter.h"
#include "otbWaveletInverseImageFilter.h"
#include "otbWaveletGenerator.h"

#include <itkConfigure.h>
#include <itkForwardFFTImageFilter.h>
#include <itkInverseFFTImageFilter.h>
#include <itkFFTShiftImageFilter.h>
#include <itkFFTWGlobalConfiguration.h>

#include "otbFunctorImageFilter.h"

#include "otbComplexToVectorImageCastFilter.h"

namespace otb
{
namespace Wrapper
{

class DomainTransform : public Application
{
public:
  /** Standard class typedefs. */
  typedef DomainTransform         Self;
  typedef Application             Superclass;
  typedef itk::SmartPointer<Self> Pointer;

  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef float                         InputPixelType;
  typedef float                         OutputPixelType;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(Self, otb::Application);

private:
  DomainTransform()
  {
  }

  ~DomainTransform() override
  {
  }

  void CleanupFFTWThreads()
  {
// This is a trick to make sure fftw will cleanup its threads when application
// shared lib is released.
#if defined(ITK_USE_FFTWF)
    fftwf_cleanup_threads();
    fftwf_cleanup();
#endif
#if defined(ITK_USE_FFTWD)
    fftw_cleanup_threads();
    fftw_cleanup();
#endif
  }

  void DoInit() override
  {
    SetName("DomainTransform");
    SetDescription("Domain Transform application for wavelet and fourier");

    // Documentation
    SetDocLongDescription("Domain Transform application for wavelet and fourier.");
    SetDocLimitations("This application is not streamed, check your system resources when processing large images");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("otbWaveletImageFilter, otbWaveletInverseImageFilter, otbWaveletTransform");
    AddDocTag(Tags::Filter);

    // Parameters
    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in",
                            "This will take an input image to be transformed"
                            " image. For FFT inverse transform, it expects a complex image as two-band"
                            " image in which the first band represents the real part and second band represents"
                            " the imaginary part.");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out",
                            "This parameter holds the output file name to"
                            " which transformed image will be written. This has a slightly different"
                            " behaviour depending on transform type. \nFor Wavelet, output is a"
                            " single band image for both forward and inverse transform. For FFT"
                            " forward transform, output is two band image where first band represents"
                            " real part and second band represents imaginary part of a complex image.");

    AddParameter(ParameterType_Choice, "mode", "Mode");
    SetParameterDescription("mode",
                            "This parameter allows one to select between"
                            " fft(fourier) and wavelet");

    AddChoice("mode.fft", "FFT transform");
    SetParameterDescription("mode.fft", "FFT transform");

    AddParameter(ParameterType_Bool, "mode.fft.shift", "Shift fft transform");
    SetParameterDescription("mode.fft.shift", "Shift transform of fft filter");

    AddChoice("mode.wavelet", "Wavelet");
    SetParameterDescription("mode.wavelet", "Wavelet transform");

    AddParameter(ParameterType_Choice, "mode.wavelet.form", "Select wavelet form");
    AddChoice("mode.wavelet.form.haar", "HAAR");
    AddChoice("mode.wavelet.form.db4", "DAUBECHIES4");
    AddChoice("mode.wavelet.form.db6", "DAUBECHIES6");
    AddChoice("mode.wavelet.form.db8", "DAUBECHIES8");
    AddChoice("mode.wavelet.form.db12", "DAUBECHIES12");
    AddChoice("mode.wavelet.form.db20", "DAUBECHIES20");
    AddChoice("mode.wavelet.form.sb24", "SPLINE_BIORTHOGONAL_2_4");
    AddChoice("mode.wavelet.form.sb44", "SPLINE_BIORTHOGONAL_4_4");
    AddChoice("mode.wavelet.form.sym8", "SYMLET8");

    // Default values for mode
    SetParameterString("mode", "wavelet");
    SetParameterString("mode.wavelet.form", "haar");

    AddParameter(ParameterType_Choice, "direction", "Direction");
    AddChoice("direction.forward", "Forward");
    AddChoice("direction.inverse", "Inverse");

    AddParameter(ParameterType_Int, "mode.wavelet.nlevels", "Number of decomposition levels");
    SetParameterDescription("mode.wavelet.nlevels", "Number of decomposition levels");
    SetDefaultParameterInt("mode.wavelet.nlevels", 2);
    SetMinimumParameterIntValue("mode.wavelet.nlevels", 2);

    AddRAMParameter();

    SetDocExampleParameterValue("in", "input.tif");
    SetDocExampleParameterValue("mode.wavelet.form", "haar");
    SetDocExampleParameterValue("out", "output_wavelet_haar.tif");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
  }

  void DoExecute() override
  {
    int dir  = GetParameterInt("direction");
    int mode = GetParameterInt("mode");

    if (dir != 0 && dir != 1)
    {
      itkExceptionMacro(<< "-dir is '" << dir << "'."
                        << "It must be either 'forward' or 'inverse'");
    }

    if (mode != 0 && mode != 1)
    {
      itkExceptionMacro(<< "mode is '" << mode << "'."
                        << "It must must be either 'fft' or 'wavelet'");
    }

    if (mode == 1)
    {
      int          wavelet_type = GetParameterInt("mode.wavelet.form");
      unsigned int nlevels      = GetParameterInt("mode.wavelet.nlevels");
      switch (wavelet_type)
      {
      case 0:
      {
        DoWaveletTransform<otb::Wavelet::HAAR>(dir, nlevels);
        break;
      }
      case 1:
      {
        DoWaveletTransform<otb::Wavelet::DB4>(dir, nlevels);
        break;
      }
      case 2:
      {
        DoWaveletTransform<otb::Wavelet::DB4>(dir, nlevels);
        break;
      }
      case 3:
      {
        DoWaveletTransform<otb::Wavelet::DB6>(dir, nlevels);
        break;
      }
      case 4:
      {
        DoWaveletTransform<otb::Wavelet::DB8>(dir, nlevels);
        break;
      }
      case 5:
      {
        DoWaveletTransform<otb::Wavelet::DB12>(dir, nlevels);
        break;
      }
      case 6:
      {
        DoWaveletTransform<otb::Wavelet::DB20>(dir, nlevels);
        break;
      }
      case 7:
      {
        DoWaveletTransform<otb::Wavelet::SPLINE_BIORTHOGONAL_2_4>(dir, nlevels);
        break;
      }
      case 8:
      {
        DoWaveletTransform<otb::Wavelet::SPLINE_BIORTHOGONAL_4_4>(dir, nlevels);
        break;
      }
      case 9:
      {
        DoWaveletTransform<otb::Wavelet::SYMLET8>(dir, nlevels);
        break;
      }
      default:
      {
        itkExceptionMacro(<< "Invalid wavelet type: '" << wavelet_type << "'");
        break;
      }
      }
    }
    else
    {
      // fft ttransform
      bool                                              shift = GetParameterInt("mode.fft.shift");
      typedef otb::Image<std::complex<OutputPixelType>> ComplexOutputImageType;

      if (dir == 0)
      {
        // forward fft
        typedef otb::Image<InputPixelType> TInputImage;
        typedef TInputImage::Pointer       TInputImagePointer;

        // get input parameter as otb::Image<InputPixelType>
        TInputImagePointer inImage = GetParameterImage<TInputImage>("in");

        // typedef itk::::ForwardFFTImageFilter over otbImage< InputPixelType >

        typedef itk::ForwardFFTImageFilter<TInputImage, ComplexOutputImageType> FFTFilter;
        FFTFilter::Pointer fwdFilter = FFTFilter::New();
        fwdFilter->SetInput(inImage);


        // typedef VectorImage for output
        typedef otb::VectorImage<OutputPixelType> TOutputImage;

        typedef otb::ComplexToVectorImageCastFilter<ComplexOutputImageType, TOutputImage> ComplexToVectorImageCastFilter;
        ComplexToVectorImageCastFilter::Pointer unaryFunctorImageFilter = ComplexToVectorImageCastFilter::New();

        if (shift)
        {
          otbAppLogINFO(<< "Applying Shift image filter");
          typedef itk::FFTShiftImageFilter<FFTFilter::OutputImageType, FFTFilter::OutputImageType> FFTShiftFilterType;

          FFTShiftFilterType::Pointer fftShiftFilter = FFTShiftFilterType::New();

          fftShiftFilter->SetInput(fwdFilter->GetOutput());
          fftShiftFilter->Update();
          unaryFunctorImageFilter->SetInput(fftShiftFilter->GetOutput());
        }
        else
        {
          unaryFunctorImageFilter->SetInput(fwdFilter->GetOutput());
        }

        unaryFunctorImageFilter->Update();

        // set output image
        SetParameterOutputImage<TOutputImage>("out", unaryFunctorImageFilter->GetOutput());
      }
      else
      {
        // inverse fft
        typedef otb::VectorImage<InputPixelType> TInputImage;
        typedef TInputImage::Pointer             TInputImagePointer;

        TInputImagePointer inImage = GetParameterImage("in");

        // typedef TComplexImage for InverseFFTImageFilter input
        // This a image type of std::complex<InputPixelType>
        typedef otb::Image<std::complex<InputPixelType>, 2> TComplexImage;
        // typedef TOutputImage for InverseFFTImageFilter output
        typedef otb::Image<OutputPixelType> TOutputImage;

        auto toComplex = [](const typename TInputImage::PixelType& A) {
          return typename TComplexImage::PixelType(static_cast<typename TComplexImage::PixelType::value_type>(A[0]),
                                                   static_cast<typename TComplexImage::PixelType::value_type>(A[1]));
        };

        auto toComplexFilter = NewFunctorFilter(toComplex);

        if (shift)
        {
          typedef itk::FFTShiftImageFilter<TInputImage, TInputImage> FFTShiftFilterType;

          FFTShiftFilterType::Pointer fftShiftFilter = FFTShiftFilterType::New();
          fftShiftFilter->SetInput(inImage);

          fftShiftFilter->Update();

          toComplexFilter->SetInputs(fftShiftFilter->GetOutput());
        }
        else
        {
          toComplexFilter->SetInputs(inImage);
        }

        toComplexFilter->Update();

        // typedef itk::::InverseFFTImageFilter over TComplexImage
        typedef itk::InverseFFTImageFilter<TComplexImage, TOutputImage> FFTFilter;
        FFTFilter::Pointer invFilter = FFTFilter::New();
        invFilter->SetInput(toComplexFilter->GetOutput());
        invFilter->Update();

        // set output image
        SetParameterOutputImage<TOutputImage>("out", invFilter->GetOutput());
      }
    }

    // at the end, cleanup FFTW Threads
    CleanupFFTWThreads();
  }

  template <otb::Wavelet::Wavelet TWaveletOperator>
  void DoWaveletTransform(const int dir, const unsigned int nlevels, const std::string inkey = "in", const std::string outkey = "out")
  {
    typedef otb::Image<InputPixelType>    TInputImage;
    typedef otb::Image<OutputPixelType>   TOutputImage;
    typedef typename TInputImage::Pointer TInputImagePointer;

    TInputImagePointer inImage = GetParameterImage<TInputImage>(inkey);

    if (dir == 0)
    {
      typedef otb::WaveletImageFilter<TInputImage, TOutputImage, TWaveletOperator> TWaveletImageFilter;

      typedef typename TWaveletImageFilter::Pointer TWaveletImageFilterPointer;
      TWaveletImageFilterPointer                    waveletImageFilter = TWaveletImageFilter::New();

      waveletImageFilter->SetInput(inImage);
      waveletImageFilter->SetNumberOfDecompositions(nlevels);
      waveletImageFilter->Update();
      SetParameterOutputImage<TOutputImage>(outkey, waveletImageFilter->GetOutput());
    }
    else
    {
      typedef otb::WaveletInverseImageFilter<TInputImage, TOutputImage, TWaveletOperator> TWaveletImageFilter;

      typedef typename TWaveletImageFilter::Pointer TWaveletImageFilterPointer;

      TWaveletImageFilterPointer waveletImageFilter = TWaveletImageFilter::New();

      waveletImageFilter->SetInput(inImage);
      waveletImageFilter->SetNumberOfDecompositions(nlevels);
      waveletImageFilter->Update();

      SetParameterOutputImage<TOutputImage>(outkey, waveletImageFilter->GetOutput());
    }
  }
};

} // end of namespace Wrapper
} // end of namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::DomainTransform)
