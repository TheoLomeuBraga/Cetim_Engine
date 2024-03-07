#pragma once

#include "RecursosT.h"
#include "game_object.h"
#include "transform.h"
#include "render_mesh.h"

#include "Recast.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshQuery.h"
#include "DetourNavMeshBuilder.h"
#include "DetourTileCache.h"

unsigned int rcMeshQuality = 1;

void navmesh_calcBminBmax(shared_ptr<malha> minhaMalha, float bmin[3], float bmax[3])
{
    if (minhaMalha->vertices.empty() || minhaMalha->indice.empty())
    {
        return;
    }

    // Inicializando bmin e bmax com valores extremos
    bmin[0] = bmin[1] = bmin[2] = std::numeric_limits<float>::max();
    bmax[0] = bmax[1] = bmax[2] = std::numeric_limits<float>::lowest();

    std::unordered_set<unsigned int> uniqueIndices(minhaMalha->indice.begin(), minhaMalha->indice.end());

    // Percorrendo apenas os índices únicos
    for (const auto &index : uniqueIndices)
    {
        const vertice &v = minhaMalha->vertices[index];
        for (int i = 0; i < 3; ++i)
        {
            bmin[i] = std::min(bmin[i], v.posicao[i]);
            bmax[i] = std::max(bmax[i], v.posicao[i]);
        }
    }
}

void navmesh_freeRcPolyMesh(rcPolyMesh *mesh)
{
    if (!mesh)
        return; // Se o ponteiro é nulo, não há nada a liberar.

    // Liberando vértices
    if (mesh->verts)
    {
        delete[] mesh->verts;
        mesh->verts = nullptr;
    }

    // Liberando polígonos
    if (mesh->polys)
    {
        delete[] mesh->polys;
        mesh->polys = nullptr;
    }

    // Adicione aqui a liberação de quaisquer outros recursos alocados dentro de rcPolyMesh.

    // Por fim, libere o próprio rcPolyMesh
    delete mesh;
}

void navmesh_fillVerticesAndIndices(rcPolyMesh *polyMesh, std::shared_ptr<malha> minhaMalha)
{

    // Preencher vértices
    polyMesh->nverts = minhaMalha->vertices.size();
    polyMesh->verts = new unsigned short[polyMesh->nverts * 3];

    for (size_t i = 0; i < minhaMalha->vertices.size(); ++i)
    {
        const vertice &vert = minhaMalha->vertices[i];
        for (int j = 0; j < 3; ++j)
        {
            polyMesh->verts[i * 3 + j] = static_cast<unsigned short>(std::round((vert.posicao[j] - polyMesh->bmin[j]) / rcMeshQuality));
        }
    }

    // Preencher índices dos polígonos
    polyMesh->npolys = minhaMalha->indice.size() / polyMesh->nvp;
    polyMesh->polys = new unsigned short[polyMesh->npolys * 2 * polyMesh->nvp];

    for (size_t i = 0; i < polyMesh->npolys; ++i)
    {
        for (int j = 0; j < polyMesh->nvp; ++j)
        {
            if (j < 3)
            {
                // índices de vértices do polígono
                polyMesh->polys[i * 2 * polyMesh->nvp + j] = minhaMalha->indice[i * 3 + j];
            }
            else
            {
                // Resto preenchido com RC_MESH_NULL_IDX
                polyMesh->polys[i * 2 * polyMesh->nvp + j] = RC_MESH_NULL_IDX;
            }

            // índices de borda
            polyMesh->polys[i * 2 * polyMesh->nvp + polyMesh->nvp + j] = RC_MESH_NULL_IDX;
        }
    }

    // Aqui, você pode adicionar lógica adicional se necessário, como configuração de áreas navegáveis.
}

std::shared_ptr<malha> convert_polyMesh_to_mesh(const rcPolyMesh *polyMesh)
{
    if (!polyMesh)
    {
        return nullptr;
    }

    malha convertedMesh;

    // Convertendo vértices
    for (int i = 0; i < polyMesh->nverts; ++i)
    {
        const unsigned short *v = &polyMesh->verts[i * 3];
        vertice vert;

        // Convertendo as coordenadas do vértice para o seu sistema de coordenadas
        vert.posicao[0] = polyMesh->bmin[0] + v[0] * polyMesh->cs;
        vert.posicao[1] = polyMesh->bmin[1] + v[1] * polyMesh->ch;
        vert.posicao[2] = polyMesh->bmin[2] + v[2] * polyMesh->cs;

        // Preencher outros atributos do vértice, se necessário
        // ...

        convertedMesh.vertices.push_back(vert);
    }

    // Convertendo índices dos polígonos
    for (int i = 0; i < polyMesh->npolys; ++i)
    {
        for (int j = 0; j < polyMesh->nvp; ++j)
        {
            unsigned short vertexIndex = polyMesh->polys[i * polyMesh->nvp * 2 + j];

            // Verificar se o índice é válido (não é uma borda ou índice nulo)
            if (vertexIndex == RC_MESH_NULL_IDX || vertexIndex >= polyMesh->nverts)
            {
                break;
            }

            convertedMesh.indice.push_back(vertexIndex);
        }
    }

    return make_shared<malha>(convertedMesh);
}

std::shared_ptr<malha> convert_nav_mesh_to_mesh(const dtNavMesh *nMesh)
{
    if (!nMesh)
    {
        return nullptr;
    }

    std::shared_ptr<malha> convertedMesh = std::make_shared<malha>();
    for (int i = 0; i < nMesh->getMaxTiles(); ++i)
    {
        const dtMeshTile *tile = nMesh->getTile(i);
        if (!tile || !tile->header)
        {
            continue;
        }

        for (int vi = 0; vi < tile->header->vertCount; ++vi)
        {
            const float *v = &tile->verts[vi * 3];
            vertice_struct vertex;
            vertex.posicao[0] = v[0];
            vertex.posicao[1] = v[1];
            vertex.posicao[2] = v[2];
            convertedMesh->vertices.push_back(vertex);
        }

        for (int pi = 0; pi < tile->header->polyCount; ++pi)
        {
            const dtPoly *poly = &tile->polys[pi];

            for (int pj = 0; pj < poly->vertCount; ++pj)
            {
                convertedMesh->indice.push_back(poly->verts[pj]);
            }
        }
    }

    return convertedMesh;
}

/*
void navmesh_fillBorderIndexes(rcPolyMesh *polyMesh)
{
    if (!polyMesh)
        return;

    // Assuming `polyMesh->polys` is your array of polygons and `polyMesh->npolys` is the number of polygons.
    for (int i = 0; i < polyMesh->npolys; ++i)
    {
        unsigned short *poly = &polyMesh->polys[i * 2 * polyMesh->nvp];

        // Iterate through edges of the polygon.
        for (int j = 0; j < polyMesh->nvp; ++j)
        {
            if (poly[j] == RC_MESH_NULL_IDX)
                break; // End of the polygon vertices.

            int nj = (j + 1) % polyMesh->nvp;
            if (poly[nj] == RC_MESH_NULL_IDX)
                nj = 0;

            int edgeStart = poly[j];
            int edgeEnd = poly[nj];

            bool isBorder = true;
            // Check if this edge is shared with any other polygon.
            for (int k = 0; k < polyMesh->npolys; ++k)
            {
                if (k == i)
                    continue; // Skip the same polygon.

                unsigned short *otherPoly = &polyMesh->polys[k * 2 * polyMesh->nvp];
                for (int l = 0; l < polyMesh->nvp; ++l)
                {
                    if (otherPoly[l] == RC_MESH_NULL_IDX)
                        break;

                    int nl = (l + 1) % polyMesh->nvp;
                    if (otherPoly[nl] == RC_MESH_NULL_IDX)
                        nl = 0;

                    if ((otherPoly[l] == edgeStart && otherPoly[nl] == edgeEnd) ||
                        (otherPoly[l] == edgeEnd && otherPoly[nl] == edgeStart))
                    {
                        // Edge is shared, so not a border.
                        isBorder = false;
                        polyMesh->polys[i * 2 * polyMesh->nvp + polyMesh->nvp + j] = k; // Update neighbor index.
                        print("not a border",i * 2 * polyMesh->nvp + polyMesh->nvp + j,k);
                        break;
                    }
                }
                if (!isBorder)
                    break;
            }

            if (isBorder)
            {
                // Mark as a border edge.
                polyMesh->polys[i * 2 * polyMesh->nvp + polyMesh->nvp + j] = RC_MESH_NULL_IDX;
            }
        }
    }
}
*/

/*
void navmesh_fillBorderIndexes(rcPolyMesh *polyMesh) {
    if (polyMesh == nullptr) return;

    const int nvp = polyMesh->nvp; // The maximum number of vertices per polygon

    // Iterate over each polygon
    for (int i = 0; i < polyMesh->npolys; ++i) {
        unsigned short *p = &polyMesh->polys[i * 2 * nvp];

        // Check each edge of the polygon
        for (int j = 0; j < nvp; ++j) {
            if (p[j] == RC_MESH_NULL_IDX) break; // End of this polygon's vertices

            if (p[nvp + j] != RC_MESH_NULL_IDX) continue; // Skip if already has a neighbor

            // Assume it's a border edge by default
            bool isBorderEdge = true;

            // Check if the edge is shared with another polygon
            for (int k = 0; k < polyMesh->npolys; ++k) {
                if (i == k) continue; // Don't check the same polygon

                unsigned short *q = &polyMesh->polys[k * 2 * nvp];

                for (int l = 0; l < nvp; ++l) {
                    if (q[l] == RC_MESH_NULL_IDX) break; // End of the other polygon's vertices

                    // Check if the current edge of polygon i is shared with polygon k
                    if ((p[j] == q[l] && p[j == nvp - 1 ? 0 : j + 1] == q[l == 0 ? nvp - 1 : l - 1]) ||
                        (p[j] == q[l == 0 ? nvp - 1 : l - 1] && p[j == nvp - 1 ? 0 : j + 1] == q[l])) {
                        // Edge is shared with another polygon
                        isBorderEdge = false;
                        p[nvp + j] = k; // Update the neighbor index
                        break;
                    }
                }

                if (!isBorderEdge) break; // No need to check further if edge is already marked as shared
            }

            if (isBorderEdge) {
                p[nvp + j] = RC_MESH_NULL_IDX; // Mark the edge as a border
            }
        }
    }
}
*/

void navmesh_fillBorderIndexes(rcPolyMesh *polyMesh)
{
    if (polyMesh == nullptr)
        return;

    const int nvp = polyMesh->nvp; // The maximum number of vertices per polygon

    // Iterate over each polygon
    for (int i = 0; i < polyMesh->npolys; ++i)
    {
        unsigned short *p = &polyMesh->polys[i * 2 * nvp];

        // Check each edge of the polygon
        for (int j = 0; j < nvp; ++j)
        {
            if (p[j] == RC_MESH_NULL_IDX)
                break; // End of this polygon's vertices

            if (p[nvp + j] != RC_MESH_NULL_IDX)
                continue; // Skip if already has a neighbor

            // Assume it's a border edge by default
            bool isBorderEdge = true;

            // Check if the edge is shared with another polygon
            for (int k = 0; k < polyMesh->npolys; ++k)
            {
                if (i == k)
                    continue; // Don't check the same polygon

                unsigned short *q = &polyMesh->polys[k * 2 * nvp];

                for (int l = 0; l < nvp; ++l)
                {
                    if (q[l] == RC_MESH_NULL_IDX)
                        break; // End of the other polygon's vertices

                    // Check if the current edge of polygon i is shared with polygon k
                    if ((p[j] == q[l] && p[j == nvp - 1 ? 0 : j + 1] == q[l == 0 ? nvp - 1 : l - 1]) ||
                        (p[j] == q[l == 0 ? nvp - 1 : l - 1] && p[j == nvp - 1 ? 0 : j + 1] == q[l]))
                    {
                        // Edge is shared with another polygon
                        isBorderEdge = false;
                        p[nvp + j] = k; // Update the neighbor index
                        break;
                    }
                }

                if (!isBorderEdge)
                    break; // No need to check further if edge is already marked as shared
            }

            if (isBorderEdge)
            {
                p[nvp + j] = RC_MESH_NULL_IDX; // Mark the edge as a border
            }
        }
    }
}

rcPolyMesh *navmesh_convertToRcPolyMesh(std::shared_ptr<malha> minhaMalha)
{
    if (!minhaMalha)
    {
        return nullptr;
    }

    rcPolyMesh *polyMesh = new rcPolyMesh();
    if (!polyMesh)
    {
        return nullptr;
    }

    // Calcular bmin e bmax da malha
    navmesh_calcBminBmax(minhaMalha, polyMesh->bmin, polyMesh->bmax);

    // Definir cell size e height
    polyMesh->cs = rcMeshQuality;
    polyMesh->ch = rcMeshQuality;

    // difine polygon size
    polyMesh->nvp = 3;

    // Preencher os dados de vértices e índices
    navmesh_fillVerticesAndIndices(polyMesh, minhaMalha);
    navmesh_fillBorderIndexes(polyMesh);

    return polyMesh;
}

void navmesh_locate_dtNavMeshCreateParams_error(dtNavMeshCreateParams *params)
{
    if (params->nvp > DT_VERTS_PER_POLYGON)
    {
        print("params->nvp > DT_VERTS_PER_POLYGON");
        return;
    }
    if (params->vertCount >= 0xffff)
    {
        print("params->vertCount >= 0xffff");
        return;
    }
    if (!params->vertCount || !params->verts)
    {
        print("!params->vertCount || !params->verts");
        return;
    }
    if (!params->polyCount || !params->polys)
    {
        print("!params->polyCount || !params->polys");
        return;
    }
    unsigned char *offMeshConClass = 0;
    if (params->offMeshConCount > 0)
    {
        offMeshConClass = (unsigned char *)dtAlloc(sizeof(unsigned char) * params->offMeshConCount * 2, DT_ALLOC_TEMP);
        if (!offMeshConClass)
        {
            print("params->offMeshConCount > 0");
            return;
        }
    }
    print("no error");
}

float navmesh_color_intensity = 0.0;

void navmesh_print_cube_in_space(vec3 pos)
{

    // ManuseioDados::importar_obj("resources/3D Models/oclusion_box.obj")->malhas["Cube"]

    shared_ptr<objeto_jogo> display_nav_mesh = novo_objeto_jogo();
    display_nav_mesh->adicionar_componente<transform_>();
    display_nav_mesh->pegar_componente<transform_>()->pos = pos;
    display_nav_mesh->pegar_componente<transform_>()->esca = vec3(0.25, 0.25, 0.25);

    display_nav_mesh->adicionar_componente<render_malha>();
    shared_ptr<render_malha> rm = display_nav_mesh->pegar_componente<render_malha>();

    rm->usar_oclusao = false;
    rm->camada = 4;

    Material mat;
    mat.shad = "resources/Shaders/mesh";
    mat.cor = vec4(navmesh_color_intensity, 0.0, 0.5, 1);
    navmesh_color_intensity += 0.5;
    if (navmesh_color_intensity > 1)
    {
        navmesh_color_intensity = 0;
    }
    mat.texturas[0] = ManuseioDados::carregar_Imagem("resources/Textures/white.png");

    rm->malhas = {ManuseioDados::importar_obj("resources/3D Models/oclusion_box.obj")->malhas["Cube"]};

    rm->mats = {mat};
    cena_objetos_selecionados->adicionar_objeto(display_nav_mesh);

    print("cube pos:", pos.x, pos.y, pos.z);
}

class navmesh : public componente
{
private:
    rcPolyMesh *rcmeshe = NULL;
    dtNavMesh *navMesh = NULL;
    int navDataSize = 0;
    unsigned char *navData = NULL;
    unsigned char *polyAreas = NULL;
    unsigned short *polyFlags = NULL;

    shared_ptr<malha> internal_path_mesh = NULL;

    shared_ptr<objeto_jogo> display = NULL;
    shared_ptr<render_malha> rm = NULL;

public:
    std::string tag = "";
    static float navmeshQuality;
    static std::map<std::string, shared_ptr<objeto_jogo>> navmeshes;

    shared_ptr<malha> path_mesh = NULL;

    navmesh() {}

    void clear()
    {

        navDataSize = 0;
        internal_path_mesh = NULL;

        if (rcmeshe)
        {
            free(rcmeshe);
            rcmeshe = NULL;
        }

        if (navMesh)
        {
            free(navMesh);
            navMesh = NULL;
        }

        if (navData)
        {
            free(navData);
            navData = NULL;
        }

        if (polyAreas)
        {
            free(polyAreas);
            polyAreas = NULL;
        }

        if (polyFlags)
        {
            free(polyFlags);
            polyFlags = NULL;
        }

        if (navmeshes.find(tag) == navmeshes.end())
        {
            navmeshes.erase(tag);
        }
    }

    void show_this(bool on = true)
    {

        if (display)
        {
            cena_objetos_selecionados->remover_objeto(display);
        }

        rm = NULL;
        display = NULL;

        if (on && navMesh)
        {

            display = novo_objeto_jogo();
            display->adicionar_componente<transform_>();
            display->adicionar_componente<render_malha>();
            rm = display->pegar_componente<render_malha>();
            rm->usar_oclusao = false;
            rm->camada = 4;

            Material mat;
            mat.shad = "resources/Shaders/mesh";
            mat.cor = vec4(0.1, 0.1, 1, 0.5);
            mat.texturas[0] = ManuseioDados::carregar_Imagem("resources/Textures/white.png");

            // rm->malhas = {convert_nav_mesh_to_mesh(navMesh)};
            rm->malhas = {convert_polyMesh_to_mesh(rcmeshe)};

            rm->mats = {mat};
            cena_objetos_selecionados->adicionar_objeto(display);
        }
    }

    static void show(bool on = true)
    {
        for (pair<std::string, shared_ptr<objeto_jogo>> p : navmeshes)
        {
            p.second->pegar_componente<navmesh>()->show_this(on);
        }
    }

    void apply()
    {
        clear();

        if (esse_objeto->pegar_componente<transform_>())
        {

            navMesh = new dtNavMesh();

            glm::mat4 tfm = esse_objeto->pegar_componente<transform_>()->pegar_matriz();
            internal_path_mesh = apply_transformation_to_mesh(path_mesh, tfm);
            rcmeshe = navmesh_convertToRcPolyMesh(internal_path_mesh);

            dtNavMeshCreateParams params;
            memset(&params, 0, sizeof(params));
            params.verts = rcmeshe->verts;
            params.vertCount = rcmeshe->nverts;
            params.polys = rcmeshe->polys;
            params.polyCount = rcmeshe->npolys;
            params.nvp = rcmeshe->nvp;
            params.cs = rcmeshe->cs;
            params.ch = rcmeshe->ch;
            memcpy(params.bmin, rcmeshe->bmin, sizeof(params.bmin));
            memcpy(params.bmax, rcmeshe->bmax, sizeof(params.bmax));
            params.walkableHeight = 2.0f;
            params.walkableRadius = 1.0f;
            params.walkableClimb = 0.0f;
            params.tileX = 0;
            params.tileY = 0;
            params.tileLayer = 0;
            params.buildBvTree = true;

            polyAreas = new unsigned char[rcmeshe->npolys];
            polyFlags = new unsigned short[rcmeshe->npolys * 2]; // *2 para incluir flags de borda
            for (int i = 0; i < rcmeshe->npolys; ++i)
            {
                polyAreas[i] = 63; // Define todas as áreas como caminháveis
                // Configura os flags para cada polígono e suas bordas
                for (int j = 0; j < 2; ++j)
                {
                    polyFlags[i * 2 + j] = 0x01; // Flag indicando que as bordas são atravessáveis
                }
            }
            params.polyAreas = polyAreas;
            params.polyFlags = polyFlags;

            if (!dtCreateNavMeshData(&params, &navData, &navDataSize))
            {
                clear();
                return;
            }

            // Inicializar dtNavMesh com os dados de navegação
            if (dtStatusFailed(navMesh->init(navData, navDataSize, DT_TILE_FREE_DATA)))
            {
                clear();
                return;
            }

            navmeshes.insert(pair<std::string, shared_ptr<objeto_jogo>>(tag, esse_objeto));
        }
    }

    std::vector<glm::vec3> generate_this_path(glm::vec3 start, glm::vec3 end)
    {

        std::vector<glm::vec3> path;

        if (!navMesh)
        {
            print("no path found no navmesh");
            return {}; // Retorna um vetor vazio se a malha de navegação não for fornecida
        }

        dtNavMeshQuery query;
        query.init(navMesh, 2048); // 2048 é o tamanho máximo do conjunto de nós da consulta

        // Converter glm::vec3 para o formato do Detour
        float startPos[3] = {start.x, start.y, start.z};
        float endPos[3] = {end.x, end.y, end.z};

        // Configurações de filtro baseadas nas habilidades do personagem
        dtQueryFilter filter;
        // unsigned short includeFlags = canJump ? 0x08 : 0x01; // Flags de inclusão
        filter.setIncludeFlags(0x01);
        filter.setExcludeFlags(0);    // Sem flags de exclusão
        filter.setAreaCost(63, 1.0f); // Custo padrão para áreas caminháveis

        float tolerance[3] = {1, 2, 1}; // Tolerância para encontrar o polígono mais próximo

        float start_pos_ptr[3] = {0,0,0};
        float end_pos_ptr[3] = {0,0,0};

        // Encontrar os polígonos mais próximos de start e end
        dtPolyRef startRef, endRef;
        query.findNearestPoly(startPos, tolerance, &filter, &startRef, start_pos_ptr);
        query.findNearestPoly(endPos, tolerance, &filter, &endRef, end_pos_ptr);

        glm::vec3 start_pos = glm::vec3(start_pos_ptr[0],start_pos_ptr[1],start_pos_ptr[2]);
        glm::vec3 end_pos = glm::vec3(end_pos_ptr[0],end_pos_ptr[1],end_pos_ptr[2]);

        if (startRef == 0 || endRef == 0)
        {
            print("no path found no start or end point was found", startRef, endRef);
            return {}; // Não foram encontrados polígonos de referência válidos
        }

        // Calcular o caminho
        dtPolyRef pathPolys[256]; // Tamanho máximo do caminho
        int nPathPolys;
        query.findPath(startRef, endRef, startPos, endPos, &filter, pathPolys, &nPathPolys, 256);

        if (nPathPolys == 0)
        {
            print("no path found couldnt construct path");
            return {}; // Caminho não encontrado
        }

        // Obter pontos do caminho
        float straightPath[256 * 3]; // Supondo um máximo de 256 pontos
        unsigned char straightPathFlags[256];
        dtPolyRef straightPathPolys[256];
        int nStraightPath;
        query.findStraightPath(startPos, endPos, pathPolys, nPathPolys, straightPath, straightPathFlags, straightPathPolys, &nStraightPath, 256, 0);

        for (int i = 0; i < nStraightPath; ++i)
        {
            glm::vec3 point(straightPath[i * 3], straightPath[i * 3 + 1], straightPath[i * 3 + 2]);
            path.push_back(point);
            navmesh_print_cube_in_space(point);
        }

        if (path.size() > 0)
        {

            if(start_pos.x+start_pos.y,start_pos.z!=0){
                path[0] = start_pos;
            }
            if(end_pos.x+end_pos.y,end_pos.z!=0){
                path[path.size() - 1] = end_pos;
            }
            
        }

        return path;
    }

    static std::vector<glm::vec3> generate_path(glm::vec3 start, glm::vec3 end, std::string tag)
    {
        if (navmeshes.find(tag) != navmeshes.end())
        {
            return navmeshes[tag]->pegar_componente<navmesh>()->generate_this_path(start, end);
        }
        return {};
    }

    ~navmesh() { clear(); }

    static void remove_navmesh(std::string tag = "")
    {
        if (navmeshes.find(tag) != navmeshes.end())
        {
            cena_objetos_selecionados->remover_objeto(navmeshes[tag]);
            // navmeshes.erase(tag);
        }
    }

    static void remove_all_navmesh()
    {
        for (pair<std::string, shared_ptr<objeto_jogo>> p : navmeshes)
        {
            remove_navmesh(p.first);
        }
    }

    static shared_ptr<objeto_jogo> create_navmesh(glm::vec3 position, glm::quat rotation, glm::vec3 scale, shared_ptr<malha> mesh, std::string tag = "")
    {
        shared_ptr<objeto_jogo> navmesh_obj = novo_objeto_jogo();
        navmesh_obj->adicionar_componente<transform_>();
        shared_ptr<transform_> tf = navmesh_obj->pegar_componente<transform_>();
        tf->pos = position;
        tf->quater = rotation;
        tf->esca = scale;
        tf->pegar_matriz();

        navmesh_obj->adicionar_componente<navmesh>();
        shared_ptr<navmesh> nm = navmesh_obj->pegar_componente<navmesh>();
        nm->tag = tag;
        nm->path_mesh = mesh;

        cena_objetos_selecionados->adicionar_objeto(navmesh_obj);

        nm->apply();
        // nm->show_this();

        return navmesh_obj;
    }
};

float navmesh::navmeshQuality = 1.0f;
std::map<std::string, shared_ptr<objeto_jogo>> navmesh::navmeshes = {};
