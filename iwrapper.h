#pragma once

#include <any>
#include <map>
#include <string>


class IWrapper {
public:
    virtual ~IWrapper() = default;

    virtual std::any execute(const std::map<std::string, std::any>& params) = 0;
};
