#ifndef OPENFLIPPER_POSSIONRECONSTRUCTIONT_HH
#define OPENFLIPPER_POSSIONRECONSTRUCTIONT_HH

#ifdef _WIN32
#include <Windows.h>
#include <Psapi.h>
#endif // _WIN32

#ifdef USE_OPENMP
#include <omp.h>
#endif

#include "PoissonReconstruction/Time.h"
#include "PoissonReconstruction/MarchingCubes.h"
#include "PoissonReconstruction/Octree.h"
#include "PoissonReconstruction/SparseMatrix.h"
#include "PoissonReconstruction/PPolynomial.h"
#include "PoissonReconstruction/MemoryUsage.h"
#include "PoissonReconstruction/MultiGridOctreeData.h"


template <class MeshT>
class PoissonReconstructionT
{
public:

    /// Constructor
    PoissonReconstructionT() {}

    /// Destructor
    ~PoissonReconstructionT() {}

    struct Parameter
    {
        Parameter() :
                Depth(8),
                MinDepth( 0 ),
                SamplesPerNode(1.0f),
                Scale(1.1f),
                Confidence(false),
                PointWeight(4.f),
                AdaptiveExponent(1),
                IsoDivide(8),
                SolverDivide(8),
                ShowResidual(false),
                MinIters(24),
                SolverAccuracy(float(1e-3)),
                FixedIters(-1),
                Verbose(true){}


        int Depth;
        int MinDepth;
        int SamplesPerNode;
        Real Scale;
        int Confidence;
        Real PointWeight;
        int AdaptiveExponent;
        Real IsoDivide; // = 0
        int SolverDivide;
        bool ShowResidual;
        int MinIters;
        double SolverAccuracy;
        int FixedIters;
        bool Verbose;

    };

    bool run(const MeshT& _orig_mesh, MeshT& _mesh, const Parameter& _parameter );

private:
    void prepare_data(const MeshT& _orig_mesh, std::vector<Real>& _pt_data);

private:
    Parameter m_parameter;
};


//=============================================================================
#if defined(INCLUDE_TEMPLATES) && !defined(POISSONRECONSTRUCTIONT_C)
#define POISSONRECONSTRUCTIONT_TEMPLATES
#include "PoissonReconstructionT.cc"
#endif
//=============================================================================

#endif //OPENFLIPPER_POSSIONRECONSTRUCTIONT_HH
