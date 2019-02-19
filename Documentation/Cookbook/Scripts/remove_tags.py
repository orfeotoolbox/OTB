import re
import sys

if __name__ == "__main__":
    regexes = [
            r" *\/\/ +Software Guide : BeginLatex\n *\/\/\n",
            r" *\/\/\n *\/\/ +Software Guide : EndLatex\n",

            r" *\/\/ +Software Guide : BeginLatex\n",
            r" *\/\/ +Software Guide : EndLatex\n",
            r" *\/\/ +Software Guide : BeginCodeSnippet\n",
            r" *\/\/ +Software Guide : EndCodeSnippet\n",
              ]

    for filename in sys.argv[1:]:
        for regex in regexes:
            with open(filename) as f:
                content = f.read()
            newcontent = re.sub(regex, "", content, flags = re.DOTALL | re.MULTILINE)
            with open(filename, "w") as f:
                f.write(newcontent)
