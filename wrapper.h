#pragma once

#include "iwrapper.h"
#include <tuple>
#include <vector>
#include <stdexcept>
#include <type_traits>
#include <string>
#include <typeindex>
#include <unordered_map>

inline std::string getTypeName(const std::type_info& type) {
    static const std::unordered_map<std::type_index, std::string> typeNames = {
        {typeid(int), "int"},
        {typeid(double), "double"},
        {typeid(float), "float"},
        {typeid(bool), "bool"},
        {typeid(char), "char"},
        {typeid(std::string), "string"},
        {typeid(long), "long"},
        {typeid(short), "short"},
        {typeid(unsigned int), "unsigned int"},
        {typeid(long long), "long long"}
    };
    
    auto it = typeNames.find(std::type_index(type));
    return it != typeNames.end() ? it->second : type.name();
}

template<typename ClassType, typename RetType, typename... ParamTypes>
class Wrapper : public IWrapper {
private:
    ClassType* object;
    RetType (ClassType::*methodPtr)(ParamTypes...);
    std::vector<std::string> parameterNames;
    std::tuple<ParamTypes...> defaults;

    template<typename T>
    T getParameter(const std::map<std::string, std::any>& params, 
                   const std::string& name, 
                   const T& defaultVal) const;

    template<size_t... Indices>
    auto gatherParams(const std::map<std::string, std::any>& params,
                      std::index_sequence<Indices...>) const;

    auto callMethod(std::tuple<ParamTypes...>& args);

public:
    Wrapper(ClassType* obj,
            RetType (ClassType::*method)(ParamTypes...),
            const std::vector<std::pair<std::string, std::any>>& defaultParams);

    std::any execute(const std::map<std::string, std::any>& params) override;

private:
    template<size_t... Indices>
    std::tuple<ParamTypes...> extractDefaults(
        const std::vector<std::pair<std::string, std::any>>& defaultParams,
        std::index_sequence<Indices...>);

    template<typename T>
    T extractDefault(const std::pair<std::string, std::any>& param);
};

#include "wrapper.tpp"