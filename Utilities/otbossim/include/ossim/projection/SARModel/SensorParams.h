#ifndef SensorParams_h
#define SensorParams_h

/**
 * @ingroup SARModel
 * @brief This class handles the sensor parameters
 * @author Magellium, Pacome Dentraygues
 * @version 1.0
 * @date 15-01-08
 */
 
 class SensorParams
 {
	public:
	 
	enum SightDirection
	{
		Left,
		Right
	};
	
	/**
	 * @brief Constructor
	 */
	SensorParams();
	/**
	 * @brief Destructor
	 */
	~SensorParams();
	
	/** 
	 *@brief Copy constructor
	 */
	SensorParams(const SensorParams& rhs);
	
	/**
	 * @brief Affectation operator
	 */
	SensorParams& operator=(const SensorParams& rhs);
	
	double get_prf()
	{
		return _prf;
	};
	
	double get_sf()
	{
		return _sf;
	};
	
	double get_rwl()
	{
		return _rwl;
	};
	
	void set_prf(double prf)
	{
		_prf = prf;
	};
	
	void set_sf(double sf)
	{
		_sf = sf;
	};
	
	void set_rwl(double rwl)
	{
		_rwl = rwl;
	};

	SightDirection get_sightDirection()
	{
		return _sightDirection;
	};

	void set_sightDirection(SightDirection sight)
	{
		_sightDirection = sight;
	};
	
	virtual SensorParams* Clone()
	{
		return new SensorParams(*this);
	};

	int get_col_direction()
	{
		return _col_direction;
	};

	int get_lin_direction()
	{
		return _lin_direction;
	};

	double get_nAzimuthLook()
	{
		return _nAzimuthLook ;
	};

	double get_nRangeLook()
	{
		return _nRangeLook ;
	};

	void set_col_direction(int dir)
	{
		_col_direction = dir;
	};

	void set_lin_direction(int dir)
	{
		_lin_direction = dir;
	};

	void set_nAzimuthLook(double look)
	{
		_nAzimuthLook = look;
	};

	void set_nRangeLook(double look)
	{
		_nRangeLook = look;
	};

	double get_semiMajorAxis()
	{
		return _semiMajorAxis ;
	};

	double get_semiMinorAxis()
	{
		return _semiMinorAxis ;
	};

	void set_semiMajorAxis(double value)
	{
		_semiMajorAxis = value;
	};

	void set_semiMinorAxis(double value)
	{
		_semiMinorAxis = value;
	};

 protected:
	/**
	 * @brief Nominal PRF, Hz
	 */
	double _prf;
	
	/**
	 * @brief Sampling frequency
	 */
	double _sf;
	
	/**
	 * @brief Radar wave length
	 */
	double _rwl;

	/**
	 * @brief Columns direction (1=increasing, -1=decreasing)
	 */
	int _col_direction;

	/**
	 * @brief Lines direction (1=increasing, -1=decreasing)
	 */
	int _lin_direction;

	/**
	 * @brief Antenna pointing direction
	 */
	SightDirection _sightDirection;

	/**
	 * @brief Ellipsoid semi_major axis, m
	 *				Default : WGS84
	 */
	double _semiMajorAxis;

	/**
	 * @brief Ellipsoid semi_minor axis, m
	 *				Default : WGS84
	 */
	double _semiMinorAxis;

	/**
	 * @brief Number of azimuth looks
	 */
	double _nAzimuthLook ;

	/**
	 * @brief Number of range looks
	 */
	double _nRangeLook ;

 private:
 };
 
#endif