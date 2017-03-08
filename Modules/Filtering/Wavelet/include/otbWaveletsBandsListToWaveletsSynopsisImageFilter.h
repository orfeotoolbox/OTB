/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef otbWaveletsBandsListToWaveletsSynopsisImageFilter_h
#define otbWaveletsBandsListToWaveletsSynopsisImageFilter_h


#include "otbImageListToImageFilter.h"

namespace otb
{
  
/** \class WaveletsBandsListToWaveletsSynopsisImageFilter
 *  \brief Converts a list of wavelets bands to the traditional multi-resolution wavelets view
 *
 *  The decimation ratio is taken into account.
 *
 * \ingroup OTBWavelet
 * \sa WaveletsSynopsisImageToWaveletsBandsListFilter
 */

template <class TImageList, class TImage>
class ITK_EXPORT WaveletsBandsListToWaveletsSynopsisImageFilter
      : public ImageListToImageFilter<typename TImageList::ImageType, TImage>
{
public:
  /** Standard typedefs */
  typedef WaveletsBandsListToWaveletsSynopsisImageFilter Self;
  typedef ImageListToImageFilter<
          typename TImageList::ImageType,TImage>         Superclass;
  typedef itk::SmartPointer<Self>                        Pointer;
  typedef itk::SmartPointer<const Self>                  ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Useful typedefs */
  typedef TImageList                                     InputImageListType;
  typedef typename InputImageListType::ImageType         InputImageType;
  typedef TImage                                         OutputImageType;
  typedef typename OutputImageType::RegionType           RegionType;

  /** Creation through object factory macro */
  itkTypeMacro(WaveletsBandsListToWaveletsSynopsisImageFilter,ImageToImageListFilter);

  /** Set the decimation ratio */
  itkSetMacro(DecimationRatio,unsigned int);

  /** Get the decimation ratio */
  itkGetMacro(DecimationRatio,unsigned int);

protected:
  /** Main computation method */
  virtual void ThreadedGenerateData(const RegionType & outputRegionForThread, itk::ThreadIdType threadId);

  /** GenerateOutputInformation
   * Set the number of bands of the output.
   * Copy informations from the first image of the list if existing.
   **/
  virtual void GenerateOutputInformation(void);

  /**
   * GenerateInputRequestedRegion
   * Set the requested region of each image in the list.
   */
  virtual void GenerateInputRequestedRegion(void);

  /** Constructor */
  WaveletsBandsListToWaveletsSynopsisImageFilter();

  /** Destructor */
  virtual ~WaveletsBandsListToWaveletsSynopsisImageFilter();

  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  WaveletsBandsListToWaveletsSynopsisImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** The decimation ratio used in the decomposition */
  unsigned int m_DecimationRatio;
};
}// End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbWaveletsBandsListToWaveletsSynopsisImageFilter.txx"
#endif

#endif
