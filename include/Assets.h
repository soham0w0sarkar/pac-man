#pragma once

#include <filesystem>
#include <optional>
#include <string>

inline std::optional<std::filesystem::path>
findAsset(const std::string &relative) {
  namespace fs = std::filesystem;
  const fs::path candidates[] = {
      fs::path(relative),
      fs::path("..") / relative,
      fs::path("../..") / relative,
  };

  for (const auto &path : candidates) {
    if (fs::exists(path)) {
      return fs::weakly_canonical(path);
    }
  }
  return std::nullopt;
}
