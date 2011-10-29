/*
*  ofxffmv.cpp
*  
*
*  Created on 12/03/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*
*/

#include "ofxffmv.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#pragma warning(disable : 4018)	// signed/unsigned mismatch

ofxffmv::ofxffmv()
{	
}

ofxffmv::~ofxffmv()
{
	deinitializeCamera();
}

void ofxffmv::callSettingsDialog()
{
	pgrcamguiInitializeSettingsDialog(cameraContext,guiContext);
}

CAMERA_BASE_FEATURE* ofxffmv::getSupportedFeatures(int* featuresCount)
{
	*featuresCount = 7;
	CAMERA_BASE_FEATURE* features = (CAMERA_BASE_FEATURE*)malloc(*featuresCount * sizeof(CAMERA_BASE_FEATURE));
	features[0] = BASE_BRIGHTNESS;
	features[1] = BASE_EXPOSURE;
	features[2] = BASE_SATURATION;
	features[3] = BASE_GAMMA;
	features[4] = BASE_SHUTTER;
	features[5] = BASE_GAIN;
	features[6] = BASE_FRAMERATE;
	return features;
}

void ofxffmv::setCameraFeature(CAMERA_BASE_FEATURE featureCode,int firstValue,int secondValue,bool isAuto,bool isEnabled)
{
	FlyCaptureProperty deviceProperty = (FlyCaptureProperty)0xFFFFFFFF;
	switch (featureCode)
	{
	case BASE_BRIGHTNESS:
		deviceProperty = FLYCAPTURE_BRIGHTNESS;
		break;
	case BASE_EXPOSURE:
		deviceProperty = FLYCAPTURE_AUTO_EXPOSURE;
		break;
	case BASE_SATURATION:
		deviceProperty = FLYCAPTURE_SATURATION;
		break;
	case BASE_GAMMA:
		deviceProperty = FLYCAPTURE_GAMMA;
		break;
	case BASE_SHUTTER:
		deviceProperty = FLYCAPTURE_SHUTTER;
		break;
	case BASE_GAIN:
		deviceProperty = FLYCAPTURE_GAIN;
		break;
	case BASE_FRAMERATE:
		deviceProperty = FLYCAPTURE_FRAME_RATE;
		framerate = firstValue;
		break;
	}
	if (deviceProperty!=0xFFFFFFFF)
		flycaptureSetCameraPropertyEx(cameraContext,deviceProperty,true,isEnabled,isAuto,firstValue,secondValue);
}

void ofxffmv::getCameraFeature(CAMERA_BASE_FEATURE featureCode,int* firstValue,int* secondValue, bool* isAuto, bool* isEnabled,int* minValue,int* maxValue)
{
	*firstValue = 0;
	*secondValue = 0;
	*isAuto = false;
	*isEnabled = false;
	*minValue = 0;
	*maxValue = 0;
	FlyCaptureProperty deviceProperty = (FlyCaptureProperty)0xFFFFFFFF;
	switch (featureCode)
	{
	case BASE_BRIGHTNESS:
		deviceProperty = FLYCAPTURE_BRIGHTNESS;
		break;
	case BASE_EXPOSURE:
		deviceProperty = FLYCAPTURE_AUTO_EXPOSURE;
		break;
	case BASE_SATURATION:
		deviceProperty = FLYCAPTURE_SATURATION;
		break;
	case BASE_GAMMA:
		deviceProperty = FLYCAPTURE_GAMMA;
		break;
	case BASE_SHUTTER:
		deviceProperty = FLYCAPTURE_SHUTTER;
		break;
	case BASE_GAIN:
		deviceProperty = FLYCAPTURE_GAIN;
		break;
	case BASE_FRAMERATE:
		deviceProperty = FLYCAPTURE_FRAME_RATE;
		break;
	}
	bool bpPresent,bpOnePush,pbReadOut,pbOnOff,pbAuto,pbManual;
	if (deviceProperty!=0xFFFFFFFF)
	{
		*isEnabled = (flycaptureGetCameraProperty(cameraContext,deviceProperty,(long*)firstValue,(long*)secondValue,isAuto) == FLYCAPTURE_OK);
		flycaptureGetCameraPropertyRangeEx(cameraContext,deviceProperty,&bpPresent,&bpOnePush,&pbReadOut,&pbOnOff,&pbAuto,&pbManual,minValue,maxValue);
	}
	else
		*isEnabled = false;
}

int ofxffmv::getCameraBaseCount()
{
   unsigned int tcamNum = _MAX_CAMERAS_;
   FlyCaptureInfoEx* tArInfo = new FlyCaptureInfoEx[_MAX_CAMERAS_];
   flycaptureBusEnumerateCamerasEx( tArInfo, &tcamNum );
   delete tArInfo;
   return tcamNum;
}

void ofxffmv::getNewFrame(unsigned char* newFrame)
{
	flycaptureGrabImage2(cameraContext,&fcImage);
	memcpy((void*)newFrame,fcImage.pData,width * height * depth * sizeof(unsigned char));
}

void ofxffmv::setCameraType()
{
	cameraType = FFMV;
	cameraTypeName = "FFMV";
}

void ofxffmv::cameraInitializationLogic()
{
	FlyCaptureInfoEx* tempArInfo = new FlyCaptureInfoEx[_MAX_CAMERAS_];
	unsigned int camNum = _MAX_CAMERAS_;
	flycaptureBusEnumerateCamerasEx( tempArInfo, &camNum );
	for (int i=0;i<camNum;i++)
	{
		if ((unsigned long)tempArInfo[i].SerialNumber == guid.Data1)
			index = i;
	}
	arInfo = tempArInfo[index];
	flycaptureCreateContext(&cameraContext);
	flycaptureInitialize( cameraContext, index );
	flycaptureStartCustomImage(cameraContext,(unsigned int)cameraPixelMode,left,top,width,height,100,depth == 1 ? (arInfo.CameraType == FLYCAPTURE_COLOR ? FLYCAPTURE_RAW8 :  FLYCAPTURE_MONO8) : FLYCAPTURE_RGB8);
	flycaptureGrabImage2( cameraContext, &fcImage );
	width = fcImage.iCols;
	height = fcImage.iRows;
	depth = (width*height!=0) ? fcImage.iRowInc/fcImage.iRows : 0;
	flycaptureSetColorProcessingMethod( cameraContext,depth == 1 ? FLYCAPTURE_DISABLE : FLYCAPTURE_NEAREST_NEIGHBOR_FAST);
	for (int i=0;i<cameraBaseSettings->propertyType.size();i++)
		setCameraFeature(cameraBaseSettings->propertyType[i],cameraBaseSettings->propertyFirstValue[i],cameraBaseSettings->propertySecondValue[i],cameraBaseSettings->isPropertyAuto[i],cameraBaseSettings->isPropertyOn[i]);
	pgrcamguiCreateContext(&guiContext);
	delete tempArInfo;
}

void ofxffmv::cameraDeinitializationLogic()
{
	flycaptureStop( cameraContext );
	flycaptureDestroyContext( cameraContext );
	pgrcamguiDestroyContext(guiContext);
}


GUID* ofxffmv::getBaseCameraGuids(int* camCount)
{
	*camCount = 0;
	unsigned int tcamNum = _MAX_CAMERAS_;
	FlyCaptureInfoEx* tArInfo = new FlyCaptureInfoEx[_MAX_CAMERAS_];
	flycaptureBusEnumerateCamerasEx( tArInfo, &tcamNum );
	*camCount = tcamNum;
	GUID* guids = (GUID*)malloc(tcamNum*sizeof(GUID));
	for (int i=0;i<tcamNum;i++)
	{
		GUID guid2;
		guid2.Data1 = tArInfo[i].SerialNumber;
		guid2.Data2 = guid2.Data3 = 0;
		memset((void*)guid2.Data4,0,8*sizeof(unsigned char));
		guids[i] = guid2;
	}
	delete tArInfo;
	return guids;
}
