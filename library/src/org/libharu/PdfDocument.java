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

package org.libharu;

import java.util.LinkedList;

public class PdfDocument {
    static {
        System.loadLibrary("hpdf");
        initIDs();
    }

    /** PDF Compression Mode Values */

    /** No compression */
    public static final int HPDF_COMP_NONE = 0x0;
    /** Compress the contents stream of the page */
    public static final int HPDF_COMP_TEXT = 0x01;
    /** Compress the streams of the image objects */
    public static final int HPDF_COMP_IMAGE = 0x02;
    /** Compress other stream data (fonts, cmaps, etc). */
    public static final int HPDF_COMP_METADATA = 0x04;
    /**
     * Compress all stream data ({@link #HPDF_COMP_TEXT} | {@link #HPDF_COMP_IMAGE} |
     * {@link #HPDF_COMP_METADATA})
     */
    public static final int HPDF_COMP_ALL = 0x0F;

    /** Whether this document has been closed (native memory has been freed) */
    private boolean mClosed = false;

    /** The pages that make up this document */
    private LinkedList<PdfPage> mPages = new LinkedList<PdfPage>();

    /** Handle to the document. */
    protected int mHPDFDocPointer;

    private PdfDocument() {
    }

    /**
     * Create a new PDF document.
     * <p>
     * NOTE: When finished with the document, you MUST call {@link #close()} to free the document in
     * native memory.
     * 
     * @return The new PDF document on success, <code>null</code> on failure.
     */
    public static PdfDocument createPdf() {
        PdfDocument pdf = new PdfDocument();
        if (pdf.create()) {
            return pdf;
        }
        return null;
    }

    /**
     * Create a new PDF document from an existing document. If the existing document hasn't been
     * destroyed, it is first destroyed and then replaced by a new document.
     * <p>
     * NOTE: When finished with the document, you MUST call {@link #close()} to free the document in
     * native memory.
     * 
     * @param pdf The document to recycle and create the new document from.
     * @return True on success, otherwise false.
     */
    public static boolean createPdf(PdfDocument pdf) {
        if (create(pdf.mHPDFDocPointer)) {
            pdf.mClosed = false;
            return true;
        }
        return false;
    }

    /**
     * Close the document. This frees all native memory. Once this has been called, the document is
     * no longer valid and should not be accessed. TODO: Can a document be used again after freeing?
     */
    public void close() {
        if (!mClosed) {
            free();
            mClosed = true;
        }
    }

    @Override
    protected void finalize() throws Throwable {
        close();
        super.finalize();
    }

    /**
     * Create a new page and add it after the last page of the document.
     * 
     * @return The new page that was added.
     */
    public PdfPage addPage() {
        PdfPage page = new PdfPage(this);
        mPages.add(page);
        return page;
    }

    /**
     * Create a new page and insert it at the page number provided.
     * 
     * @param pageNum The page number indicating where to insert the new page.
     * @return The new page that was inserted.
     */
    public PdfPage insertPage(int pageNum) {
        PdfPage target = mPages.get(pageNum);
        PdfPage page = new PdfPage(this, target);
        mPages.add(pageNum, page);
        return page;
    }

    /**
     * Create a new page and insert it just before the provided page.
     * 
     * @param target The page that the new page should be inserted before.
     * @return The new page that was inserted.
     */
    public PdfPage insertPage(PdfPage target) {
        int pageNum = mPages.indexOf(target);
        PdfPage page = new PdfPage(this, target);
        mPages.add(pageNum, page);
        return page;
    }

    /**
     * Cache the field IDs that will be accessed.
     */
    private static native void initIDs();

    /**
     * Create a new document and initialize it.
     * 
     * @return True if created successfully, otherwise false.
     */
    private native boolean create();

    /**
     * Create new document using the provided HPDF_Doc. If HPDF_Doc already has a document, the
     * current document is revoked.
     * 
     * @param pdf The handle of an HPDF_Doc document object.
     * @return True if created successfully, otherwise false.
     */
    private static native boolean create(int pdf);

    /**
     * Revoke the current document object and all resources.
     */
    private native void free();

    /**
     * Revoke the document in the provided HPDF_Doc handle. Keep and recycle loaded resources (such
     * as fonts and encodings) when a new documents requires these resources.
     * 
     * @param pdf The handle of an HPDF_Doc document object.
     */
    private static native void freeDoc(int pdf);

    /**
     * Revoke the document in the provided HPDF_Doc handle along with all the resources.
     * 
     * @param pdf The handle of an HPDF_Doc document object.
     */
    private static native void freeDocAll(int pdf);

    /**
     * Save the current document to a file.
     * 
     * @param filename The name of the file to save.
     * @return True on success, otherwise false.
     */
    public native boolean saveToFile(String filename);

    // saveToStream/getStreamSize/readFromStream/resetStream

    /**
     * Determine if the provided document is valid.
     * 
     * @param pdf The handle of an HPDF_Doc document object.
     * @return True if the provided document is valid, otherwise false.
     */
    private static native boolean hasDoc(int pdf);

    // setErrorHandler/getError/resetError

    // setPagesConfiguration

    // setPageLayout/getPageLayout

    // setPageMode/getPageMode

    // setOpenAction

    // getCurrentPage

    /**
     * Set the mode of compression.
     * 
     * @param mode One or more of the following compression modes or'ed together:
     *            <p>
     *            {@link #HPDF_COMP_NONE}
     *            <p>
     *            {@link #HPDF_COMP_TEXT}
     *            <p>
     *            {@link #HPDF_COMP_IMAGE}
     *            <p>
     *            {@link #HPDF_COMP_METADATA}
     *            <p>
     *            {@link #HPDF_COMP_ALL}
     */
    public native void setCompressionMode(int mode);
}
