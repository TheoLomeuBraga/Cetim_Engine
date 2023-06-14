#pragma once
#include <string>
#include <unordered_map>

class Table {
public:
    std::unordered_map<std::string, float> m_floatMap;
    std::unordered_map<std::string, std::string> m_stringMap;
    std::unordered_map<std::string, Table> m_tableMap;

    bool haveFloat(const std::string& key){
        if(m_floatMap.find(key) != m_floatMap.end()){return true;}else{return false;}
    }

    bool haveString(const std::string& key){
        if(m_stringMap.find(key) != m_stringMap.end()){return true;}else{return false;}
    }

    bool haveTable(const std::string& key){
        if(m_tableMap.find(key) != m_tableMap.end()){return true;}else{return false;}
    }

    void setFloat(const std::string& key, float value) {
        m_floatMap[key] = value;
    }

    float getFloat(const std::string& key) {
        return m_floatMap[key];
    }

    void setString(const std::string& key, const std::string& value) {
        m_stringMap[key] = value;
    }

    std::string getString(const std::string& key) {
        return m_stringMap[key];
    }

    void setTable(const std::string& key, const Table& table) {
        m_tableMap[key] = table;
    }

    Table getTable(const std::string& key) {
        return m_tableMap[key];
    }


    
};