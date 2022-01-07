//
//  evaluate.h
//  CPPTest
//
//  Created by Vickson on 24/08/2020.
//  Copyright © 2020 Mad Artists. All rights reserved.
//

#ifndef evaluate_h
#define evaluate_h

#include <string>

#include "types.h"

class Position;

namespace Eval {

constexpr Value Tempo = Value(20); // Must be visible to search

std::string trace(const Position& pos);

Value evaluate(const Position& pos);
}

#endif /* evaluate_h */
