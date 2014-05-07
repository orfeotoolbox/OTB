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

#ifndef __otbGreyLevelCooccurrenceIndexedList_h
#define __otbGreyLevelCooccurrenceIndexedList_h

#include "itkLightObject.h"
#include "itkArray.h"
#include "itkHistogram.h"
#include <vector>

namespace otb
{

/** \class GreyLevelCooccurrenceIndexedList
* \brief This class holds unique non-zero co-occurrence pairs called
* CooccurrenceIndexedList (GLCIL) instead of GLCM.
* GreyLevelCooccurrenceIndexedList instance is created for every neighborhood
* iteration over the given input image. This class keep an internal itk::Array
* as a lookup array with the same size of the histogram created in the Filter
* which uses it. The lookup array stores the position of each pixel pair in an
* std::vector. A pixel pair is a combination of (Center, Offset)
* Pixels. A -1 value in the lookup array indicates zero existance of pixel
* pair. Index stored in the CooccurrencePairType is the index of the pixel pair
* calculated from the histogram. Histogram instance is created outside this
* class.
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
*/

template <class THistogram >
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

  typedef THistogram                                          HistogramType;
  typedef typename HistogramType::RelativeFrequencyType       RelativeFrequencyType;
  typedef typename HistogramType::TotalAbsoluteFrequencyType  TotalAbsoluteFrequencyType;
  typedef typename HistogramType::IndexType                   IndexType;
  typedef typename HistogramType::SizeType                    SizeType;
  typedef typename IndexType::ValueType                       IndexValueType;
  typedef typename HistogramType::InstanceIdentifier          InstanceIdentifier;

  /** struct to hold non-zero frequency and its index in the histogram */
  struct CooccurrencePairType
  {
    IndexType index;
    RelativeFrequencyType frequency;
  };

  /** Lookup array used to store the index of the given pixel pair. Size of
    * LookupAray is equal to the nbins * nbins. Values in the array represents 1D
    * index of the elements in the Histogram */
  typedef itk::Array<int> LookupArrayType;

  /** std::vector to hold CooccurrencePairType. Index of the array where the
    * cooccurrence is stored is saved in the LookupArrayType.*/
  typedef std::vector<CooccurrencePairType> VectorType;

  /** Get the total frequency of Co-occurrence pairs. */
  itkGetMacro(TotalFrequency, TotalAbsoluteFrequencyType);

  GreyLevelCooccurrenceIndexedList();

  ~GreyLevelCooccurrenceIndexedList() { }

  /** Fill m_LookupArray with -1 and set m_TotalFreqency to zero */
  void Initialize(SizeType size, bool symmetry = true);

  /** Get std::vector containing non-zero co-occurrence pairs */
  VectorType GetVector();

  /** Normailze the GLCIL */
  void Normalize();

  /** Add the CooccurrencePairType to list. Next occurrence of same index is
    * checked via m_LookupArray value and the correspoding frequency value is
    * incremented. If m_Symmetry is true the co-occurrence pair is added again */
  void AddPairToList(IndexType index);

  /** Update frequency of the pair with given index */
  void AddIndex(IndexType index);

  /* Get the frequency value from Vector with index =[j,i] */
  RelativeFrequencyType GetFrequency(IndexValueType i, IndexValueType j);

  /* Get the frequency value from Vector with index =[j,i] from given
   * vector. This is used if we have a copy of m_Vector normalized. */
  RelativeFrequencyType GetFrequency(IndexValueType i, IndexValueType j, const VectorType& vect) const;

private:

  /* Store the total frequency of co-occurrence pairs in GreyLevelCooccureneIndexedList class */
  TotalAbsoluteFrequencyType m_TotalFrequency;

  /** LookupArray instance */
  LookupArrayType m_LookupArray;

  /* std::vector holding actual co-occurrence pairs */
  VectorType m_Vector;

  /* Size instance */
  SizeType m_Size;

  bool m_Symmetry;
};

} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGreyLevelCooccurrenceIndexedList.txx"
#endif

#endif
