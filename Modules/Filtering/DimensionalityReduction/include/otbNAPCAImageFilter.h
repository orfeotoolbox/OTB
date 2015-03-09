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
#ifndef __otbNAPCAImageFilter_h
#define __otbNAPCAImageFilter_h

#include "otbMNFImageFilter.h"

namespace otb {

/** \class NAPCAImageFilter
 * \brief Performs a Noise Adjusted PCA analysis of a vector image.
 *
 * The internal structure of this filter is a filter-to-filter like structure.
 * The estimation of the covariance matrix has persistent capabilities...
 *
 * The high pass filter which has to be used for the noise estimation is templated
 * for a better scalability...
 *
 * This class is very similar to the MNFImageFilter one since only the projection
 * matrix definition (through GenerateTransformationMatrix function)
 * differs.
 *
 * TODO? Utiliser une 2e entree pour donner directement une image de bruit ??
 *
 * \sa otbStreamingStatisticsVectorImageFilter
 * \sa MNFImageFilter
 *
 * \ingroup OTBDimensionalityReduction
 */
template <class TInputImage, class TOutputImage,
            class TNoiseImageFilter,
            Transform::TransformDirection TDirectionOfTransformation >
class ITK_EXPORT NAPCAImageFilter
  : public MNFImageFilter< TInputImage, TOutputImage, TNoiseImageFilter, TDirectionOfTransformation >
{
public:
  typedef NAPCAImageFilter Self;
  typedef MNFImageFilter< TInputImage, TOutputImage,
            TNoiseImageFilter, TDirectionOfTransformation > Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(NAPCAImageFilter, MNFImageFilter);

  /** Template parameters typedefs */
  typedef typename Superclass::InputImageType InputImageType;
  typedef typename Superclass::OutputImageType OutputImageType;

  typedef typename Superclass::CovarianceEstimatorFilterType CovarianceEstimatorFilterType;
  typedef typename Superclass::CovarianceEstimatorFilterPointerType CovarianceEstimatorFilterPointerType;

  typedef typename Superclass::RealType RealType;
  typedef typename Superclass::VectorType VectorType;
  typedef typename Superclass::MatrixObjectType MatrixObjectType;
  typedef typename Superclass::MatrixType MatrixType;
  typedef typename Superclass::InternalMatrixType InternalMatrixType;
  typedef typename Superclass::MatrixElementType MatrixElementType;

  typedef typename Superclass::TransformFilterType TransformFilterType;
  typedef typename Superclass::TransformFilterPointerType TransformFilterPointerType;

  typedef typename Superclass::NoiseImageFilterType NoiseImageFilterType;
  typedef typename Superclass::NoiseImageFilterPointerType NoiseImageFilterPointerType;

  typedef typename Superclass::NormalizeFilterType NormalizeFilterType;
  typedef typename Superclass::NormalizeFilterPointerType NormalizeFilterPointerType;

protected:
  NAPCAImageFilter() { }
  virtual ~NAPCAImageFilter () { }

  /** Specific functionality of NAPCA */
  virtual void GenerateTransformationMatrix();
}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbNAPCAImageFilter.txx"
#endif

#endif

