#pragma once
#include "table.h"
#include "RecursosT.h"
#include "ManusearArquivos.h"
#include <algorithm>

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
        if(t.haveString(to_string(i))){
            v.push_back(t.getFloat(to_string(i)));
        }else{
            break;
        }
        i++;
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
        }else{
            break;
        }
        i++;
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
        }else{
            break;
        }
        i++;
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

    vector<float> inputs = table_vFloat(t.getTable("inputs"));
	for (int i = 0; i < std::min((int)NO_INPUTS,(int)inputs.size()); i++) {
        m.inputs[i] = inputs[i];
	}

    return m;
}

Table material_table(Material m){
    Table t;
    t.setString("shader",m.shad);

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