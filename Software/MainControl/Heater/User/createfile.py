import os

cwd = os.getcwd()

inc = os.path.join(cwd, "Inc")
src = os.path.join(cwd, "Src")

while True:
    inp = input("File name:")
    if not inp:
        break
    open(os.path.join(inc, inp + '.h'), "w")
    open(os.path.join(src, inp + '.cpp'), "w")