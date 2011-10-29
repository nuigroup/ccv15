/*
*  ofxCameraBaseSettings.h
*  
*
*  Created on 12/03/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*
*/
#ifndef OFX_CAMERABASE_SETTINGS_H
#define OFX_CAMERABASE_SETTINGS_H

#include <vector>
#include "vector2d.h"
#include <windows.h>

typedef enum 
{
	PS3,
	CMU,
	FFMV,
	DIRECTSHOW,
	KINECT
} CAMERATYPE;

typedef enum 
{
	BASE_BRIGHTNESS,
	BASE_EXPOSURE,
	BASE_SHARPNESS,
	BASE_WHITE_BALANCE,
	BASE_HUE,
	BASE_CONTRAST,
	BASE_SATURATION,
	BASE_GAMMA,
	BASE_SHUTTER,
	BASE_GAIN,
	BASE_IRIS,
	BASE_FOCUS,
	BASE_ZOOM,
	BASE_PAN,
	BASE_TILT,
	BASE_ROLL,
	BASE_FRAMERATE,
	BASE_MOTOR_POSITION,
	BASE_MOTOR_LED,
	DEPTH_NEAR_BOUND,
	DEPTH_FAR_BOUND,
	BASE_HFLIP,				
	BASE_VFLIP,				
	BASE_HKEYSTONE,			
	BASE_VKEYSTONE,			
	BASE_XOFFSET,				
	BASE_YOFFSET,				
	BASE_ROTATION,				
	BASE_LENSCORRECTION1,		
	BASE_LENSCORRECTION2,		
	BASE_LENSCORRECTION3,		
	BASE_LENSBRIGHTNESS,
} CAMERA_BASE_FEATURE;

class ofxCameraBase;

typedef struct ofxCameraBaseSettings
{
	ofxCameraBase* camera;
	std::vector<bool> isPropertyOn;
	std::vector<bool> isPropertyAuto;
	std::vector<int> propertyFirstValue;
	std::vector<int> propertySecondValue;
	std::vector<CAMERA_BASE_FEATURE> propertyType;
} ofxCameraBaseSettings;

typedef struct ofxCameraBaseCalibration
{
	ofxCameraBase* camera;
	int index;
	std::vector<vector2df> calibrationPoints;
}ofxCameraBaseCalibration;

#endif // OFX_CAMERABASE_SETTINGS_H