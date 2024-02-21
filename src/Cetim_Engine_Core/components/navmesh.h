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

class navmesh : public componente
{
private:
    rcPolyMesh *rcmeshe = NULL;
    dtNavMesh *navMesh = NULL;
    int navDataSize = 0;
    unsigned char *navData = NULL;
    unsigned char *tempPolyAreas = NULL;
    unsigned short *tempPolyFlags = NULL;

    shared_ptr<malha> internal_path_mesh = NULL;

    shared_ptr<render_malha> rm = NULL;

public:
    static float navmeshQuality;
    static std::map<std::string, shared_ptr<objeto_jogo>> navmeshes;

    shared_ptr<malha> path_mesh = NULL;

    navmesh() {}

    void clear()
    {

        navDataSize = 0;
        internal_path_mesh = NULL;

        free(rcmeshe);
        rcmeshe = NULL;
        free(navMesh);
        navMesh = NULL;
        free(navData);
        navData = NULL;
        free(tempPolyAreas);
        tempPolyAreas = NULL;
        free(tempPolyFlags);
        tempPolyFlags = NULL;
    }

    void show_this(bool on)
    {

        if (on)
        {
            esse_objeto->adicionar_componente<render_malha>();
            rm = display_nav_mesh->pegar_componente<render_malha>();
            rm->usar_oclusao = false;
            rm->camada = 4;

            Material mat;
            mat.shad = "resources/Shaders/mesh";
            mat.cor = vec4(0.1, 0.1, 1, 0.5);
            mat.texturas[0] = ManuseioDados::carregar_Imagem("resources/Textures/white.png");

            rm->malhas = {path_mesh};

            rm->mats = {mat};
            cena_objetos_selecionados->adicionar_objeto(display_nav_mesh);

        }else if(rm){
            esse_objeto->remover_componente<render_malha>();
        }
    }

    static void show(bool on)
    {
       for(pair<std::string, shared_ptr<objeto_jogo>> p : navmeshes){
            p.second->pegar_componente<navmesh>()->show_this(on);
       }
    }

    void apply()
    {

        clear();
    }

    std::vector<glm::vec3> generate_path(glm::vec3 start, glm::vec3 end)
    {
        return {};
    }

    static std::vector<glm::vec3> generate_path(glm::vec3 start, glm::vec3 end, std::string tag)
    {
        if (navmeshes.find(tag) != navmeshes.end())
        {
            return navmeshes[tag]->pegar_componente<navmesh>()->generate_path(start, end);
        }
        return {};
    }

    ~navmesh() { clear(); }
};

float navmesh::navmeshQuality = 1.0f;
std::map<std::string, shared_ptr<objeto_jogo>> navmesh::navmeshes = {};

void remove_navmesh(std::string tag = "")
{
}

shared_ptr<objeto_jogo> create_navmesh(glm::vec3 position, glm::quat rotation, glm::vec3 scale, std::string tag = "")
{
    return NULL;
}
