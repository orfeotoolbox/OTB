/*=========================================================================
	
  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbSimpleRcsPanSharpeningFusionImageFilter_h
#define __otbSimpleRcsPanSharpeningFusionImageFilter_h

#include "itkImageToImageFilter.h"
#include "otbConvolutionImageFilter.h"
#include "itkDivideImageFilter.h"
#include "itkMultiplyImageFilter.h"
    
namespace otb {
  /**
   * \class SimpleRcsPanSharpeningFusionImageFilter
   * \brief This class performs a simple Pan sharpening operation
   * 
   * Given a Pan image and the corresponding Xs image (oversampled to have the
   * same number of pixels), this filter realizes a simple Pan sharpening 
   * operation:
   * 
   * \f[ \frac{XS}{\mathrm{Filtered}(PAN)} PAN  \f]
   *
   * \ingroup Streamed
   * \ingroup Multithreaded
   * \ingroup Fusion
   *
   *
   * \example Fusion/PanSharpeningExample.cxx
   *
   **/

  template <class TPanImageType,class TXsImageType,class TOutputImageType>
class ITK_EXPORT SimpleRcsPanSharpeningFusionImageFilter :
    public itk::ImageToImageFilter<TXsImageType, TOutputImageType>
      {
        public:
          typedef SimpleRcsPanSharpeningFusionImageFilter   Self;
          typedef itk::ImageToImageFilter
              <TXsImageType, TOutputImageType> Superclass;
          typedef itk::SmartPointer<Self>             Pointer;
          typedef itk::SmartPointer<const Self>       ConstPointer;
          typedef otb::Image<double,2>                InternalImageType;
          typedef otb::VectorImage<double>            InternalVectorImageType;
          
          typedef typename InternalImageType::PixelType InternalPixelType;
          typedef typename itk::NumericTraits<InternalPixelType>::RealType InternalRealType;
          typedef typename itk::Array<InternalRealType> ArrayType;
          
          /** Method for creation through object factory */
          itkNewMacro(Self);

          /** Run-time type information */
          itkTypeMacro(SimpleRcsPanSharpeningFusionImageFilter, 
                       itk::ImageToImageFilter);

          /** Display */
          void PrintSelf( std::ostream& os, itk::Indent indent ) const;
          
          typedef typename InternalImageType::SizeType RadiusType;
          
          /** Set the filter radius  */
          itkGetMacro( Radius, RadiusType);
          itkSetMacro( Radius, RadiusType);
          
          /** Set the input filter */
          itkSetMacro(Filter, ArrayType);
          itkGetConstReferenceMacro(Filter, ArrayType);
          
          virtual void SetPanInput( const TPanImageType * image);
          const TPanImageType * GetPanInput(void) const;
             
          virtual void SetXsInput( const TXsImageType * path);
          const TXsImageType * GetXsInput(void) const;
          
        protected:

          SimpleRcsPanSharpeningFusionImageFilter();

          typedef otb::ConvolutionImageFilter
              <TPanImageType, InternalImageType> ConvolutionFilterType;
          typedef itk::DivideImageFilter
              <TXsImageType,InternalImageType,
              InternalVectorImageType> DivideFilterType;
          typedef itk::MultiplyImageFilter
              <InternalVectorImageType,TPanImageType,TOutputImageType> MultiplyFilterType;
              

//  Software Guide : EndCodeSnippet

                  void GenerateData();

        private:

          SimpleRcsPanSharpeningFusionImageFilter(Self&);   // intentionally not implemented
          void operator=(const Self&);          // intentionally not implemented

          typename ConvolutionFilterType::Pointer     m_ConvolutionFilter;
          typename DivideFilterType::Pointer    m_DivideFilter;
          typename MultiplyFilterType::Pointer     m_MultiplyFilter;

          RadiusType m_Radius;
          ArrayType m_Filter;
          
      };
      
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSimpleRcsPanSharpeningFusionImageFilter.txx"
#endif

#endif

