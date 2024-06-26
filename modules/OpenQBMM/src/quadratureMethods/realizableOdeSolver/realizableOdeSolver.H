/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | OpenQBMM - www.openqbmm.org
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2015-2023 Alberto Passalacqua
-------------------------------------------------------------------------------
License
    This file is derivative work of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::realizableOdeSolver

Description
    Integrates a moment vector in a computational cell using a realizable ODE
    solver.

    The procedure is adapted from the Nguyen et al. (2016) to fit into the 
    OpenFOAM structure. Specifically, the time step is calculated to ensure
    realizability and fit within the global time step of the calling solver.

    Reference:
    \verbatim
        "Solution of population balance equations in applications with 
        fine particles: Mathematical modeling and numerical schemes"
        Nguyen, T.T., Laurent, F., Fox, R.O., Massot, M.
        Issue 325, 2016, pp. 129-156.
    \endverbatim

SourceFiles
    realizableOdeSolver.C

\*---------------------------------------------------------------------------*/

#ifndef realizableOdeSolver_H
#define realizableOdeSolver_H

#include "quadratureApproximation.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                Class realizableOdeSolver Declaration
\*---------------------------------------------------------------------------*/

template<class momentType, class nodeType>
class realizableOdeSolver
{

    typedef momentFieldSet<momentType, nodeType> momentFieldSetType;
    typedef quadratureApproximation<momentType, nodeType> quadratureType;

    // Private data

        //- Reference to mesh
        const fvMesh& mesh_;

        //- Absolute tolerance for realizable ODE solver
        scalar ATol_;

        //- Relative tolerance for realizable ODE solver
        scalar RTol_;

        //- Time-step change factor
        scalar fac_;

        //- Minimum time-step change factor
        scalar facMin_;

        //- Maximum time-step change factor
        scalar facMax_;

        //- Minimum local step in realizable ODE solver
        scalar minLocalDt_;

        //- Stored time step values
        volScalarField localDt_;

        //- Number of times localDt has been adjusted
        label localDtAdjustments_;


protected:

    // Protected data

        //- Switch to activate moment update due to source terms
        Switch solveSources_;

        //- Switch to activate the realizable ODE solver
        Switch solveOde_;

    // Protected member functions

        //- Calculate source term for moment equation in the specified cell
        virtual void updateCellMomentSource(const label celli) = 0;

        //- Calculate source term for moment equation in the specified cell
        virtual scalar cellMomentSource
        (
            const labelList& momentOrder,
            const label celli,
            const quadratureType& nodes,
            const label enviroment
        ) = 0;


public:

    // Constructors

        //- Construct from components
        realizableOdeSolver
        (
            const fvMesh& mesh,
            const dictionary& dict
        );


    //- Destructor
    virtual ~realizableOdeSolver();


    // Member Functions

        //- Solve univariate PDF transport equation
        void solve(quadratureType& quadrature, const label enviroment);

        //- Read coefficients from dictionary
        void read(const dictionary& dict);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#if NoRepository
    #include "realizableOdeSolver.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif
