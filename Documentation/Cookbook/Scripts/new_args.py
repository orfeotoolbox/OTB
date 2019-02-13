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

        match = re.search(rx_block, content, re.MULTILINE | re.DOTALL)

        name = os.path.splitext(os.path.basename(filename))[0]

        if match:
            print(match.group(0))
            print()
            str = match.group(1)
            str = str.replace("//    ", "")
            str = str.replace("//\n", "")
            str = str.replace("INPUTS: ", "")
            str = str.replace("OUTPUTS: ", "")
            str = str.replace("{", "")
            str = str.replace("}", "")
            str = str.replace(",", "")
            str = str.replace("\n", " ")
            str = str.strip()

            block = ("/* Example usage:\n"
                     "./{} {}\n"
                     "*/\n".format(name, str))

            print(block)

            newcontent = re.sub(rx_block, block, content, flags = re.MULTILINE | re.DOTALL)

            #open(filename, "w").write(newcontent)
            #print(newcontent)


