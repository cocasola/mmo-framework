//
// Created by Keegan Beaulieu on 2024-07-27.
//

#ifndef CONTEXTEDITOR_H
#define CONTEXTEDITOR_H

#include <UiContainer.h>
#include <Component.h>
#include <Entity.h>
#include <UiViewport.h>

#include "ContextExplorer.h"
#include "EntityInspector.h"

class EntityInspector;
class ContextExplorer;

class ContextEditor : public UiContainer
{
public:
    enum EditorTransformation
    {
        EDITOR_TRANS_NONE = 0,
        EDITOR_TRANS_TRANSLATION,
        EDITOR_TRANS_ROTATION,
        EDITOR_TRANS_SCALING
    };

    enum EditorAxis
    {
        EDITOR_AXIS_NONE = 0,
        EDITOR_AXIS_X,
        EDITOR_AXIS_Y,
        EDITOR_AXIS_Z
    };

    Entity *selected;
    EditorTransformation transformation;
    EditorAxis transformation_axes;

    static ContextEditor *current;

    void select_entity(Entity *entity);
    Entity *create_entity();
    void draw_gizmos();

    static ContextEditor *create(UiContainer *parent);

private:
    UiViewport *viewport;
    ContextExplorer *explorer;
    EntityInspector *inspector;

    Event<Window::MouseEvent>::Subscription *mouse_move_subscription;

    static void handle_mouse_move(Window::MouseEvent event, void *);
    static void handle_destroy(UiContainer *container, void *);
};

#endif //CONTEXTEDITOR_H
