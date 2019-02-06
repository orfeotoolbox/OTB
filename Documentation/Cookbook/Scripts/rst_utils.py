
def rst_section(text, delimiter, ref=None):
    "Make a rst section title"

    output = ""

    if ref is not None:
        output += ".. _" + ref + ":\n\n"

    output += text + "\n" + delimiter * len(text) + "\n\n"
    return output

