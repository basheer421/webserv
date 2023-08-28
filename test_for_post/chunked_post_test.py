import http.client

# Replace these with your server details
server_host = "localhost"
server_port = 8080

# Sample data to send in chunks
data = b"This is fdhghfdghfdhfdhfdhfdhfchunk 1\r\n" \
       b"This is dghfdhfghfdghfdghdfghdfhfghchunk 2\r\n" \
       b"This is dfjdfjdfjfdjfdhjfdjfdjfdjfdjdfhjchunk 3\r\n" \
       b""

# Create a connection to the server
conn = http.client.HTTPConnection(server_host, server_port)

# Send the initial headers with Transfer-Encoding set to chunked
conn.putrequest("POST", "/", skip_host=True)
conn.putheader("Transfer-Encoding", "chunked")
conn.endheaders()

# Send chunks of data
chunks = data.split(b"\r\n")
for chunk in chunks:
    chunk_size = bytes(hex(len(chunk))[2:], "utf-8")
    conn.send(chunk_size + b"\r\n" + chunk + b"\r\n")

# Send the final chunk
conn.send(b"0\r\n\r\n")

# Get and print the response
response = conn.getresponse()
print(response.read().decode("utf-8"))

# Close the connection
conn.close()
