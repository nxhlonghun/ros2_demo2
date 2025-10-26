/****************************************************************************
** Meta object code from reading C++ file 'Widget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/demo_cpp_qt2/include/demo_cpp_qt2/Widget.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSWidgetENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSWidgetENDCLASS = QtMocHelpers::stringData(
    "Widget",
    "PB_node_start",
    "",
    "PB_node_stop",
    "list_clicked",
    "string",
    "list",
    "onRefreshNodeBtnClicked",
    "onCustomContextMenu",
    "point",
    "onCollispeNode",
    "onExpandNode",
    "onGetTopicList",
    "onSubBtnClicked",
    "onRecv",
    "value"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSWidgetENDCLASS_t {
    uint offsetsAndSizes[32];
    char stringdata0[7];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[13];
    char stringdata4[13];
    char stringdata5[7];
    char stringdata6[5];
    char stringdata7[24];
    char stringdata8[20];
    char stringdata9[6];
    char stringdata10[15];
    char stringdata11[13];
    char stringdata12[15];
    char stringdata13[16];
    char stringdata14[7];
    char stringdata15[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSWidgetENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSWidgetENDCLASS_t qt_meta_stringdata_CLASSWidgetENDCLASS = {
    {
        QT_MOC_LITERAL(0, 6),  // "Widget"
        QT_MOC_LITERAL(7, 13),  // "PB_node_start"
        QT_MOC_LITERAL(21, 0),  // ""
        QT_MOC_LITERAL(22, 12),  // "PB_node_stop"
        QT_MOC_LITERAL(35, 12),  // "list_clicked"
        QT_MOC_LITERAL(48, 6),  // "string"
        QT_MOC_LITERAL(55, 4),  // "list"
        QT_MOC_LITERAL(60, 23),  // "onRefreshNodeBtnClicked"
        QT_MOC_LITERAL(84, 19),  // "onCustomContextMenu"
        QT_MOC_LITERAL(104, 5),  // "point"
        QT_MOC_LITERAL(110, 14),  // "onCollispeNode"
        QT_MOC_LITERAL(125, 12),  // "onExpandNode"
        QT_MOC_LITERAL(138, 14),  // "onGetTopicList"
        QT_MOC_LITERAL(153, 15),  // "onSubBtnClicked"
        QT_MOC_LITERAL(169, 6),  // "onRecv"
        QT_MOC_LITERAL(176, 5)   // "value"
    },
    "Widget",
    "PB_node_start",
    "",
    "PB_node_stop",
    "list_clicked",
    "string",
    "list",
    "onRefreshNodeBtnClicked",
    "onCustomContextMenu",
    "point",
    "onCollispeNode",
    "onExpandNode",
    "onGetTopicList",
    "onSubBtnClicked",
    "onRecv",
    "value"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSWidgetENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   74,    2, 0x06,    1 /* Public */,
       3,    0,   75,    2, 0x06,    2 /* Public */,
       4,    1,   76,    2, 0x06,    3 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       7,    0,   79,    2, 0x08,    5 /* Private */,
       8,    1,   80,    2, 0x08,    6 /* Private */,
      10,    0,   83,    2, 0x08,    8 /* Private */,
      11,    0,   84,    2, 0x08,    9 /* Private */,
      12,    0,   85,    2, 0x08,   10 /* Private */,
      13,    0,   86,    2, 0x08,   11 /* Private */,
      14,    1,   87,    2, 0x08,   12 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   15,

       0        // eod
};

Q_CONSTINIT const QMetaObject Widget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSWidgetENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSWidgetENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSWidgetENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Widget, std::true_type>,
        // method 'PB_node_start'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'PB_node_stop'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'list_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<string, std::false_type>,
        // method 'onRefreshNodeBtnClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onCustomContextMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>,
        // method 'onCollispeNode'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onExpandNode'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onGetTopicList'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSubBtnClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onRecv'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>
    >,
    nullptr
} };

void Widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Widget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->PB_node_start(); break;
        case 1: _t->PB_node_stop(); break;
        case 2: _t->list_clicked((*reinterpret_cast< std::add_pointer_t<string>>(_a[1]))); break;
        case 3: _t->onRefreshNodeBtnClicked(); break;
        case 4: _t->onCustomContextMenu((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 5: _t->onCollispeNode(); break;
        case 6: _t->onExpandNode(); break;
        case 7: _t->onGetTopicList(); break;
        case 8: _t->onSubBtnClicked(); break;
        case 9: _t->onRecv((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Widget::*)();
            if (_t _q_method = &Widget::PB_node_start; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Widget::*)();
            if (_t _q_method = &Widget::PB_node_stop; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Widget::*)(string );
            if (_t _q_method = &Widget::list_clicked; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject *Widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Widget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSWidgetENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void Widget::PB_node_start()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Widget::PB_node_stop()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Widget::list_clicked(string _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
