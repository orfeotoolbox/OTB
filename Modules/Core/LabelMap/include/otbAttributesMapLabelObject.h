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

#ifndef otbAttributesMapLabelObject_h
#define otbAttributesMapLabelObject_h

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "itkShapeLabelObject.h"
#pragma GCC diagnostic pop
#else
#include "itkShapeLabelObject.h"
#endif

#include "otbPolygon.h"
#include <map>
#include <string>

namespace otb
{

namespace Functor
{

/** \class AttributesMapLabelObjectAccessor
 *  \brief Allows accessing a given field of an AttributesMapLabelObject
 *
 * The name of the attribute to retrieve can be set by using the
 * SetAttributeName method.
 *
 * \sa AttributesMapLabelObject
 *
 * \ingroup OTBLabelMap
 */
template <class TLabelObject>
class ITK_EXPORT AttributesMapLabelObjectAccessor
{
public:
  // The Label Object typedef
  typedef TLabelObject                                  LabelObjectType;
  typedef typename LabelObjectType::AttributesValueType AttributeValueType;

  /**
   * Please note that no exception catching is done here.
   * \param labelObject The pointer to the label object
   * \return The attribute value.
   */
  inline const AttributeValueType operator()(LabelObjectType* labelObject) const
  {
    return labelObject->GetAttribute(m_AttributeName.c_str());
  }

  /// Set the name of the attribute to retrieve
  void SetAttributeName(const char* name)
  {
    m_AttributeName = name;
  }
  /// Get the the name of the attribute to retrieve
  const char* GetAttributeName() const
  {
    return m_AttributeName.c_str();
  }

  /// Constructor
  AttributesMapLabelObjectAccessor() : m_AttributeName("")
  {
  }

  /// Destructor
  ~AttributesMapLabelObjectAccessor()
  {
  }

private:
  /// Name of the attribute to retrieve
  std::string m_AttributeName;
};


/** \class AttributesMapMeasurementFunctor
*   \brief This class allows building a measurement vector from an AttributesMapLabelObject
*
*    It Allows selecting only a subset of the available attributes.
 *
 * \ingroup OTBLabelMap
*/
template <class TLabelObject, class TMeasurementVector>
class AttributesMapMeasurementFunctor
{
public:
  typedef std::vector<std::string> AttributesListType;

  inline TMeasurementVector operator()(const TLabelObject* object) const
  {
    TMeasurementVector newSample(m_Attributes.size());

    unsigned int                                attrIndex = 0;
    typename AttributesListType::const_iterator attrIt    = m_Attributes.begin();
    while (attrIt != m_Attributes.end())
    {
      newSample[attrIndex] = object->GetAttribute(attrIt->c_str());
      ++attrIt;
      ++attrIndex;
    }
    return newSample;
  }

  /** Add an attribute to the exported attributes list */
  void AddAttribute(const char* attr)
  {
    m_Attributes.push_back(attr);
  }

  /** Remove an attribute from the exported attributes list */
  void RemoveAttribute(const char* attr)
  {
    AttributesListType::iterator elt = std::find(m_Attributes.begin(), m_Attributes.end(), attr);
    if (elt != m_Attributes.end())
    {
      m_Attributes.erase(elt);
    }
  }

  /** Remove all attributes from the exported attributes list */
  void ClearAttributes()
  {
    m_Attributes.clear();
  }

  /** Get The number of exported attributes */
  unsigned int GetNumberOfAttributes()
  {
    return m_Attributes.size();
  }

private:
  AttributesListType m_Attributes;
};

} // end namespace Functor

/** \class AttributesMapLabelObject
 *  \brief A LabelObject with a generic attributes map
 *
 *  This class derives from itk::LabelObject and extends it to
 *  store pairs of key, value (of type TAttributesValue) in an internal
 *  map container.
 *
 * As such it allows storing any custom attributes as necessary.
 *
 * \sa LabelObject, ShapeLabelObject, StatisticsLabelObject
 *
 * \ingroup DataRepresentation
 *
 * \ingroup OTBLabelMap
 */
template <class TLabel, unsigned int VImageDimension, class TAttributesValue>
class ITK_EXPORT AttributesMapLabelObject : public itk::LabelObject<TLabel, VImageDimension>
{
public:
  /** Standard class typedefs */
  typedef AttributesMapLabelObject Self;
  typedef itk::LabelObject<TLabel, VImageDimension> Superclass;
  typedef typename Superclass::LabelObjectType LabelObjectType;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;
  typedef itk::WeakPointer<const Self>         ConstWeakPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(AttributesMapLabelObject, LabelObject);

  itkStaticConstMacro(ImageDimension, unsigned int, VImageDimension);

  /// Type of a label map using an AttributesMapLabelObject
  typedef itk::LabelMap<Self> LabelMapType;

  /// Template parameters typedef
  typedef TLabel           LabelType;
  typedef TAttributesValue AttributesValueType;

  // Convenient inherited typedefs
  typedef typename Superclass::IndexType  IndexType;
  typedef typename Superclass::LineType   LineType;
  typedef typename Superclass::LengthType LengthType;

  /// Map container typedefs
  typedef std::map<std::string, AttributesValueType> AttributesMapType;
  typedef typename AttributesMapType::iterator       AttributesMapIteratorType;
  typedef typename AttributesMapType::const_iterator AttributesMapConstIteratorType;

  // The polygon corresponding to the label object
  typedef Polygon<double>               PolygonType;
  typedef typename PolygonType::Pointer PolygonPointerType;

  /**
   * Set an attribute value.
   * If the key name already exists in the map, the value is overwritten.
   */
  void SetAttribute(const char* name, AttributesValueType value)
  {
    m_Attributes[name] = value;
  }

  /**
   * Set an attribute value.
   * If the key name already exists in the map, the value is overwritten.
   */
  void SetAttribute(const std::string& name, AttributesValueType value)
  {
    this->SetAttribute(name.c_str(), value);
  }

  /**
   * Returns the attribute corresponding to name
   */
  AttributesValueType GetAttribute(const char* name) const
  {
    AttributesMapConstIteratorType it = m_Attributes.find(name);
    if (it != m_Attributes.end())
    {
      return it->second;
    }
    else
    {
      itkExceptionMacro(<< "Could not find attribute named " << name);
    }
  }

  /**
   * Returns the total number of attributes
   */
  unsigned int GetNumberOfAttributes() const
  {
    return m_Attributes.size();
  }

  /**
   * Returns the list of available attributes
   */
  std::vector<std::string> GetAvailableAttributes() const
  {
    std::vector<std::string> attributesNames;

    AttributesMapConstIteratorType it = m_Attributes.begin();

    while (it != m_Attributes.end())
    {
      attributesNames.push_back(it->first);
      ++it;
    }
    return attributesNames;
  }

  /**
  * This method is overloaded to add the copy of the attributes map.
  */
  virtual void CopyAttributesFrom(const LabelObjectType* lo)
  {
    Superclass::CopyAttributesFrom(lo);

    // copy the data of the current type if possible
    const Self* src = dynamic_cast<const Self*>(lo);
    if (src == nullptr)
    {
      return;
    }
    m_Attributes = src->m_Attributes;
  }

  /** Return the polygon (const version) */
  const PolygonType* GetPolygon() const
  {
    return m_Polygon;
  }

  /** Return the polygon (non const version) */
  PolygonType* GetPolygon()
  {
    return m_Polygon;
  }

  /** Set the polygon */
  void SetPolygon(PolygonType* p)
  {
    m_Polygon = p;
  }

protected:
  /** Constructor */
  AttributesMapLabelObject() : m_Attributes(), m_Polygon(PolygonType::New())
  {
  }
  /** Destructor */
  ~AttributesMapLabelObject() override
  {
  }

  /** The printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override
  {
    Superclass::PrintSelf(os, indent);
    os << indent << "Attributes: " << std::endl;
    for (AttributesMapConstIteratorType it = m_Attributes.begin(); it != m_Attributes.end(); ++it)
    {
      os << indent << indent << it->first << " = " << it->second << std::endl;
    }
  }

private:
  AttributesMapLabelObject(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** The attributes map */
  AttributesMapType m_Attributes;

  /** The polygon corresponding to the label object. Caution, this
   *  will be empty by default */
  PolygonPointerType m_Polygon;
};

} // end namespace otb
#endif
