#pragma once

#include "iwrapper.h"
#include <tuple>
#include <vector>
#include <stdexcept>
#include <type_traits>
#include <string>

inline std::string getTypeName(const std::type_info& type) {
    if (type == typeid(int)) return "int";
    if (type == typeid(double)) return "double";
    if (type == typeid(float)) return "float";
    if (type == typeid(bool)) return "bool";
    if (type == typeid(char)) return "char";
    if (type == typeid(std::string)) return "string";
    if (type == typeid(long)) return "long";
    if (type == typeid(short)) return "short";
    if (type == typeid(unsigned int)) return "unsigned int";
    if (type == typeid(long long)) return "long long";
    return type.name();
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