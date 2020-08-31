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

#include "mvdStackedLayerModel.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)
#include <algorithm>
#include <sstream>

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdAbstractLayerModel.h"
#include "mvdAlgorithm.h"
#include "mvdVectorImageModel.h"
#include "mvdCore.h"

namespace mvd
{
/*
  TRANSLATOR mvd::StackedLayerModel

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

namespace
{
} // end of anonymous namespace.


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */

StackedLayerModel::SizeType StackedLayerModel::m_LayerCount = 0;

const StackedLayerModel::KeyType StackedLayerModel::NIL_KEY;

const StackedLayerModel::SizeType StackedLayerModel::NIL_INDEX = -1;

/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
StackedLayerModel::StackedLayerModel(QObject* p)
  : AbstractModel(p), m_LayerModels(), m_Keys(), m_Current(StackedLayerModel::NIL_INDEX), m_Reference(StackedLayerModel::NIL_INDEX)
{
}

/*******************************************************************************/
StackedLayerModel::~StackedLayerModel()
{
}

/*****************************************************************************/
StackedLayerModel::KeyType StackedLayerModel::Add(AbstractLayerModel* model)
{
  return Insert(model, StackedLayerModel::NIL_INDEX);
}

/*****************************************************************************/
void StackedLayerModel::Clear()
{
  bool emitSignal0 = !IsEmpty();
  bool emitSignal1 = !IsEmpty() && m_Current < GetCount();
  bool emitSignal2 = !IsEmpty() && m_Reference < GetCount();


  if (emitSignal0)
    Q_EMIT ContentAboutToBeReset();

  //
  // Clear current.
  if (emitSignal1)
  {
    Q_EMIT CurrentAboutToBeChanged(StackedLayerModel::NIL_INDEX);
    Q_EMIT AboutToChangeSelectedLayerModel(KeyType());
  }

  m_Current = StackedLayerModel::NIL_INDEX;

  if (emitSignal1)
  {
    Q_EMIT CurrentChanged(m_Current);
    Q_EMIT SelectedLayerModelChanged(KeyType());
  }

  //
  // Clear reference.
  if (emitSignal2)
    Q_EMIT ReferenceAboutToBeChanged(StackedLayerModel::NIL_INDEX);

  m_Reference = StackedLayerModel::NIL_INDEX;

  if (emitSignal2)
    Q_EMIT ReferenceChanged(m_Reference);

  //
  // Clear content.
  ClearPixelInfos();

  for (LayerModelMap::iterator it(m_LayerModels.begin()); it != m_LayerModels.end(); ++it)
  {
    assert(it->second != NULL);

    if (it->second->parent() == this)
    {
      delete it->second;
      it->second = NULL;
    }
  }

  m_LayerModels.clear();
  m_Keys.clear();

  if (emitSignal0)
    Q_EMIT ContentReset();
}

/*****************************************************************************/
bool StackedLayerModel::Contains(const AbstractLayerModel* layerModel) const
{
  for (LayerModelMap::const_iterator it(m_LayerModels.begin()); it != m_LayerModels.end(); ++it)
    if (it->second == layerModel)
      return true;

  return false;
}

/*****************************************************************************/
void StackedLayerModel::CountSRT(SizeType& unk, SizeType& crt, SizeType& geo, SizeType& ssr) const
{
  for (LayerModelMap::const_iterator it(m_LayerModels.begin()); it != m_LayerModels.end(); ++it)
  {
    assert(it->second != NULL);

    switch (it->second->GetSpatialReferenceType())
    {
    case SRT_UNKNOWN:
      ++unk;
      break;

    case SRT_CARTO:
      ++crt;
      break;

    case SRT_GEO:
      ++geo;
      break;

    case SRT_SENSOR:
      ++ssr;
      break;
    }
  }
}

/*****************************************************************************/
void StackedLayerModel::Deleting(unsigned int index)
{
  // qDebug() << this << "::Delete(" << index << ")";


  //
  // Check content.
  if (IsEmpty())
    return;

  if (index >= GetCount())
    return;

  assert(!m_Keys[index].empty());

  //
  // Find item.
  LayerModelMap::iterator it(m_LayerModels.find(m_Keys[index]));

  assert(it != m_LayerModels.end());

  //
  // Check if signals have to be emitted.
  bool emitCurrentChanged   = m_Current < GetCount() && index <= m_Current;
  bool emitReferenceChanged = m_Reference < GetCount() && index <= m_Reference;

  //
  // Remember new current index.
  SizeType current = index > m_Current ? m_Current : (m_Current > 0 ? m_Current - 1 : GetCount() > 1 ? 0 : StackedLayerModel::NIL_INDEX);

  //
  // Emit signals.
  Q_EMIT ContentAboutToBeChanged();
  Q_EMIT LayerAboutToBeDeleted(index);

  //
  // Clear satellite date.
  ClearPixelInfos();

  //
  // Remove layer-model.
  AbstractLayerModel* layer = it->second;

  // Disconnect the renaming signals
  QObject::disconnect(layer, SIGNAL(NameChanged()),
                      // from:
                      this, SIGNAL(LayerRenamed()));

  m_LayerModels.erase(it);

  m_Keys.erase(m_Keys.begin() + index);

  it = m_LayerModels.end();

  //
  // Update pointer to current.
  if (emitCurrentChanged)
    SetCurrent(current, true);

  //
  // Update reference pointer.
  if (emitReferenceChanged)
    SetReference(index > m_Reference ? m_Reference : (m_Reference > 0 ? m_Reference - 1 : GetCount() > 0 ? 0 : StackedLayerModel::NIL_INDEX), true);

  //
  // Eventually delete layer.
  if (layer->parent() == this)
  {
    delete layer;
    layer = NULL;
  }
}

void StackedLayerModel::Delete(SizeType index)
{
  // the work of deleting model is now done in the above function
  // Deleting(unsigned int index)
  // Emit signals.
  Q_EMIT LayerDeleted(index);
  Q_EMIT ContentChanged();
}

/*****************************************************************************/
void StackedLayerModel::EndEditResolutions()
{
  // qDebug() << this << "::EndEditResolutions()";

  Q_EMIT ResolutionsChanged(m_PixelInfos);
}

/*******************************************************************************/
std::string StackedLayerModel::GenerateKey(AbstractLayerModel* otbUseInDebug(layerModel))
{
  std::ostringstream oss;

  oss << "Layer #" << m_LayerCount++;

#ifdef OTB_DEBUG
  oss << " (" << layerModel->metaObject()->className() << "@" << std::hex << layerModel << ")";
#endif

  return oss.str();
}

/*****************************************************************************/
StackedLayerModel::KeyType StackedLayerModel::Insert(AbstractLayerModel* model, SizeType index)
{
  // qDebug() << this << "::Insert(" << model << "," << index << ")";

  //
  // Check given model.
  assert(model != NULL);

  if (model == NULL)
  {
    throw std::runtime_error(ToStdString(tr("Cannot insert NULL AbstractLayerModel.")));
  }

  //
  // Generate key for new layer.
  std::string key(StackedLayerModel::GenerateKey(model));
  assert(!key.empty());

  if (key.empty())
  {
    throw std::runtime_error(ToStdString(tr("Failed to generate string key for '%1'.").arg(model->metaObject()->className())));
  }

  //
  // Clamp index. If out of bounds, insert model at the end of stack.
  if (index > GetCount())
    index = GetCount();

  //
  // Check if signals have to be emitted.
  bool emitCurrentChanged   = m_Current < GetCount() && index <= m_Current;
  bool emitReferenceChanged = m_Reference < GetCount() && index <= m_Reference;

  //
  // Emit signals.
  Q_EMIT ContentAboutToBeChanged();

  //
  // Clear satellite date.
  ClearPixelInfos();

  //
  // Insert model.
  m_LayerModels.insert(LayerModelMap::value_type(key, model));
  m_Keys.insert(m_Keys.begin() + index, key);

  // Connect the renaming signals
  QObject::connect(model, SIGNAL(NameChanged()),
                   // to:
                   this, SIGNAL(LayerRenamed()));

  //
  // Update pointer to current.
  if (emitCurrentChanged)
    SetCurrent(m_Current + 1, true);

  //
  // Update reference pointer.
  if (emitReferenceChanged)
    SetReference(m_Reference + 1, true);

  //
  // Emit signals.
  Q_EMIT LayerAdded(index);
  Q_EMIT ContentChanged();

  //
  // Return generated key.
  return key;
}

/*****************************************************************************/
void StackedLayerModel::LowerLayer(SizeType index)
{
  assert(GetCount() > 1);
  assert(index < GetCount());

  SizeType next = Next(index);

  Q_EMIT OrderAboutToBeChanged();
  {
    ClearPixelInfos();

    std::swap(*(m_Keys.begin() + index), *(m_Keys.begin() + next));
  }
  Q_EMIT OrderChanged();

  // WARNING: This may be buggy if index!=m_Current
  Q_EMIT CurrentAboutToBeChanged(next);
  {
    m_Current = next;
  }
  Q_EMIT CurrentChanged(next);

  // WARNING: This may be buggy if index!=m_Reference
  SetReference(m_Reference == next ? index : (m_Reference == index ? next : m_Reference));
}

/*****************************************************************************/
void StackedLayerModel::MoveTo(SizeType index, SizeType position)
{
  assert(GetCount() > 1);
  assert(index < GetCount());
  assert(position < GetCount());

  if (index == position)
    return;

  //
  // Move element.
  Q_EMIT OrderAboutToBeChanged();
  {
    ClearPixelInfos();

    KeyType key(m_Keys[index]);

    m_Keys.erase(m_Keys.begin() + index);
    m_Keys.insert(m_Keys.begin() + position, key);
  }
  Q_EMIT OrderChanged();

  //
  // Compute new current element.
  SizeType current(m_Current);

  if (index == m_Current)
    current = position;

  if (index > m_Current && position <= m_Current)
    ++current;

  else if (index < m_Current && position >= m_Current)
    --current;

  //
  // Signal new current element.
  SetCurrent(current);

  //
  // Compute new reference element.
  SizeType reference(m_Reference);

  if (index == m_Reference)
    reference = position;

  if (index > m_Reference && position <= m_Reference)
    ++reference;

  else if (index < m_Reference && position >= m_Reference)
    --reference;

  SetReference(reference);
}

/*****************************************************************************/
void StackedLayerModel::MoveToBottom(SizeType index)
{
  MoveTo(index, m_Keys.size() - 1);
}

/*****************************************************************************/
void StackedLayerModel::MoveToTop(SizeType index)
{
  MoveTo(index, 0);
}

/*****************************************************************************/
void StackedLayerModel::RaiseLayer(SizeType index)
{
  assert(GetCount() > 1);
  assert(index < GetCount());

  SizeType prev = Prev(index);

  Q_EMIT OrderAboutToBeChanged();
  {
    ClearPixelInfos();

    std::swap(*(m_Keys.begin() + index), *(m_Keys.begin() + prev));
  }
  Q_EMIT OrderChanged();

  // WARNING: This may be buggy if index!=m_Current.
  Q_EMIT CurrentAboutToBeChanged(prev);
  {
    m_Current = prev;
  }
  Q_EMIT CurrentChanged(prev);

  // WARNING: This may be buggy if index!=m_Reference.
  SetReference(m_Reference == prev ? index : (m_Reference == index ? prev : m_Reference));
}

/*****************************************************************************/
void StackedLayerModel::RotateLayerUp(SizeType index)
{
  if (GetCount() < 2)
    return;

  if (index >= GetCount())
    return;

  KeyType currentKey(GetKey(m_Current));
  KeyType referenceKey(GetKey(m_Reference));

  Q_EMIT OrderAboutToBeChanged();
  {
    ClearPixelInfos();

    std::rotate(m_Keys.begin(), m_Keys.begin() + index, m_Keys.end());
  }
  Q_EMIT OrderChanged();

  if (!currentKey.empty())
  {
    SizeType current = FindKey(currentKey);

    assert(current != StackedLayerModel::NIL_INDEX);

    Q_EMIT CurrentAboutToBeChanged(current);
    {
      m_Current = current;
    }
    Q_EMIT CurrentChanged(m_Current);
  }

  if (!referenceKey.empty())
  {
    SizeType reference = FindKey(referenceKey);

    assert(reference != StackedLayerModel::NIL_INDEX);

    Q_EMIT ReferenceAboutToBeChanged(reference);
    {
      m_Reference = reference;
    }
    Q_EMIT ReferenceChanged(m_Reference);
  }

  // qDebug() << "current:" << index;
}

/*****************************************************************************/
void StackedLayerModel::RotateLayerDown(SizeType index)
{
  if (GetCount() < 2)
    return;

  if (index >= GetCount())
    return;

  KeyType currentKey(GetKey(m_Current));
  KeyType referenceKey(GetKey(m_Reference));

  Q_EMIT OrderAboutToBeChanged();
  {
    ClearPixelInfos();

    std::rotate(m_Keys.rbegin(), m_Keys.rbegin() + index, m_Keys.rend());
  }
  Q_EMIT OrderChanged();

  if (!currentKey.empty())
  {
    SizeType current = FindKey(currentKey);

    assert(current != StackedLayerModel::NIL_INDEX);

    Q_EMIT CurrentAboutToBeChanged(current);
    {
      m_Current = current;
    }
    Q_EMIT CurrentChanged(current);
  }

  if (!referenceKey.empty())
  {
    SizeType reference = FindKey(referenceKey);

    assert(reference != StackedLayerModel::NIL_INDEX);

    Q_EMIT ReferenceAboutToBeChanged(reference);
    {
      m_Reference = reference;
    }
    Q_EMIT ReferenceChanged(reference);
  }

  // qDebug() << "current:" << index;
}

/*****************************************************************************/
void StackedLayerModel::SetCurrent(const AbstractLayerModel* layerModel)
{
  SetCurrent(KeyOf(layerModel));
}

/*****************************************************************************/
void StackedLayerModel::SetCurrent(const KeyType& key)
{
  if (key == GetCurrentKey())
    return;

  if (key == StackedLayerModel::NIL_KEY)
  {
    SetCurrent(StackedLayerModel::NIL_INDEX);

    return;
  }

  for (SizeType i = 0; i < m_Keys.size(); ++i)
    if (m_Keys[i] == key)
    {
      SetCurrent(i);
      return;
    }
}

/*****************************************************************************/
void StackedLayerModel::SetReference(const KeyType& key)
{
  if (key == GetKey(m_Reference))
    return;

  if (key == StackedLayerModel::NIL_KEY)
  {
    SetReference(StackedLayerModel::NIL_INDEX);

    return;
  }

  for (SizeType i = 0; i < m_Keys.size(); ++i)
    if (m_Keys[i] == key)
    {
      SetReference(i);
      return;
    }
}

/*****************************************************************************/
void StackedLayerModel::SetReference(const AbstractLayerModel* layerModel)
{
  SetReference(KeyOf(layerModel));
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
