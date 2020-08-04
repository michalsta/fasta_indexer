import mmap
import os
import struct
import bisect

uniprot_fh = open("uniprot_trembl.fasta.sorted", "r")
index_fh = open("uniprot_trembl.fasta.sorted.idx", "rb")
index = mmap.mmap(index_fh.fileno(), 0, access = mmap.ACCESS_READ)

no_entries = len(index)//16
#print(no_entries)

def idkey_at(i):
    return struct.unpack("dQ", index[i*16:i*16+16])

min_mass = idkey_at(0)[0]
max_mass = idkey_at(no_entries-1)[0]

#print(idkey_at(2))

class ArrWrap:
    def __init__(self):
        pass
    def __getitem__(self, idx):
        return idkey_at(idx)[0]
    def __len__(self):
        return no_entries

def search(start_mass, end_mass):
    x = ArrWrap()
    start_mass = max(start_mass, min_mass)
    end_mass = min(end_mass, max_mass)
    left_idx = bisect.bisect_left(x, start_mass)
    right_idx = bisect.bisect_right(x, end_mass)

    print(str(right_idx - left_idx) + " results found, out of " + str(no_entries) + " in database.")

    for i in range(left_idx, right_idx):
        position = idkey_at(i)[1]
        uniprot_fh.seek(position, os.SEEK_SET)
        header = uniprot_fh.readline()[:-1]
        assert header[0] == '>'
        seq = []
        l = uniprot_fh.readline()[:-1]
        while len(l) > 0 and l[0] != '>':
            seq.append(l)
            l = uniprot_fh.readline()[:-1]
        seq = ''.join(seq)
        yield(header, seq)


if __name__ == "__main__":
    import sys
    mid = float(sys.argv[1])
    tol = float(sys.argv[2])
    mass_start = mid - tol
    mass_end = mid + tol
    for hdr, seq in search(mass_start, mass_end):
        print(hdr)
        while len(seq) > 0:
            # Inefficient, but it's only for testing...
            print(seq[:60])
            seq = seq[60:]
