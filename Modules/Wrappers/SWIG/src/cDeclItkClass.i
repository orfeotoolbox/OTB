class Registry : public itkObject
{
public:

  static std::vector<std::string> GetAvailableApplications();
  static Application_Pointer CreateApplication(const std::string& name);
  static void AddApplicationPath(std::string newpath);
  static void SetApplicationPath(std::string newpath);

protected:
  Registry();
  virtual ~Registry();
};

class AddProcessToWatchEvent : public itkEventObject
{
public:
  AddProcessToWatchEvent();
  ~AddProcessToWatchEvent();
  virtual char const * GetEventName() const;
  virtual bool CheckEvent(itkEventObject const * e) const;
  virtual itkEventObject * MakeObject() const;
  AddProcessToWatchEvent(AddProcessToWatchEvent const & s);

  itkProcessObject * GetProcess() const;
  void SetProcess(itkProcessObject * process);
  void SetProcessDescription(const std::string desc);
  std::string GetProcessDescription() const;
};