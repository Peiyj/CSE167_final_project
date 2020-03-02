//
//  DirectionalLight.cpp
//  HW0
//
//  Created by Patrick on 2/12/20.
//  Copyright © 2020 Patrick. All rights reserved.
//
#include "DirectionalLight.h"
#include <iostream>
#include <stdio.h>


using namespace std;

DirectionalLight::DirectionalLight(glm::vec3 color, glm::vec3 direction): color(color), direction(direction){
//    PointCloud * sphere;
}

DirectionalLight::~DirectionalLight(){}

void DirectionalLight::update()
{
    spin(0.005);
}


void DirectionalLight::spin(float deg){
    direction = glm::rotateY(direction, deg);
}
