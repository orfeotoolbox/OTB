#include "otbGlActor.h"

namespace otb
{
GlActor::GlActor()
  : m_Settings(ViewSettings::New()),
    m_Visible(false)
{}

GlActor::~GlActor()
{}

}
