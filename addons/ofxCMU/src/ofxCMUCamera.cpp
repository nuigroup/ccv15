/*
*  ofxCMUCamera.cpp
*  
*
*  Created on 12/03/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*
*/

#include "ofxCMUCamera.h"
#include <iostream>
using namespace std;

ofxCMUCamera::ofxCMUCamera()
{
	theCamera = NULL;
}

ofxCMUCamera::~ofxCMUCamera()
{
	deinitializeCamera();
}

void ofxCMUCamera::callSettingsDialog()
{
	CameraControlDialog(NULL,theCamera,TRUE);
}

void ofxCMUCamera::getNewFrame(unsigned char* newFrame)
{
	if (theCamera == NULL)
		return;
	HANDLE hFrameEvent;
	DWORD dwRet;
	bool localFrameNew = false;
	hFrameEvent = theCamera->GetFrameEvent();
	if(hFrameEvent == NULL)
	{
		if(theCamera->AcquireImageEx(FALSE,NULL) != CAM_ERROR_FRAME_TIMEOUT)
			return;
		hFrameEvent = theCamera->GetFrameEvent();
	}
	dwRet = MsgWaitForMultipleObjects(1,&hFrameEvent,FALSE,1000,QS_ALLEVENTS);
	switch(dwRet)
	{
		case WAIT_OBJECT_0:
			localFrameNew = true;
			if(theCamera->AcquireImageEx(FALSE,NULL) != CAM_SUCCESS)
				theCamera->StopImageAcquisition();
			break;
		case WAIT_OBJECT_0 + 1:
			break;
		case WAIT_TIMEOUT:
			break;
		default:
			theCamera->StopImageAcquisition();
			break;
	}
	if (localFrameNew)
	{
		theCamera->GetVideoFrameDimensions((unsigned long*)(&width),(unsigned long*)(&height));
		theCamera->getDIB(newFrame,width*height*depth*sizeof(unsigned char));
	}
}
void ofxCMUCamera::setCameraType()
{
	cameraType = CMU;
	cameraTypeName = "CMU";
}


void ofxCMUCamera::cameraInitializationLogic()
{
	if (theCamera == NULL)
		theCamera = new C1394Camera();
	int camNum = theCamera->RefreshCameraList();
	int error_code = 0;
	unsigned long long id = 0;
	for (int i=0;i<camNum;i++)
	{
		error_code = theCamera->SelectCamera(i);
		theCamera->GetCameraUniqueID((PLARGE_INTEGER)&id);
		if (error_code)	return;
		if ((unsigned long)id == guid.Data1)
		{
			index = i;
			break;
		}
	}

	if (theCamera->SelectCamera(index)!= CAM_SUCCESS)
		return;
	if (theCamera->InitCamera(false)!= CAM_SUCCESS)
		return;
	int code0,code1;
	decodePixelMode(cameraPixelMode,&code0,&code1);
	if (theCamera->SetVideoFormat(code0)!= CAM_SUCCESS)
		return;
	if (theCamera->SetVideoMode(code1)!= CAM_SUCCESS)
		return;
	if (theCamera->SetVideoFrameRate(getFramerateCode(framerate))!= CAM_SUCCESS)
		return;
	for (int i=0;i<cameraBaseSettings->propertyType.size();i++)
		setCameraFeature(cameraBaseSettings->propertyType[i],cameraBaseSettings->propertyFirstValue[i],cameraBaseSettings->propertySecondValue[i],cameraBaseSettings->isPropertyAuto[i],cameraBaseSettings->isPropertyOn[i]);
	if (theCamera->StartImageAcquisitionEx(6,0,ACQ_START_VIDEO_STREAM)!= CAM_SUCCESS)
		return;
	depth = 3;
	theCamera->GetVideoFrameDimensions((unsigned long*)&width,(unsigned long*)&height);
}

void ofxCMUCamera::cameraDeinitializationLogic()
{
	if (theCamera!=NULL)
	{	
		theCamera->StopImageAcquisition();
		delete theCamera;
	}
	theCamera = NULL;	
}

CAMERA_BASE_FEATURE* ofxCMUCamera::getSupportedFeatures(int* featuresCount)
{
	*featuresCount = 9;
	CAMERA_BASE_FEATURE* features = (CAMERA_BASE_FEATURE*)malloc(*featuresCount * sizeof(CAMERA_BASE_FEATURE));
	features[0] = BASE_BRIGHTNESS;
	features[1] = BASE_EXPOSURE;
	features[2] = BASE_SHARPNESS;
	features[3] = BASE_SATURATION;
	features[4] = BASE_GAMMA;
	features[5] = BASE_SHUTTER;
	features[6] = BASE_GAIN;
	features[7] = BASE_WHITE_BALANCE;
	features[8] = BASE_FRAMERATE;
	return features;
}

void ofxCMUCamera::setCameraFeature(CAMERA_BASE_FEATURE featureCode,int firstValue,int secondValue,bool isAuto,bool isEnabled)
{
	CAMERA_FEATURE deviceProperty = (CAMERA_FEATURE)0xFFFFFFFF;
	switch (featureCode)
	{
	case BASE_BRIGHTNESS:
		deviceProperty = FEATURE_BRIGHTNESS;
		break;
	case BASE_EXPOSURE:
		deviceProperty = FEATURE_AUTO_EXPOSURE;
		break;
	case BASE_SHARPNESS:
		deviceProperty = FEATURE_SHARPNESS;
		break;
	case BASE_SATURATION:
		deviceProperty = FEATURE_SATURATION;
		break;
	case BASE_GAMMA:
		deviceProperty = FEATURE_GAMMA;
		break;
	case BASE_SHUTTER:
		deviceProperty = FEATURE_SHUTTER;
		break;
	case BASE_GAIN:
		deviceProperty = FEATURE_GAIN;
		break;
	case BASE_WHITE_BALANCE:
		deviceProperty = FEATURE_WHITE_BALANCE;
		break;
	case BASE_FRAMERATE:
		framerate = firstValue;
		break;
	}
	if (deviceProperty!=0xFFFFFFFF)
	{
		C1394CameraControl* control = theCamera->GetCameraControl(deviceProperty);
	    if (control!=NULL)
		{
			control->SetAutoMode(isAuto);
			control->SetValue(firstValue,secondValue);
		}
	}
}

void ofxCMUCamera::getCameraFeature(CAMERA_BASE_FEATURE featureCode,int* firstValue,int* secondValue, bool* isAuto, bool* isEnabled,int* minValue,int* maxValue)
{
	*firstValue = 0;
	*secondValue = 0;
	*isAuto = false;
	*isEnabled = false;
	*minValue = 0;
	*maxValue = 0;
	CAMERA_FEATURE deviceProperty = (CAMERA_FEATURE)0xFFFFFFFF;
	switch (featureCode)
	{
		case BASE_BRIGHTNESS:
			deviceProperty = FEATURE_BRIGHTNESS;
			break;
		case BASE_EXPOSURE:
			deviceProperty = FEATURE_AUTO_EXPOSURE;
			break;
		case BASE_SHARPNESS:
			deviceProperty = FEATURE_SHARPNESS;
			break;
		case BASE_SATURATION:
			deviceProperty = FEATURE_SATURATION;
			break;
		case BASE_GAMMA:
			deviceProperty = FEATURE_GAMMA;
			break;
		case BASE_SHUTTER:
			deviceProperty = FEATURE_SHUTTER;
			break;
		case BASE_GAIN:
			deviceProperty = FEATURE_GAIN;
			break;
		case BASE_WHITE_BALANCE:
			deviceProperty = FEATURE_WHITE_BALANCE;
			break;
		case BASE_FRAMERATE:
			*minValue = 15;
			*maxValue = 240;
			*isEnabled = true;
			*firstValue = framerate;
			*secondValue = 0;
			*isAuto = false;
			return;
	}
	if (deviceProperty!=0xFFFFFFFF)
	{
		C1394CameraControl* control = theCamera->GetCameraControl(deviceProperty);
	    *isEnabled = (control!=NULL);
		if (control!=NULL)
		{
			*isAuto = control->StatusAutoMode();
			unsigned short first;
			unsigned short second;
			control->GetValue(&first,&second);
			unsigned short sMin,sMax;
			control->GetRange(&sMin,&sMax);
			*minValue = sMin;
			*maxValue = sMax;
			*firstValue = first;
			*secondValue = second;
		}
	}
	else
		*isEnabled = false;
}

int ofxCMUCamera::getCameraBaseCount()
{
	C1394Camera* theCamera = new C1394Camera();
	int cNum = theCamera->RefreshCameraList();
	delete theCamera;
	return cNum;
}

GUID* ofxCMUCamera::getBaseCameraGuids(int* camCount)
{
	C1394Camera* theCamera = new C1394Camera();
	*camCount = theCamera->RefreshCameraList();
	GUID* guids = (GUID*)malloc(*camCount * sizeof(GUID));
	int error_code = 0;
	unsigned long long id = 0;
	for (int i=0;i<*camCount;i++)
	{
		error_code = theCamera->SelectCamera(i);
		theCamera->GetCameraUniqueID((PLARGE_INTEGER)&id);
		GUID guid2;
		guid2.Data1 = (unsigned long)id;
		guid2.Data2 = guid2.Data3 = 0;
		for (int j=0;j<8;j++)
			guid2.Data4[j] = 0;
		guids[i] = guid2;
	}
	return guids;
}

void ofxCMUCamera::decodePixelMode(int pixelCode,int* format0Code,int* format1Code)
{
	if (pixelCode < 7)
	{
		*format0Code = 0;
		*format1Code = pixelCode;
	}
	else
	{
		if (pixelCode < 15)
		{
			*format0Code = 1;
			*format1Code = pixelCode - 7;
		}
		else
		{
			*format0Code = 2;
			*format1Code = pixelCode - 15;
		}
	}

}

int ofxCMUCamera::getFramerateCode(int framerate)
{
	int framerates[8];
	framerates[0] = 2;
	framerates[1] = 3;
	framerates[2] = 8;
	framerates[3] = 15;
	framerates[4] = 30;
	framerates[5] = 60;
	framerates[6] = 120;
	framerates[7] = 240;
	int index = 0;
	for (int i=0;i<8;i++)
	{
		if ((framerate - framerates[i])*(framerate - framerates[i]) < (framerate - framerates[index])*(framerate - framerates[index]))
			index = i;
	}
	return index;
}


