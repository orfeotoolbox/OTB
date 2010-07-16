/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMaskedScalarImageToGreyLevelRunLengthMatrixGenerator.h,v $
  Language:  C++
  Date:      $Date: 2009/03/23 18:13:26 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMaskedScalarImageToGreyLevelRunLengthMatrixGenerator_h
#define __itkMaskedScalarImageToGreyLevelRunLengthMatrixGenerator_h

#include "itkScalarImageToGreyLevelRunLengthMatrixGenerator.h"

namespace itk {
  namespace Statistics {

/*
*/
    
template< class TImageType,
          class THistogramFrequencyContainer = DenseFrequencyContainer >
class MaskedScalarImageToGreyLevelRunLengthMatrixGenerator : 
    public ScalarImageToGreyLevelRunLengthMatrixGenerator< TImageType, 
      THistogramFrequencyContainer >
  {
  public:
    /** Standard typedefs */
    typedef MaskedScalarImageToGreyLevelRunLengthMatrixGenerator Self;
    typedef ScalarImageToGreyLevelRunLengthMatrixGenerator< TImageType, 
      THistogramFrequencyContainer > Superclass;
    typedef SmartPointer<Self> Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(MaskedScalarImageToGreyLevelRunLengthMatrixGenerator, 
      ScalarImageToGreyLevelRunLengthMatrixGenerator);
    
    /** standard New() method support */
    itkNewMacro(Self) ;
    
    typedef TImageType                                      ImageType;
    typedef typename ImageType::Pointer                     ImagePointer;
    typedef typename ImageType::ConstPointer                ImageConstPointer;
    typedef typename ImageType::PixelType                   PixelType;
    typedef typename ImageType::IndexType                   IndexType;
    typedef typename ImageType::RegionType                  RegionType;
    typedef typename ImageType::OffsetType                  OffsetType;
    typedef VectorContainer<unsigned char, OffsetType>      OffsetVector;
    typedef typename OffsetVector::Pointer                  OffsetVectorPointer;
    typedef typename OffsetVector::ConstPointer             OffsetVectorConstPointer;
    typedef typename ImageType::PointType                   PointType;
    
    typedef typename NumericTraits<PixelType>::RealType     MeasurementType;
    typedef typename NumericTraits<PixelType>::RealType     RealType;
    
    typedef Histogram< MeasurementType, 2, THistogramFrequencyContainer >
                                                            HistogramType;
    typedef typename HistogramType::Pointer                 HistogramPointer;
    typedef typename HistogramType::ConstPointer            HistogramConstPointer;
    typedef typename HistogramType::MeasurementVectorType   MeasurementVectorType;
    
    /** ImageDimension constants */
    itkStaticConstMacro( ImageDimension, unsigned int,
                         TImageType::ImageDimension );
    
    /** Connects the mask image for which the histogram is going to be computed */
    itkSetConstObjectMacro( ImageMask, ImageType );
    itkGetConstObjectMacro( ImageMask, ImageType );

    /** Set the pixel value of the mask that should be considered "inside" the 
      object. Defaults to one. */
    itkSetMacro( InsidePixelValue, PixelType );
    itkGetMacro( InsidePixelValue, PixelType );
    
  protected:
    MaskedScalarImageToGreyLevelRunLengthMatrixGenerator();
    virtual ~MaskedScalarImageToGreyLevelRunLengthMatrixGenerator() {};
    void PrintSelf(std::ostream& os, Indent indent) const;
    virtual void FillHistogram();
        
   private:
    ImageConstPointer m_ImageMask;
    PixelType    m_InsidePixelValue;

  };
    
    
  } // end of namespace Statistics 
} // end of namespace itk 

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkMaskedScalarImageToGreyLevelRunLengthMatrixGenerator.txx"
#endif

#endif
