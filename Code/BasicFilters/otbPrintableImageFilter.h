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

#include "otbImage.h"
#include "itkImageToImageFilter.h"
#include "otbVectorRescaleIntensityImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbMultiChannelExtractROI.h"
#include "otbMultiToMonoChannelExtractROI.h"
#include "itkMultiplyImageFilter.h"
#include "itkAddImageFilter.h"
//#include "otbImageToVectorImageCastFilter.h"
#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"
#include "otbVectorRescaleIntensityImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"

#include "itkInvertIntensityImageFilter.h"

namespace otb
{
/**
 * \class PrintableImageFilter
 * \brief This class is a helper class to turn a vector image to a generic 8 bytes RGB image.
 *
 *  It is useful for publications for instance.
 **/
  
template <class TInputImage , class TMaskImage = otb::Image<unsigned char, 2> >
class ITK_EXPORT PrintableImageFilter :
public itk::ImageToImageFilter<TInputImage, otb::VectorImage<unsigned char, 2> >
{
  public:
  typedef PrintableImageFilter                            Self;
  typedef itk::ImageToImageFilter
  <TInputImage, otb::VectorImage<unsigned char,2> >       Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;
  
  typedef TInputImage                                     InputImageType;
  //typedef typename InputImageType::Pointer                InputImagePointerType;
  typedef typename InputImageType::PixelType              InputPixelType;
  typedef typename InputImageType::InternalPixelType      InputInternalPixelType;
  //typedef typename Image<InputInternalPixelType, 2>       SingleInputImageType;
  typedef unsigned char                                   OutputInternalPixelType;
  typedef VectorImage<OutputInternalPixelType,2>          OutputImageType;
  typedef OutputImageType::PixelType                      OutputPixelType;
  
  typedef TMaskImage                                                        MaskImageType;
  typedef typename MaskImageType::Pointer                                   MaskImagePointerType;
  typedef typename MaskImageType::PixelType               MaskPixelType;   

  typedef VectorRescaleIntensityImageFilter<InputImageType,OutputImageType> VectorRescalerType;
  //typedef itk::RescaleIntensityImageFilter<MaskImageType, MaskImageType>    MaskRescalerType;
  
  
  //typedef otb::MultiChannelExtractROI<OutputInternalPixelType,
  //OutputInternalPixelType>              ChannelExtractorType;
  typedef otb::MultiChannelExtractROI<InputInternalPixelType,
                                      InputInternalPixelType>              ChannelExtractorType;
  typedef typename ChannelExtractorType::ChannelsType                       ChannelsType;


  /** Filters */
  //typedef itk::InvertIntensityImageFilter<MaskImageType,MaskImageType>       InvertIntensityImageFilterType;
  typedef itk::BinaryThresholdImageFilter<MaskImageType,MaskImageType>      InvertIntensityImageFilterType;
  typedef typename InvertIntensityImageFilterType::Pointer                   InvertIntensityImageFilterPointerType;
  typedef typename itk::MultiplyImageFilter<OutputImageType, MaskImageType>  MultiplierType;
  typedef typename MultiplierType::Pointer                                   MultiplierPointerType;
  //typedef ImageToVectorImageCastFilter<MaskImageType, OutputImageType>       CasterFilterType;
  //typedef typename CasterFilterType::Pointer                                 CasterFilterPointerType;
  typedef otb::VectorRescaleIntensityImageFilter<OutputImageType,OutputImageType> RescalerFilterType;
  typedef typename RescalerFilterType::Pointer                               RescalerFilterPointerType;
  typedef typename itk::AddImageFilter<OutputImageType, OutputImageType>     AdderFilterType;
  typedef typename AdderFilterType::Pointer                                  AdderFilterPointerType;
  typedef ImageList<MaskImageType>                                           ImageListType;
  typedef ImageListToVectorImageFilter<ImageListType, OutputImageType>       CasterFilterType;
  typedef typename CasterFilterType::Pointer                                 CasterFilterPointerType;
 

  /** Method for creation through object factory */
  itkNewMacro(Self);
  
  /** Run-time type information */
  itkTypeMacro(PrintableImageFilter,
               itk::ImageToImageFilter);
  
  /** Display */
  void PrintSelf( std::ostream& os, itk::Indent indent ) const;
  
  void SetChannel( unsigned int channel);
  const ChannelsType GetChannels(void) const;
  
  otbSetObjectMemberMacro(Rescaler,AutomaticInputMinMaxComputation,bool);
  otbGetObjectMemberMacro(Rescaler,AutomaticInputMinMaxComputation,bool);
  otbSetObjectMemberMacro(Rescaler,InputMinimum,InputPixelType);
  otbGetObjectMemberMacro(Rescaler,InputMinimum,InputPixelType);
  otbSetObjectMemberMacro(Rescaler,InputMaximum,InputPixelType);
  otbGetObjectMemberMacro(Rescaler,InputMaximum,InputPixelType);
  
  
  /**
   * If set, only pixels within the mask will be classified.
   * \param mask The input mask.
   */
  void SetInputMask(const MaskImageType * mask);
  
  /**
   * Get the input mask.
   * \return The mask.
   */
  MaskImageType * GetInputMask(void);

  itkSetMacro(UseMask, bool);
  itkGetMacro(UseMask, bool);
  
  ChannelsType const GetChannelList()
  {
    return m_ChannelList;
  }
  void SetChannelList( ChannelsType chList )
  {
    if(chList.size() != 3)
      {
	itkExceptionMacro(<<"Invalid channel list, size is "<<chList.size()<<" instead of 3");
      }
    m_ChannelList = chList;
    this->Modified();
  }
  
  void SetObjectColor( OutputPixelType val )
  {
    if(val.GetSize() != 3)
    {
      itkExceptionMacro(<<"Invalid object color, size is "<<val.Size()<<" instead of 3");
    }
    m_ObjectColor = val;
    this->Modified();
  }
  itkGetMacro(ObjectColor, OutputPixelType);
  
  itkSetMacro(BackgroundMaskValue, MaskPixelType);
  itkGetMacro(BackgroundMaskValue, MaskPixelType); 
  itkSetMacro(ForegroundMaskValue, MaskPixelType);
  itkGetMacro(ForegroundMaskValue, MaskPixelType); 

  protected:

  PrintableImageFilter();

  void BeforeGenerateData();
  void GenerateData();
  
  private:
  
  PrintableImageFilter(Self&);   // intentionally not implemented
  void operator=(const Self&);          // intentionally not implemented
  
  typename VectorRescalerType::Pointer   m_Rescaler;
  typename ChannelExtractorType::Pointer m_Extractor;
    
  InvertIntensityImageFilterPointerType m_InvertIntensityImageFilter;
  MultiplierPointerType                 m_Multiplier;
  CasterFilterPointerType               m_MaskCaster;
  RescalerFilterPointerType             m_MaskRescaler;
  AdderFilterPointerType                m_Adder;

  // Objects (of the mask) will be displayer with the choosen color.
  OutputPixelType m_ObjectColor;
  // Use mask
  bool m_UseMask;
  // Used channel for output Image
  ChannelsType m_ChannelList;
  // Foreground mask value
  MaskPixelType m_ForegroundMaskValue;
  // Background mask value
  MaskPixelType m_BackgroundMaskValue;
  
};
 
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPrintableImageFilter.txx"
#endif

#endif

