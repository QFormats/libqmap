#include <iostream>
#include <string>
#include <vector>
#include "wrapper.h"

API_EXPORT qformats::map::QMap *LoadMap(const char *mapFile, qformats::map::textureBounds(getTextureBounds)(const char *))
{
    auto map = new qformats::map::QMap();
    map->LoadFile(mapFile, getTextureBounds);
    return map;
}

API_EXPORT void GenerateGeometry(qformats::map::QMap *ptr, int *outPointEntCount, int *outSolidEntCount, bool clipBrushes = true)
{
    ptr->GenerateGeometry(clipBrushes);
    *outSolidEntCount = (int)ptr->GetSolidEntities().size();
    *outPointEntCount = (int)ptr->GetPointEntities().size();
}

API_EXPORT void SetFaceType(qformats::map::QMap *ptr, const char *textureName, uint8_t faceType)
{
    ptr->SetFaceTypeByTextureID(textureName, (qformats::map::Face::eFaceType)faceType);
}

API_EXPORT void GetTextures(qformats::map::QMap *ptr, void(add)(const char *tex))
{
    for (auto texName : ptr->GetTexturesNames())
    {
        add(texName.c_str());
    }
}

API_EXPORT void GetEntityAttributes(qformats::map::QMap *ptr, uint8_t type, int idx, qformats::map::BaseEntity **entOut, void(add)(const char *first, const char *second))
{
    if (entOut == nullptr || ptr == nullptr)
    {
        return;
    }

    if (type == 0 && ptr->GetSolidEntities().size() > idx)
    {
        *entOut = ptr->GetSolidEntities()[idx].get();
    }
    else if (type == 1 && ptr->GetPointEntities().size() > idx)
    {
        *entOut = ptr->GetPointEntities()[idx].get();
    }

    if ((*entOut) == nullptr)
    {
        return;
    }

    add("classname", (*entOut)->classname.c_str());
    for (const auto &pair : (*entOut)->attributes)
    {
        add(pair.first.c_str(), pair.second.c_str());
    }
}

API_EXPORT QMapPointEntity GetPointEntityData(qformats::map::BaseEntity *ptr)
{
    QMapPointEntity outData{};
    if (ptr != nullptr)
    {
        outData.origin = toVec3(ptr->origin);
        outData.angle = ptr->angle;
    }

    return outData;
}

API_EXPORT QMapSolidEntity GetSolidEntityData(qformats::map::SolidEntity *ptr)
{
    QMapSolidEntity outData{};
    if (ptr != nullptr)
    {
        outData.center = toVec3(ptr->GetCenter());
        outData.min = toVec3(ptr->GetMin());
        outData.max = toVec3(ptr->GetMax());
    }
    return outData;
}

API_EXPORT void GetSolidEntityBrushes(qformats::map::SolidEntity *ent, void(add)(int idx, vec3 min, vec3 max))
{
    const auto brushes = ent->GetClippedBrushes();
    for (int i = 0; i < brushes.size(); i++)
    {
        add(i, toVec3(brushes[i].min), toVec3(brushes[i].max));
    }
}

API_EXPORT void GetBrushFaces(qformats::map::SolidEntity *ent, int brushIdx, void(add)(int idx, int texID, uint8_t faceType))
{
    if (ent == nullptr || ent->GetClippedBrushes().size() < brushIdx)
    {
        return;
    }
    const auto &brush = ent->GetClippedBrushes()[brushIdx];
    const auto nativeFaces = brush.GetFaces();

    for (int i = 0; i < nativeFaces.size(); i++)
    {
        add(i, nativeFaces[i]->TextureID(), nativeFaces[i]->Type());
    }
}

API_EXPORT void GetFaceData(qformats::map::SolidEntity *ent, int brushIdx, int faceIdx, vert **verts, unsigned short **indices, int *vertCount, int *indexCount)
{
    if (ent == nullptr || ent->GetClippedBrushes().size() < brushIdx)
    {
        return;
    }
    const auto &brush = ent->GetClippedBrushes()[brushIdx];
    const auto &face = brush.GetFaces()[faceIdx];

    *vertCount = (int)face->GetVertices().size();
    *indexCount = (int)face->GetIndices().size();

    if (vertCount == 0)
    {
        return;
    }

    *verts = (vert *)CoTaskMemAlloc(*vertCount * sizeof(vert));
    for (int i = 0; i < *vertCount; i++)
    {
        const auto &vert = face->GetVertices()[i];
        (*verts)[i].pos = toVec3(vert.point);
        (*verts)[i].normal = toVec3(vert.normal);
        (*verts)[i].tangent = toVec4(vert.tangent);
        (*verts)[i].uv = toVec2(vert.uv);
    }

    *indices = (unsigned short *)CoTaskMemAlloc(*indexCount * sizeof(unsigned short));
    for (int i = 0; i < *indexCount; i++)
    {
        (*indices)[i] = face->GetIndices()[i];
    }
}

API_EXPORT void DestroyMap(qformats::map::QMap *ptr)
{
    delete ptr;
}