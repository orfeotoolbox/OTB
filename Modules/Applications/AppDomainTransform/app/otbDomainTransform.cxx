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

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "otbWaveletImageFilter.h"
#include "otbWaveletInverseImageFilter.h"
#include "otbWaveletGenerator.h"

#include <itkConfigure.h>
#include <itkForwardFFTImageFilter.h>
#include <itkInverseFFTImageFilter.h>
#include <itkUnaryFunctorImageFilter.h>

namespace otb
{
	namespace Wrapper
	{

		template< class TInput, class TOutput>
		class FromComplexPixel
		{
		public:
			FromComplexPixel ( ) {  };
			
			~FromComplexPixel( ) {  };
			
			bool operator!=( const FromComplexPixel & ) const
			{
				return false;
			}
		   
			bool operator==( const FromComplexPixel & other ) const
			{
				return !(*this != other);
			}
			inline TOutput operator( )( const TInput & A ) const
			{
				TOutput out;
				out.SetSize(2);
				out[0] = A.real();
				out[1] = A.imag();	
				return out;
			}
		};

		template< class TInput, class TOutput>
		class ToComplexPixel
		{
		public:
			ToComplexPixel ( ) { };
			
			~ToComplexPixel( ) { };
			
			bool operator!=( const ToComplexPixel & ) const
			{
				return false;
			}
			bool operator==( const ToComplexPixel & other ) const
			{
				return !(*this != other);
			}
			inline TOutput operator( )( const TInput & A ) const
			{
				TOutput out(A[0], A[1]);				
				return out;
			}
		};

		
		class DomainTransform : public Application
		{
		public:
			/** Standard class typedefs. */
			typedef DomainTransform     Self;
			typedef Application                   Superclass;
			typedef itk::SmartPointer<Self>       Pointer;

			typedef itk::SmartPointer<const Self> ConstPointer;
			typedef float TInputPixel;
			typedef float TOutputPixel;

			/** Standard macro */
			itkNewMacro(Self);

			itkTypeMacro(Self, otb::Application);

		private:

			void DoInit() ITK_OVERRIDE
			{
				SetName("DomainTransform");
	
				const char * app_descr = "Domain Transform application for wavelet and fourier";
				SetDescription(app_descr);

				// Documentation
				SetDocName(app_descr);
				SetDocLongDescription("TODO");
				SetDocLimitations("None");
				SetDocAuthors("OTB-Team");
				SetDocSeeAlso("otbWaveletImageFilter, otbWaveletInverseImageFilter, otbWaveletTransform");
				AddDocTag(Tags::Filter);

				AddParameter(ParameterType_InputImage, "in",  "Input Image");
				SetParameterDescription("in", "The input image");

				AddRAMParameter();

				AddParameter(ParameterType_Choice, "mode", "mode");
				SetParameterDescription("mode", "transform mode");
				//#if defined(ITK_USE_FFTWF) || defined(ITK_USE_FFTWD)
				AddChoice("mode.fft", "FFT transform");
				SetParameterDescription("mode.fft", "FFT transform");
				//#endif
				AddChoice("mode.wavelet", "wavelet");
				SetParameterDescription("mode.wavelet", "Wavelet transform");
				AddParameter(ParameterType_Choice,
							 "mode.wavelet.form",
							 "Select wavelet form. default is HAAR");

				AddChoice("mode.wavelet.form.haar", "HAAR");
				AddChoice("mode.wavelet.form.db4", "DAUBECHIES4");
				AddChoice("mode.wavelet.form.db6", "DAUBECHIES6");
				AddChoice("mode.wavelet.form.db8", "DAUBECHIES8");
				AddChoice("mode.wavelet.form.db12", "DAUBECHIES12");
				AddChoice("mode.wavelet.form.db20", "DAUBECHIES20");
				AddChoice("mode.wavelet.form.sb24", "SPLINE_BIORTHOGONAL_2_4");
				AddChoice("mode.wavelet.form.sb44", "SPLINE_BIORTHOGONAL_4_4");
				AddChoice("mode.wavelet.form.sym8", "SYMLET8");
				
				//Default value  
				SetParameterString("mode", "wavelet");
				SetParameterString("mode.wavelet.form", "haar");


				AddParameter(ParameterType_Choice,
							 "direction",
							 "direction: forward/inverse");
				
				AddChoice("direction.forward", "forward");
				AddChoice("direction.inverse", "inverse");

				AddParameter(ParameterType_OutputImage, "out", "Output Image");
				SetParameterDescription("out", "Output image");


				SetDocExampleParameterValue("in", "input.tif");
				SetDocExampleParameterValue("mode.wavelet.form", "haar");
				SetDocExampleParameterValue("out", "output_wavelet_haar.tif");
			}

			void DoUpdateParameters() ITK_OVERRIDE
			{
				// wavelet and fourier are mutually exclusive parameters.
				// check it here
#if 0
				if ( HasUserValue("mode.wavelet.form") &&
					 GetParameterString("mode") == "fft"
					 )
				{
					std::stringstream oss;
					oss << std::endl
						<< this->GetNameOfClass() << "::DoUpdateParameters() "
						<< "Cannot use 'mode.wavelet.form' and '-mode fft' at same time"
						<< std::endl;
      
					throw std::runtime_error( oss.str() );
				}
#endif  
			}



			void DoExecute() ITK_OVERRIDE
			{
				
				int direction = GetParameterInt("direction");

				int mode = GetParameterInt("mode");

				if( direction != 0 || direction != 1)
				{
					itkExceptionMacro( << "direction must be either 'fwd' or 'inv'");
				}
				
				if( mode != 0 || mode != 1)
				{
					itkExceptionMacro( << "mode must be either 'fft' or 'wavelet'");
				}

				
				if ( mode == 1)
				{
					int wavelet_type = GetParameterInt("mode.wavelet.form");
					switch (wavelet_type)
					{
					case 0:
						DoWaveletTransform<otb::Wavelet::HAAR> ( direction);
						break;					
					case 1:
						DoWaveletTransform< otb::Wavelet::DB4 > (direction);
						break;
					case 2:
						DoWaveletTransform<otb::Wavelet::DB4> (direction);
						break;
					case 3:
						DoWaveletTransform<otb::Wavelet::DB6> (direction);
						break;
					case 4:
						DoWaveletTransform<otb::Wavelet::DB8> (direction);
						break;
					case 5:
						DoWaveletTransform<otb::Wavelet::DB12> (direction);
						break;
					case 6:
						DoWaveletTransform<otb::Wavelet::DB20> (direction);
						break;
					case 7:
						DoWaveletTransform<otb::Wavelet::SPLINE_BIORTHOGONAL_2_4 > (direction);
						break;
					case 8:
						DoWaveletTransform<otb::Wavelet::SPLINE_BIORTHOGONAL_4_4> (direction);
						break;
					case 9:
						DoWaveletTransform<otb::Wavelet::SYMLET8> (direction);
						break;        					
					default:
						itkExceptionMacro( << "Invalid wavelet type: '" <<  wavelet_type << "'");
						break;
					}

				}
				// fft ttransform
				else
				{
					//forward fft
					if (direction == 0 )
					{

						typedef otb::Image<TInputPixel>          TInputImage;
						typedef typename TInputImage::Pointer TInputImagePointer;

						//get input paramter as otb::Image<TInputPixel>
						TInputImagePointer inImage = GetParameterImage<TInputImage>("in");
						inImage->UpdateOutputInformation();


						//typedef itk::::ForwardFFTImageFilter over otbImage< TInputPixel >
						typedef itk::ForwardFFTImageFilter < TInputImage> FFTFilter;
						FFTFilter::Pointer fwdFilter = FFTFilter::New();
						fwdFilter->SetInput( inImage );

						//typedef VectorImage for output of UnaryFunctorImageFilter
						typedef otb::VectorImage<TOutputPixel>          TOutputImage;

						//UnaryFunctorImageFilter for Complex to VectorImage
						typedef itk::UnaryFunctorImageFilter<
							typename FFTFilter::OutputImageType,
							TOutputImage,
							FromComplexPixel<
								typename FFTFilter::OutputImageType::PixelType,
								TOutputImage::PixelType> > UnaryFunctorImageFilter;


						//convert complex pixel to variable length vector
						//with unaryfunctor image filter
						UnaryFunctorImageFilter::Pointer unaryFunctorImageFilter
							= UnaryFunctorImageFilter::New();
						
						unaryFunctorImageFilter->SetInput(fwdFilter->GetOutput());
						unaryFunctorImageFilter->Update();

						//set output image
						SetParameterOutputImage<TOutputImage>
							( "out", unaryFunctorImageFilter->GetOutput() );
					}
					//inverse fft
					else
					{
						typedef otb::VectorImage<TInputPixel>          TInputImage;
						typedef typename TInputImage::Pointer TInputImagePointer;
						
						TInputImagePointer inImage = GetParameterImage("in");
    
						inImage->UpdateOutputInformation();

						// typedef TComplexImage for InverseFFTImageFilter input
						// This a image type of std::complex<TInputPixel>
						typedef otb::Image<
							std::complex<TInputPixel>, 2 > TComplexImage;
						//typedef TOutputImage for InverseFFTImageFilter output 
						typedef otb::Image< TOutputPixel >  TOutputImage;
						
						// a unary functor to convert vectorimage to complex image
						typedef itk::UnaryFunctorImageFilter
							<TInputImage,
							 TComplexImage,
							 ToComplexPixel
							 <TInputImage::PixelType,
							  TComplexImage::PixelType> > UnaryFunctorImageFilter;
						
						UnaryFunctorImageFilter::Pointer
							unary_filter = UnaryFunctorImageFilter::New();
						
						unary_filter->SetInput(inImage);
						
						//typedef itk::::InverseFFTImageFilter over TComplexImage
						typedef itk::InverseFFTImageFilter
							< TComplexImage,
							  TOutputImage > FFTFilter;
						FFTFilter::Pointer invFilter = FFTFilter::New();
						invFilter->SetInput( unary_filter->GetOutput() );
						invFilter->Update();
						
						//set output image
						SetParameterOutputImage<TOutputImage>( "out", invFilter->GetOutput() );						
					}

				}

			}



			template<otb::Wavelet::Wavelet TWaveletOperator>
			void DoWaveletTransform(const int direction,
									const std::string inkey = "in",
									const std::string outkey = "out")
			{

				typedef otb::Image< TInputPixel >  TInputImage;
				typedef otb::Image< TOutputPixel >  TOutputImage;
				typedef typename TInputImage::Pointer TInputImagePointer;
     
				TInputImagePointer inImage = GetParameterImage<TInputImage>(inkey);
    
				inImage->UpdateOutputInformation();


				if( direction == 0)
				{
		
					typedef otb::WaveletImageFilter<
						TInputImage,
						TOutputImage,
						TWaveletOperator> TWaveletImageFilter;
        
					typedef typename
						TWaveletImageFilter::Pointer
						TWaveletImageFilterPointer;
					TWaveletImageFilterPointer waveletImageFilter =
						TWaveletImageFilter::New();
					
					waveletImageFilter->SetInput(inImage);
					waveletImageFilter->Update();
					SetParameterOutputImage<TOutputImage>(outkey, waveletImageFilter->GetOutput() );
					
				}
    
				else
				{
					typedef otb::WaveletInverseImageFilter<
						TInputImage,
						TOutputImage,
						TWaveletOperator > TWaveletImageFilter;
        
					typedef typename
						TWaveletImageFilter::Pointer
						TWaveletImageFilterPointer;
					
					TWaveletImageFilterPointer waveletImageFilter =
						TWaveletImageFilter::New();
					
					waveletImageFilter->SetInput(inImage);
					waveletImageFilter->Update();

					SetParameterOutputImage<TOutputImage>( outkey, waveletImageFilter->GetOutput() );
					
				}
    
			}
			
		};
	}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::DomainTransform)
