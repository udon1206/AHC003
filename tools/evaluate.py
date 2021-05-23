import os

path = "out"

files = os.listdir(path)
res = 0
for filename in files:
  if filename[0] == 's':
    with open(os.path.join(path, filename), mode = 'r') as f:
      s = f.read()
      l = s.split()
      res += int(l[2])
print(res)

