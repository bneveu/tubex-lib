/** 
 *  CtcConstellation class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <list>
#include "tubex_CtcConstellation.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcConstellation::CtcConstellation(const vector<Beacon>& map)
    : ibex::Ctc(2), m_map(map)
  {
    // todo: binary tree for logarithmic complexity
  }

  CtcConstellation::~CtcConstellation()
  {

  }

  void CtcConstellation::contract(IntervalVector &beacon_box)
  {
    assert(beacon_box.size() == 2);
    IntervalVector envelope_beacons(2, Interval::EMPTY_SET);
    for(int i = 0 ; i < m_map.size() ; i++)
      if(beacon_box.contains(m_map[i].pos().subvector(0,1)))
        envelope_beacons |= m_map[i].pos().subvector(0,1);
    beacon_box = envelope_beacons;
  }
}