x = random_prime(2^512)
p = next_prime(x)
q = next_prime(p)

n = p * q
phi = (p - 1) * (q - 1)
e = 65537

del x, p, q

m = int.from_bytes(b'CTF{...}')
c = pow(m, e, n)

del m

print(f'{n=}')
print(f'{e=}')
print(f'{c=}')
