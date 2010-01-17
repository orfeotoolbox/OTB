#ifndef ErsSarDataFactory_h
#define ErsSarDataFactory_h

#include <ossim/imaging/ErsSar/ErsSarRecordFactory.h>
#include <map>
/**
 * @ingroup DataFile
 * @brief This class is a facory able to construct Record base classes
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 03-12-07
 */
class ErsSarDataFactory : public ErsSarRecordFactory
{
public:
	/**
	 * @brief Contstructor
	 */
	ErsSarDataFactory();
	/**
	 * @brief Destructor
	 */
	~ErsSarDataFactory();

protected:
private:
};

#endif