/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


#ifndef otbHooverMatrixFilter_h
#define otbHooverMatrixFilter_h

#include "itkLabelMapFilter.h"
#include "itkVariableSizeMatrix.h"

namespace otb
{
/** \class HooverMatrixFilter
 *
 * \brief This class computes the confusion matrix from two LabelMapObject
 *
 * The confusion matrix stores the number of pixel inside the intersection between
 * couples of region. These couples are made from a ground truth segmentation and
 * a machine segmentation. The line number gives the index of the ground truth region. The
 * column number gives the index of the machine segmentation region.
 *
 * \ingroup OTBMetrics
 */

template <class TLabelMap>
class ITK_EXPORT HooverMatrixFilter : public itk::LabelMapFilter<TLabelMap, TLabelMap>
{
public:
  /** Standard class typedefs. */
  typedef HooverMatrixFilter Self;
  typedef itk::LabelMapFilter<TLabelMap, TLabelMap> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard New method. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(HooverMatrixFilter, LabelMapFilter);

  /** Some convenient typedefs. */
  typedef TLabelMap                              LabelMapType;
  typedef typename LabelMapType::LabelObjectType LabelObjectType;
  typedef typename LabelMapType::LabelVectorType LabelVectorType;

  // typedef typename LabelObjectType::LineContainerType   LineContainerType;
  typedef typename LabelObjectType::IndexType IndexType;
  typedef typename LabelObjectType::LabelType LabelType;

  typedef unsigned long                            CoefficientType;
  typedef itk::VariableSizeMatrix<CoefficientType> MatrixType;

  /** Set the ground truth label map */
  void SetGroundTruthLabelMap(const LabelMapType* gt);

  /** Set the machine segmentation label map */
  void SetMachineSegmentationLabelMap(const LabelMapType* ms);

  /** Get the ground truth label map */
  const LabelMapType* GetGroundTruthLabelMap();

  /** Get the machine segmentation label map */
  const LabelMapType* GetMachineSegmentationLabelMap();

  /** Get the output Hoover confusion matrix */
  MatrixType& GetHooverConfusionMatrix()
  {
    return m_Matrix;
  }

protected:
  /** Constructor */
  HooverMatrixFilter();

  ~HooverMatrixFilter() override{};

  /** Action :  Resize the matrix
   */
  void BeforeThreadedGenerateData() override;

  /** Action : fill the line of the confusion matrix corresponding to
   * the given label object
   */
  void ThreadedProcessLabelObject(LabelObjectType* labelObject) override;

private:
  /** Number of label objects found in the ground truth (GT) label maps */
  unsigned long m_NumberOfRegionsGT;

  /** Number of label objects found in the machine segmentation (MS) label maps */
  unsigned long m_NumberOfRegionsMS;

  /** List of labels in GT label map */
  LabelVectorType m_LabelsGT;

  /** Hoover confusion matrix */
  MatrixType m_Matrix;
};
}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbHooverMatrixFilter.hxx"
#endif

#endif
