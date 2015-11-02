
#ifndef Coach_h
#define Coach_h

// --------
// includes
// --------

#include <iostream>
#include <string>
#include <vector>
#include <utility>

using Pair = std::pair<int, int>;
using Pairs = std::vector<Pair>;
using Teams = std::vector<std::vector<int>>;

// ------------
// coach_eval
// ------------

Teams coach_eval (const int num_students, const Pairs& pairs);

// -------------
// coach_print
// -------------

void coach_print (std::ostream& w, const Teams& teams);

// -------------
// coach_solve
// -------------

void coach_solve (std::istream& r, std::ostream& w);

#endif
