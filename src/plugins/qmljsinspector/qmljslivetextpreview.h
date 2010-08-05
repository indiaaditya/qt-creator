/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** Commercial Usage
**
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://qt.nokia.com/contact.
**
**************************************************************************/

#ifndef SCRIPTBINDINGREWRITER_H
#define SCRIPTBINDINGREWRITER_H

#include <QObject>
#include <QWeakPointer>

#include <qmljs/parser/qmljsastfwd_p.h>
#include <qmljs/qmljsdocument.h>
#include "qmljsprivateapi.h"

QT_FORWARD_DECLARE_CLASS(QTextDocument)

namespace Core {
    class IEditor;
}

namespace QmlJS {
    class ModelManagerInterface;
}

namespace QmlJSEditor {
namespace Internal {
    class QmlJSTextEditor;
}
}

namespace QmlJSInspector {
namespace Internal {

class QmlJSLiveTextPreview : public QObject
{
    Q_OBJECT

public:
    explicit QmlJSLiveTextPreview(const QmlJS::Document::Ptr &doc, const QmlJS::Document::Ptr &initDoc, QObject *parent = 0);
    //void updateDocuments();

    void associateEditor(Core::IEditor *editor);
    void unassociateEditor(Core::IEditor *editor);
    void setActiveObject(const QDeclarativeDebugObjectReference &object);
    void mapObjectToQml(const QDeclarativeDebugObjectReference &object);
    void resetInitialDoc(const QmlJS::Document::Ptr &doc);

    enum UnsyncronizableChangeType {
        NoUnsyncronizableChanges,
        AttributeChangeWarning,
        ElementChangeWarning
    };

signals:
    void selectedItemsChanged(const QList<QDeclarativeDebugObjectReference> &objects);
    void reloadQmlViewerRequested();
    void disableLivePreviewRequested();

public slots:
    void setApplyChangesToQmlObserver(bool applyChanges);
    void updateDebugIds(const QDeclarativeDebugObjectReference &rootReference);

private slots:
    void changeSelectedElements(QList<int> offsets, const QString &wordAtCursor);
    void documentChanged(QmlJS::Document::Ptr doc);
    void disableLivePreview();
    void reloadQmlViewer();

private:
    static QmlJS::ModelManagerInterface *modelManager();
    QList<int> objectReferencesForOffset(quint32 offset) const;
    QVariant castToLiteral(const QString &expression, QmlJS::AST::UiScriptBinding *scriptBinding);
    void showSyncWarning(UnsyncronizableChangeType unsyncronizableChangeType, const QString &elementName,
                         unsigned line, unsigned column);
    void showExperimentalWarning();

private:
    QHash<QmlJS::AST::UiObjectMember*, QList<int> > m_debugIds;
    QHash<QmlJS::Document::Ptr, QSet<QmlJS::AST::UiObjectMember *> > m_createdObjects;

    QmlJS::Document::Ptr m_previousDoc;
    QmlJS::Document::Ptr m_initialDoc; //the document that was loaded by the server
    QString m_filename;

    QList<QWeakPointer<QmlJSEditor::Internal::QmlJSTextEditor> > m_editors;

    bool m_applyChangesToQmlObserver;
    QmlJS::Document::Ptr m_docWithUnappliedChanges;

};

} // namespace Internal
} // namespace QmlJSInspector

#endif // SCRIPTBINDINGREWRITER_H
