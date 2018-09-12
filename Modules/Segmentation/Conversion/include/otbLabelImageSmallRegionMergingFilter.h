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

#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"

namespace otb
{

/** \class PersistentLabelImageSmallRegionMergingFilter
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

  typedef itk::VariableLengthVector<double>                             RealVectorPixelType;
  
  typedef std::map<InputLabelType, std::set<InputLabelType> >                 NeigboursMapType;
  typedef std::vector<RealVectorPixelType >                                   LabelStatisticType;
  typedef std::vector<double>                                                 LabelPopulationType;
  typedef std::vector<InputLabelType>                                          LUTType;
  
  /** Set/Get size of polygon to be merged */
  itkGetMacro(Size , unsigned int);
  itkSetMacro(Size , unsigned int);

  /** Set the Label population  and initialize the LUT */
  void SetLabelPopulation( LabelPopulationType const & labelPopulation )
  {
    m_LabelPopulation = labelPopulation; 
    // Initialize m_CorrespondingMap to the identity (i.e. m[label] = label)
    m_LUT.resize( labelPopulation.size() );
    for (unsigned int i =0; i <labelPopulation.size(); i++)
    {
      m_LUT[ i ] = i;
    }
  }
  
  /** Get the Label population */
  LabelPopulationType const & GetLabelPopulation() const
  {
    return m_LabelPopulation;
  }

  /** Set the label statistic */
  void SetLabelStatistic( LabelStatisticType const & labelStatistic )
  {
    m_LabelStatistic = labelStatistic;
  }
  
  /** Get the label statistic */
  LabelStatisticType const & GetLabelStatistic() const
  {
    return m_LabelStatistic;
  }
  
  /** Get the LUT */
  LUTType const & GetLUT() const
  {
    return m_LUT;
  }
  
  virtual void Reset(void);
  virtual void Synthetize(void);

protected:
   void GenerateOutputInformation(void) override;

   void ThreadedGenerateData(const RegionType&
                outputRegionForThread, itk::ThreadIdType threadId) override;


  // Use m_LUT recurively to find the label corresponding to the input label
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
  
  /** Vector containing at position i the population of the segment labelled i */
  LabelPopulationType m_LabelPopulation;
  
  /** Vector containing at position i the population of mean of element of the segment labelled i*/
  LabelStatisticType m_LabelStatistic;
  
  /** Neigbours maps for each thread */
  std::vector <NeigboursMapType > m_NeighboursMapsTmp;
  
  /** LUT giving correspondance between labels in the original segmentation and the merged labels */
  LUTType m_LUT;
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
  typedef typename PersistentFilterType::LabelPopulationType LabelPopulationType;
  typedef typename PersistentFilterType::LabelStatisticType LabelStatisticType;
  typedef typename PersistentFilterType::LUTType LUTType;
  
  /** Sets the input image where the value of a pixel is the region id */
  void SetInputLabelImage( const InputLabelImageType * labelImage)
  {
    this->GetFilter()->SetInput( labelImage );
  }
  
  /** Returns input label image */
  InputLabelImageType * GetInputLabelImage()
  {
    return this->GetFilter()->GetInput();
  }
  
  /** Set size of segments to be merged */
  void SetSize(unsigned int size) 
  {
    this->GetFilter()->SetSize( size );
  }
  
  /** Get size of segments to be merged */
  unsigned int GetSize()
  {
    return this->GetFilter()->GetSize();
  }
  
  /** Set the Label population map */
  void SetLabelPopulation( LabelPopulationType const & labelPopulation )
  {
    this->GetFilter()->SetLabelPopulation( labelPopulation );
  }
  
  /** Get the Label population map */
  LabelPopulationType const & GetLabelPopulation(  ) const
  {
    return this->GetFilter()->GetLabelPopulation();
  }
  
  /** Set the Label statistic map */
  void SetLabelStatistic( LabelStatisticType const & labelStatistic )
  {
    this->GetFilter()->SetLabelStatistic( labelStatistic );
  }
  
  /** Get the Label statistic map */
  LabelStatisticType const & GetLabelStatistic( ) const
  {
    return this->GetFilter()->GetLabelStatistic();
  }
  
  LUTType const & GetLUT() const
  {
    return this->GetFilter()->GetLUT();
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
