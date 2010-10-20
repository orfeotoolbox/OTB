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
#ifndef __otbLocalHistogramImageFunction_h
#define __otbLocalHistogramImageFunction_h

#include "itkImageFunction.h"
#include "itkFixedArray.h"
#include "itkScalarImageToHistogramGenerator.h"
#include "itkNumericTraits.h"

namespace otb
{

/**
 * \class LocalHistogramImageFunction
 * \brief Calculate the histogram over a specified neighborhood
 *
 *
 * This class is templated over the input image type and the
 * coordinate representation type (e.g. float or double).
 *
 * \ingroup ImageFunctions
 */

template <class TInputImage, class TCoordRep = float >
class ITK_EXPORT LocalHistogramImageFunction :
  public itk::ImageFunction< TInputImage,
                         itk::VariableLengthVector<unsigned int>, TCoordRep >
{
public:
  /** Standard class typedefs. */
  typedef LocalHistogramImageFunction                                     Self;
  typedef itk::ImageFunction< TInputImage,
                    itk::VariableLengthVector<unsigned int>, TCoordRep >  Superclass;
  typedef itk::SmartPointer<Self>                                         Pointer;
  typedef itk::SmartPointer<const Self>                                   ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(LocalHistogramImageFunction, ImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef TInputImage                              InputImageType;
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::PointType           PointType;

  typedef typename Superclass::OutputType          OutputType;
  typedef itk::Statistics::ScalarImageToHistogramGenerator<TInputImage> HistogramGeneratorType;
  typedef typename HistogramGeneratorType::GeneratorType    GeneratorType;
  typedef typename GeneratorType::HistogramType             HistogramType;
  typedef typename HistogramType::Pointer                   HistogramPointer;
  typedef typename GeneratorType::Pointer                   GeneratorPointer;

  /** Dimension of the underlying image. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      InputImageType::ImageDimension);

  /** Evalulate the function at specified index */
  virtual OutputType  EvaluateAtIndex(const IndexType& index) const;

  /** Evaluate the function at non-integer positions */
  virtual OutputType Evaluate(const PointType& point) const
  {
    IndexType index;
    this->ConvertPointToNearestIndex(point, index);
    return this->EvaluateAtIndex(index);
  }
  virtual OutputType EvaluateAtContinuousIndex(
    const ContinuousIndexType& cindex) const
  {
    IndexType index;
    this->ConvertContinuousIndexToNearestIndex(cindex, index);
    return this->EvaluateAtIndex(index);
  }

  /** Get/Set the radius of the neighborhood over which the
   *  statistics are evaluated 
   */
  itkSetMacro( NeighborhoodRadius, unsigned int );
  itkGetConstReferenceMacro( NeighborhoodRadius, unsigned int );

  /** Set/Get the number of histogram bins. Default is 128. */
  itkSetClampMacro( NumberOfHistogramBins, unsigned long, 1, itk::NumericTraits<unsigned long>::max() );
  itkGetConstMacro( NumberOfHistogramBins, unsigned long );

protected:
  LocalHistogramImageFunction();
  virtual ~LocalHistogramImageFunction() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  LocalHistogramImageFunction(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented

  unsigned int     m_NeighborhoodRadius;
  unsigned long    m_NumberOfHistogramBins;

};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLocalHistogramImageFunction.txx"
#endif

#endif

