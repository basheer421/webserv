#!/usr/bin/python3

import sys
import os
import cgi
import time

print("Content-type: text/html\r\n\r")
print("<html><body>")
print("<h1>Hello World!</h1>")
print("</body></html>")

# for name, value in os.environ.items():
#     print("{0}: {1}".format(name, value), file=sys.stderr)


form = cgi.FieldStorage() # instantiate only once!
name = form.getfirst('num', '1')
print(name, file=sys.stderr)
time.sleep(20)