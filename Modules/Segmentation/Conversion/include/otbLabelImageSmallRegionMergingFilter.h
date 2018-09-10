/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbLabelImageSmallRegionMergingFilter_h
#define otbLabelImageSmallRegionMergingFilter_h

#include "otbImage.h"
#include "otbVectorImage.h"
#include "itkImageToImageFilter.h"

#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include <set>

namespace otb
{

/** \class PersistentLabelImageSmallRegionMergingFilter
 *
 *
 * This class merges regions in the input label image according to the input
 * image of spectral values and the RangeBandwidth parameter.
 *
 *
 * \ingroup ImageSegmentation
 *
 * \ingroup OTBConversion
 */
template <class TInputLabelImage, class TInputSpectralImage >
class ITK_EXPORT PersistentLabelImageSmallRegionMergingFilter
  : public PersistentImageFilter<TInputLabelImage, TInputLabelImage>
{
public:
  /** Standard class typedef */
  typedef PersistentLabelImageSmallRegionMergingFilter                                Self;
  typedef PersistentImageFilter<TInputLabelImage, TInputLabelImage> Superclass;
  typedef itk::SmartPointer<Self>                                      Pointer;
  typedef itk::SmartPointer<const Self>                                ConstPointer;

  /** Type macro */
  itkTypeMacro(PersistentLabelImageSmallRegionMergingFilter, PersistentImageFilter);
  itkNewMacro(Self);

  /** Template parameters typedefs */

  typedef TInputLabelImage                        InputImageType;
  typedef typename InputImageType::Pointer        InputImagePointerType;
  typedef typename InputImageType::PixelType      InputPixelType;
  typedef typename InputImageType::IndexType      InputIndexType;
  typedef typename InputImageType::SizeType       InputSizeType;
  typedef typename InputImageType::IndexValueType InputIndexValueType;
  typedef typename InputImageType::PointType      PointType;
  typedef typename InputImageType::RegionType     RegionType;
  typedef typename InputImageType::SizeType       SizeType;
  
  typedef TInputLabelImage                        InputLabelImageType;
  typedef typename InputLabelImageType::PixelType InputLabelType;

  typedef TInputSpectralImage                     InputSpectralImageType;
  typedef typename TInputSpectralImage::PixelType SpectralPixelType;
  
  typedef itk::VariableLengthVector<double>                             RealVectorPixelType;
  typedef std::map<InputLabelType, double>                                    LabelPopulationMapType;
  typedef std::map<InputLabelType, std::set<InputLabelType> >                 NeigboursMapType;
  typedef std::map<InputLabelType, RealVectorPixelType >                        LabelStatisticMapType;
  typedef std::map<InputLabelType, InputLabelType>                      CorrespondanceMapType;
  /** Sets the input image where the value of a pixel is the region id */
  void SetInputLabelImage( const InputLabelImageType * labelImage);
  /** Sets the input image representing spectral values */
  void SetInputSpectralImage( const InputSpectralImageType * spectralImage);
  /** Returns input label image */
  InputLabelImageType * GetInputLabelImage();
  /** Returns input spectral image */
  InputSpectralImageType * GetInputSpectralImage();
  
  /** Set/Get size of polygon to be merged */
  itkGetMacro(Size , unsigned int);
  itkSetMacro(Size , unsigned int);

  /** Set/Get the Label population map and initialize the correspondance map*/
  void SetLabelPopulation( LabelPopulationMapType const & labelPopulation )
  {
    m_LabelPopulation = labelPopulation; 
    // Initialize m_CorrespondingMap to the identity (i.e. m[label] = label)
    for (auto label : labelPopulation)
    {
      m_CorrespondanceMap[ label.first ] = label.first;
    }
  }
  
  LabelPopulationMapType const & GetLabelPopulation() const
  {
    return m_LabelPopulation;
  }

  void SetLabelStatistic( LabelStatisticMapType const & labelStatistic )
  {
    m_LabelStatistic = labelStatistic;
  }
  
  LabelStatisticMapType const & GetLabelStatistic() const
  {
    return m_LabelStatistic;
  }
  
  CorrespondanceMapType const & GetCorrespondanceMap() const
  {
    return m_CorrespondanceMap;
  }
  
  virtual void Reset(void);
  virtual void Synthetize(void);

protected:
  //void EnlargeOutputRequestedRegion( itk::DataObject *output ) override;

   void GenerateOutputInformation(void) override;

   void ThreadedGenerateData(const RegionType&
                outputRegionForThread, itk::ThreadIdType threadId) override;


  // Use m_CorrespondanceMap recurively to find the label corresponding to the input label
  InputLabelType FindCorrespondingLabel( InputLabelType label);

  /** Constructor */
  PersistentLabelImageSmallRegionMergingFilter();

  /** Destructor */
  ~PersistentLabelImageSmallRegionMergingFilter() override;

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  PersistentLabelImageSmallRegionMergingFilter(const Self &) = delete;
  void operator =(const Self&) = delete;
  
  unsigned int m_Size;
  LabelPopulationMapType m_LabelPopulation;
  
  LabelStatisticMapType m_LabelStatistic;
  
  // Neigbours maps for each thread
  std::vector <NeigboursMapType > m_NeighboursMapsTmp;
  
  CorrespondanceMapType m_CorrespondanceMap;
  //NeigboursMapType m_NeighboursMap;
};

/** \class LabelImageSmallRegionMergingFilter
 *
 *
 * This class merges regions in the input label image according to the input
 * image of spectral values and the RangeBandwidth parameter.
 *
 *
 * \ingroup ImageSegmentation
 *
 * \ingroup OTBConversion
 */
template <class TInputLabelImage,  class TInputSpectralImage>
class ITK_EXPORT LabelImageSmallRegionMergingFilter :
public PersistentFilterStreamingDecorator<PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage, TInputSpectralImage> >
{
  
public:
  /** Standard Self typedef */
  typedef LabelImageSmallRegionMergingFilter                                                 Self;
  typedef PersistentFilterStreamingDecorator
    <PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage, TInputSpectralImage> >   Superclass;
  typedef itk::SmartPointer<Self>                                                            Pointer;
  typedef itk::SmartPointer<const Self>                                                      ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(LabelImageSmallRegionMergingFilter, PersistentFilterStreamingDecorator);

  typedef PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage, TInputSpectralImage> PersistentFilterType;
  typedef typename PersistentFilterType::InputLabelImageType InputLabelImageType;
  typedef typename PersistentFilterType::InputSpectralImageType InputSpectralImageType;
  typedef typename PersistentFilterType::LabelPopulationMapType LabelPopulationMapType;
  typedef typename PersistentFilterType::LabelStatisticMapType LabelStatisticMapType;
  typedef typename PersistentFilterType::CorrespondanceMapType CorrespondanceMapType;
  
  /** Sets the input image where the value of a pixel is the region id */
  void SetInputLabelImage( const InputLabelImageType * labelImage)
  {
    this->GetFilter()->SetInputLabelImage( labelImage );
  }
  
  
  /** Sets the input image representing spectral values */
  void SetInputSpectralImage( const InputSpectralImageType * spectralImage)
  {
    this->GetFilter()->SetInputSpectralImage( spectralImage );
  }
  
  /** Returns input label image */
  InputLabelImageType * GetInputLabelImage()
  {
    return this->GetFilter()->GetInputLabelImage();
  }
  
  /** Returns input spectral image */
  InputSpectralImageType * GetInputSpectralImage()
  {
    return this->GetFilter()->GetInputSpectralImage();
  }
  
  /** Set size of polygon to be merged */
  void SetSize(unsigned int size) 
  {
    this->GetFilter()->SetSize( size );
  }
  
  /** Get size of polygon to be merged */
  unsigned int GetSize()
  {
    return this->GetFilter()->GetSize();
  }
  
  /** Set the Label population map */
  void SetLabelPopulation( LabelPopulationMapType const & labelPopulation )
  {
    this->GetFilter()->SetLabelPopulation( labelPopulation );
  }
  
  /** Get the Label population map */
  LabelPopulationMapType const & GetLabelPopulation(  ) const
  {
    return this->GetFilter()->GetLabelPopulation();
  }
  
  /** Set the Label statistic map */
  void SetLabelStatistic( LabelStatisticMapType const & labelStatistic )
  {
    this->GetFilter()->SetLabelStatistic( labelStatistic );
  }
  
  /** Get the Label statistic map */
  LabelStatisticMapType const & GetLabelStatistic( ) const
  {
    return this->GetFilter()->GetLabelStatistic();
  }
  
  CorrespondanceMapType const & GetCorrespondanceMap() const
  {
    return this->GetFilter()->GetCorrespondanceMap();
  }
  
  
protected:
  /** Constructor */
  LabelImageSmallRegionMergingFilter() {}
  /** Destructor */
  ~LabelImageSmallRegionMergingFilter() override {}

private:
  LabelImageSmallRegionMergingFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelImageSmallRegionMergingFilter.hxx"
#endif

#endif
