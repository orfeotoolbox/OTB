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
#ifndef __otbEstimateInnerProductPCAImageFilter_h
#define __otbEstimateInnerProductPCAImageFilter_h

#include "itkImageToImageFilter.h"
#include "otbStreamingInnerProductVectorImageFilter.h"

namespace otb
{
/** \class EstimateInnerProductPCAImageFilter
 * \brief Applies the Inner product to an vector image.
 * Implement the itk::ImagePCAShapeModelEstimator algorithms in streaming capabilities
 * Use by the  InnerProductPCAImageFilter composite filter
 *
 * \ingroup OTBDimensionalityReduction
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT EstimateInnerProductPCAImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef EstimateInnerProductPCAImageFilter                           Self;
  typedef typename itk::ImageToImageFilter <TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                                      Pointer;
  typedef itk::SmartPointer<const Self>                                ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(EstimateInnerProductPCAImageFilter, ImageToImageFilter);

  /** Some convenient typedefs. */
  typedef          TInputImage                        InputImageType;
  typedef typename InputImageType::Pointer            InputImagePointer;
  typedef typename InputImageType::PixelType          InputPixelType;
  typedef typename InputImageType::RegionType         InputImageRegionType;
  typedef          TOutputImage                       OutputImageType;
  typedef typename OutputImageType::Pointer           OutputImagePointer;
  typedef typename OutputImageType::RegionType        OutputImageRegionType;
  typedef typename OutputImageType::PixelType         OutputPixelType;
  typedef typename OutputImageType::InternalPixelType OutputInternalPixelType;

  typedef StreamingInnerProductVectorImageFilter<InputImageType> StreamingInnerProductType;
  typedef typename StreamingInnerProductType::Pointer            StreamingInnerProductPointer;
  typedef typename StreamingInnerProductType::MatrixType         MatrixType;
  typedef typename StreamingInnerProductType::MatrixObjectType   MatrixObjectType;

  /** Set/Get the number of required largest principal components. The filter produces
   the required number of principal components plus one outputs. Output index 0 represents
   the mean image and the remaining outputs the requested principal components.*/
  itkSetMacro(NumberOfPrincipalComponentsRequired, unsigned int);
  itkGetMacro(NumberOfPrincipalComponentsRequired, unsigned int);

  /** Enable/Disable center data */
  itkSetMacro(CenterData, bool);
  itkGetMacro(CenterData, bool);
  itkBooleanMacro(CenterData);

protected:
  EstimateInnerProductPCAImageFilter();
  virtual ~EstimateInnerProductPCAImageFilter() {}

  /** GenerateOutputInformation
   * Set the number of bands of the output.
   * Copy information from the first image of the list if existing.
   **/
  virtual void GenerateOutputInformation(void);

  /**
   * BeforeThreadedGenerateData
   **/
  virtual void BeforeThreadedGenerateData();

  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  EstimateInnerProductPCAImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** The number of output Pricipal Components */
  unsigned int m_NumberOfPrincipalComponentsRequired;

  /** Enable/Disable center data */
  bool m_CenterData;

  /** Inner Product Matrix pre computed */
  MatrixType m_InnerProduct;

  /** Contains Eigen Vectors Of Inner Product Matrix computed */
  MatrixType m_EigenVectorsOfInnerProductMatrix;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbEstimateInnerProductPCAImageFilter.txx"
#endif

#endif
