# Cancer Evolution Simulator
This simulator is developed for conducting our simulation study about cancer evolution, the details of which are described in our [mauscript](https://www.biorxiv.org/content/10.1101/762997v1) and [website](https://www.hgc.jp/~aniida/canevosim/index.html).

##  1. requirement
- C++11 compiler
- Python 2.7 and the following library: numpy
- R 3.6 and the following libraries: gplots, vegan and RColorBrewer

##  2. installation

`git clone  https://github.com/atusiniida/canevosim.git`\
`cd canevosim/src`\
(modify Makefile for calling an adequate  C++11 compiler, if necessary.)\
`make` \
`cd ../`\
`python python/canevosim.py -h`

##  3. usege

usage: python bep.py [option]\
option:  # The value in the brackets indicates the default value.\
　-r baseReplicationRate(0.000100)  # Corresponding to <em>g</em><sub><em>0</em></sub> in our [mauscript](https://www.biorxiv.org/content/10.1101/762997v1)\
　-d baseDeathRate(0.000000)  # Corresponding to <em>d</em><sub><em>0</em></sub> \
　-c driverMutationContainerSize(5)  # Corresponding to <em>N</em><sub><em>d</em></sub> \
　-g driverGenomeSize(10000)  # Number of mutable positions generating driver mutations\
　-m driverMutationRate(0.010000)  # Corresponding to <em>m</em><sub><em>d</em></sub>\
　-C passengerMutationContainerSize(1000)  # Corresponding to <em>N</em><sub><em>n</em></sub> \
　-G passengerGenomeSize(1000000)  # Number of mutable positions generating neutral mutations\
　-M passengerMutationRate(0.100000)  # Corresponding to <em>m</em><sub><em>n</em></sub>\
　-D deathRateDecrease(1.000000)  # Corresponding to <em>e</em>\
　-T maxTime(1000000)  # Corresponding to <em>T</em>\
　-P maxPopulationSize(100000)  # Corresponding to <em>P</em>\
　-p initialPopulationSize(1)  # Number of cells at the start of simulation\
　-s tumorSampleSize(1000)  # Number of cells sampled for generating the output\
　-F mutationFrequencyCutoff(0.000000)  # Clones whose frequency are less than this values are discarded for saving memory space.\
　-k populationSizeChangeCutoff (0.000000, if positive, print time course data)  # For specifying time points for obtaining evolutionary snapshots\
　-K carryingCapacity(inf)  # Corresponding to <em>m</em><sub><em>e</em></sub>\
　-S symmetricReplicationProbablity (1.000000)  # Corresponding to <em>s</em>\
　-e differentiatedCellDeathRate (0.010000)  # Corresponding to <em>d</em><sub>0</sub><sup>d</sup>\
　-i infinitizingCarryingCapacityProbability (0.000000)  # Corresponding to <em>p</em><sub><em>c</em></sub>\
　-o outfilePrefix(out)  # Prefix of the outfiles

##  3. output
you obtain the following 5 files:
- [outfilePrefix].dr # Each line contains driver mutations in each cell.
- [outfilePrefix].ps #  Each line contains  neutral  mutations in each cell.
- [outfilePrefix].prm  # containing  parameter settings
- [outfilePrefix].stat # containing summary statistics 
- [outfilePrefix].pdf # visualizing the mutation profile matrix 

When symmetricReplicationProbablity  < 1, you additionaly obtain the following file:
-  [outfilePrefix].df  # Each line contains  differenciation status of  each cell . 

When infinitizingCarryingCapacityProbability  > 0, you additionaly obtain the following file:
-  [outfilePrefix].ic  # Each line contains  explosive driver mutations infinitizing carrying capacity  in each cell.

When  populationSizeChangeCutoff  > 0, you additionaly obtain  more files generated as evolutionary snapshots at every time point when the fold increase of the population size exceeds the opulationSizeChangeCutoff value.

