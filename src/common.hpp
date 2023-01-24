#pragma once

#ifndef __COMMON_HPP
#define __COMMON_HPP

#include <iostream>

using namespace std;

const float DEG_TO_RAD = 3.141592653589793f / 180.0f;

void FAIL(const char* label);

struct nocopy {
  nocopy() = default;
  nocopy(const nocopy&) = delete;
  nocopy& operator = (const nocopy&) = delete;
};

#endif