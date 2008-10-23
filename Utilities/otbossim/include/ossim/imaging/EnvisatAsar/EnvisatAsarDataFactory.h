#ifndef EnvisatAsarDataFactory_h
#define EnvisatAsarDataFactory_h


#include <ossim/imaging/EnvisatAsar/EnvisatAsarRecordFactory.h>
#include <map>
/**
 * @ingroup EnvisatAsarDataFile
 * @brief This class is a factory able to construct Record base classes
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 23-11-07
 */
class EnvisatAsarDataFactory : public EnvisatAsarRecordFactory
{
public:
	/**
	 * @brief Constructor
	 */
	EnvisatAsarDataFactory();
	/**
	 * @brief Destructor
	 */
	~EnvisatAsarDataFactory();

	
protected:


private:
};

#endif