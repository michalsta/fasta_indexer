import mmap
import os
import struct
import bisect


class FastaIndexer:
    def __init__(self, fasta_path, idx_path = None, treat_as_sorted = None):

        if (idx_path is None) != (treat_as_sorted is None):
            raise Exception("Either all optional arguments must be provided, or none.")

        if idx_path is not None and treat_as_sorted is not None:
            do_setup(fasta_path, idx_path)
            self.sorted = treat_as_sorted
            return

        try:
            do_setup(fasta_path + ".sorted")
        except (OSError, IOError) as e:
            do_setup(fasta_path)

        self.sorted = self.fasta_path.endswith(".sorted")



    def do_setup(self, fasta_path, idx_path = None):
        self.fasta_fh = open(sorted_fp)
        if idx_path is None:
            idx_path = fasta_path + ".idx"
        self.idx_fh = open(idx_path)
        self.idx = mmap.mmap(index_fh.fileno(), 0, access = mmap.ACCESS_READ)
        self.fasta_path = fasta_path
        self.idx_path = idx_path
        self.no_entries = len(self.idx) // 16
        self.min_mass = self.idkey_at(0)[0]
        self.max_mass = self.idkey_at(no_entries-1)[0]
        try:
            self.idx.madvise(mmap.MADV_RANDOM)
        except Exception:
            pass
        
    def idkey_at(self, i):
        return struct.unpack("dQ", self.idx[i*16:i*16+16])

    def __getitem__(self, idx):
        return self.idkey_at(idx)[0]

    def __len__(self):
        return self.no_entries

    def search(self, start_mass, end_mass):
        start_mass = max(start_mass, self.min_mass)
        end_mass = min(end_mass, self.max_mass)
        left_idx = bisect.bisect_left(self, start_mass)
        right_idx = bisect.bisect_right(self, end_mass)

        print(str(right_idx - left_idx) + " results found, out of " + str(self.no_entries) + " in database.")

        if right_idx == left_idx:
            return

        position = idkey_at(i)[1]
        uniprot_fh.seek(position, os.SEEK_SET)

        for i in range(left_idx, right_idx):
            position = idkey_at(i)[1]
            if not self.treat_as_sorted:
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
        

'''
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
'''

if __name__ == "__main__":
    import sys
    fasta = sys.argv[1]
    mid = float(sys.argv[2])
    tol = float(sys.argv[3])
    mass_start = mid - tol
    mass_end = mid + tol
    for hdr, seq in search(mass_start, mass_end):
        print(hdr)
        while len(seq) > 0:
            # Inefficient, but it's only for testing...
            print(seq[:60])
            seq = seq[60:]
