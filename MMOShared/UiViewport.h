//
// Created by Keegan Beaulieu on 2024-07-21.
//

#ifndef UIVIEWPORT_H
#define UIVIEWPORT_H

#include "UiContainer.h"
#include "RenderTarget.h"
#include "Component.h"
#include "CCamera.h"

class UiViewport : public UiContainer
{
public:
    typedef void(*GizmoCallback)(void *);

    struct GizmoCallbacks
    {
        GizmoCallback on_click = nullptr;
        GizmoCallback off_click = nullptr;
        GizmoCallback on_hover = nullptr;
        GizmoCallback off_hover = nullptr;
        void *data;
    };

    RenderTarget *main_target;

#ifdef __EDITOR__
    RenderTarget *gizmos_target;
    RenderTarget *gizmos_mouse_test_target;
#endif

    CCamera *camera;

    void draw_gizmo(Mesh *mesh, Material *material, Transform *transform, GizmoCallbacks callbacks);

    static List<UiViewport *> viewports;
    static std::unordered_map<unsigned int, Material *> gizmo_mouse_test_materials;

    static void init();
    static UiViewport *create(UiContainer *parent);
    static void handle_update(UiContainer *, void *);
    static void handle_destroy(UiContainer *container, void *);
    static void clear_all();
    static void handle_mouse_move(MouseEvent event, void *);
    static void handle_click(MouseEvent event, void *);
    static void handle_off_click(MouseEvent event, void *);

private:
    void *current_gizmo_off_click_data;
    void *current_gizmo_off_hover_data;
    GizmoCallback current_gizmo_off_hover;
    GizmoCallback current_gizmo_off_click;

    std::vector<UiContainer *> layers;
    std::vector<GizmoCallbacks> gizmo_callbacks;

    static unsigned int gizmo_mouse_test_fragment;

    int get_gizmo_callbacks_id(MouseEvent event);
};

#endif //UIVIEWPORT_H
