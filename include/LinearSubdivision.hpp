
#ifndef LinearSubdivision_hpp
#define LinearSubdivision_hpp

#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/Filtered_kernel.h>
#include <CGAL/Cartesian.h>
#include <CGAL/Subdivision_method_3.h>

namespace LinearSubdivision
{
    using Real = float;
    using Kernel0 = CGAL::Cartesian<Real>;
    using Kernel = CGAL::Filtered_kernel<Kernel0>;
    using Polyhedron = CGAL::Polyhedron_3<Kernel>;
    using Point = Polyhedron::Point_3;
    using Halfedge = Polyhedron::Halfedge;
    using Face = Polyhedron::Facet;
    
    template <class Poly>
    class LinearSubdivisionMask
    {
        typedef Poly                                         Polyhedron;
        typedef typename Polyhedron::Vertex_iterator         Vertex_iterator;
        typedef typename Polyhedron::Halfedge_iterator       Halfedge_iterator;
        typedef typename Polyhedron::Facet_iterator          Facet_iterator;
        typedef typename Polyhedron::Halfedge_around_facet_circulator
                                                  Halfedge_around_facet_circulator;
        typedef typename Polyhedron::Halfedge_around_vertex_circulator
                                                  Halfedge_around_vertex_circulator;
        typedef typename Polyhedron::Traits                  Traits;
        typedef typename Traits::Kernel                      Kernel;
        typedef typename Kernel::FT                          FT;
        typedef typename Kernel::Point_3                     Point;
        typedef typename Kernel::Vector_3                    Vector;
    
        public:
            void edge_node(Halfedge_iterator eitr, Point& pt)
            {
                Point& p1 = eitr->vertex()->point();
                Point& p2 = eitr->opposite()->vertex()->point();
                pt = Point(
                    0.5f*(p1[0]+p2[0]),
                    0.5f*(p1[1]+p2[1]),
                    0.5f*(p1[2]+p2[2])                
                );
            }
            void vertex_node(Vertex_iterator vitr, Point& pt)
            {
                pt = vitr->point();
            }
    
            void border_node(Halfedge_iterator eitr, Point& ept, Point& vpt)
            {
                Point& ep1 = eitr->vertex()->point();
                Point& ep2 = eitr->opposite()->vertex()->point();
                ept = Point(
                    0.5f*(ep1[0]+ep2[0]),
                    0.5f*(ep1[1]+ep2[1]),
                    0.5f*(ep1[2]+ep2[2])
                );
    
                Halfedge_around_vertex_circulator vcir = eitr->vertex_begin();
                vpt = vcir->vertex()->point();
            }
    };
    
    void subdivide(Polyhedron &mesh, int iterations = 1)
    {
        CGAL::Subdivision_method_3::PTQ(mesh, LinearSubdivisionMask<Polyhedron>(), iterations);        
    }
}

#endif