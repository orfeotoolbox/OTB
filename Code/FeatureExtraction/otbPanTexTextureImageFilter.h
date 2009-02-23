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
#ifndef __otbPanTexTextureImageFilter_h
#define __otbPanTexTextureImageFilter_h

#include "otbUnaryFunctorNeighborhoodWithOffsetImageFilter.h"
#include "otbPanTexTextureFunctor.h"
#include "itkConstNeighborhoodIterator.h"

namespace otb
{

/** \class PanTexTextureImageFilter
  * \brief This filter computes the panchromatic Texture measure.
  *
  * It is the min vamlue of the contrast in 8 directions.
  *
  * \sa ContrastTextureFunctor
  *
  */

template <class TInputImage, class TOutputImage>
class ITK_EXPORT PanTexTextureImageFilter :
public UnaryFunctorNeighborhoodWithOffsetImageFilter< TInputImage, TOutputImage, ITK_TYPENAME Functor::PanTexTextureFunctor< ITK_TYPENAME itk::ConstNeighborhoodIterator<TInputImage>, ITK_TYPENAME TOutputImage::PixelType> >
{
public:
  /** Standard class typedefs. */
  typedef PanTexTextureImageFilter                      Self;
  typedef UnaryFunctorNeighborhoodWithOffsetImageFilter<TInputImage,
                                                        TOutputImage,
                                                        typename Functor::PanTexTextureFunctor<typename itk::ConstNeighborhoodIterator<TInputImage>,
                                                                                               typename TOutputImage::PixelType>                    > Superclass;
  typedef itk::SmartPointer<Self>                       Pointer;
  typedef itk::SmartPointer<const Self>                 ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(PanTexTextureImageFilter,UnaryFunctorNeighborhoodWithOffsetImageFilter);


  /** Some convenient typedefs. */
  typedef TInputImage                                             InputImageType;
  typedef TOutputImage                                            OutputImageType;
  typedef typename itk::ConstNeighborhoodIterator<InputImageType> NeighborhoodIteratorType;
  typedef typename OutputImageType::PixelType                     OutputPixelType;

  typedef typename InputImageType::RegionType                     InputImageRegionType;
  typedef typename InputImageType::PixelType                      InputImagePixelType;
  typedef typename InputImageType::SizeType                       InputImageSizeType;
  typedef typename InputImageType::OffsetType                     InputImageOffsetType;
  typedef typename OutputImageType::Pointer                       OutputImagePointer;
  typedef typename OutputImageType::RegionType                    OutputImageRegionType;
  typedef typename OutputImageType::PixelType                     OutputImagePixelType;
  typedef typename Superclass::RadiusType                         RadiusType;
  typedef typename Superclass::InputImageOffsetType               OffsetType;

  typedef itk::ProcessObject ProcessObjectType;


protected:

  PanTexTextureImageFilter();
  virtual ~PanTexTextureImageFilter() {};

  /** UnaryFunctorNeighborhoodWithOffsetImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData() routine
   * which is called for each processing thread. The output image data is
   * allocated automatically by the superclass prior to calling
   * ThreadedGenerateData().  ThreadedGenerateData can only write to the
   * portion of the output image specified by the parameter
   * "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, int threadId );


private:

  PanTexTextureImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  //Offset is setted to [2,2] in the publication
  virtual void SetOffset(OffsetType off)
    {
      Superclass::SetOffset(off);
    };
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPanTexTextureImageFilter.txx"
#endif



#endif
