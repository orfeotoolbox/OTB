//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Hicks
//
// Description: Helper interface class for ossimAdjustmentExecutive
//              and ossimWLSBundleSolution.
//----------------------------------------------------------------------------

#include <ossim/base/ossimAdjSolutionAttributes.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotify.h>

static ossimTrace traceDebug(ossimString("ossimAdjSolutionAttributes:debug"));
static ossimTrace traceExec(ossimString("ossimAdjSolutionAttributes:exec"));


//*****************************************************************************
// METHOD:      ossimAdjSolutionAttributes::ossimAdjSolutionAttributes
//
// DESCRIPTION: Default constructor
//
// PARAMETERS:  N/A
//
// RETURN:      N/A
//*****************************************************************************
ossimAdjSolutionAttributes::ossimAdjSolutionAttributes(
	const int& numObjObs, const int& numImages, const int& numMeas, const int& rankN)
	:
	theNumObjObs(numObjObs),
	theNumImages(numImages),
	theFullRank(rankN),
   theNumMeasurements(numMeas)
{
   theTotalCorrections.ReSize(rankN,1);
   theLastCorrections.ReSize(rankN,1);
   theTotalCorrections = 0.0;
   theLastCorrections = 0.0;
}


//*****************************************************************************
// METHOD:      ossimAdjSolutionAttributes::~ossimAdjSolutionAttributes
//
// DESCRIPTION: Destructor
//
// PARAMETERS:  N/A
//
// RETURN:      N/A
//*****************************************************************************
ossimAdjSolutionAttributes::~ossimAdjSolutionAttributes()
{
	if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG)
		<< "DEBUG: ~ossimAdjSolutionAttributes(): returning..." << std::endl;
}


//*****************************************************************************
// METHOD:      operator <<
//
// DESCRIPTION: Output point parameters.
//
// PARAMETERS:  N/A
//
// RETURN:      N/A
//*****************************************************************************
std::ostream & operator << (std::ostream &output, ossimAdjSolutionAttributes& /* data */)
{

	 output << "ossimAdjSolutionAttributes..." << std::endl;

	 return output;
}
