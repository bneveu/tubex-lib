#include "tests.h"
#include "tubex_Function.h"
#include "tubex_VIBesFigTube.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

#define VIBES_DRAWING 0

TEST_CASE("Functions")
{
  SECTION("Test 1")
  {
    TubeVector x(Interval(0.,10.), 0.01, tubex::Function("sin(t)+[-0.01,0.01]"));
    tubex::Function f("x", "t/10.+x");
    TubeVector y1(f.eval_vector(x));
    TubeVector y2(Interval(0.,10.), 0.01, tubex::Function("t/10.+sin(t)+[-0.01,0.01]"));
    CHECK(Approx(y1.volume()) == y2.volume());

    //if(VIBES_DRAWING) // drawing results
    //{
    //  vibes::beginDrawing();
    //  
    //  VIBesFigTube fig_tube_x("x", &x);
    //  fig_tube_x.set_properties(400, 400, 600, 300);
    //  fig_tube_x.show(true);
    //  
    //  VIBesFigTube fig_tube_y1("y1", &y1);
    //  fig_tube_y1.set_properties(400, 400, 600, 300);
    //  fig_tube_y1.show(true);
    //  
    //  VIBesFigTube fig_tube_y2("y2", &y2);
    //  fig_tube_y2.set_properties(400, 400, 600, 300);
    //  fig_tube_y2.show(true);
    //  
    //  vibes::endDrawing();
    //}
  }
}