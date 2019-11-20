
#ifndef G_JVM_H
#define G_JVM_H



//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../utils/tinycthread.h"

#include "../utils/d_type.h"
#include "../utils/hashtable.h"
#include "../utils/utf8_string.h"
#include "../utils/arraylist.h"
#include "../utils/pairlist.h"
#include "../utils/bytebuf.h"
#include "../utils/hashset.h"

#ifdef __cplusplus
extern "C" {
#endif


//=======================  micro define  =============================
//_JVM_DEBUG  01=thread info, 02=garage info , 03=class load, 04=method call,  06=all bytecode
#define _JVM_DEBUG_BYTECODE_DETAIL 01
#define _JVM_DEBUG_PRINT_FILE 0
#define _JVM_DEBUG_GARBAGE_DUMP 0
#define _JVM_DEBUG_PROFILE 0

#define JDWP_DEBUG 0

#if __JVM_OS_VS__ || __JVM_OS_MINGW__ || __JVM_OS_CYGWIN__
#define barrier() MemoryBarrier()
#else
#define barrier() __asm__ __volatile__("": : :"memory")
#endif


enum {
    /* 0x00 */ op_nop,
    /* 0x01 */ op_aconst_null,
    /* 0x02 */ op_iconst_m1,
    /* 0x03 */ op_iconst_0,
    /* 0x04 */ op_iconst_1,
    /* 0x05 */ op_iconst_2,
    /* 0x06 */ op_iconst_3,
    /* 0x07 */ op_iconst_4,
    /* 0x08 */ op_iconst_5,
    /* 0x09 */ op_lconst_0,
    /* 0x0A */ op_lconst_1,
    /* 0x0B */ op_fconst_0,
    /* 0x0C */ op_fconst_1,
    /* 0x0D */ op_fconst_2,
    /* 0x0E */ op_dconst_0,
    /* 0x0F */ op_dconst_1,
    /* 0x10 */ op_bipush,
    /* 0x11 */ op_sipush,
    /* 0x12 */ op_ldc,
    /* 0x13 */ op_ldc_w,
    /* 0x14 */ op_ldc2_w,
    /* 0x15 */ op_iload,
    /* 0x16 */ op_lload,
    /* 0x17 */ op_fload,
    /* 0x18 */ op_dload,
    /* 0x19 */ op_aload,
    /* 0x1A */ op_iload_0,
    /* 0x1B */ op_iload_1,
    /* 0x1C */ op_iload_2,
    /* 0x1D */ op_iload_3,
    /* 0x1E */ op_lload_0,
    /* 0x1F */ op_lload_1,
    /* 0x20 */ op_lload_2,
    /* 0x21 */ op_lload_3,
    /* 0x22 */ op_fload_0,
    /* 0x23 */ op_fload_1,
    /* 0x24 */ op_fload_2,
    /* 0x25 */ op_fload_3,
    /* 0x26 */ op_dload_0,
    /* 0x27 */ op_dload_1,
    /* 0x28 */ op_dload_2,
    /* 0x29 */ op_dload_3,
    /* 0x2A */ op_aload_0,
    /* 0x2B */ op_aload_1,
    /* 0x2C */ op_aload_2,
    /* 0x2D */ op_aload_3,
    /* 0x2E */ op_iaload,
    /* 0x2F */ op_laload,
    /* 0x30 */ op_faload,
    /* 0x31 */ op_daload,
    /* 0x32 */ op_aaload,
    /* 0x33 */ op_baload,
    /* 0x34 */ op_caload,
    /* 0x35 */ op_saload,
    /* 0x36 */ op_istore,
    /* 0x37 */ op_lstore,
    /* 0x38 */ op_fstore,
    /* 0x39 */ op_dstore,
    /* 0x3A */ op_astore,
    /* 0x3B */ op_istore_0,
    /* 0x3C */ op_istore_1,
    /* 0x3D */ op_istore_2,
    /* 0x3E */ op_istore_3,
    /* 0x3F */ op_lstore_0,
    /* 0x40 */ op_lstore_1,
    /* 0x41 */ op_lstore_2,
    /* 0x42 */ op_lstore_3,
    /* 0x43 */ op_fstore_0,
    /* 0x44 */ op_fstore_1,
    /* 0x45 */ op_fstore_2,
    /* 0x46 */ op_fstore_3,
    /* 0x47 */ op_dstore_0,
    /* 0x48 */ op_dstore_1,
    /* 0x49 */ op_dstore_2,
    /* 0x4A */ op_dstore_3,
    /* 0x4B */ op_astore_0,
    /* 0x4C */ op_astore_1,
    /* 0x4D */ op_astore_2,
    /* 0x4E */ op_astore_3,
    /* 0x4F */ op_iastore,
    /* 0x50 */ op_lastore,
    /* 0x51 */ op_fastore,
    /* 0x52 */ op_dastore,
    /* 0x53 */ op_aastore,
    /* 0x54 */ op_bastore,
    /* 0x55 */ op_castore,
    /* 0x56 */ op_sastore,
    /* 0x57 */ op_pop,
    /* 0x58 */ op_pop2,
    /* 0x59 */ op_dup,
    /* 0x5A */ op_dup_x1,
    /* 0x5B */ op_dup_x2,
    /* 0x5C */ op_dup2,
    /* 0x5D */ op_dup2_x1,
    /* 0x5E */ op_dup2_x2,
    /* 0x5F */ op_swap,
    /* 0x60 */ op_iadd,
    /* 0x61 */ op_ladd,
    /* 0x62 */ op_fadd,
    /* 0x63 */ op_dadd,
    /* 0x64 */ op_isub,
    /* 0x65 */ op_lsub,
    /* 0x66 */ op_fsub,
    /* 0x67 */ op_dsub,
    /* 0x68 */ op_imul,
    /* 0x69 */ op_lmul,
    /* 0x6A */ op_fmul,
    /* 0x6B */ op_dmul,
    /* 0x6C */ op_idiv,
    /* 0x6D */ op_ldiv,
    /* 0x6E */ op_fdiv,
    /* 0x6F */ op_ddiv,
    /* 0x70 */ op_irem,
    /* 0x71 */ op_lrem,
    /* 0x72 */ op_frem,
    /* 0x73 */ op_drem,
    /* 0x74 */ op_ineg,
    /* 0x75 */ op_lneg,
    /* 0x76 */ op_fneg,
    /* 0x77 */ op_dneg,
    /* 0x78 */ op_ishl,
    /* 0x79 */ op_lshl,
    /* 0x7A */ op_ishr,
    /* 0x7B */ op_lshr,
    /* 0x7C */ op_iushr,
    /* 0x7D */ op_lushr,
    /* 0x7E */ op_iand,
    /* 0x7F */ op_land,
    /* 0x80 */ op_ior,
    /* 0x81 */ op_lor,
    /* 0x82 */ op_ixor,
    /* 0x83 */ op_lxor,
    /* 0x84 */ op_iinc,
    /* 0x85 */ op_i2l,
    /* 0x86 */ op_i2f,
    /* 0x87 */ op_i2d,
    /* 0x88 */ op_l2i,
    /* 0x89 */ op_l2f,
    /* 0x8A */ op_l2d,
    /* 0x8B */ op_f2i,
    /* 0x8C */ op_f2l,
    /* 0x8D */ op_f2d,
    /* 0x8E */ op_d2i,
    /* 0x8F */ op_d2l,
    /* 0x90 */ op_d2f,
    /* 0x91 */ op_i2b,
    /* 0x92 */ op_i2c,
    /* 0x93 */ op_i2s,
    /* 0x94 */ op_lcmp,
    /* 0x95 */ op_fcmpl,
    /* 0x96 */ op_fcmpg,
    /* 0x97 */ op_dcmpl,
    /* 0x98 */ op_dcmpg,
    /* 0x99 */ op_ifeq,
    /* 0x9A */ op_ifne,
    /* 0x9B */ op_iflt,
    /* 0x9C */ op_ifge,
    /* 0x9D */ op_ifgt,
    /* 0x9E */ op_ifle,
    /* 0x9F */ op_if_icmpeq,
    /* 0xA0 */ op_if_icmpne,
    /* 0xA1 */ op_if_icmplt,
    /* 0xA2 */ op_if_icmpge,
    /* 0xA3 */ op_if_icmpgt,
    /* 0xA4 */ op_if_icmple,
    /* 0xA5 */ op_if_acmpeq,
    /* 0xA6 */ op_if_acmpne,
    /* 0xA7 */ op_goto,
    /* 0xA8 */ op_jsr,
    /* 0xA9 */ op_ret,
    /* 0xAA */ op_tableswitch,
    /* 0xAB */ op_lookupswitch,
    /* 0xAC */ op_ireturn,
    /* 0xAD */ op_lreturn,
    /* 0xAE */ op_freturn,
    /* 0xAF */ op_dreturn,
    /* 0xB0 */ op_areturn,
    /* 0xB1 */ op_return,
    /* 0xB2 */ op_getstatic,
    /* 0xB3 */ op_putstatic,
    /* 0xB4 */ op_getfield,
    /* 0xB5 */ op_putfield,
    /* 0xB6 */ op_invokevirtual,
    /* 0xB7 */ op_invokespecial,
    /* 0xB8 */ op_invokestatic,
    /* 0xB9 */ op_invokeinterface,
    /* 0xBA */ op_invokedynamic,
    /* 0xBB */ op_new,
    /* 0xBC */ op_newarray,
    /* 0xBD */ op_anewarray,
    /* 0xBE */ op_arraylength,
    /* 0xBF */ op_athrow,
    /* 0xC0 */ op_checkcast,
    /* 0xC1 */ op_instanceof,
    /* 0xC2 */ op_monitorenter,
    /* 0xC3 */ op_monitorexit,
    /* 0xC4 */ op_wide,
    /* 0xC5 */ op_multianewarray,
    /* 0xC6 */ op_ifnull,
    /* 0xC7 */ op_ifnonnull,
    /* 0xC8 */ op_goto_w,
    /* 0xC9 */ op_jsr_w,
    /* 0xCA */ op_breakpoint,
    /* 0xCB */ op_getstatic_ref,//
    /* 0xCC */ op_getstatic_long,
    /* 0xCD */ op_getstatic_int,
    /* 0xCE */ op_getstatic_short,
    /* 0xCF */ op_getstatic_jchar,
    /* 0xD0 */ op_getstatic_byte,
    /* 0xD1 */ op_putstatic_ref,
    /* 0xD2 */ op_putstatic_long,
    /* 0xD3 */ op_putstatic_int,
    /* 0xD4 */ op_putstatic_short,
    /* 0xD5 */ op_putstatic_byte,
    /* 0xD6 */ op_getfield_ref,
    /* 0xD7 */ op_getfield_long,
    /* 0xD8 */ op_getfield_int,
    /* 0xD9 */ op_getfield_short,
    /* 0xDA */ op_getfield_jchar,
    /* 0xDB */ op_getfield_byte,
    /* 0xDC */ op_putfield_ref,
    /* 0xDD */ op_putfield_long,
    /* 0xDE */ op_putfield_int,
    /* 0xDF */ op_putfield_short,
    /* 0xE0 */ op_putfield_byte,
    /* 0xE1 */ op_invokevirtual_fast,
    /* 0xE2 */ op_invokespecial_fast,
    /* 0xE3 */ op_invokestatic_fast,
    /* 0xE4 */ op_invokeinterface_fast,
    /* 0xE5 */ op_invokedynamic_fast,
    /* 0xE6 */ op_0xF6,
    /* 0xE7 */ op_0xF,
    /* 0xE8 */ op_0xF7,
    /* 0xE9 */ op_0xF8,
    /* 0xEA */ op_0xF9,
    /* 0xEB */ op_0xFA,
    /* 0xEC */ op_0xFB,
    /* 0xED */ op_0xFC,
    /* 0xEE */ op_0xFD,
    /* 0xEF */ op_0xFE,
};


/*
 *  TAG
 *  1 UTF-8 String
 *  3 Integer
 *  4 Float
 *  5 Long
 *  6 Double
 *  7 Class reference
 *  8 String reference
 *  9 Field reference
 *  10 Method reference
 *  11 Interface methodRef garbage_refer
 *  12 Name and type descriptor
 * */
enum {
    CONSTANT_UTF8 = 1,
    CONSTANT_INTEGER = 3,
    CONSTANT_FLOAT = 4,
    CONSTANT_LONG = 5,
    CONSTANT_DOUBLE = 6,
    CONSTANT_CLASS = 7,
    CONSTANT_STRING_REF = 8,
    CONSTANT_FIELD_REF = 9,
    CONSTANT_METHOD_REF = 10,
    CONSTANT_INTERFACE_METHOD_REF = 11,
    CONSTANT_NAME_AND_TYPE = 12,
    CONSTANT_METHOD_HANDLE = 15,
    CONSTANT_METHOD_TYPE = 16,
    CONSTANT_INVOKE_DYNAMIC = 18,
};
//=======================  typedef  =============================

#if __JVM_LITTLE_ENDIAN__
typedef union _Short2Char {
    union {
        s16 s;
        u16 us;
    };
    struct {
        c8 c0;
        c8 c1;
    };
} Short2Char;

typedef union _Int2Float {
    s32 i;
    f32 f;
    struct {
        c8 c0;
        c8 c1;
        c8 c2;
        c8 c3;
    };
} Int2Float;

typedef union _Long2Double {
    f64 d;
    __refer r;
    union {
        s64 l;
        struct {
            s32 i0;
            s32 i1;
        } i2l;
    };
    struct {
        c8 c0;
        c8 c1;
        c8 c2;
        c8 c3;
        c8 c4;
        c8 c5;
        c8 c6;
        c8 c7;
    };
} Long2Double;
#elif __JVM_BIG_ENDIAN__
typedef union _Short2Char {
    s16 s;
    struct {
        c8 c1;
        c8 c0;
    };
} Short2Char;

typedef union _Int2Float {
    s32 i;
    f32 f;
    struct {
        c8 c3;
        c8 c2;
        c8 c1;
        c8 c0;
    };
} Int2Float;

typedef union _Long2Double {
    f64 d;
    __refer r;
    union {
        s64 l;
        struct {
            s32 i1;
            s32 i0;
        } i2l;
    };
    struct {
        c8 c7;
        c8 c6;
        c8 c5;
        c8 c4;
        c8 c3;
        c8 c2;
        c8 c1;
        c8 c0;
    };
} Long2Double;
#endif

typedef struct _ClassLoader ClassLoader;
typedef struct _ClassType JClass;
typedef struct _InstanceType Instance;
typedef struct _FieldInfo FieldInfo;
typedef struct _MethodInfo MethodInfo;
typedef struct _Instruction Instruction;
typedef struct _ThreadLock ThreadLock;
typedef struct _JavaThreadInfo JavaThreadInfo;
typedef struct _Runtime Runtime;
typedef struct _CodeAttribute CodeAttribute;
typedef struct _JNIENV JniEnv;
typedef struct _ReferArr CStringArr;
typedef struct _ReferArr ReferArr;
typedef struct _StackFrame RuntimeStack;


typedef s32 (*java_native_fun)(Runtime *runtime, JClass *p);

typedef void (*StaticLibRegFunc)(JniEnv *env);


enum {
    JVM_ERROR_OUTOFMEMORY,
    JVM_ERROR_VIRTUALMACHINE,
    JVM_ERROR_NOCLASSDEFFOUND,
    JVM_EXCEPTION_EOF,
    JVM_EXCEPTION_IO,
    JVM_EXCEPTION_FILENOTFOUND,
    JVM_EXCEPTION_ARRITHMETIC,
    JVM_EXCEPTION_CLASSNOTFOUND,
    JVM_EXCEPTION_NULLPOINTER,
    JVM_EXCEPTION_NOSUCHMETHOD,
    JVM_EXCEPTION_NOSUCHFIELD,
    JVM_EXCEPTION_ILLEGALARGUMENT,
    JVM_EXCEPTION_CLASSCAST,
    JVM_EXCEPTION_ARRAYINDEXOUTOFBOUNDS,
    JVM_EXCEPTION_INSTANTIATION,
};

extern char *STRS_CLASS_EXCEPTION[];

extern c8 *STR_CLASS_JAVA_LANG_STRING;
extern c8 *STR_CLASS_JAVA_LANG_STRINGBUILDER;
extern c8 *STR_CLASS_JAVA_LANG_OBJECT;
extern c8 *STR_CLASS_JAVA_LANG_THREAD;
extern c8 *STR_CLASS_JAVA_LANG_CLASS;
extern c8 *STR_CLASS_JAVA_LANG_STACKTRACE;
extern c8 *STR_CLASS_JAVA_LANG_THROWABLE;
extern c8 *STR_CLASS_JAVA_LANG_INVOKE_METHODTYPE;
extern c8 *STR_CLASS_JAVA_LANG_INVOKE_METHODHANDLE;
extern c8 *STR_CLASS_JAVA_LANG_INVOKE_METHODHANDLES_LOOKUP;
extern c8 *STR_CLASS_ORG_MINI_REFLECT_DIRECTMEMOBJ;

extern c8 *STR_FIELD_STACKFRAME;
extern c8 *STR_FIELD_NAME;
extern c8 *STR_FIELD_VALUE;
extern c8 *STR_FIELD_COUNT;
extern c8 *STR_FIELD_OFFSET;

extern c8 *STR_FIELD_CLASSHANDLE;

extern c8 *STR_METHOD_CLINIT;
extern c8 *STR_METHOD_FINALIZE;

extern c8 *STR_INS_JAVA_LANG_STRING;
extern c8 *STR_INS_JAVA_LANG_THREAD;
extern c8 *STR_INS_JAVA_LANG_CLASS;
extern c8 *STR_INS_JAVA_LANG_OBJECT;
extern c8 *STR_INS_JAVA_LANG_STACKTRACEELEMENT;

enum {
    METHOD_INVOKE_DYNAMIC,
    METHOD_INVOKE_INTERFACE,
    METHOD_INVOKE_STATIC,
    METHOD_INVOKE_VIRTUAL,
    METHOD_INVOKE_SPECIAL
};
/**
 * 内存中几个主要对象的类型，他们是不同的数据结构，但是每种类型的第一个字节都是用来标识此内存对象的类型
 */
enum {
    MEM_TYPE_NODEF = 0, //0
    MEM_TYPE_CLASS = 1, //1
    MEM_TYPE_INS = 2,   //2
    MEM_TYPE_ARR = 4   //3
};


/*
boolean   4
char  5
float  6
double 7
byte 8
short   9
int  10
long  11
  reference 12
 */
#define DATATYPE_COUNT 14
extern c8 *data_type_str;

extern s32 data_type_bytes[DATATYPE_COUNT];

enum {
    DATATYPE_BOOLEAN = 4,
    DATATYPE_JCHAR = 5,
    DATATYPE_FLOAT = 6,
    DATATYPE_DOUBLE = 7,
    DATATYPE_BYTE = 8,
    DATATYPE_SHORT = 9,
    DATATYPE_INT = 10,
    DATATYPE_LONG = 11,
    DATATYPE_REFERENCE = 12,
    DATATYPE_ARRAY = 13,
    DATATYPE_RETURNADDRESS = 14,
};
//访问标志
enum {
    ACC_PUBLIC = 0x0001,
    ACC_PRIVATE = 0x0002,
    ACC_PROTECTED = 0x0004,
    ACC_STATIC = 0x0008,
    ACC_FINAL = 0x0010,
    ACC_SYNCHRONIZED = 0x0020,
    ACC_VOLATILE = 0x0040,
    ACC_TRANSIENT = 0x0080,
    ACC_NATIVE = 0x0100,
    ACC_INTERFACE = 0x0200,
    ACC_ABSTRACT = 0x0400,
    ACC_STRICT = 0x0800,
};
//类状态
enum {
    CLASS_STATUS_RAW,
    CLASS_STATUS_LOADED,
    CLASS_STATUS_PREPARING,
    CLASS_STATUS_PREPARED,
    CLASS_STATUS_CLINITING,
    CLASS_STATUS_CLINITED,
};
//线程
enum {
    THREAD_STATUS_ZOMBIE,
    THREAD_STATUS_RUNNING,
    THREAD_STATUS_SLEEPING,
    THREAD_STATUS_MONITOR,
    THREAD_STATUS_WAIT,
};

//指令指行返回状态
enum {
    RUNTIME_STATUS_NORMAL,
    RUNTIME_STATUS_EXCEPTION,
    RUNTIME_STATUS_ERROR,
    RUNTIME_STATUS_INTERRUPT,
};

//指令指行返回状态
enum {
    JVM_STATUS_UNKNOW,
    JVM_STATUS_INITING,
    JVM_STATUS_RUNNING,
    JVM_STATUS_STOPED,
};
//======================= global var =============================
extern s32 jvm_state;

extern ClassLoader *sys_classloader;


extern JniEnv jnienv;

extern ArrayList *thread_list;

extern ArrayList *native_libs;
extern Hashtable *sys_prop;

extern s32 STACK_LENGHT_MAX;
extern s32 STACK_LENGHT_INIT;

int get_jvm_state();

void set_jvm_state(int state);

extern c8 *inst_name[];

//==============profile============
#if _JVM_DEBUG_PROFILE

#define INST_COUNT 0xEF
typedef struct _ProfileDetail {
    s64 cost;
    s32 count;
} ProfileDetail;

extern ProfileDetail profile_instructs[];
extern spinlock_t pro_lock;

void profile_init();

void profile_put(u8 instruct_code, s64 cost_add, s64 count_add);

void profile_print();

#endif

//======================= MEM_OBJ =============================

typedef struct _MemoryBlock {

    JClass *clazz;
    struct _MemoryBlock *next; //reg for gc
    struct _MemoryBlock *tmp_next;  //hold by thread
    ThreadLock *volatile thread_lock;

    u8 type;//type of array or object runtime,class
    u8 garbage_mark;
    u8 garbage_reg;
    u8 arr_type_index;
} MemoryBlock;

struct _ClassLoader {
    ArrayList *classpath;
    Hashtable *classes;

    Hashtable *table_jstring_const;


    //
    spinlock_t lock;
};

void memoryblock_destory(__refer ref);

void classloader_release_classs_static_field(ClassLoader *class_loader);

void classloader_destory(ClassLoader *class_loader);

void classloader_add_jar_path(ClassLoader *class_loader, Utf8String *jarPath);


//======================= class file =============================


/* Java Class File */
typedef struct _ClassFileFormat {
    u8 magic_number[4];
    u16 minor_version;
    u16 major_version;
    u16 constant_pool_count;
    /* constant pool */
    u16 access_flags;
    u16 this_class;
    u16 super_class;

    u16 interface_count;
    /* interfaceRef pool */
    u16 fields_count;
    /* obj_fields pool */
    u16 methods_count;
    /* methodRef pool */
    u16 attributes_count;
    /* attributes pool */
} ClassFileFormat;


typedef struct _ConstantType {
    s16 index;
    s16 tag;
} ConstantItem;

//====
typedef struct _ConstantNameAndType {
    ConstantItem item;
    u16 nameIndex;
    u16 typeIndex;
} ConstantNameAndType;

//====

typedef struct _ConstantUTF8 {
    ConstantItem item;
    u16 string_size;
    //
    Utf8String *utfstr;
    Instance *jstr;
} ConstantUTF8;

typedef struct _ConstantInteger {
    ConstantItem item;
    s32 value;
} ConstantInteger;

typedef struct _ConstantFloat {
    ConstantItem item;
    f32 value;

} ConstantFloat;

typedef struct _ConstantLong {
    ConstantItem item;
    s64 value;
} ConstantLong;

typedef struct _ConstantDouble {
    ConstantItem item;
    f64 value;
} ConstantDouble;

typedef struct _ConstantClassRef {
    ConstantItem item;
    u16 stringIndex;

    //
    Utf8String *name;
    JClass *clazz;

} ConstantClassRef;

typedef struct _ConstantStringRef {
    ConstantItem item;
    u16 stringIndex;
} ConstantStringRef;

typedef struct _ConstantFieldRef {
    ConstantItem item;
    u16 classIndex;
    u16 nameAndTypeIndex;
    //
    FieldInfo *fieldInfo;
    ConstantNameAndType *nameAndType;
    Utf8String *name;
    Utf8String *descriptor;
    Utf8String *clsName;
} ConstantFieldRef;

typedef struct _ConstantMethodRef {
    ConstantItem item;
    u16 classIndex;
    u16 nameAndTypeIndex;
    //
    MethodInfo *methodInfo;
    s32 para_slots;
    ConstantNameAndType *nameAndType;
    Utf8String *name;
    Utf8String *descriptor;
    Utf8String *clsName;
    Pairlist *virtual_methods;
} ConstantMethodRef, ConstantInterfaceMethodRef;

typedef struct _ConstantMethodHandle {
    ConstantItem item;
    u8 reference_kind;
    u16 reference_index;
} ConstantMethodHandle;

typedef struct _ConstantMethodType {
    ConstantItem item;
    u16 descriptor_index;
} ConstantMethodType;

typedef struct _ConstantInvokeDynamic {
    ConstantItem item;
    u16 bootstrap_method_attr_index;
    u16 nameAndTypeIndex;
} ConstantInvokeDynamic;


//============================================

typedef struct _ConstantPool {
    ArrayList *utf8CP;
    ArrayList *classRef;
    ArrayList *stringRef;
    ArrayList *fieldRef;
    ArrayList *methodRef;
    ArrayList *interfaceMethodRef;
} ConstantPool;
//============================================

typedef struct _InterfacePool {
    ConstantClassRef *clasz;
    s32 clasz_used;
} InterfacePool;
//============================================

typedef struct _AttributeInfo {
    u16 attribute_name_index;
    s32 attribute_length;
    u8 *info;

} AttributeInfo;

//============================================

typedef struct _line_number {
    u16 start_pc;
    u16 line_number;
} LineNumberTable;

typedef struct _ExceptionTable {
    u16 start_pc;
    u16 end_pc;
    u16 handler_pc;
    u16 catch_type;
} ExceptionTable;

typedef struct _LocalVarTable {
    u16 start_pc;
    u16 length;
    u16 name_index;
    u16 descriptor_index;
    u16 index;
} LocalVarTable;

struct _CodeAttribute {
    u16 attribute_name_index;
    s32 attribute_length;
    u16 max_stack;
    u16 max_locals;
    s32 code_length;
    u8 *code; // [code_length];
    u16 exception_table_length;
    ExceptionTable *exception_table; //[exception_table_length];
    u16 line_number_table_length;
    LineNumberTable *line_number_table;
    u16 local_var_table_length;
    LocalVarTable *local_var_table;

};

//============================================

typedef struct _BootstrapMethod {
    u16 bootstrap_method_ref;
    u16 num_bootstrap_arguments;
    u16 *bootstrap_arguments;

    //cache
    MethodInfo *make;
} BootstrapMethod;

typedef struct BootstrapMethods_attribute {
    u16 num_bootstrap_methods;
    BootstrapMethod *bootstrap_methods;
} BootstrapMethodsAttr;
//============================================

struct _FieldInfo {
    u16 access_flags;
    u16 name_index;
    u16 descriptor_index;
    u16 attributes_count;
    AttributeInfo *attributes;
    //link
    ConstantItem *const_value_item;
    Utf8String *name;
    Utf8String *descriptor;
    JClass *_this_class;
    u16 offset;//字段的偏移地址，静态字段存放在class中
    u16 offset_instance;
    //
    u8 datatype_idx;
    u8 isrefer;
    u8 datatype_bytes;
    u8 isvolatile;
};

typedef struct _FieldPool {
    FieldInfo *field;
    s32 field_used;
} FieldPool;
//============================================

struct _MethodInfo {
    AttributeInfo *attributes;
    //
    CodeAttribute *converted_code;
    //link
    Utf8String *name;
    Utf8String *descriptor;
    Utf8String *paraType;
    Utf8String *returnType;
    JClass *_this_class;
    java_native_fun native_func;
    Pairlist *breakpoint;
    s16 para_slots;
    s16 para_count_with_this;
    s16 return_slots;
    s16 na_1;
    //
    u16 access_flags;
    u16 name_index;
    u16 descriptor_index;
    u16 attributes_count;
    //
    u8 is_native;
    u8 is_sync;
    u8 is_static;
    u8 na_2;
};
//============================================

typedef struct _MethodPool {
    MethodInfo *method;
    s32 method_used;
} MethodPool;
//============================================

typedef struct _AttributePool {
    AttributeInfo *attribute;
    s32 attribute_used;

} AttributePool;


//======================= class =============================

/*
 Gust 20170719 add Class define
 */
struct _ClassType {
    MemoryBlock mb;
    JClass *superclass;
    __refer *constant_item_ptr;//存放常量池项目地址
    s32 constant_item_count;//总数

    //类变量及实例变量的参数
    s32 field_instance_start;//实例变量模板起始起址，继承自父类的变量放在前面
    s32 field_instance_len; //非静态变量长度
    s32 field_static_len; //静态变量内存长度
    c8 *field_static; //静态变量内存地址

    //
    Instance *ins_class; //object of java.lang.Class
    Instance *jClassLoader;// java classloader

    //public:
    s32 (*_load_class_from_bytes)(struct _ClassType *_this, ByteBuf *buf);

    //
    Utf8String *source;
    BootstrapMethodsAttr *bootstrapMethodAttr;

    //
    Utf8String *name;
    MethodInfo *finalizeMethod;
    ClassFileFormat cff;
    ConstantPool constantPool;
    InterfacePool interfacePool;
    FieldPool fieldPool;
    MethodPool methodPool;
    AttributePool attributePool;

    //for array class
    Pairlist *arr_class_type;//for object array create speedup,left is utf8 index of class, right is arr class
    ArrayList *insFieldPtrIndex;//for optmize , save object pointer field index
    ArrayList *staticFieldPtrIndex; //save static field index

    //
    s8 status;
    u8 primitive;//primitive data type int/long/short/char/short/byte/float/double
};


s32 _DESTORY_CLASS(JClass *clazz);

JClass *class_create(Runtime *runtime);

JClass *getSuperClass(JClass *clazz);

void constant_list_create(JClass *clazz);

void constant_list_destory(JClass *clazz);

s32 class_destory(JClass *clazz);

JClass *class_parse(ByteBuf *bytebuf, Runtime *runtime);

JClass *load_class(ClassLoader *loader, Utf8String *pClassName, Runtime *runtime);

s32 _LOAD_CLASS_FROM_BYTES(JClass *_this, ByteBuf *buf);

s32 class_prepar(JClass *clazz, Runtime *runtime);

void _class_optimize(JClass *clazz);

void class_clinit(JClass *clazz, Runtime *runtime);

void printClassFileFormat(ClassFileFormat *cff);

s32 _class_method_info_destory(JClass *clazz);

s32 _class_attribute_info_destory(JClass *clazz);

s32 _class_interface_pool_destory(JClass *clazz);

s32 _class_constant_pool_destory(JClass *clazz);

s32 _class_field_info_destory(JClass *clazz);

u8 instance_of(JClass *clazz, Instance *ins, Runtime *runtime);

u8 isSonOfInterface(JClass *clazz, JClass *son, Runtime *runtime);

u8 assignable_from(JClass *clazzSon, JClass *clazzSuper);

void class_clear_refer(JClass *clazz);


//======================= instance =============================


struct _InstanceType {
    MemoryBlock mb;
    //
    union {
        c8 *obj_fields; //object fieldRef body
        c8 *arr_body;//array body
    };
    s32 arr_length;

};


Instance *instance_create(Runtime *runtime, JClass *clazz);

void instance_init(Instance *ins, Runtime *runtime);

void instance_init_methodtype(Instance *ins, Runtime *runtime, c8 *methodtype, RuntimeStack *para);

void instance_finalize(Instance *ins, Runtime *runtime);

s32 instance_destory(Instance *instance);

Instance *instance_copy(Runtime *runtime, Instance *src, s32 deep_copy);

//======================= bytecode =============================


/* find UTF8 */
static inline ConstantUTF8 *class_get_constant_utf8(JClass *clazz, s32 index) {
    return (ConstantUTF8 *) (clazz->constant_item_ptr[index]);
}

/* Find Class Reference */
static inline ConstantStringRef *class_get_constant_stringref(JClass *clazz, s32 index) {
    return (ConstantStringRef *) (clazz->constant_item_ptr[index]);
}


/* Find Class Reference */
static inline ConstantClassRef *class_get_constant_classref(JClass *clazz, s32 index) {
    return (ConstantClassRef *) (clazz->constant_item_ptr[index]);
}

static inline ConstantFieldRef *class_get_constant_fieldref(JClass *clazz, s32 index) {
    return (ConstantFieldRef *) (clazz->constant_item_ptr[index]);
}

static inline ConstantItem *class_get_constant_item(JClass *clazz, s32 index) {
    return (ConstantItem *) (clazz->constant_item_ptr[index]);
}

/* Find Method Reference */
static inline ConstantMethodRef *class_get_constant_method_ref(JClass *clazz, s32 index) {
    return (ConstantMethodRef *) (clazz->constant_item_ptr[index]);
}

static inline ConstantInterfaceMethodRef *
class_get_constant_interface_method_ref(JClass *clazz, s32 index) {
    return (ConstantInterfaceMethodRef *) (clazz->constant_item_ptr[index]);
}

/* Find Name and Type Reference */
static inline ConstantNameAndType *class_get_constant_name_and_type(JClass *clazz, s32 index) {
    return (ConstantNameAndType *) (clazz->constant_item_ptr[index]);
}

/* get integer from constant pool */
static inline s32 class_get_constant_integer(JClass *clazz, s32 index) {
    return ((ConstantInteger *) (clazz->constant_item_ptr[index]))->value;
}

/* get long from constant pool */
static inline s64 class_get_constant_long(JClass *clazz, s32 index) {
    return ((ConstantLong *) (clazz->constant_item_ptr[index]))->value;
}

/* get f32 from constant pool */
static inline f32 class_get_constant_float(JClass *clazz, s32 index) {
    return ((ConstantFloat *) (clazz->constant_item_ptr[index]))->value;
}

/* get f64 from constant pool */
static inline f64 class_get_double_from_constant_pool(JClass *clazz, s32 index) {
    return ((ConstantDouble *) (clazz->constant_item_ptr[index]))->value;
}

static inline Utf8String *class_get_utf8_string(JClass *clazz, s32 index) {
    return ((ConstantUTF8 *) (clazz->constant_item_ptr[index]))->utfstr;
}

static inline ConstantMethodHandle *class_get_method_handle(JClass *clazz, s32 index) {
    return (ConstantMethodHandle *) (clazz->constant_item_ptr[index]);
}

static inline ConstantMethodType *class_get_method_type(JClass *clazz, s32 index) {
    return (ConstantMethodType *) (clazz->constant_item_ptr[index]);
}

static inline ConstantInvokeDynamic *class_get_invoke_dynamic(JClass *clazz, s32 index) {
    return (ConstantInvokeDynamic *) (clazz->constant_item_ptr[index]);
}

MethodInfo *
find_instance_methodInfo_by_name(Instance *ins, Utf8String *methodName, Utf8String *methodType, Runtime *runtime);

MethodInfo *find_methodInfo_by_methodref(JClass *clazz, s32 method_ref, Runtime *runtime);

MethodInfo *
find_methodInfo_by_name(Utf8String *clsName, Utf8String *methodName, Utf8String *methodType, Runtime *runtime);

MethodInfo *find_methodInfo_by_name_c(c8 *pclsName, c8 *pmethodName, c8 *pmethodType, Runtime *runtime);

FieldInfo *find_fieldInfo_by_fieldref(JClass *clazz, s32 field_ref, Runtime *runtime);

FieldInfo *find_fieldInfo_by_name_c(c8 *pclsName, c8 *pfieldName, c8 *pfieldType, Runtime *runtime);

FieldInfo *find_fieldInfo_by_name(Utf8String *clsName, Utf8String *fieldName, Utf8String *fieldType, Runtime *runtime);

//


JClass *getClassByConstantClassRef(JClass *clazz, s32 index, Runtime *runtime);

//======================= runtime =============================

/* Stack Frame */
#define STACK_ENTRY_NONE        0
#define STACK_ENTRY_INT         1
#define STACK_ENTRY_FLOAT       2
#define STACK_ENTRY_LONG        4
#define STACK_ENTRY_DOUBLE      8
#define STACK_ENTRY_REF         16
#define STACK_ENTRY_RETURNADDRESS         32

typedef struct _StackEntry {
    union {
        s64 lvalue;
        f64 dvalue;
        f32 fvalue;
        s32 ivalue;
        __refer rvalue;
        Instance *ins;
    };
    s32 type;
} StackEntry, LocalVarItem;

struct _StackFrame {
    StackEntry *store;
    StackEntry *sp;
    s32 max_size;
};


struct _Runtime {

    MethodInfo *method;
    JClass *clazz;
    u8 *pc;
    JavaThreadInfo *threadInfo;
    MemoryBlock *lock;
    Runtime *son;//sub method's runtime
    Runtime *parent;//father method's runtime
    RuntimeStack *stack;
    LocalVarItem *localvar;
    s32 invoke_type;
    s16 localvar_slots;

    //
    union {
        Runtime *runtime_pool_header;// cache runtimes for performance
        Runtime *next;  //for runtime pools linklist
    };

    JniEnv *jnienv;
};

//======================= stack =============================
RuntimeStack *stack_create(s32 entry_size);

void stack_destory(RuntimeStack *stack);

s32 stack_expand(RuntimeStack *stack);

void push_entry_jni(RuntimeStack *stack, StackEntry *entry);

void push_int_jni(RuntimeStack *stack, s32 value);

void push_long_jni(RuntimeStack *stack, s64 value);

void push_double_jni(RuntimeStack *stack, f64 value);

void push_float_jni(RuntimeStack *stack, f32 value);

void push_ref_jni(RuntimeStack *stack, __refer value);

__refer pop_ref_jni(RuntimeStack *stack);

s32 pop_int_jni(RuntimeStack *stack);

s64 pop_long_jni(RuntimeStack *stack);

f64 pop_double_jni(RuntimeStack *stack);

f32 pop_float_jni(RuntimeStack *stack);

void pop_entry_jni(RuntimeStack *stack, StackEntry *entry);

void pop_empty_jni(RuntimeStack *stack);

s32 entry_2_int_jni(StackEntry *entry);

void peek_entry_jni(StackEntry *src, StackEntry *dst);

s64 entry_2_long_jni(StackEntry *entry);

__refer entry_2_refer_jni(StackEntry *entry);

static inline s32 stack_size(RuntimeStack *stack) {
    return (stack->sp - stack->store);
}

/* push Integer */
static inline void push_int(RuntimeStack *stack, s32 value) {
    stack->sp->ivalue = value;//clear 64bit
    stack->sp->type = STACK_ENTRY_INT;
    stack->sp++;
}


/* pop Integer */
static inline s32 pop_int(RuntimeStack *stack) {
    return (--stack->sp)->ivalue;
}

/* push Double */
static inline void push_double(RuntimeStack *stack, f64 value) {
    stack->sp->dvalue = value;
    stack->sp->type = STACK_ENTRY_DOUBLE;
    stack->sp++;
    stack->sp->type = STACK_ENTRY_DOUBLE;
//    ptr->dvalue = value;
    stack->sp++;
}

/* pop Double */
static inline f64 pop_double(RuntimeStack *stack) {
    stack->sp -= 2;
    return stack->sp->dvalue;
}

/* push Float */
static inline void push_float(RuntimeStack *stack, f32 value) {
    //ptr->lvalue = 0;//clear 64bit
    stack->sp->fvalue = value;
    stack->sp->type = STACK_ENTRY_FLOAT;
    stack->sp++;
}

/* pop Float */
static inline f32 pop_float(RuntimeStack *stack) {
    return (--stack->sp)->fvalue;
}


/* push Long */
static inline void push_long(RuntimeStack *stack, s64 value) {
    stack->sp->lvalue = value;
    stack->sp->type = STACK_ENTRY_LONG;
    stack->sp++;
    stack->sp->type = STACK_ENTRY_LONG;
//    ptr->lvalue = value;
    stack->sp++;
}

/* pop Long */
static inline s64 pop_long(RuntimeStack *stack) {
    stack->sp -= 2;
    return stack->sp->lvalue;
}

/* push Ref */
static inline void push_ref(RuntimeStack *stack, __refer value) {
    stack->sp->type = STACK_ENTRY_REF;
    stack->sp->rvalue = value;
    stack->sp++;
}

static inline __refer pop_ref(RuntimeStack *stack) {
    return (--stack->sp)->rvalue;
}


/* push ReturnAddress */
static inline void push_ra(RuntimeStack *stack, __refer value) {
    stack->sp->type = STACK_ENTRY_RETURNADDRESS;
    stack->sp->rvalue = value;
    stack->sp++;
}

static inline __refer pop_ra(RuntimeStack *stack) {
    return (--stack->sp)->rvalue;
}


static inline void push_entry(RuntimeStack *stack, StackEntry *entry) {
    stack->sp->lvalue = entry->lvalue;
    stack->sp->type = entry->type;
    stack->sp++;
}

/* Pop Stack Entry */
static inline void pop_entry(RuntimeStack *stack, StackEntry *entry) {
    stack->sp--;
    entry->lvalue = stack->sp->lvalue;
    entry->type = stack->sp->type;

}

static inline void pop_empty(RuntimeStack *stack) {
    stack->sp--;
}


static inline void peek_entry(StackEntry *src, StackEntry *dst) {
    dst->lvalue = src->lvalue;
    dst->type = src->type;
}


/* Entry to Int */
static inline s32 entry_2_int(StackEntry *entry) {
    return entry->ivalue;
}

static inline s64 entry_2_long(StackEntry *entry) {
    return entry->lvalue;
}

static inline __refer entry_2_refer(StackEntry *entry) {
    return entry->rvalue;
}

static inline s32 is_cat1(StackEntry *entry) {
    if (entry->type & (STACK_ENTRY_INT | STACK_ENTRY_FLOAT | STACK_ENTRY_REF)) {
        return 1;
    }
    return 0;
}

static inline s32 is_cat2(StackEntry *entry) {
    if (entry->type & (STACK_ENTRY_LONG | STACK_ENTRY_DOUBLE)) {
        return 1;
    }
    return 0;
}

s32 is_ref(StackEntry *entry);


//======================= localvar =============================

Runtime *runtime_create(Runtime *parent);

void runtime_destory(Runtime *runtime);

Runtime *getLastSon(Runtime *top);

Runtime *getTopRuntime(Runtime *runtime);

s64 getInstructPointer(Runtime *runtime);

void getRuntimeStack(Runtime *runtime, Utf8String *ustr);

s32 getRuntimeDepth(Runtime *top);

static inline void localvar_init(Runtime *runtime, s32 var_slots, s32 para_slots) {
    s32 max_slots = var_slots > para_slots ? var_slots : para_slots;
    RuntimeStack *stack = runtime->stack;
    runtime->localvar = stack->sp - para_slots;
    runtime->localvar_slots = max_slots;
    s32 reserve_slots = max_slots - para_slots;
    if (reserve_slots) {
        memset(&stack->store[stack_size(stack)], 0, reserve_slots * sizeof(StackEntry));
        stack->sp += reserve_slots;
    }
}

static inline void localvar_dispose(Runtime *runtime) {
    runtime->stack->sp = runtime->localvar;
}

static inline StackEntry *localvar_getEntry(LocalVarItem *localvar, s32 index) {
    return &localvar[index];
}

static inline void localvar_setEntry(LocalVarItem *localvar, s32 index, StackEntry *entry) {
    localvar[index].lvalue = entry->lvalue;
    localvar[index].type = entry->type;
}

static inline void localvar_setInt(LocalVarItem *localvar, s32 index, s32 val) {
    localvar[index].ivalue = val;
    localvar[index].type = STACK_ENTRY_INT;
}

static inline s32 localvar_getInt(LocalVarItem *localvar, s32 index) {
    return localvar[index].ivalue;
}

static inline void localvar_setLong(LocalVarItem *localvar, s32 index, s64 val) {
    localvar[index].lvalue = val;
    localvar[index].type = STACK_ENTRY_LONG;
}

static inline s64 localvar_getLong(LocalVarItem *localvar, s32 index) {
    return localvar[index].lvalue;
}

static inline void localvar_setRefer(LocalVarItem *localvar, s32 index, __refer val) {
    localvar[index].rvalue = val;
    localvar[index].type = STACK_ENTRY_REF;
}

static inline __refer localvar_getRefer(LocalVarItem *localvar, s32 index) {
    return localvar[index].rvalue;
}


void localvar_setInt_jni(LocalVarItem *localvar, s32 index, s32 val);

void localvar_setRefer_jni(LocalVarItem *localvar, s32 index, __refer val);

s32 localvar_getInt_jni(LocalVarItem *localvar, s32 index);

__refer localvar_getRefer_jni(LocalVarItem *localvar, s32 index);

s64 localvar_getLong_2slot_jni(LocalVarItem *localvar, s32 index);

void localvar_setLong_2slot_jni(LocalVarItem *localvar, s32 index, s64 val);

//======================= other =============================
//======================= execute =============================
//
void open_log(void);

void close_log(void);

c8 *getMajorVersionString(u16 major_number);


void jvm_init(c8 *p_classpath, StaticLibRegFunc regFunc);

void jvm_destroy(StaticLibRegFunc unRegFunc);

void thread_boundle(Runtime *runtime);

void thread_unboundle(Runtime *runtime);

void print_exception(Runtime *runtime);

s32 execute_jvm(c8 *p_classpath, c8 *mainclass, ArrayList *java_para);

s32 call_method_main(c8 *p_mainclass, c8 *p_methodname, c8 *p_methodtype, ArrayList *java_para);

s32 call_method_c(c8 *p_mainclass, c8 *p_methodname, c8 *p_methodtype, Runtime *runtime);

s32 execute_method_impl(MethodInfo *method, Runtime *runtime);

s32 execute_method(MethodInfo *method, Runtime *runtime);


//======================= jni =============================
typedef struct _java_native_method {
    c8 *clzname;
    c8 *methodname;
    c8 *methodtype;
    java_native_fun func_pointer;
} java_native_method;

java_native_method *find_native_method(c8 *cls_name, c8 *method_name, c8 *method_type);

s32 invoke_native_method(Runtime *runtime, JClass *p,
                         c8 *cls_name, c8 *method_name, c8 *type);


typedef struct _JavaNativeLib {
    java_native_method *methods;
    s32 methods_count;
} JavaNativeLib;


struct _ReferArr {
    __refer *arr_body;
    s32 arr_length;
};

s32 native_reg_lib(java_native_method *methods, s32 method_size);

s32 native_remove_lib(JavaNativeLib *lib);

s32 native_lib_destory(void);

void reg_std_native_lib(void);

void reg_net_native_lib(void);

void reg_jdwp_native_lib(void);

void init_jni_func_table();

struct _JNIENV {
    s32 *data_type_bytes;

    s32 (*native_reg_lib)(java_native_method *methods, s32 method_size);

    s32 (*native_remove_lib)(JavaNativeLib *lib);

    void (*push_entry)(RuntimeStack *stack, StackEntry *entry);

    void (*push_int)(RuntimeStack *stack, s32 value);

    void (*push_long)(RuntimeStack *stack, s64 value);

    void (*push_double)(RuntimeStack *stack, f64 value);

    void (*push_float)(RuntimeStack *stack, f32 value);

    void (*push_ref)(RuntimeStack *stack, __refer value);

    __refer (*pop_ref)(RuntimeStack *stack);

    s32 (*pop_int)(RuntimeStack *stack);

    s64 (*pop_long)(RuntimeStack *stack);

    f64 (*pop_double)(RuntimeStack *stack);

    f32 (*pop_float)(RuntimeStack *stack);

    void (*pop_entry)(RuntimeStack *stack, StackEntry *entry);

    void (*pop_empty)(RuntimeStack *stack);

    s32 (*entry_2_int)(StackEntry *entry);

    void (*peek_entry)(StackEntry *src, StackEntry *dst);

    s64 (*entry_2_long)(StackEntry *entry);

    __refer (*entry_2_refer)(StackEntry *entry);

    void (*localvar_setRefer)(LocalVarItem *localvar, s32 index, __refer val);

    void (*localvar_setInt)(LocalVarItem *localvar, s32 index, s32 val);

    __refer (*localvar_getRefer)(LocalVarItem *localvar, s32 index);

    s32 (*localvar_getInt)(LocalVarItem *localvar, s32 index);

    s64 (*localvar_getLong_2slot)(LocalVarItem *localvar, s32 index);

    void (*localvar_setLong_2slot)(LocalVarItem *localvar, s32 index, s64 val);

    void (*jthread_block_enter)(Runtime *runtime);

    void (*jthread_block_exit)(Runtime *runtime);

    Utf8String *(*utf8_create)();

    Utf8String *(*utf8_create_part_c)(char *str, int start, int len);

    char *(*utf8_cstr)(Utf8String *a1);

    void (*utf8_destory)(Utf8String *);

    Instance *(*jstring_create)(Utf8String *src, Runtime *runtime);

    Instance *(*jstring_create_cstr)(c8 *cstr, Runtime *runtime);

    s32 (*jstring_2_utf8)(Instance *jstr, Utf8String *utf8);

    CStringArr *(*cstringarr_create)(Instance *jstr_arr);

    void (*cstringarr_destory)(CStringArr *cstr_arr);

    ReferArr *(*referarr_create)(Instance *jobj_arr);

    void (*referarr_destory)(CStringArr *ref_arr);

    void (*referarr_2_jlongarr)(ReferArr *ref_arr, Instance *jlong_arr);

    Instance *(*jarray_create_by_type_name)(Runtime *runtime, s32 count, Utf8String *type);

    Instance *(*jarray_create_by_type_index)(Runtime *runtime, s32 count, s32 typeIdx);

    void (*jarray_set_field)(Instance *arr, s32 index, s64 val);

    s64 (*jarray_get_field)(Instance *arr, s32 index);

    void *(*jvm_calloc)(u32 size);

    void *(*jvm_malloc)(u32 size);

    void (*jvm_free)(void *ptr);

    void *(*jvm_realloc)(void *pPtr, u32 size);

    void (*instance_release_from_thread)(Instance *ref, Runtime *runtime);

    void (*instance_hold_to_thread)(Instance *ref, Runtime *runtime);

    s32 (*execute_method)(MethodInfo *method, Runtime *runtime);

    MethodInfo *
    (*find_methodInfo_by_name)(Utf8String *clsName, Utf8String *methodName, Utf8String *methodType, Runtime *runtime);

    void (*print_exception)(Runtime *runtime);

    void (*garbage_refer_hold)(__refer ref);

    void (*garbage_refer_release)(__refer ref);

    Runtime *(*runtime_create)(Runtime *parent);

    void (*runtime_destory)(Runtime *runtime);

    Runtime *(*getLastSon)(Runtime *top);

    void (*thread_boundle)(Runtime *runtime);

    void (*thread_unboundle)(Runtime *runtime);

    thrd_t (*thrd_current)(void);

    Pairlist *(*pairlist_create)(s32 len);

    __refer (*pairlist_get)(Pairlist *list, __refer left);

    s32 (*pairlist_put)(Pairlist *list, __refer left, __refer right);

    s32 (*jthread_get_daemon_value)(Instance *ins, Runtime *runtime);

    void (*jthread_set_daemon_value)(Instance *ins, Runtime *runtime, s32 daemon);

    s32 (*get_jvm_state)();

};



//=======================   =============================


#ifdef __cplusplus
}
#endif


#endif
