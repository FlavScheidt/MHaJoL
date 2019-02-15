import pandas as pd
#
import matplotlib.pyplot as plt
import numpy as np

try:
    # for Python2
    from Tkinter import *   ## notice capitalized T in Tkinter 
except ImportError:
    # for Python3
    from tkinter import *   ## notice lowercase 't' in tkinter here

# metrics=[('L2CACHE', 'L2_RQSTS_MISS', 'L2 Miss Requests', ' ', 'log'),
# 	('L2CACHE', 'L2missrate', 'L2 Miss Rate', ' ', ' '),
# 	('L2CACHE', 'L2_TRANS_ALL_REQUESTS', 'L2 Requests', ' ', 'log'),
# 	('L3CACHE', 'MEM_LOAD_RETIRED_L3_MISS', 'L3 Miss Requests', 'log', 'log'),
# 	('L3CACHE', 'L3missrate', 'L3 Miss Rate', 'log', 'log'),
# 	('TLB_DATA', 'DTLB_LOAD_MISSES_CAUSES_A_WALK', 'DTLB Load Misses', 'log', 'log')
# ]

metrics=[
('L3CACHE','UOPS_RETIRED_ALL','UOPS_RETIRED_ALL ', ' ', ' '),
('FLOPS_AVX','PackedSPMFLOP_s','PackedSPMFLOP_s ', ' ', ' '),
('FLOPS_AVX','PackedDPMFLOP_s','PackedDPMFLOP_s ', ' ', ' '),
('L3CACHE','MEM_LOAD_RETIRED_L3_MISS','MEM_LOAD_RETIRED_L3_MISS ', ' ', ' '),
('L3CACHE','MEM_LOAD_RETIRED_L3_HIT','MEM_LOAD_RETIRED_L3_HIT ', ' ', ' '),
('DATA','MEM_INST_RETIRED_ALL_STORES','MEM_INST_RETIRED_ALL_STORES ', ' ', ' '),
('DATA','MEM_INST_RETIRED_ALL_LOADS','MEM_INST_RETIRED_ALL_LOADS ', ' ', ' '),
('DATA','Loadtostoreratio','Loadtostoreratio ', ' ', ' '),
('L3CACHE','L3requestrate','L3requestrate ', ' ', ' '),
('L3CACHE','L3missratio','L3missratio ', ' ', ' '),
('L3CACHE','L3missrate','L3missrate ', ' ', ' '),
('L3','L3loaddatavolume[GBytes]','L3loaddatavolume[GBytes] ', ' ', ' '),
('L3','L3loadbandwidth[MBytes_s]','L3loadbandwidth[MBytes_s] ', ' ', ' '),
('L3','L3evictdatavolume[GBytes]','L3evictdatavolume[GBytes] ', ' ', ' '),
('L3','L3evictbandwidth[MBytes_s]','L3evictbandwidth[MBytes_s] ', ' ', ' '),
('L3','L3datavolume[GBytes]','L3datavolume[GBytes] ', ' ', ' '),
('L3','L3bandwidth[MBytes_s]','L3bandwidth[MBytes_s] ', ' ', ' '),
('L2CACHE','L2requestrate','L2requestrate ', ' ', ' '),
('L2CACHE','L2missratio','L2missratio ', ' ', ' '),
('L2CACHE','L2missrate','L2missrate ', ' ', ' '),
('L2','L2Dloaddatavolume[GBytes]','L2Dloaddatavolume[GBytes] ', ' ', ' '),
('L2','L2Dloadbandwidth[MBytes_s]','L2Dloadbandwidth[MBytes_s] ', ' ', ' '),
('L2','L2Devictdatavolume[GBytes]','L2Devictdatavolume[GBytes] ', ' ', ' '),
('L2','L2Devictbandwidth[MBytes_s]','L2Devictbandwidth[MBytes_s] ', ' ', ' '),
('L2','L2datavolume[GBytes]','L2datavolume[GBytes] ', ' ', ' '),
('L2','L2bandwidth[MBytes_s]','L2bandwidth[MBytes_s] ', ' ', ' '),
('L3','L2_TRANS_L2_WB','L2_TRANS_L2_WB ', ' ', ' '),
('L2CACHE','L2_TRANS_ALL_REQUESTS','L2_TRANS_ALL_REQUESTS ', ' ', ' '),
('L2CACHE','L2_RQSTS_MISS','L2_RQSTS_MISS ', ' ', ' '),
('L3','L2_LINES_IN_ALL','L2_LINES_IN_ALL ', ' ', ' '),
('L2','L1D_REPLACEMENT','L1D_REPLACEMENT ', ' ', ' '),
('L2','L1D_M_EVICT','L1D_M_EVICT ', ' ', ' '),
('BRANCH','Instructionsperbranch','Instructionsperbranch ', ' ', ' '),
('L2','ICACHE_64B_IFTAG_MISS','ICACHE_64B_IFTAG_MISS ', ' ', ' '),
('FLOPS_AVX','FP_ARITH_INST_RETIRED_256B_PACKED_SINGLE','FP_ARITH_INST_RETIRED_256B_PACKED_SINGLE ', ' ', ' '),
('FLOPS_AVX','FP_ARITH_INST_RETIRED_256B_PACKED_DOUBLE','FP_ARITH_INST_RETIRED_256B_PACKED_DOUBLE ', ' ', ' '),
('BRANCH','Branchrate','Branchrate ', ' ', ' '),
('BRANCH','Branchmispredictionratio','Branchmispredictionratio ', ' ', ' '),
('BRANCH','Branchmispredictionrate','Branchmispredictionrate ', ' ', ' '),
('BRANCH','BR_MISP_RETIRED_ALL_BRANCHES','BR_MISP_RETIRED_ALL_BRANCHES ', ' ', ' '),
('BRANCH','BR_INST_RETIRED_ALL_BRANCHES','BR_INST_RETIRED_ALL_BRANCHES ', ' ', ' ')
]

# metrics=[('CYCLE_ACTIVITY', 'CycleswithoutexecutionduetoL1D[_]'),
# ('CYCLE_ACTIVITY', 'Cycleswithoutexecution[_]'),
# ('CYCLE_ACTIVITY', 'CycleswithoutexecutionduetoL2[_]'),
# ('CYCLE_ACTIVITY', 'Cycleswithoutexecutionduetomemory[_]'),
# ('ENERGY', 'Power[W]'),
# ('ENERGY', 'Temperature[C]'),
# ('ENERGY', 'PowerPP1[W]'),
# ('ENERGY', 'PowerDRAM[W]'),
# ('ENERGY', 'PowerPP0[W]'),
# ('ENERGY', 'EnergyPP1[J]'),
# ('L2', 'L2bandwidth[MBytes_s]'),
# ('L2', 'L2datavolume[GBytes]'),
# ('L2CACHE', 'L2requestrate'),
# ('L2CACHE', 'L2missrate'),
# ('L2CACHE', 'L2missratio'),
# ('L3', 'L3datavolume[GBytes]'),
# ('L3', 'L3bandwidth[MBytes_s]'),
# ('L3CACHE', 'L3requestrate'),
# ('L3CACHE', 'L3missrate'),
# ('L3CACHE', 'L3missratio'),
# ('TLB_DATA', 'L1DTLBloadmisses'),
# ('TLB_DATA', 'L1DTLBloadmissrate'),
# ('TLB_DATA', 'L1DTLBstoremisses'),
# ('TLB_DATA', 'L1DTLBstoremissrate'),
# ('TLB_DATA', 'L1DTLBloadmissduration[Cyc]'),
# ('TLB_DATA', 'L1DTLBstoremissduration[Cyc]'),
# ('BRANCH', 'Instructionsperbranch'),
# ('BRANCH', 'Branchrate'),
# ('BRANCH', 'Branchmispredictionratio'),
# ('BRANCH', 'Branchmispredictionrate')
# ]

generalMetrics=[('Runtimeunhalted[s]', ' ', 'log'),
 	('RDTSCRuntime[s]', ' ', 'log'),
 	('Runtime(RDTSC)[s]', ' ', 'log'),
 	('CPI', ' ', ' ')]

regions=['Core', 'Generation']

methods=[('cuckoo', (0, (5, 5)), 'crimson', 's'), ('vecCuckoo', (0, (3, 10, 1, 10)), 'steelblue', 'p')]

df=pd.read_csv('/home/flav/Mestrado/MHaJoL/data/grouped.csv', sep=',')

for (group, metric, yAxis, logG, logC) in metrics:
	for region in regions:

		dfFilter = df.loc[(df['grp'] == group) & (df['metric'] == metric) & (df['region'] == region)]

		plt.figure(figsize=(4.8,2))
		for (method, linestyle, color, marker) in methods:
			dfMethod=dfFilter.loc[(dfFilter['method'] == method)]
			plt.plot( 'selectivity', 'value', data=dfMethod, marker=marker, label=method, linestyle=linestyle, markersize=5, color=color)

		plt.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc=3, ncol=5, mode='expand', borderaxespad=0., prop={'size': 7})

		plt.xlabel('Selectivity')
		plt.ylabel(yAxis)

		plt.subplots_adjust(left=0.19, bottom=0.09, right=0.98, top=0.92, wspace=0.17, hspace=0.17)
		
		plt.grid(alpha=0.4)  
		if  ((logG == 'log' and region == 'Generation') or (logC == 'log' and region == 'Core')):
			plt.yscale('log')

		plt.rcParams.update({'font.size': 7})

		plt.savefig('/home/flav/Mestrado/MHaJoL/data/out/plots/'+group+'_'+metric+'_'+region+'.png',  bbox_inches='tight')
		# plt.show()
		plt.clf()
		# plt.close()

for (metric, logG, logC) in generalMetrics:
	for region in regions:

		dfFilter = df.loc[(df['grp'] == group) & (df['metric'] == metric) & (df['region'] == region)]
		# plt.figure(figsize=(8,3))

		for (method, linestyle, color, marker) in methods:
			dfMethod=dfFilter.loc[(dfFilter['method'] == method)]
			plt.plot( 'selectivity', 'value', data=dfMethod, marker=marker, label=method, linestyle=linestyle,  markersize=5, color=color)

		plt.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc=3, ncol=5, mode='expand', borderaxespad=0., prop={'size': 7})

		plt.xlabel('Selectivity')
		plt.ylabel(metric)

		plt.subplots_adjust(left=0.19, bottom=0.09, right=0.98, top=0.92, wspace=0.17, hspace=0.17)
		
		plt.grid(alpha=0.4)    
		if  ((logG == 'log' and region == 'Generation') or (logC == 'log' and region == 'Core')):
			plt.yscale('log')

		plt.rcParams.update({'font.size': 7})

		plt.savefig('/home/flav/Mestrado/MHaJoL/data/out/plots/general_'+metric+'_'+region+'.png',  bbox_inches='tight')
		# plt.show()
		plt.clf()
		# plt.close()