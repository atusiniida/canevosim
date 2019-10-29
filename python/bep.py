import os
import sys
import commands
import time
import re

argv = sys.argv
del argv[0]

argvStr = " ".join(argv)

outprefix = "out"

homedir = os.path.abspath(os.path.dirname(__file__))
homedir = re.sub("/python", "", homedir)

bepCpp = homedir + "/src/bep"
bepR = homedir + "/R/bepProcess.R"

bepCpp = "'" + bepCpp + "'"
bepR  = "'" + bepR + "'"

if argvStr.find("-h") != -1:
	os.system(bepCpp  + " -h")
	exit(0)

pattern = r"-o\s+(\S+)"
match = re.search(pattern, argvStr)
if match:
	outprefix = os.path.abspath(match.group(1))
	argvStr = re.sub(pattern, "", argvStr)

pattern = r"-k\s+(\S+)"
match = re.search(pattern, argvStr)
if match:
	tmp = match.group(1)
	if tmp > 0:
		bepR = homedir + "/R/bepProcessTimeCourse.R"
		bepR  = "'" + bepR + "'"

pdf = "T"
pattern = r"-pdf\s+(\S+)"
match = re.search(pattern, argvStr)
if match:
	pdf = match.group(1)
	argvStr = re.sub(pattern, "", argvStr)

png = "F"
pattern = r"-png\s+(\S+)"
match = re.search(pattern, argvStr)
if match:
	png = match.group(1)
	argvStr = re.sub(pattern, "", argvStr)



# Usage:
#   -r baseReplicationRate(0.000100)
#   -d baseDeathRate(0.000000)
#   -c driverMutationContainerSize(5)
#   -g driverGenomeSize(10000)
#   -m driverMutationRate(0.010000)
#   -C passengerMutationContainerSize(1000)
#   -G passengerGenomeSize(1000000)
#   -M passengerMutationRate(0.100000)
#   -f replicationRateIncrease(5.000000)
#   -D deathRateDecrease(1.000000)
#   -T maxTime(1000000)
#   -P maxPopulationSize(100000)
#   -p initialPopulationSize(1)
#   -s tumorSampleSize(1000)
#   -F mutationFrequencyCutoff(0.000000)
#   -k populationSizeChangeCutoff (0.000000, if positive, print time course data)
#   -K carryingCapacity(inf)
#   -S symmetricReplicationProbablity (1.000000)
#   -e differentiatedCellDeathRate (0.010000)
#   -i infinitizingCarryingCapacityProbability (0.000000)
#   -o outfilePrefix(out)


def readOptionFromPrmFile(file):
	prm2opt = {}
	prm2opt["baseReplicationRate"] = "r"
	prm2opt["baseDeathRate"] = "d"
	prm2opt["driverMutationContainerSize"] = "c"
	prm2opt["driverGenomeSize"] = "g"
	prm2opt["driverMutationRate"] = "m"
	prm2opt["passengerMutationContainerSize"] = "C"
	prm2opt["passengerGenomeSize"] = "G"
	prm2opt["passengerMutationRate"] = "M"
	prm2opt["replicationRateIncrease"] = "f"
	prm2opt["deathRateDecrease"] = "D"
	prm2opt["maxTime"] = "T"
	prm2opt["maxPopulationSize"] = "P"
	prm2opt["initialPopulationSize"] = "p"
	prm2opt["tumorSampleSize"] = "s"
	prm2opt["mutationFrequencyCutoff"] = "F"
	prm2opt["carryingCapacity"] = "K"
	prm2opt["symmetricReplicationProbablity"] = "S"
	prm2opt["differentiatedCellDeathRate"] = "e"
	prm2opt["infinitizingCarryingCapacityProbability"] = "i"

	opt = ""
	try:
		with open(file,'r') as f:
			for line in f:
				line = line.rstrip()
				tmp = line.split("=")
				if tmp[0] in prm2opt:
					opt += " -" + prm2opt[tmp[0]] + " " + tmp[1] + " "
	except:
		opt +=  ""
	return opt

pattern = r"-prm\s+(\S+)"
match = re.search(pattern, argvStr)
if match:
	argvStr += readOptionFromPrmFile(match.group(1))
	argvStr = re.sub(pattern, "", argvStr)

outprefix0 = os.path.abspath(outprefix)
outprefix = "'" + outprefix0 + "'"

argvStr = argvStr + " -o " + outprefix
if not os.path.isfile(outprefix0 + ".prm") or not os.path.isfile(outprefix0 + ".dr"):
	command = bepCpp + " " + argvStr
	if os.system(command):
		sys.exit(1)


def runR(script, args):
	out = []
	for k, v in args.items():
		out.append(k + " <- " + v)
	out.append("source(" +  script + ")")
	tmpR = "tmp" + str(os.getpid()) + ".R"
	with open(tmpR, "w") as file:
		file.write("\n".join(out) + "\n")
	status = os.system("R --slave --args  < " + tmpR)
	os.system("rm " + tmpR)
	return status

if not os.path.isfile(outprefix0 + ".stat"):
	args = {}
	args["infilePrefix"] =  outprefix
	args["outfilePrefix"] = outprefix
	args["pdf"] = pdf
	args["png"] = png
	if runR(bepR, args):
		sys.exit(1)
