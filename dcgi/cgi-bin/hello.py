#!/usr/bin/env python

import os
import cgi

# Print the content type header
print("Content-Type: text/html\n")

# Print all environment variables and their values
for key, value in os.environ.items():
	if (key == " NAME"):
		print("[{}]: {}<br>".format(key, value))
	















#!/usr/bin/env python



# import os
# import cgi
# import sys

# # Print the content type header
# print("Content-Type: text/html\n")

# # Retrieve the form data from the POST request
# form = cgi.FieldStorage()

# # Check if the 'name' field is in the form data
# if 'name' in form:
#     name = form['name'].value
#     print("Hello, {}!".format(name))
# else:
#     print("No name provided.")

# for name, value in os.environ.items():
#     print("{0}: {1}".format(name, value), file=sys.stderr)

# #!/usr/bin/python3

# import cgi
# import time


# print("Content-type: text/html\r\n\r")
# print("<html><body>")
# print("<h1>Hello World!</h1>")
# print("</body></html>")



# form = cgi.FieldStorage() # instantiate only once!
# if "name" not in form:
# 	print("<H1>Error<H1>")
# 	return
# print("<p>name:", form["name"].value)
# name = form.getfirst('name', '1')
# print(name, file=sys.stderr)
# time.sleep(2)


