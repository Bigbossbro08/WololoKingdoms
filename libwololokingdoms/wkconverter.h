#pragma once
#include "genie/dat/DatFile.h"
#include "genie/lang/LangFile.h"
#include "platform.h"
#include "wksettings.h"
#include <fs.h>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <vector>

/**
 * WK Conversion event listener, used to update the UI and to implement
 * platform/app-specific things like UserPatch installation.
 */
class WKConvertListener {
  int m_cachedProgress = 0;

public:
  virtual ~WKConvertListener() {}

  /**
   * Called when conversion has finished.
   */
  virtual void finished() {}

  /**
   * Log a message.
   */
  virtual void log([[maybe_unused]] std::string logMessage) {}

  /**
   * Set the information line, usually describing what the converter is
   * currently working on.
   */
  virtual void setInfo([[maybe_unused]] std::string info) {}

  /**
   * Report an error.
   */
  virtual void error([[maybe_unused]] std::exception const& err, bool showDialog = false) {}

  /**
   * Report an error (message only).
   */
  virtual void error([[maybe_unused]] std::string message, bool showDialog = false) {
    error(std::runtime_error(message), showDialog);
  }

  /**
   * Create a dialog (this can usually be a noop if you're not building a UI)
   */
  virtual void createDialog([[maybe_unused]] std::string info) {}

  /**
   * Create a dialog (this can usually be a noop if you're not building a UI)
   */
  virtual void createDialog([[maybe_unused]] std::string info,
                            [[maybe_unused]] std::string title) {}

  /**
   * Create a dialog (this can usually be a noop if you're not building a UI)
   */
  virtual void createDialog([[maybe_unused]] std::string info,
                            [[maybe_unused]] std::string toReplace,
                            [[maybe_unused]] std::string replaceWith) {}

  /**
   * Set the current progress (0-100).
   */
  virtual void setProgress([[maybe_unused]] int i) {}

  /**
   * Increase the current progress.
   */
  virtual void increaseProgress([[maybe_unused]] int i) {
    m_cachedProgress += i;
    setProgress(m_cachedProgress);
  }

  /**
   * Install UserPatch.
   * userPatchExe is the path to the UserPatch SetupAoC.exe file.
   * cliFlags are strings that should be passed to the installer as command-line
   * flags
   */
  virtual void
  installUserPatch([[maybe_unused]] fs::path userPatchExe,
                   [[maybe_unused]] std::vector<std::string> cliFlags) {}
};

class WKConverter {
  WKSettings settings;
  WKConvertListener* listener;

public:
  WKConverter(WKSettings& settings, WKConvertListener* const listener)
      : settings(settings), listener(listener) {
    if (settings.resourceDir != fs::path()) {
      resourceDir = settings.resourceDir;
    }
  }

  int run();
  int retryInstall();

private:
  //int standardMapsId = 54240;
  //int specialMapsId = 55011;
  int rmsStringId = 11070;
  int rwmStringId = 10930;
  int smStringId = 10960;
  std::set<char> civLetters;
  std::set<int> aocSlpFiles;
  std::map<int, fs::path> slpFiles;
  std::map<int, fs::path> wavFiles;
  std::map<int, fs::path> mapFiles;
  std::map<std::string, fs::path> newTerrainFiles;
  std::map<int, std::string> langReplacement;
  bool secondAttempt = false;

  fs::path nfzUpOutPath;
  fs::path nfzVooblyOutPath;
  fs::path vooblyDir;
  fs::path upDir;
  fs::path installDir;
  std::string baseModName = "WololoKingdoms";
  fs::path resourceDir = "resources";

  enum TerrainType {
    None,
    WaterTerrain,
    FixedTerrain,
    LandTerrain,
    ForestTerrain,
    UnbuildableTerrain
  };

  struct MapConvertData {
    int slp_number;
    std::vector<std::string> const_names;
    std::string replaced_name;
    int old_terrain_id;
    int new_terrain_id;
    TerrainType terrain_type;
  };

  void processExpansionMaps(const fs::path& inputDir,
                            const fs::path& outputDir);
  void processCustomHDMaps(const fs::path& inputDir, const fs::path& outputDir);
  void convertExpansionMap(
      const fs::path& it, const fs::path& outputDir,
      std::map<int, fs::path>& terrainOverrides);
  bool usesMultipleWaterTerrains(const std::string& map,
                                 std::map<int, bool>& terrainsUsed);
  bool isTerrainUsed(int terrain, std::map<int, bool>& terrainsUsed,
                     const std::string& map,
                     const std::map<int, std::regex>& patterns);
  void upgradeTrees(int usedTerrain, int oldTerrain, std::string& map);
  void createZRmap(std::map<int, fs::path>& terrainOverrides,
                   fs::path outputDir, std::string mapName);
  void terrainSwap(genie::DatFile* hdDat, genie::DatFile* aocDat, int tNew,
                   int tOld, int slpID);
  void indexDrsFiles(fs::path const& src, bool expansionFiles = true);
  void copyHistoryFiles(fs::path inputDir, fs::path outputDir);
  std::pair<int, std::string> parseHDTextLine(std::string line);
  void convertLanguageFile(std::ifstream& in, std::ofstream& iniOut,
                           std::map<int, std::string>& langReplacement);
  void createLanguageFile(fs::path languageIniPath, fs::path patchFolder);
  void loadGameStrings(std::map<int, std::string>& langReplacement, fs::path file);
  void loadModdedStrings(fs::path moddedStringsFile,
                         std::map<int, std::string>& langReplacement);
  void makeRandomMapScriptsDrs(std::ofstream& out, const fs::path& drsDir);
  void makeDrs(std::ofstream& out);
  void editDrs(std::ifstream* in, std::ofstream* out);
  void copyCivIntroSounds(const fs::path& inputDir, const fs::path& outputDir);
  void copyWallFiles(const fs::path& inputDir);
  void createMusicPlaylist(const fs::path& inputDir, const fs::path& outputDir);
  void transferHdDatElements(genie::DatFile* hdDat, genie::DatFile* aocDat);
  void adjustArchitectureFlags(genie::DatFile* aocDat, fs::path flagFilename);
  void patchArchitectures(genie::DatFile* aocDat);
  bool checkGraphics(genie::DatFile* aocDat, short graphicID,
                     std::vector<int> checkedGraphics);
  void replaceGraphic(genie::DatFile* aocDat, short* graphicID, short compareID,
                      short c, std::map<short, short>& replacedGraphics,
                      bool civGroups = false);
  short duplicateGraphic(genie::DatFile* aocDat,
                         std::map<short, short>& replacedGraphics,
                         std::vector<short> duplicatedGraphics, short graphicID,
                         short compareID, short offset, bool civGroups = false);
  bool identifyHotkeyFile(const fs::path& directory, fs::path& maxHki,
                          fs::path& lastEditedHki);
  void copyHotkeyFile(const fs::path& maxHki, const fs::path& lastEditedHki,
                      fs::path dst);
  void removeWkHotkeys();
  void hotkeySetup();
  void refreshSymlink(const fs::path& oldDir, const fs::path& newDir,
                      const LinkType type, bool copyOldContents = false);
  void symlinkSetup(const fs::path& oldDir, const fs::path& newDir,
                    bool dataMod = false);
  void setupFolders(fs::path xmlOutPathUP);
};
