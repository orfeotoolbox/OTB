#ifndef bool_globals_HEADER
#define bool_globals_HEADER

#include <limits> /* for numeric_limits::min/max */
#include <string>
#include <ossim/base/ossimConstants.h>

#ifdef OSSIMMAKINGDLL
#define A2DKBOOLMAKINGDLL
#endif

#define A2DKBOOLDLLEXP OSSIM_DLL

#ifdef A2DKBOOLMAKINGDLL
#define A2DKBOOLDLLEXP_DATA(type) WXEXPORT type
#define A2DKBOOLDLLEXP_CTORFN
#else // not making nor using DLL
#define A2DKBOOLDLLEXP_DATA(type) type
#define A2DKBOOLDLLEXP_CTORFN
#endif

#define KBOOL_VERSION "1.8"

#define KBOOL_DEBUG 0
#define KBOOL_LOG 0

#if OSSIM_SIZEOF_LONG_INT == 8
#  define KBOOL_INT64 1
#elif OSSIM_SIZEOF_LONG_LONG == 8
#  define KBOOL_INT64 1
#else
#  define KBOOL_INT64 0
#endif

#define LINELENGTH 200

#ifdef  MAXDOUBLE
#undef  MAXDOUBLE
#endif
#define MAXDOUBLE 1.7976931348623158e+308

//---
// .NET 2005 fix...
// std::numeric_limits<>::max() and std::numeric_limits<>::min() will not work
// if max and min are defined as macros, undefine them if they are defined.
//---
#ifdef max
#  undef max
#endif

#ifdef min
#  undef min
#endif

#ifdef KBOOL_INT64
typedef ossim_int64 B_INT;		   // 8 bytes integer
const B_INT MAXB_INT = std::numeric_limits<ossim_int64>::max();
const B_INT MINB_INT = std::numeric_limits<ossim_int64>::min();
#else /* not defined KBOOL_INT64 */
typedef ossim_int32 B_INT;		   // 8 bytes integer
const B_INT MAXB_INT = std::numeric_limits<ossim_int32>::max();
const B_INT MINB_INT = std::numeric_limits<ossim_int32>::min();
#endif /* end of ifdef KBOOL_INT64 */

B_INT babs(B_INT);

#ifdef  M_PI
#undef  M_PI
#endif
#define M_PI		(3.1415926535897932384626433832795028841972)

#ifdef  M_PI_2
#undef  M_PI_2
#endif
#define M_PI_2      1.57079632679489661923

#ifdef  M_PI_4
#undef  M_PI_4
#endif
#define M_PI_4      0.785398163397448309616

#ifndef NULL
#define NULL 0
#endif

//B_INT bmin(B_INT const value1, B_INT const value2);
//B_INT bmax(B_INT const value1, B_INT const value2);

B_INT bmin(B_INT value1, B_INT value2);
B_INT bmax(B_INT value1, B_INT value2);

#include <string.h>



#define KBOOL_LOGFILE "kbool.log"

enum kbEdgeType 
{ 
   KB_OUTSIDE_EDGE, /*!< edge of the outside contour of a polygon */
   KB_INSIDE_EDGE,  /*!< edge of the inside hole a polygon */
   KB_FALSE_EDGE    /*!< edge to connect holes into polygons */
} ;

enum GroupType 
{ 
   GROUP_A, /*!< to set Group A for polygons */
   GROUP_B  /*!< to set Group A for polygons */
};

enum BOOL_OP 
{ 
   BOOL_NON, /*!< No operation */
   BOOL_OR, /*!< boolean OR operation */
   BOOL_AND, /*!< boolean AND operation */
   BOOL_EXOR, /*!< boolean EX_OR operation */
   BOOL_A_SUB_B, /*!< boolean Group A - Group B operation */
   BOOL_B_SUB_A, /*!< boolean Group B - Group A operation */
   BOOL_CORRECTION, /*!< polygon correction/offset operation */
   BOOL_SMOOTHEN, /*!< smooth operation */
   BOOL_MAKERING /*!< create a ring on all polygons */
};

//! errors in the boolean algorithm will be thrown using this class
class A2DKBOOLDLLEXP Bool_Engine_Error
{
	public:
		Bool_Engine_Error(const char* message, const char* header=0, int degree = 9, int fatal = 0);
		Bool_Engine_Error(const Bool_Engine_Error& a);
		~Bool_Engine_Error();
		const char* GetErrorMessage();
		const char* GetHeaderMessage();
		int	GetErrorDegree();
		int	GetFatal();

	protected:
                std::string _message;
                std::string _header;
		int _degree;
		int _fatal;
};

#endif
