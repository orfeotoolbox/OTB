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