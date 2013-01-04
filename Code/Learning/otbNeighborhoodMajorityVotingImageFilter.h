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
#ifndef __otbNeighborhoodMajorityVotingImageFilter_h
#define __otbNeighborhoodMajorityVotingImageFilter_h

// First make sure that the configuration is available.
// This line can be removed once the optimized versions
// gets integrated into the main directories.
#include "itkConfigure.h"

#include "itkMorphologyImageFilter.h"
#include "itkBinaryBallStructuringElement.h"

namespace otb {

/**
 * \class NeighborhoodMajorityVotingImageFilter
 * \Majority Voting of an image
 *
 * Filters a labeled image using Majority Voting in a specified neighbordhood. Majority Voting takes the
 * more representative value of all the pixels identified by the structuring element and then sets the
 * center pixel to this label value.
 *
 * The structuring element is assumed to be composed of binary
 * values (zero or one). Only elements of the structuring element
 * having values > 0 are candidates for affecting the center pixel.
 *
 * For the each input image pixel,
 *   - NeighborhoodIterator gives neighbors of the pixel.
 *   - Evaluate() member function returns the more representative label value among
 *     the image neighbors where the kernel has elements > 0.
 *   - Replace the original label value with the more representative label value
 *
 * \sa MorphologyImageFilter, GrayscaleFunctionDilateImageFilter, BinaryDilateImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */


/*
template<class TInputImage, class TOutputImage, class TKernel>
class ITK_EXPORT NeighborhoodMajorityVotingImageFilter :
    public itk::MorphologyImageFilter<TInputImage, TOutputImage, TKernel>
{*/

template<class TInputImage>
class ITK_EXPORT NeighborhoodMajorityVotingImageFilter :
    public itk::MorphologyImageFilter<
    TInputImage,
    TInputImage,
    itk::BinaryBallStructuringElement< typename TInputImage::PixelType, TInputImage::ImageDimension >
    >
{
public:
  /** Standard class typedefs. */
  typedef NeighborhoodMajorityVotingImageFilter Self;
  //typedef itk::MorphologyImageFilter<TInputImage, TOutputImage, TKernel> Superclass;
  typedef itk::MorphologyImageFilter<
                  TInputImage,
                  TInputImage,
                  itk::BinaryBallStructuringElement< typename TInputImage::PixelType, TInputImage::ImageDimension >
                  > Superclass;

  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(NeighborhoodMajorityVotingImageFilter,
                itk::MorphologyImageFilter);

  /** Declaration of pixel type. */
  typedef typename Superclass::PixelType PixelType;

  /** Kernel (structuring element) definition. */
  typedef typename itk::BinaryBallStructuringElement< typename TInputImage::PixelType, TInputImage::ImageDimension > BallStructuringType;

  /** Kernel (structuring element) iterator. */
  typedef typename Superclass::KernelIteratorType  KernelIteratorType;

  /** Neighborhood iterator type. */
  typedef typename Superclass::NeighborhoodIteratorType NeighborhoodIteratorType;

  /** Kernel typedef. */
  typedef typename Superclass::KernelType KernelType;

  /** Radius typedef. */
  typedef typename Superclass::RadiusType RadiusType;

  /** Default boundary condition type */
  typedef typename Superclass::DefaultBoundaryConditionType DefaultBoundaryConditionType;

  /** ImageDimension constant */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** NeighborhoodDimension constant */
  itkStaticConstMacro(KernelDimension, unsigned int,
                                           BallStructuringType::NeighborhoodDimension);
  

  /** Type of the pixels in the Kernel. */
  typedef typename BallStructuringType::PixelType            KernelPixelType;


#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(SameDimensionCheck,
    (itk::Concept::SameDimension<InputImageDimension, KernelDimension>));
  itkConceptMacro(InputGreaterThanComparableCheck,
    (itk::Concept::GreaterThanComparable<PixelType>));
  itkConceptMacro(KernelGreaterThanComparableCheck,
    (itk::Concept::GreaterThanComparable<KernelPixelType>));
  /** End concept checking */
#endif


  //Creates a SetNoDataValue method
  virtual void SetNoDataValue(const PixelType _arg)
  {
         itkDebugMacro("setting NoDataValue to " << _arg);
         if (this->m_NoDataValue != _arg)
              {
              this->m_NoDataValue = _arg;

              m_MajorityVotingBoundaryCondition.SetConstant(m_NoDataValue);
              this->OverrideBoundaryCondition(&m_MajorityVotingBoundaryCondition);

              this->Modified();
              }
  }

  //Creates a SetUndefinedValue method
  itkSetMacro(UndefinedValue, PixelType);

  //Creates a SetRadiusNeighborhood method
  /** Set kernel (structuring element). */
  virtual void SetRadiusNeighborhood(const RadiusType _arg)
  {
         itkDebugMacro("setting RadiusNeighborhood to " << _arg);
         if (this->m_RadiusNeighborhood != _arg)
              {
              this->m_RadiusNeighborhood = _arg;

              //Kernel Setting
              BallStructuringType seBall;
              seBall.SetRadius(m_RadiusNeighborhood);
              seBall.CreateStructuringElement();
              this->SetKernel(seBall);

              this->Modified();
              }
  }


protected:
  NeighborhoodMajorityVotingImageFilter();
  ~NeighborhoodMajorityVotingImageFilter() {};

  /** Evaluate image neighborhood with kernel to find the new value
   * for the center pixel value
   *
   * It will return the more representative label value of the labeled image pixels whose corresponding
   * element in the structuring element is positive. This version of
   * Evaluate is used for non-boundary pixels. */
  PixelType Evaluate(const NeighborhoodIteratorType &nit,
                     const KernelIteratorType kernelBegin,
                     const KernelIteratorType kernelEnd);


private:
  NeighborhoodMajorityVotingImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  // Default boundary condition for majority voting filter, defaults to
  DefaultBoundaryConditionType m_MajorityVotingBoundaryCondition;

  PixelType m_NoDataValue;
  PixelType m_UndefinedValue;
  RadiusType m_RadiusNeighborhood;

}; // end of class

} // end namespace otb

#ifndef ITK_MANUAL_INSTANTIATION
#include "otbNeighborhoodMajorityVotingImageFilter.txx"
#endif


#endif
