#pragma once
#include "AI/Navigation/NavigationTypes.h"
#include "NavMesh/RecastNavMesh.h"

struct NavMeshFilterHeader
{
	unsigned short int IncludeFlags;
	unsigned short int ExcludeFlags;
	float Costs[RECAST_MAX_AREAS];
	float FixedCost[RECAST_MAX_AREAS];
	float Extent[3];
};

struct FRecastGeometryCache;

class NavMeshExporter
{
public:
	static bool ExportNavigationGeom(FText& err);
	static bool ExportNavmesh(FText& err);

private:
	static void TransformVertexSoupToRecast(const TArray<FVector>& VertexSoup, TNavStatArray<FVector>& Verts, TNavStatArray<int32>& Faces);
	static void ExportGeomToOBJFile(const FString& InFileName, const TNavStatArray<float>& GeomCoords, const TNavStatArray<int32>& GeomFaces, const FString& AdditionalData);
	static void GetRecastGeometryCacheFromData(FRecastGeometryCache& self, const uint8* Memory);
};
