/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#include "mvdImportImagesDialog.h"
#include "ui_mvdImportImagesDialog.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)
#include <cassert>

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdAlgorithm.h"
#include "mvdCore.h"
#include "mvdI18nCoreApplication.h"

namespace mvd
{

/*
  TRANSLATOR mvd::ImportImagesDialog

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

namespace
{
/**
 */
enum Columns
{
  COLUMN_NONE = -1,
  //
  COLUMN_IMAGE_SIZE = 0,
  COLUMN_OVERVIEW_SIZE,
  COLUMN_FILENAME,
  //
  COLUMN_COUNT,
};


const char* const HEADERS[COLUMN_COUNT] = {
    QT_TRANSLATE_NOOP("mvd::ImportImagesDialog", "Img Size"), QT_TRANSLATE_NOOP("mvd::ImportImagesDialog", "Ovr Size"),
    QT_TRANSLATE_NOOP("mvd::ImportImagesDialog", "Filename"),
};

} // end of anonymous namespace.


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */
/*****************************************************************************/
ImportImagesDialog::ImportImagesDialog(const QStringList& filenames, QWidget* p, Qt::WindowFlags flags)
  : QDialog(p, flags), m_UI(new Ui::ImportImagesDialog()), m_EffectiveCount(0)
{
  m_UI->setupUi(this);

  {
    QItemSelectionModel* ism = m_UI->filenamesTreeView->selectionModel();

    m_UI->filenamesTreeView->setModel(new QStandardItemModel(0, COLUMN_COUNT, m_UI->filenamesTreeView));

    delete ism;
    ism = NULL;
  }

  QObject::connect(m_UI->filenamesTreeView->model(), SIGNAL(itemChanged(QStandardItem*)),
                   // to:
                   this, SLOT(OnItemChanged(QStandardItem*)));

  QObject::connect(m_UI->filenamesTreeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
                   // to:
                   this, SLOT(OnCurrentChanged(const QModelIndex&, const QModelIndex&)));

  SetFilenames(filenames);
}

/*****************************************************************************/
ImportImagesDialog::~ImportImagesDialog()
{
  delete m_UI;
  m_UI = NULL;
}

/*****************************************************************************/
const otb::GDALOverviewsBuilder::Pointer& ImportImagesDialog::GetBuilder(const QModelIndex& index) const
{
  assert(index.isValid());

  assert(index.row() >= 0 && static_cast<size_t>(index.row()) < m_GDALOverviewsBuilders.size());

  return m_GDALOverviewsBuilders[index.row()];
}

/*****************************************************************************/
int ImportImagesDialog::GetEffectiveCount() const
{
  return m_EffectiveCount;
}

/*****************************************************************************/
const ImportImagesDialog::GDALOverviewsBuilderVector& ImportImagesDialog::GetGDALOverviewsBuilders() const
{
  return m_GDALOverviewsBuilders;
}

/*****************************************************************************/
void ImportImagesDialog::SetFilenames(const QStringList& filenames)
{
  assert(m_UI != NULL);
  assert(m_UI->filenamesTreeView != NULL);

  m_GDALOverviewsBuilders.clear();

  QStandardItemModel* itemModel = qobject_cast<QStandardItemModel*>(m_UI->filenamesTreeView->model());

  assert(itemModel != NULL);

  itemModel->clear();

  SetHeaders();

  m_EffectiveCount = 0;

  for (int i = 0; i < filenames.size(); ++i)
  {
    std::string filename(QFile::encodeName(filenames[i]).constData());
    assert(!filename.empty());

    if (otb::GDALOverviewsBuilder::CanGenerateOverviews(filename))
    {
      otb::GDALOverviewsBuilder::Pointer builder(otb::GDALOverviewsBuilder::New());

      Qt::ItemFlags flags = Qt::NoItemFlags;

      try
      {
        builder->SetInputFileName(filename);

        flags |= Qt::ItemIsSelectable;

        unsigned int count = 0;

        {
          assert(I18nCoreApplication::Instance() != NULL);

          QVariant     value(I18nCoreApplication::Instance()->RetrieveSettingsKey(I18nCoreApplication::SETTINGS_KEY_OVERVIEWS_SIZE));
          unsigned int threshold = value.isValid() ? value.toInt() : OVERVIEWS_SIZE_DEFAULT;

          otb::GDALOverviewsBuilder::SizeVector ovrSizes;
          builder->SetResolutionFactor(2);
          // Count all resolution with minimum size 1 (the high requested count is
          // clamped by the builder)
          builder->ListResolutions(ovrSizes, 2, 9999);
          // Count the number of levels with a least 1 size larger than the default size
          for (unsigned int k = 0; k < ovrSizes.size(); k++)
          {
            if (std::max(ovrSizes[k][0], ovrSizes[k][1]) >= threshold)
              count++;
            else
              break;
          }
        }

        if (builder->GetOverviewsCount() > 0)
          builder->SetBypassEnabled(true);
        else if (count > 1)
        {
          flags |= Qt::ItemIsEnabled;

          builder->SetBypassEnabled(false);

          ++m_EffectiveCount;
        }
        else
        {
          builder->SetBypassEnabled(true);
        }

        builder->SetNbResolutions(count);
        builder->SetResamplingMethod(otb::GDAL_RESAMPLING_AVERAGE);
        builder->SetCompressionMethod(otb::GDAL_COMPRESSION_NONE);
        builder->SetFormat(otb::GDAL_FORMAT_GEOTIFF);
      }
      catch (const std::exception& e)
      {
        QMessageBox::warning(
            this, PROJECT_NAME,
            tr("The following exception has raised when scanning file '%1' for GDAL overview settings:\n\n%2").arg(filenames[i]).arg(e.what()));

        builder = otb::GDALOverviewsBuilder::Pointer();
      }

      m_GDALOverviewsBuilders.push_back(builder);

      typedef QList<QStandardItem*> ItemList;

      ItemList items;

      //
      // COLUMN_IMAGE_SIZE
      if (!builder.IsNull())
      {
        items.push_back(new QStandardItem(QString("%1x%2").arg(builder->GetWidth()).arg(builder->GetHeight())));

        items.back()->setFlags(flags);
      }

      //
      // COLUMN_FILE_SIZE
      items.push_back(
          new QStandardItem(ToHumanReadableSize(builder->GetOverviewsCount() > 0 ? QFileInfo(filenames[i] + ".ovr").size() : builder->GetEstimatedSize())));

      items.back()->setFlags(flags);

      //
      // COLUMN_FILENAME
      items.push_back(new QStandardItem(filenames[i]));

      if (flags.testFlag(Qt::ItemIsEnabled))
      {
        items.back()->setFlags(flags | Qt::ItemIsUserCheckable);

        items.back()->setCheckState(builder->IsBypassEnabled() ? Qt::Unchecked : Qt::Checked);
      }
      else
        items.back()->setFlags(flags);

      //
      // Append items.
      itemModel->appendRow(items);
    }
#if (defined(OTB_DEBUG) && 1) || 0
    else
      qDebug() << "Skipped:" << filenames[i];
#endif
  }
}

/*****************************************************************************/
void ImportImagesDialog::SetHeaders()
{
  assert(m_UI != NULL);
  assert(m_UI->filenamesTreeView->model() == qobject_cast<QStandardItemModel*>(m_UI->filenamesTreeView->model()));

  QStandardItemModel* model = qobject_cast<QStandardItemModel*>(m_UI->filenamesTreeView->model());

  assert(model != NULL);

  for (int i = 0; i < COLUMN_COUNT; ++i)
  {
    // qDebug() <<
    //   qApp->translate(
    // 	"mvd::ImportImagesDialog",
    // 	HEADERS[ i ]
    //   );

    // labels <<
    //   qApp->translate(
    // 	"mvd::ImportImagesDialog",
    // 	HEADERS[ i ]
    //   );

    // model->horizontalHeaderItem( i )->setText(
    //   qApp->translate(
    // 	"mvd::ImportImagesDialog",
    // 	HEADERS[ i ]
    //   )
    // );

    model->setHorizontalHeaderItem(i, new QStandardItem(qApp->translate("mvd::ImportImagesDialog", HEADERS[i])));
  }

  // qDebug() << labels;

  // model->setHorizontalHeaderLabels( labels );
}

/*****************************************************************************/
void ImportImagesDialog::UpdateFileSize(size_t row)
{
  assert(row < m_GDALOverviewsBuilders.size());

  assert(!m_GDALOverviewsBuilders[row].IsNull());
  assert(m_GDALOverviewsBuilders[row]->GetOverviewsCount() <= 0);

  //
  // Get item-model.
  assert(m_UI != NULL);
  assert(m_UI->filenamesTreeView != NULL);

  QStandardItemModel* model = qobject_cast<QStandardItemModel*>(m_UI->filenamesTreeView->model());

  assert(model != NULL);

  //
  // Create index.
  QModelIndex index(model->index(row, COLUMN_OVERVIEW_SIZE));

  //
  // Get GDAL overview builder.
  otb::GDALOverviewsBuilder::Pointer builder(m_GDALOverviewsBuilders[index.row()]);
  assert(!builder.IsNull());

  //
  // Update file size.
  model->setData(index, ToHumanReadableSize(builder->GetEstimatedSize()));
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/
void ImportImagesDialog::on_buttonBox_clicked(QAbstractButton* button)
{
  // qDebug() << this << "::on_buttonBox_clicked(" << button << ")";

  assert(m_UI != NULL);
  assert(button != NULL);

  switch (m_UI->buttonBox->standardButton(button))
  {
  case QDialogButtonBox::Ok:
    accept();
    break;

  case QDialogButtonBox::Cancel:
    reject();
    break;

  case QDialogButtonBox::Ignore:
    done(-1);
    break;

  default:
    assert(false && "Unhandled QDialogButtonBox::StandardButton enum value!");
    reject();
    break;
  }
}

/*****************************************************************************/
void ImportImagesDialog::on_pyramidWidget_BaseValueChanged(int)
{
  assert(m_UI != NULL);
  assert(m_UI->filenamesTreeView != NULL);
  assert(m_UI->filenamesTreeView->selectionModel() != NULL);

  UpdateFileSize(m_UI->filenamesTreeView->selectionModel()->currentIndex().row());
}

/*****************************************************************************/
void ImportImagesDialog::on_pyramidWidget_LevelsValueChanged(int)
{
  assert(m_UI != NULL);
  assert(m_UI->filenamesTreeView != NULL);
  assert(m_UI->filenamesTreeView->selectionModel() != NULL);

  UpdateFileSize(m_UI->filenamesTreeView->selectionModel()->currentIndex().row());
}

/*****************************************************************************/
void ImportImagesDialog::on_pyramidWidget_SizeValueChanged(int)
{
  assert(m_UI != NULL);
  assert(m_UI->filenamesTreeView != NULL);
  assert(m_UI->filenamesTreeView->selectionModel() != NULL);

  UpdateFileSize(m_UI->filenamesTreeView->selectionModel()->currentIndex().row());
}

/*****************************************************************************/
void ImportImagesDialog::OnCurrentChanged(const QModelIndex& current, const QModelIndex&)
{
  // qDebug() << this << "::OnCurrentChanged(" << current << "," << previous << ")";

  // const QStandardItemModel * itemModel =
  //   qobject_cast< const QStandardItemModel * >(
  //     m_UI->filenamesTreeView->model()
  //   );

  // assert( itemModel!=NULL );

  const otb::GDALOverviewsBuilder::Pointer& builder = GetBuilder(current);

  m_UI->pyramidWidget->setEnabled(!builder.IsNull() && !builder->IsBypassEnabled());

  m_UI->pyramidWidget->SetBuilder(builder);
}

/*****************************************************************************/
void ImportImagesDialog::OnItemChanged(QStandardItem* item)
{
  // qDebug() << this << "::OnItemChanged(" << item << ")";

  assert(item != NULL);

  QModelIndex index(item->index());


  if (index.column() != COLUMN_FILENAME)
    return;


  const otb::GDALOverviewsBuilder::Pointer& builder = GetBuilder(item->index());

  if (!builder.IsNull())
  {
    builder->SetBypassEnabled(item->checkState() != Qt::Checked);

    UpdateFileSize(index.row());
  }

  assert(m_UI != NULL);
  assert(m_UI->filenamesTreeView->selectionModel() != NULL);

  if (m_UI->filenamesTreeView->selectionModel()->currentIndex() != index)
    return;


  m_UI->pyramidWidget->setEnabled(!builder.IsNull() && !builder->IsBypassEnabled());
}

} // end namespace 'mvd'
