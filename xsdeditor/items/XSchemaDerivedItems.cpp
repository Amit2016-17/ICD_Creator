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


#include "xsdeditor/xsdwindow.h"
#include "utils.h"
#include <QGraphicsTextItem>



//-------------------------------------------------------------------------------


RestrictionItem::RestrictionItem(XsdGraphicContext *newContext, XSchemaObject *newItem, QGraphicsItem * parent)
    : XSDItem(newContext), _item(NULL), _graphicsItem(this), _facets(NULL)
{
    parent->scene()->addItem(&_graphicsItem);
    _graphicsItem.setData(XSD_ITEM_DATA, qVariantFromValue((void*)this));
    init();
    setItem(newItem);
}

RestrictionItem::~RestrictionItem()
{
    reset();
}

void RestrictionItem::reset()
{
}

void RestrictionItem::init()
{
    QPainterPath path;
    path.moveTo(0, 30);
    path.lineTo(30, 0);
    path.lineTo(155, 0);
    path.lineTo(155, 60);
    path.lineTo(30, 60);
    path.lineTo(0, 30);
    _contour = path.toFillPolygon();
    _graphicsItem.setPolygon(_contour);
    _graphicsItem.setFlag(QGraphicsItem::ItemIsMovable, false);
    _graphicsItem.setFlag(QGraphicsItem::ItemIsSelectable, true);
    _graphicsItem.setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    _graphicsItem.setPos(0, 0);

    QLinearGradient gradient(0, 0, 0, 100);
    gradient.setColorAt(0, QColor::fromRgbF(0, .7, .4));
    gradient.setColorAt(1, QColor::fromRgbF(0, .7, 1));

    _graphicsItem.setBrush(QBrush(gradient));

    _name = new QGraphicsTextItem(&_graphicsItem);
    if(NULL != _name) {
        _name->setPos(10, 5);
        _name->setParentItem(&_graphicsItem);
    }

    _facets = new QGraphicsTextItem(&_graphicsItem);
    if(NULL != _facets) {
        _facets->setPos(60, 5);
    }
    connect(&_graphicsItem, SIGNAL(itemChanged(QGraphicsItem::GraphicsItemChange, const QVariant&)), this, SLOT(itemChanged(QGraphicsItem::GraphicsItemChange, const QVariant&)));
}

XSchemaObject *RestrictionItem::restriction() const
{
    return _item ;
}

void RestrictionItem::setItem(XSchemaObject *newItem)
{
    QString facetsText ;
    if(_item != newItem) {
        if(NULL != _item) {
            disconnect(_item, SIGNAL(childAdded(XSchemaObject*)), this, SLOT(childAdded(XSchemaObject*)));
            //TODO disconnect(_item, SIGNAL(childRemoved(XSchemaObject*)), this, SLOT(childRemoved(XSchemaObject*)));
        }
        _item = newItem ;
        if(NULL != newItem) {
            connect(_item, SIGNAL(childAdded(XSchemaObject*)), this, SLOT(childAdded(XSchemaObject*)));
            //TODO connect(_item, SIGNAL(childRemoved(XSchemaObject*)), this, SLOT(childRemoved(XSchemaObject*)));
            foreach(XSchemaObject * child, _item->getChildren()) {
                childAdded(child);
            }
            facetsText = newItem->description();
        }
    }
    _facets->setPlainText(facetsText);
    if(!facetsText.isEmpty()) {
        _facets->setVisible(true);
    } else {
        _facets->setVisible(false);
    }
    QGraphicsItem * items []  = {_facets, _name } ;
    QRectF size = measureOptimumDimensions(2, items) ;
    qreal width = size.x() + size.width();
    qreal height = size.y() + size.height();
    QPainterPath path;
    path.moveTo(0, height / 2);
    path.lineTo(30, 0);
    path.lineTo(width + 20, 0);
    path.lineTo(width + 20, height + 10);
    path.lineTo(30, height + 10);
    _contour = path.toFillPolygon();
    _graphicsItem.setPolygon(_contour);

}

void RestrictionItem::itemChanged(QGraphicsItem::GraphicsItemChange change, const QVariant & /*value*/)
{
    if(change == QGraphicsItem::ItemPositionChange) {
        RChild *theChain = chain();
        if(NULL != theChain) {
            chain()->updatePosition();
            updateScenePosition(&_graphicsItem);
            foreach(RChild * child, _children.children()) {
                child->updatePosition();
            }
        }
    }
}

void RestrictionItem::childAdded(XSchemaObject *newChild)
{
    if(NULL == newChild) {
        return ;
    }
    XSDItem *child = addChild(newChild);
    if(NULL == child) {
        Utils::error(tr("An error occurred inserting the graphic item corresponding to the object."));
    }
}

//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------


UnionItem::UnionItem(XsdGraphicContext *newContext, XSchemaSimpleTypeUnion *newItem, QGraphicsItem * parent)
    : XSDItem(newContext), _item(NULL), _graphicsItem(this)
{
    parent->scene()->addItem(&_graphicsItem);
    _graphicsItem.setData(XSD_ITEM_DATA, qVariantFromValue((void*)this));
    init();
    setItem(newItem);
}

UnionItem::~UnionItem()
{
    reset();
}

void UnionItem::reset()
{
}

void UnionItem::init()
{
    QPainterPath path;
    path.moveTo(15, 60);
    path.lineTo(30, 60);
    path.lineTo(45, 30);
    path.lineTo(30, 0);
    path.lineTo(15, 0);
    path.lineTo(0, 30);

    _contour = path.toFillPolygon();
    _graphicsItem.setPolygon(_contour);
    _graphicsItem.setFlag(QGraphicsItem::ItemIsMovable, false);
    _graphicsItem.setFlag(QGraphicsItem::ItemIsSelectable, true);
    _graphicsItem.setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    _graphicsItem.setPos(0, 0);

    QLinearGradient gradient(0, 0, 0, 100);
    gradient.setColorAt(0, QColor::fromRgbF(0, 1, .4, .8));
    gradient.setColorAt(1, QColor::fromRgbF(0, 1, 1, .5));

    _graphicsItem.setBrush(QBrush(gradient));

    QGraphicsSimpleTextItem *pName = new QGraphicsSimpleTextItem(&_graphicsItem);
    if(NULL != pName) {
        pName->setText(tr("Union"));
        pName->setPos(10, 0);
        _graphicsItem.childItems().append(pName);
    }

    connect(&_graphicsItem, SIGNAL(itemChanged(QGraphicsItem::GraphicsItemChange, const QVariant&)), this, SLOT(itemChanged(QGraphicsItem::GraphicsItemChange, const QVariant&)));
}

XSchemaSimpleTypeUnion *UnionItem::xunion() const
{
    return _item ;
}

void UnionItem::setItem(XSchemaObject *newItem)
{
    if(_item != newItem) {
        if(NULL != _item) {
            disconnect(_item, SIGNAL(childAdded(XSchemaObject*)), this, SLOT(childAdded(XSchemaObject*)));
            //TODO disconnect(_item, SIGNAL(childRemoved(XSchemaObject*)), this, SLOT(childRemoved(XSchemaObject*)));
        }
        _item = (XSchemaSimpleTypeUnion*)newItem ;
        if(NULL != newItem) {
            connect(_item, SIGNAL(childAdded(XSchemaObject*)), this, SLOT(childAdded(XSchemaObject*)));
            //TODO connect(_item, SIGNAL(childRemoved(XSchemaObject*)), this, SLOT(childRemoved(XSchemaObject*)));
            foreach(XSchemaObject * child, _item->getChildren()) {
                childAdded(child);
            }

        }
    }
}

void UnionItem::itemChanged(QGraphicsItem::GraphicsItemChange change, const QVariant & /*value*/)
{
    if(change == QGraphicsItem::ItemPositionChange) {
        RChild *theChain = chain();
        if(NULL != theChain) {
            chain()->updatePosition();
            updateScenePosition(&_graphicsItem);
            foreach(RChild * child, _children.children()) {
                child->updatePosition();
            }
        }
    }
}

void UnionItem::childAdded(XSchemaObject *newChild)
{
    if(NULL == newChild) {
        return ;
    }
    XSDItem *child = addChild(newChild);
    if(NULL == child) {
        Utils::error(tr("An error occurred inserting the graphic item corresponding to the object."));
    }
}
//-------------------------------------------------------------------------------


ListItem::ListItem(XsdGraphicContext *newContext, XSchemaSimpleTypeList *newItem, QGraphicsItem * parent)
    : XSDItem(newContext), _item(NULL), _graphicsItem(this), _nameWidget(NULL)
{
    parent->scene()->addItem(&_graphicsItem);
    _graphicsItem.setData(XSD_ITEM_DATA, qVariantFromValue((void*)this));
    init();
    setItem(newItem);
}

ListItem::~ListItem()
{
    reset();
}

void ListItem::reset()
{
}

void ListItem::init()
{
    QPainterPath path;
    path.moveTo(0, 60);
    path.lineTo(30, 60);
    path.lineTo(45, 30);
    path.lineTo(30, 0);
    path.lineTo(-30, 0);
    path.lineTo(-45, 30);
    path.lineTo(-30, 60);
    _contour = path.toFillPolygon();
    _graphicsItem.setPolygon(_contour);
    _graphicsItem.setFlag(QGraphicsItem::ItemIsMovable, false);
    _graphicsItem.setFlag(QGraphicsItem::ItemIsSelectable, true);
    _graphicsItem.setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    _graphicsItem.setPos(0, 0);

    QLinearGradient gradient(0, 0, 0, 100);
    gradient.setColorAt(0, QColor::fromRgbF(0, 1, .4, .2));
    gradient.setColorAt(1, QColor::fromRgbF(0, 1, 1, 0));

    _graphicsItem.setBrush(QBrush(gradient));

    _nameWidget = new QGraphicsSimpleTextItem(&_graphicsItem);
    if(NULL != _nameWidget) {
        _nameWidget->setText(tr("List"));
        _nameWidget->setPos(0, 30);
    }

    connect(&_graphicsItem, SIGNAL(itemChanged(QGraphicsItem::GraphicsItemChange, const QVariant&)), this, SLOT(itemChanged(QGraphicsItem::GraphicsItemChange, const QVariant&)));
}

XSchemaSimpleTypeList *ListItem::list() const
{
    return _item ;
}

void ListItem::setItem(XSchemaObject *newItem)
{
    if(_item != newItem) {
        if(NULL != _item) {
            disconnect(_item, SIGNAL(childAdded(XSchemaObject*)), this, SLOT(childAdded(XSchemaObject*)));
            //TODO disconnect(_item, SIGNAL(childRemoved(XSchemaObject*)), this, SLOT(childRemoved(XSchemaObject*)));
        }
        _item = (XSchemaSimpleTypeList*)newItem ;
        if(NULL != newItem) {
            connect(_item, SIGNAL(childAdded(XSchemaObject*)), this, SLOT(childAdded(XSchemaObject*)));
            //TODO connect(_item, SIGNAL(childRemoved(XSchemaObject*)), this, SLOT(childRemoved(XSchemaObject*)));
            foreach(XSchemaObject * child, _item->getChildren()) {
                childAdded(child);
            }
            if(NULL != _nameWidget) {
                QString txt = tr("List");
                txt += " " ;
                txt += _item->itemType() ;
                _nameWidget->setText(txt);
            }
        } else {
            if(NULL != _nameWidget) {
                _nameWidget->setText(tr("List"));
            }
        }
    }
    QGraphicsItem * items []  = { _nameWidget } ;
    QRectF size = measureOptimumDimensions(1, items) ;
    qreal width = size.x() + size.width();
    qreal height = size.y() + size.height();

    qreal stepY = (height <= 30) ? height : 30 ;
    qreal stepX = (width <= 30) ? width : 30 ;

    QPainterPath path;
    path.moveTo(0, stepY);
    path.lineTo(stepX / 2, 0);
    path.lineTo(stepX + stepX / 2, 0);
    path.lineTo(2 * stepX, stepY);
    path.lineTo(stepX + stepX / 2, 2 * stepY);
    path.lineTo(stepX / 2, 2 * stepY);
    _contour = path.toFillPolygon();
    _graphicsItem.setPolygon(_contour);
}

void ListItem::itemChanged(QGraphicsItem::GraphicsItemChange change, const QVariant &/*value*/)
{
    if(change == QGraphicsItem::ItemPositionChange) {
        RChild *theChain = chain();
        if(NULL != theChain) {
            chain()->updatePosition();
            updateScenePosition(&_graphicsItem);
            foreach(RChild * child, _children.children()) {
                child->updatePosition();
            }
        }
    }
}

void ListItem::childAdded(XSchemaObject *newChild)
{
    if(NULL == newChild) {
        return ;
    }
    XSDItem *child = addChild(newChild);
    if(NULL == child) {
        Utils::error(tr("An error occurred inserting the graphic item corresponding to the object."));
    }
}
