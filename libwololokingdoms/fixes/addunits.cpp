#include "addunits.h"
#include "wololo/datPatch.h"

namespace wololo {

// Adds a Roaming Wolf unit to `df`.
// @param df the dat file to modify
void addRoamingWolf(genie::DatFile * df) {
  // Civilization ID of the gaia civilization.
  const size_t civIdGaia = 0;

  // Unit ID of the wolf.
  const size_t unitIdWolf = 126;

  // Language file entry for the roaming wolf.
  const size_t langFileNameRoamingWolf = 5055;

  // ID for graphics for the roaming wolf.
  const size_t graphicsIdWolfxWn = 1392;

  // ID for the new unit
  const size_t unitIdRoamingWolf = 1310;

  auto unit = df->Civs[civIdGaia].Units[unitIdWolf];
  unit.LanguageDLLName = langFileNameRoamingWolf;
  unit.LanguageDLLCreation = langFileNameRoamingWolf + 1000;
  unit.LanguageDLLHelp = langFileNameRoamingWolf + 100000;

  df->UnitHeaders[unitIdRoamingWolf] = df->UnitHeaders[unitIdWolf];
  auto taskList = &df->UnitHeaders[unitIdRoamingWolf].TaskList;
  auto taskFly = genie::Task();
  taskFly.IsDefault = true;
  taskFly.ActionType = genie::Task::ActionTypes::Fly;
  taskFly.MovingGraphicID = graphicsIdWolfxWn;
  taskFly.ProceedingGraphicID = graphicsIdWolfxWn;
  taskFly.WorkingGraphicID = graphicsIdWolfxWn;

  // The new task must be at position 0 in the task list.
  auto taskId = 0;
  unit.Action.DefaultTaskID = taskId;
  taskList->insert(taskList->begin(), taskFly);
  // Sets the task IDs equal to their position in the task list.
  for (int i = 0; i != taskList->size(); ++i) {
    auto task = &(*taskList)[i];
    task->ID = i;
  }

  //id fix
  unit.ID = unitIdRoamingWolf;
  unit.CopyID = unitIdRoamingWolf;
  unit.BaseID = unitIdRoamingWolf;

  for (auto& civ : df->Civs) {
    civ.Units[unitIdRoamingWolf] = unit;
  }
}

// Adds a Transport Sharkatzor unit to `df`.
// @param df the dat file to modify
void addTransportSharkatzor(genie::DatFile * df) {
  // Civilization ID of the gaia civilization.
  const size_t civIdGaia = 0;

  // Unit ID of the Sharkatzor;
  const size_t unitIdSharkatzor = 1222;

  // Resource capacity for the custom Transport Sharkatzor unit.
  const size_t sharkatzorTransportResourceCapacity = 20;

  // Garrison capacity for the custom Transport Sharkatzor unit.
  const size_t sharkatzorTransportGarrisonCapacity = 10;

  // ID for the new unit
  const size_t unitIdSharkatzorTransport = 1311;

  auto unit = df->Civs[civIdGaia].Units[unitIdSharkatzor];
  unit.Class = genie::Unit::UnitClass::TransportBoat;
  unit.ResourceCapacity = sharkatzorTransportResourceCapacity;
  unit.GarrisonCapacity = sharkatzorTransportGarrisonCapacity;
  unit.InterfaceKind = genie::Unit::InterfaceKinds::TransportShipInterface;
  unit.Trait = genie::Unit::Traits::GarrisonableTrait;
  
  unit.LanguageDLLName = 5655;
  unit.LanguageDLLCreation = 6655;
  unit.LanguageDLLHelp = 105655;

  df->UnitHeaders[unitIdSharkatzorTransport] = df->UnitHeaders[unitIdSharkatzor];
  auto taskList = &df->UnitHeaders[unitIdSharkatzorTransport].TaskList;

  // Removes the tasks at indices 0 and 2.
  taskList->erase(taskList->begin());
  taskList->erase(taskList->begin() + 1);

  auto taskUnload = genie::Task();
  taskUnload.ActionType = genie::Task::ActionTypes::UnloadBoat;
  taskList->push_back(taskUnload);
  // Sets the task IDs equal to their position in the task list.
  for (int i = 0; i != taskList->size(); ++i) {
    auto task = &(*taskList)[i];
    task->ID = i;
  }

  // id fix
  unit.ID = unitIdSharkatzorTransport;
  unit.CopyID = unitIdSharkatzorTransport;
  unit.BaseID = unitIdSharkatzorTransport;

  for (auto& civ : df->Civs) {
    civ.Units[unitIdSharkatzorTransport] = unit;
  }
}

// Adds a Unit Spawner unit to `df`.
// @param df the dat file to modify
void addUnitSpawner(genie::DatFile * df) {
  // Civilization ID of the gaia civilization.
  const size_t civIdGaia = 0;

  // Unit ID for the Llama building.
  const size_t unitIdLlamaBuilding = 888;

  // Unit Id for the first unit of the spawners
  const size_t spawnerUnitIdStart = 1312;

  auto unitA = df->Civs[civIdGaia].Units[unitIdLlamaBuilding];
  unitA.DeadUnitID = -1;
  unitA.Building.TechID = -1; // Sets "Initiates Technology".
  unitA.ID = spawnerUnitIdStart;

  auto unitB = unitA;
  unitB.ID = unitA.ID + 1;

  unitB.HideInEditor = 0;
  unitA.Building.StackUnitID = unitB.ID;

  auto unitC = unitA;
  unitC.ID = unitA.ID + 2;
  unitC.Building.HeadUnit = unitA.ID;
  auto annexC = genie::unit::BuildingAnnex();
  annexC.UnitID = unitA.ID + 2;
  annexC.Misplacement = {0.5f, -0.5f};

  auto unitD = unitA;
  unitD.ID = unitA.ID + 3;
  unitD.Building.HeadUnit = unitA.ID;
  auto annexD = genie::unit::BuildingAnnex();
  annexD.UnitID = unitA.ID + 3;
  annexD.Misplacement = {-0.5f, 0.5f};

  auto unitE = unitA;
  unitE.ID = unitA.ID + 4;
  unitE.Building.HeadUnit = unitA.ID;
  auto annexE = genie::unit::BuildingAnnex();
  annexE.UnitID = unitA.ID + 4;
  annexE.Misplacement = {0.5f, 0.5f};

  auto unitF = unitA;
  unitF.ID = unitA.ID + 5;
  unitF.Building.HeadUnit = unitA.ID;
  auto annexF = genie::unit::BuildingAnnex();
  annexF.UnitID = unitA.ID + 5;
  annexF.Misplacement = {-0.5f, -0.5f};

  std::vector<genie::unit::BuildingAnnex> annexes = {annexC, annexD, annexE,
                                                     annexF};
  unitA.Building.Annexes = annexes;
  auto units = {unitA, unitB, unitC, unitD, unitE, unitF};

  int langId = 5639;
  for (auto unit : units) {
    df->UnitHeaders[unit.ID] = df->UnitHeaders[unitIdLlamaBuilding];
    unit.LanguageDLLName = langId;
    unit.LanguageDLLCreation = langId + 1000;
    unit.LanguageDLLHelp = langId + 100000;
    unit.CopyID = unit.ID;
    unit.BaseID = unit.ID;
    ++langId;
    for (auto& civ : df->Civs) {
      civ.Units[unit.ID] = unit;
    }
  }
}

// Copies a number of units in the 900-1000 ID range in `df` to a safer range.
// @param df the dat file to modify
void copy900IdUnits(genie::DatFile* df) {

  // First unit ID in the range to copy
  const size_t originalId = 900;

  // First ID in a safe range
  const size_t newId = 1318;

  // Number of consecutive units to copy
  const size_t unitCountToCopy = 50;

  for (size_t i = 0; i < unitCountToCopy; i++) {
    df->UnitHeaders[newId + i] = df->UnitHeaders[originalId + i];
    for (auto& civ : df->Civs) {
      civ.Units[newId + i] = civ.Units[originalId + i];
      civ.Units[newId + i].ID = newId + i;
      civ.Units[newId + i].CopyID = newId + i;
      civ.Units[newId + i].BaseID = newId + i;
    }
  }

}

// Adds several new units:
// * Roaming Wolf
// * Transport Sharkatzor
// * Unit Spawner
void addUnitsPatch(genie::DatFile* aocDat) {
  addRoamingWolf(aocDat);
  addTransportSharkatzor(aocDat);
  addUnitSpawner(aocDat);
  copy900IdUnits(aocDat);
}

DatPatch addUnits = {&addUnitsPatch, "Adds various additional units."};

} // namespace wololo
