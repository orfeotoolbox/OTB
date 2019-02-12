/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbMultiChannelExtractROI_h
#define otbMultiChannelExtractROI_h

#include "otbExtractROIBase.h"
#include "otbVectorImage.h"
#include "OTBImageBaseExport.h"

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
class OTBImageBase_EXPORT_TEMPLATE MultiChannelExtractROI :
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
  ~MultiChannelExtractROI() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** MultiChannelExtractROI can produce an image which is a different
   * resolution than its input image.  As such, MultiChannelExtractROI
   * needs to provide an implementation for
   * GenerateOutputInformation() in order to inform the pipeline
   * execution model.  The original documentation of this method is
   * below.
   *
   * \sa ProcessObject::GenerateOutputInformaton()  */
  void GenerateOutputInformation() override;

  /** Reinitialize channels vector for multiple Update.*/
  void ChannelsReInitialization();
  /** If the user set m_First/LastChannel, chack avaliability or fill m_Channels Work.*/
  void SetChannelsWorkWithLimits();

  /** ExtractImageFilter can be implemented as a  multithreaded filter.
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            itk::ThreadIdType threadId) override;

private:
  MultiChannelExtractROI(const Self &) = delete;
  void operator =(const Self&) = delete;

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
#include "otbMultiChannelExtractROI.hxx"
#endif

#endif
