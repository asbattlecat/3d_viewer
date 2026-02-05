/****************************************************************************
** Meta object code from reading C++ file '3dmodel.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "include/3dmodel.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file '3dmodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_s21__ObjLoader_t {
    uint offsetsAndSizes[26];
    char stringdata0[15];
    char stringdata1[7];
    char stringdata2[1];
    char stringdata3[22];
    char stringdata4[9];
    char stringdata5[31];
    char stringdata6[6];
    char stringdata7[50];
    char stringdata8[6];
    char stringdata9[10];
    char stringdata10[12];
    char stringdata11[14];
    char stringdata12[5];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_s21__ObjLoader_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_s21__ObjLoader_t qt_meta_stringdata_s21__ObjLoader = {
    {
        QT_MOC_LITERAL(0, 14),  // "s21::ObjLoader"
        QT_MOC_LITERAL(15, 6),  // "Loaded"
        QT_MOC_LITERAL(22, 0),  // ""
        QT_MOC_LITERAL(23, 21),  // "std::vector<Vector3D>"
        QT_MOC_LITERAL(45, 8),  // "vertices"
        QT_MOC_LITERAL(54, 30),  // "std::vector<std::vector<uint>>"
        QT_MOC_LITERAL(85, 5),  // "faces"
        QT_MOC_LITERAL(91, 49),  // "std::unordered_set<std::pair<..."
        QT_MOC_LITERAL(141, 5),  // "edges"
        QT_MOC_LITERAL(147, 9),  // "LoadError"
        QT_MOC_LITERAL(157, 11),  // "std::string"
        QT_MOC_LITERAL(169, 13),  // "error_message"
        QT_MOC_LITERAL(183, 4)   // "Load"
    },
    "s21::ObjLoader",
    "Loaded",
    "",
    "std::vector<Vector3D>",
    "vertices",
    "std::vector<std::vector<uint>>",
    "faces",
    "std::unordered_set<std::pair<uint,uint>,PairHash>",
    "edges",
    "LoadError",
    "std::string",
    "error_message",
    "Load"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_s21__ObjLoader[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    3,   32,    2, 0x06,    1 /* Public */,
       9,    1,   39,    2, 0x06,    5 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      12,    0,   42,    2, 0x0a,    7 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5, 0x80000000 | 7,    4,    6,    8,
    QMetaType::Void, 0x80000000 | 10,   11,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject s21::ObjLoader::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_s21__ObjLoader.offsetsAndSizes,
    qt_meta_data_s21__ObjLoader,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_s21__ObjLoader_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ObjLoader, std::true_type>,
        // method 'Loaded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<Vector3D> &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::vector<std::vector<unsigned int>> &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::unordered_set<std::pair<unsigned int,unsigned int>,PairHash> &, std::false_type>,
        // method 'LoadError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::string &, std::false_type>,
        // method 'Load'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void s21::ObjLoader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ObjLoader *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->Loaded((*reinterpret_cast< std::add_pointer_t<std::vector<Vector3D>>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<std::vector<std::vector<uint>>>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<std::unordered_set<std::pair<uint,uint>,PairHash>>>(_a[3]))); break;
        case 1: _t->LoadError((*reinterpret_cast< std::add_pointer_t<std::string>>(_a[1]))); break;
        case 2: _t->Load(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ObjLoader::*)(const std::vector<Vector3D> & , const std::vector<std::vector<unsigned int>> & , const std::unordered_set<std::pair<unsigned int,unsigned int>,PairHash> & );
            if (_t _q_method = &ObjLoader::Loaded; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ObjLoader::*)(const std::string & );
            if (_t _q_method = &ObjLoader::LoadError; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject *s21::ObjLoader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *s21::ObjLoader::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_s21__ObjLoader.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int s21::ObjLoader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void s21::ObjLoader::Loaded(const std::vector<Vector3D> & _t1, const std::vector<std::vector<unsigned int>> & _t2, const std::unordered_set<std::pair<unsigned int,unsigned int>,PairHash> & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void s21::ObjLoader::LoadError(const std::string & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
