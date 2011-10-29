/*
*  ofxCMUCamera.h
*  
*
*  Created on 12/03/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*
*/

#ifndef _OFX_CMU_CAMERA_
#define _OFX_CMU_CAMERA_

#include <windows.h>
#include <process.h>
#include <mmsystem.h>
#include <stdio.h>
#include "1394Camera.h"
#include "ofxCameraBase.h"

class ofxCMUCamera : ofxCameraBase
{
public:
	ofxCMUCamera();
	~ofxCMUCamera();
	void setCameraFeature(CAMERA_BASE_FEATURE featureCode,int firstValue,int secondValue,bool isAuto,bool isEnabled);
	void getCameraFeature(CAMERA_BASE_FEATURE featureCode,int* firstValue,int* secondValue, bool* isAuto, bool* isEnabled,int* minValue,int* maxValue);
	int getCameraBaseCount();
	GUID* getBaseCameraGuids(int* camCount);
	CAMERA_BASE_FEATURE* getSupportedFeatures(int* featuresCount);
	void callSettingsDialog();
protected:
	void getNewFrame(unsigned char* newFrame);
	void cameraInitializationLogic();
	void cameraDeinitializationLogic();
	void setCameraType();
private:
	void decodePixelMode(int pixelCode,int* format0Code,int* format1Code);
	int getFramerateCode(int framerate);
	C1394Camera* theCamera;
};

#endif//_OFX_CMU_CAMERA_
