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
#ifndef __otbMultiChannelExtractROI_h
#define __otbMultiChannelExtractROI_h

#include "otbExtractROIBase.h"
#include "otbVectorImage.h"

#include "itkMacro.h"
#include <vector>

namespace otb
{

/** \class MultiChannelExtractROI
 * \brief Extract a spatial or spectral subset of a multi-channel image.
 *
 * It is possible to extract all the channels from the input image or only those specified by the user.
 * The SetChannel() method allows selecting one channel.
 * The SetFirstChannel() and SetLastChannel() methods allow the user to define a list of channels.
 *
 * \note If no channels are specified, then all channels from the input image are selected.
 * \note The image manipulated inside this class are of type otb::VectorImage.
 *
 * \ingroup OTBImageBase
 */
template <class TInputPixelType, class TOutputPixelType>
class ITK_EXPORT MultiChannelExtractROI :
  public ExtractROIBase<VectorImage<TInputPixelType, 2>, VectorImage<TOutputPixelType, 2> >
{
public:
  /** Standard class typedefs. */
  typedef MultiChannelExtractROI                                                             Self;
  typedef ExtractROIBase<VectorImage<TInputPixelType, 2>, VectorImage<TOutputPixelType, 2> > Superclass;
  typedef itk::SmartPointer<Self>                                                            Pointer;
  typedef itk::SmartPointer<const Self>                                                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MultiChannelExtractROI, ExtractROIBase);

  /** Image type information. */
  typedef typename Superclass::InputImageType  InputImageType;
  typedef typename Superclass::OutputImageType OutputImageType;

  /** Pixel type information */
  typedef typename OutputImageType::InternalPixelType OutputValueType;

  /** Typedef to describe the output and input image region types. */
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename InputImageType::RegionType  InputImageRegionType;

  /** Typedef to describe the type of pixel. */
  typedef typename OutputImageType::PixelType OutputImagePixelType;
  typedef typename InputImageType::PixelType  InputImagePixelType;

  /** Typedef to describe the output and input image index and size types. */
  typedef typename OutputImageType::IndexType OutputImageIndexType;
  typedef typename InputImageType::IndexType  InputImageIndexType;
  typedef typename OutputImageType::SizeType  OutputImageSizeType;
  typedef typename InputImageType::SizeType   InputImageSizeType;

  void SetFirstChannel(unsigned int id);
  itkGetConstMacro(FirstChannel, unsigned int);
  void SetLastChannel(unsigned int id);
  itkGetConstMacro(LastChannel, unsigned int);

  /** Typedef Liste des canaux */
  typedef typename std::vector<unsigned int> ChannelsType;

  /** \return The list of processed channels */
  ChannelsType GetChannels() const
  {
    return (m_Channels);
  }

  /** \return The number of processed channels */
  unsigned int GetNbChannels() const
  {
    return (m_Channels.size());
  }

  /** Select a channel to process
   \param channel The channel to process */
  void SetChannel(unsigned int channel);

  /** Clear channels selection */
  void ClearChannels(void);

  /** ImageDimension enumeration */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      InputImageType::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      OutputImageType::ImageDimension);

protected:
  MultiChannelExtractROI();
  virtual ~MultiChannelExtractROI() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** MultiChannelExtractROI can produce an image which is a different
   * resolution than its input image.  As such, MultiChannelExtractROI
   * needs to provide an implementation for
   * GenerateOutputInformation() in order to inform the pipeline
   * execution model.  The original documentation of this method is
   * below.
   *
   * \sa ProcessObject::GenerateOutputInformaton()  */
  virtual void GenerateOutputInformation();

  /** Reinitialize channels vector for multiple Update.*/
  void ChannelsReInitialization();
  /** If the user set m_First/LastChannel, chack avaliability or fill m_Channels Work.*/
  void SetChannelsWorkWithLimits();

  /** ExtractImageFilter can be implemented as a  multithreaded filter.
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            itk::ThreadIdType threadId);

private:
  MultiChannelExtractROI(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** First/ last channel to process [1...] */
  unsigned int m_FirstChannel;
  unsigned int m_LastChannel;
  /** List of channels to process  [1...] */
  ChannelsType m_Channels;
  /** List of channels actually being processed [1...] */
  ChannelsType m_ChannelsWorks;
  /** Used to know if the user has set m_First/LastChannel (1) or m_Channels (3)*/
  int m_ChannelsKind;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMultiChannelExtractROI.txx"
#endif

#endif
