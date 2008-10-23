#include "otbConfigure.h"

#define OTB_WRAP_GROUP(x) #x

#define OTB_WRAP_PACKAGE_NAME(p) p

// Wrap an otb object, the wrap name is otbname, 
// this is for non-templated otb objects, so
// OTB_WRAP_OBJECT(Object) would wrap otb::Object to the wrapped name otbObject
#define OTB_WRAP_OBJECT(name) \
typedef otb::name::name otb##name; \
typedef otb::name::Pointer::SmartPointer otb##name##_Pointer

// define the template class wrapper macros

// Wrap an otb object with one template parameter 
// The wrapname is the name that will be used and usually
// encodes the template parameters, i.e. otb::Image<float, 2> would
// otbImageF2

#define OTB_WRAP_OBJECT1(name, arg1, wrapname) \
typedef otb::name<arg1 >::name wrapname; \
typedef otb::name<arg1 >::Pointer::SmartPointer wrapname##_Pointer 

// same as OTB_WRAP_OBJECT1 but also wraps the super class 
#define OTB_WRAP_OBJECT1_WITH_SUPERCLASS(name, arg1, wrapname) \
OTB_WRAP_OBJECT1(name, arg1, wrapname); \
typedef otb::name<arg1 >::Superclass::Self wrapname##_Superclass; \
typedef otb::name<arg1 >::Superclass::Pointer::SmartPointer wrapname##_Superclass_Pointer

// same as OTB_WRAP_OBJECT1 but for two template parameters
#define OTB_WRAP_OBJECT2(name, arg1, arg2, wrapname) \
typedef otb::name<arg1, arg2 >::name wrapname; \
typedef otb::name<arg1, arg2 >::Pointer::SmartPointer wrapname##_Pointer

#define OTB_WRAP_OBJECT2_WITH_SUPERCLASS(name, arg1, arg2, wrapname) \
OTB_WRAP_OBJECT2(name, arg1, arg2, wrapname); \
typedef otb::name<arg1,arg2 >::Superclass::Self wrapname##_Superclass; \
typedef otb::name<arg1,arg2 >::Superclass::Pointer::SmartPointer wrapname##_Superclass_Pointer

// same as OTB_WRAP_OBJECT2 but for three template parameters
#define OTB_WRAP_OBJECT3(name, arg1, arg2, arg3, wrapname) \
typedef otb::name<arg1, arg2, arg3 >::name wrapname; \
typedef otb::name<arg1, arg2, arg3 >::Pointer::SmartPointer wrapname##_Pointer

#define OTB_WRAP_OBJECT3_WITH_SUPERCLASS(name, arg1, arg2, arg3, wrapname) \
OTB_WRAP_OBJECT3(name, arg1, arg2, arg3, wrapname); \
typedef otb::name<arg1,arg2, arg3 >::Superclass::Self wrapname##_Superclass; \
typedef otb::name<arg1,arg2, arg3 >::Superclass::Pointer::SmartPointer wrapname##_Superclass_Pointer

// same as OTB_WRAP_OBJECT4 but for three template parameters
#define OTB_WRAP_OBJECT4(name, arg1, arg2, arg3, arg4, wrapname) \
typedef otb::name<arg1, arg2, arg3, arg4 >::name wrapname; \
typedef otb::name<arg1, arg2, arg3, arg4 >::Pointer::SmartPointer wrapname##_Pointer

#define OTB_WRAP_OBJECT4_WITH_SUPERCLASS(name, arg1, arg2, arg3, arg4, wrapname) \
OTB_WRAP_OBJECT4(name, arg1, arg2, arg3, arg4, wrapname); \
typedef otb::name<arg1,arg2, arg3, arg4 >::Superclass::Self wrapname##_Superclass; \
typedef otb::name<arg1,arg2, arg3, arg4 >::Superclass::Pointer::SmartPointer wrapname##_Superclass_Pointer

// Added a new macro.
#define OTB_WRAP_OBJECT_WITHOUT_SPOINTER(name) \
typedef otb::name::name otb##name

// Added a new macro.
#define ITK_WRAP_OBJECT_WITHOUT_SPOINTER(name) \
typedef itk::name::name otb##name

