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


#ifndef mvdMainWindow_h
#define mvdMainWindow_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"

#define USE_PIXEL_DESCRIPTION 0

#define RENDER_IMAGE_VIEW_DISABLED 0
#define RENDER_QUICKLOOK_VIEW_DISABLED 0


/*****************************************************************************/
/* INCLUDE SECTION                                                           */


#ifndef USE_OTB_APPS
#define USE_OTB_APPS 1
#endif

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtOpenGL>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdTypes.h"
#include "mvdStackedLayerModel.h"
//
#include "mvdI18nMainWindow.h"

#include "OTBMonteverdiExport.h"

/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External class pre-declaration.
namespace
{
}

namespace mvd
{
//
// Internal class pre-declaration.
// Core
class AbstractLayerModel;
// Gui
class FilenameDragAndDropEventFilter;
class ImageViewWidget;
class KeymapDialog;
class LayerStackWidget;
class ShaderWidget;
class StatusBarWidget;
class ProjectionBarWidget;

namespace Ui
{
class MainWindow;
}

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class MainWindow
 *
 * \ingroup OTBMonteverdi
 *
 * \brief The application main-widow widget.
 */
class OTBMonteverdi_EXPORT MainWindow : public I18nMainWindow
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

  //
  // Public types.
public:
  //
  // Public methods.
public:
  /** \brief Constructor. */
  MainWindow(QWidget* p = 0, Qt::WindowFlags flags = 0);

  /** \brief Destructor. */
  ~MainWindow() override;

  /**
   */
  bool CheckGLCapabilities(bool forceNoGLSL = false);

/**
 */
#if USE_OTB_APPS
  void SetupOTBApplications();
#endif // USE_OTB_APPS

  /*-[ PROTECTED SLOTS SECTION ]---------------------------------------------*/

  //
  // Public slots.
public slots:

  /**
   */
  CountType ImportImage(const QString& filename, StackedLayerModel::SizeType index);

  /**
   */
  void ImportImages(const QStringList& filenames, bool enableOverviews = true);

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

  //
  // SIGNALS.
signals:

  void UserCoordinatesEditingFinished(const QString&);

  void UserScaleEditingFinished(const QString&);

  void UserZoomIn();

  void UserZoomOut();

  void UserZoomExtent();

  void UserZoomFull();

  void UserZoomLayer();

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

  //
  // Protected methods.
protected:
  using I18nMainWindow::ImportImage;

  //
  // QMainWindow methods.

  void closeEvent(QCloseEvent* event) override;

  //
  // Protected attributes.
protected:
  /*-[ PROTECTED SLOTS SECTION ]---------------------------------------------*/

  //
  // Protected slots.
protected slots:

  /**
   */
  void OnAboutToChangeModel(const AbstractModel*) override;

  /**
   */
  void OnModelChanged(AbstractModel*) override;

  /**
   */
  // void OnAboutToChangeSelectedDatasetModel( const DatasetModel* );

  /**
   */
  // void OnSelectedDatasetModelChanged( DatasetModel* );

  /**
   */
  void OnAboutToChangeSelectedLayerModel(const StackedLayerModel::KeyType&);

  /**
   */
  void OnSelectedLayerModelChanged(const StackedLayerModel::KeyType&);

/**
 */
#if defined(OTB_USE_QT) && USE_OTB_APPS
  void OnApplicationToLaunchSelected(const QString& appName);
#endif // defined( OTB_USE_QT ) && USE_OTB_APPS

  /** */
  void OnOTBApplicationOutputImageChanged(const QString& appName, const QString& outfname);

  /**
   */
  void OnExecutionDone(int status);

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

  //
  // Private types.
private:
  //
  // Private methods.
private:
  /**
   */
  ImageViewWidget* CreateImageViewWidget(QOpenGLWidget* sharedGlWidget = NULL);

  /**
   */
  ImageViewWidget* CreateQuicklookViewWidget(QOpenGLWidget* sharedGlWidget = NULL);

  /**
   */
  void InitializeDockWidgets();

  /**
   */
  void InitializeCentralWidget();

  /**
   */
  void InitializeRenderToolBar();

  /**
   */
  void InitializeShaderToolBar();

  /**
   */
  void InitializeProjectionBarWidget();

  /**
   */
  void InitializeStatusBarWidgets();

  /**
   */
  inline const ImageViewWidget* GetQuicklookView() const;
  /**
   */
  inline ImageViewWidget* GetQuicklookView();

  /**
   */
  void ConnectImageViews();

  /**
   */
  void ConnectViewMenu();

  /**
   */
  void ConnectReferenceLayerComboBox(StackedLayerModel*);

  /**
   */
  void DisconnectReferenceLayerComboBox(StackedLayerModel*);

  /**
   */
  void SetupReferenceLayerComboBox(StackedLayerModel*);

  /**
   */
  void ConnectStatusBar();

  /**
   */
  void ConnectProjectionBarWidget();

#if USE_PIXEL_DESCRIPTION

  /**
   */
  void ConnectPixelDescriptionWidget(AbstractLayerModel* model);

  /**
   */
  void DisconnectPixelDescriptionWidget(const AbstractLayerModel* model);

#endif // USE_PIXEL_DESCRIPTION

  /**
   */
  void SetGLSLEnabled(bool);

  //
  // I18nMainWindow methods.

  void virtual_SetupUI() override;

  void virtual_ConnectUI() override;

  void virtual_InitializeUI() override;

  //
  // Private attributes.
private:
  /**
   * \brief uic generated.
   */
  Ui::MainWindow* m_UI;

  /**
   * \brief Color dynamicsk-view dock-widget.
   */
  QDockWidget* m_ColorDynamicsDock;

  /**
   * \brief Color setup dock-widget.
   */
  QDockWidget* m_ColorSetupDock;

  /**
   * \brief Database-browser dock-widget.
   */
  // QDockWidget* m_DatabaseBrowserDock;

  /**
   * \brief Dataset-properties dock-widget.
   */
  // QDockWidget* m_DatasetPropertiesDock;

  /**
   * \brief Layer-stack dock-widget.
   */
  QDockWidget* m_LayerStackDock;

/**
 * \brief Current Pixel Description dock-widget.
 */
#if USE_PIXEL_DESCRIPTION
  QDockWidget* m_PixelDescriptionDock;
#endif // USE_PIXEL_DESCRIPTION

  /**
   * \brief Histogram dock-widget.
   */
  QDockWidget* m_HistogramDock;


#ifdef OTB_USE_QT
  /**
   *  \brief OTB-applications browser dock-widget.
   */
  QDockWidget* m_OtbApplicationsBrowserDock;
#endif

  /**
   * \brief Image-view page.
   */
  ImageViewWidget* m_ImageView;

  /**
   * \brief Quicklook-view dock.
   */
  QDockWidget* m_QuicklookViewDock;

  /**
   */
  StatusBarWidget* m_StatusBarWidget;

  /**
   */
  ShaderWidget* m_ShaderWidget;

  /**
   */
  FilenameDragAndDropEventFilter* m_FilenameDragAndDropEventFilter;

  /**
   */
  KeymapDialog* m_KeymapDialog;

  /**
   */
  ProjectionBarWidget* m_ProjectionBarWidget;

  /**
   */
  int m_GLSL140;

  /**
   */
  bool m_ForceNoGLSL : 1;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

  //
  // Private slots.
private slots:

  /**
   */
  void on_action_GLSL_triggered(bool);

  /**
   * \brief Qt auto-connected slot which is called when Help/About
   * menu action is activated.
   */
  void on_action_Keymap_triggered();

  /**
   */
  void on_action_LoadOTBApplications_triggered();

  /**
   * \brief Qt auto-connected slot which is called when 'File/Open
   * image' menu action is triggered.
   */
  void on_action_OpenImage_triggered();

  /**
   * \brief Qt auto-connected slot which is called when
   * 'File/Preferences' menu action is trigerred.
   */
  void on_action_Preferences_triggered();

  /**
   */
  void on_action_SaveScreenshot_triggered();

  /**
   * \brief Qt auto-connected slot which is called when zoom-in button
   * is pressed.
   */
  void on_action_ZoomIn_triggered();

  /**
   * \brief Qt auto-connected slot which is called when zoom-out button
   * is pressed.
   */
  void on_action_ZoomOut_triggered();

  /**
   * \brief Qt auto-connected slot which is called when zoom-extent button
   * is pressed.
   */
  void on_action_ZoomExtent_triggered();

  /**
   * \brief Qt auto-connected slot which is called when zoom-full button
   * is pressed.
   */
  void on_action_ZoomFull_triggered();

  /**
   * \brief Qt auto-connected slot which is called when zoom-layer button
   * is pressed.
   */
  void on_action_ZoomLayer_triggered();

  /**
   */
  void OnReferenceLayerCurrentIndexChanged(int);

  /**
   */
  void OnReferenceLayerChanged(size_t);

  /**
   */
  void OnSettingsUpdated();

  /**
   */
  void RefreshReferenceLayerComboBox();

  /**
   */
  void OnPixelInfoChanged(const QPoint&, const PointType&, const PixelInfo::Vector&);
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdImageViewWidget.h"

//
// Some constants.

namespace mvd
{

/*****************************************************************************/
inline const ImageViewWidget* MainWindow::GetQuicklookView() const
{
  return qobject_cast<const ImageViewWidget*>(m_QuicklookViewDock->widget());
}

/*****************************************************************************/
inline ImageViewWidget* MainWindow::GetQuicklookView()
{
  return qobject_cast<ImageViewWidget*>(m_QuicklookViewDock->widget());
}

} // end namespace 'mvd'

#endif // mvdMainWindow_h
