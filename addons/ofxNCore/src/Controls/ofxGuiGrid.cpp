//! ofxGuiGrid.cpp
/*!
*  
*
*  Created by Yishi Guo on 06/05/2011.
*  Copyright 2011 NUI Group. All rights reserved.
*
*/


#include "ofxGuiGrid.h"

ofxGuiGrid::ofxGuiGrid() 
{
	mParamType = kofxGui_Object_Grid;
	mWidthScale = GRID_WIDTH_SCALE;
	mHeightScale = GRID_HEIGHT_SCALE;
	mSelectedId = -1;
	mIndexOffset = 0;
	mOldTime = ofGetSystemTime();
	mInterval = 50;		// 50 ms
	mOffset = 0.05f;	// color changing offset
	mRising = true;
	mIsSelectable = false;
	mValidSelection = false;
	mDraggingRawIndex = -1;
	mIsActive = true;	// default: active
	bDrawSelectedText = false;
	bShowResetBtn = false;
	bShowSettingBtn = false;
	bDblClickMode = false;
	bCanDblClickMode = false;
	mPrevClickTime = 0;
	dblClickImage = new ofxGuiImage();
	multiplexerManager = NULL;
//	isSelected = NULL;
}
void ofxGuiGrid::init( int id, string name, int x, int y, int width, int height, int xGrid, int yGrid, int border, int spacing, int mode ) 
{
	int textHeight = (name == "") ? 0 : mGlobals->mParamFontHeight;
	mParamId		= id;
	mParamName		= name;
	mObjX			= x;
	mObjY			= y;
	mObjWidth		= width;
	mObjHeight		= textHeight + height;
	mXGrid			= xGrid;
	mYGrid			= yGrid;
	mBorder			= border;
	mSpacing		= spacing;
	mDisplayMode	= mode;
	calculateWH();
	setControlRegion( 0, textHeight, width, height );
	clearSelectedColor();
	createImages();
}
ofxGuiObject* ofxGuiGrid::addButton( int id, string name, int x, int y, int width, int height, bool value, int display ) 
{
	ofxGuiButton *button = new ofxGuiButton();
	button->init( id, name, x, y, width, height, value, display );
	mObjects.push_back( button );
	return button;
}
bool ofxGuiGrid::removeControl( int id ) 
{
	for (vector<ofxGuiObject*>::iterator it = mObjects.begin(); it < mObjects.end(); it++ ) 
	{
		if ((*it)->mParamId == id ) 
		{
			mObjects.erase( it );
			return true;
		}
	}
	return false;
}
bool ofxGuiGrid::removeControls() 
{
	while( !mObjects.empty()) 
	{
		mObjects.pop_back();
	}
	return true;
}
void ofxGuiGrid::setXY( int x, int y ) 
{
	if ( mDisplayMode == kofxGui_Grid_Display ) 
	{
		clearImages();
		mXGrid			= x;
		mYGrid			= y;
		calculateWH();
		setSelectedId( -1 );	// Clear the selected value
		createImages();
		removeControls();
		switchDblClickMode( false );
	}
}
void ofxGuiGrid::setSelectedId( int index ) 
{
	this->mSelectedId = index;
	clearSelectedColor();
	if ( mDisplayMode == kofxGui_Grid_List ) 
		this->mCamIndex = this->mSelectedId + this->mIndexOffset;
	else if ( mDisplayMode == kofxGui_Grid_Display ) 
		this->mCamIndex = this->multiplexerManager->getIndexOfCameraBase( multiplexerManager->getCameraBaseAtPosition(mSelectedId));
}
void ofxGuiGrid::setMultiplexerManager( ofxMultiplexerManager* multiplexer ) 
{
	if ( multiplexer == NULL ) 
		return;
	this->multiplexerManager = multiplexer;
	if ( mDisplayMode == kofxGui_Grid_List )
		setImages();
}
void ofxGuiGrid::setOffset( int offset ) 
{
	this->mIndexOffset = offset;
	this->setTitles();
	this->setImages();
}
void ofxGuiGrid::setMode( int mode, bool value ) 
{
	if (mode == kofxGui_Grid_Selectable) 
		mIsSelectable = value;
	else 
		mDisplayMode = mode;
}
void ofxGuiGrid::setDraggingRawIndex( int index ) 
{
	this->mDraggingRawIndex = index;
}
void ofxGuiGrid::resetAll() 
{
	if ( mDisplayMode == kofxGui_Grid_Display ) 
		this->setXY( mXGrid, mYGrid );
}
void ofxGuiGrid::setActive( bool active ) 
{
	this->mIsActive = active;
}
void ofxGuiGrid::setDrawInfo( bool draw ) 
{
	for ( int i = 0; i < mXGrid * mYGrid; ++i ) 
		gridImages[i]->setDrawInfo( draw );
}
void ofxGuiGrid::setDrawSelectedText( bool draw ) 
{
	bDrawSelectedText = draw;
	setImages();
}
void ofxGuiGrid::setShowResetBtn( bool show ) 
{
	bShowResetBtn = show;
}
void ofxGuiGrid::setResetBtnId( int id ) 
{
	mResetBtnId = id;
}
void ofxGuiGrid::setShowSettingBtn( bool show ) 
{
	bShowSettingBtn = show;
}
void ofxGuiGrid::setSettingBtnId( int id ) 
{
	mSettingBtnId = id;
}
void ofxGuiGrid::enableDblClickMode( bool enable ) 
{
	bCanDblClickMode = enable;
}
bool ofxGuiGrid::next() 
{
	if ( mDisplayMode == kofxGui_Grid_List ) 
	{
		if ( mXGrid * mYGrid + mIndexOffset + 1 <= multiplexerManager->getCameraBaseCount()) 
		{
			mIndexOffset++;
			setOffset( mIndexOffset );
			return true;
		}
	}
	return false;
}
bool ofxGuiGrid::previous() 
{
	if (mDisplayMode == kofxGui_Grid_List ) 
	{
		if ( mIndexOffset - 1 >= 0 ) 
		{
			mIndexOffset--;
			setOffset( mIndexOffset );
			return true;
		}
	}
	return false;
}
bool ofxGuiGrid::update() 
{
	return true;
}
void ofxGuiGrid::draw() 
{
	//mGlobals->mCoverColor.a = 0.0f;
	selectedColor();
	glPushMatrix();
	glTranslatef( mObjX, mObjY, 0.0f );
	if ( bDblClickMode ) 
		this->dblClickImage->draw();
	else 
	{
		if ( mParamName != "" )
			drawParamString( 0.0, 0.0, mParamName, false );
		ofFill();
		ofNoFill();
		for ( int j = 0; j < mYGrid; j++ ) 
		{
			for ( int i = 0; i < mXGrid; i++ ) 
			{
				int index = i + j * mXGrid;
				ofNoFill();
				if ( mSelectedId == index ) 
					drawSelectedRect( getGridX(i), getGridY(j), getGridWidth(), getGridHeight() );
				else 
				{
					glColor4f( mGlobals->mFrameColor.r, mGlobals->mFrameColor.g, mGlobals->mFrameColor.b, mGlobals->mFrameColor.a );
					ofRect( getGridX(i), getGridY(j), getGridWidth(), getGridHeight() );
				}
				if ( gridImages[index] != NULL )
					gridImages[index]->draw();
			}
		}
	}
	ofNoFill();
	glColor4f( mGlobals->mFrameColor.r, mGlobals->mFrameColor.g, mGlobals->mFrameColor.b, mGlobals->mFrameColor.a );
	ofRect( mCtrX, mCtrY, mCtrWidth, mCtrHeight );
	glPopMatrix();
	if ( !bDblClickMode ) 
	{
		for ( int i = 0; i < mObjects.size(); ++i ) 
			mObjects[i]->draw();
	}
}

bool ofxGuiGrid::update( int id, int task, void* data, int length ) 
{
	bool handled = false;
	if ( id == mParamId ) 
	{
		handled = true;
		mColorB += 0.1;
	}
	return handled;
}
bool ofxGuiGrid::mouseDragged( int x, int y, int button ) 
{
	if ( !mIsActive ) 
		return false;
	ofxPoint2f inside = mouseToLocal( x, y );
	mMouseIsDown = isPointInsideMe( inside );
	if ( mMouseIsDown ) 
	{
		if ( bDblClickMode ) 
			return true;
		if ( mIsSelectable && mValidSelection) 
		{
			int id = mouseToGridId( inside );
			//test

			//multiplexerManager->getPositionOfCameraBase(multiplexerManager->getCameraBase(index))<0
			if ( id + mIndexOffset < multiplexerManager->getCameraBaseCount() && id != -1 && (multiplexerManager->getPositionOfCameraBase(multiplexerManager->getCameraBase(id + mIndexOffset ))<0))
			{
				if ( !mDragging ) 
				{
					this->mDraggingXOffset = clickingPoint.x - getGridX( id );
					this->mDraggingYOffset = clickingPoint.y - getGridY( 0 );	//! just one "line", so y == 0
					mDragging = true;	
				}
				mGlobals->mListener->handleGui( mParamId, kofxGui_Set_Grid_Dragging, &mCamIndex, sizeof(int) );
			}
		}
	}
	return mMouseIsDown;
}

// ----------------------------------------------

bool ofxGuiGrid::mousePressed( int x, int y, int button ) 
{
	if ( !mIsActive )
		return false;
	ofxPoint2f inside = mouseToLocal( x, y );
	mMouseIsDown = isPointInsideMe( inside );
	if ( mMouseIsDown ) 
	{
		if ( bDblClickMode ) 
		{
			mNowClickTime = ofGetSystemTime();
			if ( mNowClickTime - mPrevClickTime <= OFXGUI_DBLCLICK_INTERVAL ) 
			{
				mPrevClickTime = 0;
				switchDblClickMode( false );
				return true;
			}
			mPrevClickTime = mNowClickTime;
			return true;
		}
		bool handled = false;
		for ( int i = 0; i < mObjects.size(); i++ ) 
		{
			handled = (ofxGuiObject*)mObjects[i]->mousePressed( x, y, button );
			if ( handled ) 
				return true;
		}
		int id = mouseToGridId( inside );
		setSelectedId( id );
		mGlobals->mListener->handleGui( mParamId, kofxGui_Set_Int, &mCamIndex, sizeof(int) );
		mDragging = false;
		if ( id != -1 ) 
		{
			clickingPoint = inside;
			mValidSelection = true;
		} 
		else 
			mValidSelection = false;
		mNowClickTime = ofGetSystemTime();
		if ( id != -1 && (multiplexerManager->getCameraBaseAtPosition( id )!=NULL) && mNowClickTime - mPrevClickTime <= OFXGUI_DBLCLICK_INTERVAL ) 
		{
			mPrevClickTime = 0;
			switchDblClickMode( true );
			return true;
		}
		mPrevClickTime = mNowClickTime;
		int x, y, textWidth;
		string btnText = "";
		ofxGuiButton* btn;
		//glColor4f( 0.9f, 0.9f, 0.9f, 1.0f );

		if ( bShowResetBtn ) 
		{
			removeControl( mResetBtnId );
			if ( id != -1 && (multiplexerManager->getCameraBaseAtPosition( id ))) 
			{
				//! The position is relate with the panel
				x = mObjX + getGridX( id % mXGrid ) + 5;
				y = mObjY + getGridY( id / mXGrid ) + getGridHeight() - 5 - OFXGUI_BUTTON_HEIGHT;
				btnText = "Clear";
				textWidth = mGlobals->mParamFont.stringWidth( btnText );
				if ( getGridWidth() < textWidth + 5 + OFXGUI_BUTTON_HEIGHT ) 
					btnText = "";
				btn = (ofxGuiButton*)addButton( mResetBtnId, btnText, x, y,
					OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
					kofxGui_Button_Off, kofxGui_Button_Trigger );
				btn->setHighlightMode( true );
			}
		}
		if ( bShowSettingBtn ) 
		{
			removeControl( mSettingBtnId );
			if ( id != -1 && (multiplexerManager->getCameraBaseAtPosition( id )) ) 
			{
				y -= 15;
				btnText = "Config";
				textWidth = mGlobals->mParamFont.stringWidth( btnText );
				if ( getGridWidth() < textWidth + 5 + OFXGUI_BUTTON_HEIGHT ) 
					btnText = "";
				btn = (ofxGuiButton*)addButton( mSettingBtnId, btnText, x, y,
					OFXGUI_BUTTON_HEIGHT, OFXGUI_BUTTON_HEIGHT,
					kofxGui_Button_Off, kofxGui_Button_Trigger );
				btn->setHighlightMode( true );				
			}
		}
	}
	return mMouseIsDown;
}
bool ofxGuiGrid::mouseReleased( int x, int y, int button ) 
{
	if ( !mIsActive )
		return false;
	bool handled = mMouseIsDown;
	if ( mMouseIsDown ) 
	{
		mMouseIsDown = false;
		if ( bDblClickMode )
			return true;
		//! Handle the controls
		bool handled2 = false;
		for ( int i = 0; i < mObjects.size(); ++i ) 
		{
			if ((ofxGuiObject*)mObjects[i]->mouseReleased(x,y,button))
				return true;
		}
	}
	if ( mDisplayMode == kofxGui_Grid_Display && mDraggingRawIndex != -1) 
	{
		ofxPoint2f inside = mouseToLocal(x,y);
		if (isPointInsideMe(inside)) 
		{
			int id = mouseToGridId(inside);
			if ( id < mXGrid * mYGrid && id != -1 ) 
			{
				printf( "\nofxGuiGrid::mouseReleased()\tid=%d\n", id );
				printf( "\nofxGuiGrid::mouseReleased()\tmDraggingRawIndex=%d\n", mDraggingRawIndex );
				if ( multiplexerManager->getCameraBaseAtPosition( id ) == NULL ) 
				{
					multiplexerManager->addCameraBaseToMultiplexerAtPosition( id, multiplexerManager->getCameraBase(mDraggingRawIndex) );
					//isSelected[mDraggingRawIndex] = true;;
					setImages();
					rawIdArray[id] = mDraggingRawIndex;
					mGlobals->mListener->handleGui( mParamId, kofxGui_Set_Grid_Released, &mDraggingRawIndex, sizeof(int) );
				}
			}
			mDraggingRawIndex = -1;
		}
	}
	return handled;
}
void ofxGuiGrid::buildFromXml() 
{
	// TODO
	mGlobals->mListener->handleGui( mParamId, kofxGui_Set_Int, &mValue, sizeof(int) );
}
void ofxGuiGrid::saveToXml() 
{
	// TODO
	saveObjectData();
}
void ofxGuiGrid::calculateWH() 
{
	float w, h, unit;
	w = ( (mObjWidth - 2 * mBorder - (mXGrid - 1) * mSpacing ) / mXGrid ) / mWidthScale;
	h = ( (mObjHeight - 2 * mBorder - (mYGrid - 1) * mSpacing ) / mYGrid ) / mHeightScale;
	unit = w > h ? h : w;
	mGridWidth = unit * mWidthScale;
	mGridHeight = unit * mHeightScale;
}
void ofxGuiGrid::calculateDblClickImgWH( float &width, float &height ) 
{
	float w, h, unit;
	w = mObjWidth / mWidthScale;
	h = mObjHeight / mHeightScale;
	unit = w > h ? h : w;
	width = unit * mWidthScale;
	height = unit * mHeightScale;
}
void ofxGuiGrid::drawSelectedRect( float x, float y, float width, float height ) 
{
	glShadeModel( GL_SMOOTH );
	glBegin( GL_LINE_LOOP );
	glColor4f( getColorR(), getColorG(), getColorB(), getColorA() );
	glVertex3f( x, y, 0 );
	glVertex3f( x, y + height, 0 );
	glVertex3f( x + width, y + height, 0 );
	glVertex3f( x + width, y, 0 );
	glEnd();
}
int ofxGuiGrid::mouseToGridId( ofxPoint2f p ) 
{
	int index = 0;
	for ( int j = 0; j < mYGrid; ++j ) 
	{
		for ( int i = 0; i < mXGrid; ++i ) 
		{
			int index = i + j * mXGrid;
			if ( p.x >= getGridX(i) &&
				p.x <= getGridX(i) + getGridWidth() &&
				p.y >= getGridY(j) &&
				p.y <= getGridY(j) + getGridHeight() ) 
			{
					return index;	// return the index of grid
			}
		}
	}
	return -1;	//! No id found
}
float ofxGuiGrid::getGridX( int i ) 
{
	return mCtrX + mBorder + i * (mSpacing + mGridWidth );
}
float ofxGuiGrid::getGridY( int i ) 
{
	return mCtrY + mBorder + i * (mSpacing + mGridHeight);
}
float ofxGuiGrid::getGridWidth() 
{
	return this->mGridWidth;
}
float ofxGuiGrid::getGridHeight() 
{
	return this->mGridHeight;
}
float ofxGuiGrid::getDraggingXOffset() 
{
	return this->mDraggingXOffset;
}
float ofxGuiGrid::getDraggingYOffset() 
{
	return this->mDraggingYOffset;
}
int ofxGuiGrid::getIndexOffset() 
{
	return this->mIndexOffset;
}
int ofxGuiGrid::getSelectedId() 
{
	return this->mSelectedId;
}
int ofxGuiGrid::getRawIdByDisplayId( int id ) 
{
	if ( id >= mXGrid * mYGrid || id < 0 )
		return -1;
	return rawIdArray[id];
}
ofxGuiImage* ofxGuiGrid::getFirstImage() 
{
	if ( mIndexOffset >= 0	&& mIndexOffset + 0 < multiplexerManager->getCameraBaseCount() ) 
		return this->gridImages[0];
	return NULL;
}
void ofxGuiGrid::clearSelectedColor() 
{
	mColorR = 0;
	mColorG = 0;
	mColorB = 0;
	mColorA = 1;
}
void ofxGuiGrid::selectedColor() 
{
	mNowTime = ofGetSystemTime();
	if ( mNowTime - mOldTime >= mInterval ) 
	{
		mOldTime = mNowTime;
		if ( mRising ) 
		{
			mColorR += mOffset;
			mColorG += mOffset;
			mColorB += mOffset;
		} 
		else 
		{
			mColorR -= mOffset;
			mColorG -= mOffset;
			mColorB -= mOffset;
		}
		if ( mColorR >= 1 )
			mRising = false;
		else if ( mColorR <= 0 )
			mRising = true;
		// printf( "\nR: %f\n", mColorR );
	}
}
float ofxGuiGrid::getColorR() 
{
	return mColorR;
}
float ofxGuiGrid::getColorG() 
{
	return mColorG;
}
float ofxGuiGrid::getColorB() 
{
	return mColorB;
}
float ofxGuiGrid::getColorA() 
{
	return mColorA;
}
void ofxGuiGrid::clearImages() 
{
	for ( int i = 0; i < mXGrid * mYGrid; ++i ) 
		delete gridImages[i];
}
void ofxGuiGrid::createImages() 
{
	//! Allocate the space
	gridImages = new ofxGuiImage*[mXGrid*mYGrid];
	int index = 0;
	//! Setup each image
	for ( int j = 0; j < mYGrid; ++j ) 
	{
		for ( int i = 0; i < mXGrid; ++i ) 
		{
			index = i + j * mXGrid;
			gridImages[index] = new ofxGuiImage();
			gridImages[index]->init( CAMERAS_ID_OFFSET + index, " " + ofToString(index+1), getGridX(i) + 1, getGridY(j) + 1, roundInt(mGridWidth - 2), roundInt(mGridHeight - 2)  );
		}
	}
}

void ofxGuiGrid::setImages() 
{
	if ( mDisplayMode == kofxGui_Grid_List ) 
	{	//! For list mode
		for ( int i = 0; i < mXGrid * mYGrid; ++i ) 
		{
			if ( i + mIndexOffset < multiplexerManager->getCameraBaseCount() ) 
			{
				//printf( "\nofxGuiGrid:setImages()\t i < utils->getCount()\n" );
				try 
				{
					if (gridImages[i] !=NULL)
					{
						delete gridImages[i];
						gridImages[i] = new ofxGuiImage();
						gridImages[i]->init( CAMERAS_ID_OFFSET + i, " Camera " + ofToString(i+mIndexOffset+1), getGridX(i%mXGrid) + 1, getGridY(i/mXGrid) + 1, roundInt(mGridWidth - 2), roundInt(mGridHeight - 2)  );
					}
					gridImages[i]->setCamera( multiplexerManager->getCameraBase( i + mIndexOffset ) );

					//test!!! different size
				} 
				catch( ... ) 
				{
				}
				gridImages[i]->setCamSelected(bDrawSelectedText ? (multiplexerManager->getPositionOfCameraBase(multiplexerManager->getCameraBase(i + mIndexOffset))>=0): false );
			} 
			else 
				gridImages[i]->setBlank();
		}
	} 
	else if ( mDisplayMode == kofxGui_Grid_Display ) 
	{
		for ( int i = 0; i < mXGrid * mYGrid; ++i ) 
		{
			try 
			{
				if (gridImages[i] !=NULL)
				{
					delete gridImages[i];
					gridImages[i] = new ofxGuiImage();
					gridImages[i]->init( CAMERAS_ID_OFFSET + i, " " + ofToString(i+1), getGridX(i%mXGrid) + 1, getGridY(i/mXGrid) + 1, roundInt(mGridWidth - 2), roundInt(mGridHeight - 2)  );
				}
				gridImages[i]->setCamera(multiplexerManager->getCameraBaseAtPosition(i));

			} 
			catch ( ... ) 
			{
			}
		}
	}
}
void ofxGuiGrid::setTitles() {
	for ( int i = 0; i < mXGrid * mYGrid; ++i ) 
		gridImages[i]->setTitle( " " + ofToString( i + mIndexOffset ) );
}
void ofxGuiGrid::switchDblClickMode( bool dblClick ) 
{
	printf( "\nofxGuiGrid::switchDblClickMode\n" );
	float width, height;
	if ( dblClick && bCanDblClickMode ) 
	{
		bDblClickMode = true;
		calculateDblClickImgWH( width, height );
		if (dblClickImage != NULL)
		{
			delete dblClickImage;
			dblClickImage = new ofxGuiImage();
		}
		dblClickImage->init(
			CAMERAS_ID_OFFSET + 101, " Camera " + ofToString(mSelectedId+1) + " Preview",
			mCtrX + 1, mCtrY + 1, (int)width - 2, (int)height - 2 );
		dblClickImage->setCamera( multiplexerManager->getCameraBaseAtPosition( mSelectedId ) );
		dblClickImage->setDrawInfo( true );
	} 
	else 
		bDblClickMode = false;
}
