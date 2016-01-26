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
#include "otbMultiChannelExtractROI.h"
#include "itkBinaryFunctorImageFilter.h"

namespace otb
{
namespace Functor
{
/**
 * \class MaskFunctor
 * \brief Output is a InputPixel if MaskPixel is m_Background and a
 * defined other value (m_ObjectColor) otherwise.
 *
 * \ingroup OTBImageManipulation
 */

template<class TInputPixel, class TMaskPixel, class TOutputPixel>
class ITK_EXPORT MaskFunctor
{
public:
  MaskFunctor()
    {
    m_BackgroundValue = 0;
    m_ObjectColor.SetSize(3);
    m_ObjectColor.Fill(255);
    };
  ~MaskFunctor(){}

  typedef TInputPixel                         InputPixelType;
  typedef TMaskPixel                          MaskPixelType;
  typedef TOutputPixel                        OutputPixelType;
  typedef typename OutputPixelType::ValueType OutputInternalPixelType;

  MaskPixelType GetBackgroundValue()
  {
    return m_BackgroundValue;
  }
  void SetBackgroundValue(MaskPixelType val)
  {
    m_BackgroundValue = val;
  }

  OutputPixelType GetObjectColor()
  {
    return m_ObjectColor;
  }
  void SetObjectColor(OutputPixelType val)
  {
    m_ObjectColor = val;
  }

  inline OutputPixelType operator ()(InputPixelType inPix, MaskPixelType maskPix) const
  {
    OutputPixelType outPix;
    if (maskPix == m_BackgroundValue)
      {
      outPix.SetSize(inPix.Size());
      for (unsigned int i = 0; i < outPix.Size(); ++i)
        {
        outPix[i] = static_cast<OutputInternalPixelType>(inPix[i]);
        }
      }
    else
      {
      outPix = m_ObjectColor;
      }

    return outPix;
  }

protected:
  MaskPixelType   m_BackgroundValue;
  OutputPixelType m_ObjectColor;
};
}

/**
 * \class PrintableImageFilter
 * \brief This class is a helper class to turn a vector image to a generic 8 bytes RGB image.
 * A mask can be used to highlight some objects represented by the same value.
 * The mask is a binary image. Background MaskValue is used to precise which
 * value of the mask are objects (default 0).
 * Output object color can be set using m_ObjectColor (default white).
 * The output is a 3 channel image, each channel is a channel of the input image.
 * They can be selected using m_ChannelList or SetChannel(int ch ) method.
 *
 *  It is useful for publications for instance.
 *
 * \sa itkImageToImageFilter
 *
 * \ingroup OTBImageManipulation
 **/

template <class TInputImage, class TMaskImage = otb::Image<unsigned char, 2> >
class ITK_EXPORT PrintableImageFilter :
  public itk::ImageToImageFilter<TInputImage, otb::VectorImage<unsigned char, 2> >
{
public:
  typedef PrintableImageFilter Self;
  typedef itk::ImageToImageFilter
  <TInputImage, otb::VectorImage<unsigned char, 2> >       Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef TInputImage                                InputImageType;
  typedef typename InputImageType::PixelType         InputPixelType;
  typedef typename InputImageType::InternalPixelType InputInternalPixelType;
//   typedef typename itk::NumericTraits<InputPixelType>::ValueType InputInternalPixelType;
  typedef unsigned char                           OutputInternalPixelType;
  typedef VectorImage<OutputInternalPixelType, 2> OutputImageType;
  typedef OutputImageType::PixelType              OutputPixelType;

  typedef TMaskImage                        MaskImageType;
  typedef typename MaskImageType::Pointer   MaskImagePointerType;
  typedef typename MaskImageType::PixelType MaskPixelType;

  typedef otb::MultiChannelExtractROI<InputInternalPixelType,
      InputInternalPixelType>               ChannelExtractorType;
  typedef typename ChannelExtractorType::ChannelsType ChannelsType;

  typedef VectorRescaleIntensityImageFilter
  <typename ChannelExtractorType::OutputImageType, OutputImageType>  VectorRescalerType;

  typedef Functor::MaskFunctor<InputPixelType, MaskPixelType, OutputPixelType> FunctorType;
  typedef itk::BinaryFunctorImageFilter<OutputImageType, MaskImageType,
      OutputImageType, FunctorType>     FunctorFilterType;
  typedef typename FunctorFilterType::Pointer FunctorFilterPointerType;

  /** Method for creation through object factory */
  itkNewMacro(Self);

  /** Run-time type information */
  itkTypeMacro(PrintableImageFilter,
               itk::ImageToImageFilter);

  /** Display */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  void SetChannel(unsigned int channel);
  const ChannelsType GetChannels(void) const;

  otbSetObjectMemberMacro(Rescaler, AutomaticInputMinMaxComputation, bool);
  otbGetObjectMemberMacro(Rescaler, AutomaticInputMinMaxComputation, bool);
  otbSetObjectMemberMacro(Rescaler, InputMinimum, InputPixelType);
  otbGetObjectMemberMacro(Rescaler, InputMinimum, InputPixelType);
  otbSetObjectMemberMacro(Rescaler, InputMaximum, InputPixelType);
  otbGetObjectMemberMacro(Rescaler, InputMaximum, InputPixelType);

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
  void SetChannelList(ChannelsType chList)
  {
    if (chList.size() != 3)
      {
      itkExceptionMacro(<< "Invalid channel list, size is " << chList.size() << " instead of 3");
      }
    m_ChannelList = chList;
    this->Modified();
  }

  /** Output Mask Object color. */
  void SetObjectColor(OutputPixelType val)
  {
    if (val.GetSize() != 3)
      {
      itkExceptionMacro(<< "Invalid object color, size is " << val.Size() << " instead of 3");
      }
    m_ObjectColor = val;
    m_MaskFilter->GetFunctor().SetObjectColor(val);
    this->Modified();
  }
  itkGetMacro(ObjectColor, OutputPixelType);

  void SetBackgroundMaskValue(MaskPixelType val)
  {
    m_BackgroundMaskValue = val;
    m_MaskFilter->GetFunctor().SetBackgroundValue(val);
    this->Modified();
  }
  itkGetMacro(BackgroundMaskValue, MaskPixelType);

  /** PrintableImageFilter can produce an image which is a different
   * resolution than its input image.  As such, PrintableImageFilter
   * needs to provide an implementation for
   * GenerateOutputInformation() in order to inform the pipeline
   * execution model.  The original documentation of this method is
   * below.
   *
   * \sa ProcessObject::GenerateOutputInformaton()  */
  virtual void GenerateOutputInformation();

protected:

  PrintableImageFilter();

  void BeforeGenerateData();
  void GenerateData();

private:

  PrintableImageFilter(Self &);   // intentionally not implemented
  void operator =(const Self&);          // intentionally not implemented

  typename VectorRescalerType::Pointer m_Rescaler;
  typename ChannelExtractorType::Pointer m_Extractor;
  // Foreground mask value
  FunctorFilterPointerType m_MaskFilter;
  // Objects (of the mask) will be displayer with the chosen color.
  OutputPixelType m_ObjectColor;
  // Use mask
  bool m_UseMask;
  // Used channel for output Image
  ChannelsType m_ChannelList;
  // Foreground mask value
  //MaskPixelType m_ForegroundMaskValue;
  // Background mask value
  MaskPixelType m_BackgroundMaskValue;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPrintableImageFilter.txx"
#endif

#endif
