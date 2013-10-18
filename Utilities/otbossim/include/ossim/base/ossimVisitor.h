// $Id: ossimVisitor.h 21850 2012-10-21 20:09:55Z dburken $

#ifndef ossimVisitor_HEADER
#define ossimVisitor_HEADER 1
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimId.h>
#include <ossim/base/ossimEvent.h>
#include <ossim/base/ossimRefPtr.h>
#include <set>
#include <vector>

class ossimConnectableObject;


class OSSIM_DLL ossimVisitor : public ossimReferenced
{
public:
   typedef std::set<ossimRefPtr<ossimObject> >    MarkCollectionRef;
   typedef std::set<ossimRefPtr<ossimObject> >    CollectionRef;
   typedef std::vector<ossimRefPtr<ossimObject> > ListRef;
   typedef std::set<ossimObject*>                 Collection;
   typedef std::vector<ossimObject* >             List;
   /**
    * Enumeration type can be a mask and will traverse a graph of connectables based on the values.
    */
   enum VisitorType
   {
      VISIT_NONE     = 0,
      VISIT_CHILDREN = 1,
      VISIT_INPUTS   = 2,
      VISIT_OUTPUTS  = 4,
      VISIT_ALL      = (VISIT_CHILDREN|VISIT_INPUTS|VISIT_OUTPUTS)
   };
   
   ossimVisitor(int visitorType = VISIT_NONE);
   ossimVisitor(const ossimVisitor& src);
   virtual void reset();
   virtual ossimRefPtr<ossimVisitor> dup()const=0;
   virtual void visit(ossimObject* obj);
   virtual void visit(ossimConnectableObject* obj);
   VisitorType getVisitorType()const;
   void setVisitorType(int vType, bool on=true);
   void turnOffVisitorType(int vType);
   bool hasVisited(ossimObject* obj)const;
   bool stopTraversal()const;
protected:
   VisitorType            m_visitorType;
   mutable Collection     m_markNode;
   mutable bool           m_stopTraversalFlag;
};

class OSSIM_DLL ossimCollectionVisitor : public ossimVisitor
{
public:
   ossimCollectionVisitor(int visitorType =(VISIT_INPUTS|VISIT_CHILDREN));
   ossimCollectionVisitor(const ossimCollectionVisitor& src);
   ListRef& getObjects();
   const ListRef& getObjects()const;
   ossimObject* getObject(ossim_uint32 idx = 0);
   const ossimObject* getObject(ossim_uint32 idx = 0)const;

   // Inlined to force template instantiation.
   template <class T> T* getObjectAs(ossim_uint32 idx=0)
   {
      if(idx < m_collection.size())
      {
         return dynamic_cast<T*>(m_collection[idx].get());
      }
      return 0;
   }

   // Inlined to force template instantiation.
   template<class T> const T* getObjectAs(ossim_uint32 idx=0)const
   {
      if(idx < m_collection.size())
      {
         return dynamic_cast<const T*>(m_collection[idx].get());
      }
      return 0;
   }

   virtual void reset();
 
protected:
   ListRef m_collection; 
   
};

class OSSIM_DLL ossimIdVisitor : public ossimVisitor
{
public:
   ossimIdVisitor(int visitorType =(VISIT_INPUTS|VISIT_CHILDREN));
   ossimIdVisitor(const ossimId& id = ossimId(), int visitorType =(VISIT_INPUTS|VISIT_CHILDREN));
   ossimIdVisitor(const ossimIdVisitor& src);
   virtual ossimRefPtr<ossimVisitor> dup()const;
   virtual void visit(ossimConnectableObject* obj);

   /** @brief Resets m_object to 0, calls ossimVisitor::reset(). */
   virtual void reset();
   
   ossimConnectableObject* getObject();
   const ossimConnectableObject* getObject()const;
   void setId(const ossimId& id);
   const ossimId& getId()const;
protected:
   ossimConnectableObject* m_object;
   ossimId m_id;
};

class OSSIM_DLL ossimTypeNameVisitor : public ossimCollectionVisitor
{
public:
   ossimTypeNameVisitor(int visitorType =(VISIT_INPUTS|VISIT_CHILDREN));
   ossimTypeNameVisitor(const ossimString& typeName, bool firstofTypeFlag=false, int visitorType =(VISIT_INPUTS|VISIT_CHILDREN));
   ossimTypeNameVisitor(const ossimTypeNameVisitor& src);
   virtual ossimRefPtr<ossimVisitor> dup()const;
   virtual void visit(ossimObject* obj);
   void setTypeName(const ossimString& typeName);
   const ossimString& getTypeName()const;

   /**
    * @brief Set the m_firstOfTypeFlag.
    *
    * @param flag If flag is true, ossimTypeNameVisitor::visit will stop traversing if it
    * finds an object of m_typeName.
    */
   void setFirstOfTypeFlag(bool flag);

   /** @return m_firstOfTypeFlag */
   bool getFirstOfTypeFlag() const;
   
protected:
   ossimString m_typeName;
   bool        m_firstOfTypeFlag;
};

class OSSIM_DLL ossimTypeIdVisitor : public ossimCollectionVisitor
{
public:
   ossimTypeIdVisitor(int visitorType =(VISIT_INPUTS|VISIT_CHILDREN));
   ossimTypeIdVisitor(const RTTItypeid& typeId, bool firstofTypeFlag=false, int visitorType =(VISIT_INPUTS|VISIT_CHILDREN));
   ossimTypeIdVisitor(const ossimTypeIdVisitor& src);
   virtual ossimRefPtr<ossimVisitor> dup()const;
   virtual void visit(ossimObject* obj);
   void setTypeId(const RTTItypeid& typeId);
   const RTTItypeid& getTypeId()const;
protected:
   RTTItypeid m_typeId;
   bool        m_firstOfTypeFlag;
};

class OSSIM_DLL ossimEventVisitor : public ossimVisitor
{
public:
   ossimEventVisitor(ossimEvent* evt,
                      int visitorType =(VISIT_OUTPUTS|VISIT_CHILDREN));
   ossimEventVisitor(const ossimEventVisitor& src);
   virtual ossimRefPtr<ossimVisitor> dup()const;
   virtual void visit(ossimObject* obj);
   
protected:
   ossimRefPtr<ossimEvent> m_event;
  
};

/**
 * @class ossimViewInterfaceVisitor
 *
 * Visitor to update view and then propagate property event to outputs.
 */
class OSSIM_DLL ossimViewInterfaceVisitor : public ossimVisitor
{
public:
   ossimViewInterfaceVisitor(ossimObject* view,
                             int visitorType =(VISIT_OUTPUTS|VISIT_CHILDREN));
   ossimViewInterfaceVisitor(const ossimViewInterfaceVisitor& src);
   virtual ossimRefPtr<ossimVisitor> dup()const;

   /**
    * @brief Calls ossimViewInterface::setView if obj cast to
    * ossimViewInterface*.  On success a property event is sent to
    * output side so that things like ossimCombiner can reinitialize if
    * needed.
    *
    * @param obj Object to visit.
    */
   virtual void visit(ossimObject* obj);
   
protected:
   ossimRefPtr<ossimObject> m_view;
};

#endif
