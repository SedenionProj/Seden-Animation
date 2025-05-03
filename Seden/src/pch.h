#include "src/logger.h"

// stl
#include <memory>
#include <string>
#include <type_traits>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>
#include <mutex>
#include <iostream>
#include <fstream>
#include <sstream>

#include <entt/entt.hpp>

// glm
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>