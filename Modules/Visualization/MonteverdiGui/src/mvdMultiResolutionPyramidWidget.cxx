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

#include "mvdMultiResolutionPyramidWidget.h"
#include "ui_mvdMultiResolutionPyramidWidget.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)
#include <cassert>
#include <cmath>

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdCore.h"
#include "mvdGui.h"
#include "mvdI18nCoreApplication.h"

namespace mvd
{

/*
  TRANSLATOR mvd::MultiResolutionPyramidWidget

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

char const* const GDAL_RESAMPLING_NAMES[otb::GDAL_RESAMPLING_COUNT] = {
    QT_TRANSLATE_NOOP("mvd::MultiResolutionPyramidWidget", "None"),
    QT_TRANSLATE_NOOP("mvd::MultiResolutionPyramidWidget", "Nearest"),
    QT_TRANSLATE_NOOP("mvd::MultiResolutionPyramidWidget", "Gauss"),
    QT_TRANSLATE_NOOP("mvd::MultiResolutionPyramidWidget", "Cubic"),
    QT_TRANSLATE_NOOP("mvd::MultiResolutionPyramidWidget", "Average"),
    QT_TRANSLATE_NOOP("mvd::MultiResolutionPyramidWidget", "Mode"),
    QT_TRANSLATE_NOOP("mvd::MultiResolutionPyramidWidget", "Average Mag/Phase"),
};


char const* const GDAL_COMPRESSION_NAMES[otb::GDAL_COMPRESSION_COUNT] = {
    QT_TRANSLATE_NOOP("mvd::MultiResolutionPyramidWidget", "None"),    QT_TRANSLATE_NOOP("mvd::MultiResolutionPyramidWidget", "JPEG"),
    QT_TRANSLATE_NOOP("mvd::MultiResolutionPyramidWidget", "LZW"),     QT_TRANSLATE_NOOP("mvd::MultiResolutionPyramidWidget", "PACKBITS"),
    QT_TRANSLATE_NOOP("mvd::MultiResolutionPyramidWidget", "DEFLATE"),
};


char const* const GDAL_FORMAT_NAMES[otb::GDAL_FORMAT_COUNT] = {
    QT_TRANSLATE_NOOP("mvd::MultiResolutionPyramidWidget", "ERDAS"), QT_TRANSLATE_NOOP("mvd::MultiResolutionPyramidWidget", "GeoTIFF"),
};

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */

/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */
/*****************************************************************************/
MultiResolutionPyramidWidget::MultiResolutionPyramidWidget(QWidget* p, Qt::WindowFlags flags)
  : QWidget(p, flags), m_UI(new mvd::Ui::MultiResolutionPyramidWidget())
{
  m_UI->setupUi(this);

  {
    QItemSelectionModel* ism = m_UI->resolutionsListView->selectionModel();

    m_UI->resolutionsListView->setModel(new QStandardItemModel(m_UI->resolutionsListView));

    delete ism;
    ism = NULL;
  }

  AddItemsInto(m_UI->formatComboBox, "mvd::MultiResolutionPyramidWidget", GDAL_FORMAT_NAMES, otb::GDAL_FORMAT_COUNT);

  AddItemsInto(m_UI->algorithmComboBox, "mvd::MultiResolutionPyramidWidget", GDAL_RESAMPLING_NAMES, otb::GDAL_RESAMPLING_COUNT);

  AddItemsInto(m_UI->compressionComboBox, "mvd::MultiResolutionPyramidWidget", GDAL_COMPRESSION_NAMES, otb::GDAL_COMPRESSION_COUNT);

  m_UI->baseSpinBox->setValue(2);

  m_UI->levelsSpinBox->setValue(1);

  m_UI->sizeSpinBox->setValue(2);
}

/*****************************************************************************/
MultiResolutionPyramidWidget::~MultiResolutionPyramidWidget()
{
  delete m_UI;
  m_UI = NULL;
}

/*****************************************************************************/
void MultiResolutionPyramidWidget::ClearResolutions()
{
  QStandardItemModel* itemModel = qobject_cast<QStandardItemModel*>(m_UI->resolutionsListView->model());

  assert(itemModel != NULL);

  itemModel->clear();
}

/*****************************************************************************/
void MultiResolutionPyramidWidget::SetBuilder(const otb::GDALOverviewsBuilder::Pointer& p)
{
  m_UI->formatComboBox->setCurrentIndex(otb::GDAL_FORMAT_GEOTIFF);
  m_UI->algorithmComboBox->setCurrentIndex(otb::GDAL_RESAMPLING_AVERAGE);
  m_UI->compressionComboBox->setCurrentIndex(otb::GDAL_COMPRESSION_NONE);


  {
    bool prevSignalsBlocked = m_UI->baseSpinBox->blockSignals(true);

    m_UI->baseSpinBox->setValue(2);

    m_UI->baseSpinBox->blockSignals(prevSignalsBlocked);
  }

  {
    bool prevSignalsBlocked = m_UI->levelsSpinBox->blockSignals(true);

    m_UI->levelsSpinBox->setValue(1);

    m_UI->levelsSpinBox->blockSignals(prevSignalsBlocked);
  }

  {
    bool prevSignalsBlocked = m_UI->sizeSpinBox->blockSignals(true);

    m_UI->sizeSpinBox->setRange(1, std::numeric_limits<int>::max());
    m_UI->sizeSpinBox->setValue(1);

    m_UI->sizeSpinBox->blockSignals(prevSignalsBlocked);
  }


  ClearResolutions();


  m_GDALOverviewsBuilder = p;

  if (p.IsNull())
    return;

  assert(!m_GDALOverviewsBuilder.IsNull());


  m_UI->formatComboBox->setCurrentIndex(m_GDALOverviewsBuilder->GetFormat());

  m_UI->algorithmComboBox->setCurrentIndex(m_GDALOverviewsBuilder->GetResamplingMethod());

  m_UI->compressionComboBox->setCurrentIndex(m_GDALOverviewsBuilder->GetCompressionMethod());


  unsigned int minSize = std::min(m_GDALOverviewsBuilder->GetWidth(), m_GDALOverviewsBuilder->GetHeight());


  {
    bool prevSignalsBlocked = m_UI->baseSpinBox->blockSignals(true);

    m_UI->baseSpinBox->setRange(2, minSize);
    m_UI->baseSpinBox->setValue(m_GDALOverviewsBuilder->GetResolutionFactor());

    m_UI->baseSpinBox->blockSignals(prevSignalsBlocked);
  }

  unsigned int maxResolutions = m_GDALOverviewsBuilder->CountResolutions();

  {
    bool prevSignalsBlocked = m_UI->levelsSpinBox->blockSignals(true);

    m_UI->levelsSpinBox->setRange(0, maxResolutions);
    m_UI->levelsSpinBox->setValue(m_GDALOverviewsBuilder->GetNbResolutions());

    m_UI->levelsSpinBox->blockSignals(prevSignalsBlocked);
  }

  {
    bool prevSignalsBlocked = m_UI->sizeSpinBox->blockSignals(true);

    m_UI->sizeSpinBox->setRange(1, minSize);

    assert(maxResolutions >= m_GDALOverviewsBuilder->GetNbResolutions());

    // Force C++ implicit cast by declaring local variables to force compiler
    // to find correct std::pow() signature.
    //
    // It fixes compile-time:
    // error: call of pow(unsigned int, unsigned int) is ambiguous
    double rf = m_GDALOverviewsBuilder->GetResolutionFactor();

    int m = maxResolutions - m_GDALOverviewsBuilder->GetNbResolutions();

    m_UI->sizeSpinBox->setValue(static_cast<int>(std::pow(rf, m)));

    m_UI->sizeSpinBox->blockSignals(prevSignalsBlocked);
  }


  UpdateResolutions();
}

/*****************************************************************************/
void MultiResolutionPyramidWidget::UpdateResolutions()
{
  assert(!m_GDALOverviewsBuilder.IsNull());

  QStandardItemModel* itemModel = qobject_cast<QStandardItemModel*>(m_UI->resolutionsListView->model());

  assert(itemModel != NULL);

  otb::GDALOverviewsBuilder::SizeVector resolutions;

  m_GDALOverviewsBuilder->ListResolutions(resolutions);

  for (otb::GDALOverviewsBuilder::SizeVector::const_iterator it(resolutions.begin()); it != resolutions.end(); ++it)
    itemModel->appendRow(new QStandardItem(QString("%1x%2").arg((*it)[0]).arg((*it)[1])));
}

/*****************************************************************************/
void MultiResolutionPyramidWidget::UpdateSize()
{
  assert(!m_GDALOverviewsBuilder.IsNull());

  bool prevSignalsBlocked = m_UI->sizeSpinBox->blockSignals(true);

  unsigned int count = m_GDALOverviewsBuilder->CountResolutions();

  // Force C++ implicit cast by declaring local variables to force compiler
  // to find correct std::pow() signature.
  //
  // It fixes compile-time:
  // error: call of pow(unsigned int, unsigned int) is ambiguous
  double rf = m_GDALOverviewsBuilder->GetResolutionFactor();

  int m = count - std::min(m_GDALOverviewsBuilder->GetNbResolutions(), count);

  m_UI->sizeSpinBox->setValue(static_cast<int>(std::pow(rf, m)));

  m_UI->sizeSpinBox->blockSignals(prevSignalsBlocked);
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/
void MultiResolutionPyramidWidget::on_baseSpinBox_valueChanged(int value)
{
  // qDebug() << this << "::on_baseSpinBox_valueChanged(" << value << ")";

  ClearResolutions();

  if (m_GDALOverviewsBuilder.IsNull())
    return;

  m_GDALOverviewsBuilder->SetResolutionFactor(value);

  m_UI->levelsSpinBox->setRange(0, m_GDALOverviewsBuilder->CountResolutions());

  UpdateResolutions();

  UpdateSize();

  Q_EMIT BaseValueChanged(value);
}

/*****************************************************************************/
void MultiResolutionPyramidWidget::on_levelsSpinBox_valueChanged(int value)
{
  // qDebug() << this << "::on_levelsSpinBox_valueChanged(" << value << ")";

  ClearResolutions();

  if (m_GDALOverviewsBuilder.IsNull())
    return;

  m_GDALOverviewsBuilder->SetNbResolutions(value);

  UpdateResolutions();

  UpdateSize();

  Q_EMIT LevelsValueChanged(value);
}

/*****************************************************************************/
void MultiResolutionPyramidWidget::on_sizeSpinBox_valueChanged(int value)
{
  // qDebug() << this << "::on_sizeSpinBox_valueChanged(" << value << ")";

  if (m_GDALOverviewsBuilder.IsNull())
    return;

  unsigned int count = m_GDALOverviewsBuilder->CountResolutions(m_GDALOverviewsBuilder->GetResolutionFactor(), value);

  if (static_cast<unsigned int>(m_UI->levelsSpinBox->value()) <= count)
    return;

  {
    bool prevSignalsBlocked = m_UI->levelsSpinBox->blockSignals(true);

    m_UI->levelsSpinBox->setValue(count);
    m_GDALOverviewsBuilder->SetNbResolutions(count);

    m_UI->levelsSpinBox->blockSignals(prevSignalsBlocked);
  }

  ClearResolutions();

  UpdateResolutions();

  Q_EMIT SizeValueChanged(value);
}

} // end namespace 'mvd'
