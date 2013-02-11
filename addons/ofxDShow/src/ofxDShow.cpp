/*
*  ofxffmv.cpp
*  
*
*  Created on 12/03/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*
*/

#include "ofxDShow.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"


ofxDShow::ofxDShow()
{	
	VI = new videoInput();
}
ofxDShow::~ofxDShow()
{
	deinitializeCamera();
}

void ofxDShow::callSettingsDialog()
{
	VI->showSettingsWindow(guid.Data1);
}

CAMERA_BASE_FEATURE* ofxDShow::getSupportedFeatures(int* featuresCount)
{
	*featuresCount = 15;
	CAMERA_BASE_FEATURE* features = (CAMERA_BASE_FEATURE*)malloc(*featuresCount * sizeof(CAMERA_BASE_FEATURE));
	features[0] = BASE_BRIGHTNESS;
	features[1] = BASE_CONTRAST;
	features[2] = BASE_HUE;
	features[3] = BASE_GAMMA;
	features[4] = BASE_GAIN;
	features[5] = BASE_SATURATION;
	features[6] = BASE_SHARPNESS;
	features[7] = BASE_PAN;
	features[8] = BASE_TILT;
	features[9] = BASE_ROLL;
	features[10] = BASE_ZOOM;
	features[11] = BASE_EXPOSURE;
	features[12] = BASE_IRIS;
	features[13] = BASE_FOCUS;
	features[14] = BASE_FRAMERATE;
	return features;
}

void ofxDShow::setCameraFeature(CAMERA_BASE_FEATURE featureCode,int firstValue,int secondValue,bool isAuto,bool isEnabled)
{
	int deviceID = guid.Data1;
	if (VI==NULL)
		return;
	long filterSettings = -1;
	long deviceSettings = -1;
	switch (featureCode)
	{
		case BASE_BRIGHTNESS:
			filterSettings = VI->propBrightness;
			break;
		case BASE_CONTRAST:
			filterSettings = VI->propContrast;
			break;
		case BASE_HUE:
			filterSettings = VI->propHue;
			break;
		case BASE_GAMMA:
			filterSettings = VI->propGamma;
			break;
		case BASE_GAIN:
			filterSettings = VI->propGain;
			break;
		case BASE_SATURATION:
			filterSettings = VI->propSaturation;
			break;
		case BASE_SHARPNESS:
			filterSettings = VI->propSharpness;
			break;
		case BASE_PAN:
			deviceSettings = VI->propPan;
			break;
		case BASE_TILT:
			deviceSettings = VI->propTilt;
			break;
		case BASE_ROLL:
			deviceSettings = VI->propRoll;
			break;
		case BASE_ZOOM:
			deviceSettings = VI->propZoom;
			break;
		case BASE_EXPOSURE:
			deviceSettings = VI->propExposure;
			break;
		case BASE_IRIS:
			deviceSettings = VI->propIris;
			break;
		case BASE_FOCUS:
			deviceSettings = VI->propFocus;
			break;
		case BASE_FRAMERATE:
			VI->setIdealFramerate(guid.Data1,firstValue);
			break;
	}
	if (filterSettings>=0)
		VI->setVideoSettingFilter(deviceID,filterSettings,firstValue,isAuto ? 1 : 2,false);
	if (deviceSettings>=0)
		VI->setVideoSettingCamera(deviceID,filterSettings,firstValue,isAuto ? 1 : 2,false);
}

void ofxDShow::getCameraFeature(CAMERA_BASE_FEATURE featureCode,int* firstValue,int* secondValue, bool* isAuto, bool* isEnabled,int* minValue,int* maxValue)
{
	int deviceID = guid.Data1;
	if (VI==NULL)
		return;
	long filterSettings = -1;
	long deviceSettings = -1;
	switch (featureCode)
	{
		case BASE_BRIGHTNESS:
			filterSettings = VI->propBrightness;
			break;
		case BASE_CONTRAST:
			filterSettings = VI->propContrast;
			break;
		case BASE_HUE:
			filterSettings = VI->propHue;
			break;
		case BASE_GAMMA:
			filterSettings = VI->propGamma;
			break;
		case BASE_GAIN:
			filterSettings = VI->propGain;
			break;
		case BASE_SATURATION:
			filterSettings = VI->propSaturation;
			break;
		case BASE_SHARPNESS:
			filterSettings = VI->propSharpness;
			break;
		case BASE_PAN:
			deviceSettings = VI->propPan;
			break;
		case BASE_TILT:
			deviceSettings = VI->propTilt;
			break;
		case BASE_ROLL:
			deviceSettings = VI->propRoll;
			break;
		case BASE_ZOOM:
			deviceSettings = VI->propZoom;
			break;
		case BASE_EXPOSURE:
			deviceSettings = VI->propExposure;
			break;
		case BASE_IRIS:
			deviceSettings = VI->propIris;
			break;
		case BASE_FOCUS:
			deviceSettings = VI->propFocus;
			break;
		case BASE_FRAMERATE:
			*firstValue = framerate;
			*minValue = framerate;
			*maxValue = framerate;
			*secondValue = 0;
			*isAuto = false;
			break;
	}
	long delta,flags,defaultValue,MinValue,MaxValue,CurrentValue;
	if (filterSettings>=0)
	{
		VI->getVideoSettingFilter(deviceID,filterSettings,MinValue,MaxValue,delta,CurrentValue,flags,defaultValue);
		*firstValue = CurrentValue;
		*minValue = MinValue;
		*maxValue = MaxValue;
		*secondValue = 0;
		*isAuto = flags & 1;
	}
	if (deviceSettings>=0)
	{
		VI->getVideoSettingCamera(deviceID,deviceSettings,MinValue,MaxValue,delta,CurrentValue,flags,defaultValue);
		*firstValue = CurrentValue;
		*minValue = MinValue;
		*maxValue = MaxValue;
		*secondValue = 0;
		*isAuto = flags & 1;
	}
}

int ofxDShow::getCameraBaseCount()
{
	if (VI == NULL)
		VI = new videoInput();
	return VI->getDeviceCount();
}

void ofxDShow::getNewFrame(unsigned char* newFrame)
{
	VI->getPixels(guid.Data1, newFrame, false, false);
}

void ofxDShow::setCameraType()
{
	cameraType = DIRECTSHOW;
	cameraTypeName = "DIRECTSHOW";
}

void ofxDShow::cameraInitializationLogic()
{
	if (VI == NULL)
		VI = new videoInput();
	VI->setVerbose(false);
	VI->listDevices(false);
	/*
	int count = VI->getDeviceCount(true);
	int selectedID = -1;
	for (int i=0;i<count;i++)
	{
		char* cameraGuidString = VI->getDevicePath(i);
		long cameraGuidNum = 0;
		int strLen = strlen(cameraGuidString);
		for (int j=0;j<strLen;j++)
		{
			cameraGuidNum*=0xFF;
			cameraGuidNum+=strLen[j];
			cameraGuidNum%=0xFFFFFFFF;
		}
		if (cameraGuidNum == guid.Data1)
		{
			selectedID = i;
			break;
		}
	}

	if (selectedID == -1)
		return;
	*/
	if (!VI->setupDevice(guid.Data1,width,height))
		VI->setupDevice(guid.Data1,320,240);
	VI->setIdealFramerate(guid.Data1,framerate);
	width = VI->getWidth(guid.Data1);
	height = VI->getHeight(guid.Data1);
	depth = VI->getSize(guid.Data1)/(width*height);
	if (cameraBaseSettings->propertyType.size() == 0)
	{
		long delta,flags,defaultValue,MinValue,MaxValue,CurrentValue;
		VI->getVideoSettingFilter(guid.Data1,VI->propBrightness,MinValue,MaxValue,delta,CurrentValue,flags,defaultValue);
		if (flags && (CurrentValue>-0xFF))
		{
			cameraBaseSettings->isPropertyAuto.push_back(flags&1);
			cameraBaseSettings->isPropertyOn.push_back(true);
			cameraBaseSettings->propertyFirstValue.push_back(defaultValue);
			cameraBaseSettings->propertySecondValue.push_back(0);
			cameraBaseSettings->propertyType.push_back(BASE_BRIGHTNESS);
		}
		VI->getVideoSettingFilter(guid.Data1,VI->propContrast,MinValue,MaxValue,delta,CurrentValue,flags,defaultValue);
		if (flags && (CurrentValue>-0xFF))
		{
			cameraBaseSettings->isPropertyAuto.push_back(flags&1);
			cameraBaseSettings->isPropertyOn.push_back(true);
			cameraBaseSettings->propertyFirstValue.push_back(defaultValue);
			cameraBaseSettings->propertySecondValue.push_back(0);
			cameraBaseSettings->propertyType.push_back(BASE_CONTRAST);
		}
		VI->getVideoSettingFilter(guid.Data1,VI->propHue,MinValue,MaxValue,delta,CurrentValue,flags,defaultValue);
		if (flags && (CurrentValue>-0xFF))
		{
			cameraBaseSettings->isPropertyAuto.push_back(flags&1);
			cameraBaseSettings->isPropertyOn.push_back(true);
			cameraBaseSettings->propertyFirstValue.push_back(defaultValue);
			cameraBaseSettings->propertySecondValue.push_back(0);
			cameraBaseSettings->propertyType.push_back(BASE_HUE);
		}
		VI->getVideoSettingFilter(guid.Data1,VI->propGamma,MinValue,MaxValue,delta,CurrentValue,flags,defaultValue);
		if (flags && (CurrentValue>-0xFF))
		{
			cameraBaseSettings->isPropertyAuto.push_back(flags&1);
			cameraBaseSettings->isPropertyOn.push_back(true);
			cameraBaseSettings->propertyFirstValue.push_back(defaultValue);
			cameraBaseSettings->propertySecondValue.push_back(0);
			cameraBaseSettings->propertyType.push_back(BASE_GAMMA);
		}
		VI->getVideoSettingFilter(guid.Data1,VI->propGain,MinValue,MaxValue,delta,CurrentValue,flags,defaultValue);
		if (flags && (CurrentValue>-0xFF))
		{
			cameraBaseSettings->isPropertyAuto.push_back(flags&1);
			cameraBaseSettings->isPropertyOn.push_back(true);
			cameraBaseSettings->propertyFirstValue.push_back(defaultValue);
			cameraBaseSettings->propertySecondValue.push_back(0);
			cameraBaseSettings->propertyType.push_back(BASE_GAIN);
		}
		VI->getVideoSettingFilter(guid.Data1,VI->propSaturation,MinValue,MaxValue,delta,CurrentValue,flags,defaultValue);
		if (flags && (CurrentValue>-0xFF))
		{
			cameraBaseSettings->isPropertyAuto.push_back(flags&1);
			cameraBaseSettings->isPropertyOn.push_back(true);
			cameraBaseSettings->propertyFirstValue.push_back(defaultValue);
			cameraBaseSettings->propertySecondValue.push_back(0);
			cameraBaseSettings->propertyType.push_back(BASE_SATURATION);
		}
		VI->getVideoSettingFilter(guid.Data1,VI->propSharpness,MinValue,MaxValue,delta,CurrentValue,flags,defaultValue);
		if (flags && (CurrentValue>-0xFF))
		{
			cameraBaseSettings->isPropertyAuto.push_back(flags&1);
			cameraBaseSettings->isPropertyOn.push_back(true);
			cameraBaseSettings->propertyFirstValue.push_back(defaultValue);
			cameraBaseSettings->propertySecondValue.push_back(0);
			cameraBaseSettings->propertyType.push_back(BASE_SHARPNESS);
		}
		VI->getVideoSettingCamera(guid.Data1,VI->propPan,MinValue,MaxValue,delta,CurrentValue,flags,defaultValue);
		if (flags && (CurrentValue>-0xFF))
		{
			cameraBaseSettings->isPropertyAuto.push_back(flags&1);
			cameraBaseSettings->isPropertyOn.push_back(true);
			cameraBaseSettings->propertyFirstValue.push_back(defaultValue);
			cameraBaseSettings->propertySecondValue.push_back(0);
			cameraBaseSettings->propertyType.push_back(BASE_PAN);
		}
		VI->getVideoSettingCamera(guid.Data1,VI->propTilt,MinValue,MaxValue,delta,CurrentValue,flags,defaultValue);
		if (flags && (CurrentValue>-0xFF))
		{
			cameraBaseSettings->isPropertyAuto.push_back(flags&1);
			cameraBaseSettings->isPropertyOn.push_back(true);
			cameraBaseSettings->propertyFirstValue.push_back(defaultValue);
			cameraBaseSettings->propertySecondValue.push_back(0);
			cameraBaseSettings->propertyType.push_back(BASE_TILT);
		}
		VI->getVideoSettingCamera(guid.Data1,VI->propRoll,MinValue,MaxValue,delta,CurrentValue,flags,defaultValue);
		if (flags && (CurrentValue>-0xFF))
		{
			cameraBaseSettings->isPropertyAuto.push_back(flags&1);
			cameraBaseSettings->isPropertyOn.push_back(true);
			cameraBaseSettings->propertyFirstValue.push_back(defaultValue);
			cameraBaseSettings->propertySecondValue.push_back(0);
			cameraBaseSettings->propertyType.push_back(BASE_ROLL);
		}
		VI->getVideoSettingCamera(guid.Data1,VI->propZoom,MinValue,MaxValue,delta,CurrentValue,flags,defaultValue);
		if (flags && (CurrentValue>-0xFF))
		{
			cameraBaseSettings->isPropertyAuto.push_back(flags&1);
			cameraBaseSettings->isPropertyOn.push_back(true);
			cameraBaseSettings->propertyFirstValue.push_back(defaultValue);
			cameraBaseSettings->propertySecondValue.push_back(0);
			cameraBaseSettings->propertyType.push_back(BASE_ZOOM);
		}
		VI->getVideoSettingCamera(guid.Data1,VI->propExposure,MinValue,MaxValue,delta,CurrentValue,flags,defaultValue);
		if (flags && (CurrentValue>-0xFF))
		{
			cameraBaseSettings->isPropertyAuto.push_back(flags&1);
			cameraBaseSettings->isPropertyOn.push_back(true);
			cameraBaseSettings->propertyFirstValue.push_back(defaultValue);
			cameraBaseSettings->propertySecondValue.push_back(0);
			cameraBaseSettings->propertyType.push_back(BASE_EXPOSURE);
		}
		VI->getVideoSettingCamera(guid.Data1,VI->propIris,MinValue,MaxValue,delta,CurrentValue,flags,defaultValue);
		if (flags && (CurrentValue>-0xFF))
		{
			cameraBaseSettings->isPropertyAuto.push_back(flags&1);
			cameraBaseSettings->isPropertyOn.push_back(true);
			cameraBaseSettings->propertyFirstValue.push_back(defaultValue);
			cameraBaseSettings->propertySecondValue.push_back(0);
			cameraBaseSettings->propertyType.push_back(BASE_IRIS);
		}
		VI->getVideoSettingCamera(guid.Data1,VI->propFocus,MinValue,MaxValue,delta,CurrentValue,flags,defaultValue);
		if (flags && (CurrentValue>-0xFF))
		{
			cameraBaseSettings->isPropertyAuto.push_back(flags&1);
			cameraBaseSettings->isPropertyOn.push_back(true);
			cameraBaseSettings->propertyFirstValue.push_back(defaultValue);
			cameraBaseSettings->propertySecondValue.push_back(0);
			cameraBaseSettings->propertyType.push_back(BASE_FOCUS);
		}
	}
	for (int i=0;i<cameraBaseSettings->propertyType.size();i++)
		setCameraFeature(cameraBaseSettings->propertyType[i],cameraBaseSettings->propertyFirstValue[i],cameraBaseSettings->propertySecondValue[i],cameraBaseSettings->isPropertyAuto[i],cameraBaseSettings->isPropertyOn[i]);
}

void ofxDShow::cameraDeinitializationLogic()
{
	if (VI!=NULL)
	{
		VI->stopDevice(guid.Data1);
		delete VI; VI = NULL;
	}
}

GUID* ofxDShow::getBaseCameraGuids(int* camCount)
{
	*camCount = getCameraBaseCount();
	GUID* guids = (GUID*)malloc(*camCount * sizeof(GUID));
	for (int i=0;i<*camCount;i++)
	{
		GUID tempGuid;
		tempGuid.Data1 = i;
		tempGuid.Data2 = tempGuid.Data3 = 0;
		memset(tempGuid.Data4,0,8*sizeof(unsigned char));
		guids[i] = tempGuid;
	}
	return guids;
}



