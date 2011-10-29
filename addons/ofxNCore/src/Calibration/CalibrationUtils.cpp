/*
*  CalibrationUtils.cpp
*
*
*  Created on 2/2/09.
*  Copyright 2009 NUI Group. All rights reserved.
*  Anatoly Churikov: Adapted for multiple camera use 27.08.2011
*
*/

#include "CalibrationUtils.h"
#include <cmath>

CalibrationUtils::CalibrationUtils()
{
	screenPoints = NULL;
	cameraPoints = NULL;
	drawingPoints = NULL;
	selectedCameraIndex = NULL_CAMERA;
	bNextCamera = false;
	bCalibrating = false;
	bGoToNextStep = false;
}

CalibrationUtils::~CalibrationUtils()
{
	if (screenPoints!=NULL)
		free(screenPoints);
	if (cameraPoints!=NULL)
		free(cameraPoints);
	if (drawingPoints!=NULL)
		free(drawingPoints);
}

void CalibrationUtils::setCalibrationProperties(unsigned int stitchedFrameWidth,unsigned int stitchedFrameHeight,unsigned int cameraGridWidth,unsigned int cameraGridHeight,unsigned int calibrationGridWidth,unsigned int calibrationGridHeight,bool isInterleaveMode)
{
	GRID_X = calibrationGridWidth;
	GRID_Y = calibrationGridHeight;
	GRID_POINTS = ((GRID_X+1) * (GRID_Y+1));
    GRID_INDICES = (GRID_X * GRID_Y * 3 * 2);
	screenWidth = stitchedFrameWidth;
	screenHeight = stitchedFrameHeight;
	this->cameraGridWidth = cameraGridWidth;
	this->cameraGridHeight = cameraGridHeight;
	this->isInterleaveMode = isInterleaveMode;
	if (screenPoints != NULL)
		free(screenPoints);
	screenPoints = (vector2df*)malloc(GRID_POINTS * sizeof(vector2df));
	if (cameraPoints != NULL)
		free(cameraPoints);
	cameraPoints = (vector2df*)malloc(GRID_POINTS*sizeof(vector2df));
	if (drawingPoints != NULL)
		free(drawingPoints);
	drawingPoints = (vector2df*)malloc(GRID_POINTS*sizeof(vector2df));
	setCalibrationCameraIndex(0);
}

void CalibrationUtils::setCalibrationCameraIndex(unsigned int cameraIndex)
{
	if (cameraIndex >= cameraGridWidth * cameraGridHeight)
		return;
	selectedCameraIndex = cameraIndex;
	unsigned char** cameraMap = (unsigned char**)malloc(4 * sizeof(unsigned char*));
	for (int i=0;i<4;i++)
	{
		cameraMap[i] = (unsigned char*)malloc(screenWidth * screenHeight * sizeof(unsigned char));
		memset(cameraMap[i],NULL_CAMERA,screenWidth*screenHeight*sizeof(unsigned char));
	}
	int size = screenWidth * screenHeight;
	int cameraCount = cameraGridWidth * cameraGridHeight;
	if (isInterleaveMode)
	{
		int globalCalibrationGridWidth = GRID_X * cameraGridWidth - cameraGridWidth + 1;
		int globalCalibrationGridHeight = GRID_Y * cameraGridHeight - cameraGridHeight + 1;
		float calibrationGridCellWidth = ((float)screenWidth) / globalCalibrationGridWidth;
		float calibrationGridCellHeight = ((float)screenHeight) / globalCalibrationGridHeight;
		for (int i=0;i<size;i++)
		{
			int x = i % screenWidth;
			int y = i / screenWidth;//FUCK!!!
			int blockX = (int)(((float)x) / calibrationGridCellWidth);
			int blockY = (int)(((float)y) / calibrationGridCellHeight);
			for (int j=0;j<cameraCount;j++)
			{
				int cameraX = j % cameraGridWidth;
				int cameraY = j / cameraGridWidth;
				cameraX *= (GRID_X-1);
				cameraY *= (GRID_Y-1);
				if ((blockX>=cameraX)&&(blockY>=cameraY)&&(blockX<cameraX+GRID_X) && (blockY<cameraY+GRID_Y))
				{
					for (int k=0;k<4;k++)
					{
						if (cameraMap[k][i] == NULL_CAMERA)
						{
							cameraMap[k][i] = (unsigned char)j;
							break;
						}
					}
				}
			}
		}
	}
	else
	{
		int blockWidthSize = screenWidth / cameraGridWidth;
		int blockHeightSize = screenHeight / cameraGridHeight;
		for (int i=0;i<size;i++)
		{
			int x = i % screenWidth;
			x/=blockWidthSize;
			if (x >= cameraGridWidth)
				x = cameraGridWidth - 1;
			int y = i / screenWidth;
			y/=blockHeightSize;
			if (y >= cameraGridHeight)
				y = cameraGridHeight - 1;
			int index = y * cameraGridWidth + x;
			cameraMap[0][i]=(unsigned char)index;
		}
	}
	int LeftX = 0;
	int TopY = 0;
	for (int i=0;i<size;i++)
	{
		if ((cameraMap[0][i] == cameraIndex) ||
			(cameraMap[1][i] == cameraIndex) ||
			(cameraMap[2][i] == cameraIndex) ||
			(cameraMap[3][i] == cameraIndex))
		{
			LeftX = i % screenWidth;
			TopY = i / screenWidth;
			break;
		}
	}

	int globalCalibrationGridWidth = (isInterleaveMode ?  GRID_X * cameraGridWidth - cameraGridWidth + 1 : GRID_X * cameraGridWidth);
	int globalCalibrationGridHeight = (isInterleaveMode ? GRID_Y * cameraGridHeight - cameraGridHeight + 1 : GRID_Y * cameraGridHeight);
	
	float calibrationGridCellWidth = ((float)screenWidth) / globalCalibrationGridWidth;
	float calibrationGridCellHeight = ((float)screenHeight) / globalCalibrationGridHeight;

	int cameraX = cameraIndex % cameraGridWidth;
	int cameraY = cameraIndex / cameraGridWidth;
				
	cameraX *= (isInterleaveMode  ? (GRID_X-1) : GRID_X);
	cameraY *= (isInterleaveMode  ? (GRID_Y-1) : GRID_Y);

	int P = 0;
	for (int j=0;j<=GRID_Y;j++)
	{
		for (int i=0;i<=GRID_X;i++)
		{
			drawingPoints[P].X = (float)(calibrationGridCellWidth * i + LeftX)/screenWidth;
			drawingPoints[P].Y = (float)(calibrationGridCellHeight * j + TopY)/screenHeight;
			screenPoints[P].X = (float)(calibrationGridCellWidth * i + LeftX);
			screenPoints[P].Y = (float)(calibrationGridCellHeight * j + TopY);
			P++;
		}
	}
	for (int i=0;i<4;i++)
		free(cameraMap[i]);
	free(cameraMap);
	initCameraPoints();
}

void CalibrationUtils::initCameraPoints()
{
	int p = 0;
	for(int j = 0; j <= GRID_Y; j++)
	{
		for(int i = 0; i <= GRID_X; i++)
		{
			cameraPoints[p] = vector2df((i) / (float)GRID_X, (j) / (float)GRID_Y);
			p++;
		}
	}
}

void CalibrationUtils::initScreenPoints(int cameraPosition)
{
	unsigned char** cameraMap = (unsigned char**)malloc(4 * sizeof(unsigned char*));
	for (int i=0;i<4;i++)
	{
		cameraMap[i] = (unsigned char*)malloc(screenWidth * screenHeight * sizeof(unsigned char));
		memset(cameraMap[i],NULL_CAMERA,screenWidth*screenHeight*sizeof(unsigned char));
	}
	int size = screenWidth * screenHeight;
	int cameraCount = cameraGridWidth * cameraGridHeight;
	if (isInterleaveMode)
	{
		int globalCalibrationGridWidth = GRID_X * cameraGridWidth - cameraGridWidth + 1;
		int globalCalibrationGridHeight = GRID_Y * cameraGridHeight - cameraGridHeight + 1;
		float calibrationGridCellWidth = ((float)screenWidth) / globalCalibrationGridWidth;
		float calibrationGridCellHeight = ((float)screenHeight) / globalCalibrationGridHeight;
		for (int i=0;i<size;i++)
		{
			int x = i % screenWidth;
			int y = i / screenWidth;
			int blockX = (int)(((float)x) / calibrationGridCellWidth);
			int blockY = (int)(((float)y) / calibrationGridCellHeight);
			for (int j=0;j<cameraCount;j++)
			{
				int cameraX = j % cameraGridWidth;
				int cameraY = j / cameraGridWidth;
				cameraX *= (GRID_X-1);
				cameraY *= (GRID_Y-1);
				if ((blockX>=cameraX)&&(blockY>=cameraY)&&(blockX<cameraX+GRID_X) && (blockY<cameraY+GRID_Y))
				{
					for (int k=0;k<4;k++)
					{
						if (cameraMap[k][i] == NULL_CAMERA)
						{
							cameraMap[k][i] = (unsigned char)j;
							break;
						}
					}
				}
			}
		}
	}
	else
	{
		int blockWidthSize = screenWidth / cameraGridWidth;
		int blockHeightSize = screenHeight / cameraGridHeight;
		for (int i=0;i<size;i++)
		{
			int x = i % screenWidth;
			x/=blockWidthSize;
			if (x >= cameraGridWidth)
				x = cameraGridWidth - 1;
			int y = i / screenWidth;
			y/=blockHeightSize;
			if (y >= cameraGridHeight)
				y = cameraGridHeight - 1;
			int index = y * cameraGridWidth + x;
			cameraMap[0][i]=(unsigned char)index;
		}
	}
	int LeftX = 0;
	int TopY = 0;
	for (int i=0;i<size;i++)
	{
		if ((cameraMap[0][i] == cameraPosition) ||
			(cameraMap[1][i] == cameraPosition) ||
			(cameraMap[2][i] == cameraPosition) ||
			(cameraMap[3][i] == cameraPosition))
		{
			LeftX = i % screenWidth;
			TopY = i / screenWidth;
			break;
		}
	}

	int globalCalibrationGridWidth = (isInterleaveMode ?  GRID_X * cameraGridWidth - cameraGridWidth + 1 : GRID_X * cameraGridWidth);
	int globalCalibrationGridHeight = (isInterleaveMode ? GRID_Y * cameraGridHeight - cameraGridHeight + 1 : GRID_Y * cameraGridHeight);
	
	float calibrationGridCellWidth = ((float)screenWidth) / globalCalibrationGridWidth;
	float calibrationGridCellHeight = ((float)screenHeight) / globalCalibrationGridHeight;

	int cameraX = cameraPosition % cameraGridWidth;
	int cameraY = cameraPosition / cameraGridWidth;
				
	cameraX *= (isInterleaveMode  ? (GRID_X-1) : GRID_X);
	cameraY *= (isInterleaveMode  ? (GRID_Y-1) : GRID_Y);

	int P = 0;
	for (int j=0;j<=GRID_Y;j++)
	{
		for (int i=0;i<=GRID_X;i++)
		{
			screenPoints[P].X = (float)(calibrationGridCellWidth * i + LeftX);
			screenPoints[P].Y = (float)(calibrationGridCellHeight * j + TopY);
			P++;
		}
	}
	for (int i=0;i<cameraGridWidth * cameraGridHeight;i++)
		free(cameraMap[i]);
	free(cameraMap);
}
void CalibrationUtils::nextCameraCalibration()
{
	selectedCameraIndex++;
	if (selectedCameraIndex >= cameraGridWidth * cameraGridHeight)
	{
		selectedCameraIndex = 0;
	}
	setCalibrationCameraIndex(selectedCameraIndex);
	bNextCamera = false;
}

void CalibrationUtils::beginCalibration()
{
	 bCalibrating = true;
	 calibrationStep = 0;
	 setCalibrationCameraIndex(0);
}

void CalibrationUtils::nextCalibrationStep()
{
	if(bCalibrating)
	{
		calibrationStep++;
		if (calibrationStep >= GRID_POINTS) 
		{
			bNextCamera = true;
			calibrationStep = 0;
		}
	}
}
void CalibrationUtils::revertCalibrationStep()
{
	if(bCalibrating)
	{
		calibrationStep--;
		if(calibrationStep < 0)
		{
			calibrationStep = 0;
		}
	}
}
