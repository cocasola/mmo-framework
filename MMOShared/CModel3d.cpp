//
// Created by Keegan Beaulieu on 2024-07-16.
//

#include "Component.h"
#include "CModel3d.h"

static Component::Descriptor<CCamera> *camera_descriptor;

void CModel3d::register_type()
{
    Component::register_type<CModel3d>({
        .tick_callbacks = {
            { .callback = tick, .order = RENDERING, .execute_in_editor = true}
        },
        .name = "Model3d"
    });

    camera_descriptor = get_descriptor<CCamera>();
}

void CModel3d::tick(Descriptor<CModel3d> *descriptor)
{
    for (auto context_instance_pair : camera_descriptor->context_instances) {
        Context *context = context_instance_pair.first;
        List<CCamera *> &cameras = context_instance_pair.second;
        List<CModel3d *> &models = descriptor->context_instances[context];

        for (auto camera : cameras) {
#ifdef __EDITOR__
            if (!camera->target)
                continue;
#endif

            camera->target->bind();
            Mat4x4f view = camera->build_vp_matrix();

            for (auto &model : models) {
#ifdef __EDITOR__
                if (!model->material || !model->mesh)
                    continue;
#endif

                model->material->set("mvp", view);
                model->material->bind();
                model->mesh->draw();
            }
        }
    }
}
