//
//  endgame.h
//  CPPTest
//
//  Created by Vickson on 24/08/2020.
//  Copyright © 2020 Mad Artists. All rights reserved.
//

#ifndef endgame_h
#define endgame_h

#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>

#include "Position.h"
#include "types.h"


/// EndgameCode lists all supported endgame functions by corresponding codes

enum EndgameCode {

  EVALUATION_FUNCTIONS,
  KNNK,  // KNN vs K
  KXK,   // Generic "mate lone king" eval
  KBNK,  // KBN vs K
  KPK,   // KP vs K
  KRKP,  // KR vs KP
  KRKB,  // KR vs KB
  KRKN,  // KR vs KN
  KQKP,  // KQ vs KP
  KQKR,  // KQ vs KR

  SCALING_FUNCTIONS,
  KBPsK,   // KB and pawns vs K
  KQKRPs,  // KQ vs KR and pawns
  KRPKR,   // KRP vs KR
  KRPKB,   // KRP vs KB
  KRPPKRP, // KRPP vs KRP
  KPsK,    // K and pawns vs K
  KBPKB,   // KBP vs KB
  KBPPKB,  // KBPP vs KB
  KBPKN,   // KBP vs KN
  KNPK,    // KNP vs K
  KNPKB,   // KNP vs KB
  KPKP     // KP vs KP
};


/// Endgame functions can be of two types depending on whether they return a
/// Value or a ScaleFactor.

template<EndgameCode E> using
eg_type = typename std::conditional<(E < SCALING_FUNCTIONS), Value, ScaleFactor>::type;


/// Base and derived functors for endgame evaluation and scaling functions

template<typename T>
struct EndgameBase {

  explicit EndgameBase(Color c) : strongSide(c), weakSide(~c) {}
  virtual ~EndgameBase() = default;
  virtual T operator()(const Position&) const = 0;

  const Color strongSide, weakSide;
};


template<EndgameCode E, typename T = eg_type<E>>
struct Endgame : public EndgameBase<T> {

  explicit Endgame(Color c) : EndgameBase<T>(c) {}
  T operator()(const Position&) const override;
};


/// The Endgames class stores the pointers to endgame evaluation and scaling
/// base objects in two std::map. We use polymorphism to invoke the actual
/// endgame function by calling its virtual operator().

class Endgames {

  template<typename T> using Ptr = std::unique_ptr<EndgameBase<T>>;
  template<typename T> using Map = std::map<Key, Ptr<T>>;

  template<typename T>
  Map<T>& map() {
    return std::get<std::is_same<T, ScaleFactor>::value>(maps);
  }

  template<EndgameCode E, typename T = eg_type<E>>
  void add(const std::string& code) {

    StateInfo st;
    map<T>()[Position().set(code, WHITE, &st).material_key()] = Ptr<T>(new Endgame<E>(WHITE));
    map<T>()[Position().set(code, BLACK, &st).material_key()] = Ptr<T>(new Endgame<E>(BLACK));
  }

  std::pair<Map<Value>, Map<ScaleFactor>> maps;

public:
  Endgames() {

    add<KPK>("KPK");
    add<KNNK>("KNNK");
    add<KBNK>("KBNK");
    add<KRKP>("KRKP");
    add<KRKB>("KRKB");
    add<KRKN>("KRKN");
    add<KQKP>("KQKP");
    add<KQKR>("KQKR");

    add<KNPK>("KNPK");
    add<KNPKB>("KNPKB");
    add<KRPKR>("KRPKR");
    add<KRPKB>("KRPKB");
    add<KBPKB>("KBPKB");
    add<KBPKN>("KBPKN");
    add<KBPPKB>("KBPPKB");
    add<KRPPKRP>("KRPPKRP");
  }

  template<typename T>
  const EndgameBase<T>* probe(Key key) {
    return map<T>().count(key) ? map<T>()[key].get() : nullptr;
  }
};

#endif /* endgame_h */
