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