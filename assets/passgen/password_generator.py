#!/usr/bin/env python3

import os
import string
import random
from urllib.parse import parse_qs

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
    password = generate_password(os.environ['QUERY_STRING'])
    print(PAGE_TEMPLATE.format(password))


def has_param(params, param):
    return params.get(param, ["false"]) == ["true"]


def generate_password(query):
    """
    Generate a password based on the given query string.

    :param query: The query string containing password parameters.
    :return: A randomly generated password.
    """
    params = parse_qs(query)
    length = int(params.get("length", [8])[0])
    use_special_chars = has_param(params, 'special_chars')
    use_digits = has_param(params, 'digits')
    use_uppercase = has_param(params, 'uppercase')

    characters = string.ascii_lowercase
    if use_special_chars:
        characters += string.punctuation
    if use_digits:
        characters += string.digits
    if use_uppercase:
        characters += string.ascii_uppercase

    password = "".join(random.choice(characters) for _ in range(length))
    return password


if __name__ == "__main__":
    main()
