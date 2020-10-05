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

#ifndef otbContingencyTable_h
#define otbContingencyTable_h

#include <vector>
#include <iomanip>
#include <itkObject.h>
#include <itkObjectFactory.h>
#include <itkVariableSizeMatrix.h>
#include <string>

namespace otb
{
template <class TClassLabel>
class ContingencyTable : public itk::Object
{
public:
  /** Standard class typedefs */
  typedef ContingencyTable              Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ContingencyTableCalculator, itk::Object);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef itk::VariableSizeMatrix<unsigned long> MatrixType;
  typedef std::vector<TClassLabel>               LabelList;

  MatrixType matrix;

  void SetLabels(const LabelList& referenceLabels, const LabelList& producedLabels)
  {
    m_RefLabels       = referenceLabels;
    m_ProdLabels      = producedLabels;
    unsigned int rows = static_cast<unsigned int>(m_RefLabels.size());
    unsigned int cols = static_cast<unsigned int>(m_ProdLabels.size());
    matrix.SetSize(rows, cols);
    matrix.Fill(0);
  }

  friend std::ostream& operator<<(std::ostream& o, const ContingencyTable<TClassLabel>& contingencyTable)
  {

    // Retrieve the maximal width from the matrix and the labels
    size_t maxWidth = 6;
    maxWidth        = GetLabelsMaximumLength(contingencyTable.m_ProdLabels, maxWidth);
    maxWidth        = GetLabelsMaximumLength(contingencyTable.m_RefLabels, maxWidth);

    for (unsigned int i = 0; i < contingencyTable.matrix.Rows(); ++i)
    {
      for (unsigned int j = 0; j < contingencyTable.matrix.Cols(); ++j)
      {
        std::ostringstream oss;
        oss << contingencyTable.matrix(i, j);
        size_t length = oss.str().length();
        if (length > maxWidth)
          maxWidth = length;
      }
    }

    int width = static_cast<int>(maxWidth) + 1;

    // Write the first line of the matrix (produced labels)
    o << std::setfill(' ') << std::setw(width) << "labels";
    for (size_t i = 0; i < contingencyTable.m_ProdLabels.size(); ++i)
    {
      o << std::setfill(' ') << std::setw(width) << contingencyTable.m_ProdLabels[i];
    }
    o << std::endl;

    // For each line write the reference label, then the count value
    for (unsigned int i = 0; i < contingencyTable.matrix.Rows(); ++i)
    {
      o << std::setfill(' ') << std::setw(width) << contingencyTable.m_RefLabels[i];
      for (unsigned int j = 0; j < contingencyTable.matrix.Cols(); ++j)
      {
        o << std::setfill(' ') << std::setw(width) << contingencyTable.matrix(i, j);
      }
      o << std::endl;
    }

    return o;
  }

  std::string ToCSV() const
  {
    const char separator = ',';

    std::ostringstream oss;
    oss << "labels";
    for (size_t i = 0; i < m_ProdLabels.size(); ++i)
    {
      oss << separator << m_ProdLabels[i];
    }
    oss << std::endl;

    // For each line write the reference label, then the count value
    for (unsigned int i = 0; i < matrix.Rows(); ++i)
    {
      oss << m_RefLabels[i];
      for (unsigned int j = 0; j < matrix.Cols(); ++j)
      {
        oss << separator << matrix(i, j);
      }
      oss << std::endl;
    }
    oss << std::endl;

    return oss.str();
  }

protected:
  ContingencyTable()
  {
    SetLabels(LabelList(), LabelList());
  }
  ~ContingencyTable() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent itkNotUsed(indent)) const override
  {
    os << *this;
  }

private:
  ContingencyTable(const Self&) = delete;
  void operator=(const Self&) = delete;

  static size_t GetLabelsMaximumLength(const LabelList& labels, size_t maxWidth)
  {
    size_t tmpMaxWidth = maxWidth;
    for (size_t i = 0; i < labels.size(); ++i)
    {
      std::ostringstream oss;
      oss << labels[i];
      size_t length = oss.str().length();
      if (length > tmpMaxWidth)
        tmpMaxWidth = length;
    }
    return tmpMaxWidth;
  }

  LabelList m_RefLabels;
  LabelList m_ProdLabels;
};
}

#endif // otbContingencyTable_h
