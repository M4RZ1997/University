#define POISSONRECONSTRUCTIONT_C

#include "PossionReconstructionT.hh"


template <class MeshT>
void
PoissonReconstructionT<MeshT>::
prepare_data(const MeshT& _orig_mesh, std::vector<Real>& _pt_data) {
    // ============================================================================
    // Exercise 6 : poisson reconstruction
    // The task is to store vertices coordinates and normals in the vector _pt_data
    // (see PDF for instructions)
    // ============================================================================
    //----------Add your code here----------
    _pt_data.reserve(_orig_mesh.n_vertices() * 6 );
    auto vit = _orig_mesh.vertices_begin();
    for (auto vhi : _orig_mesh.vertices())
    {
        _pt_data.push_back( _orig_mesh.point( vhi )[0] );
        _pt_data.push_back( _orig_mesh.point( vhi )[1] );
        _pt_data.push_back( _orig_mesh.point( vhi )[2] );
        _pt_data.push_back( _orig_mesh.normal( vhi )[0] );
        _pt_data.push_back( _orig_mesh.normal( vhi )[1] );
        _pt_data.push_back( _orig_mesh.normal( vhi )[2] );
    }
    //--------------------------------------
}


template <class MeshT>
bool
PoissonReconstructionT<MeshT>::
run(const MeshT& _orig_mesh, MeshT& _mesh, const Parameter& _parameter )
{
    std::vector<Real> pt_data;
    prepare_data(_orig_mesh, pt_data);

    m_parameter = _parameter;
    Real isoValue = 0;
    Octree<2> tree;
#ifdef USE_OMP
    tree.threads = omp_get_num_procs();
#else
    tree.threads = 1;
#endif
    TreeOctNode::SetAllocator( MEMORY_ALLOCATOR_BLOCK_SIZE );

    std::cout << "Tree construction with depth " << m_parameter.Depth << std::endl;
    tree.setBSplineData(m_parameter.Depth);
    double maxMemoryUsage;
    tree.maxMemoryUsage=0;
    XForm4x4< Real > xForm = XForm4x4< Real >::Identity();
    int pointCount = tree.setTreeMemory(pt_data ,  m_parameter.Depth ,  m_parameter.MinDepth , m_parameter.Depth , Real(m_parameter.SamplesPerNode),
                                         m_parameter.Scale , m_parameter.Confidence , m_parameter.PointWeight , m_parameter.AdaptiveExponent , xForm);

    if (pointCount <= 0)
    {
        std::cout << "Invalid Input Points" << std::endl;
        return false;
    }

    std::cout << "Tree Clipping" << std::endl;
    tree.ClipTree();

    std::cout << "Tree Finalize" << std::endl;
    tree.finalize( m_parameter.IsoDivide );

    std::cout <<"Input Points: "<<pointCount<<"\n";
    std::cout <<"Leaves/Nodes: "<<tree.tree.leaves()<<"/"<<tree.tree.nodes()<<"\n";
    std::cout <<"Memory Usage: "<<float( MemoryInfo::Usage() )/(1<<20)<<"MB\n";

    maxMemoryUsage = tree.maxMemoryUsage;
    tree.maxMemoryUsage=0;
    tree.SetLaplacianConstraints();
    std::cout <<"Memory Usage: "<<float( MemoryInfo::Usage() )/(1<<20)<<"MB\n";
    maxMemoryUsage = std::max< double >( maxMemoryUsage , tree.maxMemoryUsage );

    tree.maxMemoryUsage=0;
    tree.LaplacianMatrixIteration( m_parameter.SolverDivide, m_parameter.ShowResidual, m_parameter.MinIters,
            m_parameter.SolverAccuracy, m_parameter.Depth, m_parameter.FixedIters );
    std::cout <<"Memory Usage: "<<float( MemoryInfo::Usage() )/(1<<20)<<"MB\n";
    maxMemoryUsage = std::max< double >( maxMemoryUsage , tree.maxMemoryUsage );

    CoredFileMeshData mesh;
    if( m_parameter.Verbose ) tree.maxMemoryUsage=0;
    double time=Time();
    isoValue = tree.GetIsoValue();
    std::cout <<"Got average in: "<<Time()-time<<"\n";
    std::cout <<"Iso-Value: "<<isoValue<<"\n";

    tree.maxMemoryUsage = 0;
    tree.GetMCIsoTriangles( isoValue , m_parameter.IsoDivide , &mesh );
    std::cout <<"Memory Usage: "<<float( MemoryInfo::Usage() )/(1<<20)<<"MB\n";

    _mesh.clear();
    mesh.resetIterator();

    std::cout <<"Time for Iso: "<<Time()-time<<"\n";

    int nr_faces=mesh.polygonCount();

    mesh.resetIterator();

    // write vertices
    Point3D<float> p;
    for(size_t i=0 ; i < mesh.inCorePoints.size() ; i++) {
        p = mesh.inCorePoints[i];
        _mesh.add_vertex( typename MeshT::Point(p[0],p[1],p[2]) );
    }
    for(int i=0; i<mesh.outOfCorePointCount(); i++) {
        mesh.nextOutOfCorePoint(p);
        _mesh.add_vertex(typename MeshT::Point(p[0],p[1],p[2]));
    }

    // write faces
    std::vector< CoredVertexIndex > polygon;
    for(int i=0 ; i<nr_faces ; i++) {
        // create and fill a struct that the ply code can handle
        mesh.nextPolygon(polygon);
        std::vector<typename MeshT::VertexHandle> face;
        for(size_t i=0 ; i<polygon.size(); i++)
            if( polygon[i].inCore )
                face.push_back( _mesh.vertex_handle( polygon[i].idx ) );
            else
                face.push_back( _mesh.vertex_handle( polygon[i].idx + int( mesh.inCorePoints.size() ) ) );

        _mesh.add_face(face);
    }

    _mesh.update_normals();

    return true;
}