/****************************************************************************
** Meta object code from reading C++ file 'test.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../test.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'test.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_test_t {
    QByteArrayData data[15];
    char stringdata[165];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_test_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_test_t qt_meta_stringdata_test = {
    {
QT_MOC_LITERAL(0, 0, 4),
QT_MOC_LITERAL(1, 5, 10),
QT_MOC_LITERAL(2, 16, 0),
QT_MOC_LITERAL(3, 17, 8),
QT_MOC_LITERAL(4, 26, 3),
QT_MOC_LITERAL(5, 30, 7),
QT_MOC_LITERAL(6, 38, 9),
QT_MOC_LITERAL(7, 48, 14),
QT_MOC_LITERAL(8, 63, 7),
QT_MOC_LITERAL(9, 71, 13),
QT_MOC_LITERAL(10, 85, 11),
QT_MOC_LITERAL(11, 97, 13),
QT_MOC_LITERAL(12, 111, 14),
QT_MOC_LITERAL(13, 126, 21),
QT_MOC_LITERAL(14, 148, 15)
    },
    "test\0load_scene\0\0load_csv\0run\0runFast\0"
    "GoToFrame\0LoadKinectData\0drawall\0"
    "drawFurniture\0clearPoints\0load_odom_csv\0"
    "load_sonar_csv\0generate_2d_point_map\0"
    "BatchProcessing\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_test[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x0a,
       3,    0,   80,    2, 0x0a,
       4,    0,   81,    2, 0x0a,
       5,    0,   82,    2, 0x0a,
       6,    0,   83,    2, 0x0a,
       7,    0,   84,    2, 0x0a,
       8,    0,   85,    2, 0x0a,
       9,    0,   86,    2, 0x0a,
      10,    0,   87,    2, 0x0a,
      11,    0,   88,    2, 0x0a,
      12,    0,   89,    2, 0x0a,
      13,    0,   90,    2, 0x0a,
      14,    0,   91,    2, 0x0a,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void test::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        test *_t = static_cast<test *>(_o);
        switch (_id) {
        case 0: _t->load_scene(); break;
        case 1: _t->load_csv(); break;
        case 2: _t->run(); break;
        case 3: _t->runFast(); break;
        case 4: _t->GoToFrame(); break;
        case 5: _t->LoadKinectData(); break;
        case 6: _t->drawall(); break;
        case 7: _t->drawFurniture(); break;
        case 8: _t->clearPoints(); break;
        case 9: _t->load_odom_csv(); break;
        case 10: _t->load_sonar_csv(); break;
        case 11: _t->generate_2d_point_map(); break;
        case 12: _t->BatchProcessing(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject test::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_test.data,
      qt_meta_data_test,  qt_static_metacall, 0, 0}
};


const QMetaObject *test::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *test::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_test.stringdata))
        return static_cast<void*>(const_cast< test*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int test::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
