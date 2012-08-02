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

#include "org_libharu_PdfPage.h"
#include "hpdf.h"
#include <stdio.h>

// for __android_log_print(ANDROID_LOG_INFO, "YourApp", "formatted message");
#include <android/log.h>
#define TAG "Page"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__))

/* Page enum values from Java */

typedef enum _Size {
    PAGE_SIZE_LETTER = 0,
    PAGE_SIZE_LEGAL,
    PAGE_SIZE_A3,
    PAGE_SIZE_A4,
    PAGE_SIZE_A5,
    PAGE_SIZE_B4,
    PAGE_SIZE_B5,
    PAGE_SIZE_EXECUTIVE,
    PAGE_SIZE_US4X6,
    PAGE_SIZE_US4X8,
    PAGE_SIZE_US5X7,
    PAGE_SIZE_COMM10
} Size;

typedef enum _Direction {
    PAGE_DIRECTION_PORTRAIT = 0,
    PAGE_DIRECTION_LANDSCAPE
} Direction;

typedef enum _LineCap {
    LINE_CAP_BUTT_END = 0,
    LINE_CAP_ROUND_END,
    LINE_CAP_PROJECTING_SQUARE_END
} LineCap;

typedef enum _LineJoin {
    LINE_JOIN_MITER_JOIN = 0,
    LINE_JOIN_ROUND_JOIN,
    LINE_JOIN_BEVEL_JOIN
} LineJoin;

/* Cached Page Fields */

jfieldID mHPDFPagePointer;
jfieldID mParentHPDFDocPointer;

/*
 * Class:     org_libharu_PdfPage
 * Method:    initIDs
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_initIDs(JNIEnv *env, jclass cls) {
    jfieldID fid;

    mHPDFPagePointer = (*env)->GetFieldID(env, cls, "mHPDFPagePointer", "I");
    if (mHPDFPagePointer == NULL) {
        LOGE("Failed to find the field 'mHPDFPagePointer'");
    }

    mParentHPDFDocPointer = (*env)->GetFieldID(env, cls, "mParentHPDFDocPointer", "I");
    if (mParentHPDFDocPointer == NULL) {
        LOGE("Failed to find the field 'mParentHPDFDocPointer'");
    }
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    create
 * Signature: (I)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_create(JNIEnv *env, jobject obj, jint pdf) {
    HPDF_Page page;
    /* Create and add the new page */
    page = HPDF_AddPage((HPDF_Doc) pdf);
    if (page == NULL) {
        LOGE("Failed to create new page");
        return;
    }
    /* Set mHPDFPagePointer */
    (*env)->SetIntField(env, obj, mHPDFPagePointer, (jint) page);
    /* Set mParentHPDFDocPointer*/
    (*env)->SetIntField(env, obj, mParentHPDFDocPointer, pdf);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    insertPage
 * Signature: (II)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_insertPage(JNIEnv *env, jobject obj, jint pdf, jint target) {
    HPDF_Page page;
    /* Create and insert the new page before the target page */
    page = HPDF_InsertPage((HPDF_Doc) pdf, (HPDF_Page) target);
    if (page == NULL) {
        LOGE("Failed to create new page");
        return;
    }
    /* Set mHPDFPagePointer */
    (*env)->SetIntField(env, obj, mHPDFPagePointer, (jint) page);
    /* Set mParentHPDFDocPointer*/
    (*env)->SetIntField(env, obj, mParentHPDFDocPointer, pdf);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    setSize
 * Signature: (II)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_setSize(JNIEnv *env, jobject obj, jint size, jint direction) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    /* Set page size */
    HPDF_Page_SetSize((HPDF_Page) page, getHPDFPageSize(size), getHPDFPageDirection(direction));
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    setWidth
 * Signature: (F)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_setWidth(JNIEnv *env, jobject obj, jfloat width) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    /* Set page size */
    HPDF_Page_SetWidth((HPDF_Page) page, (HPDF_REAL) width);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    setHeight
 * Signature: (F)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_setHeight(JNIEnv *env, jobject obj, jfloat height) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    /* Set page size */
    HPDF_Page_SetHeight((HPDF_Page) page, (HPDF_REAL) height);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    getWidth
 * Signature: ()F
 */
JNIEXPORT jfloat JNICALL
Java_org_libharu_PdfPage_getWidth(JNIEnv *env, jobject obj) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    return HPDF_Page_GetWidth((HPDF_Page) page);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    getHeight
 * Signature: ()F
 */
JNIEXPORT jfloat JNICALL
Java_org_libharu_PdfPage_getHeight(JNIEnv *env, jobject obj) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    return HPDF_Page_GetHeight((HPDF_Page) page);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    getLineWidth
 * Signature: ()F
 */
JNIEXPORT jfloat JNICALL
Java_org_libharu_PdfPage_getLineWidth(JNIEnv *env, jobject obj) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    return HPDF_Page_GetLineWidth((HPDF_Page) page);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    setLineCap
 * Signature: (I)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_setLineCap(JNIEnv *env, jobject obj, jint lineCap) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_SetLineCap((HPDF_Page) page, getHPDFLineCap(lineCap));
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    setLineJoin
 * Signature: (I)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_setLineJoin(JNIEnv *env, jobject obj, jint lineJoin) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_SetLineJoin((HPDF_Page) page, getHPDFLineJoin(lineJoin));
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    setLineWidth
 * Signature: (F)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_setLineWidth(JNIEnv *env, jobject obj, jfloat lineWidth) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_SetLineWidth((HPDF_Page) page, (HPDF_REAL) lineWidth);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    setRGBFill
 * Signature: (FFF)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_setRGBFill(JNIEnv *env, jobject obj, jfloat red, jfloat green, jfloat blue) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_SetRGBFill((HPDF_Page) page, (HPDF_REAL) red, (HPDF_REAL) green, (HPDF_REAL) blue);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    setRGBStroke
 * Signature: (FFF)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_setRGBStroke(JNIEnv *env, jobject obj, jfloat red, jfloat green,
        jfloat blue) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_SetRGBStroke((HPDF_Page) page, (HPDF_REAL) red, (HPDF_REAL) green, (HPDF_REAL) blue);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    moveTo
 * Signature: (FF)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_moveTo(JNIEnv *env, jobject obj, jfloat x, jfloat y) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_MoveTo((HPDF_Page) page, (HPDF_REAL) x, (HPDF_REAL) y);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    lineTo
 * Signature: (FF)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_lineTo(JNIEnv *env, jobject obj, jfloat x, jfloat y) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_LineTo((HPDF_Page) page, (HPDF_REAL) x, (HPDF_REAL) y);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    stroke
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_stroke(JNIEnv *env, jobject obj) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_Stroke((HPDF_Page) page);
}

int getHPDFPageSize(jint size) {
    switch (size) {
        case PAGE_SIZE_LETTER:
            return HPDF_PAGE_SIZE_LETTER;
        case PAGE_SIZE_LEGAL:
            return HPDF_PAGE_SIZE_LEGAL;
        case PAGE_SIZE_A3:
            return HPDF_PAGE_SIZE_A3;
        case PAGE_SIZE_A4:
            return HPDF_PAGE_SIZE_A4;
        case PAGE_SIZE_A5:
            return HPDF_PAGE_SIZE_A5;
        case PAGE_SIZE_B4:
            return HPDF_PAGE_SIZE_B4;
        case PAGE_SIZE_B5:
            return HPDF_PAGE_SIZE_B5;
        case PAGE_SIZE_EXECUTIVE:
            return HPDF_PAGE_SIZE_EXECUTIVE;
        case PAGE_SIZE_US4X6:
            return HPDF_PAGE_SIZE_US4x6;
        case PAGE_SIZE_US4X8:
            return HPDF_PAGE_SIZE_US4x8;
        case PAGE_SIZE_US5X7:
            return HPDF_PAGE_SIZE_US5x7;
        case PAGE_SIZE_COMM10:
            return HPDF_PAGE_SIZE_COMM10;
        default:
            LOGE("Failed to match page size: %d", size);
            break;
    }
    return HPDF_PAGE_SIZE_LETTER;
}

int getHPDFPageDirection(jint direction) {
    switch (direction) {
        case PAGE_DIRECTION_PORTRAIT:
            return HPDF_PAGE_PORTRAIT;
        case PAGE_DIRECTION_LANDSCAPE:
            return HPDF_PAGE_LANDSCAPE;
        default:
            LOGE("Failed to match page direction: %d", direction);
            break;
    }
    return HPDF_PAGE_PORTRAIT;
}

int getHPDFLineCap(jint lineCap) {
    switch (lineCap) {
        case LINE_CAP_BUTT_END:
            return HPDF_BUTT_END;
        case LINE_CAP_ROUND_END:
            return HPDF_ROUND_END;
        case LINE_CAP_PROJECTING_SQUARE_END:
            return HPDF_PROJECTING_SCUARE_END;
        default:
            LOGE("Failed to match line cap: %d", lineCap);
            break;
    }
    return HPDF_BUTT_END;
}

int getHPDFLineJoin(jint lineJoin) {
    switch (lineJoin) {
        case LINE_JOIN_MITER_JOIN:
            return HPDF_MITER_JOIN;
        case LINE_JOIN_ROUND_JOIN:
            return HPDF_ROUND_JOIN;
        case LINE_JOIN_BEVEL_JOIN:
            return HPDF_BEVEL_JOIN;
        default:
            LOGE("Failed to match line join: %d", lineJoin);
            break;
    }
    return HPDF_MITER_JOIN;
}
