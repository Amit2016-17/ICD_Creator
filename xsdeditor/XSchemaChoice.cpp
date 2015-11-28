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

#include "xsdeditor/xschema.h"
#include "xsdeditor/XSchemaIOContants.h"

XSchemaChoice::XSchemaChoice(XSchemaObject *newParent, XSchemaRoot *newRoot) : XSchemaObject(newParent, newRoot)
{
    _maxOccurs.isUnbounded = true ;
}

XSchemaChoice::~XSchemaChoice()
{
    reset();
}

void XSchemaChoice::reset()
{
    XSchemaObject::reset();
}

XSchemaAttribute* XSchemaChoice::addAttribute()
{
    return realAddAttribute();
}

XSchemaElement* XSchemaChoice::addElement()
{
    return realAddElement();
}


XSchemaChoice* XSchemaChoice::addChoice()
{
    return realAddChoice();
}

XSchemaSequence* XSchemaChoice::addSequence()
{
    return realAddSequence();
}


bool XSchemaChoice::generateDom(QDomDocument &document, QDomNode &parent)
{
    QDomElement node = document.createElement("choice");
    if(!generateInnerDom(document, parent)) {
        return false;
    }
    parent.appendChild(node);
    return true;
}

void XSchemaChoice::loadFromDom(QDomElement &elementToExamine)
{
    XScanContext context ;
    scanDom(elementToExamine, &context);
}


void XSchemaChoice::scanForAttributes(QDomAttr &attribute, void * /*context*/)
{
    QString name = attribute.nodeName();

    if(name == IO_GENERIC_ID) {
        _id = attribute.value() ;
    } else if(name == IO_GENERIC_MINOCCURS) {
        if(!_minOccurs.setValueFromAttribute(attribute.value())) {
            raiseError(this, attribute, false);
        }
    } else if(name == IO_GENERIC_MAXOCCURS) {
        if(!_maxOccurs.setValueFromAttribute(attribute.value())) {
            raiseError(this, attribute, false);
        }
    } else {
        if(!readOtherAttributes(attribute)) {
            raiseError(this, attribute, false);
        }
    }
}

void XSchemaChoice::scanForElements(QDomElement &element, void * /*context*/)
{
    QString name = element.localName();
    if(element.prefix() == _root->namespacePrefix()) {
        if(name == IO_XSD_ANNOTATION) {
            readHandleAnnotation(element);
            return;
        } else if(name == IO_XSD_TAGELEMENT) {
            readHandleObject(element, new XSchemaElement(this, _root));
            return ;
        } else if(name == IO_XSD_GROUP) {
            readHandleObject(element, new XSchemaGroup(this, _root));
            return ;
        } else if(name == IO_XSD_CHOICE) {
            readHandleObject(element, new XSchemaChoice(this, _root));
            return ;
        } else if(name == IO_XSD_SEQUENCE) {
            readHandleObject(element, new XSchemaSequence(this, _root));
            return ;
        } else if(name == IO_XSD_ANY) {
            readHandleObject(element, new XSchemaAny(this, _root));
            return ;
        }
    }
    raiseError(this, element, true);
}

