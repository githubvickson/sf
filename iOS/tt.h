//
//  tt.h
//  CPPTest
//
//  Created by Vickson on 24/08/2020.
//  Copyright © 2020 Mad Artists. All rights reserved.
//

#ifndef tt_h
#define tt_h

#include "misc.h"
#include "types.h"

/// TTEntry struct is the 10 bytes transposition table entry, defined as below:
///
/// key        16 bit
/// move       16 bit
/// value      16 bit
/// eval value 16 bit
/// generation  6 bit
/// bound type  2 bit
/// depth       8 bit

struct TTEntry {

  Move  move()  const { return (Move )move16; }
  Value value() const { return (Value)value16; }
  Value eval()  const { return (Value)eval16; }
  Depth depth() const { return (Depth)(depth8 * int(ONE_PLY)); }
  Bound bound() const { return (Bound)(genBound8 & 0x3); }
  void save(Key k, Value v, Bound b, Depth d, Move m, Value ev);

private:
  friend class TranspositionTable;

  uint16_t key16;
  uint16_t move16;
  int16_t  value16;
  int16_t  eval16;
  uint8_t  genBound8;
  int8_t   depth8;
};


/// A TranspositionTable consists of a power of 2 number of clusters and each
/// cluster consists of ClusterSize number of TTEntry. Each non-empty entry
/// contains information of exactly one position. The size of a cluster should
/// divide the size of a cache line size, to ensure that clusters never cross
/// cache lines. This ensures best cache performance, as the cacheline is
/// prefetched, as soon as possible.

class TranspositionTable {

  static constexpr int CacheLineSize = 64;
  static constexpr int ClusterSize = 3;

  struct Cluster {
    TTEntry entry[ClusterSize];
    char padding[2]; // Align to a divisor of the cache line size
  };

  static_assert(CacheLineSize % sizeof(Cluster) == 0, "Cluster size incorrect");

public:
 ~TranspositionTable() { free(mem); }
  void new_search() { generation8 += 4; } // Lower 2 bits are used by Bound
  TTEntry* probe(const Key key, bool& found) const;
  int hashfull() const;
  void resize(size_t mbSize);
  void clear();

  // The 32 lowest order bits of the key are used to get the index of the cluster
  TTEntry* first_entry(const Key key) const {
    return &table[(uint32_t(key) * uint64_t(clusterCount)) >> 32].entry[0];
  }

private:
  friend struct TTEntry;

  size_t clusterCount;
  Cluster* table;
  void* mem;
  uint8_t generation8; // Size must be not bigger than TTEntry::genBound8
};

extern TranspositionTable TT;


#endif /* tt_h */
