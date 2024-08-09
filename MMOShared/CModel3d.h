//
// Created by Keegan Beaulieu on 2024-07-16.
//

#ifndef CMODEL3D_H
#define CMODEL3D_H

#include "Mesh.h"
#include "Component.h"
#include "Material.h"

class CModel3d : Component
{
public:
    Mesh *mesh;
	Material *material;

	static void register_type();
	static void tick(Descriptor<CModel3d> *descriptor);
};

#endif //CMODEL3D_H
