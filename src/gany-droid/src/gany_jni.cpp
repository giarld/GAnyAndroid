/*
 * Copyright (c) 2023 Gxin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <jni.h>
#include <string>
#include <memory>

#include "gx/gany_c_api.h"
#include <gx/gany_core.h>

#include <android/log.h>


#define LogE(fmt, ...) __android_log_print(ANDROID_LOG_ERROR, "GxAny", fmt, ##__VA_ARGS__);

extern void importModules();

struct JVMLife
{
    JavaVM *javaVM;

    explicit JVMLife(JavaVM *javaVM)
            : javaVM(javaVM)
    {}

    ~JVMLife()
    {
        if (javaVM) {
            javaVM->DetachCurrentThread();
        }
    }
};


static JavaVM *sJavaVM = nullptr;

static jobject sJGAnyGlobalObj;     // 此对象未构造，用于GetObjectClass

thread_local std::unique_ptr<JVMLife> sLocalVMLife;


static bool getEnv(JNIEnv **env)
{
    bool bRet = false;
    switch (sJavaVM->GetEnv((void **) env, JNI_VERSION_1_6)) {
        case JNI_OK:
            bRet = true;
            break;
        case JNI_EDETACHED:
            if (sJavaVM->AttachCurrentThread(env, nullptr) < 0) {
                break;
            }
            if (!sLocalVMLife) {
                sLocalVMLife = std::make_unique<JVMLife>(sJavaVM);
            }
            bRet = true;
            break;
        default:
            break;
    }
    return bRet;
}

static GAnyPtr cAnyFunctionProxy(CAnyFuncPtr funcPtr, GAnyPtr *args, int32_t argc)
{
    if (sJavaVM == nullptr) {
        return ganyCreateUndefined();
    }
    JNIEnv *env;
    if (!getEnv(&env)) {
        LogE("cAnyFunctionProxy get env error!");
        return ganyCreateUndefined();
    }

    jlongArray jArgs = env->NewLongArray(argc);
    env->SetLongArrayRegion(jArgs, 0, argc, (const jlong *) args);

    jclass clazz = env->GetObjectClass(sJGAnyGlobalObj);
    if (!clazz) {
        LogE("cAnyFunctionProxy get class failure!!");
    }
    jmethodID method = env->GetStaticMethodID(clazz, "callFunctionProxy", "(J[J)J");
    jlong ret = env->CallStaticLongMethod(clazz, method, (jlong) funcPtr, jArgs);

    return ret;
}

static void cAnyFunctionDtorListener(CAnyFuncPtr funcPtr)
{
    if (sJavaVM == nullptr) {
        return;
    }
    JNIEnv *env;
    if (!getEnv(&env)) {
        LogE("cAnyFunctionDtorListener get env error!");
        return;
    }

    jclass clazz = env->GetObjectClass(sJGAnyGlobalObj);
    if (!clazz) {
        LogE("cAnyFunctionProxy get class failure!!");
    }
    jmethodID method = env->GetStaticMethodID(clazz, "functionDtorListener", "(J)V");
    env->CallStaticVoidMethod(clazz, method, (jlong) funcPtr);
}

extern "C" JNIEXPORT void JNICALL
Java_com_gx_gany_GAny_nGAnyInit(JNIEnv *env, jclass clazz)
{
    env->GetJavaVM(&sJavaVM);
    jobject obj = env->AllocObject(clazz);
    sJGAnyGlobalObj = env->NewGlobalRef(obj);

    initGAnyCore();

    ganySetFunctionProxy(cAnyFunctionProxy);
    ganySetFunctionDtorListener(cAnyFunctionDtorListener);

    importModules();
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyCreate(JNIEnv *, jclass, jlong v)
{
    return ganyCreate(v);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyCreateBool(JNIEnv *, jclass, jboolean v)
{
    return ganyCreateBool(v);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyCreateInt32(JNIEnv *, jclass, jint v)
{
    return ganyCreateInt32(v);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyCreateInt64(JNIEnv *, jclass, jlong v)
{
    return ganyCreateInt64(v);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyCreateInt8(JNIEnv *, jclass, jbyte v)
{
    return ganyCreateInt8(v);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyCreateFloat(JNIEnv *, jclass, jfloat v)
{
    return ganyCreateFloat(v);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyCreateDouble(JNIEnv *, jclass, jdouble v)
{
    return ganyCreateDouble(v);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyCreateString(JNIEnv *env, jclass, jstring v)
{
    const char *cStr = env->GetStringUTFChars(v, nullptr);
    GAnyPtr ret = ganyCreateString(cStr);
    env->ReleaseStringUTFChars(v, cStr);
    return ret;
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyCreateArray(JNIEnv *, jclass)
{
    return ganyCreateArray();
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyCreateObject(JNIEnv *, jclass)
{
    return ganyCreateObject();
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyCreateFunction(JNIEnv *, jclass, jlong funcPtr)
{
    return ganyCreateFunction(funcPtr);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyCreateUndefined(JNIEnv *, jclass)
{
    return ganyCreateUndefined();
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyCreateNull(JNIEnv *, jclass)
{
    return ganyCreateNull();
}

extern "C" JNIEXPORT void JNICALL
Java_com_gx_gany_GAny_nGAnyDestroy(JNIEnv *, jclass, jlong any)
{
    ganyDestroy(any);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyEnvironment(JNIEnv *, jclass)
{
    return ganyEnvironment();
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyClone(JNIEnv *, jclass, jlong any)
{
    return ganyClone(any);
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_gx_gany_GAny_nGAnyClassTypeName(JNIEnv *env, jclass, jlong any)
{
    return env->NewStringUTF(ganyClassTypeName(any));
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_gx_gany_GAny_nGAnyTypeName(JNIEnv *env, jclass, jlong any)
{
    return env->NewStringUTF(ganyTypeName(any));
}

extern "C" JNIEXPORT jint JNICALL
Java_com_gx_gany_GAny_nGAnyLength(JNIEnv *, jclass, jlong any)
{
    return ganyLength(any);
}

extern "C" JNIEXPORT jint JNICALL
Java_com_gx_gany_GAny_nGAnySize(JNIEnv *, jclass, jlong any)
{
    return ganySize(any);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_gx_gany_GAny_nGAnyIs(JNIEnv *env, jclass, jlong any, jstring typeStr)
{
    const char *cTypeStr = env->GetStringUTFChars(typeStr, nullptr);
    bool ret = ganyIs(any, cTypeStr);
    env->ReleaseStringUTFChars(typeStr, cTypeStr);
    return ret;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_gx_gany_GAny_nGAnyIsUndefined(JNIEnv *, jclass, jlong any)
{
    return ganyIsUndefined(any);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_gx_gany_GAny_nGAnyIsNull(JNIEnv *, jclass, jlong any)
{
    return ganyIsNull(any);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_gx_gany_GAny_nGAnyIsFunction(JNIEnv *, jclass, jlong any)
{
    return ganyIsFunction(any);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_gx_gany_GAny_nGAnyIsClass(JNIEnv *, jclass, jlong any)
{
    return ganyIsClass(any);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_gx_gany_GAny_nGAnyIsProperty(JNIEnv *, jclass, jlong any)
{
    return ganyIsProperty(any);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_gx_gany_GAny_nGAnyIsEnum(JNIEnv *, jclass, jlong any)
{
    return ganyIsEnum(any);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_gx_gany_GAny_nGAnyIsObject(JNIEnv *, jclass, jlong any)
{
    return ganyIsObject(any);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_gx_gany_GAny_nGAnyIsArray(JNIEnv *, jclass, jlong any)
{
    return ganyIsArray(any);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_gx_gany_GAny_nGAnyIsInt32(JNIEnv *, jclass, jlong any)
{
    return ganyIsInt32(any);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_gx_gany_GAny_nGAnyIsInt64(JNIEnv *, jclass, jlong any)
{
    return ganyIsInt64(any);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_gx_gany_GAny_nGAnyIsInt8(JNIEnv *, jclass, jlong any)
{
    return ganyIsInt8(any);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_gx_gany_GAny_nGAnyIsInt16(JNIEnv *, jclass, jlong any)
{
    return ganyIsInt16(any);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_gx_gany_GAny_nGAnyIsFloat(JNIEnv *, jclass, jlong any)
{
    return ganyIsFloat(any);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_gx_gany_GAny_nGAnyIsDouble(JNIEnv *, jclass, jlong any)
{
    return ganyIsDouble(any);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_gx_gany_GAny_nGAnyIsNumber(JNIEnv *, jclass, jlong any)
{
    return ganyIsNumber(any);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_gx_gany_GAny_nGAnyIsString(JNIEnv *, jclass, jlong any)
{
    return ganyIsString(any);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_gx_gany_GAny_nGAnyIsBoolean(JNIEnv *, jclass, jlong any)
{
    return ganyIsBoolean(any);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_gx_gany_GAny_nGAnyIsUserObject(JNIEnv *, jclass, jlong any)
{
    return ganyIsUserObject(any);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_gx_gany_GAny_nGAnyIsPointer(JNIEnv *, jclass, jlong any)
{
    return ganyIsPointer(any);
}

extern "C" JNIEXPORT jint JNICALL
Java_com_gx_gany_GAny_nGAnyToInt32(JNIEnv *, jclass, jlong any)
{
    return ganyToInt32(any);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyToInt64(JNIEnv *, jclass, jlong any)
{
    return ganyToInt64(any);
}

extern "C" JNIEXPORT jbyte JNICALL
Java_com_gx_gany_GAny_nGAnyToInt8(JNIEnv *, jclass, jlong any)
{
    return ganyToInt8(any);
}

extern "C" JNIEXPORT jfloat JNICALL
Java_com_gx_gany_GAny_nGAnyToFloat(JNIEnv *, jclass, jlong any)
{
    return ganyToFloat(any);
}

extern "C" JNIEXPORT jdouble JNICALL
Java_com_gx_gany_GAny_nGAnyToDouble(JNIEnv *, jclass, jlong any)
{
    return ganyToDouble(any);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_gx_gany_GAny_nGAnyToBool(JNIEnv *, jclass, jlong any)
{
    return ganyToBool(any);
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_gx_gany_GAny_nGAnyToString(JNIEnv *env, jclass, jlong ptr)
{
    GAnyString cStr = ganyToString(ptr);
    jstring str = env->NewStringUTF(cStr);
    ganyFreeString(cStr);
    return str;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_gx_gany_GAny_nGAnyToJsonString(JNIEnv *env, jclass, jlong ptr, int indent)
{
    GAnyString cStr = ganyToJsonString(ptr, indent);
    jstring str = env->NewStringUTF(cStr);
    ganyFreeString(cStr);
    return str;
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyToObject(JNIEnv *, jclass, jlong ptr)
{
    return ganyToObject(ptr);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyParseJson(JNIEnv *env, jclass, jstring json)
{
    const char *cStr = env->GetStringUTFChars(json, nullptr);
    GAnyPtr ret = ganyParseJson(cStr);
    env->ReleaseStringUTFChars(json, cStr);
    return ret;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_gx_gany_GAny_nGAnyDump(JNIEnv *env, jclass, jlong ptr)
{
    GAnyString cStr = ganyDump(ptr);
    jstring str = env->NewStringUTF(cStr);
    ganyFreeString(cStr);
    return str;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_gx_gany_GAny_nGAnyContains(JNIEnv *, jclass, jlong any, jlong id)
{
    return ganyContains(any, id);
}

extern "C" JNIEXPORT void JNICALL
Java_com_gx_gany_GAny_nGAnyErase(JNIEnv *, jclass, jlong any, jlong id)
{
    ganyErase(any, id);
}

extern "C" JNIEXPORT void JNICALL
Java_com_gx_gany_GAny_nGAnyPushBack(JNIEnv *, jclass, jlong any, jlong rh)
{
    ganyPushBack(any, rh);
}

extern "C" JNIEXPORT void JNICALL
Java_com_gx_gany_GAny_nGAnyClear(JNIEnv *, jclass, jlong any)
{
    ganyClear(any);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyIterator(JNIEnv *, jclass, jlong any)
{
    return ganyIterator(any);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_gx_gany_GAny_nGAnyHasNext(JNIEnv *, jclass, jlong any)
{
    return ganyHasNext(any);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyNext(JNIEnv *, jclass, jlong any)
{
    return ganyNext(any);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyCallMethod(JNIEnv *env, jclass, jlong any,
                                      jstring methodName, jlongArray args)
{
    jlong *argsArray = env->GetLongArrayElements(args, nullptr);
    if (argsArray == nullptr) {
        return ganyCreateUndefined();
    }
    int argc = env->GetArrayLength(args);
    const char *cMethodNameStr = env->GetStringUTFChars(methodName, nullptr);

    auto ret = (jlong) ganyCallMethod(any, cMethodNameStr, (GAnyPtr *) argsArray, argc);
    env->ReleaseLongArrayElements(args, argsArray, 0);
    env->ReleaseStringUTFChars(methodName, cMethodNameStr);
    return ret;
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyCallFunction(JNIEnv *env, jclass, jlong any, jlongArray args)
{
    jlong *argsArray = env->GetLongArrayElements(args, nullptr);
    if (argsArray == nullptr) {
        return ganyCreateUndefined();
    }
    int argc = env->GetArrayLength(args);

    auto ret = (jlong) ganyCallFunction(any, (GAnyPtr *) argsArray, argc);
    env->ReleaseLongArrayElements(args, argsArray, 0);
    return ret;
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyGetItem(JNIEnv *, jclass, jlong any, jlong i)
{
    return ganyGetItem(any, i);
}

extern "C" JNIEXPORT void JNICALL
Java_com_gx_gany_GAny_nGAnySetItem(JNIEnv *, jclass, jlong any, jlong i, jlong v)
{
    ganySetItem(any, i, v);
}

extern "C" JNIEXPORT void JNICALL
Java_com_gx_gany_GAny_nGAnyDelItem(JNIEnv *, jclass, jlong any, jlong i)
{
    ganyDelItem(any, i);
}


extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyOpNeg(JNIEnv *, jclass, jlong v)
{
    return ganyOperatorNeg(v);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyOpAdd(JNIEnv *, jclass, jlong a, jlong b)
{
    return ganyOperatorAdd(a, b);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyOpSub(JNIEnv *, jclass, jlong a, jlong b)
{
    return ganyOperatorSub(a, b);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyOpMul(JNIEnv *, jclass, jlong a, jlong b)
{
    return ganyOperatorMul(a, b);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyOpDiv(JNIEnv *, jclass, jlong a, jlong b)
{
    return ganyOperatorDiv(a, b);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyOpMod(JNIEnv *, jclass, jlong a, jlong b)
{
    return ganyOperatorMod(a, b);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyOpBitXor(JNIEnv *, jclass, jlong a, jlong b)
{
    return ganyOperatorBitXor(a, b);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyOpBitOr(JNIEnv *, jclass, jlong a, jlong b)
{
    return ganyOperatorBitOr(a, b);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyOpBitAnd(JNIEnv *, jclass, jlong a, jlong b)
{
    return ganyOperatorBitAnd(a, b);
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_gx_gany_GAny_nGAnyOpBitNot(JNIEnv *, jclass, jlong v)
{
    return ganyOperatorBitNot(v);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_gx_gany_GAny_nGAnyOpEqualTo(JNIEnv *, jclass, jlong a, jlong b)
{
    return ganyOperatorEqualTo(a, b);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_gx_gany_GAny_nGAnyOpLessThan(JNIEnv *, jclass, jlong a, jlong b)
{
    return ganyOperatorLessThan(a, b);
}
