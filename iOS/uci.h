//
//  uci.h
//  CPPTest
//
//  Created by Vickson on 24/08/2020.
//  Copyright © 2020 Mad Artists. All rights reserved.
//

#ifndef uci_h
#define uci_h

#include <map>
#include <string>

#include "types.h"
#include "Position.h"

class Position;

namespace UCI {

class Option;

/// Custom comparator because UCI options should be case insensitive
struct CaseInsensitiveLess {
  bool operator() (const std::string&, const std::string&) const;
};

/// Our options container is actually a std::map
typedef std::map<std::string, Option, CaseInsensitiveLess> OptionsMap;

/// Option class implements an option as defined by UCI protocol
class Option {

  typedef void (*OnChange)(const Option&);

public:
  Option(OnChange = nullptr);
  Option(bool v, OnChange = nullptr);
  Option(const char* v, OnChange = nullptr);
  Option(double v, int minv, int maxv, OnChange = nullptr);
  Option(const char* v, const char* cur, OnChange = nullptr);

  Option& operator=(const std::string&);
  void operator<<(const Option&);
  operator double() const;
  operator std::string() const;
  bool operator==(const char*) const;

private:
  friend std::ostream& operator<<(std::ostream&, const OptionsMap&);

  std::string defaultValue, currentValue, type;
  int min, max;
  size_t idx;
  OnChange on_change;
};

void init(OptionsMap&, double skill, double time);
void init(Position &pos, StateListPtr &states);
void init(Position &pos, StateListPtr &states, const char* customFEN);

Move think(Position& pos, std::deque<Move> &moveHistory);
void undo_move(Position& pos, std::deque<Move> &moveHistory);
void new_game(Position &pos, StateListPtr &states, std::deque<Move> &moveHistory);
void set_position(Position& pos, StateListPtr& states, std::string fen);
void pos_possible_moves(const Position &pos, int &size, int *mPointer);
void release_resources(Position &pos);
std::string value(Value v);
std::string square(Square s);
std::string move(Move m, bool chess960);
std::string pv(const Position& pos, Depth depth, Value alpha, Value beta);
Move to_move(const Position& pos, std::string& str);
bool is_game_draw(Position &pos);

//call from swift
void init_move(int from, int to, Position &pos, std::deque<Move> &moveHistory);
void castle_move(int castleSide, Position &pos, std::deque<Move> &moveHistory);
void enpassant_move(int from, Position &pos, std::deque<Move> &moveHistory);
void promotion_move(int from, int to, int PromoType, Position &pos, std::deque<Move> &moveHistory);

//Testing purpose
int fivety_move_rule_count(Position &pos);
bool all_possible_moves_match(Position &pos, const int* from, const int* to, const int count);
bool all_possible_moves_match_inverse(Position &pos, const int* from, const int* to, const int count);
} // namespace UCI

extern UCI::OptionsMap Options;


#endif /* uci_h */
