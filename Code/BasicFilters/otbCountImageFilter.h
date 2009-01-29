/*=========================================================================

Program:   ORFEO Toolbox
Language:  C++
Date:      $Date$
Version:   $Revision$


Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.

Copyright (c) CS Systemes d'information. All rights reserved.
See CSCopyright.txt for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbCountImageFilter_h
#define __otbCountImageFilter_h


#include "itkImageToImageFilter.h"
#include "itkProcessObject.h"
#include "otbCountImageFunction.h"
#include "itkNumericTraits.h"


/** \class CountImageFilter
 *  \brief This class extracts key points from an image through a pyramidal gaussian based decomposition

 *
 */

namespace otb
{
  template <class TInputImage , class TDetector, class TCount, class TOutputImage>
    class ITK_EXPORT CountImageFilter
    : public itk::ImageToImageFilter<TInputImage, TOutputImage>
    {

    public:

    /** Standard class typedefs. */
      typedef CountImageFilter                                            Self;
      typedef itk::ImageToImageFilter<TInputImage,TOutputImage>           Superclass ;
      typedef itk::SmartPointer<Self>                                     Pointer;
      typedef itk::SmartPointer<const Self>                               ConstPointer;

      /** Method for creation through the object factory. */
      itkNewMacro(Self);

      /** Run-time type information (and related methods). */
      itkTypeMacro(CountImageFilter,itk::ImageToImageFilter);


      /** Template parameters typedefs*/
      typedef TInputImage                                  InputImageType;
      typedef typename InputImageType::Pointer             InputImagePointerType;
      typedef typename InputImageType::IndexType           IndexType;

      /** OutputImageType typedef support*/
      typedef typename Superclass::OutputImageType         OutputImageType;
      typedef typename OutputImageType::Pointer            OutputImagePointerType;
      typedef typename OutputImageType::RegionType  OutputImageRegionType;
      typedef typename OutputImageType::PixelType          OutputPixelType; 
      
      typedef typename itk::NumericTraits< OutputPixelType>::RealType  OutputRealType;

      /** Detector typedef Support*/
      typedef TDetector                                    DetectorType;
      
      /** Count Function typedef Support*/
      typedef TCount                                       CountMethodType;
      
      /** CountImageFunction support*/
      typedef otb::CountImageFunction<InputImageType,DetectorType, 
	            CountMethodType >                     CountImageFunctionType;
      typedef typename CountImageFunctionType::Pointer    CountImageFunctionTypePointer;
      
      /** Get/Set the radius of the neighborhood over which the
	  statistics are evaluated */
      itkSetMacro( NeighborhoodRadius, unsigned int );
      itkGetConstReferenceMacro( NeighborhoodRadius, unsigned int );


      /**Set/Get Descriptor from the otbCountmageFunction*/
      virtual void SetDetector(DetectorType* detector);
      virtual DetectorType* GetDetector();
      
    protected:

      /**
       * Constructor.
       */
      CountImageFilter();
      /**
       * Destructor.
       */
      virtual ~CountImageFilter();
      /**
       * Standard PrintSelf method.
       */
      virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
      /**
       * Main computation method.
       */
      virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, int threadId );

  private:

      CountImageFilter(const Self&); //purposely not implemented
      void operator=(const Self&); //purposely not implemented
      
      CountImageFunctionTypePointer m_CountImageFunction;

      unsigned int m_NeighborhoodRadius;
    };
}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbCountImageFilter.txx"
#endif

#endif


