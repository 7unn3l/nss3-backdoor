import socket

addr = ('127.0.0.1',6969)

class Logger():
    def __init__(self,addr):
        self.addr = addr
        self.sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
    
    def start(self):
        print(f'--starting logging server on {self.addr[0]}:{self.addr[1]}--')
        self.sock.bind(addr)
        
        while True:
            data, _ = self.sock.recvfrom(4096)
            print(data.decode())


def main():
    l = Logger(addr)
    l.start()

if __name__ == '__main__':
    main()
