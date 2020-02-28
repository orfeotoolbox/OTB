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

#ifndef otbImageMetadataNG_h
#define otbImageMetadataNG_h

#include "otbBandMetadata.h"
//#include "otbGeometryMetadata.h"
#include "otbMetaDataKey.h"
#include "OTBMetadataExport.h"

#include <boost/any.hpp>
#include <vector>
#include <string>
#include <ctime>
#include <bitset>
//~ #include <stdexcept>

namespace otb
{

// TODO code factorization with a template MDSimpleDict
template <class TKey, class T>
class EnumDict
{
public:
  const T & operator[](const TKey& key) const
    {
    assert(key != TKey::END);
    int pos = static_cast<int>(key);
    if (Flag[pos] == false)
      {
      itkGenericExceptionMacro(<< "Missing value for key: "<< pos);
      }
    return Values[pos];
    }
  
  void Set(const TKey& key, const double &value)
    {
    assert(key != TKey::END);
    int pos = static_cast<int>(key);
    Values[pos] = value;
    Flag[pos] = true;
    }
  
  void Remove(const TKey& key)
    {
    assert(key != TKey::END);
    int pos = static_cast<int>(key);
    Flag[pos] = false;
    T defVal;
    Values[pos] = defVal;
    }
  
  bool Has(const TKey& key) const
    {
    assert(key != TKey::END);
    int pos = static_cast<int>(key);
    return Flag[pos];
    }

  struct const_iterator
   {
      typedef char const                         value_type;
      typedef char const&                        reference;
      typedef char const*                        pointer;
      typedef std::size_t                        size_type;
      typedef std::ptrdiff_t                     difference_type;
      typedef std::forward_iterator_tag    iterator_category;

      explicit char_iterator(const_pointer p) : m_ptr(p) {}
      char_iterator & operator++() { ++m_ptr; return *this; }
      char_iterator   operator++(int) { char_iterator tmp(*this); ++m_ptr; return tmp; }
      char_iterator & operator--() { --m_ptr; return *this; }
      char_iterator   operator--(int) { char_iterator tmp(*this); --m_ptr; return tmp; }
      const_reference operator*() const { return *m_ptr; }

      char_iterator & operator+=(difference_type off) { m_ptr += off; return *this; }
      char_iterator & operator-=(difference_type off) { m_ptr -= off; return *this; }

      friend char_iterator operator+(char_iterator lhs, difference_type off) { return lhs += off; }
      friend char_iterator operator+(difference_type off, char_iterator rhs) { return rhs += off; }
      friend char_iterator operator-(char_iterator lhs, difference_type off) { return lhs -= off; }
      friend char_iterator operator-(difference_type off, char_iterator rhs) { return rhs -= off; }

      friend difference_type operator-(char_iterator lhs, char_iterator rhs) { return lhs.m_ptr - rhs.m_ptr; }

      friend bool operator==(char_iterator lhs, char_iterator rhs) {
         return lhs.m_ptr == rhs.m_ptr;
      }
      friend bool operator!=(char_iterator lhs, char_iterator rhs) {
         return lhs.m_ptr != rhs.m_ptr;
      }
      friend bool operator<(char_iterator lhs, char_iterator rhs) {
         return lhs.m_ptr < rhs.m_ptr;
      }
   private:
      friend bool operator<=(char_iterator lhs, char_iterator rhs) {
         return lhs.m_ptr <= rhs.m_ptr;
      }
   private:
      const_pointer m_ptr;
   };

  iterator {
      iterator(const iterator&);
      ~iterator();
      iterator& operator=(const iterator&);
      iterator& operator++(); //prefix increment
      reference operator*() const;
      friend void swap(iterator& lhs, iterator& rhs); //C++11 I think
  };
  
  input_iterator : public virtual iterator {
      iterator operator++(int); //postfix increment
      value_type operator*() const;
      pointer operator->() const;
      friend bool operator==(const iterator&, const iterator&);
      friend bool operator!=(const iterator&, const iterator&); 
  };
  //once an input iterator has been dereferenced, it is 
  //undefined to dereference one before that.
  
  output_iterator : public virtual iterator {
      reference operator*() const;
      iterator operator++(int); //postfix increment
  };
  //dereferences may only be on the left side of an assignment
  //once an output iterator has been dereferenced, it is 
  //undefined to dereference one before that.
  
  forward_iterator : input_iterator, output_iterator {
      forward_iterator();
  };

private:
  std::bitset<static_cast<int>(TKey::END)> Flag;
  std::array<T, static_cast<int>(TKey::END)> Values;
};


/** \class ImageMetadataBase
 *
 *  \brief Metadata hybrid dictionary
 *
 *  Contains a stack of geometry parameters, several typed dictionaries for
 *  double, string, and other types. The different metadatas are retrieved
 *  with enum classes ( one for each type stored in the dictionary).
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT ImageMetadataBase
{
public:
  /** Stack of geometry parameters, sorted by decreasing priority
   *  Cases:
   *    - projected
   *        * WKT
   *        * EPSG
   *        * proj4
   *    - sensor
   *        * RPC
   *        * SAR
   *        * optional adjustment
   *        * optional GCP
   *        * other sensor model (need a factory to deal with it)
   *    - epipolar ?
   *        + epipolar rectification grid
   *
   * Corner cases we may encounter:
   *   - sensor refinement: [RPC, GCP, adjustment]
   *   - piece-wise sensor model: [RPC_R1C1, RPC_R1C2, ...]
   *   - raw georeferencing: [GCP]
   *   - Fine registration : [WKT, adjustment grid]
   */
  std::vector< std::pair<MDGeom, boost::any> > GeometryKeys;

  std::pair<
    std::bitset<static_cast<int>(MDNum::END)>,
    std::array<double, static_cast<int>(MDNum::END)> > NumericKeys;

  std::pair<
    std::bitset<static_cast<int>(MDStr::END)>,
    std::array<std::string, static_cast<int>(MDStr::END)> > StringKeys;

  std::pair<
    std::bitset<static_cast<int>(MDL1D::END)>,
    std::array<MetaData::LUT1D, static_cast<int>(MDL1D::END)> > LUT1DKeys;

  std::pair<
    std::bitset<static_cast<int>(MDL2D::END)>,
    std::array<MetaData::LUT2D, static_cast<int>(MDL2D::END)> > LUT2DKeys;

  std::pair<
    std::bitset<static_cast<int>(MDTime::END)>,
    std::array<MetaData::Time, static_cast<int>(MDTime::END)> > TimeKeys;

  std::map<std::string, std::string> ExtraKeys;

  // TODO : iterators ?

  // -------------------- Double utility function ----------------------------

  /** Read-only accessor to numeric keys */
  const double & operator[](const MDNum& key) const;
  
  /** Setter for numeric keys */
  void Set(const MDNum& key, const double &value);
  
  /** Remove a key from the dictionary (even if the key is already missing) */
  void Remove(const MDNum& key);
  
  /** Test if a key is available */
  bool Has(const MDNum& key) const;
  
  // -------------------- String utility function ----------------------------

  /** Read-only accessor to numeric keys */
  const std::string & operator[](const MDStr& key) const;
  
  /** Setter for numeric keys */
  void Set(const MDStr& key, const std::string &value);
  
  /** Remove a key from the dictionary (even if the key is already missing) */
  void Remove(const MDStr& key);
  
  /** Test if a key is available */
  bool Has(const MDStr& key) const;
  
  // -------------------- LUT1D utility function ----------------------------

  /** Read-only accessor to numeric keys */
  const MetaData::LUT1D & operator[](const MDL1D& key) const;
  
  /** Setter for numeric keys */
  void Set(const MDL1D& key, const MetaData::LUT1D &value);
  
  /** Remove a key from the dictionary (even if the key is already missing) */
  void Remove(const MDL1D& key);
  
  /** Test if a key is available */
  bool Has(const MDL1D& key) const;
  
  // -------------------- 2D LUT utility function ----------------------------

  /** Read-only accessor to numeric keys */
  const MetaData::LUT2D & operator[](const MDL2D& key) const;
  
  /** Setter for numeric keys */
  void Set(const MDL2D& key, const MetaData::LUT2D &value);
  
  /** Remove a key from the dictionary (even if the key is already missing) */
  void Remove(const MDL2D& key);
  
  /** Test if a key is available */
  bool Has(const MDL2D& key) const;
  
  // -------------------- Time utility function ----------------------------

  /** Read-only accessor to numeric keys */
  const MetaData::Time & operator[](const MDTime& key) const;
  
  /** Setter for numeric keys */
  void Set(const MDTime& key, const MetaData::Time &value);

  /** Remove a key from the dictionary (even if the key is already missing) */
  void Remove(const MDTime& key);

  /** Test if a key is available */
  bool Has(const MDTime& key) const;

  // -------------------- Extra keys utility function --------------------------

  /** Read-only accessor to numeric keys */
  const std::string & operator[](const std::string & key) const;

  /** Setter for numeric keys */
  void Set(const std::string& key, const std::string &value);

  /** Remove a key from the dictionary (even if the key is already missing) */
  void Remove(const std::string& key);

  /** Test if a key is available */
  bool Has(const std::string& key) const;

};

/** \class ImageMetadata
 *
 * \brief Generic class containing image metadatas used in OTB (first try)
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT ImageMetadataNG: public ImageMetadataBase
{
public:
  /** Band-specific metadatas */
  std::vector<ImageMetadataBase> Bands;

  // utility functions
  /** Extract metadata from a subset of the bands */
  ImageMetadataNG slice(int start, int end);

  /** concatenate with an other ImageMetadata */
  void append(const ImageMetadataNG& );

  /** if all bands share the same value of a key, put it at top level */
  void compact();
};

} // end namespace otb

#endif
