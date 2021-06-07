#!/usr/bin/env python3

from subprocess import Popen, PIPE
from os import system, path
import shlex

OUT = "winbuild/src/main"

def find(file, where):
	command = f"find {where} -name {file}"
	command = shlex.split(command)
	
	c = Popen(command, stdout=PIPE)
	stdout, stderr = [(x.decode() if x is not None else None) for x in c.communicate()]
	if stdout == "":
		#print(f"Could Not Find: '{file}'")
		return None
	else:
		src = stdout.strip()
		#print(f"Found '{file}' at '{src}'")
		return src

for d in open("deps.txt", "r").readlines():
	file, dst = d.strip().split(" ", 1)
	
	if "/usr/" in file:
		src = file
	else:
		src = find(file, "'/usr/x86_64-w64-mingw32/sys-root/mingw/lib' '/usr/x86_64-w64-mingw32/sys-root/mingw/bin'")
	if src != None:
		if not path.exists(f"{OUT}/{dst}"):
			system(f"mkdir -p {OUT}/{dst}")
		system(f"cp {src} {OUT}/{dst}")

system(f"chmod -R 777 {OUT}")
