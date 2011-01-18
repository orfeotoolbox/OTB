/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: otbHAlphaImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:28:51 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbHAlphaImageFilter_h
#define __otbHAlphaImageFilter_h

#include <complex>
#include "math.h"


#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkNumericTraits.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "vnl/vnl_matrix_fixed.h"
#include "vnl/algo/vnl_generalized_eigensystem.h"

#include "itkEigenAnalysis3DImageFilter.h"
#include "itkNthElementImageAdaptor.h"
#include "otbHermitianEigenAnalysis.h"


/** Déclaration des constantes */

#ifndef PI
#define PI 3.14159265358979323846
#endif



namespace otb
 {
/** \class otbHAlphaImageFilter
 * \brief Applies an averaging filter to an image
 *
 * Computes an image where a given pixel is the mean value of the
 * the pixels in a neighborhood about the corresponding input pixel.
 *
 * A mean filter is one of the family of linear filters.  
 *
 * \sa Image
 * \sa Neighborhood
 * \sa NeighborhoodOperator
 * \sa NeighborhoodIterator
 * 
 * \ingroup IntensityImageFilters
 */




template <class TPixel>
class  HAlphaImageFilter :
   public itk::ImageToImageFilter< otb::Image<itk::Vector<TPixel,9>,2>, otb::Image<itk::Vector<TPixel,3>,2> >



{
public:
  /** Convenient typedefs for simplifying declarations. */
  typedef TPixel InputPixelType;
  typedef otb::Image<itk::Vector<TPixel,9>,2> InputImageType;
  typedef otb::Image<itk::Vector<TPixel,3>,2> OutputImageType;

  /** Extract dimension from input and output image. */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      InputImageType::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      OutputImageType::ImageDimension);



 

  /** Standard class typedefs. */
  typedef HAlphaImageFilter Self;
  typedef itk::ImageToImageFilter< InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(HAlphaImageFilter, ImageToImageFilter);
  
  /** Image typedef support. */
   typedef typename InputImageType::PixelType InputPixelType; 
   typedef typename OutputImageType::PixelType OutputPixelType; 
   typedef typename itk::NumericTraits<InputPixelType>::RealType InputRealType; 
  
   typedef typename InputImageType::RegionType InputImageRegionType; 
   typedef typename OutputImageType::RegionType OutputImageRegionType; 

   typedef typename InputImageType::SizeType InputSizeType; 
   typedef typename InputImageType::IndexType IndexType ; 

   

  /** CoherencyMatrix type **/
  typedef itk::Vector<float,9> CoherencyMatrixType;

  /** Vector type used to store eigenvalues. */
  typedef itk::Vector<float, 3> EigenvalueType;

  /** Matrix type used to store eigenvectors. */
  // typedef itk::Vector<itk::Vector<double,2>,3> VectorType;// 3 vecteurs propres partie réelle + parties imaginaire =  6 composantes
  typedef itk::Vector<float, 2> VectorType;
  typedef itk::Vector<VectorType,3> EigenVectorFirstComposantType;
   typedef itk::Vector<VectorType,3> EigenVectorType; // type d'un vecteur propre (partie réelle, partie imaginaire)
   typedef itk::Vector<itk::Vector<float, 6>,3> EigenMatrixType;

  typedef itk::Image<EigenVectorType,2> EigenVectorImageType;
  typedef itk::Image<double,2> EigenValueImageType;

  typedef itk::Vector<double, 3> OutputVectorType;


  typedef itk::Vector<float, 2> ComplexVectorType;
  typedef itk::Vector<ComplexVectorType, 3> HermitianVectorType;
  typedef itk::Vector<HermitianVectorType,3> HermitianMatrixType;
   

 


  /** Set the radius of the neighborhood used to compute the mean. */
  itkSetMacro(Radius, InputSizeType);

  /** Get the radius of the neighborhood used to compute the mean */
  itkGetConstReferenceMacro(Radius, InputSizeType);


  
  /** otbHAlphaImageFilter needs a larger input requested region than
   * the output requested region.  As such, otbHAlphaImageFilter needs
   * to provide an implementation for GenerateInputRequestedRegion()
   * in order to inform the pipeline execution model.
   *
   * \sa ImageToImageFilter::GenerateInputRequestedRegion() */
  virtual void GenerateInputRequestedRegion() throw(itk::InvalidRequestedRegionError);

protected:
 /**class typedefs for eigenvectors / eigenvalues */
 /*  typedef itk::EigenAnalysis3DImageFilter<InputImageType,EigenValueImageType, EigenVectorImageType> EigenAnalysisType; */
  
  typedef otb::HermitianEigenAnalysis<CoherencyMatrixType,EigenvalueType, EigenMatrixType> HermitianAnalysisType;
  
  HAlphaImageFilter();
  virtual ~HAlphaImageFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** otbHAlphaImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData()
   * routine which is called for each processing thread. The output
   * image data is allocated automatically by the superclass prior to
   * calling ThreadedGenerateData().  ThreadedGenerateData can only
   * write to the portion of the output image specified by the
   * parameter "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData() */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId );

private:
  HAlphaImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  InputSizeType m_Radius;
  CoherencyMatrixType m_CoherencyMatrix;
  EigenvalueType m_Eigenvalues;
  EigenVectorFirstComposantType m_Eigenvectors; //récupération de la première composante de chaque vecteur propre;
  float m_Entropie;
  float m_Alpha;
  float m_Anisotropie;
  

};
  
} // end namespace otb

#ifndef ITK_MANUAL_INSTANTIATION
#include "otbHAlphaImageFilter.txx"
#endif

#endif
