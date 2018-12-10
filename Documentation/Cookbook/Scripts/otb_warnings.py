import sys

def parameter_warnings(app_warn, app, key):

    def warn(message):
        app_warn("Parameter '{}' ".format(key) + message)

    name = app.GetParameterName(key)
    description = app.GetParameterDescription(key)

    if name[-1] == " ":
        warn("name ends with a space")

    if ":" in name:
        warn("name contains a special character (:)")

    if "." in name:
        warn("name contains a special character (.)")

    #if description == "":
        #warn("missing description")

    #if len(description) > 0 and description[-1] != ".":
        #warn("description does not end with a period")

    if len(description) > 0 and " :" in description:
        warn("description has a space before a colon")

def application_documentation_warnings(app):
    "Emit warnings about application documentation"

    def warn(message):
        print("OTB Documentation Warning ({}): {}".format(app.GetName(), message), file=sys.stderr)

    description = app.GetDescription()
    longdescription = app.GetDocLongDescription()

    if not longdescription[-1] == ".":
        warn("Application Long Description does not end with a period (.)")

    if " :" in longdescription:
        warn("Application Long Description has a space before a colon")

    if app.GetNumberOfExamples() == 0:
        warn("Application has no examples")

    keys = app.GetParametersKeys()
    for key in app.GetParametersKeys():
        parameter_warnings(warn, app, key)

    if "ram" in keys and not keys[-3] == "ram":
        warn("'ram' parameter is not third from last")

    if "inxml" in keys and not keys[-2] == "inxml":
        warn("'inxml' parameter is not second from last parameter")

    if "outxml" in keys and not keys[-1] == "outxml":
        warn("'outxml' is not last parameter")

