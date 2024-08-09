//
// Created by Keegan Beaulieu on 2024-07-17.
//

#include "Components.h"

#include "NetworkTypeMapping.h"
#include "CPlayer.h"
#include "CCamera.h"
#include "CModel3d.h"

void Components::register_all()
{
    CPlayer::register_type();

#if defined(__CLIENT__) || defined(__EDITOR__)
    CCamera::register_type();
    CModel3d::register_type();
#endif
}
