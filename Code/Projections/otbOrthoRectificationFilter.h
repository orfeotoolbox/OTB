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
#ifndef __otbOrthoRectificationFilter_h
#define __otbOrthoRectificationFilter_h

#include "otbOrthoRectificationFilterBase.h"
#include "itkImageToImageFilter.h"
#include "itkChangeInformationImageFilter.h"

namespace otb
{
/** \class OrthoRectificationFilter
 * 
 * \brief Class for Orthorectifying an image
 *
 * This class is used to apply map projection and sensor model transformation
 * to orthorectify an image, with or without DEM.
 * 
 */

template <class TInputImage,
          class TOutputImage,
          class TMapProjection,
          class TInterpolatorPrecision=double>
class ITK_EXPORT OrthoRectificationFilter : public itk::ImageToImageFilter< TInputImage, TOutputImage>
{
        public :
        /** Standard class typedefs */
        typedef OrthoRectificationFilter                          Self;
        typedef itk::ImageToImageFilter< TInputImage, TOutputImage>  Superclass;
        typedef itk::SmartPointer<Self>                           Pointer;
        typedef itk::SmartPointer<const Self>                     ConstPointer;
  
        typedef OrthoRectificationFilterBase<   TInputImage,
                                                TOutputImage,
                                                TMapProjection,
                                                TInterpolatorPrecision > OrthoRectificationFilterBaseType;
        typedef typename OrthoRectificationFilterBaseType::Pointer OrthoRectificationFilterBasePointer;
        typedef itk::ChangeInformationImageFilter<TInputImage > ChangeInfoFilterType;
        typedef typename ChangeInfoFilterType::Pointer ChangeInfoFilterPointer;
        
        typedef typename OrthoRectificationFilterBaseType::InputImageType       InputImageType;
	typedef typename OrthoRectificationFilterBaseType::OutputImageType       OutputImageType;
        typedef typename OrthoRectificationFilterBaseType::MapProjectionType    MapProjectionType;
        typedef typename OrthoRectificationFilterBaseType::InterpolatorType     InterpolatorType;
        typedef typename OrthoRectificationFilterBaseType::IndexType            IndexType;
        typedef typename OrthoRectificationFilterBaseType::SizeType             SizeType;
        typedef typename OrthoRectificationFilterBaseType::PointType            PointType;
	typedef typename OrthoRectificationFilterBaseType::RegionType           RegionType;
        typedef typename OrthoRectificationFilterBaseType::OriginPointType      OriginPointType;
        typedef typename OrthoRectificationFilterBaseType::SpacingType          SpacingType;
  
  
        /** Method for creation through the object factory. */
        itkNewMacro( Self );
  
        /** Run-time type information (and related methods). */
        itkTypeMacro( OrthoRectificationFilter, itk::ImageToImageFilter );
        
        /** Accessors */
        virtual void SetMapProjection (MapProjectionType* _arg) 
        { 
                m_OrthoRectificationFilter->SetMapProjection( _arg);
        } 
        
        virtual MapProjectionType * GetMapProjection()
        {
                return (m_OrthoRectificationFilter->GetMapProjection());
        }
        
        /** Specify where are DEM files, and load useful ones */
        virtual void SetDEMDirectory(const std::string& directory)
        {
                m_OrthoRectificationFilter->SetDEMDirectory(directory);
        }        
  
        /** Methods to decide to use DEM */                
        virtual void EnableDEM()
        { 
                m_OrthoRectificationFilter->EnableDEM(); 
        }
        virtual void DisableDEM()
        {        
                m_OrthoRectificationFilter->DisableDEM(); 
        }
        
        
        virtual void SetOutputStartIndex(const IndexType index)
        {
                m_OrthoRectificationFilter->SetOutputStartIndex(index);
        }
        virtual const IndexType & GetOutputStartIndex()const
        {
                return ( m_OrthoRectificationFilter->GetOutputStartIndex());
        }
        virtual void SetOutputSpacing(const SpacingType spacing)
        {
                m_OrthoRectificationFilter->SetOutputSpacing(spacing);
        }
        virtual const SpacingType & GetOutputSpacing()const
        {
                return (m_OrthoRectificationFilter->GetOutputSpacing());
        }
        virtual void SetOutputOrigin(const OriginPointType origin)
        {
                m_OrthoRectificationFilter->SetOutputOrigin(origin);
        }
        virtual const OriginPointType & GetOutputOrigin()const
        {
                return (m_OrthoRectificationFilter->GetOutputOrigin());
        }
        
        virtual void SetInterpolator(InterpolatorType * interpol)
        {
                m_OrthoRectificationFilter->SetInterpolator(interpol);
        }
        virtual const InterpolatorType * GetInterpolator()const
        {
                return (m_OrthoRectificationFilter->GetInterpolator());
        }
        virtual void SetSize(const SizeType size)
        {
                m_OrthoRectificationFilter->SetSize(size);
        }
        virtual const SizeType & GetSize()const
        {
                return (m_OrthoRectificationFilter->GetSize());
        }
       
  
  protected:
        OrthoRectificationFilter();
        ~OrthoRectificationFilter();
        void PrintSelf(std::ostream& os, itk::Indent indent) const;
        
        /** Main computation method */
        virtual void GenerateData(void);
	
        /** Generate Output Information */
        virtual void GenerateOutputInformation(void);

	 virtual void GenerateInputRequestedRegion(void);

  private:
        OrthoRectificationFilter(const Self&); //purposely not implemented
        void operator=(const Self&); //purposely not implemented
  
        /** Orthorectification filter */
        OrthoRectificationFilterBasePointer m_OrthoRectificationFilter;
        ChangeInfoFilterPointer m_ChangeInfoFilter;
  
};
  
} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbOrthoRectificationFilter.txx"
#endif

#endif
