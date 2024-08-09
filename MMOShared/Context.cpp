//
// Created by Keegan Beaulieu on 2024-07-24.
//

#include "Context.h"

Context *Context::_default = nullptr;
List<Context> Context::contexts;

void Context::init()
{
    _default = create("Default");
}

void Context::cleanup()
{
    contexts.clear();
}

Context *Context::create()
{
    return create("Context");
}

Context *Context::create(const std::string &name)
{
    Context *context = contexts.alloc();
    context->name = name;

    return context;
}
