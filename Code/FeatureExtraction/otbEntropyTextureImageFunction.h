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
#ifndef __otbEntropyTextureImageFunction_h
#define __otbEntropyTextureImageFunction_h

#include "itkImageFunction.h"
#include "itkNumericTraits.h"


namespace otb
{

/**
 * \class EntropyImageFunction
 * \brief Calculate the energy in the neighborhood of a pixel
 *
 * This class is templated over the input image type and the
 * coordinate representation type (e.g. float or double ).
 *
 * \ingroup ImageFunctions
 */
template <class TInputImage, class TCoordRep = float >
class ITK_EXPORT EntropyTextureImageFunction :
      public itk::ImageFunction< TInputImage, ITK_TYPENAME itk::NumericTraits<typename TInputImage::PixelType>::RealType, TCoordRep >
{
  public:
  /** Standard class typedefs. */
  typedef EntropyTextureImageFunction Self;
  typedef itk::ImageFunction<TInputImage, ITK_TYPENAME itk::NumericTraits<typename TInputImage::PixelType>::RealType,
  TCoordRep > Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(EntropyTextureImageFunction, itk::ImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** typedef support. */
  typedef TInputImage InputImageType;
  typedef typename InputImageType::OffsetType OffsetType;
  typedef typename InputImageType::SizeType  SizeType;
  typedef typename InputImageType::PixelType  PixelType;
  typedef typename Superclass::OutputType OutputType;
  typedef typename Superclass::IndexType IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::PointType PointType;
  typedef typename itk::NumericTraits<typename InputImageType::PixelType>::RealType  RealType;

  /** Dimension of the underlying image. */
  itkStaticConstMacro(ImageDimension, unsigned int,InputImageType::ImageDimension);


  /** Evalulate the function at specified index */
  virtual RealType EvaluateAtIndex( const IndexType& index ) const;

  /** Evaluate the function at non-integer positions */
  virtual RealType Evaluate( const PointType& point ) const
  {
    IndexType index;
    this->ConvertPointToNearestIndex( point, index );
    return this->EvaluateAtIndex( index );
  }
  virtual RealType EvaluateAtContinuousIndex(
    const ContinuousIndexType& cindex ) const
  {
    IndexType index;
    this->ConvertContinuousIndexToNearestIndex( cindex, index );
    return this->EvaluateAtIndex( index ) ;
  }

  /** Get/Set the radius of the neighborhood over which the
      statistics are evaluated */
  itkSetMacro( Radius, SizeType);
  itkGetMacro( Radius, SizeType);
  itkSetMacro( Offset, OffsetType);
  itkGetMacro( Offset, OffsetType );

  protected:
  EntropyTextureImageFunction();
  ~EntropyTextureImageFunction() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  EntropyTextureImageFunction( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  SizeType m_Radius;
  OffsetType m_Offset;

};

} // end namespace otb



#ifndef OTB_MANUAL_INSTANTIATION
# include "otbEntropyTextureImageFunction.txx"
#endif

#endif

