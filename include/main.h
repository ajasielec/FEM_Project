#pragma once

// all includes
#include "includes.h"
#include "grid.h"
#include "ElemUniv.h"
#include "Jakobian.h"
#include "GlobalSystemOfEquation.h"

// functions
Grid readFromFile(const std::string& path);
void calculateH(Grid& grid);

