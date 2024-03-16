#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import warnings

warnings.filterwarnings("ignore", category=DeprecationWarning)

import os
import cgi
import datetime
from string import Template


UPLOAD_DIR = os.environ.get("UPLOAD_DIR", "")

ITEM_TEMPLATE = """\
<tr>
  <td>{filename}</td>
  <td>{file_size} KB</td>
  <td>{modified}</td>
  <td>
    <div class="buttons">
      <a class="button is-primary is-small" href="{upload_dir}/{filename}">Ver</a>
      <button class="button is-danger is-small" onclick="deleteFile('{upload_dir}/{filename}')">Apagar</button>
    </div>
  </td>
</tr>
"""


def main():
    root = os.environ.get("DOCUMENT_ROOT", "")
    if not os.path.isdir(root):
        os.makedirs(root)

    table = ""
    file_list = list_files(root)
    for filename, file_size, modified in file_list:
        table += make_item(filename, file_size, modified)

    html = load_page(os.path.join(root, "../index.html"))
    page = Template(html)
    print(page.substitute(FILE_LISTING=table), end="")


def make_item(filename, file_size, modified):
    """Generate HTML markup for an item in the file listing."""
    return ITEM_TEMPLATE.format(
        filename=filename,
        file_size=file_size,
        modified=modified,
        upload_dir=UPLOAD_DIR,
    )


def list_files(directory):
    """
    List files in the specified directory and return information about them.
    """
    files = []
    for filename in os.listdir(directory):
        filepath = os.path.join(directory, filename)
        if os.path.isfile(filepath):
            file_size = os.path.getsize(filepath) / 1024
            file_size = round(file_size, 2)
            modified = os.path.getmtime(filepath)
            modified = datetime.datetime.fromtimestamp(modified).strftime(
                "%Y-%m-%d %H:%M:%S"
            )
            files.append((filename, file_size, modified))
    return files


def load_page(filename):
    """Load the contents of an HTML file."""
    with open(filename, "r") as file:
        contents = file.read()
    return contents


if __name__ == "__main__":
    try:
        main()
    except:
        cgi.print_exception()
