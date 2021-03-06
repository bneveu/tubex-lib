/** 
 *  Slice class (polygons)
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_Slice.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  Interval yilb(const Interval& t, const Slice& x, const Slice& v)
  {
    return x.input_gate().lb() + v.codomain().lb() * (t - x.domain().lb());
  }

  Interval yilb_inv(const Interval& y, const Slice& x, const Slice& v)
  {
    return ((y - x.input_gate().lb()) / v.codomain().lb()) + x.domain().lb();
  }

  Interval yiub(const Interval& t, const Slice& x, const Slice& v)
  {
    return x.input_gate().ub() + v.codomain().ub() * (t - x.domain().lb());
  }

  Interval yiub_inv(const Interval& y, const Slice& x, const Slice& v)
  {
    return ((y - x.input_gate().ub()) / v.codomain().ub()) + x.domain().lb();
  }

  Interval yolb(const Interval& t, const Slice& x, const Slice& v)
  {
    return x.output_gate().lb() + v.codomain().ub() * (t - x.domain().ub());
  }

  Interval yolb_inv(const Interval& y, const Slice& x, const Slice& v)
  {
    return ((y - x.output_gate().lb()) / v.codomain().ub()) + x.domain().ub();
  }

  Interval youb(const Interval& t, const Slice& x, const Slice& v)
  {
    return x.output_gate().ub() + v.codomain().lb() * (t - x.domain().ub());
  }

  Interval youb_inv(const Interval& y, const Slice& x, const Slice& v)
  {
    return ((y - x.output_gate().ub()) / v.codomain().lb()) + x.domain().ub();
  }

  Interval ylb_inv(const Interval& y, const Slice& x)
  {
    if(x.input_gate().lb() == x.output_gate().lb())
      return Interval::ALL_REALS;
    return x.domain().lb() + (y - x.input_gate().lb()) / ((x.output_gate().lb() - x.input_gate().lb()) / (x.domain().diam()));
  }

  Interval yub_inv(const Interval& y, const Slice& x)
  {
    if(x.input_gate().ub() == x.output_gate().ub())
      return Interval::ALL_REALS;
    return x.domain().lb() + (y - x.input_gate().ub()) / ((x.output_gate().ub() - x.input_gate().ub()) / (x.domain().diam()));
  }

  Interval lines_intersection_ub(const Slice& x, const Slice& v)
  {
    return (x.output_gate().ub() - x.input_gate().ub()
            + v.codomain().ub() * x.domain().lb()
            - v.codomain().lb() * x.domain().ub()) / v.codomain().diam();
  }

  Interval lines_intersection_lb(const Slice& x, const Slice& v)
  {
    return (x.input_gate().lb() - x.output_gate().lb()
            + v.codomain().ub() * x.domain().ub()
            - v.codomain().lb() * x.domain().lb()) / v.codomain().diam();
  }

  const ConvexPolygon Slice::polygon(const Slice& v) const
  {
    assert(domain() == v.domain());

    Interval t = domain();
    assert(!t.is_degenerated());
    
    if(t.is_empty() || codomain().is_empty())
      return ConvexPolygon(); // empty polygon

    else if(v.codomain() == Interval::ALL_REALS)
    {
      IntervalVector box(2);
      box[0] = domain();
      box[1] = codomain();
      return ConvexPolygon(box);
    }

    else
    {
      vector<Point> v_pts;
      v_pts.push_back(Point(t.lb(), input_gate().lb()));

      // Lower bounds

        if(!v.codomain().is_degenerated())
        {
          Interval t_inter_lb, y_inter_lb;

          if(v.codomain().lb() == NEG_INFINITY)
            t_inter_lb = t.lb();

          else if(v.codomain().ub() == POS_INFINITY)
            t_inter_lb = t.ub();

          else
            t_inter_lb = lines_intersection_lb(*this, v);

          if(t_inter_lb.lb() >= t.lb() && t_inter_lb.ub() <= t.ub())
          {
            y_inter_lb = yolb(t_inter_lb, *this, v) | yilb(t_inter_lb, *this, v);

            if(y_inter_lb.ub() >= codomain().lb())
              v_pts.push_back(Point(t_inter_lb, y_inter_lb));

            else
            {
              Interval t_a = yilb_inv(codomain().lb(), *this, v);
              v_pts.push_back(Point(t_a.lb(), codomain().lb()));
              Interval t_b = yolb_inv(codomain().lb(), *this, v);
              v_pts.push_back(Point(t_b.ub(), codomain().lb()));
            }
          }
        }

        v_pts.push_back(Point(t.ub(), output_gate().lb()));

      // Upper bounds

        v_pts.push_back(Point(t.ub(), output_gate().ub()));

        if(!v.codomain().is_degenerated())
        {
          Interval t_inter_ub, y_inter_ub;

          if(v.codomain().lb() == NEG_INFINITY)
            t_inter_ub = t.ub();

          else if(v.codomain().ub() == POS_INFINITY)
            t_inter_ub = t.lb();

          else
            t_inter_ub = lines_intersection_ub(*this, v);

          if(t_inter_ub.lb() >= t.lb() && t_inter_ub.ub() <= t.ub())
          {
            y_inter_ub = youb(t_inter_ub, *this, v) | yiub(t_inter_ub, *this, v);

            if(y_inter_ub.lb() <= codomain().ub())
              v_pts.push_back(Point(t_inter_ub, y_inter_ub));

            else
            {
              Interval t_b = youb_inv(codomain().ub(), *this, v);
              v_pts.push_back(Point(t_b.ub(), codomain().ub()));
              Interval t_a = yiub_inv(codomain().ub(), *this, v);
              v_pts.push_back(Point(t_a.lb(), codomain().ub()));
            }
          }
        }
      
      v_pts.push_back(Point(t.lb(), input_gate().ub()));
      return ConvexPolygon(v_pts, true);
    }
  }
}