//
//  thread.h
//  CPPTest
//
//  Created by Vickson on 24/08/2020.
//  Copyright © 2020 Mad Artists. All rights reserved.
//

#ifndef thread_h
#define thread_h

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>
#include <future>

#include "material.h"
#include "movepick.h"
#include "pawn.h"
#include "Position.h"
#include "search.h"
#include "thread_win32.h"


/// Thread class keeps together all the thread-related stuff. We use
/// per-thread pawn and material hash tables so that once we get a
/// pointer to an entry its life time is unlimited and we don't have
/// to care about someone changing the entry under our feet.

class Thread {

  Mutex mutex;
  ConditionVariable cv;
  size_t idx;
  bool exit = false, searching = true; // Set before starting std::thread
  std::thread stdThread;

public:
  explicit Thread(size_t);
  virtual ~Thread();
  virtual void search();
  void clear();
  void idle_loop();
  void start_searching();
  void wait_for_search_finished();

  Pawns::Table pawnsTable;
  Material::Table materialTable;
  Endgames endgames;
  size_t pvIdx, pvLast;
  int selDepth, nmpMinPly;
  Color nmpColor;
  std::atomic<uint64_t> nodes, tbHits;

  Position rootPos;
  Search::RootMoves rootMoves;
  Depth rootDepth, completedDepth;
  CounterMoveHistory counterMoves;
  ButterflyHistory mainHistory;
  CapturePieceToHistory captureHistory;
  ContinuationHistory continuationHistory;
  Score contempt;
};


/// MainThread is a derived class specific for main thread

struct MainThread : public Thread {

  using Thread::Thread;
  std::promise<Move> pMove;
    
//  MainThread(size_t t) : Thread(t) { }
  void search() override;
//    void search(std::promise<Move> &pMove);
  void check_time();

  double bestMoveChanges, previousTimeReduction;
  Value previousScore;
  int callsCnt;
};


/// ThreadPool struct handles all the threads-related stuff like init, starting,
/// parking and, most importantly, launching a thread. All the access to threads
/// is done through this class.

struct ThreadPool : public std::vector<Thread*> {

  void start_thinking(Position&, StateListPtr&, const Search::LimitsType&, bool = false);
  void think(Position &pos, const Search::LimitsType& limits, bool ponderMode);
  void setup(Position& pos, StateListPtr& states);
  void do_move(Move m, Position &pos);
  void undo_move(Move m);
  void clear();
  void set(size_t);

  MainThread* main()        const { return static_cast<MainThread*>(front()); }
  uint64_t nodes_searched() const { return accumulate(&Thread::nodes); }
  uint64_t tb_hits()        const { return accumulate(&Thread::tbHits); }

  std::atomic_bool stop, ponder, stopOnPonderhit;

private:
  uint64_t accumulate(std::atomic<uint64_t> Thread::* member) const {

    uint64_t sum = 0;
    for (Thread* th : *this)
        sum += (th->*member).load(std::memory_order_relaxed);
    return sum;
  }
};

extern ThreadPool Threads;


#endif /* thread_h */
