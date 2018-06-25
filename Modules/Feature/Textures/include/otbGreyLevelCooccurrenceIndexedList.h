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


#ifndef otbGreyLevelCooccurrenceIndexedList_h
#define otbGreyLevelCooccurrenceIndexedList_h

#include "itkNumericTraits.h"
#include "itkObjectFactory.h"
#include "itkLightObject.h"
#include "itkArray.h"
#include "itkIndex.h"
#include "itkSize.h"
#include <utility>
#include <vector>

namespace otb
{
/** \class GreyLevelCooccurrenceIndexedList
* \brief This class holds a VectorType of CooccurrencePairType with each pair is
* a combination of pixel index and frequency.
*
* GreyLevelCooccurrenceIndexedList instance is created for every neighborhood
* iteration over the given input image. This class keep an internal itk::Array
* as a lookup array with size as [nbbins x nbbins]. The lookup array stores
* position CooccurrencePairType in the VectorType. It ensures us that all elements
* in Vector are unqiue in terms of the index value in the pair. For any given
* pixel index, -1 value indicates zero existence of the index in the
* VectorType. This avoid searching all elements in VectorType for each pixel
* index added during neighborhood iterator. It is also used to decide wheather
* to update the frequency of pair or to insert a new element in the vector.
*
* Print references:
* David A. Clausi and Yongping Zhao. 2002. Rapid extraction of image texture by
* co-occurrence using a hybrid data structure. Comput. Geosci. 28, 6 (July
* 2002), 763-774. DOI=10.1016/S0098-3004(01)00108-X
* http://dx.doi.org/10.1016/S0098-3004(01)00108-X
*
* de O.Bastos, L.; Liatsis, P.; Conci, A., Automatic texture segmentation based
* on k-means clustering and efficient calculation of co-occurrence
* features. Systems, Signals and Image Processing, 2008. IWSSIP 2008. 15th
* International Conference on , vol., no., pp.141,144, 25-28 June 2008
* doi: 10.1109/IWSSIP.2008.4604387
 *
 * \ingroup OTBTextures
*/

template <class TPixel>
class ITK_EXPORT GreyLevelCooccurrenceIndexedList : public itk::LightObject
{
public:

  /** Standard typedefs */
  typedef GreyLevelCooccurrenceIndexedList Self;
  typedef itk::LightObject                 Superclass;
  typedef itk::SmartPointer<Self>          Pointer;
  typedef itk::SmartPointer<const Self>    ConstPointer;

  /** Creation through the object factory */
  itkNewMacro(Self);

  /** RTTI */
  itkTypeMacro(GreyLevelCooccurrenceIndexedList, itk::LightObject);

  typedef TPixel PixelType;

  itkStaticConstMacro(PixelPairSize, unsigned int, 2);

  typedef itk::Index<PixelPairSize>  IndexType;
  typedef itk::Size<PixelPairSize>   SizeType;
  typedef typename itk::IndexValueType    IndexValueType;

  typedef typename itk::IdentifierType InstanceIdentifier;
  typedef InstanceIdentifier FrequencyType;
  typedef typename itk::NumericTraits< FrequencyType >::RealType RelativeFrequencyType;
  typedef typename itk::NumericTraits< FrequencyType >::AccumulateType TotalFrequencyType;

  typedef typename itk::NumericTraits< PixelType >::RealType PixelValueType;
  typedef itk::FixedArray<PixelValueType, PixelPairSize> PixelPairType;
  // lower bound of each bin
  std::vector< std::vector< PixelValueType > > m_Min;
  // upper bound of each bin
  std::vector< std::vector< PixelValueType > > m_Max;

  /* std::pair to hold pixel pair and its frequency count */
  typedef std::pair<IndexType, FrequencyType> CooccurrencePairType;

  /** Lookup array used to store the index of the given pixel pair. Size of
    * LookupAray is equal to the nbins * nbins. Values in the array represents 1D
    * index of the IndexType*/
  typedef itk::Array<int> LookupArrayType;

  /** std::vector to hold CooccurrencePairType. Index of the array where the
    * cooccurrence is stored is saved in the LookupArrayType.*/
  typedef std::vector<CooccurrencePairType> VectorType;

  /** Get the total frequency of Co-occurrence pairs. */
  itkGetMacro(TotalFrequency, TotalFrequencyType);

  /** Get the total frequency of Co-occurrence pairs. */
  itkGetMacro(Symmetry, bool);

  /** Get std::vector containing non-zero co-occurrence pairs */
  VectorType GetVector();

  /** Initialize the lowerbound and upper bound vecotor, Fill m_LookupArray with
    * -1 and set m_TotalFrequency to zero */
  void Initialize(const unsigned int nbins, const PixelValueType min,
                  const PixelValueType max, const bool symmetry = true);

  //check if both pixel values fall between m_InputImageMinimum and
  //m_InputImageMaximum. If so add to m_Vector via AddPairToVector method */
  void AddPixelPair(const PixelValueType& pixelvalue1, const PixelValueType& pixelvalue2);

  /* Get the frequency value from Vector with index =[j,i] */
  RelativeFrequencyType GetFrequency(IndexValueType i, IndexValueType j);

  /* Get the frequency value from Vector with index =[j,i] from given
   * vector. This is used if we have a copy of m_Vector normalized. */
  RelativeFrequencyType GetFrequency(IndexValueType i, IndexValueType j, const VectorType& vect) const;

protected:
  GreyLevelCooccurrenceIndexedList();
  ~GreyLevelCooccurrenceIndexedList() override { }

  /** create a cooccurrence pair with given index and frequency = 1
    * value. Next occurrence of same index is checked via m_LookupArray and the
    * corresponding frequency value is incremented. If m_Symmetry is true the
    * co-occurrence pair is added again with index values swapped */
  void AddPairToVector(IndexType index);

  void SetBinMin(const unsigned int dimension, const InstanceIdentifier nbin,
                 PixelValueType min);

  void SetBinMax(const unsigned int dimension, const InstanceIdentifier nbin,
                 PixelValueType max);

  /** Get index of the pixelPair combination and save the result in index **/
  bool GetIndex(const PixelPairType & pixelPair, IndexType & index) const;

  void PrintSelf(std::ostream & os, itk::Indent indent) const override;

private:

  GreyLevelCooccurrenceIndexedList(const Self&) = delete;
  void operator =(const Self&) = delete;

  /** LookupArray instance */
  LookupArrayType m_LookupArray;

  /* std::vector holding actual co-occurrence pairs */
  VectorType m_Vector;

  /* Size instance */
  SizeType m_Size;

  /** booleab to check if co-occurrene list is a symmtrical. true by default */
  bool m_Symmetry;

  /* Store the total frequency of co-occurrence pairs in GreyLevelCooccureneIndexedList class */
  TotalFrequencyType m_TotalFrequency;

  /** boolean to check pixel values fall under m_InputImageMinimum and
    * m_InputImageMaximum. Used in GetIndex method. false by default */
  bool m_ClipBinsAtEnds;

  /** Input image minimum */
  PixelValueType m_InputImageMinimum;

  /** Input image maximum */
  PixelValueType m_InputImageMaximum;
};

} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGreyLevelCooccurrenceIndexedList.hxx"
#endif

#endif
