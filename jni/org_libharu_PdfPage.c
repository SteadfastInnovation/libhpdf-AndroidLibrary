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

typedef enum _TextRenderingMode {
    TEXT_RENDERING_MODE_FILL = 0,
    TEXT_RENDERING_MODE_STROKE,
    TEXT_RENDERING_MODE_FILL_THEN_STROKE,
    TEXT_RENDERING_MODE_INVISIBLE,
    TEXT_RENDERING_MODE_FILL_CLIPPING,
    TEXT_RENDERING_MODE_STROKE_CLIPPING,
    TEXT_RENDERING_MODE_FILL_STROKE_CLIPPING,
    TEXT_RENDERING_MODE_CLIPPING
} TextRenderingMode;

typedef enum _TextAlign {
    TEXT_ALIGN_LEFT = 0,
    TEXT_ALIGN_RIGHT,
    TEXT_ALIGN_CENTER,
    TEXT_ALIGN_JUSTIFY
} TextAlign;

/* Helper functions to map Java enum values to native HPDF enum values */

HPDF_PageSizes getHPDFPageSize(jint size) {
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

HPDF_PageDirection getHPDFPageDirection(jint direction) {
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

HPDF_LineCap getHPDFLineCap(jint lineCap) {
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

HPDF_LineJoin getHPDFLineJoin(jint lineJoin) {
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

HPDF_TextRenderingMode getHPDFTextRenderingMode(jint mode) {
    switch (mode) {
        case TEXT_RENDERING_MODE_FILL:
            return HPDF_FILL;
        case TEXT_RENDERING_MODE_STROKE:
            return HPDF_STROKE;
        case TEXT_RENDERING_MODE_FILL_THEN_STROKE:
            return HPDF_FILL_THEN_STROKE;
        case TEXT_RENDERING_MODE_INVISIBLE:
            return HPDF_INVISIBLE;
        case TEXT_RENDERING_MODE_FILL_CLIPPING:
            return HPDF_FILL_CLIPPING;
        case TEXT_RENDERING_MODE_STROKE_CLIPPING:
            return HPDF_STROKE_CLIPPING;
        case TEXT_RENDERING_MODE_FILL_STROKE_CLIPPING:
            return HPDF_FILL_STROKE_CLIPPING;
        case TEXT_RENDERING_MODE_CLIPPING:
            return HPDF_CLIPPING;
        default:
            LOGE("Failed to match text rendering mode: %d", mode);
            break;
    }
    return HPDF_FILL;
}

HPDF_TextAlignment getHPDFTextAlign(jint align) {
    switch (align) {
        case TEXT_ALIGN_LEFT:
            return HPDF_TALIGN_LEFT;
        case TEXT_ALIGN_RIGHT:
            return HPDF_TALIGN_RIGHT;
        case TEXT_ALIGN_CENTER:
            return HPDF_TALIGN_CENTER;
        case TEXT_ALIGN_JUSTIFY:
            return HPDF_TALIGN_JUSTIFY;
        default:
            LOGE("Failed to match text align: %d", align);
            break;
    }
    return HPDF_TALIGN_LEFT;
}

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
 * Method:    arc
 * Signature: (FFFFF)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_arc(JNIEnv *env, jobject obj, jfloat x, jfloat y, jfloat radius,
        jfloat startAngle, jfloat endAngle) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_Arc((HPDF_Page) page, (HPDF_REAL) x, (HPDF_REAL) y, (HPDF_REAL) radius,
            (HPDF_REAL) startAngle, (HPDF_REAL) endAngle);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    beginText
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_beginText(JNIEnv *env, jobject obj) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_BeginText((HPDF_Page) page);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    circle
 * Signature: (FFF)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_circle(JNIEnv *env, jobject obj, jfloat x, jfloat y, jfloat radius) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_Circle((HPDF_Page) page, (HPDF_REAL) x, (HPDF_REAL) y, (HPDF_REAL) radius);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    clip
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_clip(JNIEnv *env, jobject obj) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_Clip((HPDF_Page) page);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    closePath
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_closePath(JNIEnv *env, jobject obj) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_ClosePath((HPDF_Page) page);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    closePathStroke
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_closePathStroke(JNIEnv *env, jobject obj) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_ClosePathStroke((HPDF_Page) page);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    closePathEofillStroke
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_closePathEofillStroke(JNIEnv *env, jobject obj) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_ClosePathEofillStroke((HPDF_Page) page);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    closePathFillStroke
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_closePathFillStroke(JNIEnv *env, jobject obj) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_ClosePathFillStroke((HPDF_Page) page);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    concat
 * Signature: (FFFFFF)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_concat(JNIEnv *env, jobject obj, jfloat a, jfloat b, jfloat c, jfloat d,
        jfloat x, jfloat y) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_Concat((HPDF_Page) page, (HPDF_REAL) a, (HPDF_REAL) b, (HPDF_REAL) c, (HPDF_REAL) d,
            (HPDF_REAL) x, (HPDF_REAL) y);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    curveTo
 * Signature: (FFFFFF)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_curveTo(JNIEnv *env, jobject obj, jfloat x1, jfloat y1, jfloat x2,
        jfloat y2, jfloat x3, jfloat y3) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_CurveTo((HPDF_Page) page, (HPDF_REAL) x1, (HPDF_REAL) y1, (HPDF_REAL) x2,
            (HPDF_REAL) y2, (HPDF_REAL) x3, (HPDF_REAL) y3);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    curveTo2
 * Signature: (FFFF)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_curveTo2(JNIEnv *env, jobject obj, jfloat x2, jfloat y2, jfloat x3,
        jfloat y3) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_CurveTo2((HPDF_Page) page, (HPDF_REAL) x2, (HPDF_REAL) y2, (HPDF_REAL) x3,
            (HPDF_REAL) y3);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    curveTo3
 * Signature: (FFFF)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_curveTo3(JNIEnv *env, jobject obj, jfloat x1, jfloat y1, jfloat x3,
        jfloat y3) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_CurveTo3((HPDF_Page) page, (HPDF_REAL) x1, (HPDF_REAL) y1, (HPDF_REAL) x3,
            (HPDF_REAL) y3);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    drawJpegImage
 * Signature: ([BFFFF)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_drawJpegImage(JNIEnv *env, jobject obj, jbyteArray imageData, jfloat x,
        jfloat y, jfloat width, jfloat height) {
    jint page, pdf;
    jbyte* buffer; /* The image data as a jbyte[] (really signed char[]) */
    jsize len; /* The number of elements in the image byte array */

    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    /* Get mParentHPDFDocPointer */
    pdf = (*env)->GetIntField(env, obj, mParentHPDFDocPointer);

    /* Get the image data as a native byte array */
    buffer = (*env)->GetByteArrayElements(env, imageData, NULL);
    /* Get the number of elements in the image byte array */
    len = (*env)->GetArrayLength(env, imageData);

    /* Load an HPDF_Image from the image byte array */
    HPDF_Image image = HPDF_LoadJpegImageFromMem((HPDF_Doc) pdf, (HPDF_BYTE*) buffer,
            (HPDF_UINT) (len * sizeof(jbyte)));

    /* Actually draw the image */
    HPDF_Page_DrawImage((HPDF_Page) page, image, (HPDF_REAL) x, (HPDF_REAL) y, (HPDF_REAL) width,
            (HPDF_REAL) height);

    /* Release (free) the native byte array */
    (*env)->ReleaseByteArrayElements(env, imageData, buffer, JNI_ABORT);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    drawJpegImageFromFile
 * Signature: (Ljava/lang/String;FFFF)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_drawJpegImageFromFile(JNIEnv *env, jobject obj, jstring path, jfloat x,
        jfloat y, jfloat width, jfloat height) {
    jint page, pdf;
    const jchar* filename; /* The path of the file to load the image from */

    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    /* Get mParentHPDFDocPointer */
    pdf = (*env)->GetIntField(env, obj, mParentHPDFDocPointer);

    /* Get the filename as a native char array */
    filename = (*env)->GetStringChars(env, path, NULL);

    /* Load an HPDF_Image from the file */
    HPDF_Image image = HPDF_LoadJpegImageFromFile((HPDF_Doc) pdf, (char*) filename);

    /* Actually draw the image */
    HPDF_Page_DrawImage((HPDF_Page) page, image, (HPDF_REAL) x, (HPDF_REAL) y, (HPDF_REAL) width,
            (HPDF_REAL) height);

    /* Release (free) the native char array */
    (*env)->ReleaseStringChars(env, path, filename);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    drawPngImage
 * Signature: ([BFFFF)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_drawPngImage(JNIEnv *env, jobject obj, jbyteArray imageData, jfloat x,
        jfloat y, jfloat width, jfloat height) {
    jint page, pdf;
    jbyte* buffer; /* The image data as a jbyte[] (really signed char[]) */
    jsize len; /* The number of elements in the image byte array */

    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    /* Get mParentHPDFDocPointer */
    pdf = (*env)->GetIntField(env, obj, mParentHPDFDocPointer);

    /* Get the image data as a native byte array */
    buffer = (*env)->GetByteArrayElements(env, imageData, NULL);
    /* Get the number of elements in the image byte array */
    len = (*env)->GetArrayLength(env, imageData);

    /* Load an HPDF_Image from the image byte array */
    HPDF_Image image = HPDF_LoadPngImageFromMem((HPDF_Doc) pdf, (HPDF_BYTE*) buffer,
            (HPDF_UINT) (len * sizeof(jbyte)));

    /* Actually draw the image */
    HPDF_Page_DrawImage((HPDF_Page) page, image, (HPDF_REAL) x, (HPDF_REAL) y, (HPDF_REAL) width,
            (HPDF_REAL) height);

    /* Release (free) the native byte array */
    (*env)->ReleaseByteArrayElements(env, imageData, buffer, JNI_ABORT);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    drawPngImageFromFile
 * Signature: (Ljava/lang/String;FFFF)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_drawPngImageFromFile(JNIEnv *env, jobject obj, jstring path, jfloat x,
        jfloat y, jfloat width, jfloat height) {
    jint page, pdf;
    const jchar* filename; /* The path of the file to load the image from */

    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    /* Get mParentHPDFDocPointer */
    pdf = (*env)->GetIntField(env, obj, mParentHPDFDocPointer);

    /* Get the filename as a native char array */
    filename = (*env)->GetStringChars(env, path, NULL);

    /* Load an HPDF_Image from the file */
    HPDF_Image image = HPDF_LoadPngImageFromFile((HPDF_Doc) pdf, (char*) filename);

    /* Actually draw the image */
    HPDF_Page_DrawImage((HPDF_Page) page, image, (HPDF_REAL) x, (HPDF_REAL) y, (HPDF_REAL) width,
            (HPDF_REAL) height);

    /* Release (free) the native char array */
    (*env)->ReleaseStringChars(env, path, filename);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    ellipse
 * Signature: (FFFF)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_ellipse(JNIEnv *env, jobject obj, jfloat x, jfloat y, jfloat radiusX,
        jfloat radiusY) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_Ellipse((HPDF_Page) page, (HPDF_REAL) x, (HPDF_REAL) y, (HPDF_REAL) radiusX,
            (HPDF_REAL) radiusY);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    endPath
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_endPath(JNIEnv *env, jobject obj) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_EndPath((HPDF_Page) page);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    endText
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_endText(JNIEnv *env, jobject obj) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_EndText((HPDF_Page) page);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    eoclip
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_eoclip(JNIEnv *env, jobject obj) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_Eoclip((HPDF_Page) page);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    eofill
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_eofill(JNIEnv *env, jobject obj) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_Eofill((HPDF_Page) page);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    eofillStroke
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_eofillStroke(JNIEnv *env, jobject obj) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_EofillStroke((HPDF_Page) page);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    fill
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_fill(JNIEnv *env, jobject obj) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_Fill((HPDF_Page) page);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    fillStroke
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_fillStroke(JNIEnv *env, jobject obj) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_FillStroke((HPDF_Page) page);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    gRestore
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_gRestore(JNIEnv *env, jobject obj) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_GRestore((HPDF_Page) page);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    gSave
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_gSave(JNIEnv *env, jobject obj) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_GSave((HPDF_Page) page);
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
 * Method:    moveTextPos
 * Signature: (FF)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_moveTextPos(JNIEnv *env, jobject obj, jfloat x, jfloat y) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_MoveTextPos((HPDF_Page) page, (HPDF_REAL) x, (HPDF_REAL) y);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    moveTextPos2
 * Signature: (FF)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_moveTextPos2(JNIEnv *env, jobject obj, jfloat x, jfloat y) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_MoveTextPos2((HPDF_Page) page, (HPDF_REAL) x, (HPDF_REAL) y);
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
 * Method:    moveToNextLine
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_moveToNextLine(JNIEnv *env, jobject obj) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_MoveToNextLine((HPDF_Page) page);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    rectangle
 * Signature: (FFFF)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_rectangle(JNIEnv *env, jobject obj, jfloat x, jfloat y, jfloat width,
        jfloat height) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_Rectangle((HPDF_Page) page, (HPDF_REAL) x, (HPDF_REAL) y, (HPDF_REAL) width,
            (HPDF_REAL) height);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    setCharSpace
 * Signature: (F)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_setCharSpace(JNIEnv *env, jobject obj, jfloat value) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_SetCharSpace((HPDF_Page) page, (HPDF_REAL) value);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    setCMYKFill
 * Signature: (FFFF)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_setCMYKFill(JNIEnv *env, jobject obj, jfloat c, jfloat m, jfloat y,
        jfloat k) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_SetCMYKFill((HPDF_Page) page, (HPDF_REAL) c, (HPDF_REAL) m, (HPDF_REAL) y,
            (HPDF_REAL) k);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    setCMYKStroke
 * Signature: (FFFF)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_setCMYKStroke(JNIEnv *env, jobject obj, jfloat c, jfloat m, jfloat y,
        jfloat k) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_SetCMYKStroke((HPDF_Page) page, (HPDF_REAL) c, (HPDF_REAL) m, (HPDF_REAL) y,
            (HPDF_REAL) k);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    setGrayFill
 * Signature: (F)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_setGrayFill(JNIEnv *env, jobject obj, jfloat gray) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_SetGrayFill((HPDF_Page) page, (HPDF_REAL) gray);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    setGrayStroke
 * Signature: (F)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_setGrayStroke(JNIEnv *env, jobject obj, jfloat gray) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_SetGrayStroke((HPDF_Page) page, (HPDF_REAL) gray);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    setHorizontalScaling
 * Signature: (F)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_setHorizontalScaling(JNIEnv *env, jobject obj, jfloat value) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_SetHorizontalScalling((HPDF_Page) page, (HPDF_REAL) value);
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
 * Method:    setMiterLimit
 * Signature: (F)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_setMiterLimit(JNIEnv *env, jobject obj, jfloat miterLimit) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_SetMiterLimit((HPDF_Page) page, (HPDF_REAL) miterLimit);
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
 * Method:    setTextLeading
 * Signature: (F)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_setTextLeading(JNIEnv *env, jobject obj, jfloat value) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_SetTextLeading((HPDF_Page) page, (HPDF_REAL) value);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    setTextRenderingMode
 * Signature: (I)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_setTextRenderingMode(JNIEnv *env, jobject obj, jint mode) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_SetTextRenderingMode((HPDF_Page) page, getHPDFTextRenderingMode(mode));
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    setTextRise
 * Signature: (F)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_setTextRise(JNIEnv *env, jobject obj, jfloat value) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_SetTextRise((HPDF_Page) page, (HPDF_REAL) value);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    setTextWordSpace
 * Signature: (F)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_setTextWordSpace(JNIEnv *env, jobject obj, jfloat value) {
    jint page;
    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);
    HPDF_Page_SetWordSpace((HPDF_Page) page, (HPDF_REAL) value);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    showText
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_showText(JNIEnv *env, jobject obj, jstring textString) {
    jint page;
    const jchar* text;

    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);

    /* Get the text as a native char array */
    text = (*env)->GetStringChars(env, textString, NULL);

    HPDF_Page_ShowText((HPDF_Page) page, (char*) text);

    /* Release (free) the native char array */
    (*env)->ReleaseStringChars(env, textString, text);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    showTextNextLine
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_showTextNextLine(JNIEnv *env, jobject obj, jstring textString) {
    jint page;
    const jchar* text;

    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);

    /* Get the text as a native char array */
    text = (*env)->GetStringChars(env, textString, NULL);

    HPDF_Page_ShowTextNextLine((HPDF_Page) page, (char*) text);

    /* Release (free) the native char array */
    (*env)->ReleaseStringChars(env, textString, text);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    showTextNextLineEx
 * Signature: (FFLjava/lang/String;)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_showTextNextLineEx(JNIEnv *env, jobject obj, jfloat wordSpace,
        jfloat charSpace, jstring textString) {
    jint page;
    const jchar* text;

    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);

    /* Get the text as a native char array */
    text = (*env)->GetStringChars(env, textString, NULL);

    HPDF_Page_ShowTextNextLineEx((HPDF_Page) page, (HPDF_REAL) wordSpace, (HPDF_REAL) charSpace,
            (char*) text);

    /* Release (free) the native char array */
    (*env)->ReleaseStringChars(env, textString, text);
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

/*
 * Class:     org_libharu_PdfPage
 * Method:    textOut
 * Signature: (FFLjava/lang/String;)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_textOut(JNIEnv *env, jobject obj, jfloat x, jfloat y, jstring textString) {
    jint page;
    const jchar* text;

    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);

    /* Get the text as a native char array */
    text = (*env)->GetStringChars(env, textString, NULL);

    HPDF_Page_TextOut((HPDF_Page) page, (HPDF_REAL) x, (HPDF_REAL) y, (char*) text);

    /* Release (free) the native char array */
    (*env)->ReleaseStringChars(env, textString, text);
}

/*
 * Class:     org_libharu_PdfPage
 * Method:    textRect
 * Signature: (FFFFLjava/lang/String;I)V
 */
JNIEXPORT void JNICALL
Java_org_libharu_PdfPage_textRect(JNIEnv *env, jobject obj, jfloat l, jfloat t, jfloat r, jfloat b,
        jstring textString, jint align) {
    jint page;
    const jchar* text;

    /* Get mHPDFPagePointer */
    page = (*env)->GetIntField(env, obj, mHPDFPagePointer);

    /* Get the text as a native char array */
    text = (*env)->GetStringChars(env, textString, NULL);

    HPDF_Page_TextRect((HPDF_Page) page, (HPDF_REAL) l, (HPDF_REAL) t, (HPDF_REAL) r, (HPDF_REAL) b,
            (char*) text, getHPDFTextAlign(align), NULL);

    /* Release (free) the native char array */
    (*env)->ReleaseStringChars(env, textString, text);
}
