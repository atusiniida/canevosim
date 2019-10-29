import os
import sys
import commands
import time
import re
import numpy as np




outdir = ""
indir = ""
argv = sys.argv
if (len(argv) == 3):
	indir = argv[1]
	outdir = argv[2]
elif (len(argv) == 2):
	indir = argv[1]
	outdir = argv[1]
else:
	print 'Usage: # python %s  indir outdir' % argv[0]
	quit()


homedir = os.path.abspath(os.path.dirname(__file__)) 
homedir = re.sub("/python", "", homedir)

scriptFile =  homedir + "/python/collectStatistics.py"

if os.path.exists(indir + ".stat"):
	os.system("rm -rf " + indir + ".stat")

os.system("myqsub  python " + scriptFile + " " +  indir )

explorerDir = os.path.expanduser('~') +  "/public_html/" + outdir

if os.path.exists(explorerDir):
	os.system("rm -rf " + explorerDir)

os.system("cp -r " +   homedir  + "/explorer " +  explorerDir)

os.system( "mkdir " +  explorerDir  + "/image")
os.system( "mkdir " +  explorerDir  + "/data")


os.system( "myqsub  cp " +  indir + "/*png "  +  explorerDir  + "/image")
os.system( "cp " +  indir + ".stat/*tsv " + explorerDir  + "/data")
os.system( "chmod -R 755 "  + explorerDir)

#os.system("rm tmp*")

