
# Regex used to find examples usage
examples_usage_regex = r"\/\* Example usage:\n(\.\/\w+ (.*?))\*\/"

def rst_section(text, delimiter, ref=None):
    "Make a rst section title"

    output = ""

    if ref is not None:
        output += ".. _" + ref + ":\n\n"

    output += text + "\n" + delimiter * len(text) + "\n\n"
    return output

def RstPageHeading(text, maxdepth, ref=None):
    output = rst_section(text, "=", ref=ref)
    output += ".. toctree::\n"
    output += "\t:maxdepth: {}\n\n\n".format(maxdepth)
    return output

