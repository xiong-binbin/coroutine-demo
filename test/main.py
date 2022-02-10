import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

sock.connect(('127.0.0.1', 1234))

send_data = 'hi'
sock.send(send_data.encode('utf-8'))

recv_data = sock.recv(1024)
print('recv_data:', recv_data.decode('utf-8'))

sock.close()
