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
#ifndef __otbGridResampleImageFilter_h
#define __otbGridResampleImageFilter_h


#include "itkImageToImageFilter.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkDefaultConvertPixelTraits.h"

#include "otbMacro.h"

namespace otb
{

/** \class GridResampleImageFilter
 *  \brief Resample input image on a new origin/spacing/size grid
 * 
 *  This filter resample the input image on a new grid defined by
 *  OutputOrigin, OutputSpacing and OutputSize, using the provided
 *  interpolator.
 * 
 *  This is equivalent to a itk::ResampleImageFilter using an
 *  itk::IdentityTransform, except that it is slightly more efficient
 *  and that in this simplified case it is possible to explicitely
 *  compute the input requested region. The GridResampleImageFilter
 *  therefore supports streaming, contrary to the
 *  itk::ResampleImageFilter.
 *  
 *  When grid position is outside of the input image domain, the
 *  default EdgePaddingValue is used.
 *
 *  If CheckOutputBounds flag is set to true (default value), the
 *  interpolated value will be checked for output pixel type range
 *  prior to casting.
 *   
 * \ingroup OTBImageManipulation
 * \ingroup Streamed
 * \ingroup Threaded
 **/

template <typename TInputImage, typename TOutputImage,
          typename TInterpolatorPrecision = double>
class ITK_EXPORT GridResampleImageFilter :
    public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef GridResampleImageFilter                                         Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage>              Superclass;
  typedef itk::SmartPointer<Self>                                         Pointer;
  typedef itk::SmartPointer<const Self>                                   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GridResampleImageFilter, itk::ImageToImageFilter);

  /** Number of dimensions. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TOutputImage::ImageDimension);
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Typedef parameters*/
  typedef TInputImage                                                     InputImageType;
  typedef TOutputImage                                                    OutputImageType;
  typedef typename OutputImageType::RegionType                            OutputImageRegionType;
  typedef typename TOutputImage::PixelType                                OutputPixelType;

  
  typedef itk::DefaultConvertPixelTraits<OutputPixelType>                 OutputPixelConvertType;
  typedef typename OutputPixelConvertType::ComponentType                  OutputPixelComponentType;
  
  /** Interpolator type */
  typedef itk::InterpolateImageFunction<InputImageType,
                                        TInterpolatorPrecision>           InterpolatorType;
  typedef typename InterpolatorType::Pointer                              InterpolatorPointerType;
  typedef itk::LinearInterpolateImageFunction<InputImageType,
                                              TInterpolatorPrecision>     DefaultInterpolatorType;
  typedef typename InterpolatorType::OutputType                           InterpolatorOutputType;
  typedef itk::DefaultConvertPixelTraits< InterpolatorOutputType >        InterpolatorConvertType;
  typedef typename InterpolatorConvertType::ComponentType                 InterpolatorComponentType;
  
  /** Input pixel continuous index typdef */
  typedef typename itk::ContinuousIndex<double,InputImageDimension >      ContinuousInputIndexType;
  
  /** ImageBase typedef */
  typedef itk::ImageBase<OutputImageType::ImageDimension>                 ImageBaseType;
  typedef typename ImageBaseType::SpacingType                             SpacingType;
  typedef typename ImageBaseType::SizeType                                SizeType;
  typedef typename ImageBaseType::PointType                               PointType;
  typedef typename ImageBaseType::IndexType                               IndexType;
  
  itkSetMacro(OutputStartIndex,IndexType);
  itkGetConstReferenceMacro(OutputStartIndex,IndexType);
  
  itkSetMacro(OutputSize,SizeType);
  itkGetConstReferenceMacro(OutputSize,SizeType);

  itkSetMacro(OutputOrigin,PointType);
  itkGetConstReferenceMacro(OutputOrigin,PointType);

  itkSetMacro(OutputSpacing,SpacingType);
  itkGetConstReferenceMacro(OutputSpacing,SpacingType);

  itkSetMacro(EdgePaddingValue,OutputPixelType);
  itkGetConstReferenceMacro(EdgePaddingValue,OutputPixelType);

  itkSetMacro(CheckOutputBounds,bool);
  itkGetMacro(CheckOutputBounds,bool);
  itkBooleanMacro(CheckOutputBounds);
  
  itkSetObjectMacro(Interpolator,InterpolatorType);
  itkGetObjectMacro(Interpolator,InterpolatorType);
  
  /** Import output parameters from a given image */
  void SetOutputParametersFromImage(const ImageBaseType * image);

  /** Method Compute the Modified Time based on changed to the components. */
  itk::ModifiedTimeType GetMTime(void) const;
  
protected:
  GridResampleImageFilter();

  /** Destructor */
  virtual ~GridResampleImageFilter() {};

  virtual void GenerateOutputInformation();

  virtual void GenerateInputRequestedRegion();

  virtual void BeforeThreadedGenerateData();
  
  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

  virtual void AfterThreadedGenerateData();

  inline void CastPixelWithBoundsChecking( const InterpolatorOutputType& value,
                                                      const InterpolatorComponentType& minComponent,
                                                      const InterpolatorComponentType& maxComponent,
                                                      OutputPixelType& outputValue ) const
  {
    // Method imported from itk::ResampleImageFilter
    const unsigned int nComponents = InterpolatorConvertType::GetNumberOfComponents(value);
    
    itk::NumericTraits<OutputPixelType>::SetLength( outputValue, nComponents );
    
    for (unsigned int n=0; n<nComponents; n++)
      {
      InterpolatorComponentType component = InterpolatorConvertType::GetNthComponent( n, value );
      
      if ( m_CheckOutputBounds && component < minComponent )
        {
        OutputPixelConvertType::SetNthComponent( n, outputValue, static_cast<OutputPixelComponentType>( minComponent ) );
        }
      else if ( m_CheckOutputBounds && component > maxComponent )
        {
        OutputPixelConvertType::SetNthComponent( n, outputValue, static_cast<OutputPixelComponentType>( maxComponent ) );
        }
      else
        {
        OutputPixelConvertType::SetNthComponent(n, outputValue,
                                                static_cast<OutputPixelComponentType>( component ) );
        }
      }
  }
  
  
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  GridResampleImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  IndexType              m_OutputStartIndex;     // output image start index
  SizeType               m_OutputSize;            // output image size
  PointType              m_OutputOrigin;         // output image origin
  SpacingType            m_OutputSpacing;        // output image spacing

  OutputPixelType        m_EdgePaddingValue;     // Default pixel value

  bool                   m_CheckOutputBounds;    // Shall we check
                                                 // output bounds when
                                                 // casting?
  
  InterpolatorPointerType m_Interpolator;        // Interpolator used
                                                 // for resampling

  OutputImageRegionType   m_ReachableOutputRegion; // Internal
                                                   // variable for
                                                   // speed-up. Computed
                                                   // in BeforeThreadedGenerateData
  
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGridResampleImageFilter.txx"
#endif

#endif
