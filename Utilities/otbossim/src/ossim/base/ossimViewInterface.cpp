#include <ossim/base/ossimViewInterface.h>

RTTI_DEF(ossimViewInterface, "ossimViewInterface");

ossimViewInterface::ossimViewInterface()
   : theObject(NULL)
{}

ossimViewInterface::ossimViewInterface(ossimObject* base)
   : theObject(base)
{}

ossimViewInterface::~ossimViewInterface()
{}

void ossimViewInterface::refreshView()
{}

ossimObject* ossimViewInterface::getBaseObject()
{
   return theObject;
}
