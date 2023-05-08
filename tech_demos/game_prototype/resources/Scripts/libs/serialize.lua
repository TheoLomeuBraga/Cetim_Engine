local serpent = require("libs.serpent")
local base64 = require("libs.base64")
local json = require("libs.json")

local serializer = {}

function serializer.save_table(file_name, table)
    local file = io.open(file_name, "w")
    if file then
        local table_serializada = serpent.dump(table)
        file:write(table_serializada)
        file:close()
    else
        error("Erro ao abrir file para salvar table")
    end
end

-- Função para carregar uma table a partir de um file
function serializer.load_table(file_name)
    local file = io.open(file_name, "r")
    if file then
        local conteudo_file = file:read("*all")
        file:close()
        local table = load(conteudo_file)()
        return table
    else
        error("Erro ao abrir file para carregar table")
    end
end

function serializer.save_table_base64(file_name, table)
    local file = io.open(file_name, "w")
    if file then
        local table_serializada = serpent.dump(table)
        file:write(base64:encode(table_serializada))
        file:close()
    else
        error("Erro ao abrir file para salvar table")
    end
end

-- Função para carregar uma table a partir de um file
function serializer.load_table_base64(file_name)
    local file = io.open(file_name, "r")
    if file then
        local conteudo_file = file:read("*all")
        file:close()
        local table = load(base64:decode(conteudo_file))()
        return table
    else
        error("Erro ao abrir file para carregar table")
    end
end

function serializer.save_table_json(file_name, table)
    local file = io.open(file_name, "w")
    if file then
        file:write(json.encode(table))
        file:close()
    else
        error("Erro ao abrir file para salvar table")
    end
end

-- Função para carregar uma table a partir de um file
function serializer.load_table_json(file_name)
    local file = io.open(file_name, "r")
    if file then
        local conteudo_file = file:read("*all")
        file:close()
        local table = json.decode(conteudo_file)
        return table
    else
        error("Erro ao abrir file para carregar table")
    end
end

return serializer
