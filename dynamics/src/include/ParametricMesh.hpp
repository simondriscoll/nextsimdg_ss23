/*!
 * @file ParametricMesh.hpp
 * @date 9 Juli 2022
 * @author Thomas Richter <thomas.richter@ovgu.de>
 */

#ifndef __PARAMETRICMESH_HPP
#define __PARAMETRICMESH_HPP

#include <Eigen/Dense>
#include <array>
#include <cassert>
#include <cstddef>

#include "codeGenerationParametricMesh.hpp"
#include "nextsimFloatType.hpp"
#include <iostream>
#include <string>

namespace Nextsim {

inline constexpr double SQR(double x) { return x * x; }

/*!
 * Stores the spacial mesh of the domain.
 *
 * The mesh has a structured topology with nelements = nx * ny elements
 * and nnodes = (nx+1) * (ny+1) nodes.
 * Everything is sorted from lower left to upper right, using
 *   cellindex = ny     * rowindex + colindex
 *   nodeindex = (ny+1) * rowindex + colindex
 *
 * Coordinates of the nnodes vertices are stored as
 *
 *   Eigen::Matrix< NextSim::FloatType , nnodes, 2 > vertices;
 *
 */

typedef std::array<double, 2> Vertex;

class ParametricMesh {
public:
    int statuslog; //!< -1 no output, 1 full status output

    size_t nx, ny; //!< no of elements in x- and y-direction
    size_t nnodes; //!< total number of nodes
    size_t nelements; //!< total number of nodes

    Eigen::Matrix<Nextsim::FloatType, Eigen::Dynamic, 2> vertices; // stores the

    ParametricMesh(int loglevel = 1)
        : statuslog(loglevel)
        , nx(-1)
        , ny(-1)
        , nnodes(-1)
        , nelements(-1)
    {
    }

    /*!
     * Reads mesh from a .smesh - file
     *
     * File format:
     *
     * ParametricMesh 1.0 % Identifier & Version
     *
     * nx ny          % number of elements in x- and y- direction  << Version: 1.0
     * x1       y1    % x- and y- coordinates of vertices. first vertex lower left
     * x2       y2    % then to the right, then 2nd row, ... up to upper right
     * ...
     * xxnnodes ynnodes
     */
    void readmesh(std::string fname);

    /*!
     * Returns index of lower left node in element with index eid
     */
    size_t eid2nid(const size_t eid) const
    {
        return (eid / nx) * (nx + 1) + (eid % nx);
    }

    /*!
     * returns the coordinates of one element with index eid as 4 x 2 matrix
     */
    const Eigen::Matrix<Nextsim::FloatType, 4, 2> coordinatesOfElement(const size_t eid) const
    {
        const size_t nid = eid2nid(eid);
        return Eigen::Matrix<Nextsim::FloatType, 4, 2>({ { vertices(nid, 0), vertices(nid, 1) },
            { vertices(nid + 1, 0), vertices(nid + 1, 1) },
            { vertices(nid + nx + 1, 0), vertices(nid + nx + 1, 1) },
            { vertices(nid + nx + 2, 0), vertices(nid + nx + 2, 1) } });
    }

    /*!
     * return the area of the mesh element with index eid
     */
    double area(const size_t eid) const
    {
        const size_t nid = eid2nid(eid);

        const double a = (vertices.block<1, 2>(nid, 0) - vertices.block<1, 2>(nid + 1, 0)).squaredNorm(); // lower
        const double b = (vertices.block<1, 2>(nid + 1, 0) - vertices.block<1, 2>(nid + nx + 2, 0)).squaredNorm(); // right
        const double c = (vertices.block<1, 2>(nid + 1 + nx, 0) - vertices.block<1, 2>(nid + 2 + nx, 0)).squaredNorm(); // top
        const double d = (vertices.block<1, 2>(nid, 0) - vertices.block<1, 2>(nid + nx + 1, 0)).squaredNorm(); // left
        const double e = (vertices.block<1, 2>(nid, 0) - vertices.block<1, 2>(nid + nx + 2, 0)).squaredNorm(); // diag 1
        const double f = (vertices.block<1, 2>(nid + 1, 0) - vertices.block<1, 2>(nid + nx + 1, 0)).squaredNorm(); // diag 2

        return 0.25 * sqrt(4.0 * e * f - SQR(b + d - a - c));
    }
    /*!
     * return the mesh size (as square root of area)  of the mesh element eid
     */
    double h(const size_t eid) const
    {
        return sqrt(area(eid));
    }

    /*!
     * returns the cooridnate of a vertex by an index (ix,iy)
     * For CG1 this is directly the corresponding entry ( (nx+1) iy + ix ) in the vertices-array
     * For CG2 we linearly interpolate.
     */
    template <int CGdegree>
    Vertex coordinate(size_t ix, size_t iy) const
    {
        if (CGdegree == 1) {
            assert(ix < nx + 1);
            assert(iy < ny + 1);
            const size_t ii = (nx + 1) * iy + ix;
            return Vertex({ vertices(ii, 0), vertices(ii, 1) });
        } else if (CGdegree == 2) {
            assert(ix < 2 * nx + 1);
            assert(iy < 2 * ny + 1);
            const size_t cx = ix / 2; // index of the element where the cooridnate belongs to
            const size_t cy = iy / 2;
            const size_t mx = ix % 2; // where are we in the element?
            const size_t my = iy % 2;
            const size_t n0 = (nx + 1) * cy + cx; // lower left node

            const size_t shift_y = nx + 1;

            if ((my == 0) && (mx == 0))
                return Vertex({ vertices(n0, 0), vertices(n0, 1) });
            else if ((my == 0) && (mx == 1))
                return Vertex({ 0.5 * (vertices(n0, 0) + vertices(n0 + 1, 0)),
                    0.5 * (vertices(n0, 1) + vertices(n0 + 1, 1)) });
            else if ((my == 1) && (mx == 0))
                return Vertex({ 0.5 * (vertices(n0, 0) + vertices(n0 + shift_y, 0)),
                    0.5 * (vertices(n0, 1) + vertices(n0 + shift_y, 1)) });
            else if ((my == 1) && (mx == 1))
                return Vertex({ 0.25 * (vertices(n0, 0) + vertices(n0 + 1, 0) + vertices(n0 + shift_y, 0) + vertices(n0 + shift_y + 1, 0)),
                    0.25 * (vertices(n0, 1) + vertices(n0 + 1, 1) + vertices(n0 + shift_y, 1) + vertices(n0 + shift_y + 1, 1)) });
            else
                abort();
        } else
            abort();
    }

    /*!
     * computes the vector between two points n1, n2 (not normed)
     */
    Eigen::Matrix<Nextsim::FloatType, 1, 2> edgevector(const size_t n1, const size_t n2) const
    {
        return vertices.block<1, 2>(n2, 0) - vertices.block<1, 2>(n1, 0);
    }

    // Global access functions

    double hmin() const; //! returns the minimum mesh size
    double area() const; //! returns the area of the domain
};

} /* namespace Nextsim */

#endif /* __MESH_HPP */
