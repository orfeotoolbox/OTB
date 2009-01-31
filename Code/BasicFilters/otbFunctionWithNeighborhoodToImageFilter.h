/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbFunctionWithNeighborhoodToImageFilter_h
#define __otbFunctionWithNeighborhoodToImageFilter_h

#include "otbFunctionToImageFilter.h"






namespace otb
{

/** \class FunctionWithNeighborhoodToImageFilter
  * \brief Evaluates a ImageFunction onto a source image
 *
 * The class walks an input image and evaluates
 * the function at every pixel location. The output of the spatial function
 * and the pixel type of the output image must be compatible.
 *
 * Like its parent ImageToImageFilter, this class functions in the filtering
 * pipeline and produces a unique output image.
 *
 * The function hs to inherite from itkImageFunction
 *
 * \sa SpatialFunctionImageEvaluatorFilter
 * \sa SpatialFunctionImageFilter
 * \sa ImageFunction
 * \ingroup ImageFilters
 */

template <class TInputImage, class TOutputImage,class TFunction    >
class ITK_EXPORT FunctionWithNeighborhoodToImageFilter :
      public FunctionToImageFilter<TInputImage,TOutputImage,TFunction>
{
public:
  /** Standard class typedefs. */
  typedef FunctionWithNeighborhoodToImageFilter  Self;
  typedef FunctionToImageFilter<TInputImage,TOutputImage,TFunction>   Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(FunctionWithNeighborhoodToImageFilter, FunctionToImageFilter);

  /** Some typedefs. */
  /** Image size typedef. */
  typedef TInputImage                           InputImageType;
  typedef typename InputImageType::ConstPointer InputImageConstPointer;
  typedef typename InputImageType::Pointer      InputImagePointer;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef typename InputImageType::SizeType     InputImageSizeType;
  typedef typename InputImageType::OffsetType   InputImageOffsetType;
  typedef TOutputImage                          OutputImageType;
  typedef typename OutputImageType::Pointer     OutputImagePointer;
  typedef typename OutputImageType::RegionType  OutputImageRegionType;
  typedef typename OutputImageType::PixelType   OutputImagePixelType;
  /** Type of function. */
  typedef TFunction                             FunctionType;
  typedef typename FunctionType::OutputType     FunctionValueType;
  typedef typename FunctionType::InputType      FunctionPositionType;

  /** Connect one of the operands for pixel-wise addition. */
  //void SetInput( const TInputImage *image);

  /** Set the internal spatial function. */
  void SetFunction( FunctionType* PixelFunction )
  {
    Superclass::SetFunction( PixelFunction );
    //m_Radius = this->GetFunction()->GetRadius();
    //m_Offset = this->GetFunction()->GetOffset();
    this->Modified();
  };

  /** Image dimensions */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Accessors */
  itkGetMacro(Radius, InputImageSizeType);
  itkGetMacro(Offset, InputImageOffsetType);

protected:
  FunctionWithNeighborhoodToImageFilter();
  virtual ~FunctionWithNeighborhoodToImageFilter() {};

  void BeforeThreadedGenerateData();

  void GenerateInputRequestedRegion();

  /** SpatialFunctionImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData() routine
   * which is called for each processing thread. The output image data is
   * allocated automatically by the superclass prior to calling
   * ThreadedGenerateData().  ThreadedGenerateData can only write to the
   * portion of the output image specified by the parameter
   * "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId );

private:
  FunctionWithNeighborhoodToImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented


  InputImageSizeType m_Radius;
  InputImageOffsetType m_Offset;
  //std::vector<FunctionType*> m_FunctionVector;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFunctionWithNeighborhoodToImageFilter.txx"
#endif

#endif
