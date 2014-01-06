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
#include "otbGlView.h"
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h> 
#include <algorithm>

namespace otb
{
GlView::GlView()
  : m_Settings(ViewSettings::New()),
    m_Actors(),
    m_RenderingOrder()
{}

GlView::~GlView()
{}

void GlView::Initialize(unsigned int sx, unsigned int sy)
{
  m_Settings = ViewSettings::New();
 
  this->ClearActors();

  this->Resize(sx,sy);
 
}

void GlView::Resize(unsigned int sx, unsigned int sy)
{
 ViewSettings::SizeType size;
  size[0] = sx;
  size[1] = sy;
  m_Settings->SetViewportSize(size);
}

void GlView::BeforeRendering()
{
  // First, get all actors informed of new settings
  for(ActorMapType::iterator it = m_Actors.begin();
      it!=m_Actors.end();++it)
    {
    if(it->second->GetVisible())
      {
      it->second->ProcessViewSettings();
      }
    }

  // Second, get opengl ready

  // Reset viewport
  glViewport(0, 0, (GLint)m_Settings->GetViewportSize()[0], (GLint)m_Settings->GetViewportSize()[1]);

  // Clear back-buffer(s) before rendering
  glClear( GL_COLOR_BUFFER_BIT );

  // Setup projection according to view settings
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  double ulx, uly, lrx, lry;
  
  m_Settings->GetViewportExtent(ulx,uly,lrx,lry);

  glOrtho(ulx, lrx, uly, lry, -1, 1);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glPushMatrix();

  // glScalef(m_Settings->GetSpacing()[0],m_Settings->GetSpacing()[1],1);
}

void GlView::AfterRendering()
{
  m_Settings->SetGeometryChanged(false);
  glPopMatrix();
}

std::string GlView::AddActor(ActorType * actor, const std::string& key)
{
  std::string newKey = key;

  if(newKey == "")
    {
    std::ostringstream oss;
    oss<<"Actor"<<m_Actors.size()+1<<std::endl;
    newKey = oss.str();
    }

  if(m_Actors.count(newKey)!=0)
    {
    itkExceptionMacro(<<"An actor with key "<<newKey<<" already exists, please choose another key");
    }

  actor->SetSettings(m_Settings);
  m_Actors[newKey] = actor;
  m_RenderingOrder.push_back(newKey);

  return newKey;
}

void GlView::ClearActors()
{
  m_Actors.clear();
  m_RenderingOrder.clear();
}

GlView::ActorType::Pointer GlView::GetActor(const std::string & key)
{
  if(m_Actors.count(key)==0)
    {
    itkExceptionMacro(<<"No actor found with key"<<key);
    }
  
  return m_Actors[key];
}

void GlView::LightRender()
{
  for(StringVectorType::reverse_iterator it = m_RenderingOrder.rbegin();
      it!=m_RenderingOrder.rend();++it)
    {
    ActorMapType::iterator actIt = m_Actors.find(*it);

    if(actIt != m_Actors.end() && actIt->second->GetVisible())
      {
      actIt->second->Render();
      }
    }
}

void GlView::HeavyRender()
{
  // TODO: Light render/ heavy render could be optimized
  for(StringVectorType::reverse_iterator it = m_RenderingOrder.rbegin();
      it!=m_RenderingOrder.rend();++it)
    {
    ActorMapType::iterator actIt = m_Actors.find(*it);

    if(actIt != m_Actors.end() && actIt->second->GetVisible())
      {
      actIt->second->UpdateData();
      }
    }

  this->LightRender();
}


void GlView::RotateRenderingOrder(bool down)
{
  if(m_RenderingOrder.size()>1)
    {
    if(down)
      {
      std::rotate(m_RenderingOrder.rbegin(),m_RenderingOrder.rbegin()+1,m_RenderingOrder.rend());
      }
    else
      {
      std::rotate(m_RenderingOrder.begin(),m_RenderingOrder.begin()+1,m_RenderingOrder.end());
      }
    }


}

// Move actor in rendering order
void GlView::MoveActorInRenderingOrder(std::string key, bool down)
{
  StringVectorType::iterator it = std::find(m_RenderingOrder.begin(),m_RenderingOrder.end(),key);

  if(it!=m_RenderingOrder.end())
    {
    if(down && it+1 != m_RenderingOrder.end())
      {
      std::swap(*it,*(it+1));
      }
    else if(!down && it != m_RenderingOrder.begin())
      {
      std::swap((*it),*(it-1));
      }
    }
}

}
