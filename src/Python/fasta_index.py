import mmap
import os
import struct
import bisect


class FastaIndexer:
    def __init__(self, fasta_path, idx_path = None, treat_as_sorted = None):

        if (idx_path is None) != (treat_as_sorted is None):
            raise Exception("Either all optional arguments must be provided, or none.")

        if idx_path is not None and treat_as_sorted is not None:
            self.do_setup(fasta_path, idx_path)
            self.sorted = treat_as_sorted
            return

        try:
            self.do_setup(fasta_path + ".sorted")
        except (OSError, IOError) as e:
            self.do_setup(fasta_path)

        self.sorted = self.fasta_path.endswith(".sorted")



    def do_setup(self, fasta_path, idx_path = None):
        self.fasta_fh = open(fasta_path)
        if idx_path is None:
            idx_path = fasta_path + ".idx"
        self.idx_fh = open(idx_path)
        self.idx = mmap.mmap(self.idx_fh.fileno(), 0, access = mmap.ACCESS_READ)
        self.fasta_path = fasta_path
        self.idx_path = idx_path
        self.no_entries = len(self.idx) // 16
        self.min_mass = self.idkey_at(0)[0]
        self.max_mass = self.idkey_at(self.no_entries-1)[0]
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

        position = self.idkey_at(left_idx)[1]
        self.fasta_fh.seek(position, os.SEEK_SET)
        assert self.fasta_fh.read(1) == '>'

        for i in range(left_idx, right_idx):
            if not self.sorted:
                position = self.idkey_at(i)[1]
                self.fasta_fh.seek(position, os.SEEK_SET)
                assert self.fasta_fh.read() == '>'
            header = self.fasta_fh.readline()[:-1]
            print("hdr:", header)
            seq = []
            c = self.fasta_fh.read(1)
            while len(c) > 0 and c != '>':
                if c != '\n':
                    seq.append(c)
                c = self.fasta_fh.read(1)
            seq = ''.join(seq)
            yield(header, seq)
            if len(c) == 0:
                return
        

if __name__ == "__main__":
    import sys
    fasta = sys.argv[1]
    mid = float(sys.argv[2])
    tol = float(sys.argv[3])
    mass_start = mid - tol
    mass_end = mid + tol
    for hdr, seq in FastaIndexer(fasta).search(mass_start, mass_end):
        print(hdr)
        while len(seq) > 0:
            # Inefficient, but it's only for testing...
            print(seq[:60])
            seq = seq[60:]
