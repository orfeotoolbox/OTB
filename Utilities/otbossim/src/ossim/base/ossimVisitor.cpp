// $Id: ossimVisitor.cpp 22158 2013-02-20 12:29:10Z gpotts $

#include <ossim/base/ossimVisitor.h>
#include <ossim/base/ossimConnectableObject.h>
#include <ossim/base/ossimPropertyEvent.h>
#include <ossim/base/ossimViewInterface.h>


ossimVisitor::ossimVisitor(int visitorType)
:m_visitorType(static_cast<VisitorType>(visitorType)),
m_stopTraversalFlag(false)
{
   
}

ossimVisitor::ossimVisitor(const ossimVisitor& src)
:m_visitorType(src.m_visitorType),
m_markNode(src.m_markNode),
m_stopTraversalFlag(src.m_stopTraversalFlag)
{
}

ossimVisitor::VisitorType ossimVisitor::getVisitorType()const
{
   return m_visitorType;
}

void  ossimVisitor::setVisitorType(int vType, bool on)
{
   if(on)
   {
      m_visitorType = (VisitorType)((vType|m_visitorType)&VISIT_ALL);
   }
   else 
   {
      m_visitorType = (VisitorType)(((~vType) & m_visitorType)&VISIT_ALL);
   }
}

void ossimVisitor::turnOffVisitorType(int vType)
{
   setVisitorType(vType, false);
}

void ossimVisitor::reset()
{
   m_markNode.clear();
   m_stopTraversalFlag = false;
}

void ossimVisitor::visit(ossimObject* obj)
{
   m_markNode.insert(obj);
}

void ossimVisitor::visit(ossimConnectableObject* obj)
{
   visit(static_cast<ossimObject*>(obj));
}

bool ossimVisitor::hasVisited(ossimObject* obj)const
{
   return m_markNode.find(obj) != m_markNode.end(); 
}

bool ossimVisitor::stopTraversal()const
{
   return m_stopTraversalFlag;
}


ossimIdVisitor::ossimIdVisitor()
   :
   ossimVisitor(ossimVisitor::VISIT_INPUTS|ossimVisitor::VISIT_CHILDREN),
   m_object(0),
   m_id(0)
{
}

ossimIdVisitor::ossimIdVisitor(int visitorType)
   :
   ossimVisitor(visitorType),
   m_object(0),
   m_id(0)
{
}

ossimIdVisitor::ossimIdVisitor(const ossimId& id, int visitorType)
   :
   ossimVisitor(visitorType),
   m_object(0),
   m_id(id)
{
}

ossimIdVisitor::ossimIdVisitor(const ossimIdVisitor& src)
   :
   ossimVisitor(src),
   m_object(src.m_object),
   m_id(src.m_id)
{
}

ossimRefPtr<ossimVisitor> ossimIdVisitor::dup()const
{
   return new ossimIdVisitor(*this);
}

void ossimIdVisitor::visit(ossimConnectableObject* obj)
{
   if ( !hasVisited(obj) )
   {
      if(obj->getId() == m_id)
      {
         m_object = obj;
         m_stopTraversalFlag = true;
      }
      ossimVisitor::visit(obj);
   }
}

void ossimIdVisitor::reset()
{
   ossimVisitor::reset();
   m_object = 0;
}

ossimConnectableObject* ossimIdVisitor::getObject()
{
   return m_object;
}

const ossimConnectableObject* ossimIdVisitor::getObject()const
{
   return m_object;
}

void ossimIdVisitor::setId(const ossimId& id)
{
   m_id = id;
}

const ossimId& ossimIdVisitor::getId()const
{
   return m_id;
}

ossimCollectionVisitor::ossimCollectionVisitor(int visitorType)
:ossimVisitor(visitorType)
{
}

ossimCollectionVisitor::ossimCollectionVisitor(const ossimCollectionVisitor& src)
:m_collection(src.m_collection)
{
}

ossimCollectionVisitor::ListRef& ossimCollectionVisitor::getObjects()
{
   return m_collection;
}

const ossimCollectionVisitor::ListRef& ossimCollectionVisitor::getObjects()const
{
   return m_collection;
}

ossimObject* ossimCollectionVisitor::getObject(ossim_uint32 idx)
{
   if(idx < m_collection.size())
   {
      return m_collection[idx].get();
   }
   return 0;
}

const ossimObject* ossimCollectionVisitor::getObject(ossim_uint32 idx)const
{
   if(idx < m_collection.size())
   {
      return m_collection[idx].get();
   }
   return 0;
}

void ossimCollectionVisitor::reset()
{
   ossimVisitor::reset();
   m_collection.clear();
}

ossimTypeNameVisitor::ossimTypeNameVisitor(int visitorType)
   :ossimCollectionVisitor(visitorType),
    m_typeName(),
    m_firstOfTypeFlag(false)
{
}

ossimTypeNameVisitor::ossimTypeNameVisitor(const ossimString& typeName,
                                           bool firstOfTypeFlag,
                                           int visitorType)
   :
   ossimCollectionVisitor(visitorType),
   m_typeName(typeName),
   m_firstOfTypeFlag(firstOfTypeFlag)
{
}

ossimTypeNameVisitor::ossimTypeNameVisitor(const ossimTypeNameVisitor& src)
:m_typeName(src.m_typeName),
 m_firstOfTypeFlag(src.m_firstOfTypeFlag)
{
}

ossimRefPtr<ossimVisitor> ossimTypeNameVisitor::dup()const
{
   return new ossimTypeNameVisitor(*this);
}

void ossimTypeNameVisitor::visit(ossimObject* obj)
{
   if(!hasVisited(obj))
   {
      ossimCollectionVisitor::visit(obj);
      if(obj&&obj->canCastTo(m_typeName))
      {
         m_collection.push_back(obj);
         if(m_firstOfTypeFlag) m_stopTraversalFlag = true;
      }
  }
}

void ossimTypeNameVisitor::setTypeName(const ossimString& typeName)
{
   m_typeName = typeName;
}

const ossimString& ossimTypeNameVisitor::getTypeName()const
{
   return m_typeName;
}

void ossimTypeNameVisitor::setFirstOfTypeFlag(bool flag)
{
   m_firstOfTypeFlag = flag;
}

bool ossimTypeNameVisitor::getFirstOfTypeFlag() const
{
   return m_firstOfTypeFlag;
}

ossimTypeIdVisitor::ossimTypeIdVisitor(int visitorType)
:ossimCollectionVisitor(visitorType)
{
}

ossimTypeIdVisitor::ossimTypeIdVisitor(const RTTItypeid& typeId, bool firstOfTypeFlag, int visitorType)
:ossimCollectionVisitor(visitorType),
m_typeId(typeId),
m_firstOfTypeFlag(firstOfTypeFlag)
{
}

ossimTypeIdVisitor::ossimTypeIdVisitor(const ossimTypeIdVisitor& src)
:m_typeId(src.m_typeId),
m_firstOfTypeFlag(src.m_firstOfTypeFlag)
{
}

ossimRefPtr<ossimVisitor> ossimTypeIdVisitor::dup()const
{
   return new ossimTypeIdVisitor(*this);
}

void ossimTypeIdVisitor::setTypeId(const RTTItypeid& typeId)
{
   m_typeId = typeId;
}

const RTTItypeid& ossimTypeIdVisitor::getTypeId()const
{
   return m_typeId;
}

void ossimTypeIdVisitor::visit(ossimObject* obj)
{
   if(!hasVisited(obj))
   {
      ossimCollectionVisitor::visit(obj);
      if(obj&&obj->canCastTo(m_typeId))
      {
         m_collection.push_back(obj);
         if(m_firstOfTypeFlag) m_stopTraversalFlag = true;
      }
   }
}

ossimEventVisitor::ossimEventVisitor(ossimEvent* evt, int visitorType)
:ossimVisitor(visitorType),
m_event(evt)
{
}
ossimEventVisitor::ossimEventVisitor(const ossimEventVisitor& src)
:ossimVisitor(src),
m_event(src.m_event)
{
}

ossimRefPtr<ossimVisitor> ossimEventVisitor::dup()const
{
   return new ossimEventVisitor(*this);
}

void ossimEventVisitor::visit(ossimObject* obj)
{
   if(!hasVisited(obj))
   {
      ossimListenerManager* manager = dynamic_cast<ossimListenerManager*> (obj);
      if(manager)
      {
         manager->fireEvent(*m_event);
         ossimVisitor::visit(obj);
      }
   }
}

ossimViewInterfaceVisitor::ossimViewInterfaceVisitor(ossimObject* view, int visitorType)
   :
   ossimVisitor(visitorType),
   m_view(view)
{
}

ossimViewInterfaceVisitor::ossimViewInterfaceVisitor(const ossimViewInterfaceVisitor& src)
   :
   ossimVisitor(src),
   m_view(src.m_view)
{
}

ossimRefPtr<ossimVisitor> ossimViewInterfaceVisitor::dup()const
{
   return new ossimViewInterfaceVisitor(*this);
}

ossimObject* ossimViewInterfaceVisitor::getView()
{
   return m_view.get();
}

const ossimObject* ossimViewInterfaceVisitor::getView()const
{
   return m_view.get();
}

void ossimViewInterfaceVisitor::visit(ossimObject* obj)
{
   if( !hasVisited(obj) )
   {
      if ( m_view.get() )
      {
         ossimViewInterface* vi = dynamic_cast<ossimViewInterface*>(obj);
         if ( vi )
         {
            // Set the view:
            vi->setView( m_view.get() );

            // Send property event for outputs connected to us.
            ossimRefPtr<ossimEvent> propEvent = new ossimPropertyEvent(obj);
            ossimEventVisitor ev( propEvent.get(), VISIT_OUTPUTS );
            obj->accept(ev);
         }
      }
      
      // This will put the obj in the m_markNode for hasVisited method.
      ossimVisitor::visit(obj);
   }
}
