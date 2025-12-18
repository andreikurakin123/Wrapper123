#pragma once

template<typename ClassType, typename RetType, typename... ParamTypes>
template<typename T>
T Wrapper<ClassType, RetType, ParamTypes...>::getParameter(
    const std::map<std::string, std::any>& params, 
    const std::string& name, 
    const T& defaultVal) const {
    
    auto found = params.find(name);
    if (found != params.end()) {
        try {
            return std::any_cast<T>(found->second);
        } catch (const std::bad_any_cast&) {
            throw std::invalid_argument("Parameter '" + name + 
                "' has wrong type. Expected: " + getTypeName(typeid(T)) + 
                ", got: " + getTypeName(found->second.type()));
        }
    }
    return defaultVal;
}

template<typename ClassType, typename RetType, typename... ParamTypes>
template<size_t... Indices>
auto Wrapper<ClassType, RetType, ParamTypes...>::gatherParams(
    const std::map<std::string, std::any>& params,
    std::index_sequence<Indices...>) const {
    
    return std::make_tuple(
        getParameter(params, 
                    parameterNames[Indices], 
                    std::get<Indices>(defaults))...
    );
}

template<typename ClassType, typename RetType, typename... ParamTypes>
auto Wrapper<ClassType, RetType, ParamTypes...>::callMethod(
    std::tuple<ParamTypes...>& args) {
    
    auto fullArgs = std::tuple_cat(std::make_tuple(object), args);
    
    if constexpr (std::is_void_v<RetType>) {
        std::apply(methodPtr, fullArgs);
        return std::any();
    } else {
        return std::any(std::apply(methodPtr, fullArgs));
    }
}

template<typename ClassType, typename RetType, typename... ParamTypes>
Wrapper<ClassType, RetType, ParamTypes...>::Wrapper(
    ClassType* obj,
    RetType (ClassType::*method)(ParamTypes...),
    const std::vector<std::pair<std::string, std::any>>& defaultParams)
    : object(obj), methodPtr(method) {
    
    if (defaultParams.size() != sizeof...(ParamTypes)) {
        throw std::invalid_argument(
            "Parameter count mismatch: expected " + 
            std::to_string(sizeof...(ParamTypes)) + 
            " parameter(s), got " + std::to_string(defaultParams.size()));
    }

    defaults = extractDefaults(defaultParams, std::index_sequence_for<ParamTypes...>{});
    
    for (const auto& [name, value] : defaultParams) {
        parameterNames.push_back(name);
    }
}

template<typename ClassType, typename RetType, typename... ParamTypes>
std::any Wrapper<ClassType, RetType, ParamTypes...>::execute(
    const std::map<std::string, std::any>& params) {
    
    for (const auto& name : parameterNames) {
        if (params.find(name) == params.end()) {
            throw std::invalid_argument("Missing required parameter: " + name);
        }
    }
    
    auto args = gatherParams(params, std::index_sequence_for<ParamTypes...>{});
    return callMethod(args);
}

template<typename ClassType, typename RetType, typename... ParamTypes>
template<size_t... Indices>
std::tuple<ParamTypes...> Wrapper<ClassType, RetType, ParamTypes...>::extractDefaults(
    const std::vector<std::pair<std::string, std::any>>& defaultParams,
    std::index_sequence<Indices...>) {
    
    return std::make_tuple(
        extractDefault<std::tuple_element_t<Indices, std::tuple<ParamTypes...>>>(
            defaultParams[Indices])...
    );
}

template<typename ClassType, typename RetType, typename... ParamTypes>
template<typename T>
T Wrapper<ClassType, RetType, ParamTypes...>::extractDefault(
    const std::pair<std::string, std::any>& param) {
    
    try {
        return std::any_cast<T>(param.second);
    } catch (const std::bad_any_cast&) {
        throw std::invalid_argument(
            "Default value for parameter '" + param.first + 
            "' has wrong type. Expected: " + getTypeName(typeid(T)) + 
            ", got: " + getTypeName(param.second.type()));
    }
}
