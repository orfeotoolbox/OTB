import argparse
import re
import os
import os.path
from os.path import join
import glob

if __name__ == "__main__":
    parser = argparse.ArgumentParser(usage="Export examples to rst")
    parser.add_argument("otb_root", help="OTB repository root")
    args = parser.parse_args()

    rx_block = r"\/\/  Software Guide : BeginCommandLineArgs\n(.*?)\/\/  Software Guide : EndCommandLineArgs"

    list_of_examples = glob.glob(join(args.otb_root, "Examples/*/*.cxx"))
    print(len(list_of_examples))

    for filename in list_of_examples:
        content = open(filename).read()
        newcontent = content
        name = os.path.splitext(os.path.basename(filename))[0]

        for match in re.finditer(rx_block, content, re.MULTILINE | re.DOTALL):
            print(match.group(0))
            print()

            cmd_args = []
            for line in match.group(1).split("\n"):
                if "INPUT" in line:
                    for match in re.finditer("{(.*?)}", line):
                        cmd_args.append("Input/" + match.group(1))
                elif "OUTPUT" in line:
                    for match in re.finditer("{(.*?)}", line):
                        cmd_args.append("Output/" + match.group(1))
                elif re.match("// *\n", line):
                    pass
                else:
                    cmd_args.extend(line.replace("//", "").split())

            cmd_line = "./{} {}".format(name, " ".join(cmd_args))

            if len(cmd_line) > 160:
                joiner = " \\\n" + " "*(3+len(name)) # backslash and indentation
                cmd_line = "./{} {}".format(name, joiner.join(cmd_args))

            block = ("/* Example usage:\n"
                     "{}\n"
                     "*/\n").format(cmd_line)

            print(block)

            newcontent = re.sub(rx_block, block, newcontent, flags = re.MULTILINE | re.DOTALL, count=1)

        open(filename, "w").write(newcontent)


