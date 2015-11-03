
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

#include "Coach.h"

// ------------
// coach_eval
// ------------

Teams coach_eval (const int num_students, const Pairs& pairs)
{
  Teams teams;
  const int max_team_size = 3;
  const int max_num_teams = num_students / max_team_size;

  std::vector<int> sa (num_students + 1, -1);

  for (const Pair& pair : pairs)
  {
    int s1 = pair.first;
    int s2 = pair.second;
    int s1_assignment = sa[s1];
    int s2_assignment = sa[s2];

    // Neither assigned
    if (s1_assignment == -1 && s2_assignment == -1) {
      if (teams.size() == max_num_teams) {
	teams.clear();
	return teams;
      } else {
	teams.emplace_back();
	teams.back().emplace_back(s1);
	teams.back().emplace_back(s2);
	sa[s1] = teams.size() - 1;
	sa[s2] = teams.size() - 1;
      }
    }

    // Both already assigned
    else if (s1_assignment != -1 && s2_assignment != -1) {
      if (s1_assignment != s2_assignment) {
	teams.clear();
	return teams;
      }
    }

    // s1 assigned and s2 not assigned
    else if (s1_assignment != -1) {
      if (teams[s1_assignment].size() == max_team_size) {
	teams.clear();
	return teams;
      } else {
	teams[s1_assignment].emplace_back(s2);
	sa[s2] = s1_assignment;
      }
    }

    // s2 assigned and s1 not assigned
    else {
      if (teams[s2_assignment].size() == max_team_size) {
	teams.clear();
	return teams;
      } else {
	teams[s2_assignment].emplace_back(s1);
	sa[s1] = s2_assignment;
      }
    }
  }

  // Fill out remainder of team vector with empty teams
  for (int i = teams.size(); i < max_num_teams; ++i) {
    teams.emplace_back();
  }

  // Assign losers
  for (int i = 1; i <= sa.size(); ++i) {
    if (sa[i] == -1) {
      for (int j = 0; j < teams.size(); ++j) {
	if (teams[j].size() < max_team_size) {
	  teams[j].emplace_back(i);
	  sa[i] = j;
	  break;
	}
      }
    }
  }

  return teams;
}

// -------------
// coach_print
// -------------

void coach_print (std::ostream& w, const Teams& teams) {

  if (teams.size() == 0) {
    w << "-1" << std::endl;
  } else {
    for (const auto& team : teams) {
      assert (team.size() == 3);
      w << team[0] << " " << team[1] << " " << team[2] << std::endl;
    }
  }
}

// -------------
// coach_solve
// -------------

void coach_solve (std::istream& r, std::ostream& w) 
{
  std::string s;

  // Get the number of students and number of pair requests
  int num_students = 0;
  int num_pairs = 0;
  if (std::getline(r, s)) {
    std::istringstream sin(s);
    sin >> num_students >> num_pairs;
  }

  // Get the pairs of students who want to work together
  Pairs pairs;
  for (int i = 0; i < num_pairs; ++i) {
    if (std::getline(r, s)) {
      std::istringstream sin(s);
      int s1 = 0;
      int s2 = 0;
      sin >> s1 >> s2;
      pairs.emplace_back(s1, s2);
    }
  }

  // Determine the team assignments
  Teams teams = coach_eval(num_students, pairs);

  // Print the result
  coach_print(w, teams);
}
