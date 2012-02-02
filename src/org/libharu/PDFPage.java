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

public class PDFPage {
    static {
        System.loadLibrary("hpdf");
        initIDs();
    }

    /**
     * The page size.
     */
    public enum Size {
        LETTER, LEGAL, A3, A4, A5, B4, B5, EXECUTIVE, US4X6, US4X8, US5X7, COMM10
    }

    /**
     * The page direction.
     */
    public static enum Direction {
        PORTRAIT, LANDSCAPE
    }

    /**
     * The shape used at the end of lines.
     */
    public static enum LineCap {
        BUTT_END, ROUND_END, PROJECTING_SQUARE_END
    }

    /**
     * The style used to join lines.
     */
    public static enum LineJoin {
        MITER_JOIN, ROUND_JOIN, BEVEL_JOIN
    }

    /** Handle to the page. */
    protected int mHPDFPagePointer;
    /** Handle to the page's parent document. */
    protected int mParentHPDFDocPointer;

    /**
     * Create a new PDF page at the end of the provided PDF document.
     * 
     * @param pdf The PDF document to add a page to.
     */
    protected PDFPage(PDFDocument pdf) {
        create(pdf.mHPDFDocPointer);
    }

    /**
     * Create a new PDF page in the provided PDF document and insert it just before the provided
     * target page.
     * 
     * @param pdf The PDF document to add a page to.
     * @param target The page to insert the new page before.
     */
    protected PDFPage(PDFDocument pdf, PDFPage target) {
        insertPage(pdf.mHPDFDocPointer, target.mHPDFPagePointer);
    }

    /**
     * Change the size and direction of the page to a predefined size.
     * 
     * @param size A predefined page size value.
     * @param direction The direction of the page.
     */
    public void setSize(Size size, Direction direction) {
        setSize(size.ordinal(), direction.ordinal());
    }

    /**
     * Set the shape to be used at the ends of lines.
     * 
     * @param lineCap The line cap style.
     */
    public void setLineCap(LineCap lineCap) {
        setLineCap(lineCap.ordinal());
    }

    /**
     * Set the line join style. Default is {@link LineJoin#MITER_JOIN}.
     * 
     * @param lineJoin The line join style.
     */
    public void setLineJoin(LineJoin lineJoin) {
        setLineJoin(lineJoin.ordinal());
    }

    /**
     * Cache the field IDs that will be accessed.
     */
    private static native void initIDs();

    /**
     * Creates a new page and adds it after the last page of a document.
     * 
     * @param pdf The handle of an HPDF_Doc document object.
     */
    private native void create(int pdf);

    /**
     * Create a new page and insert it just before the specified page.
     * 
     * @param pdf The handle of an HPDF_Doc document object.
     * @param page The handle of an HPDF_Page page object. Insert new page just before.
     */
    private native void insertPage(int pdf, int page);

    /**
     * Change the size and direction of the page to a predefined size.
     * 
     * @param size A predefined page size value.
     * @param direction The direction of the page.
     */
    private native void setSize(int size, int direction);

    /**
     * Set the width of the page.
     * 
     * @param width The width of the page in pixels. The default dpi is 72. Valid values are between
     *            3 and 14400.
     */
    public native void setWidth(float width);

    /**
     * Set the height of the page.
     * 
     * @param height The height of the page in pixels. The default dpi is 72. Valid values are
     *            between 3 and 14400.
     */
    public native void setHeight(float height);

    /**
     * Get the width of the page.
     * 
     * @return The width of the page, or 0 on failure.
     */
    public native float getWidth();

    /**
     * Get the height of the page.
     * 
     * @return The height of the page, or 0 on failure.
     */
    public native float getHeight();

    /**
     * Get the current line width of the page.
     * 
     * @return The current line width for path painting of the page, otherwise it returns the
     *         default line width.
     */
    public native float getLineWidth();

    // getLineCap/getLineJoin

    // getMiterLimit

    // getDash

    // getFlat

    // Graphics

    /**
     * Set the shape to be used at the ends of lines.
     * 
     * @param lineCap The line cap style.
     */
    private native void setLineCap(int lineCap);

    /**
     * Set the line join style. Default is {@link LineJoin#MITER_JOIN}.
     * 
     * @param lineJoin The line join style.
     */
    private native void setLineJoin(int lineJoin);

    /**
     * Set the line width used to stroke a path.
     * 
     * @param lineWidth The line width to use (default is 1).
     */
    public native void setLineWidth(float lineWidth);

    /**
     * Set the filling color.
     * 
     * @param red The level of red (must be between 0 and 1).
     * @param green The level of green (must be between 0 and 1).
     * @param blue The level of blue (must be between 0 and 1).
     */
    public native void setRGBFill(float red, float green, float blue);

    /**
     * Set the stroking color.
     * 
     * @param red The level of red (must be between 0 and 1).
     * @param green The level of green (must be between 0 and 1).
     * @param blue The level of blue (must be between 0 and 1).
     */
    public native void setRGBStroke(float red, float green, float blue);

    /**
     * Start a new subpath and move the current point for drawing path. Sets the start point for the
     * path to the specified point.
     * 
     * @param x The x coordinate of the start point of the path.
     * @param y The y coordinate of the start point of the path.
     */
    public native void moveTo(float x, float y);

    /**
     * Append a path from the current point to the specified point.
     * 
     * @param x The x coordinate of the end point of the path.
     * @param y The y coordinate of the end point of the path.
     */
    public native void lineTo(float x, float y);

    /**
     * Paints the current path.
     */
    public native void stroke();
}
