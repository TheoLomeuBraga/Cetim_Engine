#pragma once
#include <sstream>
#include <math.h>
#include <vector>
#include <fstream>
#include <map>
#include <set>
#include <iostream>
#include <filesystem>
#include <stdio.h>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <any>
#include <unordered_set>
#include <variant>

#include "table.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

using namespace glm;

using namespace std;

// se esse progeto for para frende pfv adicionar suporte a linux

// #define EDITOR_MODE true
#define EDITOR_MODE false

#define AAAAA cout << "AAAAA\n" /

vec3 gravidade = vec3(0, -9.8f, 0);
// vec3 gravidade = vec3(0, 0, 0);

struct printable_any
{
	std::variant<std::string, int, double> m_val;

	template <class... Args>
	printable_any(Args &&...args) : m_val(std::forward<Args>(args)...)
	{
	}

	friend std::ostream &operator<<(std::ostream &os, const printable_any &val)
	{
		std::visit([&os](const auto &value)
				   { os << value; },
				   val.m_val);
		return os;
	}
};

template <typename... Args>
void print(const Args &...args)
{
	((std::cout << args << " "), ...);
	std::cout << '\n';
}

class asset
{
public:
	asset() {}
	static void deletar() {}
};

class input_mouse
{
public:
	input_mouse() {}
	unordered_map<string, int> botoes;
	unordered_map<string, float> movimentos;
};

class teclado
{
public:
	teclado() {}
	bool pegar_input_texto = false;
	string input_texto = "";
	unordered_map<string, int> teclas;
};

class joystick
{
public:
	joystick() {}
	// unordered_map<string, int> botoes;
	// unordered_map<string, float> eixos;
	unordered_map<string, float> inputs;
};

class progresso_carregamento
{
public:
	bool carregado;
	uint8_t progresso;
};

enum filtro_textura
{
	PIXEL = 0,
	LINEAR = 1

};

class imagem;
void remover_textura(imagem *img);

class imagem : public asset
{
public:
	int ID = -1;

	string local = "";
	ivec2 res;
	unsigned int ContagemPixels, canais, tamanho;
	int mip_map;

	vector<unsigned char> data;

	imagem() {}
	imagem(unsigned int x, unsigned int y, unsigned int c, unsigned char *dat)
	{
		res.x = x;
		res.y = y;
		ContagemPixels = x * y;
		canais = c;
		tamanho = x * y * c;

		data.resize(tamanho);
		for (unsigned int i = 0; i < tamanho; i++)
		{
			data[i] = dat[i];
		}

		// delete[] dat;
	}

	imagem(unsigned int x, unsigned int y, unsigned int c, vector<unsigned char> dat)
	{
		res.x = x;
		res.y = y;
		ContagemPixels = x * y;
		canais = c;
		tamanho = x * y * c;

		data = dat;
	}

	friend void remover_da_api_grafica();

	void deletar()
	{
		remover_textura(this);
		vector<unsigned char> nada;
		data.swap(nada);
	}
	~imagem()
	{
		deletar();
	}

	vec4 pegarPixel(unsigned int x, unsigned int y)
	{
		unsigned int valor = canais * (res.x * y) + x;
		return vec4(data[valor], data[valor + 1], data[valor + 2], data[valor + 3]);
	}

	void mudarPixel(unsigned int x, unsigned int y, vec4 cor)
	{
		unsigned int valor = canais * (res.x * y) + x;
		data[valor] = cor.x;
		data[valor + 1] = cor.y;
		data[valor + 2] = cor.z;
		data[valor + 3] = cor.w;
	}
};

int pegar_inicio_pixel(ivec2 res, ivec2 local, unsigned int canais)
{
	return (unsigned int)(canais * (res.x * local.y) + local.x);
}

struct tile_struct
{
	uint8 sprite_id = 0;
	string nome = "";
};
typedef struct tile_struct tile;

class tile_set : public asset
{
public:
	tile_set() {}
	string local, local_imagem;
	ivec2 res_tile, quant_tiles;
	shared_ptr<imagem> tiles_img;
	vector<tile> tiles;

	tile pegar_tile(int n)
	{
		return tiles[n];
	}

	void deletar()
	{
	}
	~tile_set()
	{
		deletar();
	}
};

class tile_map_info : public asset
{
public:
	string local;
	tile_map_info() {}
	ivec2 res;
	vector<vector<int>> info;

	~tile_map_info()
	{
		cout << "tilemap info: " << this << " apagado" << endl;
	}
};

class JanelaConfig
{
public:
	const char *nome = "";
	int X = 256, Y = 256;
	bool janelaInteira = false, transparente = false;
	int antiCerrilhado = 0;

	JanelaConfig(const char *n, int x, int y, bool fs, bool transp)
	{
		nome = n;
		X = x;
		Y = y;
		janelaInteira = fs;
		transparente = transp;
	}
	JanelaConfig(const char *n, int x, int y, bool fs, int aa, bool transp)
	{
		nome = n;
		X = x;
		Y = y;
		janelaInteira = fs;
		antiCerrilhado = aa;
		transparente = transp;
	}
};

struct instrucoes_render_struct
{
	int camera = 0;
	bool iniciar_render = true, limpar_buffer_cores = true, limpar_buffer_profundidade = true, desenhar_objetos = true, terminar_render = true, usar_profundidade = true;
};
typedef struct instrucoes_render_struct instrucoes_render;

struct colis_info_struct
{
	void *obj = NULL;
	void *cos_obj = NULL;
	bool sensor = false;
	bool colidindo = true;
	float distancia = 0;
	vec3 pos = vec3(0, 0, 0);
	float velocidade = 0;
	vec3 nor = vec3(0, 0, 0);
};
typedef struct colis_info_struct colis_info;

struct info_camada_collision_struct
{
	int camada = 1;
	vector<int> camada_colide = {
		1,
	};
};
typedef info_camada_collision_struct info_camada;

enum tipo_dinamica_corpo
{
	estatico = 0,
	dinamico = 1,
	cinematico = 2,
};
enum formato_colisao
{
	caixa = 0,
	esfera = 1,
	convexo = 2,
	tile_suave = 3,
	volume_tiled = 4,
	cilindro = 5,
	capsula = 6,
};

// #define NUM_CARACTERES 128

struct Caractere_info_struct
{
	char letra;
	ivec2 res;
	vec4 pos_sca;
	float avancamento;
	vector<unsigned char> data;
};
typedef struct Caractere_info_struct Caractere_info;

class caractere_info
{
public:
	caractere_info() {}
	wchar_t char_;
	int width;
	int height;
	int left;
	int top;
	int pitch;
	unsigned int adivancement;
	vector<unsigned char> bitmap;
};

class fonte;
void remover_fonte(fonte *f);
class fonte : public asset
{
public:
	string path;
	int quality;
	bool pixel_perfect = true;
	std::map<wchar_t, caractere_info> chars;

	~fonte()
	{

		remover_fonte(this);
	}
};

class TMP_L2D
{
public:
	string nome;
	int X, Y;
	string tema, musica;
	vector<string> level, fundo;

	TMP_L2D(){};
	TMP_L2D(int x, int y)
	{
		X = x;
		Y = y;
		level.resize(X * Y);
		fundo.resize(X * Y);
	};
};

class config_audio
{
public:
	config_audio();

	float volume = 1, volumeMusica = 1, volumeEfeitosSonoros = 1, volumeVozes = 1;

	config_audio(float volumeGeral)
	{
		volume = volumeGeral;
		volumeMusica = volumeGeral;
		volumeEfeitosSonoros = volumeGeral;
		volumeVozes = volumeGeral;
	}

	config_audio(float volumeGeral, float musica, float som, float vozes)
	{

		volume = volumeGeral;
		volumeMusica = musica;
		volumeEfeitosSonoros = som;
		volumeVozes = vozes;
	}
};

enum tag_audio
{
	volumeEfeitosSonoros = 0,
	volumeMusica = 1,
	volumeVozes = 2,

};

config_audio config_som = config_audio(1.0f);

class audio_info
{
public:
	string nome;
	int ID = -1;
	tag_audio tag;

	bool pausa = false, loop = false;
	float tempo = 0.001f, velocidade = 1, volume = 1, min_distance = 1, atenuation = 5;

	audio_info() {}
	audio_info(string n) { nome = n; }
};

class shader : public asset
{
public:
	shader() {}
	string local;
	shader(string local) { this->local = local; }
};

// struct PP_struct {
//	shader* shad;
//
//	vec4 cor;
//	float gama;
//
// };
// typedef struct PP_struct PosProcessamento;

template <typename X>
bool tem_item(vector<X> vec, X item)
{
	bool ret = false;
	for (X x : vec)
	{
		if (x == item)
		{
			ret = true;
			break;
		}
	}
	return ret;
}

template <typename X>
vector<X> remover_elementos_duplicados(vector<X> vec)
{
	vector<X> ret;
	for (X x : vec)
	{
		if (!tem_item<X>(ret, x))
		{
			ret.push_back(x);
		}
	}

	return ret;
}

enum lado_render_malha
{
	front = 0,
	back = 1,
	both = 2,
};

#define NO_TEXTURAS 12
#define NO_INPUTS 32
class Material
{
public:
	string shad = "";

	char lado_render = lado_render_malha::both;

	int filtro[NO_TEXTURAS];
	shared_ptr<imagem> texturas[NO_TEXTURAS];

	// float inputs[NO_INPUTS];

	map<string, float> inputs = {};

	float interpolacao, gama = 1, metalico = 0, suave = 0;
	vec4 cor = vec4(1, 1, 1, 1);
	vec4 uv_pos_sca = vec4(0, 0, 1, 1);
	Material()
	{
		for (int i = 0; i < NO_TEXTURAS; i++)
		{
			texturas[i] = NULL;
			filtro[i] = 0;
		}
	}

	bool operator==(const Material &other) const
	{
		return (
			shad == other.shad &&
			lado_render == other.lado_render &&
			equal(begin(filtro), end(filtro), begin(other.filtro)) &&
			equal(begin(texturas), end(texturas), begin(other.texturas), [](const auto &a, const auto &b)
				  { return a == b || (!a && !b); }) &&
			inputs == other.inputs &&
			interpolacao == other.interpolacao &&
			gama == other.gama &&
			metalico == other.metalico &&
			suave == other.suave &&
			cor == other.cor &&
			uv_pos_sca == other.uv_pos_sca);
	}

	bool operator!=(const Material &other) const
	{
		return !(*this == other);
	}
};

// 3D
#define MAX_BONE_INFLUENCE 4
#define base_bone_ids_and_weights \
	{                             \
		0, 0, 0, 0                \
	}
struct vertice_struct
{
	float posicao[3] = {0, 0, 0};
	float uv[2] = {0, 0};
	float normal[3] = {0, 0, 0};
	float cor[3] = {1, 1, 1};
	int id_ossos[MAX_BONE_INFLUENCE] = base_bone_ids_and_weights;
	float peso_ossos[MAX_BONE_INFLUENCE] = base_bone_ids_and_weights;

	bool operator==(const vertice_struct &v) const
	{
		return posicao[0] == v.posicao[0] && posicao[1] == v.posicao[1] && posicao[2] == v.posicao[2] && uv[0] == v.uv[0] && uv[1] == v.uv[1] && normal[0] == v.normal[0] && normal[1] == v.normal[1] && normal[2] == v.normal[2] && cor[0] == v.cor[0] && cor[1] == v.cor[1] && cor[2] == v.cor[2];
	}
	bool operator<(const vertice_struct &v) const
	{
		return posicao[0] + posicao[1] + posicao[2] + uv[0] + uv[1] + normal[0] + normal[1] + normal[2] + cor[0] + cor[1] + cor[2] < v.posicao[0] + v.posicao[1] + v.posicao[2] + v.uv[0] + v.uv[1] + v.normal[0] + v.normal[1] + v.normal[2] + v.cor[0] + v.cor[1] + v.cor[2];
	}
};
typedef struct vertice_struct vertice;

void escrever_vertice(vertice v)
{
	cout << "{" << endl;
	cout << "posicao: " << v.posicao[0] << "," << v.posicao[1] << "," << v.posicao[2] << endl;
	cout << "uv: " << v.uv[0] << "," << v.uv[1] << endl;
	cout << "normal: " << v.normal[0] << "," << v.normal[1] << "," << v.normal[2] << endl;
	cout << "cor: " << v.cor[0] << "," << v.cor[1] << "," << v.cor[2] << endl;
	cout << "}" << endl;
}

bool has_duplicates(const std::vector<float> &values)
{
	std::unordered_set<float> unique_values;
	for (const auto &value : values)
	{
		// Verifica se o valor já está no conjunto de valores únicos
		if (unique_values.count(value) > 0)
		{
			return true; // Valor duplicado encontrado
		}
		unique_values.insert(value); // Adiciona o valor ao conjunto de valores únicos
	}
	return false; // Nenhum valor duplicado encontrado
}

class malha;
void remover_malha(malha *ma);
class malha : public asset
{
public:
	malha() {}
	bool pele = false;
	string arquivo_origem, nome;
	vector<unsigned int> indice = {};
	vector<vertice> vertices = {};
	vec3 tamanho_maximo = vec3(0, 0, 0), centro = vec3(0, 0, 0);
	malha(vector<unsigned int> indice, vector<vertice> vertices)
	{
		this->indice = indice;
		this->vertices = vertices;
	}

	void comprimir()
	{
		vector<vertice> novos_vertices = remover_elementos_duplicados<vertice>(vertices);
		vector<unsigned int> novo_indice;

		novo_indice.resize(indice.size());

		for (int a = 0; a < vertices.size(); a++)
		{
			for (int b = 0; b < novos_vertices.size(); b++)
			{
				if (vertices[a] == novos_vertices[b])
				{
					novo_indice[a] = b;
				}
			}
		}
		indice = novo_indice;
		vertices = novos_vertices;
	}

	void limpar_malha()
	{
		std::map<vertice, unsigned int> uniqueVertices;
		std::vector<vertice> newVertices;
		std::vector<unsigned int> newIndices;

		// Passo 1: Identificar vértices únicos e seus índices
		for (auto &v : vertices)
		{
			if (uniqueVertices.find(v) == uniqueVertices.end())
			{
				uniqueVertices[v] = newVertices.size();
				newVertices.push_back(v);
			}
		}

		// Passo 2: Atualizar índices para refletir vértices únicos
		for (auto &idx : indice)
		{
			vertice &v = vertices[idx];
			newIndices.push_back(uniqueVertices[v]);
		}

		// Passo 3: Substituir vértices e índices antigos pelos novos
		vertices = newVertices;
		indice = newIndices;
	}

	glm::vec3 pegar_tamanho_maximo()
	{
		tamanho_maximo = vec3(0, 0, 0);
		centro = vec3(0, 0, 0);
		for (vertice v : vertices)
		{
			centro += vec3(v.posicao[0], v.posicao[1], v.posicao[2]);
		}
		centro /= vertices.size();
		for (vertice v : vertices)
		{
			vec3 pos = vec3(v.posicao[0], v.posicao[1], v.posicao[2]);
			vec3 pos_relation_center = pos - centro;
			tamanho_maximo.x = std::max(tamanho_maximo.x, std::abs(pos_relation_center.x));
			tamanho_maximo.y = std::max(tamanho_maximo.y, std::abs(pos_relation_center.y));
			tamanho_maximo.z = std::max(tamanho_maximo.z, std::abs(pos_relation_center.z));
		}
		return tamanho_maximo;
	}

	malha operator+(const malha &other) const
	{
		malha ret = *this;
		for (unsigned int i : other.indice)
		{
			ret.indice.push_back(i + vertices.size());
		}
		for (vertice v : other.vertices)
		{
			ret.vertices.push_back(v);
		}
		return ret;
	}

	malha &operator+=(const malha &other)
	{
		for (unsigned int i : other.indice)
		{
			this->indice.push_back(i + vertices.size());
		}
		for (vertice v : other.vertices)
		{
			this->vertices.push_back(v);
		}
		return *this;
	}

	bool is_not_empty()
	{
		return indice.size() > 2;
	}

	~malha()
	{
		remover_malha(this);
	}
};

struct Bounds
{
	glm::vec3 min; // Minimum coordinates (x, y, z)
	glm::vec3 max; // Maximum coordinates (x, y, z)

	Bounds(glm::vec3 min, glm::vec3 max) : min(min), max(max) {}

	// Check if a point is inside the bounds
	bool contains(const glm::vec3 &point) const
	{
		return (point.x >= min.x && point.x <= max.x) &&
			   (point.y >= min.y && point.y <= max.y) &&
			   (point.z >= min.z && point.z <= max.z);
	}
};

// Helper functions
vector<Bounds> calculateChunkBounds(shared_ptr<malha> m, float chunk_size_x, float chunk_size_y, float chunk_size_z)
{
	vector<Bounds> chunkBounds;
	// Assuming we have a way to calculate the total size of the mesh
	glm::vec3 meshSize = m->pegar_tamanho_maximo();

	// Calculate the number of chunks in each dimension
	int numChunksX = ceil(meshSize.x / chunk_size_x);
	int numChunksY = ceil(meshSize.y / chunk_size_y);
	int numChunksZ = ceil(meshSize.z / chunk_size_z);

	// Create Bounds for each chunk
	for (int x = 0; x < numChunksX; ++x)
	{
		for (int y = 0; y < numChunksY; ++y)
		{
			for (int z = 0; z < numChunksZ; ++z)
			{
				glm::vec3 min(x * chunk_size_x, y * chunk_size_y, z * chunk_size_z);
				glm::vec3 max(min.x + chunk_size_x, min.y + chunk_size_y, min.z + chunk_size_z);
				chunkBounds.push_back(Bounds(min, max));
			}
		}
	}
	return chunkBounds;
}

map<vertice, vector<int>> classifyVertices(const vector<vertice> &vertices, const vector<Bounds> &chunkBounds)
{
	map<vertice, vector<int>> vertexClassification;
	for (const auto &v : vertices)
	{
		glm::vec3 vertexPos(v.posicao[0], v.posicao[1], v.posicao[2]);
		for (int i = 0; i < chunkBounds.size(); ++i)
		{
			if (chunkBounds[i].contains(vertexPos))
			{
				vertexClassification[v].push_back(i);
			}
		}
	}
	return vertexClassification;
}

vector<vector<vector<shared_ptr<malha>>>> createChunks(shared_ptr<malha> m, const map<vertice, vector<int>> &vertexClassification, const vector<Bounds> &chunkBounds, float chunk_size_x, float chunk_size_y, float chunk_size_z)
{
	// Calculate the number of chunks in each dimension
	glm::vec3 meshSize = m->pegar_tamanho_maximo();
	int xSize = ceil(meshSize.x / chunk_size_x);
	int ySize = ceil(meshSize.y / chunk_size_y);
	int zSize = ceil(meshSize.z / chunk_size_z);

	vector<vector<vector<shared_ptr<malha>>>> chunks(xSize, vector<vector<shared_ptr<malha>>>(ySize, vector<shared_ptr<malha>>(zSize)));

	// Initialize each chunk as a new malha object
	for (int x = 0; x < xSize; ++x)
	{
		for (int y = 0; y < ySize; ++y)
		{
			for (int z = 0; z < zSize; ++z)
			{
				chunks[x][y][z] = std::make_shared<malha>();
			}
		}
	}

	// Create a map from vertice to int for ID mapping
	std::map<vertice, int> verticeToID;
	for (int i = 0; i < m->vertices.size(); ++i)
	{
		verticeToID[m->vertices[i]] = i;
	}

	// Mapping from global vertex to local chunk vertex index
	std::map<int, std::map<int, int>> globalToLocalIndex;

	// Iterate over each vertex in the original mesh and classify into chunks
	for (const auto &vc : vertexClassification)
	{
		const vertice &v = vc.first;
		const vector<int> &containingChunks = vc.second;

		int verticeID = verticeToID[v]; // Get the integer ID for v

		for (int chunkIndex : containingChunks)
		{
			int x = chunkIndex % xSize;
			int y = (chunkIndex / xSize) % ySize;
			int z = chunkIndex / (xSize * ySize);

			int localIndex = chunks[x][y][z]->vertices.size();
			chunks[x][y][z]->vertices.push_back(v);
			globalToLocalIndex[chunkIndex][verticeID] = localIndex;
		}
	}

	// Copy and adjust indices to each chunk
	for (int i = 0; i < m->indice.size(); i += 3)
	{
		int idx1 = m->indice[i];
		int idx2 = m->indice[i + 1];
		int idx3 = m->indice[i + 2];

		for (int chunkIndex = 0; chunkIndex < chunkBounds.size(); ++chunkIndex)
		{
			int x = chunkIndex % xSize;
			int y = (chunkIndex / xSize) % ySize;
			int z = chunkIndex / (xSize * ySize);

			auto it1 = vertexClassification.find(m->vertices[idx1]);
			auto it2 = vertexClassification.find(m->vertices[idx2]);
			auto it3 = vertexClassification.find(m->vertices[idx3]);

			if ((it1 != vertexClassification.end() && std::find(it1->second.begin(), it1->second.end(), chunkIndex) != it1->second.end()) ||
				(it2 != vertexClassification.end() && std::find(it2->second.begin(), it2->second.end(), chunkIndex) != it2->second.end()) ||
				(it3 != vertexClassification.end() && std::find(it3->second.begin(), it3->second.end(), chunkIndex) != it3->second.end()))
			{

				int localIdx1 = globalToLocalIndex[chunkIndex][idx1];
				int localIdx2 = globalToLocalIndex[chunkIndex][idx2];
				int localIdx3 = globalToLocalIndex[chunkIndex][idx3];

				chunks[x][y][z]->indice.push_back(localIdx1);
				chunks[x][y][z]->indice.push_back(localIdx2);
				chunks[x][y][z]->indice.push_back(localIdx3);
			}
		}
	}

	return chunks;
}

void handleBorderVertices(vector<vector<vector<shared_ptr<malha>>>> &chunks, const map<vertice, vector<int>> &vertexClassification, const vector<Bounds> &chunkBounds)
{
	// Iterate over each vertex and check if it belongs to multiple chunks (border vertex)
	for (const auto &vc : vertexClassification)
	{
		const vertice &v = vc.first;
		const vector<int> &containingChunks = vc.second;

		if (containingChunks.size() > 1)
		{
			// Vertex is a border vertex; duplicate it for each chunk
			for (int chunkIndex : containingChunks)
			{
				// Correctly access the chunk using its 3D indices
				int x, y, z; // Determine the correct x, y, z indices
				chunks[x][y][z]->vertices.push_back(v);
			}
		}
	}
}

void rebuildIndices(vector<vector<vector<shared_ptr<malha>>>> &chunks)
{
	// Iterate over each chunk
	for (auto &chunkRow : chunks)
	{
		for (auto &chunkColumn : chunkRow)
		{
			for (auto &chunk : chunkColumn)
			{
				std::map<unsigned int, unsigned int> indexMapping;

				// Build a mapping from old indices to new indices
				for (unsigned int i = 0; i < chunk->indice.size(); ++i)
				{
					unsigned int oldIndex = chunk->indice[i];
					if (indexMapping.find(oldIndex) == indexMapping.end())
					{
						indexMapping[oldIndex] = indexMapping.size();
					}
					chunk->indice[i] = indexMapping[oldIndex];
				}

				// Ensure indices form valid triangles
				// This might involve additional logic to adjust indices
			}
		}
	}
}

vector<vector<vector<shared_ptr<malha>>>> divide_mesh(shared_ptr<malha> m, float chunk_size_x, float chunk_size_y, float chunk_size_z)
{
	// Step 1: Calculate Chunk Bounds
	auto chunkBounds = calculateChunkBounds(m, chunk_size_x, chunk_size_y, chunk_size_z);

	// Step 2: Classify Vertices
	auto vertexClassification = classifyVertices(m->vertices, chunkBounds);

	// Step 3: Divide Mesh into Chunks
	auto chunks = createChunks(m, vertexClassification, chunkBounds, chunk_size_x, chunk_size_y, chunk_size_z);

	// Step 4: Handle Border Vertices
	handleBorderVertices(chunks, vertexClassification, chunkBounds);

	// Step 5: Rebuild Indices
	rebuildIndices(chunks);

	// Step 6: Return Chunks
	return chunks;
}

/*
vector<vector<vector<shared_ptr<malha>>>> divide_mesh(shared_ptr<malha> malha_bruta, float chunck_size_x, float chunck_size_y, float chunck_size_z)
{

	shared_ptr<malha> m = cut_mesh(m,chunck_size_x,chunck_size_y,chunck_size_z);

	vector<vector<vector<shared_ptr<malha>>>> chuncks;

	//print("m->vertices.size()", m->vertices.size());
	//print("m->indice.size()", m->indice.size());

	// Certificar-se de que os tamanhos dos chuncks são maiores que zero
	chunck_size_x = std::max(chunck_size_x, 1.0f);
	chunck_size_y = std::max(chunck_size_y, 1.0f);
	chunck_size_z = std::max(chunck_size_z, 1.0f);

	// Calcular o número de chuncks em cada dimensão
	m->pegar_tamanho_maximo();
	unsigned int numChuncksX = std::ceil(m->tamanho_maximo.x / chunck_size_x);
	unsigned int numChuncksY = std::ceil(m->tamanho_maximo.y / chunck_size_y);
	unsigned int numChuncksZ = std::ceil(m->tamanho_maximo.z / chunck_size_z);

	// print("divide_mesh size",numChuncksX,numChuncksY,numChuncksZ);

	chuncks.resize(numChuncksX, vector<vector<shared_ptr<malha>>>(numChuncksY, vector<shared_ptr<malha>>(numChuncksZ)));

	// Dividir a malha em chuncks
	for (unsigned int x = 0; x < numChuncksX; ++x)
	{
		for (unsigned int y = 0; y < numChuncksY; ++y)
		{
			for (unsigned int z = 0; z < numChuncksZ; ++z)
			{
				shared_ptr<malha> new_malha = std::make_shared<malha>();

				float minX = x * chunck_size_x;
				float maxX = (x + 1) * chunck_size_x;
				float minY = y * chunck_size_y;
				float maxY = (y + 1) * chunck_size_y;
				float minZ = z * chunck_size_z;
				float maxZ = (z + 1) * chunck_size_z;

				// Processar cada triângulo da malha original
				for (unsigned int i = 0; i < m->indice.size(); i += 3)
				{
					vertice v1 = m->vertices[m->indice[i]];
					vertice v2 = m->vertices[m->indice[i + 1]];
					vertice v3 = m->vertices[m->indice[i + 2]];

					//print("divide_mesh AAAAA");

					if (is_within_chunck(v1, minX, maxX, minY, maxY, minZ, maxZ) ||
						is_within_chunck(v2, minX, maxX, minY, maxY, minZ, maxZ) ||
						is_within_chunck(v3, minX, maxX, minY, maxY, minZ, maxZ))
					{
						// O triângulo intersecta o chunck
						new_malha->vertices.push_back(v1);
						new_malha->vertices.push_back(v2);
						new_malha->vertices.push_back(v3);
						new_malha->indice.push_back(new_malha->vertices.size() - 3);
						new_malha->indice.push_back(new_malha->vertices.size() - 2);
						new_malha->indice.push_back(new_malha->vertices.size() - 1);

						print("add item to new_malha");
					}
				}

				//print("new_malha->vertices.size()", new_malha->vertices.size());
				//print("new_malha->indice.size()", new_malha->indice.size());
				chuncks[x][y][z] = new_malha;
			}
		}
	}

	// return {{{m}}};

	return chuncks;
}
*/

struct objeto_3D_struct
{
	string nome;
	unsigned int id = -1;
	vec3 posicao = glm::vec3(0, 0, 0);
	quat quaternion = glm::quat(1, 0, 0, 0);
	vec3 escala = glm::vec3(1, 1, 1);
	mat4 inverseBindMatrix = glm::mat4(1.0f);
	vector<shared_ptr<malha>> minhas_malhas;
	vector<Material> meus_materiais;
	Table variaveis;
	vector<struct objeto_3D_struct> filhos;
};
typedef struct objeto_3D_struct objeto_3D;

struct key_frame_struct
{
	unsigned int object_id;
	bool has_position = false, has_scale = false, has_rotation = false;
	vec3 position = glm::vec3(0, 0, 0);
	vec3 scale = glm::vec3(1, 1, 1);
	quat rotation = glm::quat(1, 0, 0, 0);
};
typedef struct key_frame_struct key_frame;

struct animacao_struct
{
	string nome;
	float start_time = 0, duration = 0;
	std::vector<std::vector<key_frame>> keyFrames;
};
typedef struct animacao_struct animacao;

float transicionar(float temp, float A, float B)
{
	return A + ((B - A) * temp);
}

void transicionar_objeto_3D_individual(float progreso, objeto_3D *A, objeto_3D B)
{
	A->posicao = vec3(transicionar(progreso, A->posicao.x, B.posicao.x), transicionar(progreso, A->posicao.y, B.quaternion.y), transicionar(progreso, A->posicao.z, B.posicao.z));
	A->quaternion = quat(transicionar(progreso, A->quaternion.x, B.quaternion.x), transicionar(progreso, A->quaternion.y, B.quaternion.y), transicionar(progreso, A->quaternion.z, B.quaternion.z), transicionar(progreso, A->quaternion.w, B.quaternion.w));
	A->posicao = vec3(transicionar(progreso, A->escala.x, B.escala.x), transicionar(progreso, A->escala.y, B.escala.y), transicionar(progreso, A->escala.z, B.escala.z));
}

void transicionar_objeto_3D(float progreso, objeto_3D *A, objeto_3D B)
{
	transicionar_objeto_3D_individual(progreso, A, B);
	for (int i = 0; i < A->filhos.size(); i++)
	{
		transicionar_objeto_3D(progreso, &A->filhos[i], B.filhos[i]);
	}
}

class cena_3D : public asset
{
public:
	cena_3D() {}
	string nome;

	map<string, shared_ptr<malha>> malhas;
	map<string, Material> materiais;
	map<string, shared_ptr<imagem>> texturas;

	objeto_3D objetos;
	map<string, animacao> animacoes;
	map<size_t, mat4> offset_matrices;
	Table extras;
};
vec3 calculateNormal(const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c)
{
	glm::vec3 edge1 = b - a;
	glm::vec3 edge2 = c - a;
	return glm::normalize(glm::cross(edge1, edge2));
}

string mostFrequentString(vector<string> vec)
{
	unordered_map<string, int> freqMap;
	for (string value : vec)
	{
		freqMap[value]++;
	}
	string mostFrequentValue;
	int highestFrequency = 0;
	for (auto it = freqMap.begin(); it != freqMap.end(); it++)
	{
		if (it->second > highestFrequency)
		{
			highestFrequency = it->second;
			mostFrequentValue = it->first;
		}
	}
	return mostFrequentValue;
}

void triangulate(const glm::vec3 *vertices, size_t vertexCount, std::vector<glm::vec3> &output)
{
	if (vertexCount < 3)
		return;

	for (size_t i = 1; i < vertexCount - 1; ++i)
	{
		output.push_back(vertices[0]);
		output.push_back(vertices[i]);
		output.push_back(vertices[i + 1]);
	}
}

float vec3_distance(glm::vec3 a, glm::vec3 b)
{
	return glm::length(a - b);
}

// Function to sort the input vector of vec3 points based on the distance from the given point
std::vector<glm::vec3> vec3_ordenate_by_distance(glm::vec3 point, std::vector<glm::vec3> vec_points)
{
	// Create a vector to store the distances
	std::vector<float> distances(vec_points.size());

	// Calculate the distances from the given point to each of the points in the input vector
	for (int i = 0; i < vec_points.size(); i++)
	{
		distances[i] = distance(point, vec_points[i]);
	}

	// Create an index vector and sort it based on the distances
	std::vector<int> indices(vec_points.size());
	std::iota(indices.begin(), indices.end(), 0);
	std::sort(indices.begin(), indices.end(), [&](int i, int j)
			  { return distances[i] < distances[j]; });

	// Create a new vector to store the sorted vec3 points
	std::vector<glm::vec3> sorted_vec_points(vec_points.size());

	// Fill the new vector with the vec3 points in the order determined by the sorted index vector
	for (int i = 0; i < indices.size(); i++)
	{
		sorted_vec_points[i] = vec_points[indices[i]];
	}

	// Return the sorted vector of vec3 points
	return sorted_vec_points;
}

template <typename X, typename Y>
vector<X> pegar_primeiros_map(map<X, Y> m)
{
	vector<X> ret;
	for (pair<X, Y> p : m)
	{
		ret.push_back(p.first);
	}
	return ret;
}
template <typename X, typename Y>
void pegar_primeiros_map_thread(map<X, Y> m, vector<X> *ret)
{
	*ret = pegar_primeiros_map<X, Y>(m);
}

void pegar_nomes_malhas_thread(map<string, shared_ptr<malha>> m, vector<string> *ret)
{
	pegar_primeiros_map_thread<string, shared_ptr<malha>>(m, ret);
}

void pegar_nomes_materiais_thread(map<string, Material> m, vector<string> *ret)
{
	pegar_primeiros_map_thread<string, Material>(m, ret);
}

void pegar_nomes_texturas_thread(map<string, shared_ptr<imagem>> m, vector<string> *ret)
{
	pegar_primeiros_map_thread<string, shared_ptr<imagem>>(m, ret);
}

#define SAIDAS_SHADER 6
#define quantidade_fontes_luz 99
class API_grafica_classe
{
public:
	API_grafica_classe() {}

	bool update_res = false;
	bool retro_texture_compresion = true;

	string modelo_gpu;
	bool pixel_perfeito = true;
	ivec2 res_interna = ivec2(256, 256);
	vector<instrucoes_render> info_render;
	Material pos_processamento_info;
	uint8_t relevancia_camera = 0;

	virtual void iniciar_lib() {}
	virtual void desenhar_tela(Material mat) {}
	virtual void mudar_render_res(int X, int Y) {}
	virtual shared_ptr<imagem> captura_de_tela() { return NULL; }
	shared_ptr<imagem> rodar_compute_shader(Material mat, int resx, int resy, int chanels) { return NULL; }
	virtual int pegar_id_obj(int X, int Y) { return 0; }
	virtual void limpar_bufers_render(bool cor, bool profundidade) {}
	virtual void remover_textura(imagem *img) {}
	virtual void remover_malha(malha *ma) {}
	virtual void remover_fonte(fonte *f) {}
	virtual void remover_shader(shader *nome) {}
	virtual void reindenizar_cenario() {}
};
API_grafica_classe *api_grafica;

class network_api
{
public:
	network_api();
};

void remover_textura(imagem *img)
{
	if (api_grafica != NULL)
	{
		api_grafica->remover_textura(img);
		// cout << "textura: " << img << " foi removida\n";
	}
}
void remover_malha(malha *ma)
{
	if (api_grafica != NULL)
	{
		api_grafica->remover_malha(ma);
		// cout << "malha: " << ma->nome << " foi removida\n";
	}
}
void remover_fonte(fonte *f)
{
	if (api_grafica != NULL)
	{
		api_grafica->remover_fonte(f);
		// cout << "fonte: " << f << " foi removida\n";
	}
}

void mudar_render_res(int X, int Y)
{
	if (api_grafica != NULL)
	{
		api_grafica->mudar_render_res(X, Y);
	}
}
void limpar_bufers_render(bool cor, bool profundidade)
{
	if (api_grafica != NULL)
	{
		api_grafica->limpar_bufers_render(cor, profundidade);
	}
}
void reindenizar_cenario()
{
	if (api_grafica != NULL)
	{
		api_grafica->reindenizar_cenario();
	}
}
shared_ptr<imagem> captura_de_tela()
{
	return api_grafica->captura_de_tela();
}
shared_ptr<imagem> rodar_compute_shader(Material mat, int resx, int resy, int chanels)
{
	return api_grafica->rodar_compute_shader(mat, resx, resy, chanels);
}

class network_conection
{
private:
	string ip;
	int port;

public:
	network_conection();
	void send(string s) {}
	string receve() { return ""; }
};

template <typename Y>
class mapeamento_assets
{
private:
	map<string, shared_ptr<Y>> mapa;

public:
	mapeamento_assets() {}
	string local;

	shared_ptr<Y> aplicar(string local, Y valor)
	{
		if (mapa.find(local) == mapa.end())
		{
			mapa.insert(pair<string, shared_ptr<Y>>(local, NULL));
		}
		mapa[local] = make_shared<Y>(valor);
		return mapa[local];
	}

	void aplicar_ptr(string local, shared_ptr<Y> valor)
	{
		if (mapa.find(local) == mapa.end())
		{
			mapa.insert(pair<string, shared_ptr<Y>>(local, NULL));
		}
		mapa[local] = valor;
	}

	void retirar(string local)
	{
		map<string, shared_ptr<Y>> mapa2;
		for (pair<string, shared_ptr<Y>> p : mapa)
		{
			if (p.first.compare(local) != 0)
			{
				mapa2.insert(p);
			}
		}
		mapa.swap(mapa2);
	}

	void limpar()
	{
		map<string, shared_ptr<Y>> m;
		mapa.swap(m);
	}
	void limpar_lixo()
	{
		map<string, shared_ptr<Y>> mapa2;
		for (pair<string, shared_ptr<Y>> p : mapa)
		{
			if (p.second.use_count() > 2)
			{
				// cout << p.second.use_count() << endl;
				mapa2.insert(p);
			}
			else
			{
				// cout << p.second.get() << " foi deletado\n";
			}
		}
		mapa.swap(mapa2);
	}

	shared_ptr<Y> pegar(string local)
	{
		if (mapa.find(local) != mapa.end())
		{
			return mapa[local];
		}
		else
		{
			shared_ptr<Y> ret = NULL;
			return ret;
		}
	}
};

map<char, int> char_int = {
	pair<char, int>('0', 0),
	pair<char, int>('1', 1),
	pair<char, int>('2', 2),
	pair<char, int>('3', 3),
	pair<char, int>('4', 4),
	pair<char, int>('5', 5),
	pair<char, int>('6', 6),
	pair<char, int>('7', 7),
	pair<char, int>('8', 8),
	pair<char, int>('9', 9)};

map<char, bool> YN = {
	pair<char, bool>('y', true),
	pair<char, bool>('n', false),
	pair<char, bool>('Y', true),
	pair<char, bool>('N', false),
};

template <typename X>
void escrever(X texto)
{
	cout << texto << endl;
}

vec3 quat_graus(quat q)
{
	// return degrees(glm::eulerAngles(q) * 3.14159f / 180.f);
	return glm::degrees(glm::eulerAngles(q));
}

quat graus_quat(vec3 r)
{
	return quat(radians(r));
}

bool e_visivel(mat4 P, mat4 V, mat4 T, vector<vec3> pos)
{
	bool ret = false;

	mat4 mat = P * V * T;

	for (vec3 p : pos)
	{

		vec4 pos_final = mat * vec4(p.x, p.y, p.z, 1);
	}

	return ret;
}

bool e_visivel(mat4 P, mat4 V, mat4 T, vec3 pos)
{
	return e_visivel(P, V, T, {pos});
}

bool e_visivel(mat4 P, mat4 V, mat4 T)
{
	return e_visivel(P, V, T, vec3(0, 0, 0));
}

template <typename X>
X *vetor_ponteiro(vector<X> a)
{
	X *ret;
	ret = new X[a.size()];
	for (int i = 0; i < a.size(); i++)
	{
		ret[i] = a[i];
	}
	return ret;
}

template <typename X>
vector<X> ponteiro_vetor(X *a, int tamanho)
{
	vector<X> ret;
	ret.resize(tamanho);

	return ret;
}

template <typename X>
string pegar_tipo_variavel(X x)
{
	return typeid(x).name();
}
template <typename X>
string pegar_tipo_variavel()
{
	return typeid(X).name();
}

// template<typename X>
// void escrever(X texto) { cout << texto << endl; }

template <typename X>
string ponteiro_string(X *p)
{
	stringstream ss;
	ss << p;
	return ss.str();
}

template <typename X>
X *string_ponteiro(string s)
{
	stringstream ss(s);
	void *ret = NULL;
	ss >> ret;
	return (X *)ret;
}

template <typename X, typename Y>
vector<pair<X, Y>> map_vetor(map<X, Y> m)
{
	vector<pair<X, Y>> ret;
	ret.resize(0);
	for (pair<X, Y> p : m)
	{
		ret.push_back(p);
	}
	return ret;
}

void setar_diretorio_aplicacao(string local)
{
	std::filesystem::current_path(local);
}

string pegar_estencao_arquivo(string arquivo)
{
	string ret = arquivo;
	for (int i = 0; i < arquivo.length(); i++)
	{
		if (arquivo.at(i) != '.')
		{
			ret.erase(ret.begin());
		}
		else
		{
			break;
		}
	}
	return ret;
}
string pegar_nome_arquivo(string arquivo)
{
	return arquivo.substr(arquivo.find_last_of("/\\") + 1);
}
string pegar_pasta_arquivo(string arquivo)
{
	string ret = arquivo;
	for (int i = arquivo.length() - 1; i > 0; i--)
	{
		if (arquivo.at(i) != '/')
		{
			ret.pop_back();
		}
		else
		{
			break;
		}
	}
	return ret;
}

class lista_ponteiros
{
	string tipo;
	vector<void *> lista;
	vector<int> items_vazios;

	template <typename X>
	void Reset()
	{
		tipo = pegar_tipo_variavel<X>();
		lista.clear();
	}

	template <typename X>
	lista_ponteiros()
	{
		Reset<X>();
	}

	void adicionar(void *item)
	{
		if (items_vazios.size() == 0)
		{
			lista.push_back(item);
		}
		else
		{
			lista[items_vazios[0]] = item;
			items_vazios.erase(items_vazios.begin());
		}
	}

	void remover(int i)
	{
		lista[i] = NULL;
		items_vazios.push_back(i);
	}
};

std::vector<std::string> stringRemoveDuplicates(const std::vector<std::string> &vec)
{
	std::vector<std::string> result;
	for (const std::string &str : vec)
	{
		if (std::find(result.begin(), result.end(), str) == result.end())
		{
			result.push_back(str);
		}
	}
	return result;
}
