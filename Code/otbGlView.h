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
#ifndef otb_GlView_h
#define otb_GlView_h

#include "otbViewSettings.h"
#include "otbGlActor.h"

#include <map>
#include <vector>

namespace otb
{

class GlView 
  : public itk::Object
{
public:
  typedef GlView                                          Self;
  typedef itk::Object                                     Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  typedef otb::GlActor                                    ActorType;
  typedef std::map<std::string,ActorType::Pointer>        ActorMapType;
  typedef std::vector<std::string>                        StringVectorType;

  itkNewMacro(Self);

  // Initialize 
  void Initialize(unsigned int sx, unsigned int sy);

  // Add a new actor to the map
  std::string AddActor(ActorType * actor, const std::string & key = "");

  // Clear all actors
  void ClearActors();

  // Retrieve a given actor
  ActorType::Pointer GetActor(const std::string & key);

  // Things to do before rendering
  void BeforeRendering();

  // Things to do after rendering
  void AfterRendering();

  // Involves opengl only operations, no data loading
  void LightRender();

  // Possible data loading
  void HeavyRender();

  // Resize viewport
  void Resize(unsigned int sx, unsigned int sy);

  itkSetObjectMacro(Settings,ViewSettings);
  itkGetObjectMacro(Settings,ViewSettings);
  itkGetConstObjectMacro(Settings,ViewSettings);

  //comment this macro (not compiling with OTB 3.X)
  // Get Rendering order
  StringVectorType GetRenderingOrder() const
	{
    return m_RenderingOrder;
	};

  // This will rotate the rendering order (without modifying the order)
  void RotateRenderingOrder(bool down = false);

  // Move actor in rendering order
  void MoveActorInRenderingOrder(std::string key, bool down = false);

protected:
  GlView();

  virtual ~GlView();

private:
  // prevent implementation
  GlView(const Self&);
  void operator=(const Self&);

  ViewSettings::Pointer m_Settings;

  ActorMapType          m_Actors;

  StringVectorType      m_RenderingOrder;

}; // End class GlView

} // End namespace otb

#endif
