import cgi
import random
import string

import os, sys


def generate_password(length, use_special_chars, use_digits, use_uppercase):
    characters = string.ascii_lowercase
    if use_special_chars:
        characters += string.punctuation
    if use_digits:
        characters += string.digits
    if use_uppercase:
        characters += string.ascii_uppercase

    password = ''.join(random.choice(characters) for i in range(length))
    return password

# Processamento de dados do formulário CGI
form = cgi.FieldStorage()
length = int(form.getvalue('length', 8))  # Valor padrão é 8 se não especificado
use_special_chars = form.getvalue('special_chars', 'n').lower() == 's'
use_digits = form.getvalue('digits', 'n').lower() == 's'
use_uppercase = form.getvalue('uppercase', 'n').lower() == 's'

generated_password = generate_password(length, use_special_chars, use_digits, use_uppercase)

generated_password = generate_password(10, 's','s', 's')


print("Content-type: text/html\n")
print("<html><body>")
print("<p>Senha gerada: {}</p>".format(generated_password))
print("</body></html>")
