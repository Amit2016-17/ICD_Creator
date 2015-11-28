/**************************************************************************
 *  This file is part of QXmlEdit                                         *
 *  Copyright (C) 2011 by Luca Bellonda and individual contributors       *
 *    as indicated in the AUTHORS file                                    *
 *  lbellonda _at_ gmail.com                                              *
 *                                                                        *
 * This library is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU Library General Public            *
 * License as published by the Free Software Foundation; either           *
 * version 2 of the License, or (at your option) any later version.       *
 *                                                                        *
 * This library is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      *
 * Library General Public License for more details.                       *
 *                                                                        *
 * You should have received a copy of the GNU Library General Public      *
 * License along with this library; if not, write to the                  *
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,       *
 * Boston, MA  02110-1301  USA                                            *
 **************************************************************************/

#include "paintinfo.h"
#include "config.h"

#define MAX_ZOOM    (5)
#define MIN_ZOOM    (1)

PaintInfo::PaintInfo()
{
    isChanged = false;
    isOneAttrPerLine = false ;
    isIndexPerChild = false ;
    isCompactView = false;
    isShowAttributesLength = false ;
    isUseFixedLengthFont = false ;
    isShowUnBase64 = false;
    _currentStyle = NULL ;
    zoomFactor = 1 ;
    isShowElementTextLength = false ;
    isShowElementSize = false ;
    isExplore = false ;
    isAttributesHTML = false ;
    isHideView = false ;
    recalcColumns();
}

PaintInfo::~PaintInfo()
{
}

void PaintInfo::loadState()
{
    isIndexPerChild = Config::getBool(Config::KEY_MAIN_INDEXPERCHILD, false);
    isOneAttrPerLine = Config::getBool(Config::KEY_MAIN_ONEATTRLINE, false);
    isCompactView = Config::getBool(Config::KEY_MAIN_COMPACTVIEW, false);
    isShowAttributesLength = Config::getBool(Config::KEY_MAIN_SHOWATTRLEN, false);
    isUseFixedLengthFont = Config::getBool(Config::KEY_MAIN_ATTRFIXEDS, false);
    isShowUnBase64  = Config::getBool(Config::KEY_MAIN_SHOWBASE64, false);
    isShowElementTextLength = Config::getBool(Config::KEY_MAIN_SHOWELTEXTLEN, false);
    isShowElementSize = Config::getBool(Config::KEY_MAIN_SHOWELSIZE, false);
    isHideView = Config::getBool(Config::KEY_MAIN_HIDEVIEW, false);
    internalSetZoom(Config::getInt(Config::KEY_MAIN_SHOWZOOM, 1));
    recalcColumns();
    isChanged = false;
}

bool PaintInfo::saveState()
{
    if(!isChanged) {
        return true;
    }
    bool isOK = true ;
    if(!Config::saveBool(Config::KEY_MAIN_INDEXPERCHILD, indexPerChild()))
        isOK = false;
    if(!Config::saveBool(Config::KEY_MAIN_ONEATTRLINE, oneAttrPerLine()))
        isOK = false;
    if(!Config::saveBool(Config::KEY_MAIN_COMPACTVIEW, compactView()))
        isOK = false;
    if(!Config::saveBool(Config::KEY_MAIN_SHOWATTRLEN, showAttributesLength()))
        isOK = false;
    if(!Config::saveBool(Config::KEY_MAIN_ATTRFIXEDS, userFixedLengthFont()))
        isOK = false;
    if(!Config::saveBool(Config::KEY_MAIN_SHOWBASE64, showUnBase64()))
        isOK = false;
    if(!Config::saveBool(Config::KEY_MAIN_SHOWELTEXTLEN, showItemTextLength()))
        isOK = false;
    if(!Config::saveBool(Config::KEY_MAIN_SHOWELSIZE, showItemSize()))
        isOK = false;
    if(!Config::saveInt(Config::KEY_MAIN_SHOWZOOM, zoom()))
        isOK = false;
    if(!Config::saveBool(Config::KEY_MAIN_HIDEVIEW, hideView()))
        isOK = false;

    return isOK;
}


void PaintInfo::setOneAttrPerLine(const bool newValue)
{
    isOneAttrPerLine = newValue ;
    isChanged = true ;
    saveState();
}

bool PaintInfo::oneAttrPerLine() const
{
    return isOneAttrPerLine;
}

void PaintInfo::setIndexPerChild(const bool newValue)
{
    isIndexPerChild = newValue ;
    isChanged = true ;
    saveState();
}

void PaintInfo::setCompactView(const bool newValue)
{
    isCompactView = newValue ;
    isChanged = true ;
    saveState();
}

bool PaintInfo::indexPerChild() const
{
    return isIndexPerChild ;
}

bool PaintInfo::compactView() const
{
    return isCompactView ;
}

bool PaintInfo::showAttributesLength() const
{
    return isShowAttributesLength ;
}

void PaintInfo::setShowAttributesLength(const bool newValue)
{
    isShowAttributesLength  = newValue ;
    isChanged = true ;
    saveState();
}

bool PaintInfo::userFixedLengthFont() const
{
    return isUseFixedLengthFont ;
}

void PaintInfo::setUserFixedLengthFont(const bool newValue)
{
    isUseFixedLengthFont = newValue ;
    isChanged = true ;
    saveState();
}

void PaintInfo::setCurrentStyle(VStyle *newStyle)
{
    _currentStyle = newStyle ;
}

VStyle *PaintInfo::currentStyle() const
{
    return _currentStyle ;
}

bool PaintInfo::showUnBase64() const
{
    return isShowUnBase64 ;
}

void PaintInfo::setShowUnBase64(const bool newValue)
{
    isShowUnBase64 = newValue ;
    isChanged = true ;
    saveState();
}

int PaintInfo::zoom() const
{
    return zoomFactor ;
}

void PaintInfo::setZoom(const int newValue)
{
    internalSetZoom(newValue);
    isChanged = true ;
    saveState();
}

void PaintInfo::internalSetZoom(const int newValue)
{
    if(newValue > MAX_ZOOM) {
        zoomFactor = MAX_ZOOM;
    } else if(zoomFactor < MIN_ZOOM) {
        zoomFactor = MIN_ZOOM ;
    } else {
        zoomFactor = newValue ;
    }
}

bool PaintInfo::zoomIn()
{
    if(zoomFactor < MAX_ZOOM) {
        zoomFactor++;
        if(NULL != _currentStyle) {
            _currentStyle->setZoom(zoomFactor);
        }
        return true ;
    }
    return false ;
}

bool PaintInfo::zoomOut()
{
    if(zoomFactor > MIN_ZOOM) {
        zoomFactor--;
        if(NULL != _currentStyle) {
            _currentStyle->setZoom(zoomFactor);
        }
        return true;
    }
    return false ;
}

bool PaintInfo::canZoomOut()
{
    return (zoomFactor > MIN_ZOOM);
}

bool PaintInfo::canZoomIn()
{
    return (zoomFactor < MAX_ZOOM);
}

bool PaintInfo::showItemTextLength() const
{
    return isShowElementTextLength ;
}

void PaintInfo::setShowItemTextLength(const bool newValue)
{
    isShowElementTextLength  = newValue ;
    isChanged = true ;
    saveState();
}

bool PaintInfo::showItemSize() const
{
    return isShowElementSize ;
}

void PaintInfo::setShowItemSize(const bool newValue)
{
    isShowElementSize  = newValue ;
    recalcColumns() ;
    isChanged = true ;
    saveState();
}

void PaintInfo::setExploreMode(const bool newIsExplore)
{
    isExplore = newIsExplore;
}

bool PaintInfo::exploreMode() const
{
    return isExplore ;
}

void PaintInfo::recalcColumns()
{
    columnForTag = 0 ;

    //if( isShowElementSize ) {
    columnForChildren = 1;
    columnForSize = 2;
    columnForAttributes = 3;
    columnForText = 4;
    columnForDecodedText = 5;
    /*} else {
        //TODO
        columnForChildren = -1;
        columnForSize = -2;

        columnForAttributes = 1;
        columnForText = 2;
        columnForDecodedText = 3;
    }*/
}

void PaintInfo::setAttributesHTML(const bool value)
{
    isAttributesHTML = value ;
}

bool PaintInfo::attributesHTML() const
{
    return isAttributesHTML;
}


bool PaintInfo::hideView() const
{
    return isHideView ;
}

void PaintInfo::setHideView(const bool value)
{
    isHideView = value ;
    isChanged = true ;
    saveState();
}

