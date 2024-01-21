package com.gx.gany;

import java.util.HashMap;

public class GAny implements Cloneable {
    static {
        System.loadLibrary("gany-droid");
        nGAnyInit();
    }

    public interface GAnyFunction {
        GAny call(GAny... args);
    }

    private static final HashMap<Long, GAnyFunction> mGAnyFunctions = new HashMap<>();

    private long mNativePtr;

    public GAny() {
        mNativePtr = nGAnyCreateUndefined();
    }

    public GAny(GAny v) {
        mNativePtr = nGAnyCreate(v.mNativePtr);
    }

    public GAny(int v) {
        mNativePtr = nGAnyCreateInt32(v);
    }

    public GAny(long v) {
        mNativePtr = nGAnyCreateInt64(v);
    }

    public GAny(byte v) {
        mNativePtr = nGAnyCreateInt8(v);
    }

    public GAny(boolean v) {
        mNativePtr = nGAnyCreateBool(v);
    }

    public GAny(float v) {
        mNativePtr = nGAnyCreateFloat(v);
    }

    public GAny(double v) {
        mNativePtr = nGAnyCreateDouble(v);
    }

    public GAny(String v) {
        mNativePtr = nGAnyCreateString(v);
    }

    @Override
    protected void finalize() throws Throwable {
        nGAnyDestroy(mNativePtr);
        super.finalize();
    }


    public String toString() {
        return nGAnyToString(mNativePtr);
    }


    @Override
    public GAny clone() {
        return createFromPtr(nGAnyClone(mNativePtr));
    }

    private GAny call(GAny... args) {
        long[] argsPtr = new long[args.length];
        for (int i = 0; i < args.length; i++) {
            argsPtr[i] = args[i].mNativePtr;
        }
        long retPtr = nGAnyCallFunction(mNativePtr, argsPtr);
        return createFromPtr(retPtr);
    }

    public GAny call(Object... args) {
        GAny[] argsAny = new GAny[args.length];
        for (int i = 0; i < args.length; i++) {
            argsAny[i] = create(args[i]);
        }
        return call(argsAny);
    }

    private GAny callMethod(String methodName, GAny... args) {
        long[] argsPtr = new long[args.length];
        for (int i = 0; i < args.length; i++) {
            argsPtr[i] = args[i].mNativePtr;
        }
        long retPtr = nGAnyCallMethod(mNativePtr, methodName, argsPtr);
        return createFromPtr(retPtr);
    }

    public GAny callMethod(String methodName, Object... args) {
        GAny[] argsAny = new GAny[args.length];
        for (int i = 0; i < args.length; i++) {
            argsAny[i] = create(args[i]);
        }
        return callMethod(methodName, argsAny);
    }

    public GAny New(Object... args) {
        if (isClass()) {
            return call(args);
        }
        throw new RuntimeException("Can only be used for new of class");
    }

    public static GAny Import(String path) {
        return createFromPtr(nGAnyImport(path));
    }

    public static void Export(GAny clazz)
    {
        nGAnyExport(clazz.mNativePtr);
    }

    public static GAny create(Object v) {
        if (v == null) {
            return Null();
        }
        if (v instanceof Integer) {
            return new GAny((int) v);
        }
        if (v instanceof Long) {
            return new GAny((long) v);
        }
        if (v instanceof Byte) {
            return new GAny((byte) v);
        }
        if (v instanceof Boolean) {
            return new GAny((boolean) v);
        }
        if (v instanceof Float) {
            return new GAny((float) v);
        }
        if (v instanceof Double) {
            return new GAny((double) v);
        }
        if (v instanceof String) {
            return new GAny((String) v);
        }
        if (v instanceof GAny) {
            return (GAny) v;
        }
        if (v instanceof GAnyFunction) {
            return function((GAnyFunction) v);
        }
        throw new RuntimeException("GAny.create: unsupported type");
    }

    public static GAny undefined() {
        return createFromPtr(nGAnyCreateUndefined());
    }

    public static GAny Null() {
        return createFromPtr(nGAnyCreateNull());
    }

    public static GAny array() {
        return createFromPtr(nGAnyCreateArray());
    }

    public static GAny object() {
        return createFromPtr(nGAnyCreateObject());
    }

    public interface GAnyFuncRT0 {
        GAny call();
    }

    public interface GAnyFuncRT1 {
        GAny call(GAny v0);
    }

    public interface GAnyFuncRT2 {
        GAny call(GAny v0, GAny v1);
    }

    public interface GAnyFuncRT3 {
        GAny call(GAny v0, GAny v1, GAny v2);
    }

    public interface GAnyFuncRT4 {
        GAny call(GAny v0, GAny v1, GAny v2, GAny v3);
    }

    public interface GAnyFuncRT5 {
        GAny call(GAny v0, GAny v1, GAny v2, GAny v3, GAny v4);
    }

    public interface GAnyFuncRT6 {
        GAny call(GAny v0, GAny v1, GAny v2, GAny v3, GAny v4, GAny v5);
    }

    public interface GAnyFuncRT7 {
        GAny call(GAny v0, GAny v1, GAny v2, GAny v3, GAny v4, GAny v5, GAny v6);
    }

    public interface GAnyFuncRT8 {
        GAny call(GAny v0, GAny v1, GAny v2, GAny v3, GAny v4, GAny v5, GAny v6, GAny v7);
    }

    public interface GAnyFuncT0 {
        void call();
    }

    public interface GAnyFuncT1 {
        void call(GAny v0);
    }

    public interface GAnyFuncT2 {
        void call(GAny v0, GAny v1);
    }

    public interface GAnyFuncT3 {
        void call(GAny v0, GAny v1, GAny v2);
    }

    public interface GAnyFuncT4 {
        void call(GAny v0, GAny v1, GAny v2, GAny v3);
    }

    public interface GAnyFuncT5 {
        void call(GAny v0, GAny v1, GAny v2, GAny v3, GAny v4);
    }

    public interface GAnyFuncT6 {
        void call(GAny v0, GAny v1, GAny v2, GAny v3, GAny v4, GAny v5);
    }

    public interface GAnyFuncT7 {
        void call(GAny v0, GAny v1, GAny v2, GAny v3, GAny v4, GAny v5, GAny v6);
    }

    public interface GAnyFuncT8 {
        void call(GAny v0, GAny v1, GAny v2, GAny v3, GAny v4, GAny v5, GAny v6, GAny v7);
    }

    public static GAny function(GAnyFunction func) {
        return createFromPtr(registerGAnyFunction(func));
    }

    public static GAny function(GAnyFuncRT0 func) {
        return function((GAnyFunction) args -> func.call());
    }

    public static GAny function(GAnyFuncT0 func) {
        return function((GAnyFunction) args -> {
            func.call();
            return undefined();
        });
    }

    public static GAny function(GAnyFuncRT1 func) {
        return function((GAnyFunction) args -> func.call(args[0]));
    }

    public static GAny function(GAnyFuncT1 func) {
        return function((GAnyFunction) args -> {
            func.call(args[0]);
            return undefined();
        });
    }

    public static GAny function(GAnyFuncRT2 func) {
        return function((GAnyFunction) args -> func.call(args[0], args[1]));
    }

    public static GAny function(GAnyFuncT2 func) {
        return function((GAnyFunction) args -> {
            func.call(args[0], args[1]);
            return undefined();
        });
    }

    public static GAny function(GAnyFuncRT3 func) {
        return function((GAnyFunction) args -> func.call(args[0], args[1], args[2]));
    }

    public static GAny function(GAnyFuncT3 func) {
        return function((GAnyFunction) args -> {
            func.call(args[0], args[1], args[2]);
            return undefined();
        });
    }

    public static GAny function(GAnyFuncRT4 func) {
        return function((GAnyFunction) args -> func.call(args[0], args[1], args[2], args[3]));
    }

    public static GAny function(GAnyFuncT4 func) {
        return function((GAnyFunction) args -> {
            func.call(args[0], args[1], args[2], args[3]);
            return undefined();
        });
    }

    public static GAny function(GAnyFuncRT5 func) {
        return function((GAnyFunction) args -> func.call(args[0], args[1], args[2], args[3], args[4]));
    }

    public static GAny function(GAnyFuncT5 func) {
        return function((GAnyFunction) args -> {
            func.call(args[0], args[1], args[2], args[3], args[4]);
            return undefined();
        });
    }

    public static GAny function(GAnyFuncRT6 func) {
        return function((GAnyFunction) args -> func.call(args[0], args[1], args[2], args[3], args[4], args[5]));
    }

    public static GAny function(GAnyFuncT6 func) {
        return function((GAnyFunction) args -> {
            func.call(args[0], args[1], args[2], args[3], args[4], args[5]);
            return undefined();
        });
    }

    public static GAny function(GAnyFuncRT7 func) {
        return function((GAnyFunction) args -> func.call(args[0], args[1], args[2], args[3], args[4], args[5], args[6]));
    }

    public static GAny function(GAnyFuncT7 func) {
        return function((GAnyFunction) args -> {
            func.call(args[0], args[1], args[2], args[3], args[4], args[5], args[6]);
            return undefined();
        });
    }

    public static GAny function(GAnyFuncRT8 func) {
        return function((GAnyFunction) args -> func.call(args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7]));
    }

    public static GAny function(GAnyFuncT8 func) {
        return function((GAnyFunction) args -> {
            func.call(args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7]);
            return undefined();
        });
    }

    public static GAny createFromPtr(long anyPtr) {
        GAny any = new GAny();
        any.mNativePtr = anyPtr;
        return any;
    }

    public String classTypeName() {
        return nGAnyClassTypeName(mNativePtr);
    }

    public String typeName() {
        return nGAnyTypeName(mNativePtr);
    }

    public int length() {
        return nGAnyLength(mNativePtr);
    }

    public int size() {
        return nGAnySize(mNativePtr);
    }

    public boolean is(String type) {
        return nGAnyIs(mNativePtr, type);
    }

    public boolean isUndefined() {
        return nGAnyIsUndefined(mNativePtr);
    }

    public boolean isNull() {
        return nGAnyIsNull(mNativePtr);
    }

    public boolean isFunction() {
        return nGAnyIsFunction(mNativePtr);
    }

    public boolean isClass() {
        return nGAnyIsClass(mNativePtr);
    }

    public boolean isProperty() {
        return nGAnyIsProperty(mNativePtr);
    }

    public boolean isEnum() {
        return nGAnyIsEnum(mNativePtr);
    }

    public boolean isObject() {
        return nGAnyIsObject(mNativePtr);
    }

    public boolean isArray() {
        return nGAnyIsArray(mNativePtr);
    }

    public boolean isString() {
        return nGAnyIsString(mNativePtr);
    }

    public boolean isInt32() {
        return nGAnyIsInt32(mNativePtr);
    }

    public boolean isInt64() {
        return nGAnyIsInt64(mNativePtr);
    }

    public boolean isInt8() {
        return nGAnyIsInt8(mNativePtr);
    }

    public boolean isInt16() {
        return nGAnyIsInt16(mNativePtr);
    }

    public boolean isFloat() {
        return nGAnyIsFloat(mNativePtr);
    }

    public boolean isDouble() {
        return nGAnyIsDouble(mNativePtr);
    }

    public boolean isNumber() {
        return nGAnyIsNumber(mNativePtr);
    }

    public boolean isBoolean() {
        return nGAnyIsBoolean(mNativePtr);
    }

    public boolean isPointer() {
        return nGAnyIsPointer(mNativePtr);
    }

    public boolean isUserObject() {
        return nGAnyIsUserObject(mNativePtr);
    }

    public int toInt32() {
        return nGAnyToInt32(mNativePtr);
    }

    public long toInt64() {
        return nGAnyToInt64(mNativePtr);
    }

    public byte toInt8() {
        return nGAnyToInt8(mNativePtr);
    }

    public float toFloat() {
        return nGAnyToFloat(mNativePtr);
    }

    public double toDouble() {
        return nGAnyToDouble(mNativePtr);
    }

    public boolean toBoolean() {
        return nGAnyToBool(mNativePtr);
    }

    public String toJsonString(int indent) {
        return nGAnyToJsonString(mNativePtr, indent);
    }

    public String toJsonString() {
        return toJsonString(-1);
    }

    public GAny toObject() {
        return createFromPtr(nGAnyToObject(mNativePtr));
    }

    public static GAny parseJson(String json) {
        return createFromPtr(nGAnyParseJson(json));
    }

    public String dump() {
        return nGAnyDump(mNativePtr);
    }

    public String help() {
        return dump();
    }

    public boolean contains(GAny id) {
        return nGAnyContains(mNativePtr, id.mNativePtr);
    }

    public boolean contains(Object id) {
        return contains(create(id));
    }

    public void erase(GAny id) {
        nGAnyErase(mNativePtr, id.mNativePtr);
    }

    public void erase(Object id) {
        erase(create(id));
    }

    public void pushBask(GAny rh) {
        nGAnyPushBack(mNativePtr, rh.mNativePtr);
    }

    public void pushBask(Object rh) {
        pushBask(create(rh));
    }

    public void clear() {
        nGAnyClear(mNativePtr);
    }

    public GAny iterator() {
        return createFromPtr(nGAnyIterator(mNativePtr));
    }

    public boolean hasNext() {
        return nGAnyHasNext(mNativePtr);
    }

    public GAnyIteratorItem next() {
        return new GAnyIteratorItem(createFromPtr(nGAnyNext(mNativePtr)));
    }

    public GAny getItem(GAny rh) {
        return createFromPtr(nGAnyGetItem(mNativePtr, rh.mNativePtr));
    }

    public GAny getItem(Object rh) {
        return getItem(create(rh));
    }

    public void setItem(GAny i, GAny v) {
        nGAnySetItem(mNativePtr, i.mNativePtr, v.mNativePtr);
    }

    public void setItem(Object i, Object v) {
        setItem(create(i), create(v));
    }

    public void delItem(GAny i) {
        nGAnyDelItem(mNativePtr, i.mNativePtr);
    }

    public void delItem(Object i) {
        delItem(create(i));
    }


    public static GAny OpNeg(GAny v) {
        return createFromPtr(nGAnyOpNeg(v.mNativePtr));
    }

    public static GAny OpAdd(GAny a, GAny b) {
        return createFromPtr(nGAnyOpAdd(a.mNativePtr, b.mNativePtr));
    }

    public static GAny OpSub(GAny a, GAny b) {
        return createFromPtr(nGAnyOpSub(a.mNativePtr, b.mNativePtr));
    }

    public static GAny OpMul(GAny a, GAny b) {
        return createFromPtr(nGAnyOpMul(a.mNativePtr, b.mNativePtr));
    }

    public static GAny OpDiv(GAny a, GAny b) {
        return createFromPtr(nGAnyOpDiv(a.mNativePtr, b.mNativePtr));
    }

    public static GAny OpMod(GAny a, GAny b) {
        return createFromPtr(nGAnyOpMod(a.mNativePtr, b.mNativePtr));
    }

    public static GAny OpBitXor(GAny a, GAny b) {
        return createFromPtr(nGAnyOpBitXor(a.mNativePtr, b.mNativePtr));
    }

    public static GAny OpBitOr(GAny a, GAny b) {
        return createFromPtr(nGAnyOpBitOr(a.mNativePtr, b.mNativePtr));
    }

    public static GAny OpBitAnd(GAny a, GAny b) {
        return createFromPtr(nGAnyOpBitAnd(a.mNativePtr, b.mNativePtr));
    }

    public static GAny OpBitNot(GAny v) {
        return createFromPtr(nGAnyOpBitNot(v.mNativePtr));
    }

    public static boolean OpEqualTo(GAny a, GAny b) {
        return nGAnyOpEqualTo(a.mNativePtr, b.mNativePtr);
    }

    public static boolean OpLessThan(GAny a, GAny b) {
        return nGAnyOpLessThan(a.mNativePtr, b.mNativePtr);
    }


    private static long registerGAnyFunction(GAnyFunction func) {
        long newPtr = System.currentTimeMillis();
        synchronized (mGAnyFunctions) {
            while (mGAnyFunctions.containsKey(newPtr)) {
                newPtr++;
            }

            mGAnyFunctions.put(newPtr, func);
        }

        return nGAnyCreateFunction(newPtr);
    }

    public static long callFunctionProxy(long funcPtr, long[] args) {
        GAnyFunction function;
        synchronized (mGAnyFunctions) {
            if (!mGAnyFunctions.containsKey(funcPtr)) {
                return 0;
            }
            function = mGAnyFunctions.get(funcPtr);
        }
        if (function == null) {
            return 0;
        }

        GAny[] ganyArgs = new GAny[args.length];
        for (int i = 0; i < args.length; i++) {
            ganyArgs[i] = createFromPtr(args[i]);
        }

        GAny ret = function.call(ganyArgs);
        // 这个对象 CFunction 在执行完函数后会释放
        return nGAnyCreate(ret.mNativePtr);
    }

    public static void functionDtorListener(long funcPtr) {
        synchronized (mGAnyFunctions) {
            mGAnyFunctions.remove(funcPtr);
        }
    }

    private static native void nGAnyInit();

    private static native long nGAnyCreate(long v);

    private static native long nGAnyCreateBool(boolean v);

    private static native long nGAnyCreateInt32(int v);

    private static native long nGAnyCreateInt64(long v);

    private static native long nGAnyCreateInt8(byte v);

    private static native long nGAnyCreateFloat(float v);

    private static native long nGAnyCreateDouble(double v);

    private static native long nGAnyCreateString(String v);

    private static native long nGAnyCreateArray();

    private static native long nGAnyCreateObject();

    private static native long nGAnyCreateFunction(long funcPtr);

    private static native long nGAnyCreateUndefined();

    private static native long nGAnyCreateNull();

    private static native void nGAnyDestroy(long any);

    private static native long nGAnyImport(String path);

    private static native void nGAnyExport(long clazz);

    private static native long nGAnyClone(long any);

    private static native String nGAnyClassTypeName(long any);

    private static native String nGAnyTypeName(long any);

    private static native int nGAnyLength(long any);

    private static native int nGAnySize(long any);

    private static native boolean nGAnyIs(long any, String typeStr);

    private static native boolean nGAnyIsUndefined(long any);

    private static native boolean nGAnyIsNull(long any);

    private static native boolean nGAnyIsFunction(long any);

    private static native boolean nGAnyIsClass(long any);

    private static native boolean nGAnyIsProperty(long any);

    private static native boolean nGAnyIsEnum(long any);

    private static native boolean nGAnyIsObject(long any);

    private static native boolean nGAnyIsArray(long any);

    private static native boolean nGAnyIsInt32(long any);

    private static native boolean nGAnyIsInt64(long any);

    private static native boolean nGAnyIsInt8(long any);

    private static native boolean nGAnyIsInt16(long any);

    private static native boolean nGAnyIsFloat(long any);

    private static native boolean nGAnyIsDouble(long any);

    private static native boolean nGAnyIsNumber(long any);

    private static native boolean nGAnyIsString(long any);

    private static native boolean nGAnyIsBoolean(long any);

    private static native boolean nGAnyIsUserObject(long any);

    private static native boolean nGAnyIsPointer(long any);

    private static native int nGAnyToInt32(long any);

    private static native long nGAnyToInt64(long any);

    private static native byte nGAnyToInt8(long any);

    private static native float nGAnyToFloat(long any);

    private static native double nGAnyToDouble(long any);

    private static native boolean nGAnyToBool(long any);

    private static native String nGAnyToString(long ptr);

    private static native String nGAnyToJsonString(long any, int indent);

    private static native long nGAnyToObject(long any);

    private static native long nGAnyParseJson(String json);

    private static native String nGAnyDump(long any);

    private static native boolean nGAnyContains(long any, long id);

    private static native void nGAnyErase(long any, long id);

    private static native void nGAnyPushBack(long any, long rh);

    private static native void nGAnyClear(long any);

    private static native long nGAnyIterator(long any);

    private static native boolean nGAnyHasNext(long any);

    private static native long nGAnyNext(long any);


    private static native long nGAnyCallMethod(long any, String methodName, long[] args);

    private static native long nGAnyCallFunction(long any, long[] args);


    private static native long nGAnyGetItem(long any, long i);

    private static native void nGAnySetItem(long any, long i, long v);

    private static native void nGAnyDelItem(long any, long i);

    private static native long nGAnyOpNeg(long v);    // -v

    private static native long nGAnyOpAdd(long a, long b);  // a + b

    private static native long nGAnyOpSub(long a, long b);  // a - b

    private static native long nGAnyOpMul(long a, long b);  // a * b

    private static native long nGAnyOpDiv(long a, long b);  // a / b

    private static native long nGAnyOpMod(long a, long b);  // a % b

    private static native long nGAnyOpBitXor(long a, long b);  // a ^ b

    private static native long nGAnyOpBitOr(long a, long b);  // a | b

    private static native long nGAnyOpBitAnd(long a, long b);  // a & b

    private static native long nGAnyOpBitNot(long v);  // ~v

    private static native boolean nGAnyOpEqualTo(long a, long b);  // a == b

    private static native boolean nGAnyOpLessThan(long a, long b);  // a < b
}
