#include <Dunjun/World.hpp>
#include <Dunjun/SceneGraph.hpp>
#include <Dunjun/Core/Memory.hpp>

#include <Dunjun/ResourceHolders.hpp>

#include <cstdio>

namespace Dunjun
{
World::World()
: entitySystem{}
, sceneGraph{defaultAllocator()}
, renderSystem{defaultAllocator(), sceneGraph}
, names{}
, camera{}
{
}
} // namespace Dunjun
