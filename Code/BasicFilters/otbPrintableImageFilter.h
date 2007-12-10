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
        
#ifndef __otbPrintableImageFilter_h
#define __otbPrintableImageFilter_h

#include "itkImageToImageFilter.h"
#include "otbVectorRescaleIntensityImageFilter.h"
#include "otbMultiChannelExtractROI.h"
    
namespace otb {
  /**
       * \class PrintableImageFilter
       * \brief This class 
       * 
       * 
       * 
       * 
       *
       * 
       * 
   **/

      template <class TInputImageType>
class ITK_EXPORT PrintableImageFilter :
          public itk::ImageToImageFilter<TInputImageType, otb::VectorImage<unsigned char,2> >
    {
      public:
        typedef PrintableImageFilter   Self;
        typedef itk::ImageToImageFilter
            <TInputImageType, otb::VectorImage<unsigned char,2> > Superclass;
        typedef itk::SmartPointer<Self>             Pointer;
        typedef itk::SmartPointer<const Self>       ConstPointer;
        typedef otb::VectorImage<unsigned char,2>   OutputImageType;
//         typedef typename OutputImageType::PixelType PixelType;
        typedef unsigned char PixelType;

        typedef otb::VectorRescaleIntensityImageFilter
            <TInputImageType,OutputImageType>            VectorRescalerType;
        typedef otb::MultiChannelExtractROI
            <PixelType,PixelType>           ChannelExtractorType;
        typedef typename ChannelExtractorType::ChannelsType ChannelsType;
          
          
        /** Method for creation through object factory */
        itkNewMacro(Self);

        /** Run-time type information */
        itkTypeMacro(PrintableImageFilter, 
                     itk::ImageToImageFilter);

        /** Display */
        void PrintSelf( std::ostream& os, itk::Indent indent ) const;
          
        void SetChannel( unsigned int channel);
        const ChannelsType GetChannels(void);
          
      protected:

        PrintableImageFilter();

        void GenerateData();

      private:

        PrintableImageFilter(Self&);   // intentionally not implemented
        void operator=(const Self&);          // intentionally not implemented

        typename VectorRescalerType::Pointer m_Rescaler;
        typename ChannelExtractorType::Pointer m_Extractor;



    };
      
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPrintableImageFilter.txx"
#endif

#endif

