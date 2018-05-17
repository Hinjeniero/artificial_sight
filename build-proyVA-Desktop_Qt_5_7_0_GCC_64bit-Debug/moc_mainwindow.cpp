/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../5_proyVA/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[23];
    char stringdata0[223];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 7), // "compute"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 17), // "change_color_gray"
QT_MOC_LITERAL(4, 38, 5), // "color"
QT_MOC_LITERAL(5, 44, 12), // "selectWindow"
QT_MOC_LITERAL(6, 57, 1), // "p"
QT_MOC_LITERAL(7, 59, 1), // "w"
QT_MOC_LITERAL(8, 61, 1), // "h"
QT_MOC_LITERAL(9, 63, 14), // "deselectWindow"
QT_MOC_LITERAL(10, 78, 14), // "load_from_file"
QT_MOC_LITERAL(11, 93, 10), // "getCorners"
QT_MOC_LITERAL(12, 104, 19), // "initializeDisparity"
QT_MOC_LITERAL(13, 124, 18), // "propagateDisparity"
QT_MOC_LITERAL(14, 143, 17), // "analyzeAllRegions"
QT_MOC_LITERAL(15, 161, 13), // "analyzeRegion"
QT_MOC_LITERAL(16, 175, 5), // "Point"
QT_MOC_LITERAL(17, 181, 6), // "pStart"
QT_MOC_LITERAL(18, 188, 4), // "Mat&"
QT_MOC_LITERAL(19, 193, 6), // "imgReg"
QT_MOC_LITERAL(20, 200, 6), // "Region"
QT_MOC_LITERAL(21, 207, 6), // "region"
QT_MOC_LITERAL(22, 214, 8) // "analyzed"

    },
    "MainWindow\0compute\0\0change_color_gray\0"
    "color\0selectWindow\0p\0w\0h\0deselectWindow\0"
    "load_from_file\0getCorners\0initializeDisparity\0"
    "propagateDisparity\0analyzeAllRegions\0"
    "analyzeRegion\0Point\0pStart\0Mat&\0imgReg\0"
    "Region\0region\0analyzed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x0a /* Public */,
       3,    1,   65,    2, 0x0a /* Public */,
       5,    3,   68,    2, 0x0a /* Public */,
       9,    0,   75,    2, 0x0a /* Public */,
      10,    0,   76,    2, 0x0a /* Public */,
      11,    0,   77,    2, 0x0a /* Public */,
      12,    0,   78,    2, 0x0a /* Public */,
      13,    0,   79,    2, 0x0a /* Public */,
      14,    0,   80,    2, 0x0a /* Public */,
      15,    4,   81,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::QPointF, QMetaType::Int, QMetaType::Int,    6,    7,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 16, 0x80000000 | 18, 0x80000000 | 20, 0x80000000 | 18,   17,   19,   21,   22,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->compute(); break;
        case 1: _t->change_color_gray((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->selectWindow((*reinterpret_cast< QPointF(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: _t->deselectWindow(); break;
        case 4: _t->load_from_file(); break;
        case 5: _t->getCorners(); break;
        case 6: _t->initializeDisparity(); break;
        case 7: _t->propagateDisparity(); break;
        case 8: _t->analyzeAllRegions(); break;
        case 9: _t->analyzeRegion((*reinterpret_cast< Point(*)>(_a[1])),(*reinterpret_cast< Mat(*)>(_a[2])),(*reinterpret_cast< Region(*)>(_a[3])),(*reinterpret_cast< Mat(*)>(_a[4]))); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
