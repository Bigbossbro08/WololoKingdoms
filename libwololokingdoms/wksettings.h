#pragma once
#include <fs.h>
#include <map>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

class WKSettings {
public:
  typedef std::tuple<std::string, std::string, std::string, int, std::string>
      DataMod;

  WKSettings() = default;

  bool useVoobly = true;
  bool useExe = false;
  bool useBoth = false;
  bool copyMaps = true;
  bool copyCustomMaps = false;
  bool restrictedCivMods = true;
  bool fixFlags = false;
  bool replaceTooltips = true;
  // Only used for information
  bool useGrid = true;
  bool useShortWalls = false;
  bool useMonks = false;

  std::string language = "en";
  int patch = -1;
  int hotkeyChoice = 0;
  int dlcLevel = 0;
  fs::path resourceDir;
  fs::path hdPath;
  fs::path outPath;
  fs::path vooblyDir;
  fs::path upDir;
  fs::path nfzVooblyOutPath;
  fs::path nfzUpOutPath;
  std::map<int, DataMod> dataModList;
  std::string modName;
  std::string baseModName;

  std::vector<fs::path> drsModDirectories;

  inline void addDrsResources(const fs::path& directory) {
    drsModDirectories.push_back(directory);
  }
};
