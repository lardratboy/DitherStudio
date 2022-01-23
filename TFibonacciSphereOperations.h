// ----------------------------------------------------------------------------
/*
The MIT License (MIT)

Copyright (c) 2000- Brad P. Taylor

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
// ----------------------------------------------------------------------------

//	-- FILE NOTES --
// 
//  technique from article and others
// 
//  http://extremelearning.com.au/how-to-evenly-distribute-points-on-a-sphere-more-effectively-than-the-canonical-fibonacci-lattice/
//
//////////////////////////////////////////////////////////////////////

#if !defined(TFIBONACCISPHEREOPERATIONS_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_)
#define TFIBONACCISPHEREOPERATIONS_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cmath>
#include <algorithm>

namespace BPT {

    // ------------------------------------------------------------------------

    template<typename V,typename N = double>
    class TFibonacciLattice {

    protected:

        const N sqrt5 = (N)2.2360679;
        const N pi = (N)3.1415926;
        const N twopi = pi * (N)2;
        const N goldenRatio = ((N)1 + sqrt5) / (N)2;

        N EpsilonFromCount(int n) const {

            N epsilon;

            if (n >= 600000)
                epsilon = (N)214;
            else if (n >= 400000)
                epsilon = (N)75;
            else if (n >= 11000)
                epsilon = (N)27;
            else if (n >= 890)
                epsilon = (N)10;
            else if (n >= 177)
                epsilon = (N)3.33;
            else if (n >= 24)
                epsilon = (N)1.33;
            else
                epsilon = (N)0.33;

            return epsilon;

        }

    public:

        std::vector<V> lattice;

        bool Create(const int n)
        {
            lattice.reserve(n);

            const N epsilon = EpsilonFromCount(n);
            const N oone = (N)1 / ((N)n - (N)1 + (N)2 * epsilon);
            const N oogr = (N)1 / goldenRatio;

            for (int i = 0; i < n; i++)
            {
                const N theta = (twopi * (N)i) * oogr;
                const N phi = std::acos( (N)1 - ((N)2 * (i + epsilon)) * oone );

                N cos_theta = std::cos(theta);
                N sin_theta = std::sin(theta);
                N cos_phi = std::cos(phi);
                N sin_phi = std::sin(phi);

                V v(cos_theta * sin_phi, sin_theta * sin_phi, cos_phi);

                lattice.push_back(v);
            }

            return true;
        }

    };

    // ------------------------------------------------------------------------


} // namespace BPT

#endif // TFIBONACCISPHEREOPERATIONS_H__4EE3AB65_FB91_4E19_9877_81FB6B6BDBFD__INCLUDED_

