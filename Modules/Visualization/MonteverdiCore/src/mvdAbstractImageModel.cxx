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


#include "mvdAbstractImageModel.h"

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdQuicklookModel.h"

namespace mvd
{
/*
  TRANSLATOR mvd::AbstractImageModel

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

namespace
{

/**
 * \brief Array of enhanced band names that OTB can return.
 *
 * It is defined (statically) as a constant for translation purposes.
 */
char const* const ENHANCED_BAND_NAMES[10] = {
    QT_TRANSLATE_NOOP("mvd::AbstractImageModel", "Red"),     QT_TRANSLATE_NOOP("mvd::AbstractImageModel", "Green"),
    QT_TRANSLATE_NOOP("mvd::AbstractImageModel", "Blue"),    QT_TRANSLATE_NOOP("mvd::AbstractImageModel", "Yellow"),
    QT_TRANSLATE_NOOP("mvd::AbstractImageModel", "RedEdge"), QT_TRANSLATE_NOOP("mvd::AbstractImageModel", "Coastal"),
    QT_TRANSLATE_NOOP("mvd::AbstractImageModel", "NIR"),     QT_TRANSLATE_NOOP("mvd::AbstractImageModel", "NIR1"),
    QT_TRANSLATE_NOOP("mvd::AbstractImageModel", "NIR2"),    QT_TRANSLATE_NOOP("mvd::AbstractImageModel", "PAN"),
};

} // end of anonymous namespace.

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */

namespace hidden
{

// Do never call this private hidden function.
void fix_unused_variable_warning()
{
  // This variable is used to declare OTB band names to the Qt I18N
  // translation process.
  //
  // The translated string are used by side effect to the
  // QObject::translate() function which searches the translated
  // strings from the .qm files loaded at runtime.
  (void)ENHANCED_BAND_NAMES;
}
};

/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
AbstractImageModel::AbstractImageModel(QObject* p) : AbstractLayerModel(p), m_NativeLargestRegion(), m_Id(-1), m_CurrentLod(0), m_Properties(NULL)
{
}

/*******************************************************************************/
AbstractImageModel::~AbstractImageModel()
{
  SetProperties(NULL);
}

/*****************************************************************************/
const QuicklookModel* AbstractImageModel::GetQuicklookModel() const
{
  return findChild<const QuicklookModel*>();
}

/*****************************************************************************/
QuicklookModel* AbstractImageModel::GetQuicklookModel()
{
  return findChild<QuicklookModel*>();
}

/*******************************************************************************/
QStringList AbstractImageModel::GetBandNames(bool enhanced) const
{
  ImageBaseType::ConstPointer output(ToImageBase());

  otb::ImageMetadataInterfaceBase::ConstPointer metaDataInterface(GetMetaDataInterface());

  //
  // Basic band names.
  StringVector stdBandNames1(metaDataInterface->GetBandName());

  // output band name list
  QStringList qBandNames1(QString(""));

  //
  // PS: need to handle images with extracted channels and a geom file
  // storing the original image band names.
  if (stdBandNames1.empty() || stdBandNames1.size() != output->GetNumberOfComponentsPerPixel())
  {
    // output stringVector
    StringVector outputBandNames;

    //
    // Fill the output std::vector with default values
    for (unsigned int count = 0; count < output->GetNumberOfComponentsPerPixel(); count++)
    {
      std::ostringstream oss;
      oss << ToStdString(tr("BAND ")) << count + 1;
      outputBandNames.push_back(oss.str());
    }

    // set the output QStringList
    qBandNames1 = ToQStringList(outputBandNames);
  }
  else
  {
    // useless
    assert(!stdBandNames1.empty() || stdBandNames1.size() == output->GetNumberOfComponentsPerPixel());

    if (stdBandNames1.size() != output->GetNumberOfComponentsPerPixel())
    {
      stdBandNames1.resize(output->GetNumberOfComponentsPerPixel());
    }

    //
    // Enhanced band names.
    StringVector stdBandNames2;
    if (enhanced)
    {
      try
      {
        stdBandNames2 = metaDataInterface->GetEnhancedBandNames();
      }
      catch (std::exception& exc)
      {
        qWarning() << exc.what();
      }
    }

    assert(stdBandNames2.empty() || stdBandNames2.size() == output->GetNumberOfComponentsPerPixel());

    if (stdBandNames2.size() != output->GetNumberOfComponentsPerPixel())
    {
      stdBandNames2.resize(output->GetNumberOfComponentsPerPixel());
    }

    //
    // Join basic and enhanced band-name lists.
    assert(stdBandNames1.size() == stdBandNames2.size());

    qBandNames1 = ToQStringList(stdBandNames1);
    QStringList qBandNames2(ToQStringList(stdBandNames2));

    assert(qApp != NULL);

    QStringList::iterator       it1(qBandNames1.begin());
    QStringList::const_iterator it2(qBandNames2.begin());

    for (; it1 != qBandNames1.end(); ++it1, ++it2)
    {
      if (!it2->isEmpty())
        it1->append(" (" + qApp->translate("mvd::AbstractImageModel", it2->toLatin1().constData()) + ")");
    }
  }

  //
  // Return joined band-name list.
  return qBandNames1;
}

/*******************************************************************************/
void AbstractImageModel::RefreshHistogram(void* const context)
{
  // qDebug() << this << "::RefreshHistogram(" << context << ")";

  // Remember existing child histogram.
  HistogramModel* currentHistogramModel = GetHistogramModel();

#if 0
  // Access histogram-model build-context.
  HistogramModel::BuildContext* const buildContext =
    static_cast< HistogramModel::BuildContext* const >( context );

  // Force writing histgram file by setting IsBeingStored flag.
  buildContext->m_IsBeingStored = true;
#endif

  // Create new child histogram providing build-context.
  newChildModel<HistogramModel>(context);

  // Delete (and remove) previous child histogram.
  delete currentHistogramModel;
  currentHistogramModel = NULL;
}

/*******************************************************************************/
void AbstractImageModel::virtual_BuildModel(void* context)
{
  //
  // Retrieve proper build-context.
  assert(context != NULL);
  BuildContext* buildContext = static_cast<BuildContext*>(context);

  //
  // ID.
  m_Id = buildContext->m_Id;

  //
  // QUICKLOOK.
  //
  // Build quicklook before histogram because histogram calculation is
  // based on quicklook image.
  newChildModel<QuicklookModel>(context);

  //
  // HISTOGRAM.
  if (buildContext->m_Histogram.isEmpty())
    newChildModel<HistogramModel>();

  else
  {
    HistogramModel::BuildContext histogramContext(buildContext->IsBeingStored(), buildContext->m_Histogram);

    newChildModel<HistogramModel>(&histogramContext);
  }
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
