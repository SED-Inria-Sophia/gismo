/** @file ParaviewUtils_.cpp

    @brief ParaView output Utilities

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): A. Mantzaflaris, J. Zwar, C. Karampatzakis
*/

// #include <gismo/Common/ForwardDeclarations.h>
#include<gismo/Common/TemplateTools.h>
#include<gismo/IO/Paraview/ParaviewUtils.h>
#include<gismo/IO/Paraview/ParaviewUtils.hpp>
// #include <gsLsdyna/gsBextFormat.h>

// #include<fstream>
// #include<iostream>


namespace gismo
{
    TEMPLATE_INST
    std::vector<std::string> toVTK(const gsFunctionSet<real_t>& funSet,
                                   unsigned nPts,
                                   unsigned precision,
                                   std::string label,
                                   const bool& export_base64);


    TEMPLATE_INST
    std::vector<std::string> toVTK(const gsField<real_t>& field,
                                   unsigned nPts,
                                   unsigned precision,
                                   std::string label,
                                   const bool& export_base64);


    TEMPLATE_INST
    std::string toDataArray(const gsMatrix<real_t> & matrix,
                            std::map<std::string, std::string> attributes,
                            unsigned precision,
                            const bool& export_base64);


    TEMPLATE_INST
    std::string toDataArray(const gsMatrix<index_t> & matrix,
                            std::map<std::string, std::string> attributes,
                            unsigned precision,
                            const bool& export_base64);

    TEMPLATE_INST
    std::string BezierVTK(const gsMultiPatch<real_t> & mPatch);

} // namespace gismo

