/* ============================================================================
 *  Catch extended to Ibex
 *
 *  This code is used for unit-tests purposes and based on the Catch library.
 *  https://github.com/philsquared/Catch
 *
 *  The Approx classes enable the floating point tolerance comparison of
 *  Intervals objects and pairs of Intervals.
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2016
 * ---------------------------------------------------------------------------- */

#include "ibex.h"

namespace Catch
{
  namespace Detail
  {
    #define DEFAULT_EPSILON std::numeric_limits<double>::epsilon()*10

    class ApproxIntv
    {
      public:
        explicit ApproxIntv(ibex::Interval value) :
            m_epsilon(DEFAULT_EPSILON),
            m_value(value)
        {}

        friend bool operator ==(ibex::Interval lhs, ApproxIntv const& rhs)
        {
          double e = rhs.m_epsilon;
          return lhs == rhs.m_value ||
                 (fabs(lhs.lb() - rhs.m_value.lb()) < e && fabs(lhs.ub() - rhs.m_value.ub()) < e);
        }

        friend bool operator ==(ApproxIntv const& lhs, ibex::Interval rhs)
        {
          return operator ==(rhs, lhs);
        }

        friend bool operator !=(ibex::Interval lhs, ApproxIntv const& rhs)
        {
          return !operator ==(lhs, rhs);
        }

        friend bool operator !=(ApproxIntv const& lhs, ibex::Interval rhs)
        {
          return !operator ==(rhs, lhs);
        }

        std::string toString() const
        {
          std::ostringstream oss;
          oss << "Approx( " << Catch::toString(m_value) << " )";
          return oss.str();
        }

      private:
        double m_epsilon;
        ibex::Interval m_value;
    };

    class ApproxVector
    {
      public:
        explicit ApproxVector(ibex::Vector value) :
            m_epsilon(DEFAULT_EPSILON),
            m_value(value)
        {}

        friend bool operator ==(ibex::Vector lhs, ApproxVector const& rhs)
        {
          double e = rhs.m_epsilon;

          if(lhs != rhs.m_value)
            return false;

          for(int i = 0 ; i < lhs.size() ; i++)
            if(fabs(lhs[i] - rhs.m_value[i]) >= e)
              return false;

          return true;
        }

        friend bool operator ==(ApproxVector const& lhs, ibex::Vector rhs)
        {
          return operator ==(rhs, lhs);
        }

        friend bool operator !=(ibex::Vector lhs, ApproxVector const& rhs)
        {
          return !operator ==(lhs, rhs);
        }

        friend bool operator !=(ApproxVector const& lhs, ibex::Vector rhs)
        {
          return !operator ==(rhs, lhs);
        }

        std::string toString() const
        {
          std::ostringstream oss;
          oss << "Approx( " << Catch::toString(m_value) << " )";
          return oss.str();
        }

      private:
        double m_epsilon;
        ibex::Vector m_value = ibex::Vector(1);
    };

    class ApproxIntvPair
    {
      public:
        explicit ApproxIntvPair(std::pair<ibex::Interval,ibex::Interval> value) :
            m_epsilon(DEFAULT_EPSILON),
            m_value(value)
        {}

        friend bool operator ==(std::pair<ibex::Interval,ibex::Interval> lhs, ApproxIntvPair const& rhs)
        {
          double e = rhs.m_epsilon;
          return lhs == rhs.m_value ||
                 (fabs(lhs.first.lb() - rhs.m_value.first.lb()) < e &&
                  fabs(lhs.first.ub() - rhs.m_value.first.ub()) < e &&
                  fabs(lhs.second.lb() - rhs.m_value.second.lb()) < e &&
                  fabs(lhs.second.ub() - rhs.m_value.second.ub()) < e);
        }

        friend bool operator ==(ApproxIntvPair const& lhs, std::pair<ibex::Interval,ibex::Interval> rhs)
        {
          return operator ==(rhs, lhs);
        }

        friend bool operator !=(std::pair<ibex::Interval,ibex::Interval> lhs, ApproxIntvPair const& rhs)
        {
          return !operator ==(lhs, rhs);
        }

        friend bool operator !=(ApproxIntvPair const& lhs, std::pair<ibex::Interval,ibex::Interval> rhs)
        {
          return !operator ==(rhs, lhs);
        }

        std::string toString() const
        {
          std::ostringstream oss;
          oss << "Approx( " << Catch::toString(m_value.first) << "," << Catch::toString(m_value.second) << " )";
          return oss.str();
        }

      private:
        double m_epsilon;
        std::pair<ibex::Interval,ibex::Interval> m_value;
    };
  }

  template<>
  inline std::string toString<Detail::ApproxIntv>(Detail::ApproxIntv const& value)
  {
    return value.toString();
  }

  template<>
  inline std::string toString<Detail::ApproxVector>(Detail::ApproxVector const& value)
  {
    return value.toString();
  }

  template<>
  inline std::string toString<Detail::ApproxIntvPair>(Detail::ApproxIntvPair const& value)
  {
    return value.toString();
  }
}