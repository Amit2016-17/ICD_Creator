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

#include "xsdeditor/validator/xvalidationcontext.h"
#include "xsdeditor/xschema.h"

XValidationContext::XValidationContext(XElementContent *rootContent)
{
    _isError = false ;
    _currentTarget = NULL ;
    _content = rootContent;
}

XValidationContext::~XValidationContext()
{
}

void XValidationContext::setError(const QString &errorMessage)
{
    _isError = true ;
    _errorMessage = errorMessage;
}

XSingleElementContent  *XValidationContext::currentTarget()
{
    return _currentTarget;
}

void XValidationContext::setCurrentTarget(XSingleElementContent *newTarget)
{
    _currentTarget = newTarget ;
}

XSingleElementContent *XValidationContext::addAllowed(XSingleElementContent *parent, XSchemaObject *object)
{
    XSingleElementContent * newObj = NULL ;
    if(NULL == parent) {
        newObj =  _content->addAllowed(this, object);
    } else {
        newObj = parent->addAChild(object);
        if(NULL == newObj) {
            setError(QString(tr("Unable to add allowed item %1")).arg((NULL != object) ? object->name() : "?"));
        } else {
            setCurrentTarget(newObj);
        }
    }
    return newObj;
}
