#ifndef GLOBAL
#define GLOBAL

#include "Object.hpp"
#include "Light.hpp"
#include "SpotLight.hpp"
#include <bits/stdc++.h>
using namespace std;




extern vector <Light*> lights;
extern vector <SpotLight*> spotlights;
extern vector <Object*> objects;
extern int recursionLevel;









#endif