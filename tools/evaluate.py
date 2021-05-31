import os

path = "score.txt"
res = 0
cnt = 0
with open(path) as f:
  string = f.readlines()
  for line in string:
    res += int(line.split()[2])
    cnt += 1
print(f"合計: {res}")
print(f"平均: {res / cnt}")


