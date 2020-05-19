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

#ifndef otbAttributesMapLabelObjectWithClassLabel_h
#define otbAttributesMapLabelObjectWithClassLabel_h

#include "otbAttributesMapLabelObject.h"

namespace otb
{

/** \class AttributesMapLabelObjectWithClassLabel
 *  \brief An AttributesMapLabelObject with an optional class label.
 *
 *  The label type is defined by the template parameter TClassLabel and
 *  accessible using the ClassLabelType typedef.
 *
 * The HasClassLabel() method returns true if the LabelObject has a
 * class label and false otherwise.
 *
 * The SetClassLabel() method set the class label and set the internal flag
 * m_HasClassLabel to true.
 *
 * The GetClassLabel() method returns the class label or an exception if m_HasClassLabel
 * is set to false.
 *
 * The RemoveClassLabel() method set m_HasClassLabel to false and the
 * class label value to a default value.
 *
 *\sa LabelObject, ShapeLabelObject, StatisticsLabelObject
 *
 * \ingroup DataRepresentation
 *
 * \ingroup OTBLabelMap
 */
template <class TLabel, unsigned int VImageDimension, class TAttributesValue, class TClassLabel>
class ITK_EXPORT AttributesMapLabelObjectWithClassLabel : public AttributesMapLabelObject<TLabel, VImageDimension, TAttributesValue>
{
public:
  /** Standard class typedefs */
  typedef AttributesMapLabelObjectWithClassLabel Self;
  typedef AttributesMapLabelObject<TLabel, VImageDimension, TAttributesValue> Superclass;
  typedef itk::LabelObject<TLabel, VImageDimension> LabelObjectType;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef itk::WeakPointer<const Self>  ConstWeakPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(AttributesMapLabelObjectWithClassLabel, AttributesMapLabelObject);

  itkStaticConstMacro(ImageDimension, unsigned int, VImageDimension);

  /** Class label typedef */
  typedef TClassLabel ClassLabelType;

  /** Set the class label */
  void SetClassLabel(const ClassLabelType& label)
  {
    m_ClassLabel    = label;
    m_HasClassLabel = true;
  }

  /** Get the class label. Throws an exception if no class label is
   *  available */
  const ClassLabelType& GetClassLabel() const
  {
    if (!m_HasClassLabel)
    {
      itkExceptionMacro(<< "LabelObject has no class label!");
    }
    return m_ClassLabel;
  }

  /** \return true if a class label is available */
  bool HasClassLabel() const
  {
    return m_HasClassLabel;
  }

  /** Invalidate the class label if any */
  void RemoveClassLabel()
  {
    m_ClassLabel    = itk::NumericTraits<ClassLabelType>::Zero;
    m_HasClassLabel = false;
  }

  void CopyAttributesFrom(const LabelObjectType* lo) override
  {
    Superclass::CopyAttributesFrom(lo);

    // copy the data of the current type if possible
    const Self* src = dynamic_cast<const Self*>(lo);
    if (src == nullptr)
    {
      return;
    }

    m_ClassLabel    = src->m_ClassLabel;
    m_HasClassLabel = src->m_HasClassLabel;
  }

protected:
  /** Constructor */
  AttributesMapLabelObjectWithClassLabel() : m_ClassLabel(itk::NumericTraits<ClassLabelType>::Zero), m_HasClassLabel(false)
  {
  }
  /** Destructor */
  ~AttributesMapLabelObjectWithClassLabel() override
  {
  }

  /** The printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override
  {
    Superclass::PrintSelf(os, indent);
    if (m_HasClassLabel)
    {
      os << indent << "Class Label: " << m_ClassLabel << std::endl;
    }
    else
    {
      os << indent << "No class label available." << std::endl;
    }
  }

private:
  AttributesMapLabelObjectWithClassLabel(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** The class label */
  ClassLabelType m_ClassLabel;

  /** Does the LabelObject have a class label ? */
  bool m_HasClassLabel;
};

} // end namespace otb
#endif
