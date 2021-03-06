/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2013 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

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

\*---------------------------------------------------------------------------*/

#include "fvPatch.H"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
Foam::tmp<Foam::gpuField<Type> > Foam::fvPatch::patchInternalField
(
    const gpuList<Type>& f
) const
{
    tmp<gpuField<Type> > tpif(new gpuField<Type>(size()));
    gpuField<Type>& pif = tpif();

    const labelgpuList& faceCells = this->faceCells();

    thrust::copy(thrust::make_permutation_iterator(f.begin(),faceCells.begin()),
                 thrust::make_permutation_iterator(f.begin(),faceCells.end()),
                 pif.begin());

    return tpif;
}


template<class Type>
void Foam::fvPatch::patchInternalField
(
    const gpuList<Type>& f,
    gpuField<Type>& pif
) const
{
    pif.setSize(size());

    const labelgpuList& faceCells = this->faceCells();

    thrust::copy(thrust::make_permutation_iterator(f.begin(),faceCells.begin()),
                 thrust::make_permutation_iterator(f.begin(),faceCells.end()),
                 pif.begin());
}

template<class Type>
Foam::tmp<Foam::Field<Type> > Foam::fvPatch::patchInternalField
(
    const UList<Type>& f
) const
{
    tmp<Field<Type> > tpif(new Field<Type>(size()));
    Field<Type>& pif = tpif();

    const labelUList& faceCells = this->faceCellsHost();

    forAll(pif, facei)
    {
        pif[facei] = f[faceCells[facei]];
    }

    return tpif;
}


template<class Type>
void Foam::fvPatch::patchInternalField
(
    const UList<Type>& f,
    Field<Type>& pif
) const
{
    pif.setSize(size());

    const labelUList& faceCells = this->faceCellsHost();

    forAll(pif, facei)
    {
        pif[facei] = f[faceCells[facei]];
    }
}


template<class GeometricField, class Type>
const typename GeometricField::PatchFieldType& Foam::fvPatch::patchField
(
    const GeometricField& gf
) const
{
    return gf.boundaryField()[index()];
}


// ************************************************************************* //
