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
#ifndef __otbLabelizeImageFilter_h
#define __otbLabelizeImageFilter_h

// itk
#include "itkImageToImageFilter.h"
#include "itkConnectedThresholdImageFilter.h"
#include "itkAddImageFilter.h"

// otb::fe
#include "otbThresholdImageToPointSetFilter.h"
#include "otbMultiplyByScalarImageFilter.h"

namespace otb
{

  /** \class LabelizeImageFilter
   *  \brief Labelize an gray level image
   *
   *  This filter labelizes an input image and produces
   *  an output image label
   */

  template <class TInputImage, class TOutputImage>
    class ITK_EXPORT LabelizeImageFilter
    : public itk::ImageToImageFilter<TInputImage, TOutputImage>
    {
    public:
      /** typedef for standard classes. */
      typedef LabelizeImageFilter Self;
      typedef itk::ImageToImageFilter<TInputImage,TOutputImage> Superclass;
      typedef itk::SmartPointer<Self> Pointer;
      typedef itk::SmartPointer<const Self>  ConstPointer;
      
      /** "object factory" management method. */
      itkNewMacro(Self);
      
      /** Return the class name. */
      itkTypeMacro(LabelizeImageFilter, ImageToImageFilter);

      /** typedef to simplify variables definition and declaration. */
      typedef TInputImage InputImageType;
      typedef TOutputImage OutputImageType;
      typedef typename InputImageType::PixelType InputPixelType;
      typedef itk::PointSet<InputPixelType, 2> PointSetType;
      typedef typename PointSetType::Pointer PointSetPointerType;
      
      /** Get lower threshold */
      itkGetMacro(LowerThreshold, InputPixelType);
      
      /** Set lower threshold */
      itkSetMacro(LowerThreshold, InputPixelType);
      
      /** Get upper threshold */
      itkGetMacro(UpperThreshold, InputPixelType);
      
      /** Set upper threshold */
      itkSetMacro(UpperThreshold, InputPixelType);
      
      /** Get delta lower threshold */
      itkGetMacro(DeltaLowerThreshold, InputPixelType);
      
      /** Set delta lower threshold */
      itkSetMacro(DeltaLowerThreshold, InputPixelType);
      
      /** Get delta upper threshold */
      itkGetMacro(DeltaUpperThreshold, InputPixelType);
      
      /** Set delta upper threshold */
      itkSetMacro(DeltaUpperThreshold, InputPixelType);
      
      /** Internal */
      typedef otb::ThresholdImageToPointSetFilter<InputImageType, PointSetType> ThresholdFilterType;
      typedef typename ThresholdFilterType::Pointer ThresholdFilterPointerType;
      
      typedef otb::MultiplyByScalarImageFilter<InputImageType, OutputImageType> MultiplyFilterType;
      typedef typename MultiplyFilterType::Pointer MultiplyFilterPointerType;
      
      typedef itk::ConnectedThresholdImageFilter<InputImageType, InputImageType> ConnectedFilterType;
      typedef typename ConnectedFilterType::Pointer ConnectedFilterPointerType;
      
      typedef itk::AddImageFilter<InputImageType, InputImageType, OutputImageType> AddImageFilterType;
      typedef typename AddImageFilterType::Pointer AddImageFilterPointerType;
      
    protected:
      LabelizeImageFilter();
      virtual ~LabelizeImageFilter() {};
      virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
      
      /** Main computation method */
      virtual void GenerateData();
      
    private:
      LabelizeImageFilter(const Self&); //purposely not implemented
      void operator=(const Self&); //purposely not implemented
      
      /** Lower threshold */
      InputPixelType m_LowerThreshold;
      
      /** Upper threshold */
      InputPixelType m_UpperThreshold;
      
      /** Delta + threshold for growing region */
      InputPixelType m_DeltaUpperThreshold;
      
      /** Delta - threshold for growing region */
      InputPixelType m_DeltaLowerThreshold;
      
      /** Point set to store thresold results */
      PointSetPointerType m_PointSet;
      
    };
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelizeImageFilter.txx"
#endif

#endif
