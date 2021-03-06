/** 
 *  CtcStatic class
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_CtcStatic.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcStatic::CtcStatic(ibex::Ctc *ibex_ctc, bool dynamic_ctc)
    : Ctc(), m_ibex_ctc(ibex_ctc), m_dynamic_ctc(dynamic_ctc ? 1 : 0)
  {

  }
  
  CtcStatic::~CtcStatic()
  {

  }

  void CtcStatic::contract(TubeVector& x)
  {
    assert(x.size()+m_dynamic_ctc == m_ibex_ctc->nb_var);

    Slice **v_x_slices = new Slice*[x.size()];
    for(int i = 0 ; i < x.size() ; i++)
      v_x_slices[i] = x[i].first_slice();

    contract(v_x_slices, x.size());
    delete v_x_slices;
  }

  void CtcStatic::contract(Tube& x1)
  {
    int n = 1;
    assert(n+m_dynamic_ctc == m_ibex_ctc->nb_var);

    Slice **v_x_slices = new Slice*[n];
    v_x_slices[0] = x1.first_slice();

    contract(v_x_slices, n);
    delete v_x_slices;
  }

  void CtcStatic::contract(Tube& x1, Tube& x2)
  {
    int n = 2;
    assert(n+m_dynamic_ctc == m_ibex_ctc->nb_var);

    Slice **v_x_slices = new Slice*[n];
    v_x_slices[0] = x1.first_slice();
    v_x_slices[1] = x2.first_slice();

    contract(v_x_slices, n);
    delete v_x_slices;
  }

  void CtcStatic::contract(Tube& x1, Tube& x2, Tube& x3)
  {
    int n = 3;
    assert(n+m_dynamic_ctc == m_ibex_ctc->nb_var);

    Slice **v_x_slices = new Slice*[n];
    v_x_slices[0] = x1.first_slice();
    v_x_slices[1] = x2.first_slice();
    v_x_slices[2] = x3.first_slice();

    contract(v_x_slices, n);
    delete v_x_slices;
  }

  void CtcStatic::contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4)
  {
    int n = 4;
    assert(n+m_dynamic_ctc == m_ibex_ctc->nb_var);

    Slice **v_x_slices = new Slice*[n];
    v_x_slices[0] = x1.first_slice();
    v_x_slices[1] = x2.first_slice();
    v_x_slices[2] = x3.first_slice();
    v_x_slices[3] = x4.first_slice();

    contract(v_x_slices, n);
    delete v_x_slices;
  }

  void CtcStatic::contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5)
  {
    int n = 5;
    assert(n+m_dynamic_ctc == m_ibex_ctc->nb_var);

    Slice **v_x_slices = new Slice*[n];
    v_x_slices[0] = x1.first_slice();
    v_x_slices[1] = x2.first_slice();
    v_x_slices[2] = x3.first_slice();
    v_x_slices[3] = x4.first_slice();
    v_x_slices[4] = x5.first_slice();

    contract(v_x_slices, n);
    delete v_x_slices;
  }

  void CtcStatic::contract(Tube& x1, Tube& x2, Tube& x3, Tube& x4, Tube& x5, Tube& x6)
  {
    int n = 6;
    assert(n+m_dynamic_ctc == m_ibex_ctc->nb_var);

    Slice **v_x_slices = new Slice*[n];
    v_x_slices[0] = x1.first_slice();
    v_x_slices[1] = x2.first_slice();
    v_x_slices[2] = x3.first_slice();
    v_x_slices[3] = x4.first_slice();
    v_x_slices[4] = x5.first_slice();
    v_x_slices[5] = x6.first_slice();

    contract(v_x_slices, n);
    delete v_x_slices;
  }

  void CtcStatic::contract(Slice **v_x_slices, int n)
  {
    IntervalVector envelope(n + m_dynamic_ctc);
    IntervalVector ingate(n + m_dynamic_ctc);

    while(v_x_slices[0] != NULL)
    {
      // If this slice should not be impacted by the contractor
      if(!v_x_slices[0]->domain().intersects(m_restricted_domain))
      {
        for(int i = 0 ; i < n ; i++)
          v_x_slices[i] = v_x_slices[i]->next_slice();

        // todo: Thin contraction with respect to tube's slicing:
        // the contraction should not be optimal on purpose if the
        // restricted domain does not cover the slice's domain
        
        continue; // moving to next slice
      }

      if(m_dynamic_ctc)
      {
        envelope[0] = v_x_slices[0]->domain();
        ingate[0] = v_x_slices[0]->domain().lb();
      }

      for(int i = 0 ; i < n ; i++)
      {
        envelope[i+m_dynamic_ctc] = v_x_slices[i]->codomain();
        ingate[i+m_dynamic_ctc] = v_x_slices[i]->input_gate();
      }

      m_ibex_ctc->contract(envelope);
      m_ibex_ctc->contract(ingate);

      for(int i = 0 ; i < n ; i++)
      {
        v_x_slices[i]->set_envelope(envelope[i+m_dynamic_ctc]);
        v_x_slices[i]->set_input_gate(ingate[i+m_dynamic_ctc]);
      }

      if(v_x_slices[0]->next_slice() == NULL) // output gate
      {
        IntervalVector outgate(n + m_dynamic_ctc);

        if(m_dynamic_ctc)
          outgate[0] = v_x_slices[0]->domain().ub();

        for(int i = 0 ; i < n ; i++)
          outgate[i+m_dynamic_ctc] = v_x_slices[i]->output_gate();

        m_ibex_ctc->contract(outgate);

        for(int i = 0 ; i < n ; i++)
          v_x_slices[i]->set_output_gate(outgate[i+m_dynamic_ctc]);

        break; // end of contractions
      }
      
      else
        for(int i = 0 ; i < n ; i++)
          v_x_slices[i] = v_x_slices[i]->next_slice();
    }
  }
}