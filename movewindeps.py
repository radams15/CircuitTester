#!/usr/bin/env python3
import os
import shlex
import re

from sys import argv
from subprocess import Popen, PIPE

env = dict(os.environ)
env["WINEDEBUG"] = "+loaddll"


def move_deps(exe):
	p = Popen(f"wine {exe}", stdin=None, stdout=PIPE, stderr=PIPE, env=env, shell=True)
	output, err = map(bytes.decode, p.communicate())
	rc = p.returncode

	#err = open("test.txt", "r").read()

	for p in re.findall(r"([^C]:\\\\usr\\\\x86_64-w64-mingw32\\\\sys-root\\\\mingw\\\\.*\.dll)", err):
		src = re.sub(r"[^C]:\\\\|\\\\", "/", p)

		dst = src.replace("/usr/x86_64-w64-mingw32/sys-root/mingw/", "")
		if dst.startswith("bin/"):
			dst = dst.replace("bin/", "", 1)

		if "/" not in dst:
			os.system(f"cp {src} {dst}")

		else:
			dst = dst.replace("lib/qt5/plugins/", "", 1)
			parts = dst.split("/")
			fname = parts[-1]
			together = ""
			for f in parts[:-1]:
				together += f + os.sep
				os.system(f"mkdir -p {together}")
			os.system(f"cp {src} {together}")

move_deps("CircuitTester.exe")
