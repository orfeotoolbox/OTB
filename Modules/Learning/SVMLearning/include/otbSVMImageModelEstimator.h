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
#ifndef __otbSVMImageModelEstimator_h
#define __otbSVMImageModelEstimator_h

#include "vcl_deprecated_header.h"

#include "itkImageModelEstimatorBase.h"
#include "itkImageRegionIterator.h"
#include "otbSVMModelEstimator.h"

namespace otb
{

/** \class SVMImageModelEstimator
 * \brief Class for SVM model estimation from images used for classification.
 *
 *
 * The basic functionality of the SVMImageModelEstimator is to
 * generate the models used in SVM classification. It
 * requires input images and a training image to be provided by the
 * user. This object supports data handling of multiband images. The
 * object accepts the input image as a VectorImage only, where each
 * pixel is a vector and each element of the vector corresponds to an
 * entry from 1 particular band of a multiband dataset. A single band
 * image is treated as a vector image with a single element for every
 * vector. The classified image is treated as a single band scalar
 * image.
 *
 * \deprecated
 *
 * \sa MachineLearningModel
 * \sa LibSVMMachineLearningModel
 * \sa ImageClassificationFilter
 *
 * \ingroup ClassificationFilters
 */
template <class TInputImage, class TTrainingImage>
class ITK_EXPORT SVMImageModelEstimator :
  public SVMModelEstimator<typename TInputImage::InternalPixelType, typename TTrainingImage::PixelType>
{
public:
  /** Standard class typedefs. */
  typedef SVMImageModelEstimator Self;
  typedef SVMModelEstimator<typename TInputImage::PixelType::ComponentType,
      typename TTrainingImage::PixelType>
  Superclass;

  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SVMImageModelEstimator, otb::SVMModelEstimator);

  /** Type definition for the input image. */
  typedef typename TInputImage::Pointer InputImagePointer;

  /** Type definitions for the training image. */
  typedef typename TTrainingImage::Pointer TrainingImagePointer;

  /** Type definition for the vector associated with
   * input image pixel type. */
  typedef typename TInputImage::PixelType InputImagePixelType;

  /** Type definitions for the vector holding
   * training image pixel type. */
  typedef typename TTrainingImage::PixelType TrainingImagePixelType;

  /** Type definitions for the iterators for the input and training images. */
  typedef
  itk::ImageRegionIterator<TInputImage>  InputImageIterator;
  typedef
  itk::ImageRegionIterator<TTrainingImage> TrainingImageIterator;

  /** Set the input image */
  void SetInputImage(const TInputImage * inputImage);

  /** Set the training image */
  void SetTrainingImage(const TTrainingImage * trainingImage);

  /** Get the input image */
  const TInputImage * GetInputImage();

  /** Get the training image */
  const TTrainingImage * GetTrainingImage();

protected:
  /** Constructor */
  SVMImageModelEstimator();
  /** Destructor */
  virtual ~SVMImageModelEstimator();
  /** PrintSelf */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Implement the prepare data step from SVMModelEstimator */
  virtual void PrepareData();

private:
  SVMImageModelEstimator(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

}; // class SVMImageModelEstimator

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSVMImageModelEstimator.txx"
#endif

#endif
