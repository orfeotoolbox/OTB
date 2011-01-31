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
#ifndef __otbHistogramOfOrientedGradientCovariantImageFunction_h
#define __otbHistogramOfOrientedGradientCovariantImageFunction_h

#include "itkImageFunction.h"
#include "itkFixedArray.h"

namespace otb
{

/**
 * \class HistogramOfOrientedGradientCovariantImageFunction
 * \brief Calculate the centered HOG features
 *
 * This filter implements the centered histogram of gradient
 * feature. It expects a gradient covariant image as input, such as
 * the output of the itk::GradientImageFilter. Steps of the algorithm
 * are as follows.
 *
 * In order to make the C-HOG descriptor rotation-invariant, a
 * principal gradient orientation is first looked for. Within the
 * m_NeighborhoodRadius ($r$), an histogram of the local orientation is
 * computed with m_NumberOfOrientationBins bins. Values cumulated in
 * this histogram are the gradient magnitude weighted by a gaussian
 * kernel of $\sigma = 0.5 * r$.
 *
 * From this orientation histogram, the principal orientation is
 * computed by looking for the maximum valued bin.
 *
 * Once principal orientation is computed,
 * gradient magnitude values weighted by the gaussian kernel are
 * cumulated in five different histograms corresponding to five
 * distinct spatial areas : the center area (radius of the center area
 * is computed using $r_{c}=log_{2}(r)$), and the upper-left,
 * upper-right, lower-left and lower-right radial areas. Orientation
 * of these radial areas is shifted to match the principal
 * orientation, and gradient orientations to determine histogram bins
 * are also compensated with the principal orientation. Last, each
 * histogram is normalized by its $L_2$ norm, ensuring that they
 * all lie in the range $[0, 1]$.
 *
 * This class is templated over the input image type, the output
 * precision (e.g. float ou double) and the
 * coordinate representation type (e.g. float or double).

 * \ingroup ImageFunctions
 */

template <class TInputImage, class TOutputPrecision = double, class TCoordRep = double >
class ITK_EXPORT HistogramOfOrientedGradientCovariantImageFunction :
  public itk::ImageFunction< TInputImage, std::vector< std::vector<
       TOutputPrecision > >, TCoordRep >
{
public:
  /** Standard class typedefs. */
  typedef HistogramOfOrientedGradientCovariantImageFunction              Self;
  typedef itk::ImageFunction< TInputImage, std::vector<std::vector
    <TOutputPrecision> >, TCoordRep >                                    Superclass;
  typedef itk::SmartPointer<Self>                                        Pointer;
  typedef itk::SmartPointer<const Self>                                  ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(HistogramOfOrientedGradientCovariantImageFunction, ImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef TInputImage                              InputImageType;
  typedef typename InputImageType::PixelType       InputPixelType;
  typedef typename Superclass::IndexType           IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::PointType           PointType;

  typedef TOutputPrecision                         OutputPrecisionType;
  typedef typename Superclass::OutputType          OutputType;

  typedef TCoordRep                                CoordRepType;

  /** Dimension of the underlying image. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      InputImageType::ImageDimension);

  /** Evalulate the function at specified index */
  virtual OutputType EvaluateAtIndex(const IndexType& index) const;

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

  /** Get/Set the number of bins of the orientation histograms
   */
  itkSetMacro( NumberOfOrientationBins, unsigned int );
  itkGetConstReferenceMacro( NumberOfOrientationBins, unsigned int );


protected:
  HistogramOfOrientedGradientCovariantImageFunction();
  virtual ~HistogramOfOrientedGradientCovariantImageFunction() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  HistogramOfOrientedGradientCovariantImageFunction(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented

  // Radius over which the principal orientation will be computed
  unsigned int m_NeighborhoodRadius;

  // Number of bins in the orientation
  unsigned int m_NumberOfOrientationBins;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbHistogramOfOrientedGradientCovariantImageFunction.txx"
#endif

#endif

