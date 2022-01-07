//
//  timem.h
//  CPPTest
//
//  Created by Vickson on 24/08/2020.
//  Copyright © 2020 Mad Artists. All rights reserved.
//

#ifndef timem_h
#define timem_h

#include "misc.h"
#include "search.h"
#include "thread.h"

/// The TimeManagement class computes the optimal time to think depending on
/// the maximum available time, the game move number and other parameters.

class TimeManagement {
public:
  void init(Search::LimitsType& limits, Color us, int ply);
  TimePoint optimum() const { return optimumTime; }
  TimePoint maximum() const { return maximumTime; }
  TimePoint elapsed() const { return Search::Limits.npmsec ?
                                     TimePoint(Threads.nodes_searched()) : now() - startTime; }

  int64_t availableNodes; // When in 'nodes as time' mode

private:
  TimePoint startTime;
  TimePoint optimumTime;
  TimePoint maximumTime;
};

extern TimeManagement Time;

#endif /* timem_h */
