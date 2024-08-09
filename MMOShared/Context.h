//
// Created by Keegan Beaulieu on 2024-07-24.
//

#ifndef CONTEXT_H
#define CONTEXT_H

#include "List.h"
#include "Component.h"
#include "Entity.h"

class Entity;

class Context
{
public:
    List<Entity *> entities;
    std::string name;

    static List<Context> contexts;
    static Context *_default;

    static void init();
    static void cleanup();

    static Context *create();
    static Context *create(const std::string &name);
};

#endif //CONTEXT_H
