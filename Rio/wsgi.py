import socket ## The address is roboRIO-6341-FRC:5801
from flask import Flask, escape, request

#app = Flask(__name__)

#@app.route('/')
#def hello():
#    name = request.args.get("name", "World")
#    return f'Hello, {escape(name)}!'

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(("roboRIO-6341-FRC.local", 5801))

initialBroadcast = client.recv(5)

while 1:
    xm = int(input("Xmov> "))
    ym = int(input("Ymov> "))
    client.sendall(chr(1).encode())
    client.sendall(xm.to_bytes(4, "big"))
    client.sendall(ym.to_bytes(4, "big"))
    print("Sent.")
    bs = client.recv(1)
    print(bs)
    if bs == b"0":
        print("Ran successfully")
