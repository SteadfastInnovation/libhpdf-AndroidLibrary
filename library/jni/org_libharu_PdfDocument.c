/*
 * Copyright (C) 2012 Andrew Hughes <ashughes@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "org_libharu_PdfDocument.h"
#include "hpdf.h"
#include <stdio.h>

// for __android_log_print(ANDROID_LOG_INFO, "YourApp", "formatted message");
#include <android/log.h>
#define TAG "Document"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__))

/* Cached Document Fields */

jfieldID mHPDFDocPointer;

/*
 * Class:     org_libharu_PdfDocument
 * Method:    initIDs
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfDocument_initIDs(JNIEnv *env, jclass cls) {
    mHPDFDocPointer = (*env)->GetFieldID(env, cls, "mHPDFDocPointer", "I");
    if (mHPDFDocPointer == NULL) {
        LOGE("Failed to find the field 'mHPDFDocPointer'");
    }
}

/*
 * Class:     org_libharu_PdfDocument
 * Method:    create
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL
Java_org_libharu_PdfDocument_create__(JNIEnv *env, jobject obj) {
    HPDF_Doc pdf;
    /* Create the new document */
    pdf = HPDF_New(NULL, NULL);
    if (!pdf) {
        LOGE("Failed to create pdf object");
        return JNI_FALSE;
    }

    /* Set mHPDFDocPointer */
    (*env)->SetIntField(env, obj, mHPDFDocPointer, (jint) pdf);

    return JNI_TRUE;
}

/*
 * Class:     org_libharu_PdfDocument
 * Method:    create
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL
Java_org_libharu_PdfDocument_create__I(JNIEnv *env, jclass cls, jint pdf) {
    HPDF_STATUS status;
    status = HPDF_NewDoc((HPDF_Doc) pdf);
    if (status != HPDF_OK) {
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

/*
 * Class:     org_libharu_PdfDocument
 * Method:    free
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfDocument_free(JNIEnv *env, jobject obj) {
    jint pdf;
    /* Get mHPDFDocPointer */
    pdf = (*env)->GetIntField(env, obj, mHPDFDocPointer);
    /* Free the document */
    HPDF_Free((HPDF_Doc) pdf);
}

/*
 * Class:     org_libharu_PdfDocument
 * Method:    freeDoc
 * Signature: (I)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfDocument_freeDoc(JNIEnv *env, jclass cls, jint pdf) {
    /* Free the provided document */
    HPDF_FreeDoc((HPDF_Doc) pdf);
}

/*
 * Class:     org_libharu_PdfDocument
 * Method:    freeDocAll
 * Signature: (I)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfDocument_freeDocAll(JNIEnv *env, jclass cls, jint pdf) {
    /* Free the provided document */
    HPDF_FreeDocAll((HPDF_Doc) pdf);
}

/*
 * Class:     org_libharu_PdfDocument
 * Method:    saveToFile
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL
Java_org_libharu_PdfDocument_saveToFile(JNIEnv *env, jobject obj, jstring filename) {
    jboolean success = JNI_FALSE;

    /* Get mHPDFDocPointer */
    jint pdf = (*env)->GetIntField(env, obj, mHPDFDocPointer);

    /* Get filename */
    const char *name = (*env)->GetStringUTFChars(env, filename, NULL);

    /* Save to file */
    HPDF_STATUS status = HPDF_SaveToFile((HPDF_Doc) pdf, name);
    if (status == HPDF_OK) {
        success = JNI_TRUE;
    } else {
        if (status == HPDF_INVALID_DOCUMENT) {
            LOGE("Error saving to file %s: HPDF_INVALID_DOCUMENT", name);
        } else if (status == HPDF_FAILD_TO_ALLOC_MEM) {
            LOGE("Error saving to file %s: HPDF_FAILD_TO_ALLOC_MEM", name);
        } else if (status == HPDF_FILE_IO_ERROR) {
            LOGE("Error saving to file %s: HPDF_FILE_IO_ERROR", name);
        }
    }

    /* Release the native char array */
    (*env)->ReleaseStringUTFChars(env, filename, name);

    return success;
}

/*
 * Class:     org_libharu_PdfDocument
 * Method:    setCreator
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfDocument_setCreator(JNIEnv *env, jobject obj, jstring creator) {

    /* Get mHPDFDocPointer */
    jint pdf = (*env)->GetIntField(env, obj, mHPDFDocPointer);

    /* Get filename */
    const char *name = (*env)->GetStringUTFChars(env, creator, NULL);

    /* Set the creator info attribute */
    HPDF_SetInfoAttr((HPDF_Doc) pdf, HPDF_INFO_CREATOR, name);

    /* Release the native char array */
    (*env)->ReleaseStringUTFChars(env, creator, name);
}

/*
 * Class:     org_libharu_PdfDocument
 * Method:    hasDoc
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL
Java_org_libharu_PdfDocument_hasDoc(JNIEnv *env, jclass cls, jint pdf) {
    if (HPDF_HasDoc((HPDF_Doc) pdf)) {
        return JNI_TRUE;
    }
    return JNI_FALSE;
}

/*
 * Class:     org_libharu_PdfDocument
 * Method:    setCompressionMode
 * Signature: (I)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfDocument_setCompressionMode(JNIEnv *env, jobject obj, jint mode) {
    /* Get mHPDFDocPointer */
    jint pdf = (*env)->GetIntField(env, obj, mHPDFDocPointer);

    HPDF_SetCompressionMode((HPDF_Doc) pdf, (HPDF_UINT) mode);
}
