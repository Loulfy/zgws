#include "zcl_id.h"


static struct dict_t haDevId[] = {
	// Generic 
	{0x0000, "onOffSwitch"},
	{0x0001, "levelControlSwitch"},
	{0x0002, "onOffOutput"},
	{0x0003, "levelControllableOutput"},
	{0x0004, "sceneSelector"},
	{0x0005, "configurationTool"},
	{0x0006, "remoteControl"},
	{0x0007, "combinedInterface"},
	{0x0008, "rangeExtender"},
	{0x0009, "mainPowerOutlet"},
	{0x000A, "doorLock"},
	{0x000B, "doorLockController"},
	{0x000C, "simpleSensor"},
	{0x000D, "consumptionAwarenessDevice"},
	{0x0050, "homeGateway"},
	{0x0051, "smartPlug"},
	{0x0052, "whiteGoods"},
	{0x0053, "meterInterface"},
	// Lighning
	{0x00FF, "testDevice"},
	{0x0100, "onOffLight"},
	{0x0101, "dimmableLight"},
	{0x0102, "coloredDimmableLight"},
	{0x0103, "onOffLightSwitch"},
	{0x0104, "dimmerSwitch"},
	{0x0105, "colorDimmerSwitch"},
	{0x0106, "lightSensor"},
	{0x0107, "occupancySensor"},
	// ZLO device type
	{0x0108, "onOffBallast"},
	{0x0109, "dimmableBallast"},
	{0x010A, "onOffPlugInUnit"},
	{0x010B, "dimmablePlugInUnit"},
	{0x010C, "coloredTemperatureLight"},
	{0x010D, "extendedColoredLight"},
	{0x010E, "lightLevelSensor"},
	// Closure
	{0x0200, "shade"},
	{0x0201, "shadeController"},
	{0x0202, "windowCoveringDevice"},
	{0x0203, "windowCoveringController"},
	// HVAC
	{0x0300, "heatingCoolingUnit"},
	{0x0301, "thermostat"},
	{0x0302, "temperatureSensor"},
	{0x0303, "pump"},
	{0x0304, "pumpController"},
	{0x0305, "pressureSensor"},
	{0x0306, "flowSensor"},
	{0x0307, "miniSplitAc"},
	// Intruder Alarm Systems
	{0x0400, "iasControlIndicatingEquipment"},
	{0x0401, "iasAncillaryControlEquipment"},
	{0x0402, "iasZone"},
	{0x0403, "iasWarningDevice"}
};

static struct dict_t clusterId[] = {
	{0x0000, "genBasic"},
	{0x0001, "genPowerCfg"},
	{0x0002, "genDeviceTempCfg"},
	{0x0003, "genIdentify"},
	{0x0004, "genGroups"},
	{0x0005, "genScenes"},
	{0x0006, "genOnOff"},
	{0x0007, "genOnOffSwitchCfg"},
	{0x0008, "genLevelCtrl"},
	{0x0009, "genAlarms"},
	{0x000A, "genTime"},
	{0x000B, "genRssiLocation"},
	{0x000C, "genAnalogInput"},
	{0x000D, "genAnalogOutput"},
	{0x000E, "genAnalogValue"},
	{0x000F, "genBinaryInput"},
	{0x0010, "genBinaryOutput"},
	{0x0011, "genBinaryValue"},
	{0x0012, "genMultistateInput"},
	{0x0013, "genMultistateOutput"},
	{0x0014, "genMultistateValue"},
	{0x0015, "genCommissioning"},
	{0x0016, "genPartition"},
	{0x0019, "genOta"},
	{0x001A, "genPowerProfile"},
	{0x001B, "genApplianceCtrl"},
	{0x0B05, "haDiagnostic"},
    {0x1000, "lightLink"}
};

char* zcl_device_str(uint16_t devId)
{
	for(int i=0; i < ZCL_DEVICE_SIZE; ++i)
	{
		if(haDevId[i].id == devId) return haDevId[i].name;
	}
	return "undefined";
}

char* zcl_cluster_str(uint16_t id)
{	
	for(int i=0; i < ZCL_CLUSTER_SIZE; ++i)
	{
		if(clusterId[i].id == id) return clusterId[i].name;
	}
	return "manuSpecificCluster";
}
