#pragma once
#include <set>
#include "RecursosT.h"
#include "scene.h"
#include "Tempo.h"
#include "game_object.h"
#include <thread>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <cmath>

#include "render_mesh.h"
#include "ManusearArquivos.h"

#include <btBulletDynamicsCommon.h>
#include "BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolverMt.cpp"
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/Gimpact/btGImpactShape.h>

#include "Recast.h"

#include "DetourNavMesh.h"
#include "DetourNavMeshQuery.h"
#include "DetourNavMeshBuilder.h"
#include "DetourTileCache.h"

btDiscreteDynamicsWorld *dynamicsWorld;

int global_bullet_iniciado = 0;

vector<thread> bullet_threads;

int btMeshes_shapes_count = 0;
std::map<shared_ptr<std::string>, btCollisionShape *> btMeshes_shapes;
std::map<string, shared_ptr<std::string>> btMeshes_shapes_addresses;
void btClean_Meshes_shapes()
{

    std::map<shared_ptr<std::string>, btCollisionShape *> mapa2;
    std::map<string, shared_ptr<std::string>> mapa3;

    for (pair<shared_ptr<std::string>, btCollisionShape *> p : btMeshes_shapes)
    {

        if (p.first.use_count() > 3)
        {
            mapa2.insert(p);
            mapa3.insert(pair<string, shared_ptr<std::string>>(*p.first.get(), p.first));
        }
        else
        {
            delete p.second;
            btMeshes_shapes_count -= 1;
        }
    }
    btMeshes_shapes.clear();
    btMeshes_shapes_addresses.clear();
}

shared_ptr<std::string> get_mesh_shape_address(std::string addres)
{
    if (btMeshes_shapes_addresses.find(addres) != btMeshes_shapes_addresses.end())
    {
        return btMeshes_shapes_addresses[addres];
    }
    else
    {
        shared_ptr<std::string> ret = make_shared<std::string>(addres);
        btMeshes_shapes_addresses.insert(pair<string, shared_ptr<std::string>>(addres, ret));
        return ret;
    }
}

map<btCollisionObject *, shared_ptr<objeto_jogo>> collisionObject_obj;
map<objeto_jogo *, vector<objeto_jogo *>> bu_collisions_no_per_object;

dtNavMesh *navMesh = nullptr;
int navDataSize = 0;
unsigned char *navData = nullptr;
unsigned char *tempPolyAreas = nullptr;
unsigned short *tempPolyFlags = nullptr;

const float rcMeshQuality = 1.0f;

void calcBminBmax(shared_ptr<malha> minhaMalha, float bmin[3], float bmax[3])
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

void freeRcPolyMesh(rcPolyMesh *mesh)
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

std::shared_ptr<malha> meshes_fused = NULL;
rcPolyMesh *rcmeshes;
//rcPolyMeshDetail *rcmeshes;

std::shared_ptr<malha> fuse_meshes(const std::vector<std::shared_ptr<malha>> &minhasMalhas, const std::vector<glm::mat4> &transformacoes)
{

    if (minhasMalhas.size() != transformacoes.size())
    {
        // O número de malhas e transformações deve ser o mesmo
        return nullptr;
    }

    auto malhaFusionada = std::make_shared<malha>();

    int vertOffset = 0;
    for (size_t i = 0; i < minhasMalhas.size(); ++i)
    {
        const auto &malha = minhasMalhas[i];
        const glm::mat4 &transform = transformacoes[i];

        // Transformar e adicionar vértices
        for (const auto &vert : malha->vertices)
        {
            glm::vec4 pos(vert.posicao[0], vert.posicao[1], vert.posicao[2], 1.0f);
            pos = transform * pos;

            vertice vertTransformado = vert;
            vertTransformado.posicao[0] = pos.x;
            vertTransformado.posicao[1] = pos.y;
            vertTransformado.posicao[2] = pos.z;

            malhaFusionada->vertices.push_back(vertTransformado);
        }

        // Adicionar índices dos polígonos com o deslocamento correto
        for (const auto &indice : malha->indice)
        {
            malhaFusionada->indice.push_back(indice + vertOffset);
        }

        vertOffset += malha->vertices.size();
    }

    meshes_fused = malhaFusionada;

    return malhaFusionada;
}



void fillVerticesAndIndices(rcPolyMesh* polyMesh, std::shared_ptr<malha> minhaMalha) {

    // Preencher vértices
    polyMesh->nverts = minhaMalha->vertices.size();
    polyMesh->verts = new unsigned short[polyMesh->nverts * 3];

    for (size_t i = 0; i < minhaMalha->vertices.size(); ++i) {
        const vertice& vert = minhaMalha->vertices[i];
        for (int j = 0; j < 3; ++j) {
            polyMesh->verts[i * 3 + j] = static_cast<unsigned short>(std::round((vert.posicao[j] - polyMesh->bmin[j]) / rcMeshQuality));
        }
    }

    // Preencher índices dos polígonos
    polyMesh->npolys = minhaMalha->indice.size() / polyMesh->nvp;
    polyMesh->polys = new unsigned short[polyMesh->npolys * 2 * polyMesh->nvp];

    for (size_t i = 0; i < polyMesh->npolys; ++i) {
        for (int j = 0; j < polyMesh->nvp; ++j) {
            if (j < 3) {
                // índices de vértices do polígono
                polyMesh->polys[i * 2 * polyMesh->nvp + j] = minhaMalha->indice[i * 3 + j];
            } else {
                // Resto preenchido com RC_MESH_NULL_IDX
                polyMesh->polys[i * 2 * polyMesh->nvp + j] = RC_MESH_NULL_IDX;
            }

            // índices de borda
            polyMesh->polys[i * 2 * polyMesh->nvp + polyMesh->nvp + j] = RC_MESH_NULL_IDX;
        }
    }

    // Aqui, você pode adicionar lógica adicional se necessário, como configuração de áreas navegáveis.
}


void fillBorderIndexes(rcPolyMesh* polyMesh) {
    // Mapa para rastrear as bordas compartilhadas
    std::map<std::pair<unsigned short, unsigned short>, int> edgeToPolyMap;

    // Preencher índices de borda
    for (int i = 0; i < polyMesh->npolys; ++i) {
        for (int j = 0; j < 3; ++j) { // Considerando 3 vértices por polígono (triângulos)
            unsigned short v0 = polyMesh->polys[i * 2 * polyMesh->nvp + j];
            unsigned short v1 = polyMesh->polys[i * 2 * polyMesh->nvp + (j + 1) % 3];
            if (v0 > v1) std::swap(v0, v1); // Garantir consistência na ordem dos vértices

            auto edge = std::make_pair(v0, v1);
            if (edgeToPolyMap.find(edge) == edgeToPolyMap.end()) {
                // Primeira ocorrência da borda
                edgeToPolyMap[edge] = i;
                polyMesh->polys[i * 2 * polyMesh->nvp + polyMesh->nvp + j] = RC_MESH_NULL_IDX;
            } else {
                // Borda compartilhada encontrada
                int adjacentPoly = edgeToPolyMap[edge];
                polyMesh->polys[i * 2 * polyMesh->nvp + polyMesh->nvp + j] = adjacentPoly;
                polyMesh->polys[adjacentPoly * 2 * polyMesh->nvp + polyMesh->nvp + j] = i;
            }
        }
    }

    // Preencher restante dos índices de borda não compartilhados com RC_MESH_NULL_IDX
    for (int i = 0; i < polyMesh->npolys; ++i) {
        for (int j = 3; j < polyMesh->nvp; ++j) {
            polyMesh->polys[i * 2 * polyMesh->nvp + polyMesh->nvp + j] = RC_MESH_NULL_IDX;
        }
    }
}


rcPolyMesh* convertToRcPolyMesh(std::shared_ptr<malha> minhaMalha) {
    if (!minhaMalha) {
        return nullptr;
    }

    // Limpar malha para remover vértices duplicados e ajustar índices
    minhaMalha->limpar_malha();

    //print("minhaMalha info",minhaMalha->vertices.size(),minhaMalha->indice.size());

    rcPolyMesh* polyMesh = new rcPolyMesh();
    if (!polyMesh) {
        return nullptr;
    }

    // Calcular bmin e bmax da malha
    calcBminBmax(minhaMalha, polyMesh->bmin, polyMesh->bmax);

    // Definir cell size e height
    polyMesh->cs = rcMeshQuality;
    polyMesh->ch = rcMeshQuality;

    //difine polygon size
    polyMesh->nvp = 3;

    // Preencher os dados de vértices e índices
    fillVerticesAndIndices(polyMesh, minhaMalha);
    fillBorderIndexes(polyMesh);

    return polyMesh;
}


void fillVerticesAndIndicesDetail(rcPolyMeshDetail* polyMeshDetail, std::shared_ptr<malha> minhaMalha) {
    if (!minhaMalha || !polyMeshDetail) {
        return; // Verifique a validade dos parâmetros
    }

    // Preencher vértices detalhados
    polyMeshDetail->nverts = minhaMalha->vertices.size();
    polyMeshDetail->verts = new float[polyMeshDetail->nverts * 3]; // Cada vértice tem 3 coordenadas

    for (size_t i = 0; i < minhaMalha->vertices.size(); ++i) {
        const vertice& vert = minhaMalha->vertices[i];
        for (int j = 0; j < 3; ++j) {
            polyMeshDetail->verts[i * 3 + j] = vert.posicao[j];
        }
    }

    // Preencher triângulos detalhados
    polyMeshDetail->ntris = minhaMalha->indice.size() / 3;
    polyMeshDetail->tris = new unsigned char[polyMeshDetail->ntris * 4]; // Cada triângulo tem 3 vértices + 1 flag de área

    for (size_t i = 0; i < polyMeshDetail->ntris; ++i) {
        for (int j = 0; j < 3; ++j) {
            polyMeshDetail->tris[i * 4 + j] = static_cast<unsigned char>(minhaMalha->indice[i * 3 + j]);
        }
        polyMeshDetail->tris[i * 4 + 3] = RC_WALKABLE_AREA; // Definir flag de área para cada triângulo
    }
}


rcPolyMeshDetail* convertToRcPolyMeshDetail(std::shared_ptr<malha> minhaMalha) {
    if (!minhaMalha) {
        return nullptr;
    }

    minhaMalha->limpar_malha();

    rcPolyMeshDetail* polyMeshDetail = new rcPolyMeshDetail();
    if (!polyMeshDetail) {
        return nullptr;
    }
    
    fillVerticesAndIndicesDetail(polyMeshDetail, minhaMalha);

    return polyMeshDetail;
}

void printDtStatus(dtStatus status)
{
    if (dtStatusSucceed(status))
    {
        std::cout << "Success" << std::endl;
    }
    else
    {
        if (dtStatusDetail(status, DT_WRONG_MAGIC))
        {
            std::cout << "Failure: Wrong magic" << std::endl;
        }
        if (dtStatusDetail(status, DT_WRONG_VERSION))
        {
            std::cout << "Failure: Wrong version" << std::endl;
        }
        if (dtStatusDetail(status, DT_OUT_OF_MEMORY))
        {
            std::cout << "Failure: Out of memory" << std::endl;
        }
        if (dtStatusDetail(status, DT_INVALID_PARAM))
        {
            std::cout << "Failure: Invalid parameter" << std::endl;
        }
        if (dtStatusDetail(status, DT_BUFFER_TOO_SMALL))
        {
            std::cout << "Failure: Buffer too small" << std::endl;
        }
        if (dtStatusDetail(status, DT_OUT_OF_NODES))
        {
            std::cout << "Failure: Out of nodes" << std::endl;
        }
        if (dtStatusDetail(status, DT_PARTIAL_RESULT))
        {
            std::cout << "Partial result" << std::endl;
        }
    }
}

rcPolyMesh *combinedPolyMesh = NULL;

void locate_dtNavMeshCreateParams_error(dtNavMeshCreateParams *params)
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


dtNavMesh *rcPolyMesh_chuncks_to_navMesh(
    rcPolyMesh *meshDetails,
    float walkableHeight = 0.1f,
    float walkableRadius = 0.1f,
    float walkableClimb = 0.1f)
{
    if (!meshDetails)
    {
        return nullptr;
    }

    // Configuração do dtNavMesh
    dtNavMesh *navMesh = new dtNavMesh();
    if (!navMesh)
    {
        return nullptr;
    }

    // Definir os parâmetros para criação do dtNavMesh
    dtNavMeshCreateParams params;
    memset(&params, 0, sizeof(params));

    params.verts = meshDetails->verts;
    params.vertCount = meshDetails->nverts;
    params.polys = meshDetails->polys;
    params.polyCount = meshDetails->npolys;
    params.nvp = meshDetails->nvp;
    params.cs = meshDetails->cs;
    params.ch = meshDetails->ch;
    memcpy(params.bmin, meshDetails->bmin, sizeof(params.bmin));
    memcpy(params.bmax, meshDetails->bmax, sizeof(params.bmax));
    
   
    params.walkableHeight = walkableHeight;
    params.walkableRadius = walkableRadius;
    params.walkableClimb = walkableClimb;
    params.tileX = 0;
    params.tileY = 0;
    params.tileLayer = 0;
    
    params.buildBvTree = true;

    

    // Definir áreas caminháveis e flags de borda
    unsigned char *polyAreas = new unsigned char[meshDetails->npolys];
    unsigned short *polyFlags = new unsigned short[meshDetails->npolys * 2]; // *2 para incluir flags de borda
    for (int i = 0; i < meshDetails->npolys; ++i)
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

    // Criar dados de navegação
    unsigned char *navData;
    int navDataSize;
    if (!dtCreateNavMeshData(&params, &navData, &navDataSize))
    {
        delete navMesh;
        delete[] polyAreas;
        delete[] polyFlags;
        return nullptr;
    }

    // Inicializar dtNavMesh com os dados de navegação

    if (dtStatusFailed(navMesh->init(navData, navDataSize, DT_TILE_FREE_DATA)))
    {
        delete[] navData;
        delete navMesh;
        delete[] polyAreas;
        delete[] polyFlags;
        return nullptr;
    }


    delete[] polyAreas;
    delete[] polyFlags;

    return navMesh;
}





int getVertexCount(const dtNavMesh *navMesh)
{
    if (!navMesh)
        return 0;

    int vertexCount = 0;

    for (int i = 0; i < navMesh->getMaxTiles(); ++i)
    {
        const dtMeshTile *tile = navMesh->getTile(i);
        if (!tile || !tile->header)
            continue;

        vertexCount += tile->header->vertCount;
    }

    return vertexCount;
}

void reportDtStatusError(dtStatus status)
{
    if (dtStatusFailed(status))
    {
        if (dtStatusDetail(status, DT_WRONG_MAGIC))
        {
            std::cout << "Erro: Dados de entrada não são reconhecidos." << std::endl;
        }
        if (dtStatusDetail(status, DT_WRONG_VERSION))
        {
            std::cout << "Erro: Versão de dados de entrada é incorreta." << std::endl;
        }
        if (dtStatusDetail(status, DT_OUT_OF_MEMORY))
        {
            std::cout << "Erro: Operação ficou sem memória." << std::endl;
        }
        if (dtStatusDetail(status, DT_INVALID_PARAM))
        {
            std::cout << "Erro: Parâmetro de entrada inválido." << std::endl;
        }
        if (dtStatusDetail(status, DT_BUFFER_TOO_SMALL))
        {
            std::cout << "Erro: Buffer é muito pequeno para armazenar todos os resultados." << std::endl;
        }
        if (dtStatusDetail(status, DT_OUT_OF_NODES))
        {
            std::cout << "Erro: A consulta esgotou os nós durante a busca." << std::endl;
        }
        if (dtStatusDetail(status, DT_PARTIAL_RESULT))
        {
            std::cout << "Aviso: Resultado parcial, o destino final não foi alcançado." << std::endl;
        }
        if (dtStatusDetail(status, DT_ALREADY_OCCUPIED))
        {
            std::cout << "Erro: Um tile já foi atribuído para a coordenada x, y fornecida." << std::endl;
        }
    }
}

float color_intensity = 0.0;

void print_cube_in_space(vec3 pos)
{

    // ManuseioDados::importar_obj("resources/3D Models/oclusion_box.obj")->malhas["Cube"]

    shared_ptr<objeto_jogo> display_nav_mesh = novo_objeto_jogo();
    display_nav_mesh->adicionar_componente<transform_>();
    display_nav_mesh->pegar_componente<transform_>()->pos = pos;

    display_nav_mesh->adicionar_componente<render_malha>();
    shared_ptr<render_malha> rm = display_nav_mesh->pegar_componente<render_malha>();
    rm->usar_oclusao = false;
    rm->camada = 4;

    Material mat;
    mat.shad = "resources/Shaders/mesh";
    mat.cor = vec4(color_intensity, 0.0, 0.5, 1);
    color_intensity += 0.5;
    if (color_intensity > 1)
    {
        color_intensity = 0;
    }
    mat.texturas[0] = ManuseioDados::carregar_Imagem("resources/Textures/white.png");

    rm->malhas = {ManuseioDados::importar_obj("resources/3D Models/oclusion_box.obj")->malhas["Cube"]};

    rm->mats = {mat};
    cena_objetos_selecionados->adicionar_objeto(display_nav_mesh);

    print("cube pos:", pos.x, pos.y, pos.z);
}

std::vector<glm::vec3> get_navmesh_path(
    glm::vec3 start,
    glm::vec3 end,
    dtNavMesh *nMesh = navMesh,
    float characterHeight = 2,
    float characterRadius = 1,
    bool canJump = false)
{
    std::vector<glm::vec3> path;

    if (!nMesh)
    {
        print("no path found no navmesh");
        return path; // Retorna um vetor vazio se a malha de navegação não for fornecida
    }

    dtNavMeshQuery query;
    query.init(nMesh, 2048); // 2048 é o tamanho máximo do conjunto de nós da consulta

    // Converter glm::vec3 para o formato do Detour
    float startPos[3] = {start.x, start.y, start.z};
    float endPos[3] = {end.x, end.y, end.z};

    // Configurações de filtro baseadas nas habilidades do personagem
    dtQueryFilter filter;
    unsigned short includeFlags = canJump ? 0x08 : 0x01; // Flags de inclusão
    filter.setIncludeFlags(0x01);
    filter.setExcludeFlags(0);    // Sem flags de exclusão
    filter.setAreaCost(63, 1.0f); // Custo padrão para áreas caminháveis

    float tolerance[3] = {characterRadius, characterHeight, characterRadius}; // Tolerância para encontrar o polígono mais próximo

    // Encontrar os polígonos mais próximos de start e end
    dtPolyRef startRef, endRef;
    // reportDtStatusError(query.findNearestPoly(startPos, tolerance, &filter, &startRef, nullptr));
    // reportDtStatusError(query.findNearestPoly(endPos, tolerance, &filter, &endRef, nullptr));
    query.findNearestPoly(startPos, tolerance, &filter, &startRef, nullptr);
    query.findNearestPoly(endPos, tolerance, &filter, &endRef, nullptr);

    if (startRef == 0 || endRef == 0)
    {
        print("no path found no start or end point was found",startRef,endRef);
        return path; // Não foram encontrados polígonos de referência válidos
    }

    // Calcular o caminho
    dtPolyRef pathPolys[256]; // Tamanho máximo do caminho
    int nPathPolys;
    query.findPath(startRef, endRef, startPos, endPos, &filter, pathPolys, &nPathPolys, 256);

    if (nPathPolys == 0)
    {
        print("no path found couldnt construct path");
        return path; // Caminho não encontrado
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
        print_cube_in_space(point);
    }

    return path;
}

// print_cube_in_space(point);

std::shared_ptr<malha> nav_mesh_mesh = NULL;

std::shared_ptr<malha> convert_nav_mesh_to_mesh(const dtNavMesh *nMesh = navMesh)
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

    nav_mesh_mesh = convertedMesh;

    return convertedMesh;
}

// corrigir
std::shared_ptr<malha> convert_polyMesh_to_mesh(const rcPolyMesh *polyMesh = combinedPolyMesh)
{
    if (!polyMesh)
    {
        return nullptr;
    }

    auto convertedMesh = std::make_shared<malha>();

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

        convertedMesh->vertices.push_back(vert);
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

            convertedMesh->indice.push_back(vertexIndex);
        }
    }

    return convertedMesh;
}

shared_ptr<objeto_jogo> display_nav_mesh = NULL;
void draw_navmesh()
{
    display_nav_mesh = novo_objeto_jogo();
    display_nav_mesh->adicionar_componente<transform_>();
    display_nav_mesh->adicionar_componente<render_malha>();
    shared_ptr<render_malha> rm = display_nav_mesh->pegar_componente<render_malha>();
    rm->usar_oclusao = false;
    rm->camada = 4;

    Material mat;
    mat.shad = "resources/Shaders/mesh";
    mat.cor = vec4(0.1, 0.1, 1, 0.5);
    mat.texturas[0] = ManuseioDados::carregar_Imagem("resources/Textures/white.png");

    rm->malhas = {convert_nav_mesh_to_mesh(navMesh)};

    rm->mats = {mat};
    cena_objetos_selecionados->adicionar_objeto(display_nav_mesh);
}

void printNavMeshVertices(const dtNavMesh *nMesh = navMesh)
{
    if (!nMesh)
    {
        std::cout << "NavMesh is null" << std::endl;
        return;
    }

    for (int i = 0; i < nMesh->getMaxTiles(); ++i)
    {
        const dtMeshTile *tile = nMesh->getTile(i);
        if (!tile || !tile->header)
        {
            continue;
        }

        std::cout << "Tile " << i << std::endl;
        for (int vi = 0; vi < tile->header->vertCount; ++vi)
        {
            const float *v = &tile->verts[vi * 3];
            std::cout << "Vertex " << vi << ": (" << v[0] << ", " << v[1] << ", " << v[2] << ")" << std::endl;
        }
    }
}

dtNavMesh *gerarNavMesh(std::vector<std::shared_ptr<malha>> minhasMalhas, std::vector<glm::mat4> transforms)
{
    // https://recastnav.com/structrcPolyMesh.html
    // bool 	rcBuildPolyMesh (rcContext *ctx, const rcContourSet &cset, const int nvp, rcPolyMesh &mesh) https://recastnav.com/group__recast.html#ga8688f9cb5dab904bbbe43c362a69e769
    // bool 	rcMergePolyMeshes (rcContext *ctx, rcPolyMesh **meshes, const int nmeshes, rcPolyMesh &mesh) https://recastnav.com/group__recast.html#gaa28c3eb627ca7d96015c7978ff0eb8f7

    rcContext ctx;

    if (navMesh)
    {
        delete navMesh;
        navMesh = NULL;
    }
    if (navData)
    {
        free(navData);
        navDataSize = 0;
        navData = nullptr;
    }
    if (tempPolyAreas)
    {
        delete[] tempPolyAreas;
        tempPolyAreas = nullptr;
    }
    if (tempPolyFlags)
    {
        delete[] tempPolyFlags;
        tempPolyFlags = nullptr;
    }

    meshes_fused = fuse_meshes(minhasMalhas, transforms);
    rcmeshes = convertToRcPolyMesh(meshes_fused);
    navMesh = rcPolyMesh_chuncks_to_navMesh(rcmeshes);

    /*
    for (int x = 0; x < rcmeshes_chuncks.size(); ++x)
    {
        for (int y = 0; y < rcmeshes_chuncks[0].size(); ++y)
        {
            for (int z = 0; z < rcmeshes_chuncks[0][0].size(); ++z)
            {
                if (rcmeshes_chuncks[x][y][z])
                {
                    // std::cout << rcmeshes_chuncks[x][y][z] << std::endl;
                    freeRcPolyMesh(rcmeshes_chuncks[x][y][z]);
                }
            }
        }
    }
    rcmeshes_chuncks = {};
    */

    draw_navmesh();

    // get_navmesh_path(vec3(-21, 40.5, -138), vec3(90.0, 40.5, -71.0));
    get_navmesh_path(vec3(-21, 40.5, -138), vec3(21.0, 40.5, -205.0));

    return navMesh;
}

glm::vec3 btToGlm(const btVector3 &v)
{
    return glm::vec3(v.getX(), v.getY(), v.getZ());
}

btVector3 glmToBt(const glm::vec3 &v)
{
    return btVector3(v.x, v.y, v.z);
}

struct physics_3D_collisionInfo
{
    btCollisionObject *objectA;
    btCollisionObject *objectB;
    btVector3 contactPointA;
    btVector3 contactPointB;
    btVector3 normalOnB;
    btScalar appliedImpulse;
};
std::vector<colis_info> physics_3D_collisionInfos;

void deleteCollisionObject(btCollisionObject *object)
{
    // Remove the object from the dynamics world
    dynamicsWorld->removeCollisionObject(object);

    // Delete the motion state if the object is a rigid body
    if (btRigidBody::upcast(object))
    {
        btRigidBody *rigidBody = btRigidBody::upcast(object);
        btMotionState *motionState = rigidBody->getMotionState();
        if (motionState)
        {
            delete motionState;
        }
    }

    btClean_Meshes_shapes();

    // Finally, delete the collision object itself
    delete object;
}

void getObjectPositionAndQuaternion(const btCollisionObject *object, glm::vec3 &position, glm::quat &quaternion)
{
    btTransform transform;
    if (object->getInternalType() == btCollisionObject::CO_RIGID_BODY)
    {
        ((btRigidBody *)(object))->getMotionState()->getWorldTransform(transform);
    }
    else
    {
        transform = object->getWorldTransform();
    }
    btVector3 btPosition = transform.getOrigin();
    btQuaternion btQuaternion = transform.getRotation();
    position = glm::vec3(btPosition.getX(), btPosition.getY(), btPosition.getZ());

    quaternion = glm::quat(btQuaternion.getW(), btQuaternion.getX(), btQuaternion.getY(), btQuaternion.getZ());
}

btTransform getObjectTransform(const btCollisionObject *object)
{
    btTransform transform;
    if (object->getInternalType() == btCollisionObject::CO_RIGID_BODY)
    {
        ((btRigidBody *)(object))->getMotionState()->getWorldTransform(transform);
    }
    else
    {
        transform = object->getWorldTransform();
    }
    return transform;
}
void iniciar_global_bullet();

class bullet : public componente
{
private:
public:
    shared_ptr<malha> collision_mesh;
    char forma = 0;
    float densidade = 1, atrito = 1;
    bool gatilho = false;
    char dinamica = estatico;
    vec3 escala = vec3(1, 1, 1);
    float escala_gravidade = 1;
    bool rotacionarX = true, rotacionarY = true, rotacionarZ = true;
    float elasticidade = 0;
    float gravity_force = 1;
    info_camada layer;
    btCollisionObject *bt_obj = NULL;
    btRigidBody *bt_obj_rb = NULL;
    btCompoundShape *compound = NULL;
    shared_ptr<std::string> mesh_shape_address = NULL;
    vector<objeto_jogo *> objs_touching;
    vector<colis_info> colis_infos;
    bool get_collision_info = false;
    btTransform transform;
    btCollisionShape *Shape;

    void iniciar()
    {
        bu_collisions_no_per_object[esse_objeto.get()] = {};

        transform.setIdentity();

        if (forma == formato_colisao::caixa)
        {
            mesh_shape_address = get_mesh_shape_address("box:" + std::to_string(escala.x) + ":" + std::to_string(escala.y) + ":" + std::to_string(escala.z));
            if (btMeshes_shapes.find(mesh_shape_address) != btMeshes_shapes.end())
            {
                Shape = btMeshes_shapes[mesh_shape_address];
            }
            else
            {
                Shape = new btBoxShape(glmToBt(escala));
                btMeshes_shapes.insert(pair<shared_ptr<std::string>, btCollisionShape *>(mesh_shape_address, Shape));
                btMeshes_shapes_count++;
            }
        }
        else if (forma == formato_colisao::esfera)
        {
            mesh_shape_address = get_mesh_shape_address("sphere:" + std::to_string(escala.x));
            if (btMeshes_shapes.find(mesh_shape_address) != btMeshes_shapes.end())
            {
                Shape = btMeshes_shapes[mesh_shape_address];
            }
            else
            {
                Shape = new btSphereShape(escala.x);
                btMeshes_shapes.insert(pair<shared_ptr<std::string>, btCollisionShape *>(mesh_shape_address, Shape));
                btMeshes_shapes_count++;
            }
        }
        else if (forma == formato_colisao::cilindro)
        {
            mesh_shape_address = get_mesh_shape_address("cylinder:" + std::to_string(escala.x) + ":" + std::to_string(escala.y) + ":" + std::to_string(escala.z));
            if (btMeshes_shapes.find(mesh_shape_address) != btMeshes_shapes.end())
            {
                Shape = btMeshes_shapes[mesh_shape_address];
            }
            else
            {
                Shape = new btCylinderShape(glmToBt(escala));
                btMeshes_shapes.insert(pair<shared_ptr<std::string>, btCollisionShape *>(mesh_shape_address, Shape));
                btMeshes_shapes_count++;
            }
        }
        else if (forma == formato_colisao::capsula)
        {
            mesh_shape_address = get_mesh_shape_address("capsule:" + std::to_string(escala.x) + ":" + std::to_string(escala.y));
            if (btMeshes_shapes.find(mesh_shape_address) != btMeshes_shapes.end())
            {
                Shape = btMeshes_shapes[mesh_shape_address];
            }
            else
            {
                Shape = new btCapsuleShape(escala.x, escala.y);
                btMeshes_shapes.insert(pair<shared_ptr<std::string>, btCollisionShape *>(mesh_shape_address, Shape));
                btMeshes_shapes_count++;
            }
        }
        else if (forma == formato_colisao::convexo)
        {

            if (collision_mesh != NULL)
            {

                if (dinamica == estatico)
                {
                    mesh_shape_address = get_mesh_shape_address(collision_mesh->arquivo_origem + ":" + collision_mesh->nome + ":static:" + std::to_string(escala.x) + ":" + std::to_string(escala.y) + ":" + std::to_string(escala.z));

                    if (btMeshes_shapes.find(mesh_shape_address) != btMeshes_shapes.end())
                    {
                        Shape = btMeshes_shapes[mesh_shape_address];
                    }
                    else
                    {
                        btTriangleMesh *tm = new btTriangleMesh();
                        for (int i = 0; i < collision_mesh->indice.size(); i += 3)
                        {

                            int index = collision_mesh->indice[i];
                            vertice v = collision_mesh->vertices[index];
                            btVector3 vertex1 = btVector3(v.posicao[0] * escala.x, v.posicao[1] * escala.y, v.posicao[2] * escala.z);

                            index = collision_mesh->indice[i + 1];
                            v = collision_mesh->vertices[index];
                            btVector3 vertex2 = btVector3(v.posicao[0] * escala.x, v.posicao[1] * escala.y, v.posicao[2] * escala.z);

                            index = collision_mesh->indice[i + 2];
                            v = collision_mesh->vertices[index];
                            btVector3 vertex3 = btVector3(v.posicao[0] * escala.x, v.posicao[1] * escala.y, v.posicao[2] * escala.z);

                            tm->addTriangle(vertex1, vertex2, vertex3);
                        }

                        Shape = new btBvhTriangleMeshShape(tm, true);
                        btMeshes_shapes.insert(pair<shared_ptr<std::string>, btCollisionShape *>(mesh_shape_address, Shape));
                        btMeshes_shapes_count++;
                    }
                }
                else
                {
                    mesh_shape_address = get_mesh_shape_address(std::to_string(escala.x) + ":" + std::to_string(escala.y) + ":" + std::to_string(escala.z) + ":" + collision_mesh->arquivo_origem + ":" + collision_mesh->nome + ":dynamic");

                    if (btMeshes_shapes.find(mesh_shape_address) != btMeshes_shapes.end())
                    {
                        Shape = btMeshes_shapes[mesh_shape_address];
                    }
                    else
                    {
                        btConvexHullShape *convexHullShape = new btConvexHullShape();

                        for (int i = 0; i < collision_mesh->vertices.size(); i++)
                        {
                            convexHullShape->addPoint(btVector3(collision_mesh->vertices[i].posicao[0] * escala.x, collision_mesh->vertices[i].posicao[1] * escala.y, collision_mesh->vertices[i].posicao[2] * escala.z));
                        }

                        Shape = convexHullShape;
                        btMeshes_shapes.insert(pair<shared_ptr<std::string>, btCollisionShape *>(mesh_shape_address, Shape));
                        btMeshes_shapes_count++;
                    }
                }
            }
            else
            {
                print("fail load collision mesh");

                mesh_shape_address = get_mesh_shape_address("box:" + std::to_string(escala.x) + ":" + std::to_string(escala.y) + ":" + std::to_string(escala.z));
                if (btMeshes_shapes.find(mesh_shape_address) != btMeshes_shapes.end())
                {
                    Shape = btMeshes_shapes[mesh_shape_address];
                }
                else
                {
                    Shape = new btBoxShape(glmToBt(escala));
                    btMeshes_shapes.insert(pair<shared_ptr<std::string>, btCollisionShape *>(mesh_shape_address, Shape));
                    btMeshes_shapes_count++;
                }
            }
        }

        vec3 position = vec3(0, 0, 0);
        transform.setOrigin(glmToBt(position));
        quat quaternion;
        compound = new btCompoundShape();

        shared_ptr<transform_> tf = esse_objeto->pegar_componente<transform_>();
        if (tf != NULL)
        {

            quaternion = tf->quater;

            transform.setOrigin(glmToBt(tf->pos));
            transform.setRotation(btQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w));
        }

        if (gatilho)
        {

            bt_obj = new btPairCachingGhostObject();
            bt_obj->setCollisionShape(Shape);
            bt_obj->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
            bt_obj->setWorldTransform(transform);
            // dynamicsWorld->addCollisionObject(bt_obj, btBroadphaseProxy::SensorTrigger, btBroadphaseProxy::AllFilter);
            dynamicsWorld->addCollisionObject(bt_obj, btBroadphaseProxy::SensorTrigger, btBroadphaseProxy::KinematicFilter);
        }
        else
        {
            btDefaultMotionState *MotionState = new btDefaultMotionState(transform);
            if (dinamica == dinamico || dinamica == cinematico)
            {
                btVector3 Inertia = btVector3(0, 0, 0);
                Shape->calculateLocalInertia(densidade, Inertia);
                btRigidBody::btRigidBodyConstructionInfo CI(densidade, MotionState, Shape, Inertia);
                bt_obj_rb = new btRigidBody(CI);
                bt_obj_rb->setAngularFactor(btVector3(rotacionarX, rotacionarY, rotacionarZ));
                bt_obj_rb->setRestitution(elasticidade);
                bt_obj_rb->setGravity(btVector3(0, 0, 0));
                bt_obj_rb->setFriction(atrito);
                bt_obj_rb->setActivationState(DISABLE_DEACTIVATION);
                dynamicsWorld->addRigidBody(bt_obj_rb);
                bt_obj = bt_obj_rb;
                bt_obj_rb->setGravity(btVector3(gravidade.x * gravity_force, gravidade.y * gravity_force, gravidade.z * gravity_force));
            }
            else if (dinamica == estatico)
            {

                shared_ptr<bullet> pai_bu;
                shared_ptr<transform_> tf = esse_objeto->pegar_componente<transform_>();

                shared_ptr<transform_> pai_tf = esse_objeto->pai->pegar_componente<transform_>();

                if (tf != NULL && esse_objeto->pegar_componente<transform_>() != NULL && esse_objeto->pai->pegar_componente<bullet>() != NULL)
                {

                    pai_bu = esse_objeto->pai->pegar_componente<bullet>();
                }

                btRigidBody::btRigidBodyConstructionInfo CI(0, MotionState, Shape);
                bt_obj_rb = new btRigidBody(CI);

                dynamicsWorld->addRigidBody(bt_obj_rb);
                bt_obj = bt_obj_rb;

                //
            }
            else if (dinamica == cinematico)
            {

                btVector3 Inertia = btVector3(0, 0, 0);
                Shape->calculateLocalInertia(densidade, Inertia);
                btRigidBody::btRigidBodyConstructionInfo CI(0, MotionState, Shape, Inertia);
                bt_obj_rb = new btRigidBody(CI);
                bt_obj_rb->setAngularFactor(btVector3(rotacionarX, rotacionarY, rotacionarZ));
                bt_obj_rb->setRestitution(elasticidade);
                bt_obj_rb->setGravity(btVector3(0, 0, 0));
                bt_obj_rb->setFriction(atrito);
                bt_obj_rb->setActivationState(DISABLE_DEACTIVATION);
                bt_obj_rb->setCollisionFlags(bt_obj_rb->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
                dynamicsWorld->addRigidBody(bt_obj_rb);
                bt_obj = bt_obj_rb;
                bt_obj_rb->setGravity(btVector3(gravidade.x * gravity_force, gravidade.y * gravity_force, gravidade.z * gravity_force));
            }
        }

        collisionObject_obj.insert(pair<btCollisionObject *, shared_ptr<objeto_jogo>>(bt_obj, esse_objeto));
    }

    void aply_gravity()
    {
        if (bt_obj_rb != NULL && dinamica == dinamico)
        {
            bt_obj_rb->applyCentralForce(btVector3(gravidade.x * gravity_force * densidade, gravidade.y * gravity_force * densidade, gravidade.z * gravity_force * densidade));
        }
    }

    void atualisar()
    {
        // aply_gravity();
        shared_ptr<transform_> tf = esse_objeto->pegar_componente<transform_>();
        if (tf != NULL && bt_obj != NULL && !gatilho)
        {
            getObjectPositionAndQuaternion(bt_obj, tf->pos, tf->quater);
        }
    }

    void colidir(colis_info col) {}

    void finalisar()
    {
        if (bu_collisions_no_per_object.find(esse_objeto.get()) != bu_collisions_no_per_object.end())
        {
            for (objeto_jogo *obj : bu_collisions_no_per_object[esse_objeto.get()])
            {
                bu_collisions_no_per_object[esse_objeto.get()].clear();
            }
            bu_collisions_no_per_object.erase(esse_objeto.get());
        }

        if (bt_obj != NULL)
        {
            ///*
            mesh_shape_address = NULL;
            if (bt_obj->getInternalType() == btCollisionObject::CO_RIGID_BODY)
            {
                dynamicsWorld->removeRigidBody((btRigidBody *)bt_obj);
            }
            else
            {
                dynamicsWorld->removeCollisionObject(bt_obj);
            }
            //*/
            if (compound != NULL)
            {
                delete compound;
            }

            deleteCollisionObject(bt_obj);
            if (collisionObject_obj.find(bt_obj) != collisionObject_obj.end())
            {
                collisionObject_obj.erase(bt_obj);
            }
            bt_obj = NULL;
            bt_obj_rb = NULL;
        }
    }

    void aplay()
    {
        finalisar();
        iniciar();
    }

    void mudar_pos(vec3 pos)
    {
        btTransform tf = getObjectTransform(bt_obj);
        tf.setOrigin(btVector3(pos.x, pos.y, pos.z));
        if (bt_obj != NULL && bt_obj_rb != NULL && bt_obj->getInternalType() == btCollisionObject::CO_RIGID_BODY)
        {
            bt_obj_rb->getMotionState()->setWorldTransform(tf);
            bt_obj->setWorldTransform(tf);
        }
        else
        {
            if (bt_obj != NULL)
            {
                bt_obj->setWorldTransform(tf);
            }
        }
    }
    void mudar_rot(quat rot)
    {
        btTransform tf = getObjectTransform(bt_obj);
        tf.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
        if (bt_obj->getInternalType() == btCollisionObject::CO_RIGID_BODY)
        {
            if (bt_obj_rb != NULL && bt_obj != NULL)
            {
                bt_obj_rb->getMotionState()->setWorldTransform(tf);
                bt_obj->setWorldTransform(tf);
            }
        }
        else
        {
            if (bt_obj_rb != NULL && bt_obj != NULL)
            {
                bt_obj->setWorldTransform(tf);
            }
        }
    }
    void mudar_rot(vec3 rot)
    {
        mudar_rot(graus_quat(rot));
    }
    void adicionar_forca(vec3 forca)
    {
        if (bt_obj->getInternalType() == btCollisionObject::CO_RIGID_BODY)
        {
            if (bt_obj_rb != NULL && bt_obj != NULL)
            {
                bt_obj_rb->applyCentralForce(btVector3(forca.x, forca.y, forca.z));
                bt_obj_rb->activate();
            }
        }
    }
    void adicionar_impulso(vec3 forca)
    {
        if (bt_obj->getInternalType() == btCollisionObject::CO_RIGID_BODY)
        {
            if (bt_obj_rb != NULL && bt_obj != NULL)
            {
                bt_obj_rb->applyCentralImpulse(btVector3(forca.x, forca.y, forca.z));
                bt_obj_rb->activate();
            }
        }
    }
    void adicionar_velocidade(vec3 forca)
    {
        if (bt_obj->getInternalType() == btCollisionObject::CO_RIGID_BODY)
        {
            if (bt_obj_rb != NULL && bt_obj != NULL)
            {
                bt_obj_rb->setLinearVelocity(btVector3(forca.x, forca.y, forca.z));
                bt_obj_rb->activate();
            }
        }
    }

    void adicionar_forca_rotativo(vec3 forca)
    {
        if (bt_obj->getInternalType() == btCollisionObject::CO_RIGID_BODY)
        {
            if (bt_obj_rb != NULL && bt_obj != NULL)
            {
                bt_obj_rb->applyTorque(btVector3(forca.x, forca.y, forca.z));
                bt_obj_rb->activate();
            }
        }
    }
    void adicionar_impulso_rotativo(vec3 forca)
    {
        if (bt_obj->getInternalType() == btCollisionObject::CO_RIGID_BODY)
        {
            if (bt_obj_rb != NULL && bt_obj != NULL)
            {
                bt_obj_rb->applyTorqueImpulse(btVector3(forca.x, forca.y, forca.z));
                bt_obj_rb->activate();
            }
        }
    }
    void aplicar_velocidade_rotativa(vec3 forca)
    {
        if (bt_obj->getInternalType() == btCollisionObject::CO_RIGID_BODY)
        {
            if (bt_obj_rb != NULL && bt_obj != NULL)
            {
                bt_obj_rb->setAngularVelocity(btVector3(forca.x, forca.y, forca.z));
                bt_obj_rb->activate();
            }
        }
    }
    ~bullet()
    {
    }
};

void bake_navmesh_3D()
{
    std::vector<std::shared_ptr<malha>> listaMeshes;
    std::vector<glm::mat4> listTransforms;
    for (pair<btCollisionObject *, shared_ptr<objeto_jogo>> p : collisionObject_obj)
    {

        shared_ptr<transform_> tf = p.second->pegar_componente<transform_>();
        shared_ptr<bullet> bu = p.second->pegar_componente<bullet>();

        if (tf && bu && !bu->gatilho && bu->dinamica == estatico && bu->collision_mesh)
        {

            listaMeshes.push_back(bu->collision_mesh);
            listTransforms.push_back(tf->pegar_matriz());
        }
    }

    if (navMesh)
    {
        dtFreeNavMesh(navMesh);
    }
    navMesh = gerarNavMesh(listaMeshes, listTransforms);
}

class CustomContactResultCallback : public btCollisionWorld::ContactResultCallback
{
public:
    CustomContactResultCallback()
    {
    }

    virtual btScalar addSingleResult(btManifoldPoint &cp, const btCollisionObjectWrapper *colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper *colObj1Wrap, int partId1, int index1)
    {
        colis_info collisionInfo;

        collisionInfo.pos = btToGlm(cp.getPositionWorldOnA());
        collisionInfo.nor = btToGlm(cp.m_normalWorldOnB);
        collisionInfo.velocidade = cp.m_appliedImpulse;

        objeto_jogo *A = collisionObject_obj[const_cast<btCollisionObject *>(colObj0Wrap->getCollisionObject())].get();
        objeto_jogo *B = collisionObject_obj[const_cast<btCollisionObject *>(colObj1Wrap->getCollisionObject())].get();
        shared_ptr<bullet> bu = A->pegar_componente<bullet>();

        if (bu->get_collision_info)
        {
            collisionInfo.obj = A;
            collisionInfo.cos_obj = B;
            physics_3D_collisionInfos.emplace_back(collisionInfo);
            bu->colis_infos.emplace_back(collisionInfo);
        }

        return 0; // return 0 to process all collisions
    }
};

class CustomOverlapFilterCallback : public btOverlapFilterCallback
{
public:
    virtual bool needBroadphaseCollision(btBroadphaseProxy *proxy0, btBroadphaseProxy *proxy1) const
    {
        // Obtenha os objetos associados aos proxies
        btCollisionObject *obj0 = static_cast<btCollisionObject *>(proxy0->m_clientObject);
        btCollisionObject *obj1 = static_cast<btCollisionObject *>(proxy1->m_clientObject);

        // Verifique se ambos os objetos são do tipo SensorTrigger (ou seja, não devem colidir entre si)
        bool isSensorTrigger0 = obj0->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE;
        bool isSensorTrigger1 = obj1->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE;

        // Evite a colisão entre objetos SensorTrigger
        if (isSensorTrigger0 && isSensorTrigger1)
        {
            return false;
        }

        if (collisionObject_obj.find(obj0) != collisionObject_obj.end() && collisionObject_obj.find(obj1) != collisionObject_obj.end())
        {
            shared_ptr<bullet> bullet1 = collisionObject_obj[obj0]->pegar_componente<bullet>();
            shared_ptr<bullet> bullet2 = collisionObject_obj[obj1]->pegar_componente<bullet>();

            if (bullet1 != NULL && bullet2 != NULL)
            {

                for (int i : bullet1->layer.camada_colide)
                {
                    if (i == bullet2->layer.camada)
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }
};

bool raycast_dir_bullet_3D(vec3 rayFrom, vec3 rayTo, colis_info &result)
{

    btCollisionWorld::ClosestRayResultCallback rayCallback(glmToBt(rayFrom), glmToBt(rayTo));
    dynamicsWorld->rayTest(glmToBt(rayFrom), glmToBt(rayTo), rayCallback);

    if (rayCallback.hasHit())
    {
        result.pos = btToGlm(rayCallback.m_hitPointWorld);
        result.nor = btToGlm(rayCallback.m_hitNormalWorld);
        result.cos_obj = collisionObject_obj[const_cast<btCollisionObject *>(rayCallback.m_collisionObject)].get();
        result.distancia = glm::distance(rayFrom, result.pos);
        if (rayCallback.hasHit())
        {
            return true;
        }
    }
    return false;
}

class bullet_charter : public componente
{
public:
    shared_ptr<objeto_jogo> roof_cheker, floor_cheker;
    bool in_roof, in_floor;

    bullet_charter() {}
    void iniciar() {}
    void atualisar() {}
    void colidir(colis_info col) {}
    void finalisar() {}
};

void iniciar_global_bullet()
{
    if (global_bullet_iniciado == 0)
    {
        cout << "iniciar global bullet\n";

        btDefaultCollisionConfiguration *collisionConfiguration = new btDefaultCollisionConfiguration();
        btCollisionDispatcher *dispatcher = new btCollisionDispatcher(collisionConfiguration);
        btDbvtBroadphase *broadphase = new btDbvtBroadphase();
        CustomOverlapFilterCallback *customFilterCallback = new CustomOverlapFilterCallback();
        broadphase->getOverlappingPairCache()->setOverlapFilterCallback(customFilterCallback);
        btSequentialImpulseConstraintSolver *solver = new btSequentialImpulseConstraintSolver();
        dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

        dynamicsWorld->setGravity(btVector3(0, 0, 0));

        global_bullet_iniciado++;
    }
}

thread bullet_starter;
void iniciar_iniciar_global_bullet()
{

    thread t(iniciar_global_bullet);
    bullet_threads.push_back(std::move(t));
}

void terminar_iniciar_global_bullet()
{

    for (std::thread &thread : bullet_threads)
    {
        thread.join();
    }
    bullet_threads.clear();
}

CustomContactResultCallback bullet_callback;
void get_3D_collisions()
{
    btCollisionObjectArray &bcoa = dynamicsWorld->getCollisionObjectArray();

    for (int i = 0; i < bcoa.size(); i++)
    {
        dynamicsWorld->contactTest(bcoa[i], bullet_callback);
    }
}

void clean_collisions()
{
    for (auto p : collisionObject_obj)
    {
        // p.second->pegar_componente<bullet>()->objects_touching = 0;
        p.second->pegar_componente<bullet>()->colis_infos.clear();
        p.second->pegar_componente<bullet>()->objs_touching.clear();
    }
    physics_3D_collisionInfos.clear();
}

void clean_bu_collisions_no_per_object()
{
    for (pair<objeto_jogo *, std::vector<objeto_jogo *>> p : bu_collisions_no_per_object)
    {
        bu_collisions_no_per_object[p.first].clear();
    }
}

void get_bu_collisions_no_per_object()
{
    for (colis_info ci : physics_3D_collisionInfos)
    {
        bu_collisions_no_per_object[(objeto_jogo *)ci.obj].push_back((objeto_jogo *)ci.cos_obj);
    }
}

void atualisar_global_bullet()
{

    clean_collisions();
    get_3D_collisions();
    clean_bu_collisions_no_per_object();
    get_bu_collisions_no_per_object();
    float bullet_passo_tempo = Tempo::varTempRender * Tempo::velocidadeTempo;
    if (Tempo::velocidadeTempo > 0)
    {
        dynamicsWorld->stepSimulation(bullet_passo_tempo, 0);
    }
}

void iniciar_atualisar_global_bullet()
{

    thread t(atualisar_global_bullet);
    bullet_threads.push_back(std::move(t));
}

void terminar_atualisar_global_bullet()
{

    for (std::thread &thread : bullet_threads)
    {
        thread.join();
    }
    bullet_threads.clear();
}