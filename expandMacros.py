from pathlib import Path

N = 32
for f in Path(".").glob("*.c"):
  print(f)
  if input(f"Do you want to expand {f}? [y/N] ") != "y": continue
  lines = []
  with open(f,"r") as o:
    lines = o.readlines()
  with open(f,"w") as o:
    for line in lines:
      if "#define N " in line:
        N = int(line[9:].strip())
        print(f"N: {N}")
      if "#define QUEENS Queen " in line:
        line = f"#define QUEENS Queen q{', q'.join(map(str,range(N)))}\n"
      if "@all" in line:
        lhs, rhs = tuple(line.split("@all",1))
        line = f"{''.join([f'{lhs}qs({q}){rhs}' for q in range(0,N)])}"
      o.write(f"{line}")
