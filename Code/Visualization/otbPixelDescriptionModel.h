/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbPixelDescriptionModel_h
#define __otbPixelDescriptionModel_h

#include "otbMVCModel.h"
#include "otbLayerBasedModel.h"
#include "otbPixelDescriptionModelListener.h"
#include "otbImageLayerBase.h"

namespace otb
{
/** \class PixelDescriptionModel
*   \brief This class is a layer based model reporting pixel information
*
*   \sa ImageLayer
*
*  \ingroup Visualization
 */

template <class TOutputImage>
class PixelDescriptionModel
  : public MVCModel<PixelDescriptionModelListener>, public LayerBasedModel<ImageLayerBase<TOutputImage> >
{
public:
  /** Standard class typedefs */
  typedef PixelDescriptionModel                          Self;
  typedef LayerBasedModel<ImageLayerBase<TOutputImage> > Superclass;
  typedef itk::SmartPointer<Self>                        Pointer;
  typedef itk::SmartPointer<const Self>                  ConstPointer;

  /** Runtime information */
  itkTypeMacro(PixelDescriptionModel, LayerBasedModel);

  /** New macro */
  itkNewMacro(Self);

  /** Layer typedef */
  typedef typename Superclass::LayerType LayerType;

  /** We assume that TLayer is has an IndexType definition */
  typedef typename LayerType::IndexType IndexType;

  /** Listener typedef */
  typedef PixelDescriptionModelListener ListenerType;

  /** Get/Set the viewer name */
  itkGetStringMacro(PixelDescription);

  /** Clear the pixel description */
  void ClearPixelDescription();

  /** Update the pixel description */
  void UpdatePixelDescription(const IndexType& index, bool usePlaceName = true);

protected:
  /** Constructor */
  PixelDescriptionModel();
  /** Destructor */
  ~PixelDescriptionModel();

  /** Printself method */
  void         PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Notify a registered listener */
  void        NotifyListener(ListenerType * listener);

private:
  PixelDescriptionModel(const Self&);     // purposely not implemented
  void operator =(const Self&); // purposely not implemented

  /** The pixel description */
  std::string m_PixelDescription;

}; // end class
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPixelDescriptionModel.txx"
#endif

#endif
