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

public class PdfPage {
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
    public enum Direction {
        PORTRAIT, LANDSCAPE
    }

    /**
     * The shape used at the end of lines.
     */
    public enum LineCap {
        BUTT_END, ROUND_END, PROJECTING_SQUARE_END
    }

    /**
     * The style used to join lines.
     */
    public enum LineJoin {
        MITER_JOIN, ROUND_JOIN, BEVEL_JOIN
    }

    /**
     * The text rendering mode.
     */
    public enum TextRenderingMode {
        FILL, STROKE, FILL_THEN_STROKE, INVISIBLE, FILL_CLIPPING, STROKE_CLIPPING, FILL_STROKE_CLIPPING, CLIPPING
    }

    /**
     * The alignment of text.
     */
    public enum TextAlign {
        LEFT, RIGHT, CENTER, JUSTIFY
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
    protected PdfPage(PdfDocument pdf) {
        create(pdf.mHPDFDocPointer);
    }

    /**
     * Create a new PDF page in the provided PDF document and insert it just before the provided
     * target page.
     * 
     * @param pdf The PDF document to add a page to.
     * @param target The page to insert the new page before.
     */
    protected PdfPage(PdfDocument pdf, PdfPage target) {
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
     * Sets the text rendering mode. The initial value of text rendering mode is
     * {@link TextRenderingMode#FILL}.
     * 
     * @param mode The text rendering mode.
     */
    public void setTextRenderingMode(TextRenderingMode mode) {
        setTextRenderingMode(mode.ordinal());
    }

    /**
     * Prints the text inside the specified region.
     * 
     * @param l The left side of the region to output text.
     * @param t The top side of the region to output text.
     * @param r The right side of the region to output text.
     * @param b The bottom side of the region to output text.
     * @param text The text to print.
     * @param align The alignment of the text.
     */
    public void textRect(float l, float t, float r, float b, String text, TextAlign align) {
        textRect(l, t, r, b, text, align.ordinal());
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
     * Appends a circle arc to the current path.
     * 
     * @param x The x coordinate of the center point of the circle.
     * @param y The y coordinate of the center point of the circle.
     * @param radius The radius of the circle.
     * @param startAngle The angle of the beginning of the arc.
     * @param endAngle The angle of the end of the arc. It must be greater than ang1.
     */
    public native void arc(float x, float y, float radius, float startAngle, float endAngle);

    /**
     * Begins a text object and sets the text position to (0, 0).
     */
    public native void beginText();

    /**
     * Appends a circle to the current path.
     * 
     * @param x The x coordinate of the center point of the circle.
     * @param y The y coordinate of the center point of the circle.
     * @param radius The radius of the circle.
     */
    public native void circle(float x, float y, float radius);

    /**
     * Clip draw calls to the current path.
     */
    public native void clip();

    /**
     * Appends a straight line from the current point to the start point of sub path. The current
     * point is moved to the start point of sub path.
     */
    public native void closePath();

    /**
     * Closes the current path. Then, it paints the path.
     */
    public native void closePathStroke();

    /**
     * Closes the current path, fills the current path using the even-odd rule, then paints the
     * path.
     */
    public native void closePathEofillStroke();

    /**
     * Closes the current path, fills the current path using the nonzero winding number rule, then
     * paints the path.
     */
    public native void closePathFillStroke();

    /**
     * Concatenates the page's current transformation matrix and the specified matrix.
     * 
     * @param a
     * @param b
     * @param c
     * @param d
     * @param x
     * @param y
     */
    public native void concat(float a, float b, float c, float d, float x, float y);

    /**
     * Appends a Bézier curve to the current path using the control points (x1, y1) and (x2, y2) and
     * (x3, y3), then sets the current point to (x3, y3).
     * 
     * @param x1 The x coordinate of the first control point.
     * @param y1 The y coordinate of the first control point.
     * @param x2 The x coordinate of the second control point.
     * @param y2 The y coordinate of the second control point.
     * @param x3 The x coordinate of the third control point.
     * @param y3 The y coordinate of the third control point.
     */
    public native void curveTo(float x1, float y1, float x2, float y2, float x3, float y3);

    /**
     * Appends a Bézier curve to the current path using the current point and (x2, y2) and (x3, y3)
     * as control points. Then, the current point is set to (x3, y3).
     * 
     * @param x2 The x coordinate of the second control point.
     * @param y2 The y coordinate of the second control point.
     * @param x3 The x coordinate of the third control point.
     * @param y3 The y coordinate of the third control point.
     */
    public native void curveTo2(float x2, float y2, float x3, float y3);

    /**
     * Appends a Bézier curve to the current path using two specified points. The point (x1, y1) and
     * the point (x3, y3) are used as the control points for a Bézier curve and current point is
     * moved to the point (x3, y3)
     * 
     * @param x1 The x coordinate of the first control point.
     * @param y1 The y coordinate of the first control point.
     * @param x3 The x coordinate of the third control point.
     * @param y3 The y coordinate of the third control point.
     */
    public native void curveTo3(float x1, float y1, float x3, float y3);

    /**
     * Draws a JPEG image.
     * 
     * @param image The array containing the JPEG image data.
     * @param x The x coordinate of the lower-left point of the region where image is displayed.
     * @param y The y coordinate of the lower-left point of the region where image is displayed.
     * @param width The width of the region where image is displayed.
     * @param height The height of the region where image is displayed.
     */
    public native void drawJpegImage(byte[] image, float x, float y, float width, float height);

    /**
     * Draws a JPEG image.
     * 
     * @param path The file path to a JPEG image.
     * @param x The x coordinate of the lower-left point of the region where image is displayed.
     * @param y The y coordinate of the lower-left point of the region where image is displayed.
     * @param width The width of the region where image is displayed.
     * @param height The height of the region where image is displayed.
     */
    public native void drawJpegImageFromFile(String path, float x, float y, float width,
        float height);

    /**
     * Draws a PNG image.
     * 
     * @param image The array containing the PNG image data.
     * @param x The x coordinate of the lower-left point of the region where image is displayed.
     * @param y The y coordinate of the lower-left point of the region where image is displayed.
     * @param width The width of the region where image is displayed.
     * @param height The height of the region where image is displayed.
     */
    public native void drawPngImage(byte[] image, float x, float y, float width, float height);

    /**
     * Draws a PNG image.
     * 
     * @param path The file path to a PNG image.
     * @param x The x coordinate of the lower-left point of the region where image is displayed.
     * @param y The y coordinate of the lower-left point of the region where image is displayed.
     * @param width The width of the region where image is displayed.
     * @param height The height of the region where image is displayed.
     */
    public native void drawPngImageFromFile(String path, float x, float y, float width, float height);

    /**
     * Appends an ellipse to the current path.
     * 
     * @param x The x coordinate of the center of the ellipse.
     * @param y The y coordinate of the center of the ellipse.
     * @param radiusX The horizontal radius of the ellipse.
     * @param radiusY The vertical radius of the ellipse.
     */
    public native void ellipse(float x, float y, float radiusX, float radiusY);

    /**
     * Ends the path object without filling or painting.
     */
    public native void endPath();

    /**
     * Ends a text object.
     */
    public native void endText();

    /**
     * Clip draw calls to the current path using the even-odd rule.
     */
    public native void eoclip();

    /**
     * Fills the current path using the even-odd rule.
     */
    public native void eofill();

    /**
     * Fills the current path using the even-odd rule, then paints the path.
     */
    public native void eofillStroke();

    //  executeXObject

    /**
     * Fills the current path using the nonzero winding number rule.
     */
    public native void fill();

    /**
     * Fills the current path using the nonzero winding number rule, then paints the path.
     */
    public native void fillStroke();

    /**
     * Restore the graphics state which is saved by {@link #gSave()}.
     */
    public native void gRestore();

    /**
     * Saves the page's current graphics parameter to the stack. An application can invoke
     * {@link #gSave()} up to 28 (???) times and can restore the saved parameter by invoking
     * {@link #gRestore()}.
     * <p>
     * The parameters that are saved by {@link #gSave()} are:
     * <ul>
     * <li>Character Spacing</li>
     * <li>Dash Mode</li>
     * <li>Filling Color</li>
     * <li>Flatness</li>
     * <li>Font</li>
     * <li>Font Size</li>
     * <li>Horizontal Scaling</li>
     * <li>Line Width</li>
     * <li>Line Cap Style</li>
     * <li>Line Join Style</li>
     * <li>Miter Limit</li>
     * <li>Rendering Mode</li>
     * <li>Stroking Color</li>
     * <li>Text Leading</li>
     * <li>Text Rise</li>
     * <li>Transformation Matrix</li>
     * <li>Word Spacing</li>
     * </ul>
     */
    public native void gSave();

    /**
     * Append a path from the current point to the specified point.
     * 
     * @param x The x coordinate of the end point of the path.
     * @param y The y coordinate of the end point of the path.
     */
    public native void lineTo(float x, float y);

    /**
     * Changes the current text position, using the specified offset values. If the current text
     * position is (x1, y1), the new text position will be (x1 + x, y1 + y).
     * 
     * @param x The horizontal offset to the new text position.
     * @param y The vertical offset to the new text position.
     */
    public native void moveTextPos(float x, float y);

    /**
     * Changes the current text position, using the specified offset values. If the current text
     * position is (x1, y1), the new text position will be (x1 + x, y1 + y). Also, the text-leading
     * is set to -y.
     * 
     * @param x The horizontal offset to the new text position.
     * @param y The vertical offset to the new text position.
     */
    public native void moveTextPos2(float x, float y);

    /**
     * Start a new subpath and move the current point for drawing path. Sets the start point for the
     * path to the specified point.
     * 
     * @param x The x coordinate of the start point of the path.
     * @param y The y coordinate of the start point of the path.
     */
    public native void moveTo(float x, float y);

    /**
     * Moves the current text position to the start of the next line.
     */
    public native void moveToNextLine();

    /**
     * Appends a rectangle to the current path.
     * 
     * @param x The x coordinate of the lower-left point of the rectangle.
     * @param y The y coordinate of the lower-left point of the rectangle.
     * @param width The width of the rectangle.
     * @param height The height of the rectangle.
     */
    public native void rectangle(float x, float y, float width, float height);

    /**
     * Sets the character spacing for text.
     * 
     * @param value The character spacing (initial value is 0).
     */
    public native void setCharSpace(float value);

    /**
     * Sets the filling color.
     * 
     * @param c The level of the c color element. Must be between 0 and 1.
     * @param m The level of the m color element. Must be between 0 and 1.
     * @param y The level of the y color element. Must be between 0 and 1.
     * @param k The level of the k color element. Must be between 0 and 1.
     */
    public native void setCMYKFill(float c, float m, float y, float k);

    /**
     * Sets the stroking color.
     * 
     * @param c The level of the c color element. Must be between 0 and 1.
     * @param m The level of the m color element. Must be between 0 and 1.
     * @param y The level of the y color element. Must be between 0 and 1.
     * @param k The level of the k color element. Must be between 0 and 1.
     */
    public native void setCMYKStroke(float c, float m, float y, float k);

    // setDash

    // setExtGState

    // setFontAndSize

    /**
     * Sets the filling color.
     * 
     * @param gray The value of the gray level between 0 and 1.
     */
    public native void setGrayFill(float gray);

    /**
     * Sets the stroking color.
     * 
     * @param gray The value of the gray level between 0 and 1.
     */
    public native void setGrayStroke(float gray);

    /**
     * Sets the horizontal scaling for text showing.
     * 
     * @param value The value of horizontal scaling (initially 100).
     */
    public native void setHorizontalScaling(float value);

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
     * Set the maximum miter length.
     * 
     * @param miterLimit The maximum miter length.
     */
    public native void setMiterLimit(float miterLimit);

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
     * Sets the text leading (line spacing) for text showing.
     * 
     * @param value The value of text leading (initial value is 0).
     */
    public native void setTextLeading(float value);

    // setTextMatrix

    /**
     * Sets the text rendering mode. The initial value of text rendering mode is
     * {@link TextRenderingMode#FILL}.
     * 
     * @param mode The text rendering mode.
     */
    private native void setTextRenderingMode(int mode);

    /**
     * Moves the text position in vertical direction by the amount of value. Useful for making
     * subscripts or superscripts.
     * 
     * @param value Text rise, in user space units.
     */
    public native void setTextRise(float value);

    /**
     * Sets the word spacing for text.
     * 
     * @param value The value of word spacing (initial value is 0).
     */
    public native void setTextWordSpace(float value);

    /**
     * Prints the text at the current position on the page.
     * 
     * @param text The text to print.
     */
    public native void showText(String text);

    /**
     * Moves the current text position to the start of the next line, then prints the text at the
     * current position on the page.
     * 
     * @param text The text to print.
     */
    public native void showTextNextLine(String text);

    /**
     * Moves the current text position to the start of the next line; then sets word spacing and
     * character spacing; finally prints the text at the current position on the page.
     * 
     * @param wordSpace The word spacing.
     * @param charSpace The character spacing.
     * @param text The text to print.
     */
    public native void showTextNextLineEx(float wordSpace, float charSpace, String text);

    /**
     * Paints the current path.
     */
    public native void stroke();

    /**
     * Prints the text at the specified position.
     * 
     * @param x The x coordinate of the point position where the text is displayed.
     * @param y The y coordinate of the point position where the text is displayed.
     * @param text The text to print.
     */
    public native void textOut(float x, float y, String text);

    /**
     * Prints the text inside the specified region.
     * 
     * @param l The left side of the region to output text.
     * @param t The top side of the region to output text.
     * @param r The right side of the region to output text.
     * @param b The bottom side of the region to output text.
     * @param text The text to print.
     * @param align The alignment of the text.
     */
    private native void textRect(float l, float t, float r, float b, String text, int align);
}
