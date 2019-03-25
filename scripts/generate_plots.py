import numpy as np
import pandas as pd
#
import matplotlib.pyplot as plt

import matplotlib.ticker as ticker
# from collections import namedtuple

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

# metrics=[
# ('L3CACHE','UOPS_RETIRED_ALL','UOPS_RETIRED_ALL ', ' ', ' '),
# # ('FLOPS_AVX','PackedSPMFLOP_s','PackedSPMFLOP_s ', ' ', ' '),
# # ('FLOPS_AVX','PackedDPMFLOP_s','PackedDPMFLOP_s ', ' ', ' '),
# ('L3CACHE','MEM_LOAD_RETIRED_L3_MISS','MEM_LOAD_RETIRED_L3_MISS ', ' ', ' '),
# ('L3CACHE','MEM_LOAD_RETIRED_L3_HIT','MEM_LOAD_RETIRED_L3_HIT ', ' ', ' '),
# ('DATA','MEM_INST_RETIRED_ALL_STORES','MEM_INST_RETIRED_ALL_STORES ', ' ', ' '),
# ('DATA','MEM_INST_RETIRED_ALL_LOADS','MEM_INST_RETIRED_ALL_LOADS ', ' ', ' '),
# ('DATA','Loadtostoreratio','Load to Store Ratio ', ' ', ' '),
# ('L3CACHE','L3requestrate','L3requestrate ', ' ', ' '),
# ('L3CACHE','L3missrate','L3missrate ', ' ', ' '),
# # ('L3','L3loaddatavolume[GBytes]','L3loaddatavolume[GBytes] ', ' ', ' '),
# # ('L3','L3loadbandwidth[MBytes_s]','L3loadbandwidth[MBytes_s] ', ' ', ' '),
# # ('L3','L3datavolume[GBytes]','L3datavolume[GBytes] ', ' ', ' '),
# # ('L3','L3bandwidth[MBytes_s]','L3bandwidth[MBytes_s] ', ' ', ' '),
# ('L2CACHE','L2requestrate','L2requestrate ', ' ', ' '),
# ('L2CACHE','L2missrate','L2missrate ', ' ', ' '),
# # ('L2','L2Dloaddatavolume[GBytes]','L2Dloaddatavolume[GBytes] ', ' ', ' '),
# # ('L2','L2Dloadbandwidth[MBytes_s]','L2Dloadbandwidth[MBytes_s] ', ' ', ' '),
# # ('L2','L2datavolume[GBytes]','L2datavolume[GBytes] ', ' ', ' '),
# # ('L2','L2bandwidth[MBytes_s]','L2bandwidth[MBytes_s] ', ' ', ' '),
# ('L3','L2_TRANS_L2_WB','L2_TRANS_L2_WB ', ' ', ' '),
# ('L2CACHE','L2_TRANS_ALL_REQUESTS','L2_TRANS_ALL_REQUESTS ', ' ', ' '),
# ('L2CACHE','L2_RQSTS_MISS','L2_RQSTS_MISS ', ' ', ' '),
# ('L3','L2_LINES_IN_ALL','L2_LINES_IN_ALL ', ' ', ' '),
# ('L2','L1D_REPLACEMENT','L1D_REPLACEMENT ', ' ', ' '),
# ('L2','L1D_M_EVICT','L1D_M_EVICT ', ' ', ' '),
# ('BRANCH','Instructionsperbranch','Instructions per branch ', ' ', ' '),
# ('L2','ICACHE_64B_IFTAG_MISS','L2 misses ', ' ', ' '),
# # ('FLOPS_AVX','FP_ARITH_INST_RETIRED_256B_PACKED_SINGLE','FP_ARITH_INST_RETIRED_256B_PACKED_SINGLE ', ' ', ' '),
# # ('FLOPS_AVX','FP_ARITH_INST_RETIRED_256B_PACKED_DOUBLE','FP_ARITH_INST_RETIRED_256B_PACKED_DOUBLE ', ' ', ' '),
# ('BRANCH','Branchrate','Branch rate ', ' ', ' '),
# ('BRANCH','Branchmispredictionrate','Branch misprediction rate ', ' ', ' ')
# ]

metrics=[
#@s('L2CACHE', 'L2missrate', 'L2 Miss Rate (log2)', 'log', ''),
# ('L2CACHE', 'L2missratio', 'L2 Miss Ratio', '', ''),
('L2CACHE', 'L2missratio', 'L2 Miss Ratio', '', ''),
('BRANCH', 'Branchmispredictionratio', 'Misprediction Ratio', '', ''),
('BRANCH', 'Branchrate', 'Branch Rate', '', '')
# ('ENERGY', 'Power[W]', 'Power [W]', '',''),
# ('ENERGY', 'PowerDRAM[W]', 'Power DRAM [W]', '',''),
# ('ENERGY', 'PowerPP0[W]', 'Power Core Devices [W]', '','')
# ]

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

# generalMetrics=[#('Runtimeunhalted[s]', ' ', ' '),
 	# ('RDTSCRuntime[s]', ' ', ' '),
 	# ('Runtime(RDTSC)[s]', 'Build Time [s]', ' ', ' ')
 	# ('CPI', ' ', ' ')
 	]

regions=['Core']

# pd.set_option('display.max_columns', 30)

# df=pd.read_csv('/home/flav/Mestrado/MHaJoL/data/grouped.csv', sep=';', encoding='utf-8-sig')
df=pd.read_csv('/home/flav/Mestrado/MHaJoL/data/results.csv', sep=',', encoding='utf-8-sig')

#First of all, find the outliers!
dfQuantile1 = df.groupby(['grp', 'metric', 'method', 'region', 'selectivity']).quantile(q=0.25, axis=0, numeric_only=True, interpolation='linear').reset_index()
dfQuantile1 = dfQuantile1.rename(index=str, columns={"value": "first"})

dfQuantile2 = df.groupby(['grp', 'metric', 'method', 'region', 'selectivity']).quantile(q=0.75, axis=0, numeric_only=True, interpolation='linear').reset_index()
dfQuantile2 = dfQuantile2.rename(index=str, columns={"value": "third"})

dfQuantile = pd.merge(dfQuantile1, dfQuantile2, how='inner', on=['grp', 'metric', 'method', 'region', 'selectivity'])
dfQuantile['IQR'] = dfQuantile['third'] - dfQuantile['first']

dfAggMedian = df.groupby(['grp', 'metric', 'method', 'region', 'selectivity'], as_index = False).aggregate(np.median)
dfAggMedian = dfAggMedian.rename(index=str, columns={"value": "median"})
dfIQR = pd.merge(dfQuantile, dfAggMedian, how='inner', on=['grp', 'metric', 'method', 'region', 'selectivity'])
dfIQR = pd.merge(dfIQR, df, how='inner', on=['grp', 'metric', 'method', 'region', 'selectivity'])

dfIQR['values'] = np.where((dfIQR['value'] < (dfIQR['first'] - (1.5 * dfIQR['IQR']))) | (dfIQR['value'] < (dfIQR['third'] - (1.5 * dfIQR['IQR']))), dfIQR['median'], dfIQR['value'])

dfIQR = dfIQR.drop(columns=['value', 'first', 'third', 'IQR', 'median'])
dfIQR = dfIQR.rename(index=str, columns={"values": "value"})

dfAggAVG = dfIQR.groupby(['grp', 'metric', 'method', 'region', 'selectivity'], as_index = False).aggregate(['mean','std'])

# print(dfAggAVG.head(100))

for (group, metric, yAxis, logG, logC) in metrics:
	for region in regions:

		# dfFilter = dfAggAVG.loc((dfAggAVG['grp'] == group) & (dfAggAVG['metric'] == metric) & (dfAggAVG['region'] == region))
		dfFilter = dfAggAVG.xs(group, level='grp').xs(metric, level='metric').xs(region, level='region')
		# print(dfFilter.head(100))
		n_groups = 9
		
		index = np.arange(n_groups)
		bar_width = 0.35

		opacity = 0.4

		dfMethodVec = dfFilter.xs("vecCuckoo", level='method')['value'].reset_index().sort_values('selectivity')
		dfValuesVec = dfMethodVec["mean"]
		dfMethod = dfFilter.xs("cuckoo", level='method')['value'].reset_index().sort_values('selectivity')
		dfValues = dfMethod["mean"]

		plt.style.use('ggplot')

		plt.rcParams.update({'font.size': 14})

		fig, ax = plt.subplots()

		rects1 = ax.bar(index, dfValues, bar_width, alpha=opacity,  label='Scalar')#color='steelblue',
		rects2 = ax.bar(index + bar_width, dfValuesVec, bar_width, alpha=opacity, label='ViViD')#color='deeppink', 

		ax.yaxis.set_major_formatter(ticker.ScalarFormatter(useMathText=True))

		ax.set_xlabel('Selectivity')
		ax.set_ylabel(yAxis)
		ax.set_xticks(index + bar_width / 2)
		ax.set_xticklabels(('10', '20', '30', '40', '50', '60', '70', '80', '90'))
		ax.legend()

		ax.grid(alpha=0.4)  

		fig.tight_layout()

		# plt.figure(figsize=(4.8,2))
		fig.subplots_adjust(left=0.19, bottom=0.09, right=0.98, top=0.92, wspace=0.17, hspace=0.17)
		
		if  ((logG == 'log' and region == 'Generation') or (logC == 'log' and region == 'Core')):
			plt.yscale('log', basey=2)
		# else:
		# 	plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0), useOffset=3)

		fig.savefig('/home/flav/Mestrado/MHaJoL/data/out/plots/'+group+'_'+metric+'_'+region+'.png',  bbox_inches='tight')
		# fig.show()
		fig.clf()
		# print('1')
		# # plt.close()

dfAggAVG = df.groupby(['metric', 'method', 'region', 'selectivity'], as_index = False).aggregate(['mean', np.median, 'std'])

# for (metric, yAxis, logG, logC) in generalMetrics:
# 	for region in regions:

# 		dfFilter = dfAggAVG.xs(metric, level='metric').xs(region, level='region')
# 		# plt.figure(figsize=(8,3))

# 		n_groups = 9
# 		fig, ax = plt.subplots()

# 		index = np.arange(n_groups)
# 		bar_width = 0.35

# 		opacity = 0.4

# 		dfMethod = dfFilter.xs("cuckoo", level='method')['value'].reset_index().sort_values('selectivity')
# 		dfValues = dfMethod["mean"]
# 		dfMethodVec = dfFilter.xs("vecCuckoo", level='method')['value'].reset_index().sort_values('selectivity')
# 		dfValuesVec = dfMethodVec["mean"]

# 		rects1 = ax.bar(index, dfValues, bar_width, alpha=opacity,  label='Scalar')#color='steelblue',
# 		rects2 = ax.bar(index + bar_width, dfValuesVec, bar_width, alpha=opacity, label='ViViD')#color='deeppink', 

# 		ax.yaxis.set_major_formatter(ticker.ScalarFormatter(useMathText=True))

# 		ax.set_xlabel('Selectivity')
# 		ax.set_ylabel(yAxis)
# 		ax.set_xticks(index + bar_width / 2)
# 		ax.set_xticklabels(('10', '20', '30', '40', '50', '60', '70', '80', '90'))
# 		ax.legend()

# 		fig.tight_layout()

# 		# plt.figure(figsize=(4.8,2))
# 		fig.subplots_adjust(left=0.19, bottom=0.09, right=0.98, top=0.92, wspace=0.17, hspace=0.17)
		
# 		ax.grid(alpha=0.4)  
# 		if  ((logG == 'log' and region == 'Generation') or (logC == 'log' and region == 'Core')):
# 			plt.yscale('log', basey=2)
# 		else:
# 			plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0), useOffset=0)

# 		# plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0))		
# 		fig.savefig('/home/flav/Mestrado/MHaJoL/data/out/plots/general_'+metric+'_'+region+'.png',  bbox_inches='tight')
# 		# plt.show()
# 		fig.clf()
# 		# plt.close()