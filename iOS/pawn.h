//
//  pawn.h
//  CPPTest
//
//  Created by Vickson on 24/08/2020.
//  Copyright © 2020 Mad Artists. All rights reserved.
//

#ifndef pawn_h
#define pawn_h

#include "misc.h"
#include "Position.h"
#include "types.h"

namespace Pawns {

/// Pawns::Entry contains various information about a pawn structure. A lookup
/// to the pawn hash table (performed by calling the probe function) returns a
/// pointer to an Entry object.

struct Entry {

  Score pawn_score(Color c) const { return scores[c]; }
  Bitboard pawn_attacks(Color c) const { return pawnAttacks[c]; }
  Bitboard passed_pawns(Color c) const { return passedPawns[c]; }
  Bitboard pawn_attacks_span(Color c) const { return pawnAttacksSpan[c]; }
  int weak_unopposed(Color c) const { return weakUnopposed[c]; }
  int pawn_asymmetry() const { return asymmetry; }
  int open_files() const { return openFiles; }

  int semiopen_file(Color c, File f) const {
    return semiopenFiles[c] & (1 << f);
  }

  int pawns_on_same_color_squares(Color c, Square s) const {
    return pawnsOnSquares[c][bool(DarkSquares & s)];
  }

  template<Color Us>
  Score king_safety(const Position& pos) {
    return  kingSquares[Us] == pos.square<KING>(Us) && castlingRights[Us] == pos.can_castle(Us)
          ? kingSafety[Us] : (kingSafety[Us] = do_king_safety<Us>(pos));
  }

  template<Color Us>
  Score do_king_safety(const Position& pos);

  template<Color Us>
  Value evaluate_shelter(const Position& pos, Square ksq);

  Key key;
  Score scores[COLOR_ALL];
  Bitboard passedPawns[COLOR_ALL];
  Bitboard pawnAttacks[COLOR_ALL];
  Bitboard pawnAttacksSpan[COLOR_ALL];
  Square kingSquares[COLOR_ALL];
  Score kingSafety[COLOR_ALL];
  int weakUnopposed[COLOR_ALL];
  int castlingRights[COLOR_ALL];
  int semiopenFiles[COLOR_ALL];
  int pawnsOnSquares[COLOR_ALL][COLOR_ALL]; // [color][light/dark squares]
  int asymmetry;
  int openFiles;
};

typedef HashTable<Entry, 16384> Table;

void init();
Entry* probe(const Position& pos);

} // namespace Pawns

#endif /* pawn_h */
