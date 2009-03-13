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
#ifndef __otbLayerBasedModel_h
#define __otbLayerBasedModel_h

#include "itkObject.h"
#include "otbObjectList.h"

namespace otb
{
/** \class LayerBasedModel
*   \brief Base class providing layer stack model
*
* \sa Layer, ImageLayer
* \sa BlendingFunction
*
*/

template <class TLayer >
class LayerBasedModel
  : public itk::Object
{
public:
  /** Standard class typedefs */
  typedef LayerBasedModel                      Self;
  typedef itk::Object                          Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;

  /** Runtime information */
  itkTypeMacro(LayerBasedModel,Object);

  /** New macro */
  itkNewMacro(Self);

  /** Layer typedef */
  typedef TLayer                               LayerType;

  /** Layer list typedef */
  typedef otb::ObjectList<LayerType>            LayerListType;
  typedef typename LayerListType::Pointer       LayerListPointerType;
  typedef typename LayerListType::ConstIterator LayerIteratorType;

  /** Add a new layer
   *  \param layer The layer to add.
   *  \return The location of the added layer.
   */
  virtual unsigned int AddLayer(LayerType * layer);

  /** Get the layer at the current index
   *  \param index The index of the layer to get.
   *  \return a pointer to the layer or NULL if no layer was found at
   * this location.
   */
  virtual LayerType *  GetLayer(unsigned int index);

  /** Remove the layer at the current index
   *  \param index The index of the layer to remove.
   *  \return true if a layer was actually deleted, false otherwise.
   * this location.
   */
  virtual bool         DeleteLayer(unsigned int index);

  /** Returns the first layer whose name matches the given name.
   *  \param name The name of the layer.
   *  \return a pointer to the layer or NULL if no layer was found.
   */
  virtual LayerType * GetLayerByName(std::string name);

  /** Delete the first layer whose name matches the given name.
   *  \param name The name of the layer.
   *  \return true if a layer was actually deleted, false otherwise.
   * this location.
   */
  virtual bool         DeleteLayerByName(std::string name);

  /** Clear all layers */
  virtual void         ClearLayers(void);

  /** \return The number of layers */
  virtual unsigned int GetNumberOfLayers(void);

  /** Set/Get the layers stack */
  itkSetObjectMacro(Layers,LayerListType);
  itkGetObjectMacro(Layers,LayerListType);


protected:
  /** Constructor */
  LayerBasedModel();
  /** Destructor */
  virtual ~LayerBasedModel();

  /** Printself method */
  void          PrintSelf(std::ostream& os, itk::Indent indent) const;

/** Get the layer list (Convenience for subclasses) */
  itkGetConstObjectMacro(Layers,LayerListType);

private:
  LayerBasedModel(const Self&);     // purposely not implemented
  void operator=(const Self&);      // purposely not implemented

  /** Layer list */
  LayerListPointerType  m_Layers;

}; // end class
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLayerBasedModel.txx"
#endif

#endif


