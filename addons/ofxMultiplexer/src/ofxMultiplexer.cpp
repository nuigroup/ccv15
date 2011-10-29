/*
*  ofxMultiplexer.cpp
*  
*
*  Created on 12/03/11.
*  Copyright 2011 NUI Group. All rights reserved.
*  Author: Anatoly Churikov
*
*/
#include "ofxMultiplexer.h"

ofxMultiplexer::ofxMultiplexer()
{
	offsetMap = NULL;
	weightMap = NULL;
	stitchedFrame = NULL;
	cameraFrames = NULL;
	cameraMap = NULL;
	cameraFramesWidth = NULL;
	cameraFramesHeight = NULL;
	cameras = NULL;
	blackCapturingMode = NULL;
	cameraCalibrationPoints = NULL;
	calibratingMode = false;
}

ofxMultiplexer::~ofxMultiplexer()
{
	deinitializeMultiplexer();
}

void ofxMultiplexer::setIsCalibrationMode(bool isCalibrating)
{
	calibratingMode = isCalibrating;
}

void ofxMultiplexer::getIsCalibrationMode(bool* isCalibrating)
{
	*isCalibrating = calibratingMode;
}

void ofxMultiplexer::setCalibrationPointsToCamera(int index,vector2df* calibrationPoints)
{
	int newCameraIndex = -1;
	for (int i=0;i<cameraBasesCalibration.size();i++)
	{
		if (cameraBasesCalibration[i]->index == index)
			newCameraIndex = i;
	}
	if (newCameraIndex>=0)
	{
		cameraBasesCalibration[newCameraIndex]->calibrationPoints.clear();
		for (int i=0;i<(actualCalibrationGridWidth+1)*(actualCalibrationGridHeight+1);i++)
		{
			vector2df newPoint;
			newPoint.X = calibrationPoints[i].X;
			newPoint.Y = calibrationPoints[i].Y;
			cameraBasesCalibration[newCameraIndex]->calibrationPoints.push_back(newPoint);
		}
	}
}

void ofxMultiplexer::initializeMultiplexer()
{
	deinitializeMultiplexer();
	stitchedFrame = (unsigned char*)malloc(stitchedFrameWidth * stitchedFrameHeight * sizeof(unsigned char));
	offsetMap = (unsigned int**)malloc(cameraGridWidth*cameraGridHeight * sizeof(unsigned int*));
	weightMap = (float**)malloc(cameraGridWidth*cameraGridHeight * sizeof(float*));
	cameraFrames = (unsigned char**)malloc(cameraGridWidth*cameraGridHeight * sizeof(unsigned char*));
	cameraCalibrationPoints = (vector2df**)malloc(cameraGridWidth*cameraGridHeight * sizeof(vector2df*));
	blackCapturingMode = (bool*)malloc(cameraGridWidth*cameraGridHeight*sizeof(bool));
	cameras = (ofxCameraBase**)malloc(cameraGridWidth*cameraGridHeight * sizeof(ofxCameraBase*));
	cameraMap = (unsigned char**)malloc(4 * sizeof(unsigned char*));
	cameraFramesWidth = (int*)malloc(cameraGridWidth*cameraGridHeight * sizeof(int));
	cameraFramesHeight = (int*)malloc(cameraGridWidth*cameraGridHeight * sizeof(int));
	for (int i=0;i<4;i++)
	{
		cameraMap[i] = (unsigned char*)malloc(stitchedFrameWidth*stitchedFrameHeight*sizeof(unsigned char));
		memset((void*)cameraMap[i],NULL_CAMERA,stitchedFrameWidth*stitchedFrameHeight*sizeof(unsigned char));
	}
	for (int i=0;i<cameraGridWidth*cameraGridHeight;i++)
	{
		cameraCalibrationPoints[i] = (vector2df*)malloc((calibrationGridWidth+1)*(calibrationGridHeight+1)*sizeof(vector2df));
		cameras[i] = NULL;
		offsetMap[i] = (unsigned int*)malloc(stitchedFrameWidth * stitchedFrameHeight * sizeof(unsigned int));
		memset(offsetMap[i],0,stitchedFrameWidth * stitchedFrameHeight * sizeof(unsigned int));
		weightMap[i] = (float*)malloc(stitchedFrameWidth * stitchedFrameHeight * sizeof(float));
		memset(weightMap[i],0,stitchedFrameWidth * stitchedFrameHeight * sizeof(float));
		blackCapturingMode[i] = true;
		int newCameraIndex = -1;
		for (int j=0;j<cameraBasesCalibration.size();j++)
		{
			if (cameraBasesCalibration[j]->index == i)
				newCameraIndex = j;
		}
		if (newCameraIndex>=0)
		{
			unsigned int cameraWidth,cameraHeight;
			unsigned char cameraDepth,cameraPixel;
			cameraBasesCalibration[newCameraIndex]->camera->getCameraSize(&cameraWidth,&cameraHeight,&cameraDepth,&cameraPixel);
			cameraBasesCalibration[newCameraIndex]->camera->resumeCamera();
			cameraFramesWidth[i] = cameraWidth;
			cameraFramesHeight[i] = cameraHeight;
			if (cameraBasesCalibration[newCameraIndex]->calibrationPoints.size() == (calibrationGridWidth+1)*(calibrationGridHeight+1))
			{
				for (int j=0;j<(calibrationGridWidth+1)*(calibrationGridHeight+1);j++)
				{
					cameraCalibrationPoints[i][j].X = cameraBasesCalibration[newCameraIndex]->calibrationPoints[j].X * cameraFramesWidth[i];
					cameraCalibrationPoints[i][j].Y = cameraBasesCalibration[newCameraIndex]->calibrationPoints[j].Y * cameraFramesHeight[i];
				}
			}
			else
			{
				cameraBasesCalibration[newCameraIndex]->calibrationPoints.clear();
				float dx = cameraFramesWidth[i] / calibrationGridWidth;
				float dy = cameraFramesHeight[i] / calibrationGridHeight;
				int P = 0;
				for (int y=0;y<=calibrationGridHeight;y++)
				{
					for (int x=0;x<=calibrationGridWidth;x++)
					{
						cameraCalibrationPoints[i][P].X = dx * x;
						cameraCalibrationPoints[i][P].Y = dy * y;
						P++;
					}
				}
			}
			blackCapturingMode[i] = false;
			cameras[i] = cameraBasesCalibration[newCameraIndex]->camera;
		}
		else
		{
			cameraFramesWidth[i] = 320;
			cameraFramesHeight[i] = 240;
			float dx = cameraFramesWidth[i] / calibrationGridWidth;
			float dy = cameraFramesHeight[i] / calibrationGridHeight;
			int P = 0;
			for (int y=0;y<=calibrationGridHeight;y++)
			{
				for (int x=0;x<=calibrationGridWidth;x++)
				{
					cameraCalibrationPoints[i][P].X = dx * x;
					cameraCalibrationPoints[i][P].Y = dy * y;
					P++;
				}
			}
		}
		cameraFrames[i] = (unsigned char*)malloc(cameraFramesWidth[i] * cameraFramesHeight[i] * sizeof(unsigned char));
		memset(cameraFrames[i],1,cameraFramesWidth[i] * cameraFramesHeight[i] * sizeof(unsigned char));
	}
	actualStitchedFrameWidth = stitchedFrameWidth;
	actualStitchedFrameHeight = stitchedFrameHeight;
	actualCameraGridWidth = cameraGridWidth;
	actualCameraGridHeight = cameraGridHeight;
	actualCalibrationGridWidth = calibrationGridWidth;
	actualCalibrationGridHeight = calibrationGridHeight;
	computeDistortion();
}

void ofxMultiplexer::deinitializeMultiplexer()
{
	if (stitchedFrame != NULL)
		free(stitchedFrame);
	if (offsetMap != NULL)
	{
		for (int i=0;i<actualCameraGridWidth*actualCameraGridHeight;i++)
			free(offsetMap[i]);
		free(offsetMap);
	}
	if (weightMap != NULL)
	{
		for (int i=0;i<actualCameraGridWidth*actualCameraGridHeight;i++)
			free(weightMap[i]);
		free(weightMap);
	}
	if (cameraFrames != NULL)
	{
		for (int i=0;i<actualCameraGridWidth*actualCameraGridHeight;i++)
			free(cameraFrames[i]);
		free(cameraFrames);
	}
	if (cameraCalibrationPoints != NULL)
	{
		for (int i=0;i<actualCameraGridWidth*actualCameraGridHeight;i++)
			free(cameraCalibrationPoints[i]);
		free(cameraCalibrationPoints);
	}
	if (cameraMap != NULL)
	{
		for (int i=0;i<4;i++)
			free(cameraMap[i]);
		free(cameraMap);
	}
	if (blackCapturingMode!=NULL)
		free(blackCapturingMode);
	if (cameras!=NULL)
		free(cameras);
	if (cameraFramesHeight == NULL)
		free(cameraFramesHeight);
	if (cameraFramesWidth == NULL)
	{
		free(cameraFramesWidth);
		cameraFramesWidth = NULL;
	}
}

void ofxMultiplexer::addCameraBase(ofxCameraBaseCalibration* settings)
{
	bool isNewCamera = true;
	for (int i=0;i<cameraBasesCalibration.size();i++)
	{
		if ((cameraBasesCalibration[i]->camera->getBaseCameraType() == settings->camera->getBaseCameraType()) && (cameraBasesCalibration[i]->camera->getCameraGUID() == settings->camera->getCameraGUID()))
		{
			isNewCamera = false;
			break;
		}
	}
	if (isNewCamera)
		cameraBasesCalibration.push_back(settings);
}

void ofxMultiplexer::removeCameraBase(ofxCameraBaseCalibration* settings)
{
	for (int i=0;i<cameraBasesCalibration.size();i++)
	{
		if ((cameraBasesCalibration[i]->camera->getBaseCameraType() == settings->camera->getBaseCameraType()) && (cameraBasesCalibration[i]->camera->getCameraGUID() == settings->camera->getCameraGUID()))
		{
			cameraBasesCalibration.erase(cameraBasesCalibration.begin()+i);
			break;
		}
	}
}

void ofxMultiplexer::removeCameraBase(int index)
{
	if (index<cameraBasesCalibration.size())
		cameraBasesCalibration.erase(cameraBasesCalibration.begin()+index);
}

void ofxMultiplexer::clearAllCameraBase()
{
	cameraBasesCalibration.clear();
}

void ofxMultiplexer::startStreamingFromCamera(int index)
{
	if (index<actualCameraGridWidth*actualCameraGridHeight)
		blackCapturingMode[index] = (cameras[index]==NULL);
}

void ofxMultiplexer::pauseStreamingFromCamera(int index)
{
	if (index<actualCameraGridWidth*actualCameraGridHeight)
		blackCapturingMode[index] = true;
}

void ofxMultiplexer::startStreamingFromAllCameras()
{
	for (int i=0;i<actualCameraGridWidth*actualCameraGridHeight;i++)
	{
		blackCapturingMode[i] = (cameras[i]==NULL);
		if (!blackCapturingMode[i])
			cameras[i]->resumeCamera();
	}
}

void ofxMultiplexer::pauseStreamingFromAllCameras()
{
	for (int i=0;i<actualCameraGridWidth*actualCameraGridHeight;i++)
		blackCapturingMode[i] = true;
}

void ofxMultiplexer::updateStitchedFrame()
{
	int size = actualStitchedFrameWidth*actualStitchedFrameHeight;
	int threadsCount = actualCameraGridWidth*actualCameraGridHeight > 4 ? 4 : actualCameraGridWidth*actualCameraGridHeight;
	#pragma omp parallel num_threads(threadsCount)
	{
		#pragma omp for
		for (int i=0;i<actualCameraGridWidth*actualCameraGridHeight;i++)
		{	
			if (blackCapturingMode[i])
				memset(cameraFrames[i],0,cameraFramesWidth[i]*cameraFramesHeight[i]*sizeof(unsigned char));
			else
				cameras[i]->getCameraFrame(cameraFrames[i]);
		}
	}
	if (interleaveMode)
	{
		if (!calibratingMode)
		{
			#pragma omp parallel num_threads(threadsCount)
			{
				#pragma omp for
				for (int i=0;i<size;i++)
				{
					if (cameraMap[1][i] == NULL_CAMERA)
					{
						stitchedFrame[i] = cameraFrames[cameraMap[0][i]][offsetMap[cameraMap[0][i]][i]];
					}
					else
					{
						float result = ((float)cameraFrames[cameraMap[0][i]][offsetMap[cameraMap[0][i]][i]]*weightMap[cameraMap[0][i]][i]) + ((float)cameraFrames[cameraMap[1][i]][offsetMap[cameraMap[1][i]][i]]*weightMap[cameraMap[1][i]][i]);
						if(cameraMap[2][i]!=NULL_CAMERA)
							result+=((float)cameraFrames[cameraMap[2][i]][offsetMap[cameraMap[2][i]][i]]*weightMap[cameraMap[2][i]][i]);
						if(cameraMap[3][i]!=NULL_CAMERA)
							result+=((float)cameraFrames[cameraMap[3][i]][offsetMap[cameraMap[3][i]][i]]*weightMap[cameraMap[3][i]][i]);
						if (result>255.0f)
							result = 255.0f;
						stitchedFrame[i] = (unsigned char)(result);
					}
				}
			}
		}
		else
		{
			#pragma omp parallel num_threads(threadsCount)
			{
				#pragma omp for
				for (int i=0;i<size;i++)
				{
					if (cameraMap[1][i] == NULL_CAMERA)
					{
						stitchedFrame[i] = cameraFrames[cameraMap[0][i]][offsetMap[cameraMap[0][i]][i]];
					}
					else
					{
						unsigned char result = 0;
						for (int j=0;j<4;j++)
						{
							if ((cameraMap[j][i] != NULL_CAMERA)&& (cameraFrames[cameraMap[j][i]][offsetMap[cameraMap[j][i]][i]]!=0))
								result = cameraFrames[cameraMap[j][i]][offsetMap[cameraMap[j][i]][i]];
						}
						stitchedFrame[i] = result;
					}
				}
			}
		}
	}
	else
	{
		#pragma omp parallel num_threads(threadsCount)
		{
			#pragma omp for
			for (int i=0;i<size;i++)
			{
				stitchedFrame[i] = cameraFrames[cameraMap[0][i]][offsetMap[cameraMap[0][i]][i]];
			}
		}
	}
}

void ofxMultiplexer::getStitchedFrame(int* width,int* height,unsigned char* frameData)
{
	*width = actualStitchedFrameWidth;
	*height = actualStitchedFrameHeight;
	memcpy(frameData,stitchedFrame,actualStitchedFrameWidth*actualStitchedFrameHeight*sizeof(unsigned char));
}

void ofxMultiplexer::setCameraGridSize(int width,int height)
{
	cameraGridWidth = width;
	cameraGridHeight = height;
}

void ofxMultiplexer::getCameraGridSize(int* width,int* height)
{
	*width = actualCameraGridWidth;
	*height = actualCameraGridHeight;
}

void ofxMultiplexer::setCalibrationGridSize(int width,int height)
{
	calibrationGridWidth = width;
	calibrationGridHeight = height;
}

void ofxMultiplexer::getCalibrationGridSize(int* width,int* height)
{
	*width = actualCalibrationGridWidth;
	*height = actualCalibrationGridHeight;
}

void ofxMultiplexer::setStitchedFrameSize(int width, int height)
{
	stitchedFrameWidth = width;
	stitchedFrameHeight = height;
}

void ofxMultiplexer::getStitchedFrameSize(int* width,int* height)
{
	*width = stitchedFrameWidth;
	*height = stitchedFrameHeight;
}

void ofxMultiplexer::computeDistortion()
{
	computeCameraMaps();
	for (int i=0;i<actualCameraGridWidth*actualCameraGridHeight;i++)
	{
		vector2df* calibrations = (vector2df*)malloc((actualCalibrationGridWidth+1)*(actualCalibrationGridHeight+1)*sizeof(vector2df));
		for (int j=0;j<(actualCalibrationGridWidth+1)*(actualCalibrationGridHeight+1);j++)
		{
			calibrations[j].X = cameraCalibrationPoints[i][j].X;
			calibrations[j].Y = cameraCalibrationPoints[i][j].Y;
		}
		computeOffsetMap(offsetMap[i],calibrations,i);
		computeWeightMap(weightMap[i],calibrations,i);
		free(calibrations);
	}
}

void ofxMultiplexer::computeCameraMaps()
{
	if (cameraMap == NULL)
		return;
	int size = actualStitchedFrameWidth*actualStitchedFrameHeight;
	int cameraCount = actualCameraGridWidth * actualCameraGridHeight;
	for (int i=0;i<4;i++)
		memset(cameraMap[i],NULL_CAMERA,actualStitchedFrameWidth*actualStitchedFrameHeight*sizeof(unsigned char));
	if (interleaveMode)
	{
		int globalCalibrationGridWidth = actualCalibrationGridWidth * actualCameraGridWidth - actualCameraGridWidth + 1;
		int globalCalibrationGridHeight = actualCalibrationGridHeight * actualCameraGridHeight - actualCameraGridHeight + 1;
		float calibrationGridCellWidth = ((float)actualStitchedFrameWidth) / globalCalibrationGridWidth;
		float calibrationGridCellHeight = ((float)actualStitchedFrameHeight) / globalCalibrationGridHeight;
		for (int i=0;i<size;i++)
		{
			int x = i % actualStitchedFrameWidth;
			int y = i / actualStitchedFrameWidth;
			int blockX = (int)(((float)x) / calibrationGridCellWidth);
			int blockY = (int)(((float)y) / calibrationGridCellHeight);
			for (int j=0;j<cameraCount;j++)
			{
				int cameraX = j % cameraGridWidth;
				int cameraY = j / cameraGridWidth;
				cameraX *= (actualCalibrationGridWidth-1);
				cameraY *= (actualCalibrationGridHeight-1);
				if ((blockX>=cameraX)&&(blockY>=cameraY)&&(blockX<cameraX+actualCalibrationGridWidth) && (blockY<cameraY+actualCalibrationGridHeight))
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
		int blockWidthSize = stitchedFrameWidth / cameraGridWidth;
		int blockHeightSize = stitchedFrameHeight / cameraGridHeight;
		for (int i=0;i<size;i++)
		{
			int x = i % stitchedFrameWidth;
			x/=blockWidthSize;
			if (x >= cameraGridWidth)
				x = cameraGridWidth - 1;
			int y = i / stitchedFrameWidth;
			y/=blockHeightSize;
			if (y >= cameraGridHeight)
				y = cameraGridHeight - 1;
			int index = y * cameraGridWidth + x;
			cameraMap[0][i]=(unsigned char)index;
		}
	}
}

void ofxMultiplexer::computeWeightMap(float* cameraWeightMap,vector2df* calibrationPoints,int cameraPosition)
{
	if (cameraWeightMap == NULL)
		cameraWeightMap = (float*)malloc(actualStitchedFrameWidth * actualStitchedFrameHeight * sizeof(float));
	int size = actualStitchedFrameWidth*actualStitchedFrameHeight;
	for (int i=0;i<size;i++)
		cameraWeightMap[i] = ((cameraMap[0][i] == cameraPosition) || 
			(cameraMap[1][i] == cameraPosition) || 
			(cameraMap[2][i] == cameraPosition) || 
			(cameraMap[3][i] == cameraPosition)) ? 1.0f : 0.0f;		
	int globalCalibrationGridWidth = actualCalibrationGridWidth * actualCameraGridWidth - actualCameraGridWidth + 1;
	int globalCalibrationGridHeight = actualCalibrationGridHeight * actualCameraGridHeight - actualCameraGridHeight + 1;
	float calibrationGridCellWidth = ((float)actualStitchedFrameWidth) / globalCalibrationGridWidth;
	float calibrationGridCellHeight = ((float)actualStitchedFrameHeight) / globalCalibrationGridHeight;
	float horizontalOffset =  1.0f / calibrationGridCellWidth;
	float verticalOffset = 1.0f / calibrationGridCellHeight;
	if (interleaveMode)
	{
		for (int i=0;i<size;i++)
		{
			if (((cameraMap[0][i] == cameraPosition) || 
				(cameraMap[1][i] == cameraPosition)) &&
				(cameraMap[1][i] != NULL_CAMERA) &&
				(cameraMap[2][i] == NULL_CAMERA) &&
				(cameraMap[3][i] == NULL_CAMERA))
			{
				unsigned int distance = INF;
				int closestX = 0;
				int closestY = 0;
				int X = i % actualStitchedFrameWidth;
				int Y = i / actualStitchedFrameWidth;
				for (int x=-calibrationGridCellWidth;x<=calibrationGridCellWidth;x++)
				{
					if ((X+x+(Y*actualStitchedFrameWidth)>=0) && (X+x+(Y*actualStitchedFrameWidth)<size))
					{
						if ((cameraMap[0][X+x+(Y*actualStitchedFrameWidth)] == cameraPosition) && 
							(cameraMap[1][X+x+(Y*actualStitchedFrameWidth)] == NULL_CAMERA) &&
							(cameraMap[2][X+x+(Y*actualStitchedFrameWidth)] == NULL_CAMERA) &&
							(cameraMap[3][X+x+(Y*actualStitchedFrameWidth)] == NULL_CAMERA))
							{
								if (abs(x)<distance)
								{
									distance = (unsigned int)abs(x);
									closestX = X+x;
									closestY = Y;
								}
							}
					}
				}
				
				for (int y=-calibrationGridCellHeight;y<calibrationGridCellHeight;y++)
				{
					if ((X+((Y+y)*actualStitchedFrameWidth)>=0) && (X+((Y+y)*actualStitchedFrameWidth)<size))
					{
						if ((cameraMap[0][X+((Y+y)*actualStitchedFrameWidth)] == cameraPosition) && 
							(cameraMap[1][X+((Y+y)*actualStitchedFrameWidth)] == NULL_CAMERA) &&
							(cameraMap[2][X+((Y+y)*actualStitchedFrameWidth)] == NULL_CAMERA) &&
							(cameraMap[3][X+((Y+y)*actualStitchedFrameWidth)] == NULL_CAMERA))
							{
								if (abs(y)<distance)
								{
									distance = (unsigned int)abs(y);
									closestX = X;
									closestY = Y+y;
								}
							}
					}
				}
				if (distance!=INF)
				{
					if (closestX == X)
						cameraWeightMap[i] = (verticalOffset * (calibrationGridCellHeight-distance));
					else
						if (closestY == Y)
							cameraWeightMap[i] = (horizontalOffset * (calibrationGridCellWidth-distance));
						else
							cameraWeightMap[i] = 0.0f;
				}
				else
					cameraWeightMap[i] = 0.0f;
			}
		}
		for (int i=0;i<size;i++)
		{
			if (((cameraMap[0][i] == cameraPosition) || 
				(cameraMap[1][i] == cameraPosition) ||
				(cameraMap[2][i] == cameraPosition) ||
				(cameraMap[3][i] == cameraPosition)) &&
				((cameraMap[0][i] != NULL_CAMERA) &&
				(cameraMap[1][i] != NULL_CAMERA) &&
				(cameraMap[2][i] != NULL_CAMERA) &&
				(cameraMap[3][i] != NULL_CAMERA)))
			{
				unsigned int distance = INF;
				int closestX = 0;
				int closestY = 0;
				int X = i % actualStitchedFrameWidth;
				int Y = i / actualStitchedFrameWidth;
				float result = 0.0f;
				for (int x=-calibrationGridCellWidth;x<=calibrationGridCellWidth;x++)
				{
					if ((X+x+(Y*actualStitchedFrameWidth)>=0) && (X+x+(Y*actualStitchedFrameWidth)<size))
					{
					if (((cameraMap[0][X+x+(Y*actualStitchedFrameWidth)] == cameraPosition) || 
						(cameraMap[1][X+x+(Y*actualStitchedFrameWidth)] == cameraPosition)) &&
						((cameraMap[0][X+x+(Y*actualStitchedFrameWidth)] != NULL_CAMERA) &&
						(cameraMap[1][X+x+(Y*actualStitchedFrameWidth)] != NULL_CAMERA) &&
						(cameraMap[2][X+x+(Y*actualStitchedFrameWidth)] == NULL_CAMERA) &&
						(cameraMap[3][X+x+(Y*actualStitchedFrameWidth)] == NULL_CAMERA)))
						{
							if (abs(x)<distance)
							{
								distance = (unsigned int)abs(x);
								closestX = X+x;
								closestY = Y;
							}
						}
					}
				}
				if (distance!=INF)
					result = cameraWeightMap[closestX+closestY*actualStitchedFrameWidth];
				distance = INF;
				for (int y=-calibrationGridCellHeight;y<calibrationGridCellHeight;y++)
				{
					if ((X+(Y+y)*actualStitchedFrameWidth>=0) && (X+(Y+y)*actualStitchedFrameWidth<size))
					{
					if (((cameraMap[0][X+((Y+y)*actualStitchedFrameWidth)] == cameraPosition) || 
						(cameraMap[1][X+((Y+y)*actualStitchedFrameWidth)] == cameraPosition)) &&
						((cameraMap[0][X+((Y+y)*actualStitchedFrameWidth)] != NULL_CAMERA) &&
						(cameraMap[1][X+((Y+y)*actualStitchedFrameWidth)] != NULL_CAMERA) &&
						(cameraMap[2][X+((Y+y)*actualStitchedFrameWidth)] == NULL_CAMERA) &&
						(cameraMap[3][X+((Y+y)*actualStitchedFrameWidth)] == NULL_CAMERA)))
						{
							if (abs(y)<distance)
							{
								distance = (unsigned int)abs(y);
								closestX = X;
								closestY = Y+y;
							}
						}
					}
				}
				cameraWeightMap[i] = result * cameraWeightMap[closestX+closestY*actualStitchedFrameWidth];
			}
		}
	}
}

void ofxMultiplexer::computeOffsetMap(unsigned int* cameraOffsetMap,vector2df* calibrationPoints,int cameraPosition)
{
	vector2df* screenPoints = (vector2df*)malloc((actualCalibrationGridWidth+1) * (actualCalibrationGridHeight+1) * sizeof(vector2df));
	vector2df* cameraPoints = (vector2df*)malloc((actualCalibrationGridWidth+1) * (actualCalibrationGridHeight+1) * sizeof(vector2df));
	int* triangles = (int*)malloc((actualCalibrationGridWidth+1)*(actualCalibrationGridHeight+1)*6*sizeof(int));

	int t = 0;
	for(int j = 0; j < actualCalibrationGridHeight; j++)
	{
		for(int i = 0; i < actualCalibrationGridWidth; i++)
		{
			triangles[t+0] = (i+0) + ((j+0) * (actualCalibrationGridWidth+1));
			triangles[t+1] = (i+1) + ((j+0) * (actualCalibrationGridWidth+1));
			triangles[t+2] = (i+0) + ((j+1) * (actualCalibrationGridWidth+1));
			t += 3;
			triangles[t+0] = (i+1) + ((j+0) * (actualCalibrationGridWidth+1));
			triangles[t+1] = (i+1) + ((j+1) * (actualCalibrationGridWidth+1));
			triangles[t+2] = (i+0) + ((j+1) * (actualCalibrationGridWidth+1));
			t += 3;
		}
	}

	int size = actualStitchedFrameWidth * actualStitchedFrameHeight;
	int LeftX = 0;
	int TopY = 0;
	for (int i=0;i<size;i++)
	{
		if ((cameraMap[0][i] == cameraPosition) ||
			(cameraMap[1][i] == cameraPosition) ||
			(cameraMap[2][i] == cameraPosition) ||
			(cameraMap[3][i] == cameraPosition))
		{
			LeftX = i % actualStitchedFrameWidth;
			TopY = i / actualStitchedFrameWidth;
			break;
		}
	}
	int globalCalibrationGridWidth = (interleaveMode ?  actualCalibrationGridWidth * actualCameraGridWidth - actualCameraGridWidth + 1 : actualCalibrationGridWidth * actualCameraGridWidth);
	int globalCalibrationGridHeight = (interleaveMode ? actualCalibrationGridHeight * actualCameraGridHeight - actualCameraGridHeight + 1 : actualCalibrationGridHeight * actualCameraGridHeight);
	float calibrationGridCellWidth = ((float)actualStitchedFrameWidth) / globalCalibrationGridWidth;
	float calibrationGridCellHeight = ((float)actualStitchedFrameHeight) / globalCalibrationGridHeight;
	int cameraX = cameraPosition % actualCameraGridWidth;
	int cameraY = cameraPosition / actualCameraGridWidth;
	cameraX *= (interleaveMode  ? (actualCalibrationGridWidth-1) : actualCalibrationGridWidth);
	cameraY *= (interleaveMode  ? (actualCalibrationGridHeight-1) : actualCalibrationGridHeight);
	int P = 0;
	for (int j=0;j<=actualCalibrationGridHeight;j++)
	{
		for (int i=0;i<=actualCalibrationGridWidth;i++)
		{
			screenPoints[P].X = (float)(calibrationGridCellWidth * i + LeftX);
			screenPoints[P].Y = (float)(calibrationGridCellHeight * j + TopY);
			P++;
		}
	}
	memcpy((void*)cameraPoints,calibrationPoints,(actualCalibrationGridWidth+1) * (actualCalibrationGridHeight+1) * sizeof(vector2df));
	memset(cameraOffsetMap,0,actualStitchedFrameWidth * actualStitchedFrameHeight * sizeof(unsigned int));
	
	int T = 0;
	for (int j=0;j<actualStitchedFrameHeight;j++)
	{
		for (int i=0;i<actualStitchedFrameWidth;i++)
		{
			unsigned int transformedX = i;
			unsigned int transformedY = j;
			if ((cameraMap[0][i+j*actualStitchedFrameWidth] != cameraPosition)&&
				(cameraMap[1][i+j*actualStitchedFrameWidth] != cameraPosition)&&
				(cameraMap[2][i+j*actualStitchedFrameWidth] != cameraPosition)&&
				(cameraMap[3][i+j*actualStitchedFrameWidth] != cameraPosition))
				transformedX = transformedY = 0;
			else
				cameraToScreenSpace(transformedX,transformedY,screenPoints,cameraPoints,triangles);
			if (transformedX >= cameraFramesWidth[cameraPosition])
				transformedX=cameraFramesWidth[cameraPosition]-1;
			if (transformedY >= cameraFramesHeight[cameraPosition])
				transformedY = cameraFramesHeight[cameraPosition]-1;
 			cameraOffsetMap[T] = transformedX+transformedY*cameraFramesWidth[cameraPosition];
			T++;
		}
	}
	free(screenPoints);
	free(cameraPoints);
	free(triangles);
}

void ofxMultiplexer::cameraToScreenSpace(unsigned int &x, unsigned int &y,vector2df* screenPoints,vector2df* cameraPoints,int* triangles)
{
	vector2df pt((float)x, (float)y);
	int t = findTriangleWithin(pt,screenPoints,triangles);
	if(t != -1)
	{
		vector2df A = screenPoints[triangles[t+0]];
		vector2df B = screenPoints[triangles[t+1]];
		vector2df C = screenPoints[triangles[t+2]];

		float total_area = (A.X - B.X) * (A.Y - C.Y) - (A.Y - B.Y) * (A.X - C.X);
		float area_A = (pt.X - B.X) * (pt.Y - C.Y) - (pt.Y - B.Y) * (pt.X - C.X);
		float area_B = (A.X - pt.X) * (A.Y - C.Y) - (A.Y - pt.Y) * (A.X - C.X);
		float bary_A = area_A / total_area;
		float bary_B = area_B / total_area;
		float bary_C = 1.0f - bary_A - bary_B;
		
		vector2df sA = cameraPoints[triangles[t+0]];
		vector2df sB = cameraPoints[triangles[t+1]];
		vector2df sC = cameraPoints[triangles[t+2]];

		vector2df transformedPos;

		transformedPos = (sA*bary_A) + (sB*bary_B) + (sC*bary_C);

		x = (unsigned int)transformedPos.X;
		y = (unsigned int)transformedPos.Y;
		return;
	}
	x = 0;
	y = 0;
}

void ofxMultiplexer::setInterleaveMode(bool isInterlaveMode)
{
	interleaveMode = isInterlaveMode;
}

void ofxMultiplexer::getInterleaveMode(bool* isInterleaveMode)
{
	*isInterleaveMode = interleaveMode;
}

bool ofxMultiplexer::isPointInTriangle(vector2df p, vector2df a, vector2df b, vector2df c)
{
	return (vector2df::isOnSameSide(p,a, b,c) && vector2df::isOnSameSide(p,b, a,c) && vector2df::isOnSameSide(p, c, a, b));
}

int ofxMultiplexer::findTriangleWithin(vector2df pt,vector2df* screenPoints,int* triangles)
{
	for(int t = 0; t < actualCalibrationGridWidth * actualCalibrationGridHeight * 6; t += 3)
	{
		if(isPointInTriangle(pt, screenPoints[triangles[t]], screenPoints[triangles[t+1]], screenPoints[triangles[t+2]]) )
			return t;
	}
	return -1;
}