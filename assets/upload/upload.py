#!/usr/bin/env python3

import warnings

warnings.filterwarnings("ignore", category=DeprecationWarning)

import os
import cgi

PAGE_TEMPLATE = """\
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Sucesso no Upload</title>
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/bulma/0.9.4/css/bulma.min.css">
</head>
<body>
  <section class="section">
    <div class="container">
      <h1 class="title">Upload Concluído</h1>
      <p class="subtitle">{} foi enviado com sucesso!</p>
      <a class="button is-primary" href="/">Voltar</a>
    </div>
  </section>
</body>
</html>
"""

def main():
    form = cgi.FieldStorage()
    if "fileToUpload" in form:
        content = form["fileToUpload"]
        file_content = form["fileToUpload"].file.read()
        file_path = os.environ["DOCUMENT_ROOT"] + "/" + content.filename

        directory = os.path.dirname(file_path)
        if not os.path.exists(directory):
            os.makedirs(directory)

        with open(file_path, "wb") as f:
            f.write(file_content)
        print(PAGE_TEMPLATE.format(content.filename))


if __name__ == "__main__":
    try:
        main()
    except:
        cgi.print_exception()
