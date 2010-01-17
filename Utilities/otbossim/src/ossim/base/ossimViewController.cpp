#include <ossim/base/ossimViewController.h>
#include <ossim/base/ossimViewInterface.h>
#include <ossim/base/ossimConnectableContainerInterface.h>
#include <ossim/base/ossimListenerManager.h>
#include <ossim/base/ossimPropertyEvent.h>
#include <ossim/base/ossimNotifyContext.h>
#include <vector>
using namespace std;

RTTI_DEF1(ossimViewController, "ossimViewController", ossimSource);

ossimViewController::ossimViewController()
   :
      ossimSource((ossimObject*)NULL, 0, 0, true, false)
{
   theView = NULL;
}

ossimViewController::ossimViewController(ossimObject* owner,
                                         ossim_uint32 inputListSize,
                                         ossim_uint32 outputListSize,
                                         bool   inputListFixedFlag,
                                         bool   outputListFixedFlag)
   :
      ossimSource(owner,
                  inputListSize,
                  outputListSize,
                  inputListFixedFlag,
                  outputListFixedFlag)
{
   theView = NULL;
}

ossimViewController::~ossimViewController()
{
   theView = 0;
}

ossimString ossimViewController::getShortName()const
{
   return ossimString("View");
}

 ossimString ossimViewController::getLongName()const
{
   return ossimString("View controller");
}

bool ossimViewController::canConnectMyInputTo(ossim_int32,
                                              const ossimConnectableObject*)const
{
   return false;
}

bool ossimViewController::propagateView()
{
   bool returnResult = true;
   if(!theOwner)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL: " << getClassName() << "::propagateView(), has no owner! We must find all views to set.\n";
      return false;
   }
   ossimConnectableContainerInterface* inter = PTR_CAST(ossimConnectableContainerInterface,
                                                        theOwner);
   if(inter)
   {
      RTTItypeid typeId = STATIC_TYPE_INFO(ossimViewInterface);
      ossimConnectableObject::ConnectableObjectList result = inter->findAllObjectsOfType(typeId,
                                                                           true);
      if(result.size() > 0)
      {
         ossim_uint32 index = 0;

         // first set all views then update all outputs
         //
         for(index = 0; index < result.size(); ++index)
         {
            ossimViewInterface* viewInterface = PTR_CAST(ossimViewInterface, result[index].get());

            if(viewInterface)
            {
               if(!viewInterface->setView(theView.get()))
               {
                  returnResult = false;
               }
            }
         }

         for(index = 0; index < result.size(); ++index)
         {
            ossimPropertyEvent event(result[index].get());
            result[index]->fireEvent(event);
            result[index]->propagateEventToOutputs(event);
         }
      }
   }

   return returnResult;
}

bool ossimViewController::setView(ossimObject* object)
{
   theView = object;

   return true;
}

ossimObject* ossimViewController::getView()
{
   return theView.get();
}

const ossimObject* ossimViewController::getView()const
{
   return theView.get();
}

const ossimObject* ossimViewController::findFirstViewOfType(RTTItypeid typeId)const
{
   ossimConnectableContainerInterface* inter = PTR_CAST(ossimConnectableContainerInterface,
                                                        theOwner);
   if(inter)
   {
      RTTItypeid viewInterfaceType = STATIC_TYPE_INFO(ossimViewInterface);
      ossimConnectableObject::ConnectableObjectList result = inter->findAllObjectsOfType(viewInterfaceType,
                                                                           true);
      if(result.size() > 0)
      {
         ossim_uint32 index = 0;

         for(index = 0; index < result.size(); ++index)
         {

            ossimViewInterface* viewInterface = PTR_CAST(ossimViewInterface, result[index].get());

            if(viewInterface)
            {
               if(viewInterface->getView())
               {
                  if(typeId.can_cast(viewInterface->getView()->getType()))
                  {
                     return viewInterface->getView();
                  }
               }
            }
         }
      }
   }

   return (ossimObject*)NULL;
}
