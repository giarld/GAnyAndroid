package com.gx.gany;

public class GAnyIteratorItem {
    GAny mNativeObj;

    public GAnyIteratorItem(GAny nativeObj) {
        if (!nativeObj.is("GAnyIteratorItem")) {
            throw new RuntimeException("Native obj not the current type of object.");
        }
        mNativeObj = nativeObj;
    }

    public GAny key() {
        return mNativeObj.getItem("key");
    }

    public GAny value() {
        return mNativeObj.getItem("value");
    }
}
