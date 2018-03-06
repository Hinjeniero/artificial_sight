/****************************************************************************
** Meta object code from reading C++ file 'rcdraw.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../proyVA_2/rcdraw.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rcdraw.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_RCDraw_t {
    QByteArrayData data[7];
    char stringdata0[53];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RCDraw_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RCDraw_t qt_meta_stringdata_RCDraw = {
    {
QT_MOC_LITERAL(0, 0, 6), // "RCDraw"
QT_MOC_LITERAL(1, 7, 14), // "windowSelected"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 6), // "center"
QT_MOC_LITERAL(4, 30, 5), // "sizeX"
QT_MOC_LITERAL(5, 36, 5), // "sizeY"
QT_MOC_LITERAL(6, 42, 10) // "pressEvent"

    },
    "RCDraw\0windowSelected\0\0center\0sizeX\0"
    "sizeY\0pressEvent"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RCDraw[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   24,    2, 0x06 /* Public */,
       6,    0,   31,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QPointF, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void,

       0        // eod
};

void RCDraw::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RCDraw *_t = static_cast<RCDraw *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->windowSelected((*reinterpret_cast< QPointF(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->pressEvent(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RCDraw::*_t)(QPointF , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RCDraw::windowSelected)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (RCDraw::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RCDraw::pressEvent)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject RCDraw::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_RCDraw.data,
      qt_meta_data_RCDraw,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *RCDraw::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RCDraw::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_RCDraw.stringdata0))
        return static_cast<void*>(const_cast< RCDraw*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int RCDraw::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void RCDraw::windowSelected(QPointF _t1, int _t2, int _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RCDraw::pressEvent()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
