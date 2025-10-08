/** @file gsViewField.cpp

    @brief Export a geometry with a metric/field to ParaView and LibMeshb formats

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Gobe
*/

#include <iostream>
#include <gismo.h>

#ifdef gsLibMeshb_ENABLED
#include <gsLibMeshb/gsLibMeshb.h>
#endif

using namespace gismo;

int main(int argc, char *argv[])
{
    std::string pname("field_output"), fn("");
    index_t numSamples(1000);
    bool export_meshb = false;
    std::string funcExpr("x+y");

    //! [Parse Command line]
    gsCmdLine cmd("Create a field on geometry and export to ParaView/LibMeshb format.");

    cmd.addPlainString("filename", "File containing geometry data (.xml or third-party)", fn);
    cmd.addString("o", "oname", "Filename to use for output", pname);
    cmd.addInt("s", "samples", "Number of samples to use for viewing", numSamples);
    cmd.addString("f", "function", "Function expression to use as metric (e.g., 'x+y', 'x*x+y*y')", funcExpr);
#ifdef gsLibMeshb_ENABLED
    cmd.addSwitch("meshb", "Export to LibMeshb .meshb format instead of ParaView", export_meshb);
#endif

    try { cmd.getValues(argc,argv); } catch (int rv) { return rv; }
    //! [Parse Command line]

    if ( fn.empty() )
    {
        gsInfo << cmd.getMessage();
        gsInfo << "\nType " << argv[0] << " -h, to get the list of command line options.\n";
        return 0;
    }

    //! [Read geometry]
    gsFileData<> filedata(fn);

    if (!filedata.has< gsMultiPatch<> >())
    {
        gsInfo << "File does not contain a gsMultiPatch. Please provide a multipatch geometry.\n";
        return EXIT_FAILURE;
    }

    gsMultiPatch<> mp;
    filedata.getFirst(mp);
    gsInfo << "Loaded " << mp << "\n";
    //! [Read geometry]

    //! [Create field]
    // Determine domain dimension from geometry
    int domainDim = mp.domainDim();

    gsInfo << "Creating field with function: " << funcExpr << "\n";
    gsInfo << "Domain dimension: " << domainDim << "\n";

    // Create a function expression
    gsFunctionExpr<> func(funcExpr, domainDim);

    // Create field (geometry + solution)
    gsField<> field(mp, func, false);

    gsInfo << "Field created with " << field.nPieces() << " pieces\n";
    //! [Create field]

    //! [Export field]
#ifdef gsLibMeshb_ENABLED
    if (export_meshb)
    {
        gsInfo << "Exporting field to LibMeshb format...\n";
        gsWriteLibMeshb(field, pname, numSamples, 2);
        gsInfo << "LibMeshb export complete: " << pname << ".meshb, " << pname << ".solb\n";
    }
    else
#endif
    {
        gsInfo << "Exporting field to ParaView unstructured grid format...\n";
        gsWriteParaviewUnstructuredGrid(field, pname, numSamples);
        gsInfo << "ParaView export complete: " << pname << ".pvd, " << pname << ".vtu\n";
    }
    //! [Export field]

    return EXIT_SUCCESS;
}
