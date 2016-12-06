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
#ifndef otbWaveletsSynopsisImageToWaveletsBandsListFilter_h
#define otbWaveletsSynopsisImageToWaveletsBandsListFilter_h

#include "otbImageToImageListFilter.h"
#include "itkRegionOfInterestImageFilter.h"


namespace otb
{
/** \class WaveletsSynopsisImageToWaveletsBandsListFilter
 *  \brief This class convert the standard wavelets view to a list of bands.
 *
 * \ingroup Streamed
 */
template <class TImage, class TImageList>
class ITK_EXPORT WaveletsSynopsisImageToWaveletsBandsListFilter
      : public ImageToImageListFilter<TImage,typename TImageList::ImageType>
{
public:
  /** Standard typedefs */
  typedef WaveletsSynopsisImageToWaveletsBandsListFilter Self;
  typedef ImageToImageListFilter<TImage,
  typename TImageList::ImageType>                        Superclass;
  typedef itk::SmartPointer<Self>                        Pointer;
  typedef itk::SmartPointer<const Self>                  ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(WaveletsSynopsisImageToWaveletsBandsListFilter,ImageToImageListFilter);

  /** Template parameters typedefs */
  typedef TImage                                          InputImageType;
  typedef typename InputImageType::RegionType             RegionType;
  typedef TImageList                                      OutputImageListType;
  typedef typename OutputImageListType::ImageType         OutputImageType;

  typedef itk::RegionOfInterestImageFilter<
	  InputImageType,
	  InputImageType > ExtractFilterType;
  
  typedef typename ExtractFilterType::Pointer ExtractFilterPointerType;
  
  typedef std::vector<ExtractFilterPointerType>           ExtractFilterVectorType;

  /** Set the number of levels */
  itkSetMacro(NumberOfLevels,unsigned int);

  /** Get the number of levels */
  itkGetMacro(NumberOfLevels,unsigned int);

  /** Set the decimation ratio */
  itkSetMacro(DecimationRatio,unsigned int);

  /** Get the decimation ratio */
  itkGetMacro(DecimationRatio,unsigned int);

  /** If the filter is modified, the extract list need to be regenerated */
  virtual void Modified() const;

protected:
  /** Constructor */
  WaveletsSynopsisImageToWaveletsBandsListFilter();

  /** Destructor */
  virtual ~WaveletsSynopsisImageToWaveletsBandsListFilter();

  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Generate the input requested region from the first element in the list. */
  virtual void GenerateInputRequestedRegion(void);

  /** Generate the output information by building the output list. */
  virtual void GenerateOutputInformation(void);

  /** Main computation method */
  virtual void GenerateData(void);

private:
  WaveletsSynopsisImageToWaveletsBandsListFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** The number of levels in the decomposition */
  unsigned int m_NumberOfLevels;

  /** The decimation ratio used in the decomposition */
  unsigned int m_DecimationRatio;

  /** The vector of extraction filters for each band */
  ExtractFilterVectorType m_ExtractFilters;

  /** True if extract list is up-to-date */
  mutable bool m_ExtractFiltersUpToDate;
};
}// End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbWaveletsSynopsisImageToWaveletsBandsListFilter.txx"
#endif

#endif
