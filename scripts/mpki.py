import numpy as np
import pandas as pd
#
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

try:
    # for Python2
    from Tkinter import *   ## notice capitalized T in Tkinter 
except ImportError:
    # for Python3
    from tkinter import *   ## notice lowercase 't' in tkinter here

df=pd.read_csv('/home/flav/Mestrado/MHaJoL/data/results.csv', sep=',', encoding='utf-8-sig')

df = df.loc[(df['grp'] == 'L2CACHE') & (df['region'] == 'Core') & ((df['metric'] == 'L2_RQSTS_MISS') | (df['metric'] == 'INSTR_RETIRED_ANY'))]

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

dfAggAVG = dfIQR.groupby(['grp', 'metric', 'method', 'region', 'selectivity'], as_index = False).aggregate(['mean'])

dfInstr = dfAggAVG.xs('INSTR_RETIRED_ANY', level='metric').rename(index=str, columns={"value": "instr"}).stack()
dfMiss = dfAggAVG.xs('L2_RQSTS_MISS', level='metric').rename(index=str, columns={"value": "miss"}).stack()
dfMPKI = pd.merge(dfInstr, dfMiss, how='inner', on=['grp', 'method', 'region', 'selectivity'])

dfMPKI['MPKI'] = (dfMPKI['miss']*1000)/dfMPKI['instr']

print(dfMPKI.head(100))

# dfMPKI = 

# print(dfAggAVG.head())

# group="L2CACHE"
# metric1="L2_RQSTS_MISS"
# metric2="INSTR_RETIRED_ANY"
# region="Core"

# dfFilter = dfAggAVG.xs(group, level='grp').xs(metric, level='metric').xs(region, level='region')

n_groups = 9
		
index = np.arange(n_groups)
bar_width = 0.35

opacity = 0.4

dfMethodVec = dfMPKI.xs("vecCuckoo", level='method').reset_index().sort_values('selectivity')
dfValuesVec = dfMethodVec["MPKI"]
print(dfValuesVec.head())
dfMethod = dfMPKI.xs("cuckoo", level='method').reset_index().sort_values('selectivity')
dfValues = dfMethod["MPKI"]

plt.style.use('ggplot')

plt.rcParams.update({'font.size': 14})

fig, ax = plt.subplots()

rects1 = ax.bar(index, dfValues, bar_width, alpha=opacity,  label='Scalar')#color='steelblue',
rects2 = ax.bar(index + bar_width, dfValuesVec, bar_width, alpha=opacity, label='ViViD')#color='deeppink', 

ax.yaxis.set_major_formatter(ticker.ScalarFormatter(useMathText=True))

ax.set_xlabel('Selectivity')
ax.set_ylabel('Misses per Kilo Instructions')
ax.set_xticks(index + bar_width / 2)
ax.set_xticklabels(('10', '20', '30', '40', '50', '60', '70', '80', '90'))
ax.legend()

ax.grid(alpha=0.4)  

fig.tight_layout()

# plt.figure(figsize=(4.8,2))
fig.subplots_adjust(left=0.19, bottom=0.09, right=0.98, top=0.92, wspace=0.17, hspace=0.17)

# plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0), useOffset=0)

fig.savefig('/home/flav/Mestrado/MHaJoL/data/out/plots/L3CACHE_L2MPKI_Core.png',  bbox_inches='tight')
# fig.show()
fig.clf()
# print('1')
# # plt.close()
