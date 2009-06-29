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
#include "otbMultiChannelExtractROI.h"
#include "itkMultiplyImageFilter.h"
#include "itkAddImageFilter.h"
#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"


namespace otb
{
/**
 * \class PrintableImageFilter
 * \brief This class is a helper class to turn a vector image to a generic 8 bytes RGB image.
 * A mask can be used to highlight some object represebted by the same value.
 * The mask is a binary image. Foregroung and background MaskValue are used to precise which 
 * value of the mask in the background ans which values are objects (default resp. 0 and 255). 
 * Output object color can be set using m_ObjectColor (default white).
 * The output is a 3 channelsbands images, each channel is a channel of the input image. 
 * They can be selected usin m_ChannelList or SetChannel(int ch ) method.
 *
 *  It is useful for publications for instance.
 * 
 * \sa itkImageToImageFilter
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
  typedef typename InputImageType::PixelType              InputPixelType;
  typedef typename InputImageType::InternalPixelType      InputInternalPixelType;
  typedef unsigned char                                   OutputInternalPixelType;
  typedef VectorImage<OutputInternalPixelType,2>          OutputImageType;
  typedef OutputImageType::PixelType                      OutputPixelType;
  
  typedef TMaskImage                                      MaskImageType;
  typedef typename MaskImageType::Pointer                 MaskImagePointerType;
  typedef typename MaskImageType::PixelType               MaskPixelType;   

  typedef VectorRescaleIntensityImageFilter<InputImageType,OutputImageType> VectorRescalerType;
  
  typedef otb::MultiChannelExtractROI<InputInternalPixelType,
                                      InputInternalPixelType>               ChannelExtractorType;
  typedef typename ChannelExtractorType::ChannelsType                       ChannelsType;


  /** Filters */
  typedef itk::BinaryThresholdImageFilter<MaskImageType,MaskImageType>       ThresholdImageFilterType;
  typedef typename ThresholdImageFilterType::Pointer                         ThresholdImageFilterPointerType;
  typedef typename itk::MultiplyImageFilter<OutputImageType, MaskImageType>  MultiplierType;
  typedef typename MultiplierType::Pointer                                   MultiplierPointerType;
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
  /* Set the selected channle index (order is important) */
  void SetChannelList( ChannelsType chList )
  {
    if(chList.size() != 3)
      {
	itkExceptionMacro(<<"Invalid channel list, size is "<<chList.size()<<" instead of 3");
      }
    m_ChannelList = chList;
    this->Modified();
  }
  
  /** Output Mask Object color. */
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
    
  ThresholdImageFilterPointerType       m_MaskThresholder;
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

