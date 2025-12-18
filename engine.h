#pragma once

#include "iwrapper.h"
#include <unordered_map>
#include <memory>
#include <string>
#include <mutex>
#include <stdexcept>

class Engine {
private:
    std::unordered_map<std::string, std::shared_ptr<IWrapper>> registry;
    mutable std::mutex accessMutex;

public:
    void registerCommand(std::shared_ptr<IWrapper> wrapper, const std::string& commandName);

    void registerCommand(IWrapper* wrapper, const std::string& commandName);

    std::any execute(const std::string& commandName, 
                    const std::map<std::string, std::any>& args);

    std::any execute(const std::string& commandName,
                    std::initializer_list<std::pair<std::string, std::any>> args);

    bool hasCommand(const std::string& commandName) const;

    bool unregisterCommand(const std::string& commandName);
};
