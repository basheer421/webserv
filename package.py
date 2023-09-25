#!/usr/bin/python3
import socket
import time
import sys



def send(packet, target_host, target_port):
    try:
        # Create a socket object
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        # Connect to the server
        client_socket.connect((target_host, target_port))

        # Send the packet
        c = 0
        for pack in packet:
            client_socket.sendall(pack.encode())
            c += len(pack)
        print(c)
        # Receive the response (you can do something with the response if needed)
        while(1):
            response = client_socket.recv(20000)
            print("Response received:")
            print(response.decode())
            if len(response) == 0:
                exit(0)
        # response = client_socket.recv(20000)
        # print("Response received:")
        # print(response.decode())
        # response = client_socket.recv(20000)
        # print("Response received:")
        # print(response.decode())

    except Exception as e:
        print(f"Error occurred: {e}")

    finally:
        # Close the socket
        client_socket.close()

def send_packet(path):
    
    packet = f"""GET {path} HTTP/1.1\r
Host: 127.0.0.1:8080\r
User-Agent: curl/7.87.0\r
Content-Type: application/x-www-form-urlencoded\r
Content-Length: 0\r
Accept: */*\r\n\r\ncomment=Ahmed\0Mahdy\0\0\0\0\0hadi"""
    target_host = "127.0.0.1"
    target_port = 8080

    send(packet, target_host, target_port)
        
def send_chunked_packet(path, comment):
    
    str = f"""POST {path} HTTP/1.1\r
Host: 127.0.0.1:8080\r
User-Agent: curl/7.87.0\r
Accept: */*\r
Content-Type: application/x-www-form-urlencoded\r
Transfer-Encoding: chunked\r\n\r\n"""
    
    length = hex(len("comment=" + comment))
    packet = [str ,
    f"""{length}\r\n""", f"""comment={comment}\r\n""", """0\r\n\r\n""" ]
    target_host = "127.0.0.1"
    target_port = 8080
    
    send(packet, target_host, target_port)

def send_cgi_packet(path, num):
    
    str = f"""POST {path} HTTP/1.1\r
Host: 127.0.0.1:8080\r
User-Agent: curl/7.87.0\r
Accept: */*\r
Content-Type: application/x-www-form-urlencoded\r
Transfer-Encoding: chunked\r\n\r\n"""
    
    print(num)
    packet = [str]
    for i in range(1, int(num)):
        length = hex(len(f"""{i} """))
        packet += [f"""{length}\r\n""", f"""{i} \r\n"""]
    packet += ["0\r\n\r\n"]
    target_host = "127.0.0.1"
    target_port = 8080
    # for i in packet:
    #     print(i)
    # exit()
    send(packet, target_host, target_port)
# Call the function to send the packet

# send_chunked_packet("/", sys.argv[1]);
if(len(sys.argv) == 1):
    send_packet("/cgi-bin/minicgi.sh")
elif(len(sys.argv) == 2):
    send_chunked_packet("/", sys.argv[1])
else:
    send_cgi_packet("/directory/youpi.bla", sys.argv[2])
# send_packet("/hello");
# send_packet("/attacks/out");