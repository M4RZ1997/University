/*===========================================================================*\
 *                                                                           *
 *                            AlgoHex Library                                *
 *     Copyright (C) 2019 by Computer Graphics Group, University of Bern     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 *  This file is part of AlgoHex Library.                                    *
 *                                                                           *
 *  AlgoHex is free software: you can redistribute it and/or modify          *
 *  it under the terms of the GNU Lesser General Public License as           *
 *  published by the Free Software Foundation, either version 3 of           *
 *  the License, or (at your option) any later version with the              *
 *  following exceptions:                                                    *
 *                                                                           *
 *  If other files instantiate templates or use macros                       *
 *  or inline functions from this file, or you compile this file and         *
 *  link it with other files to produce an executable, this file does        *
 *  not by itself cause the resulting executable to be covered by the        *
 *  GNU Lesser General Public License. This exception does not however       *
 *  invalidate any other reasons why the executable file might be            *
 *  covered by the GNU Lesser General Public License.                        *
 *                                                                           *
 *  AlgoHex is distributed in the hope that it will be useful,               *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU Lesser General Public License for more details.                      *
 *                                                                           *
 *  You should have received a copy of the GNU LesserGeneral Public          *
 *  License along with AlgoHex.  If not,                                     *
 *  see <http://www.gnu.org/licenses/>.                                      *
 *                                                                           *
\*===========================================================================*/

#pragma once

//== INCLUDES =================================================================

#include <chrono>

//== FORWARDDECLARATIONS ======================================================

//== NAMESPACES ===============================================================

namespace AlgoHex {
    
//== CLASS DEFINITION =========================================================
    
    
    
    
/** \class StopWatch StopWatch.hh
     
     Brief Description.
     
     A more elaborate description follows.
*/
    
template <typename UnitT = std::chrono::milliseconds>
class StopWatch
{
public:
    // Constructor
    StopWatch() {}
    // Destructor
    ~StopWatch() {}

    // Start time measurement
    void start()
    {
        starttime_ = std::chrono::steady_clock::now();
    }

    // Restart, return time elapsed until now.
    double restart()
    {
        double t = elapsed();
        start();
        return t;
    }

    // Stop time measurement, return time.
    double stop()
    {
        endtime_ = std::chrono::steady_clock::now();
        return elapsed();
    }

    // Get the total time in UnitT (watch has to be stopped).
    double elapsed() const
    {
        auto duration = std::chrono::duration_cast<UnitT>(endtime_ - starttime_);
        return (double) duration.count();
    }

private:
    std::chrono::steady_clock::time_point starttime_, endtime_;
};

//=============================================================================
} // namespace AlgoHex
//=============================================================================
