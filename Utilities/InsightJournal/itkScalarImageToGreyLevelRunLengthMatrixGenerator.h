/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkScalarImageToGreyLevelRunLengthMatrixGenerator.h,v $
  Language:  C++
  Date:      $Date: 2009/03/23 18:13:34 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkScalarImageToGreyLevelRunLengthMatrixGenerator_h
#define __itkScalarImageToGreyLevelRunLengthMatrixGenerator_h

#include "itkImage.h"
#include "itkHistogram.h"
#include "itkDenseFrequencyContainer.h"
#include "itkObject.h"
#include "itkVectorContainer.h"

namespace itk {
  namespace Statistics {

/** \class ScalarImageToGreyLevelRunLengthMatrixGenerator 
*
* Author: Nick Tustison
*/
    
template< class TImageType,
          class THistogramFrequencyContainer = DenseFrequencyContainer >
class ScalarImageToGreyLevelRunLengthMatrixGenerator : public Object
  {
  public:
    /** Standard typedefs */
    typedef ScalarImageToGreyLevelRunLengthMatrixGenerator Self;
    typedef Object Superclass;
    typedef SmartPointer<Self> Pointer;
    typedef SmartPointer<const Self> ConstPointer;
    
    /** Run-time type information (and related methods). */
    itkTypeMacro(ScalarImageToGreyLevelRunLengthMatrixGenerator, Object);
    
    /** standard New() method support */
    itkNewMacro(Self) ;
    
    typedef TImageType                                      ImageType;
    typedef typename ImageType::Pointer                     ImagePointer;
    typedef typename ImageType::ConstPointer                ImageConstPointer;
    typedef typename ImageType::PixelType                   PixelType;
    typedef typename ImageType::IndexType                   IndexType;
    typedef typename ImageType::RegionType                  RegionType;
    typedef typename ImageType::OffsetType                  OffsetType;
    typedef typename ImageType::SizeType                    RadiusType;
    typedef VectorContainer<unsigned char, OffsetType>      OffsetVector;
    typedef typename OffsetVector::Pointer                  OffsetVectorPointer;
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
    
    itkStaticConstMacro( DefaultBinsPerAxis, unsigned int, 256 );
    
    /** Triggers the Computation of the histogram */
    void Compute( void );
    
    /** Connects the input image for which the 
        histogram is going to be computed */
    itkSetConstObjectMacro( Input, ImageType );
    itkGetConstObjectMacro( Input, ImageType );
        
    /** Return the histogram.
      \warning This output is only valid after the 
        Compute() method has been invoked 
      \sa Compute */
    itkGetObjectMacro( Output, HistogramType );
    
    /** Set number of histogram bins along each axis */
    itkSetMacro( NumberOfBinsPerAxis, unsigned int );
    itkGetMacro( NumberOfBinsPerAxis, unsigned int );

    /** Set the min and max (inclusive) pixel 
        value that will be placed in the histogram */
    void SetPixelValueMinMax( PixelType min, PixelType max );
    itkGetMacro( Min, PixelType );
    itkGetMacro( Max, PixelType );
    
    /** Set the min and max (inclusive) distance 
        value that will be placed in the histogram */
    void SetDistanceValueMinMax( RealType min, RealType max );
    itkGetMacro( MinDistance, RealType );
    itkGetMacro( MaxDistance, RealType );

  /** Set the offset or offsets over which the co-occurrence pairs
   * will be computed. Calling either of these methods clears the
   * previous offsets. */
  itkSetObjectMacro( Offsets, OffsetVector );
  itkGetConstObjectMacro( Offsets, OffsetVector );
  void SetOffset( const OffsetType offset )
    {
    OffsetVectorPointer offsetVector = OffsetVector::New();
    offsetVector->push_back( offset );
    this->SetOffsets( offsetVector );
    }
  void AddOffset( const OffsetType offset )
    {
    if( this->m_Offsets )
      {
      this->m_Offsets->push_back( offset );
      }
    else
      {
      this->SetOffset( offset );
      }   
    }
    
  protected:
    ScalarImageToGreyLevelRunLengthMatrixGenerator();
    virtual ~ScalarImageToGreyLevelRunLengthMatrixGenerator() {};
    void PrintSelf(std::ostream& os, Indent indent) const;
    virtual void FillHistogram(RadiusType radius, RegionType region);

    ImageConstPointer        m_Input;
        
   private:
  
    HistogramPointer         m_Output;
    OffsetVectorPointer      m_Offsets;
    PixelType                m_Min, m_Max;
    RealType                 m_MinDistance, m_MaxDistance;

    unsigned int            m_NumberOfBinsPerAxis;
    MeasurementVectorType   m_LowerBound, m_UpperBound;

  };
    
    
  } // end of namespace Statistics 
} // end of namespace itk 

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkScalarImageToGreyLevelRunLengthMatrixGenerator.txx"
#endif

#endif
