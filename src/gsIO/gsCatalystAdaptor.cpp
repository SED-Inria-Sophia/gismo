#include <gsIO/gsCatalystAdaptor.h>

#include <gsUtils/gsUtils.h>
#include <gsIO/gsParaviewCollection.h>
#include <gsIO/gsWriteParaview.hpp>
#include <gsUtils/gsPointGrid.h>

#include <cstdlib>
#include <exception>
#include <vector>

#ifdef GISMO_WITH_CATALYST
#  include <catalyst.hpp>
#  include <catalyst_conduit.hpp>
#  include <conduit/conduit.hpp>
#  include <conduit/conduit_blueprint.hpp>
#  include <conduit/conduit_blueprint_mesh.hpp>
#endif

#ifdef GISMO_WITH_CATALYST
namespace
{
using gismo::real_t;
using gismo::index_t;

struct PointCloud
{
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> z;
};

PointCloud sampleMultiPatchGeometry(const gismo::gsMultiPatch<real_t> & geometry,
                                    unsigned npts)
{
    PointCloud cloud;
    const index_t patches = geometry.nPatches();
    cloud.x.reserve(patches * npts);
    cloud.y.reserve(patches * npts);
    cloud.z.reserve(patches * npts);

    for (index_t p = 0; p < patches; ++p)
    {
        const auto & patch = geometry.patch(p);
        gsMatrix<real_t> ab = patch.support();
        gsVector<real_t> a = ab.col(0);
        gsVector<real_t> b = ab.col(1);
        const gsVector<unsigned> np = uniformSampleCount(a, b, npts);
        const gsMatrix<real_t> paramPts = gsPointGrid(a, b, np);
        const gsMatrix<real_t> evaluated = patch.eval(paramPts);

        for (index_t c = 0; c < evaluated.cols(); ++c)
        {
            cloud.x.push_back(static_cast<double>(evaluated(0, c)));
            cloud.y.push_back(evaluated.rows() > 1 ? static_cast<double>(evaluated(1, c)) : 0.0);
            cloud.z.push_back(evaluated.rows() > 2 ? static_cast<double>(evaluated(2, c)) : 0.0);
        }
    }

    return cloud;
}

PointCloud fromMatrix(const gismo::gsMatrix<real_t> & points)
{
    PointCloud cloud;
    cloud.x.reserve(points.cols());
    cloud.y.reserve(points.cols());
    cloud.z.reserve(points.cols());

    for (index_t c = 0; c < points.cols(); ++c)
    {
        cloud.x.push_back(static_cast<double>(points(0, c)));
        cloud.y.push_back(points.rows() > 1 ? static_cast<double>(points(1, c)) : 0.0);
        cloud.z.push_back(points.rows() > 2 ? static_cast<double>(points(2, c)) : 0.0);
    }

    return cloud;
}

void addPointCloudChannel(const PointCloud & cloud,
                          const std::string & channelName,
                          conduit::Node & execParams)
{
    conduit::Node &channel = execParams["catalyst/channels/" + channelName];
    channel["type"] = "mesh";
    channel["name"] = channelName;
    conduit::Node &mesh = channel["data"];

    mesh["coordsets/coords/type"] = "explicit";
    mesh["coordsets/coords/values/x"].set(cloud.x);
    mesh["coordsets/coords/values/y"].set(cloud.y);
    mesh["coordsets/coords/values/z"].set(cloud.z);

    mesh["topologies/points/type"] = "points";
    mesh["topologies/points/coordset"] = "coords";
}
}
#endif

namespace gismo
{

gsCatalystAdaptor::gsCatalystAdaptor()
    : m_initialized(false)
    , m_timeStep(0)
{
}

gsCatalystAdaptor::~gsCatalystAdaptor()
{
    if (m_initialized)
        finalize();
}

void gsCatalystAdaptor::initialize()
{
    if (m_initialized)
        return;

#ifdef GISMO_WITH_CATALYST
    try
    {
        conduit::Node params;
        if (const char * script = std::getenv("GISMO_CATALYST_SCRIPT"))
        {
            conduit::Node &scriptNode = params["catalyst/scripts/script1"];
            scriptNode["filename"] = std::string(script);
        }
        catalyst::Initialize(params);
    }
    catch (const std::exception &ex)
    {
        gsWarn << "Catalyst initialisation failed: " << ex.what() << "\n";
    }
#endif

    m_initialized = true;
}

void gsCatalystAdaptor::finalize()
{
    if (!m_initialized)
        return;

#ifdef GISMO_WITH_CATALYST
    try
    {
        conduit::Node params;
        catalyst::Finalize(params);
    }
    catch (const std::exception &ex)
    {
        gsWarn << "Catalyst finalisation failed: " << ex.what() << "\n";
    }
#endif

    m_initialized = false;
}

void gsCatalystAdaptor::publishMultiPatch(const gsMultiPatch<real_t> & geometry,
                                          const std::string & baseName,
                                          const gsMultiBasis<real_t> * basis,
                                          unsigned npts) const
{
#ifdef GISMO_WITH_CATALYST
    try
    {
        conduit::Node execParams;
        execParams["catalyst/state/time"] = static_cast<double>(m_timeStep);
        execParams["catalyst/state/timestep"] = static_cast<long long>(m_timeStep);

        const PointCloud cloud = sampleMultiPatchGeometry(geometry, npts);
        addPointCloudChannel(cloud, baseName, execParams);

        catalyst::Execute(execParams);
    }
    catch (const std::exception &ex)
    {
        gsWarn << "Catalyst publishMultiPatch failed: " << ex.what() << "\n";
    }
#else
    if (basis)
        gsWriteParaview(*basis, geometry, baseName, npts);
    else
        gsWriteParaview(geometry, baseName, npts);
#endif

    ++m_timeStep;
}

void gsCatalystAdaptor::publishPoints(const gsMatrix<real_t> & points,
                                      const std::string & baseName) const
{
#ifdef GISMO_WITH_CATALYST
    try
    {
        conduit::Node execParams;
        execParams["catalyst/state/time"] = static_cast<double>(m_timeStep);
        execParams["catalyst/state/timestep"] = static_cast<long long>(m_timeStep);

        const PointCloud cloud = fromMatrix(points);
        addPointCloudChannel(cloud, baseName, execParams);

        catalyst::Execute(execParams);
    }
    catch (const std::exception &ex)
    {
        gsWarn << "Catalyst publishPoints failed: " << ex.what() << "\n";
    }
#else
    gsWriteParaviewPoints(points, baseName);
#endif
}

void gsCatalystAdaptor::publishBezier(const gsMultiPatch<real_t> & geometry,
                                      const std::string & baseName,
                                      bool ctrlNet) const
{
#ifdef GISMO_WITH_CATALYST
    try
    {
        conduit::Node execParams;
        execParams["catalyst/state/time"] = static_cast<double>(m_timeStep);
        execParams["catalyst/state/timestep"] = static_cast<long long>(m_timeStep);

        const PointCloud cloud = sampleMultiPatchGeometry(geometry, 1000u);
        addPointCloudChannel(cloud, baseName, execParams);

        catalyst::Execute(execParams);
    }
    catch (const std::exception &ex)
    {
        gsWarn << "Catalyst publishBezier failed: " << ex.what() << "\n";
    }
#endif

    gsWriteParaviewBezier(geometry, baseName, ctrlNet);
}

gsCatalystAdaptor::Collection
::Collection(gsCatalystAdaptor & adaptor,
             const std::string & baseName,
             gsExprEvaluator<real_t> * evaluator)
    : m_adaptor(adaptor)
    , m_collection(baseName, evaluator)
{}

gsOptionList & gsCatalystAdaptor::Collection::options()
{
    return m_collection.options();
}

void gsCatalystAdaptor::Collection::newTimeStep(gsMultiPatch<real_t> * geometry,
                                                real_t time)
{
    m_collection.newTimeStep(geometry, time);
}

void gsCatalystAdaptor::Collection::saveTimeStep()
{
    m_collection.saveTimeStep();
}

void gsCatalystAdaptor::Collection::save()
{
    m_collection.save();
}

gsCatalystAdaptor::Collection
gsCatalystAdaptor::createCollection(const std::string & baseName,
                                    gsExprEvaluator<real_t> * evaluator)
{
    initialize();
    return Collection(*this, baseName, evaluator);
}

} // namespace gismo
