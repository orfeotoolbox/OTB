/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#include <unordered_map>

namespace otb
{

/** \class PersistentLabelImageSmallRegionMergingFilter
 *
 * This class can be used to merge each segments of a given size in a label
 * image to the connected segment with the closest radiometry (in the sense of
 * the euclidean squared distance).
 * This persistent filter should be used as template parameter of a
 * PersistentFilterStreamingDecorator.
 * It computes from an input label image an equivalence table
 * that gives for each pixel, the corresponding label in the merged image.
 * The merged image can then be computed using a ChangeLabelImageFilter.
 *
 * This filter can be updated several times for different values of size,
 * the output equivalence table will be the results of all computations.
 *
 * \ingroup ImageSegmentation
 *
 * \ingroup OTBConversion
 */
template <class TInputLabelImage>
class ITK_EXPORT PersistentLabelImageSmallRegionMergingFilter : public PersistentImageFilter<TInputLabelImage, TInputLabelImage>
{
public:
  /** Standard class typedef */
  typedef PersistentLabelImageSmallRegionMergingFilter Self;
  typedef PersistentImageFilter<TInputLabelImage, TInputLabelImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkTypeMacro(PersistentLabelImageSmallRegionMergingFilter, PersistentImageFilter);
  itkNewMacro(Self);

  /** Template parameters typedefs */

  typedef TInputLabelImage                    InputImageType;
  typedef typename InputImageType::Pointer    InputImagePointerType;
  typedef typename InputImageType::PixelType  InputLabelType;
  typedef typename InputImageType::SizeType   InputSizeType;
  typedef typename InputImageType::PointType  PointType;
  typedef typename InputImageType::RegionType RegionType;

  typedef itk::VariableLengthVector<double> RealVectorPixelType;

  typedef std::unordered_map<InputLabelType, std::set<InputLabelType>> NeighboursMapType;

  typedef std::unordered_map<InputLabelType, RealVectorPixelType> LabelStatisticType;
  typedef std::unordered_map<InputLabelType, double>              LabelPopulationType;
  typedef std::unordered_map<InputLabelType, InputLabelType>      LUTType;

  /** Set/Get size of segments to be merged */
  itkGetMacro(Size, unsigned int);
  itkSetMacro(Size, unsigned int);

  /** Set the Label population  and initialize the LUT */
  void SetLabelPopulation(LabelPopulationType const& labelPopulation);

  /** Get the Label population */
  LabelPopulationType const& GetLabelPopulation() const;

  /** Set the label statistic */
  void SetLabelStatistic(LabelStatisticType const& labelStatistic);

  /** Get the label statistic */
  LabelStatisticType const& GetLabelStatistic() const;

  /** Get the LUT */
  LUTType const& GetLUT() const;

  virtual void Reset(void) override;
  virtual void Synthetize(void) override;

protected:
  /** The input requested region should be padded by a radius of 1 to use the
   * neighbourhood iterator */
  void GenerateInputRequestedRegion() override;

  /** Threaded Generate Data : find the neighbours of each segments of size
   * m_Size for each tile and store them in an accumulator */
  void ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId) override;


  /** Use the LUT recursively to find the label corresponding to the input
   * label */
  InputLabelType FindCorrespondingLabel(InputLabelType label);

  /** Constructor */
  PersistentLabelImageSmallRegionMergingFilter();

  /** Destructor */
  ~PersistentLabelImageSmallRegionMergingFilter() override = default;

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  PersistentLabelImageSmallRegionMergingFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** Size of the segments to be merged */
  unsigned int m_Size;

  /** Map containing at key i the population of the segment labelled i */
  LabelPopulationType m_LabelPopulation;

  /** Map containing at key i the mean of element of the segment labelled i */
  LabelStatisticType m_LabelStatistic;

  /** Neighbours maps for each thread */
  std::vector<NeighboursMapType> m_NeighboursMapsTmp;

  /** LUT giving correspondence between labels in the original segmentation
   * and the merged labels */
  LUTType m_LUT;
};

/** \class LabelImageSmallRegionMergingFilter
 *
 * This filter computes from a label image an equivalence table that gives for
 * each pixel, the corresponding label in the merged image. It uses a
 * PersistentFilterStreamingDecorator templated over a
 * PersistentLabelImageSmallRegionMergingFilter
 * to merge the segments recursively from segment of size 1 to segment of a
 * size specified by the attribute MinSize.
 * The equivalence table can be accessed with the method GetLut and used to
 * compute the merged image with a ChangeLabelImageFilterType.
 *
 * \ingroup ImageSegmentation
 *
 * \ingroup OTBConversion
 */
template <class TInputLabelImage>
class ITK_EXPORT LabelImageSmallRegionMergingFilter : public itk::ProcessObject
{
public:
  /** Standard Self typedef */
  typedef LabelImageSmallRegionMergingFilter Self;
  typedef itk::ProcessObject                 Superclass;
  typedef itk::SmartPointer<Self>            Pointer;
  typedef itk::SmartPointer<const Self>      ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(LabelImageSmallRegionMergingFilter, itk::ProcessObject);

  // Small region merging filter typedefs
  typedef PersistentLabelImageSmallRegionMergingFilter<TInputLabelImage>                       PersistentLabelImageSmallRegionMergingFilterType;
  typedef PersistentFilterStreamingDecorator<PersistentLabelImageSmallRegionMergingFilterType> LabelImageSmallRegionMergingFilterType;

  typedef typename PersistentLabelImageSmallRegionMergingFilterType::LabelPopulationType LabelPopulationType;

  typedef typename PersistentLabelImageSmallRegionMergingFilterType::LabelStatisticType LabelStatisticType;

  typedef typename PersistentLabelImageSmallRegionMergingFilterType::LUTType LUTType;


  /** Set/Get size of polygon to be merged */
  itkGetMacro(MinSize, unsigned int);
  itkSetMacro(MinSize, unsigned int);

  /** Set the Label population map */
  void SetInputLabelImage(const TInputLabelImage* labelImage);

  /** Set the Label population map */
  void SetLabelPopulation(LabelPopulationType const& labelPopulation);

  /** Get the Label population map */
  LabelPopulationType const& GetLabelPopulation() const;

  /** Set the Label statistic map */
  void SetLabelStatistic(LabelStatisticType const& labelStatistic);

  /** Get the Label statistic map */
  LabelStatisticType const& GetLabelStatistic() const;

  /** Get the Label statistic map */
  LUTType const& GetLUT() const;

  /** Call GenerateData() */
  void Update() override;

protected:
  /** Constructor */
  LabelImageSmallRegionMergingFilter();
  /** Destructor */
  ~LabelImageSmallRegionMergingFilter() override = default;

  /** Generate Data method (Update LabelImageSmallRegionMergingFilterType
   * recursively) */
  void GenerateData() override;

private:
  LabelImageSmallRegionMergingFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  // Filter used recursively to build the equivalence table
  typename LabelImageSmallRegionMergingFilterType::Pointer m_SmallRegionMergingFilter;

  // All segments with size < m_MinSize will be merged to bigger segments.
  unsigned int m_MinSize;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelImageSmallRegionMergingFilter.hxx"
#endif

#endif
