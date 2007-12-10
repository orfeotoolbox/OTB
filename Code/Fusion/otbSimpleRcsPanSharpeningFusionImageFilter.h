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
#include "itkMeanImageFilter.h"
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
   * 
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
          
          
          /** Method for creation through object factory */
          itkNewMacro(Self);

          /** Run-time type information */
          itkTypeMacro(SimpleRcsPanSharpeningFusionImageFilter, 
                       itk::ImageToImageFilter);

          /** Display */
          void PrintSelf( std::ostream& os, itk::Indent indent ) const;
          
          typedef typename InternalImageType::SizeType RadiusType;

          itkGetMacro( Radius, RadiusType);
          itkSetMacro( Radius, RadiusType);
          
          virtual void SetPanInput( const TPanImageType * image);
          const TPanImageType * GetPanInput(void);
             
          virtual void SetXsInput( const TXsImageType * path);
          const TXsImageType * GetXsInput(void);
          
        protected:

          SimpleRcsPanSharpeningFusionImageFilter();

          typedef itk::MeanImageFilter
              <TPanImageType, InternalImageType> MeanFilterType;
          typedef itk::DivideImageFilter
              <InternalVectorImageType,InternalImageType,
              InternalVectorImageType> DivideFilterType;
          typedef itk::MultiplyImageFilter
              <InternalVectorImageType,TPanImageType,TOutputImageType> MultiplyFilterType;
              

//  Software Guide : EndCodeSnippet

                  void GenerateData();

        private:

          SimpleRcsPanSharpeningFusionImageFilter(Self&);   // intentionally not implemented
          void operator=(const Self&);          // intentionally not implemented

          typename MeanFilterType::Pointer     m_MeanFilter;
          typename DivideFilterType::Pointer    m_DivideFilter;
          typename MultiplyFilterType::Pointer     m_MultiplyFilter;

          RadiusType m_Radius;

      };
      
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSimpleRcsPanSharpeningFusionImageFilter.txx"
#endif

#endif

