# Cancer Evolution Simulator
This simulator is developed for conducting our simulation study about cancer evolution, the details of which are described in our [manuscript](https://www.biorxiv.org/content/10.1101/762997v1) and [website](https://www.hgc.jp/~aniida/canevosim/index.html).

##  1. requirement
- C++11 compiler
- Python 2.7
- R 3.6 and the following libraries: gplots, vegan and RColorBrewer

##  2. installation

`git clone  https://github.com/atusiniida/canevosim.git`\
`cd canevosim/src`\
(Modify Makefile for calling an adequate  C++11 compiler, if necessary.)\
`make` \
`cd ../`\
`python python/canevosim.py -h`

##  3. usage

usage: python bep.py [option]\
option:  # the value in the brackets indicates the default value.\
　-r baseReplicationRate(0.000100)  # corresponding to <em>g</em><sub><em>0</em></sub> in our [mauscript](https://www.biorxiv.org/content/10.1101/762997v1)\
　-d baseDeathRate(0.000000)  # corresponding to <em>d</em><sub><em>0</em></sub> \
　-c driverMutationContainerSize(5)  # corresponding to <em>N</em><sub><em>d</em></sub> \
　-g driverGenomeSize(10000)  # number of mutable positions generating driver mutations\
　-m driverMutationRate(0.010000)  # corresponding to <em>m</em><sub><em>d</em></sub>\
　-C passengerMutationContainerSize(1000)  # corresponding to <em>N</em><sub><em>n</em></sub> \
　-G passengerGenomeSize(1000000)  # number of mutable positions generating neutral mutations\
　-M passengerMutationRate(0.100000)  # corresponding to <em>m</em><sub><em>n</em></sub>\
　-D deathRateDecrease(1.000000)  # corresponding to <em>e</em>\
　-T maxTime(1000000)  # corresponding to <em>T</em>\
　-P maxPopulationSize(100000)  # corresponding to <em>P</em>\
　-p initialPopulationSize(1)  # number of cells at the start of simulation\
　-s tumorSampleSize(1000)  # number of cells sampled for generating the output\
　-F mutationFrequencyCutoff(0.000000)  # clones whose frequency are less than this values are discarded for saving memory space.\
　-k populationSizeChangeCutoff (0.000000, if positive, print time course data)  # for specifying time points for obtaining evolutionary snapshots\
　-K carryingCapacity(inf)  # corresponding to <em>m</em><sub><em>e</em></sub>\
　-S symmetricReplicationProbablity (1.000000)  # corresponding to <em>s</em>\
　-e differentiatedCellDeathRate (0.010000)  # corresponding to <em>d</em><sub>0</sub><sup>d</sup>\
　-i infinitizingCarryingCapacityProbability (0.000000)  # corresponding to <em>p</em><sub><em>c</em></sub>\
　-o outfilePrefix(out)  # prefix of the result files

##  3. output
You obtain the following 5 result files:
- [outfilePrefix].dr # each line contains driver mutations in each cell.
- [outfilePrefix].ps #  each line contains  neutral  mutations in each cell.
- [outfilePrefix].prm  # containing  parameter settings
- [outfilePrefix].stat # containing summary statistics
- [outfilePrefix].pdf # visualizing the mutation profile matrix

When symmetricReplicationProbablity  < 1, you additionally obtain the following file:
-  [outfilePrefix].df  # each line contains  differentiation status of each cell.

When infinitizingCarryingCapacityProbability  > 0, you additionally obtain the following file:
-  [outfilePrefix].ic  # each line contains  explosive driver mutations infinitizing carrying capacity in each cell.

When  populationSizeChangeCutoff  > 0, you obtain  more files generated as evolutionary snapshots at every time point when the fold increase of the population size exceeds the populationSizeChangeCutoff value.
