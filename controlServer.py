import socket ## The address is roboRIO-6341-FRC:5801

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(("roboRIO-6341-FRC.local", 5801))

initialBroadcast = client.recv(5)

while 1:
    xm = int(input("Xmov> "))
    ym = int(input("Ymov> "))
    client.sendall(chr(1).encode())
    client.sendall(xm.to_bytes(4, "big"))
    client.sendall(ym.to_bytes(4, "big"))
