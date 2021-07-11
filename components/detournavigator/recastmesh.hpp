#ifndef OPENMW_COMPONENTS_DETOURNAVIGATOR_RECASTMESH_H
#define OPENMW_COMPONENTS_DETOURNAVIGATOR_RECASTMESH_H

#include "areatype.hpp"
#include "bounds.hpp"

#include <components/bullethelpers/operators.hpp>

#include <memory>
#include <string>
#include <vector>
#include <tuple>

#include <LinearMath/btTransform.h>

namespace DetourNavigator
{
    class Mesh
    {
    public:
        Mesh(std::vector<int>&& indices, std::vector<float>&& vertices, std::vector<AreaType>&& areaTypes);

        const std::vector<int>& getIndices() const noexcept { return mIndices; }
        const std::vector<float>& getVertices() const noexcept { return mVertices; }
        const std::vector<AreaType>& getAreaTypes() const noexcept { return mAreaTypes; }
        std::size_t getVerticesCount() const noexcept { return mVertices.size() / 3; }
        std::size_t getTrianglesCount() const noexcept { return mAreaTypes.size(); }

    private:
        std::vector<int> mIndices;
        std::vector<float> mVertices;
        std::vector<AreaType> mAreaTypes;

        friend inline bool operator<(const Mesh& lhs, const Mesh& rhs) noexcept
        {
            return std::tie(lhs.mIndices, lhs.mVertices, lhs.mAreaTypes)
                    < std::tie(rhs.mIndices, rhs.mVertices, rhs.mAreaTypes);
        }

        friend inline std::size_t getSize(const Mesh& value) noexcept
        {
            return value.mIndices.size() * sizeof(int)
                + value.mVertices.size() * sizeof(float)
                + value.mAreaTypes.size() * sizeof(AreaType);
        }
    };

    class RecastMesh
    {
    public:
        struct Water
        {
            int mCellSize;
            btTransform mTransform;
        };

        RecastMesh(std::size_t generation, std::size_t revision, Mesh mesh, std::vector<Water> water);

        std::size_t getGeneration() const
        {
            return mGeneration;
        }

        std::size_t getRevision() const
        {
            return mRevision;
        }

        const Mesh& getMesh() const noexcept { return mMesh; }

        const std::vector<Water>& getWater() const
        {
            return mWater;
        }

        const Bounds& getBounds() const
        {
            return mBounds;
        }

    private:
        std::size_t mGeneration;
        std::size_t mRevision;
        Mesh mMesh;
        std::vector<Water> mWater;
        Bounds mBounds;

        friend inline bool operator <(const RecastMesh& lhs, const RecastMesh& rhs) noexcept
        {
            return std::tie(lhs.mMesh, lhs.mWater) < std::tie(rhs.mMesh, rhs.mWater);
        }

        friend inline std::size_t getSize(const RecastMesh& value) noexcept
        {
            return getSize(value.mMesh) + value.mWater.size() * sizeof(RecastMesh::Water);
        }
    };

    inline bool operator<(const RecastMesh::Water& lhs, const RecastMesh::Water& rhs) noexcept
    {
        return std::tie(lhs.mCellSize, lhs.mTransform) < std::tie(rhs.mCellSize, rhs.mTransform);
    }
}

#endif
