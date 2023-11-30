import dis
import marshal

with open(input("Enter .pyc path: "), "rb") as f:
    f.seek(16)
    print(dis.dis(marshal.load(f)))

