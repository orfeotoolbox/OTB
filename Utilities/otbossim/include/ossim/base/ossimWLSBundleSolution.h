//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Hicks
// test
//
// Description: Weighted least squares bundle adjustment solution.
//----------------------------------------------------------------------------
#ifndef ossimWLSBundleSolution_HEADER
#define ossimWLSBundleSolution_HEADER

#include <ossim/matrix/newmat.h>
#include <ossim/matrix/newmatap.h>
#include <ossim/matrix/newmatio.h>
#include <vector>

class ossimAdjSolutionAttributes;


class OSSIM_DLL ossimWLSBundleSolution
{
public:

   /**
    * @brief Constructor
    */
   ossimWLSBundleSolution();
   
   /**
    * @brief Run solution
    */
   bool run(ossimAdjSolutionAttributes* solAttributes);
   
   /**
    * @brief Destructor
    */
   ~ossimWLSBundleSolution();


protected:
   bool theSolValid;

   // Internal solution methods
   bool solveSystem(double* d, double* c, double* delta, int jb);
   bool recurFwd(double* d, double* c, std::vector<double>& rc, std::vector<int>& nz, int jb);
   bool recurBack(double* d, int jb);
   void trimv(double* pc, double* h, int pcIndex, int hIndex, int mr, std::vector<double>& sum);
   void moveAndNegate(std::vector<double>& from, double* to, int indexFrom, int indexTo, int nElements);

};

#endif // #ifndef ossimWLSBundleSolution_HEADER
