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
#ifndef __otbAngularProjectionImageFilter_h
#define __otbAngularProjectionImageFilter_h

#include "itkImageToImageFilter.h"


namespace otb {
/** \class AngularProjectionImageFilter
 * \brief Performs spherical transformation in ND space
 *
 * This class performs the projections of the N input images according 
 * to angular value through the generalized spherical projection.
 *
 * The angle value are considered to come from a itk::FixedArray...
 *
 * \ingroup Threaded
 */
template < class TInputImage, class TOutputImage, class TAngleArray, class TPrecision >
class ITK_EXPORT AngularProjectionImageFilter
  : public itk::ImageToImageFilter< TInputImage, TOutputImage >
{
public:
  /** Standard typedefs */
  typedef AngularProjectionImageFilter Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>            Pointer;
  typedef itk::SmartPointer<const Self>      ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(AngularProjectionImageFilter, ImageToImageFilter);

  /** Dimension */
  itkStaticConstMacro(InputImageDimension,  unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);
  itkStaticConstMacro(NumberOfInputImages,  unsigned int, TAngleArray::Dimension+1);

  /** Template parameters typedefs */
  typedef TInputImage                         InputImageType;
  typedef typename InputImageType::Pointer    InputImagePointerType;
  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename InputImageType::SizeType   InputSizeType;
  typedef typename InputImageType::IndexType  InputIndexType;
  typedef typename InputImageType::PixelType  InputPixelType;

  typedef TOutputImage                         OutputImageType;
  typedef typename OutputImageType::PixelType  OutputImagePixelType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename OutputImageType::Pointer    OutputImagePointerType;

  typedef TPrecision PrecisionType;
  typedef TAngleArray AngleArrayType;
  typedef typename AngleArrayType::ValueType AngleValueType;

  typedef itk::ImageRegionConstIterator<InputImageType> ImageRegionConstIteratorType;
  typedef std::vector< ImageRegionConstIteratorType >   ImageRegionConstIteratorVectorType;

  /** Set/Get Angle set */
  itkGetMacro(AngleArray,AngleArrayType);
  itkSetMacro(AngleArray,AngleArrayType);

  void SetInput ( unsigned int i, const InputImageType * );
  const InputImageType * GetInput( unsigned int i ) const;



protected:
  AngularProjectionImageFilter();
  virtual ~AngularProjectionImageFilter() { }

  virtual void ThreadedGenerateData( const OutputImageRegionType & outputRegionForThread, int threadID );
  virtual OutputImagePixelType GenerateData ( const ImageRegionConstIteratorVectorType & ) const;

private:
  AngularProjectionImageFilter(const Self&); // not implemented
  void operator=(const Self&); // not implemented

  AngleArrayType m_AngleArray;

}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbAngularProjectionImageFilter.txx"
#endif


#endif

