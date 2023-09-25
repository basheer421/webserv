#!/usr/bin/env python

import cgi, cgitb 
# Create an instance of FieldStorage
form = cgi.FieldStorage()
Your_name = form.getvalue('name1')
# getting the data from fields
print ("<html>")
print ("<head>")
print ("<title>First CGI Script</title>")
print ("</head>")
print ("<body>")
print ("<h2>Hi, %s</h2>"
% (Your_name))


print ("</body>")
print ("</html>")
