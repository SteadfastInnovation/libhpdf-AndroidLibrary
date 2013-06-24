package com.steadfastinnovation.sample.libhpdf;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;

import org.libharu.PdfDocument;
import org.libharu.PdfPage;
import org.libharu.PdfPage.LineCap;

import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.Bitmap.Config;
import android.graphics.Color;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Toast;

public class MainActivity extends Activity {
    private static final String TAG = MainActivity.class.getSimpleName();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        findViewById(R.id.generatePdfButton).setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                // PDF generation should be done on a background thread in a real application
                createPdf();
            }
        });
    }

    private void createPdf() {
        PdfDocument doc = PdfDocument.createPdf();
        if (doc == null) {
            // Failed to create PDF document
            showToast("Unable to create PDF", Toast.LENGTH_SHORT);
            return;
        }

        // Set compression mode
        doc.setCompressionMode(PdfDocument.HPDF_COMP_ALL);

        // Add a page and set the page size
        PdfPage page = doc.addPage();
        page.setSize(PdfPage.Size.LETTER, PdfPage.Direction.PORTRAIT);
        // You can also use setWidth/setHeight for a custom size

        // PDF origin is bottom left corner, so you need to subtract from the height if drawing from
        // the upper left corner.
        float height = page.getHeight();

        // Draw red line
        setStrokeColor(page, Color.RED);
        page.setLineWidth(5);
        page.moveTo(12, height - 18);
        page.lineTo(86, height - 60);
        page.stroke();

        // Draw blue circle
        setStrokeColor(page, Color.BLUE);
        page.setLineWidth(2);
        page.circle(50, height - 100, 38);
        page.stroke();

        // Draw green arc with round ends
        setStrokeColor(page, Color.GREEN);
        page.setLineCap(LineCap.ROUND_END);
        page.setLineWidth(6);
        page.arc(125, height - 40, 28, 35, 175);
        page.stroke();

        // Draw magenta 50x50 bitmap
        Bitmap bitmap = Bitmap.createBitmap(50, 50, Config.ARGB_8888);
        bitmap.eraseColor(Color.MAGENTA);
        ByteArrayOutputStream outStream = new ByteArrayOutputStream();
        bitmap.compress(CompressFormat.PNG, 100, outStream);
        page.drawPngImage(outStream.toByteArray(), 120, height - 150, bitmap.getWidth(),
            bitmap.getHeight());
        bitmap.recycle();
        try {
            outStream.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        // Save PDF
        File pdfFile = new File(Environment.getExternalStorageDirectory(), "libhpdf-sample.pdf");
        boolean success = doc.saveToFile(pdfFile.getAbsolutePath());

        // Close the PDF to free up native memory
        doc.close();

        if (success) {
            showToast("PDF saved to " + pdfFile.getAbsolutePath(), Toast.LENGTH_LONG);

            // Start activity to display PDF
            Intent intent = new Intent(Intent.ACTION_VIEW);
            intent.setType("application/pdf");
            intent.setData(Uri.fromFile(pdfFile));
            try {
                startActivity(intent);
            } catch (ActivityNotFoundException e) {
                showToast("No activity found to display PDF", Toast.LENGTH_SHORT);
            }
        } else {
            showToast("Failed to save PDF", Toast.LENGTH_SHORT);
        }
    }

    private static void setStrokeColor(PdfPage page, int color) {
        page.setRGBStroke(
            Color.red(color) / (float) 0xff,
            Color.green(color) / (float) 0xff,
            Color.blue(color) / (float) 0xff);
    }

    private void showToast(String text, int duration) {
        Toast.makeText(this, text, duration).show();
    }
}
