#include "engine.h"

void Engine::registerCommand(std::shared_ptr<IWrapper> wrapper, const std::string& commandName) {
    std::lock_guard<std::mutex> guard(accessMutex);
    registry[commandName] = wrapper;
}

void Engine::registerCommand(IWrapper* wrapper, const std::string& commandName) {
    auto ptr = std::shared_ptr<IWrapper>(wrapper, [](IWrapper*){});
    registerCommand(ptr, commandName);
}

std::any Engine::execute(const std::string& commandName, 
                        const std::map<std::string, std::any>& args) {
    std::lock_guard<std::mutex> guard(accessMutex);
    
    auto it = registry.find(commandName);
    if (it == registry.end()) {
        throw std::invalid_argument("Command not found: " + commandName);
    }
    
    return it->second->execute(args);
}

std::any Engine::execute(const std::string& commandName,
                        std::initializer_list<std::pair<std::string, std::any>> args) {
    return execute(commandName, std::map<std::string, std::any>(args.begin(), args.end()));
}

bool Engine::hasCommand(const std::string& commandName) const {
    std::lock_guard<std::mutex> guard(accessMutex);
    return registry.find(commandName) != registry.end();
}

bool Engine::unregisterCommand(const std::string& commandName) {
    std::lock_guard<std::mutex> guard(accessMutex);
    return registry.erase(commandName) > 0;
}
