%pythoncode {
import sys

class ApplicationProxy(object):
  def __init__(self, application, groupkey, value = None):
    self.__dict__["application"] = application
    self.__dict__["groupkey"] = groupkey
    if value is not None:
      self.__dict__["application"].SetParameterString(groupkey, value)

  def __str__(self):
    return self.__dict__["application"].GetParameterAsString(self.groupkey)

  def __eq__(self, other):
    if not type(other) == type(self):
      return (self.__str__() == other)
    else:
      return (isinstance(other, self.__class__) and self.__dict__ == other.__dict__)

  def __ne__(self, other):
    return not self.__eq__(other)

  def __getattr__(self,attr):
    return self.__dict__["application"].GetParameterValue( self.groupkey + "." + attr.lower() )

  def __setattr__(self,attr,value):
    if attr not in self.__dict__:
      return self.__dict__["application"].SetParameterValue( self.groupkey + "." + attr.lower(), value )
    else:
      return dict.__setattr__(self, attr, value)

}