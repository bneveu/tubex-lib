#include "tests.h"
#include <cstdio>
#include "tubex_VIBesFigTube.h"
#include "vibes.h"

#define protected public     // Using #define so that we can access protected
#include "tubex_CtcPicard.h" // methods of CtcPicard for tests purposes

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

#define VIBES_DRAWING 0

TEST_CASE("CtcPicard")
{
  SECTION("Test CtcPicard, eval base")
  {
    CtcPicard ctc_picard(1.1);

    Interval t(0.5,1.0);
    TubeVector tube(t, 1);
    tube.set(IntervalVector(1, 1.5), t.lb());
    TubeVector tube_raw = tube;
    Slice *slice;

    tube = tube_raw;
    tubex::Function f2("x", "3.");
    Interval dtest = tube[0].slice_domain(0);
    IntervalVector test = f2.eval_vector(dtest, tube);
    ctc_picard.guess_kth_slices_envelope(f2, tube, 0, FORWARD);
    CHECK(tube(0).is_superset(1.5 + Interval(0.,0.5) * 3.));

    tube = tube_raw;
    tubex::Function f3("x", "t");
    ctc_picard.guess_kth_slices_envelope(f3, tube, 0, FORWARD);
    CHECK(tube(0).is_superset(1.5 + Interval(0.,0.5) * t));

    tube = tube_raw;
    f3 = tubex::Function("x", "t^2"); // with operator= of class Function
    ctc_picard.guess_kth_slices_envelope(f3, tube, 0, FORWARD);
    CHECK(tube(0).is_superset(1.5 + Interval(0.,0.5) * t * t));

    tube = tube_raw;
    tubex::Function f5("x", "x");
    ctc_picard.guess_kth_slices_envelope(f5, tube, 0, FORWARD);
    CHECK(tube(0).is_superset(1.5 + Interval(0.,0.5) * Interval(1.,2.)));

    tube = tube_raw;
    tubex::Function f6("x", "x*t");
    ctc_picard.guess_kth_slices_envelope(f6, tube, 0, FORWARD);
    CHECK(tube(0).is_superset(1.5 + Interval(0.,0.5) * Interval(1.,2.) * t));

    tube = tube_raw;
    tube.set(IntervalVector(1, 2.*atan(exp(-t.lb())*tan(0.5))), t.lb());
    tubex::Function f7("x", "-sin(x)");
    ctc_picard.guess_kth_slices_envelope(f7, tube, 0, FORWARD);
    CHECK(tube(0).is_superset(Interval(2.*atan(exp(-t.lb())*tan(0.5))) | 2.*atan(exp(-t.ub())*tan(0.5))));
    
    tube = tube_raw;
    tube.set(IntervalVector(1, exp(-t.lb())), t.lb());
    tubex::Function f8("x", "-x");
    ctc_picard.guess_kth_slices_envelope(f8, tube, 0, FORWARD);
    CHECK(tube(0).is_superset(IntervalVector(Interval(exp(-t.lb()))) | IntervalVector(Interval(exp(-t.ub())))));
  }

  SECTION("Test CtcPicard / Slice - dim 1")
  {
    Interval domain(0.,0.1);
    TubeVector tube(domain, 1);
    tube.set(IntervalVector(1, exp(domain.lb())), domain.lb());
    Slice *x = tube[0].first_slice();

    tubex::Function f("x", "x");
    CtcPicard ctc_picard(1.1);

    CHECK(x->codomain() == Interval::ALL_REALS);
    CHECK(x->input_gate() == exp(0.));
    CHECK(x->output_gate() == Interval::ALL_REALS);
    ctc_picard.contract(f, tube, FORWARD);
    x = tube[0].first_slice();
    // TODO : CHECK(x->codomain()==(Interval(exp(domain))));
    CHECK(x->output_gate().is_superset(Interval(exp(domain.ub()))));
    CHECK(ctc_picard.picard_iterations() != 1);
    CHECK(ctc_picard.picard_iterations() < 4);
  }

  SECTION("Test CtcPicard / TubeVector - dim 1")
  {
    Interval domain(0.,1.);

    TubeVector x_preserve_sampling(domain, 1);
    IntervalVector condition(1, Interval(1.));
    x_preserve_sampling.set(condition, 0.);

    TubeVector x_auto_sampling(x_preserve_sampling);

    tubex::Function f("x", "-x");
    CtcPicard ctc_picard_preserve(1.1);
    ctc_picard_preserve.preserve_slicing(true);
    CtcPicard ctc_picard_auto(1.1);
    ctc_picard_auto.preserve_slicing(false);

    ctc_picard_preserve.contract(f, x_preserve_sampling, FORWARD); // todo: remove FORWARD
    ctc_picard_auto.contract(f, x_auto_sampling, FORWARD); // todo: remove FORWARD
    
    CHECK_FALSE(x_preserve_sampling.codomain().is_unbounded());
    // TODO : CHECK(x_preserve_sampling.codomain().is_superset(exp(-domain)));
    CHECK(x_preserve_sampling(0.).is_superset(Interval(exp(-0.))));
    CHECK(x_preserve_sampling(1.).is_superset(Interval(exp(-1.))));
    CHECK(x_preserve_sampling.nb_slices() == 1);
    
    CHECK(x_auto_sampling.codomain() == x_preserve_sampling.codomain());
    CHECK(x_auto_sampling(0.) == x_preserve_sampling(0.));
    CHECK(x_auto_sampling(1.) == x_preserve_sampling(1.));
    CHECK(x_auto_sampling.nb_slices() != 1);

    //if(VIBES_DRAWING) // drawing results
    //{
    //  vibes::beginDrawing();
    //  VIBesFigTube fig_tube("picard", &x_auto_sampling);
    //  fig_tube.set_properties(100, 100, 500, 500);
    //  fig_tube.show(true);
    //  vibes::endDrawing();
    //}
  }

  SECTION("Test CtcPicard / TubeVector - dim 2")
  {
    Interval domain(0.,1.);
    
    TubeVector x(domain, 2);
    IntervalVector condition(2, Interval(1.));
    x.set(condition, 0.);
    
    tubex::Function f("x", "y", "(-x ; y)");
    CtcPicard ctc_picard(1.1/*, false*/);
    ctc_picard.contract(f, x, FORWARD);

    CHECK_FALSE(x.codomain().is_unbounded());
    // TODO : CHECK(x.codomain()[0].is_superset(exp(-domain)));
    CHECK(x(0.)[0].is_superset(Interval(exp(-0.))));
    CHECK(x(1.)[0].is_superset(Interval(exp(-1.))));
    // TODO : CHECK(x.codomain()[1].is_superset(exp(domain)));
    CHECK(x(0.)[1].is_superset(Interval(exp(0.))));
    CHECK(x(1.)[1].is_superset(Interval(exp(1.))));
    
    //if(false & VIBES_DRAWING) // drawing results
    //{
    //  vibes::beginDrawing();
    //  VIBesFigTube fig_tube("picard", &x);
    //  fig_tube.set_properties(100, 100, 500, 500);
    //  fig_tube.show(true);
    //  vibes::endDrawing();
    //}
  }

  SECTION("Test CtcPicard / TubeVector - dim 1 - forward")
  {
    Interval domain(0.,1.);
    Tube x(domain);
    x.set(1., 0.);

    tubex::Function f("x", "-x");
    CtcPicard ctc_picard(1.1/*, false*/);

    ctc_picard.contract(f, x, FORWARD);
    
    CHECK_FALSE(x.codomain().is_unbounded());
    // TODO : CHECK(x.codomain().is_superset(exp(-domain)));
    CHECK(x(0.).is_superset(Interval(exp(-0.))));
    CHECK(x(1.).is_superset(Interval(exp(-1))));

    if(false & VIBES_DRAWING) // drawing results
    {
      //vibes::beginDrawing();
      //VIBesFigTube fig_tube("picard", &x);
      //fig_tube.set_properties(100, 100, 500, 500);
      //fig_tube.show(true);
      //vibes::endDrawing();
    }
  }

  SECTION("Test CtcPicard / TubeVector - dim 1 - backward")
  {
    Interval domain(0.,1.);
    Tube x(domain);
    x.set(exp(-1), 1.);

    tubex::Function f("x", "-x");
    CtcPicard ctc_picard(1.1/*, false*/);

    ctc_picard.contract(f, x, BACKWARD);
    CHECK_FALSE(x.codomain().is_unbounded());
    CHECK(x.codomain().is_superset(Interval(exp(-domain.lb())) | exp(-domain.ub())));
    CHECK(x(0.).is_superset(Interval(exp(-0.))));
    CHECK(x(1.).is_superset(Interval(exp(-1))));

    if(false & VIBES_DRAWING) // drawing results
    {
      //vibes::beginDrawing();
      //VIBesFigTube fig_tube("picard", &x);
      //fig_tube.set_properties(100, 100, 500, 500);
      //fig_tube.show(true);
      //vibes::endDrawing();
    }
  }
}