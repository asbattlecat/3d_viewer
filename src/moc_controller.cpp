/****************************************************************************
** Meta object code from reading C++ file 'controller.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "include/controller.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'controller.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_s21__Controller_t {
    uint offsetsAndSizes[28];
    char stringdata0[16];
    char stringdata1[12];
    char stringdata2[1];
    char stringdata3[21];
    char stringdata4[14];
    char stringdata5[8];
    char stringdata6[29];
    char stringdata7[19];
    char stringdata8[12];
    char stringdata9[14];
    char stringdata10[19];
    char stringdata11[24];
    char stringdata12[18];
    char stringdata13[4];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_s21__Controller_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_s21__Controller_t qt_meta_stringdata_s21__Controller = {
    {
        QT_MOC_LITERAL(0, 15),  // "s21::Controller"
        QT_MOC_LITERAL(16, 11),  // "save_state_"
        QT_MOC_LITERAL(28, 0),  // ""
        QT_MOC_LITERAL(29, 20),  // "handle_model_loaded_"
        QT_MOC_LITERAL(50, 13),  // "Model3DDataGl"
        QT_MOC_LITERAL(64, 7),  // "gl_data"
        QT_MOC_LITERAL(72, 28),  // "handle_model_loaded_memento_"
        QT_MOC_LITERAL(101, 18),  // "handle_load_error_"
        QT_MOC_LITERAL(120, 11),  // "std::string"
        QT_MOC_LITERAL(132, 13),  // "error_message"
        QT_MOC_LITERAL(146, 18),  // "handle_update_mvp_"
        QT_MOC_LITERAL(165, 23),  // "handle_update_temp_mvp_"
        QT_MOC_LITERAL(189, 17),  // "S21MatrixWrapper&"
        QT_MOC_LITERAL(207, 3)   // "mvp"
    },
    "s21::Controller",
    "save_state_",
    "",
    "handle_model_loaded_",
    "Model3DDataGl",
    "gl_data",
    "handle_model_loaded_memento_",
    "handle_load_error_",
    "std::string",
    "error_message",
    "handle_update_mvp_",
    "handle_update_temp_mvp_",
    "S21MatrixWrapper&",
    "mvp"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_s21__Controller[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   50,    2, 0x08,    1 /* Private */,
       3,    1,   51,    2, 0x08,    2 /* Private */,
       6,    1,   54,    2, 0x08,    4 /* Private */,
       7,    1,   57,    2, 0x08,    6 /* Private */,
      10,    0,   60,    2, 0x08,    8 /* Private */,
      11,    1,   61,    2, 0x08,    9 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,   13,

       0        // eod
};

Q_CONSTINIT const QMetaObject s21::Controller::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_s21__Controller.offsetsAndSizes,
    qt_meta_data_s21__Controller,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_s21__Controller_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Controller, std::true_type>,
        // method 'save_state_'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handle_model_loaded_'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const Model3DDataGl &, std::false_type>,
        // method 'handle_model_loaded_memento_'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const Model3DDataGl &, std::false_type>,
        // method 'handle_load_error_'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const std::string &, std::false_type>,
        // method 'handle_update_mvp_'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handle_update_temp_mvp_'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<S21MatrixWrapper &, std::false_type>
    >,
    nullptr
} };

void s21::Controller::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Controller *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->save_state_(); break;
        case 1: _t->handle_model_loaded_((*reinterpret_cast< std::add_pointer_t<Model3DDataGl>>(_a[1]))); break;
        case 2: _t->handle_model_loaded_memento_((*reinterpret_cast< std::add_pointer_t<Model3DDataGl>>(_a[1]))); break;
        case 3: _t->handle_load_error_((*reinterpret_cast< std::add_pointer_t<std::string>>(_a[1]))); break;
        case 4: _t->handle_update_mvp_(); break;
        case 5: _t->handle_update_temp_mvp_((*reinterpret_cast< std::add_pointer_t<S21MatrixWrapper&>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *s21::Controller::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *s21::Controller::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_s21__Controller.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int s21::Controller::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
