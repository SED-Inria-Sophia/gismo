#pragma once

// Include the base class first
#include <gismo/Core/Basis/Basis.h>

// XML serialization dependencies
#include <gismo/IO/Xml.h>

template<class T>
class gsXml< gsBasis<T> >
{
private:
    gsXml() { }
public:
    GSXML_COMMON_FUNCTIONS(gsBasis<T>);
    static std::string tag () { return "Basis"; }
    static std::string type () { return ""; }

    static gsBasis<T> * get (gsXmlNode * node)
    {
        GISMO_ASSERT( ( !strcmp( node->name(),"Basis") ), "Something went wrong, waiting for a basis." );

        gsXmlAttribute * btype = node->first_attribute("type");
        if ( ! btype )
        {
            gsWarn<< "Basis without a type in the xml file.\n";
            return NULL;
        }
        std::string s = btype->value() ;

        // GSXML_GET_TYPE(gsBSplineBasis<T>)
        // GSXML_GET_TYPE(gsNurbsBasis<T>)

        GSXML_GET_TYPE(gsTHBSplineBasis<TMPLA2(1, T)>)
        // GSXML_GET_TYPE(gsTHBSplineBasis<TMPLA2(2, T)>)
        // GSXML_GET_TYPE(gsTHBSplineBasis<TMPLA2(3, T)>)
        // GSXML_GET_TYPE(gsTHBSplineBasis<TMPLA2(4, T)>)

        // GSXML_GET_TYPE(gsHBSplineBasis<TMPLA2(1, T)>)
        // GSXML_GET_TYPE(gsHBSplineBasis<TMPLA2(2, T)>)
        // GSXML_GET_TYPE(gsHBSplineBasis<TMPLA2(3, T)>)
        // GSXML_GET_TYPE(gsHBSplineBasis<TMPLA2(4, T)>)

        // GSXML_GET_TYPE(gsRationalTHBSplineBasis<TMPLA2(1, T)>)
        // GSXML_GET_TYPE(gsRationalTHBSplineBasis<TMPLA2(2, T)>)
        // GSXML_GET_TYPE(gsRationalTHBSplineBasis<TMPLA2(3, T)>)

        // GSXML_GET_TYPE(gsTensorBSplineBasis<TMPLA2(2, T)>)
        // GSXML_GET_TYPE(gsTensorBSplineBasis<TMPLA2(3, T)>)
        // GSXML_GET_TYPE(gsTensorBSplineBasis<TMPLA2(4, T)>)

        // GSXML_GET_TYPE(gsTensorNurbsBasis<TMPLA2(2, T)>)
        // GSXML_GET_TYPE(gsTensorNurbsBasis<TMPLA2(3, T)>)
        // GSXML_GET_TYPE(gsTensorNurbsBasis<TMPLA2(4, T)>)

        GSXML_GET_TYPE(gsComposedBasis<T>)

        gsWarn<<"gsXmlUtils: getBasis: No known basis \""<<s<<"\". Error.\n";
        return NULL;
    }

    static gsXmlNode * put (const gsBasis<T> & obj,
                            gsXmlTree & data )
    {
        const gsBasis<T> * ptr = & obj;

        GSXML_PUT_DYNAMIC_CAST(gsBSplineBasis<T>)
        GSXML_PUT_DYNAMIC_CAST(gsNurbsBasis<T>)

        // Tensor B-spline
        GSXML_PUT_DYNAMIC_CAST(gsTensorBSplineBasis<TMPLA2(2, T)>)
        GSXML_PUT_DYNAMIC_CAST(gsTensorBSplineBasis<TMPLA2(3, T)>)
        GSXML_PUT_DYNAMIC_CAST(gsTensorBSplineBasis<TMPLA2(4, T)>)

        // Tensor Nurbs
        GSXML_PUT_DYNAMIC_CAST(gsTensorNurbsBasis<TMPLA2(2, T)>)
        GSXML_PUT_DYNAMIC_CAST(gsTensorNurbsBasis<TMPLA2(3, T)>)
        GSXML_PUT_DYNAMIC_CAST(gsTensorNurbsBasis<TMPLA2(4, T)>)

        // Tensor-Hier. B-splines
        GSXML_PUT_DYNAMIC_CAST(gsHTensorBasis<TMPLA2(1, T)>)
        GSXML_PUT_DYNAMIC_CAST(gsHTensorBasis<TMPLA2(2, T)>)
        GSXML_PUT_DYNAMIC_CAST(gsHTensorBasis<TMPLA2(3, T)>)
        GSXML_PUT_DYNAMIC_CAST(gsHTensorBasis<TMPLA2(4, T)>)

        GSXML_PUT_DYNAMIC_CAST(gsTHBSplineBasis<TMPLA2(3, T)>)
        GSXML_PUT_DYNAMIC_CAST(gsRationalTHBSplineBasis<TMPLA2(3, T)>)

        //if ( const gsTriangularBezierBasis<2,T>  * g =
        //     dynamic_cast<const gsTriangularBezierBasis<2,T> *>( ptr ) )
        //    return gsXml< gsTriangularBezierBasis<2,T> >::put(*g,data);

        GSXML_PUT_DYNAMIC_CAST(gsComposedBasis<T>)

        gsWarn<<"gsXmlUtils put: getBasis: No known basis \""<<obj<<"\". Error.\n";
        return NULL;
    }
};
