/*
*  ofxMultiplexer.h
*  
*
*  Created on 12/03/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*
*/

#ifndef _OFX_MULTIPLEXER_
#define _OFX_MULTIPLEXER_

#include <stdio.h>
#include <omp.h>
#include <memory.h>
#include <vector>
#include "omp.h"
#include "ofxCameraBase.h"
#include "ofxCameraBaseSettings.h"
#include "ofxffmv.h"
#include "ofxPS3.h"
#include "ofxDShow.h"
#include "ofxKinect.h"
#include "ofxCMUCamera.h"
#include "ofxGUIDHelper.h"
#include "ofxXMLSettings.h"
#include "Calibration.h"

#define NULL_CAMERA 0xFF

class ofxMultiplexer
{
public:
	ofxMultiplexer();
	~ofxMultiplexer();

	void initializeMultiplexer();
	void deinitializeMultiplexer();

	void addCameraBase(ofxCameraBaseCalibration* settings);
	void removeCameraBase(ofxCameraBaseCalibration* settings);
	void removeCameraBase(int index);
	void clearAllCameraBase();

	void startStreamingFromCamera(int index);
	void pauseStreamingFromCamera(int index);
	void startStreamingFromAllCameras();
	void pauseStreamingFromAllCameras();
	void updateStitchedFrame();
	void getStitchedFrame(int* width,int* height,unsigned char* frameData);
	void setCalibrationPointsToCamera(int index,vector2df* calibrationPoints);
	void setCameraGridSize(int width,int height);
	void getCameraGridSize(int* width,int* height);
	void setCalibrationGridSize(int width,int height);
	void getCalibrationGridSize(int* width,int* height);
	void setStitchedFrameSize(int width, int height);
	void getStitchedFrameSize(int* width,int* height);
	void setInterleaveMode(bool isInterlaveMode);
	void getInterleaveMode(bool* isInterleaveMode);
	void setIsCalibrationMode(bool isCalibrating);
	void getIsCalibrationMode(bool* isCalibrating);
private:
	void computeDistortion();
	void computeCameraMaps();
	void computeOffsetMap(unsigned int* cameraOffsetMap,vector2df* calibrationPoints,int cameraPosition);
	void computeWeightMap(float* cameraWeightMap,vector2df* calibrationPoints,int cameraPosition);
	int findTriangleWithin(vector2df pt,vector2df* screenPoints,int* triangles);
	bool isPointInTriangle(vector2df p, vector2df a, vector2df b, vector2df c);
	void cameraToScreenSpace(unsigned int &x, unsigned int &y,vector2df* screenPoints,vector2df* cameraPoints,int* triangles);
private:
	vector<ofxCameraBaseCalibration* > cameraBasesCalibration;
	vector2df** cameraCalibrationPoints;
	unsigned int** offsetMap;
	float**		   weightMap;
	unsigned char** cameraMap;
	unsigned char** cameraFrames;
	unsigned char* stitchedFrame;
	int* cameraFramesWidth;
	int* cameraFramesHeight;
	ofxCameraBase** cameras;
	bool* blackCapturingMode;
	int stitchedFrameWidth,stitchedFrameHeight,cameraGridWidth,cameraGridHeight,calibrationGridWidth,calibrationGridHeight;
	int actualStitchedFrameWidth,actualStitchedFrameHeight,actualCameraGridWidth,actualCameraGridHeight,actualCalibrationGridWidth,actualCalibrationGridHeight;
	bool interleaveMode,calibratingMode;
};

#endif//_OFX_MULTIPLEXER_