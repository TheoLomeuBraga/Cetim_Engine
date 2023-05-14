#pragma once
#include "table.h"
#include "RecursosT.h"
#include "ManusearArquivos.h"
#include <algorithm>

#include <nlohmann/json.hpp>
#include <unordered_map>

using json = nlohmann::json;

//vectors

vec2 table_vec2(Table t){
    return vec2(t.getFloat("x"),t.getFloat("y"));
}
Table vec2_table(vec2 v){
    Table t;
    t.setFloat("x",v.x);
    t.setFloat("y",v.y);
    return t;
}

vec3 table_vec3(Table t){
    return vec3(t.getFloat("x"),t.getFloat("y"),t.getFloat("z"));
}
Table vec3_table(vec3 v){
    Table t;
    t.setFloat("x",v.x);
    t.setFloat("y",v.y);
    t.setFloat("z",v.z);
    return t;
}

vec4 table_vec4(Table t){
    return vec4(t.getFloat("x"),t.getFloat("y"),t.getFloat("z"),t.getFloat("w"));
}
Table vec4_table(vec4 v){
    Table t;
    t.setFloat("x",v.x);
    t.setFloat("y",v.y);
    t.setFloat("z",v.z);
    t.setFloat("w",v.w);
    return t;
}

quat table_quat(Table t){
    return quat(t.getFloat("x"),t.getFloat("y"),t.getFloat("z"),t.getFloat("w"));
}
Table quat_table(quat q){
    Table t;
    t.setFloat("x",q.x);
    t.setFloat("y",q.y);
    t.setFloat("z",q.z);
    t.setFloat("w",q.w);
    return t;
}

//list vectors

vector<float> table_vFloat (Table t){
    vector<float> v;
    int i = 1;
    while(true){
        if(t.haveFloat(to_string(i))){
            v.push_back(t.getFloat(to_string(i)));
            i++;
        }else{
            break;
        }
    }
    return v;
}
Table vFloat_table(vector<float> v){
    Table t;
    for(int i = 0; i < v.size();i++){
        t.setFloat(to_string(i + 1),v[i]);
    }
    return t;
}

vector<std::string> table_vString(Table t){
    vector<std::string> v;
    int i = 1;
    while(true){
        if(t.haveString(to_string(i))){
            v.push_back(t.getString(to_string(i)));
            i++;
        }else{
            break;
        }
    }
    return v;
}
Table vString_table (vector<std::string> v){
    Table t;
    for(int i = 0; i < v.size();i++){
        t.setString(to_string(i + 1),v[i]);
    }
    return t;
}

vector<Table> table_vTable (Table t){
    vector<Table> v;
    int i = 1;
    while(true){
        if(t.haveTable(to_string(i))){
            v.push_back(t.getTable(to_string(i)));
            i++;
        }else{
            break;
        }
    }
    return v;
}
Table vTable_table(vector<Table> v){
    Table t;
    for(int i = 0; i < v.size();i++){
        t.setTable(to_string(i + 1),v[i]);
    }
    return t;
}

//Material

Material table_material(Table t){
    Material m;
    m.shad = t.getString("shader");
    m.lado_render = (char)t.getFloat("normal_direction");

    Table color = t.getTable("color");
    m.cor = vec4(color.getFloat("r"),color.getFloat("g"),color.getFloat("b"),color.getFloat("a"));

    Table position_scale = t.getTable("position_scale");
    m.uv_pos_sca = vec4(position_scale.getFloat("x"),position_scale.getFloat("y"),position_scale.getFloat("z"),position_scale.getFloat("w"));

    m.metalico = t.getFloat("metallic");
    m.suave = t.getFloat("softness");

    
    vector<string> textures = table_vString(t.getTable("textures"));
    for (int i = 0; i < std::min((int)NO_TEXTURAS,(int)textures.size()); i++) {
		m.texturas[i] = ManuseioDados::carregar_Imagem(textures[i]);
	}

    vector<float> filters = table_vFloat(t.getTable("texture_filter"));
    for (int i = 0; i < std::min((int)NO_TEXTURAS,(int)filters.size()); i++) {
		m.filtro[i] = filters[i];
	}

    vector<float> inputs = table_vFloat(t.getTable("inputs"));
	for (int i = 0; i < std::min((int)NO_INPUTS,(int)inputs.size()); i++) {
        m.inputs[i] = inputs[i];
	}

    

    return m;
}

Table material_table(Material m){
    Table t;
    t.setString("shader",m.shad);
    t.setFloat("normal_direction",(float)m.lado_render);

    Table color;
    color.setFloat("r",m.cor.r);
    color.setFloat("g",m.cor.g);
    color.setFloat("b",m.cor.b);
    color.setFloat("a",m.cor.a);
    t.setTable("color",color);

    Table position_scale;
    position_scale.setFloat("x",m.cor.r);
    position_scale.setFloat("y",m.cor.g);
    position_scale.setFloat("z",m.cor.b);
    position_scale.setFloat("w",m.cor.a);
    t.setTable("position_scale",position_scale);

    t.setFloat("metallic",m.metalico);
    t.setFloat("softness",m.suave);

    vector<std::string> textures;
    for (int i = 0; i < NO_TEXTURAS; i++) {
        if(m.texturas[i] != NULL){
            textures.push_back(m.texturas[i]->local);
        }
    }
    t.setTable("textures",vString_table(textures));

    vector<float> filters;
    for (int i = 0; i < NO_TEXTURAS; i++) {
        filters.push_back(m.filtro[i]);
    }
    t.setTable("texture_filter",vFloat_table(filters));

    vector<float> inputs;
    for (int i = 0; i < NO_INPUTS; i++) {
        inputs.push_back(m.inputs[i]);
    }
    t.setTable("inputs",vFloat_table(inputs));

    
    return t;
}

instrucoes_render table_instrucoes_render(Table t){
    instrucoes_render ret;
    ret.camera = t.getFloat("camera_selected");
    ret.iniciar_render = t.getFloat("start_render");
    ret.limpar_buffer_cores = t.getFloat("clean_color");
    ret.limpar_buffer_profundidade = t.getFloat("clean_deph");
    ret.desenhar_objetos = t.getFloat("enable");
    ret.terminar_render = t.getFloat("end_render");
    ret.usar_profundidade = t.getFloat("use_deeph");
    return ret;
}

Table table_instrucoes_render(instrucoes_render ir){
    Table ret;
    ret.setFloat("camera_selected",ir.camera);
    ret.setFloat("start_render",ir.iniciar_render);
    ret.setFloat("clean_color",ir.limpar_buffer_cores);
    ret.setFloat("clean_deph",ir.limpar_buffer_profundidade);
    ret.setFloat("enable",ir.desenhar_objetos);
    ret.setFloat("end_render",ir.terminar_render);
    ret.setFloat("use_deeph",ir.usar_profundidade);
    return ret;
}

info_camada table_info_camada(Table t){
    info_camada ret;
    ret.camada = t.getFloat("layer");
    vector<int> camada_colide;
    for(float i : table_vFloat(t.getTable("layers_can_colide"))){
        camada_colide.push_back(i);
    }
    ret.camada_colide = camada_colide;
    return ret;
}

Table info_camada_table(info_camada ic){
    Table ret;
    ret.setFloat("layer",ic.camada);
    vector<float> layers_can_colide;
    for(int i : ic.camada_colide){
        layers_can_colide.push_back(i);
    }
    ret.setTable("layers_can_colide",vFloat_table(layers_can_colide));
    return ret;
}

Table colis_info_table(colis_info col){
    Table ret;
    ret.setString("object",ponteiro_string(col.obj));
    ret.setString("collision_object",ponteiro_string(col.cos_obj));
    ret.setFloat("triger",col.sensor);
    ret.setFloat("colliding",col.colidindo);
    ret.setFloat("distance",col.distancia);
    ret.setTable("position",vec3_table(col.pos));
    ret.setFloat("speed",col.velocidade);
    ret.setTable("normal",vec3_table(col.nor));
    return ret;
}

Table json_table(const json& j) {
    Table table;
    for (auto& el : j.items()) {
        if (el.value().is_number_float()) {
            table.setFloat(el.key(), el.value().get<float>());
        } else if (el.value().is_string()) {
            table.setString(el.key(), el.value().get<std::string>());
        } else if (el.value().is_object()) {
            Table innerTable = json_table(el.value());
            table.setTable(el.key(), innerTable);
        }
    }
    return table;
}

json table_json(const Table& table) {
    json j;
    for (const auto& kv : table.m_floatMap) {
        j[kv.first] = kv.second;
    }
    for (const auto& kv : table.m_stringMap) {
        j[kv.first] = kv.second;
    }
    for (const auto& kv : table.m_tableMap) {
        json innerJson = table_json(kv.second);
        j[kv.first] = innerJson;
    }
    return j;
}

Table object_3D_table(objeto_3D obj){
    Table ret;

    ret.setString("name",obj.nome);

    ret.setFloat("id",(int)obj.id);

    ret.setTable("position",vec3_table(obj.posicao));
    ret.setTable("rotation",vec3_table(quat_graus(obj.quaternion)));
    ret.setTable("scale",vec3_table(obj.escala));

    vector<Table> meshes;
    for(shared_ptr<malha> p : obj.minhas_malhas){
        Table this_mesh;
        this_mesh.setString("file",p->arquivo_origem);
        this_mesh.setString("name",p->nome);
        meshes.push_back(this_mesh);
    }
    ret.setTable("meshes",vTable_table(meshes));

    vector<Table> materials;
    for(Material p : obj.meus_materiais){
        materials.push_back(material_table(p));
    }
    ret.setTable("materials",vTable_table(materials));

    ret.setTable("variables",obj.variaveis);

    vector<Table> children;
    for(objeto_3D o : obj.filhos){
        children.push_back(object_3D_table(o));
    }
    ret.setTable("children",vTable_table(children));

    return ret;
}

Table scene_3D_table(cena_3D sceane){
    Table ret;

    ret.setString("path",sceane.nome);

    vector<Table> meshes;
    for(pair<string, shared_ptr<malha>> p : sceane.malhas){
        Table this_mesh;
        this_mesh.setString("file",p.second->arquivo_origem);
        this_mesh.setString("name",p.second->nome);
        meshes.push_back(this_mesh);
    }
    ret.setTable("meshes",vTable_table(meshes));

    vector<Table> materials;
    for(pair<string, Material> p : sceane.materiais){
        materials.push_back(material_table(p.second));
    }
    ret.setTable("materials",vTable_table(materials));

    vector<string> textures;
    for(pair<string, shared_ptr<imagem>> p : sceane.texturas){
        textures.push_back(p.first);
    }
    ret.setTable("textures",vString_table(textures));

    vector<Table> animations;
    for(pair<string, animacao> p : sceane.animacoes){
        Table animation_table;
        animation_table.setFloat("duration",p.second.duration);

        vector<Table> key_frames;
        for(key_frame kf : p.second.keyFrames){
            Table kf_table;

            kf_table.setFloat("object_id",kf.object_id);

            kf_table.setFloat("has_position",kf.has_position);
            kf_table.setFloat("has_scale",kf.has_scale);
            kf_table.setFloat("has_rotation",kf.has_rotation);

            kf_table.setTable("position",vec3_table(kf.position));
            kf_table.setTable("scale",vec3_table(kf.scale));
            kf_table.setTable("rotation",quat_table(kf.rotation));

            key_frames.push_back(kf_table);
        }

        animation_table.setTable("key_frames",vTable_table(key_frames));

        animations.push_back(animation_table);
    }
    ret.setTable("animations",vTable_table(animations));
    
    ret.setTable("objects",object_3D_table(sceane.objetos));
    ret.setTable("extra",sceane.extras);
    return ret;
}




