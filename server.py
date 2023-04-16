import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
port = 0 # picked by you 
lhost = "X.X.X.X"
s.bind((lhost, port));
s.listen(1);
print(f"[~] Listening on {lhost}:{port}");
conn, addr = s.accept();
print(f"[+] Connection accepted {addr}");
while True:
    try:
        cmd = input("> ")
        conn.sendall(cmd.encode())
        data = conn.recv(1024).decode()
        if data:
            print(data)
    except SyntaxError:
        print("[!] ERROR!")

print("Closing connection")
conn.close()