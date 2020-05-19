import mmap
import os
import struct
import bisect
from tqdm import tqdm
from collections import Counter
import matplotlib.pyplot as plt

#uniprot_fh = open("uniprot_trembl.fasta", "r")
index_fh = open("uniprot_trembl.fasta.idx", "rb")
#uniprot = mmap.mmap(uniprot_fh.fileno(), 0, mmap.MAP_PRIVATE, mmap.PROT_READ)
index = mmap.mmap(index_fh.fileno(), 0, mmap.MAP_PRIVATE, mmap.PROT_READ)

no_entries = len(index)//16
#print(no_entries)

def parse(i):
    return struct.unpack("dQ", i)[0]


vals = Counter()

b = index_fh.read(16)
pbar = tqdm(total = no_entries)
i = 0
while len(b) == 16:
    mass = parse(b)
    vals[int(mass)] += 1
    pbar.update()
    i += 1
    if i > no_entries:
        break
    b = index_fh.read(16)



mv = max(vals.keys())

X = list(range(mv))
Y = [vals[x] for x in range(mv)]

plt.plot(X, Y)
plt.show()



