#!/usr/bin/env python3

import warnings

warnings.filterwarnings("ignore", category=DeprecationWarning)

import cgi
import sys

PAGE_TEMPLATE = """\
<!DOCTYPE HTML>
<html lang="en">
    <head>
        <meta charset="utf-8">
        <title>Password Generator</title>
    </head>
    <body>
        <h1>Password Generator</h1>
        <p>Your password: {}</p>
    </body>
</html>
"""


def main():
    form = cgi.FieldStorage()
    print("Comprimento:", len(form.keys()), file=sys.stderr)
    print("Form:", form.keys(), file=sys.stderr)
    content = form["file"]
    print("Name:", content.filename, file=sys.stderr)


if __name__ == "__main__":
    main()
    print(PAGE_TEMPLATE.format("blablabla"))
