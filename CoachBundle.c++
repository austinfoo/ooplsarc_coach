
#ifndef LongPath_h
#define LongPath_h

// --------
// includes
// --------

#include <iostream>
#include <string>
#include <vector>

// ------------
// longpath_eval
// ------------

int longpath_eval (const int num_rooms, const std::vector<int>& taken_rooms);

// -------------
// longpath_print
// -------------

void longpath_print (std::ostream& w, int num_portals);

// -------------
// longpath_solve
// -------------

void longpath_solve (std::istream& r, std::ostream& w);

#endif

// ----------------------------
// projects/twobuttons/Twobuttons.c++
// Copyright (C) 2015
// Glenn P. Downing
// ----------------------------

// --------
// includes
// --------

#include <cassert>  // assert
#include <iostream> // endl, istream, ostream
#include <sstream>  // istringstream
#include <string>   // getline, string
#include <utility>
#include <cstdint>
#include <algorithm>
#include <vector>
#include <list>
#include <limits>



// ------------
// longpath_eval
// ------------


// Observation
// If we get to room I along the forward portal, then all rooms 1 to I-1 must
// be even.  For example, to get from room 1 to room 4:
//   To move from room 1 to room 2, room 1 must be even
//   To move from room 2 to room 3, room 2 must be even
//   To move from room 3 to room 4, room 3 must be even

int longpath_eval (const int num_rooms, const std::vector<int>& taken_rooms)
{
  // Each element in num_portals is the number of portals required to traverse
  // from room I to room I+1 when all the rooms are initially even.
  std::vector<uint64_t> num_portals (num_rooms+1);

  // Seed element 1 in num_portals with 2 since this must always be the case
  // regardless of the configuration.
  num_portals[1] = 2;

  // The magic modulus value specified in the problem description.
  const uint64_t magic_mod_val = 1000000007;

  // Compute elements 2 to N in num_portals
  for (int i = 2; i <= num_rooms; ++i) {
    // Start at room I
    int current_room = i;
    uint64_t current_portals = 0;
    // Since room I is initially even it takes one step to move backwards along
    // the taken path to room J.  Note that room I is now odd.
    current_room = taken_rooms[current_room];
    ++current_portals;
    // Since room J to I-1 are even, just add up the previously computed number
    // of portals to move forward from J to I.  Note that when we finish this
    // loop that the current room is now back to I.
    while (current_room < i) {
      current_portals += num_portals[current_room];
      ++current_room;
    }
    // We are in room I and it is odd.  It takes one forward step to move from
    // I to I+1.
    ++current_room;
    ++current_portals;
    // We are done.  Save the answer.
    num_portals[i] = current_portals % magic_mod_val;
  }

  // Now sum the full number of steps to get from room 1 to room N.
  uint64_t total = 0;
  for (int i = 1; i <= num_rooms; ++i) {
    total += num_portals[i];
  }
  total = total % magic_mod_val;

  return total;
}

// -------------
// longpath_print
// -------------

void longpath_print (std::ostream& w, int num_portals) {
  w << num_portals << std::endl;
}

// -------------
// longpath_solve
// -------------

void longpath_solve (std::istream& r, std::ostream& w) 
{
  std::string s;

  // Get the number of rooms
  int num_rooms = 0;
  if (std::getline(r, s)) {
    std::istringstream sin(s);
    sin >> num_rooms;
  }

  // Get the taken rooms
  std::vector<int> taken_rooms (num_rooms+1);
  if (std::getline(r, s)) {
    std::istringstream sin(s);
    for (int i = 1; i <= num_rooms; ++i) {
      sin >> taken_rooms[i];
    }
  }

  // Determine the number of pieces
  int num_portals = longpath_eval(num_rooms, taken_rooms);

  // Print the result
  longpath_print(w, num_portals);
}

#include <iostream>



// ----
// main
// ----

int main () {
    using namespace std;
    longpath_solve(cin, cout);
    return 0;
}
