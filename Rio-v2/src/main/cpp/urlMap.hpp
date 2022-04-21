// URL mapping stuff, experimental so don't delete it.

// Please, don't delete it.
#ifndef URLMAP_HPP
#define URLMAP_HPP

#include <string>
#include <vector>
#include "httpserver.hpp"

struct mapping{
    std::string urlRoot;
    bool matchFollowing = false; // Is the url full, or only the top-level? This allows things like /snow/showers to also be caught by /snow.
    bool matchMetadata = true; // Can have data after the #.
    uint8_t importance = 0; // Override importance. This is added to the specificity value.
    std::function<void(HTTPARGS)> func;
};

struct UrlMap{
private:
    std::vector <mapping*> maps;
public:
    std::function<void(HTTPARGS)> no_binding_error = [](HTTPARGS){
        ron -> status = 404;
        ron -> content = "No such thing exists!";
    };
    static uint8_t match(std::string url, mapping *map){
        uint8_t specificity = map -> importance;
        if (url == map -> urlRoot){
            specificity = 3;
        }
        else if (map -> urlRoot == url.substr(0, map -> urlRoot.size())){
            if (map -> matchFollowing){
                specificity = 1;
            }
            else if (map -> matchMetadata && (url.at(map -> urlRoot.size()) == '#')) {
                specificity = 2;
            }
        }
        return specificity;
    }
    void push(mapping *map){
        maps.push_back(map);
    }
    uint8_t operator() (HTTPARGS) const {
        mapping *winner = nullptr;
        uint8_t winningValue = 0;
        for (unsigned int x = 0; x < maps.size(); x ++){
            uint8_t matchValue = match(req -> url, maps[x]);
            if (winner == nullptr || winningValue < matchValue){
                winningValue = matchValue;
                winner = maps[x];
            }
        }
        if (winner == nullptr || winningValue == 0){
            no_binding_error(HTTPPASSARGS);
        }
        else{
            winner -> func(HTTPPASSARGS);
        }
        return winningValue;
    }
};
#endif
