import mmap
import os
import struct
import bisect


class FastaIndexer:
    '''Allows fast retrieval of proteins in a given mass range.'''

    def __init__(self, fasta_path, idx_path=None, treat_as_sorted=None):
        '''Construct a fast indexer retrieving proteins from FASTA by mass.

        The FASTA file should have been previously indexed and preferably sorted
        by the mkndex or mkindex_nosort tools. The mass of the protein is defined
        as the average mass over possible isotopic compositions, using Earth matter
        isotopic abundances.

        Note
        ----
        The optional parameters idx_path and treat_as_sorted must either be both
        provided or both omitted.

        Parameters
        ----------
        fasta_path : str
            Path to the (possibly sorted) FASTA file, containing the indexed proteins.
            If the other two arguments are not provided, some simple logic is used to
            try and find the sorted version if present
        idx_path : str, optional
            Path to the .fasta.idx file containing the index
        treat_as_sorted : bool, optional
            Whether to treat the FASTA file as sorted by mass (created by mkindex tool,
            rather than left as-is and only indexed by mkindex_nosorted)

        '''

        self.fasta_fh = None
        self.idx_fh = None
        self.idx = None

        if (idx_path is None) != (treat_as_sorted is None):
            raise Exception("Either all optional arguments must be provided, or none.")

        if idx_path is not None and treat_as_sorted is not None:
            self._do_setup(fasta_path, idx_path)
            self.sorted = treat_as_sorted
            return

        try:
            self._do_setup(fasta_path + ".sorted")
        except (OSError, IOError) as e:
            self._do_setup(fasta_path)

        self.sorted = self.fasta_path.endswith(".sorted")

    def __getitem__(self, idx):
        '''Returns the file offset at which ith protein starts in FASTA'''
        return self._idkey_at(idx)[0]

    def __len__(self):
        '''Number of proteins in the FASTA file'''
        return self.no_entries

    def search(self, start_mass, end_mass):
        '''Returns a generator of proteins within the given mass range.

        The generator returns a sequence of pairs (Python tuples): containing
        the header (FASTA protein name) and the sequence going over all proteins.

        Paramters
        ---------
        start_mass : float
            Lower end of masses of proteins that need to be returned
        end_mass : float
            Upper end of masses of proteins that need to be returned

        '''

        start_mass = max(start_mass, self.min_mass)
        end_mass = min(end_mass, self.max_mass)
        left_idx = bisect.bisect_left(self, start_mass)
        right_idx = bisect.bisect_right(self, end_mass)

        if right_idx == left_idx:
            return

        position = self._idkey_at(left_idx)[1]
        self.fasta_fh.seek(position, os.SEEK_SET)
        assert self.fasta_fh.read(1) == '>'

        for i in range(left_idx, right_idx):
            if not self.sorted:
                position = self._idkey_at(i)[1]
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

    def _do_setup(self, fasta_path, idx_path=None):
        self.fasta_fh = open(fasta_path)
        if idx_path is None:
            idx_path = fasta_path + ".idx"
        self.idx_fh = open(idx_path)
        self.idx = mmap.mmap(self.idx_fh.fileno(), 0, access=mmap.ACCESS_READ)
        self.fasta_path = fasta_path
        self.idx_path = idx_path
        self.no_entries = len(self.idx) // 16
        self.min_mass = self._idkey_at(0)[0]
        self.max_mass = self._idkey_at(self.no_entries-1)[0]
        try:
            self.idx.madvise(mmap.MADV_RANDOM)
        except Exception:
            pass

    def _idkey_at(self, i):
        return struct.unpack("dQ", self.idx[i*16:i*16+16])

    def __del__(self):
        if self.idx is not None:
            self.idx.close()
        if self.fasta_fh is not None:
            self.fasta_fh.close()
        if self.idx_fh is not None:
            self.idx_fh.close()



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
