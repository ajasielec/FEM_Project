#pragma once

// all includes
#include "includes.h"
#include "grid.h"
#include "ElemUniv.h"
#include "Jakobian.h"

// functions
Grid readFromFile(const std::string& path);

std::vector<Jakobian> calculateJakobiansOfElement(int element_id, Grid grid, ElemUniv e);

Jakobian calculateJakobian(int element_id, Grid grid, ElemUniv e);

