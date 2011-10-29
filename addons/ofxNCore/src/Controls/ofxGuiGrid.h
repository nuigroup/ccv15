//! ofxGuiGrid.h
/*!
*  
*
*  Created by Yishi Guo on 06/05/2011.
*  Copyright 2011 NUI Group. All rights reserved.
*
*/

// ----------------------------------------------

#ifndef OFX_GUI_GRID
#define OFX_GUI_GRID

// ----------------------------------------------

#define GRID_WIDTH_SCALE 4
#define GRID_HEIGHT_SCALE 3
#define CAMERAS_ID_OFFSET 1000

// ----------------------------------------------

#include "ofxGuiTypes.h"
#include "ofxGuiObject.h"
#include "ofxGuiImage.h"
#include "ofxGuiButton.h"
#include "ofxMultiplexerManager.h"

// ----------------------------------------------

class ofxGuiGrid : public ofxGuiObject {
public:
	ofxGuiGrid();
	~ofxGuiGrid();

	void init( int id, string name, int x, int y, int width, int height, int xGrid, int yGrid, int border, int spacing, int mode );

	ofxGuiObject* addButton( int id, string name, int x, int y, int width, int height, bool value, int display );

	bool removeControl( int id );
	bool removeControls();

	void setXY( int x, int y );
	void setSelectedId( int index );
	void setMultiplexerManager( ofxMultiplexerManager* manager );
	void setOffset( int offset );
	void setMode( int mode, bool value = true );
	void setDraggingRawIndex( int index );
	void setImages();
	void resetAll();
	void setActive( bool active = true );
	void setDrawInfo( bool draw = true );
	void setDrawSelectedText( bool draw = true );
	void setShowResetBtn( bool show = true );
	void setResetBtnId( int id );
	void setShowSettingBtn( bool show = true );
	void setSettingBtnId( int id );
	void enableDblClickMode( bool enable = true );

	bool next();
	bool previous();

	float getGridX( int x );
	float getGridY( int y );
	float getGridWidth();
	float getGridHeight();
	float getDraggingXOffset();
	float getDraggingYOffset();
	int getIndexOffset();
	int getSelectedId();
	int getRawIdByDisplayId( int id );

	//! Return the first image
	ofxGuiImage* getFirstImage();

	ofxGuiObject* addImage( int id, string name, int targetId, unsigned char* image );

	bool update( int id, int task, void* data, int length );
	bool update();
	void draw();

	bool mouseDragged( int x, int y, int button );
	bool mousePressed( int x, int y, int button );
	bool mouseReleased( int x, int y, int button );

	void buildFromXml();
	void saveToXml();

	int mWidthScale, mHeightScale;
	float mGridWidth, mGridHeight;
	int mXGrid, mYGrid, mBorder, mSpacing;

	int mValue, mSelectedId;
	int mTotal;
	int mDisplayMode;

	float mColorR, mColorG, mColorB, mColorA;

	ofxGuiImage** gridImages;
	ofxMultiplexerManager* multiplexerManager;
	//bool* isSelected;

private:
	void calculateWH();
	void calculateDblClickImgWH( float &width, float &height );

	int  mouseToGridId( ofxPoint2f point );

	void drawSelectedRect( float x, float y, float width, float height );

	void clearSelectedColor();
	void selectedColor();
	float getColorR();
	float getColorG();
	float getColorB();
	float getColorA();


	void clearImages();
	void createImages();

	void setTitles();

	void switchDblClickMode( bool dblClick );

	int mIndexOffset;
	int mCamIndex;	//! index of all raw cams

	float mDraggingXOffset;
	float mDraggingYOffset;

	//! Selected color calculation
	unsigned long mOldTime;
	unsigned long mNowTime;
	int mInterval;
	float mOffset;
	bool mRising;

	////////////////
	//! Dragging
	bool mIsSelectable;
	bool mDragging;
	ofxPoint2f clickingPoint;
	bool mValidSelection;
	int mDraggingRawIndex;

	//////////////////
	bool mIsActive;

	bool bDrawSelectedText;

	//////////////////
	vector<ofxGuiObject*> mObjects;
	bool bShowResetBtn;
	int mResetBtnId;

	bool bShowSettingBtn;
	int mSettingBtnId;

	int rawIdArray[256];
	
	//////////////////
	bool bDblClickMode;
	bool bCanDblClickMode;
	unsigned long mPrevClickTime;
	unsigned long mNowClickTime;
	ofxGuiImage* dblClickImage;
};
// ----------------------------------------------
#endif