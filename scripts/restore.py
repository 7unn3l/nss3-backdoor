import ctypes,os,sys
import shutil


def elevate():
    try:
        is_admin = os.getuid() == 0
    except AttributeError:
        is_admin = ctypes.windll.shell32.IsUserAnAdmin() != 0
    
    if not is_admin:
        ctypes.windll.shell32.ShellExecuteW(None, "runas", sys.executable, " ".join(sys.argv), None, 1)
        exit(0)

def main():
    elevate()
    print('[*] restoring firefox nss3 dll')

    print('[*] restoring original nss3.dll...')
    shutil.copyfile("../ext/nss3_original.dll","C:/Program Files/Mozilla Firefox/nss3.dll")

    print('[*] deleting old nss3.orig...')
    os.remove("C:/Program Files/Mozilla Firefox/nss3.orig")

if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        print('[-] Exception during restoration process: ',e)
    input('[+] finished. [ENTER] to exit')